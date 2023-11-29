/* erofs.c - Enhanced Read-Only File System */
/*
 *  GRUB  --  GRand Unified Bootloader
 *  Copyright (C) 2023 Free Software Foundation, Inc.
 *
 *  GRUB is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  GRUB is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with GRUB.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <grub/disk.h>
#include <grub/err.h>
#include <grub/file.h>
#include <grub/fs.h>
#include <grub/fshelp.h>
#include <grub/misc.h>
#include <grub/mm.h>
#include <grub/safemath.h>
#include <grub/types.h>

#include "../lib/lz4/lz4.h"

GRUB_MOD_LICENSE("GPLv3+");

#define EROFS_SUPER_OFFSET (1024)
#define EROFS_MAGIC 0xE0F5E1E2
#define EROFS_ISLOTBITS (5)
#define EROFS_MAX_BLOCK_SIZE (4096)

#define EROFS_FEATURE_INCOMPAT_ZERO_PADDING 0x00000001
#define EROFS_FEATURE_INCOMPAT_BIG_PCLUSTER 0x00000002
#define EROFS_FEATURE_INCOMPAT_CHUNKED_FILE 0x00000004
#define EROFS_FEATURE_INCOMPAT_ZTAILPACKING 0x00000010
#define EROFS_FEATURE_INCOMPAT_FRAGMENTS 0x00000020
#define EROFS_ALL_FEATURE_INCOMPAT                                             \
  (EROFS_FEATURE_INCOMPAT_ZERO_PADDING | EROFS_FEATURE_INCOMPAT_BIG_PCLUSTER | \
   EROFS_FEATURE_INCOMPAT_CHUNKED_FILE | EROFS_FEATURE_INCOMPAT_ZTAILPACKING | \
   EROFS_FEATURE_INCOMPAT_FRAGMENTS)

GRUB_PACKED_START
struct grub_erofs_super
{
	grub_uint32_t magic;
	grub_uint32_t checksum;
	grub_uint32_t feature_compat;
	grub_uint8_t log2_blksz;
	grub_uint8_t sb_extslots;

	grub_uint16_t root_nid;
	grub_uint64_t inos;

	grub_uint64_t build_time;
	grub_uint32_t build_time_nsec;
	grub_uint32_t blocks;
	grub_uint32_t meta_blkaddr;
	grub_uint32_t xattr_blkaddr;
	grub_uint8_t uuid[16];
	grub_uint8_t volume_name[16];
	grub_uint32_t feature_incompat;

	union
	{
		grub_uint16_t available_compr_algs;
		grub_uint16_t lz4_max_distance;
	} u1;

	grub_uint16_t extra_devices;
	grub_uint16_t devt_slotoff;
	grub_uint8_t log2_dirblksz;
	grub_uint8_t xattr_prefix_count;
	grub_uint32_t xattr_prefix_start;
	grub_uint64_t packed_nid;
	grub_uint8_t reserved2[24];
};
GRUB_PACKED_END

#define EROFS_INODE_LAYOUT_COMPACT 0
#define EROFS_INODE_LAYOUT_EXTENDED 1

enum
{
	EROFS_INODE_FLAT_PLAIN = 0,
	EROFS_INODE_COMPRESSED_FULL = 1,
	EROFS_INODE_FLAT_INLINE = 2,
	EROFS_INODE_COMPRESSED_COMPACT = 3,
	EROFS_INODE_CHUNK_BASED = 4,
	EROFS_INODE_DATALAYOUT_MAX
};

#define EROFS_I_VERSION_MASKS 0x01
#define EROFS_I_DATALAYOUT_MASKS 0x07

#define EROFS_I_VERSION_BIT 0
#define EROFS_I_DATALAYOUT_BIT 1

GRUB_PACKED_START
struct grub_erofs_inode_chunk_info
{
	grub_uint16_t format;
	grub_uint16_t reserved;
};
GRUB_PACKED_END

#define EROFS_CHUNK_FORMAT_BLKBITS_MASK 0x001F
#define EROFS_CHUNK_FORMAT_INDEXES 0x0020

#define EROFS_BLOCK_MAP_ENTRY_SIZE 4

#define EROFS_NULL_ADDR -1

struct grub_erofs_inode_chunk_index
{
	grub_uint16_t advise;
	grub_uint16_t device_id;
	grub_uint32_t blkaddr;
};

union grub_erofs_inode_i_u
{
	grub_uint32_t compressed_blocks;
	grub_uint32_t raw_blkaddr;

	grub_uint32_t rdev;

	struct grub_erofs_inode_chunk_info c;
};

GRUB_PACKED_START
struct grub_erofs_inode_compact
{
	grub_uint16_t i_format;

	grub_uint16_t i_xattr_icount;
	grub_uint16_t i_mode;
	grub_uint16_t i_nlink;
	grub_uint32_t i_size;
	grub_uint32_t i_reserved;

	union grub_erofs_inode_i_u i_u;

	grub_uint32_t i_ino;
	grub_uint16_t i_uid;
	grub_uint16_t i_gid;
	grub_uint32_t i_reserved2;
};
GRUB_PACKED_END

GRUB_PACKED_START
struct grub_erofs_inode_extended
{
	grub_uint16_t i_format;

	grub_uint16_t i_xattr_icount;
	grub_uint16_t i_mode;
	grub_uint16_t i_reserved;
	grub_uint64_t i_size;

	union grub_erofs_inode_i_u i_u;

	grub_uint32_t i_ino;

	grub_uint32_t i_uid;
	grub_uint32_t i_gid;
	grub_uint64_t i_mtime;
	grub_uint32_t i_mtime_nsec;
	grub_uint32_t i_nlink;
	grub_uint8_t i_reserved2[16];
};
GRUB_PACKED_END

enum
{
	EROFS_FT_UNKNOWN,
	EROFS_FT_REG_FILE,
	EROFS_FT_DIR,
	EROFS_FT_CHRDEV,
	EROFS_FT_BLKDEV,
	EROFS_FT_FIFO,
	EROFS_FT_SOCK,
	EROFS_FT_SYMLINK,
	EROFS_FT_MAX
};

#define EROFS_NAME_LEN 255

GRUB_PACKED_START
struct grub_erofs_dirent
{
	grub_uint64_t nid;
	grub_uint16_t nameoff;
	grub_uint8_t file_type;
	grub_uint8_t reserved;
};
GRUB_PACKED_END

#define EROFS_MAP_MAPPED (1 << 1)
#define EROFS_MAP_FULL_MAPPED (1 << 3)
#define EROFS_MAP_FRAGMENT (1 << 4)
#define EROFS_MAP_PARTIAL_REF (1 << 5)

/* Used to map tail extent for tailpacking inline or fragment pcluster */
#define EROFS_ZIP_GET_BLOCKS_FINDTAIL 0x0008

struct grub_erofs_map_blocks
{
	grub_off_t m_pa, m_la;
	grub_off_t m_plen, m_llen;
	grub_uint32_t m_flags;
	grub_uint8_t m_algorithmformat;

	grub_off_t index;
	char mpage[EROFS_MAX_BLOCK_SIZE];
};

enum
{
	EROFS_COMPRESSION_LZ4,
	EROFS_COMPRESSION_MAX
};

enum
{
	EROFS_COMPRESSION_SHIFTED = EROFS_COMPRESSION_MAX,
	EROFS_COMPRESSION_INTERLACED,
	EROFS_COMPRESSION_RUNTIME_MAX
};

struct grub_erofs_zip_maprecorder
{
	struct grub_fshelp_node* node;
	struct grub_erofs_map_blocks* map;

	grub_off_t lcn;
	grub_uint8_t type, headtype;
	grub_uint16_t clusterofs;
	grub_uint16_t delta[2];
	grub_off_t pblk, compressedblks;
	grub_off_t nextpackoff;

	bool partialref;
};

#define EROFS_ZIP_ADVISE_COMPACTED_2B 0x0001
#define EROFS_ZIP_ADVISE_BIG_PCLUSTER_1 0x0002
#define EROFS_ZIP_ADVISE_INLINE_PCLUSTER 0x0008
#define EROFS_ZIP_ADVISE_INTERLACED_PCLUSTER 0x0010
#define EROFS_ZIP_ADVISE_FRAGMENT_PCLUSTER 0x0020

#define EROFS_ZIP_FRAGMENT_INODE_BIT 7

struct grub_erofs_zip_header
{
	union
	{
		grub_uint32_t h_fragmentoff;

		struct
		{
			grub_uint16_t h_reserved1;
			/* indicate the encoded size of tailpacking data */
			grub_uint16_t h_idata_size;
		};
	};

	grub_uint16_t h_advise;
	/*
	   * bit 0-3 : algorithm type of head 1 (logical cluster type 01);
	   * bit 4-7 : algorithm type of head 2 (logical cluster type 11).
	   */
	grub_uint8_t h_algorithmtype;
	/*
	   * bit 0-2 : logical cluster bits - 12, e.g. 0 for 4096;
	   * bit 3-6 : reserved;
	   * bit 7   : move the whole file into packed inode or not.
	   */
	grub_uint8_t h_clusterbits;
};

enum
{
	EROFS_ZIP_LCLUSTER_TYPE_PLAIN = 0,
	EROFS_ZIP_LCLUSTER_TYPE_HEAD1 = 1,
	EROFS_ZIP_LCLUSTER_TYPE_NONHEAD = 2,
	EROFS_ZIP_LCLUSTER_TYPE_HEAD2 = 3,
	EROFS_ZIP_LCLUSTER_TYPE_MAX
};

#define EROFS_ZIP_LI_LCLUSTER_TYPE_MASKS 0x03
#define EROFS_ZIP_LI_LCLUSTER_TYPE_BIT 0

/* (noncompact only, HEAD) This pcluster refers to partial decompressed data */
#define EROFS_ZIP_LI_PARTIAL_REF (1 << 15)

/*
 * D0_CBLKCNT will be marked _only_ at the 1st non-head lcluster to store the
 * compressed block count of a compressed extent (in logical clusters, aka.
 * block count of a pcluster).
 */
#define EROFS_ZIP_LI_D0_CBLKCNT (1 << 11)

struct grub_erofs_zip_lcluster_index
{
	grub_uint16_t di_advise;
	grub_uint16_t di_clusterofs;

	union
	{
		/* for the HEAD lclusters */
		grub_uint32_t blkaddr;
		/* for the NONHEAD lclusters */
		grub_uint16_t delta[2];
	} di_u;
};

#define EROFS_ZIP_FULL_INDEX_ALIGN(end) \
  (ALIGN_UP (end, 8) + sizeof (struct grub_erofs_zip_header) + 8)

struct grub_erofs_zip_decompress_req
{
	struct grub_erofs_data* data;

	char* in, * out;

	grub_uint32_t decodedskip;
	grub_uint32_t inputsize, decodedlength;

	grub_uint32_t interlaced_offset;

	grub_uint8_t alg;
	bool partial_decoding;
};

struct grub_erofs_xattr_ibody_header
{
	grub_uint32_t h_reserved;
	grub_uint8_t h_shared_count;
	grub_uint8_t h_reserved2[7];
	grub_uint32_t h_shared_xattrs[0];
};

struct grub_fshelp_node
{
	struct grub_erofs_data* data;
	struct grub_erofs_inode_extended inode;

	grub_uint64_t ino;
	grub_uint8_t inode_type;
	grub_uint8_t inode_datalayout;

	/* if the inode has been read into memory? */
	bool inode_read;

	grub_uint16_t z_advise;
	grub_uint8_t z_algorithmtype[2];
	grub_uint8_t z_log2_lclustersize;
	grub_uint64_t z_tailextent_headlcn;

	grub_uint32_t z_idataoff;
	grub_uint16_t z_idatasize;

	grub_uint64_t fragment_off;
	grub_uint32_t fragment_size;

	bool z_header_read;
};

struct grub_erofs_data
{
	grub_disk_t disk;
	struct grub_erofs_super sb;

	struct grub_fshelp_node inode;
};

#define erofs_blocksz(data) (1u << data->sb.log2_blksz)

static inline grub_uint64_t
erofs_iloc(grub_fshelp_node_t node)
{
	struct grub_erofs_super* sb = &node->data->sb;

	return (grub_le_to_cpu32(sb->meta_blkaddr) << sb->log2_blksz) +
		(node->ino << EROFS_ISLOTBITS);
}

static grub_err_t
grub_erofs_read_inode(struct grub_erofs_data* data, grub_fshelp_node_t node)
{
	struct grub_erofs_inode_compact* dic;
	grub_err_t err;
	grub_uint64_t addr = erofs_iloc(node);

	dic = (struct grub_erofs_inode_compact*)&node->inode;

	err = grub_disk_read(data->disk, addr >> GRUB_DISK_SECTOR_BITS,
		addr & (GRUB_DISK_SECTOR_SIZE - 1),
		sizeof(struct grub_erofs_inode_compact), dic);
	if (err)
		return err;

	node->inode_type =
		(dic->i_format >> EROFS_I_VERSION_BIT) & EROFS_I_VERSION_MASKS;
	node->inode_datalayout =
		(dic->i_format >> EROFS_I_DATALAYOUT_BIT) & EROFS_I_DATALAYOUT_MASKS;

	switch (node->inode_type)
	{
	case EROFS_INODE_LAYOUT_EXTENDED:
		addr += sizeof(struct grub_erofs_inode_compact);
		err = grub_disk_read(
			data->disk, addr >> GRUB_DISK_SECTOR_BITS,
			addr & (GRUB_DISK_SECTOR_SIZE - 1),
			sizeof(struct grub_erofs_inode_extended) -
			sizeof(struct grub_erofs_inode_compact),
			(char*)dic + sizeof(struct grub_erofs_inode_compact));
		if (err)
			return err;
		break;
	case EROFS_INODE_LAYOUT_COMPACT:
		break;
	default:
		return grub_error(GRUB_ERR_BAD_FS,
			"invalid inode version %u @ inode %" PRIuGRUB_UINT64_T,
			node->inode_type, node->ino);
	}

	node->inode_read = true;

	return 0;
}

static inline grub_uint64_t
erofs_inode_size(grub_fshelp_node_t node)
{
	return node->inode_type == EROFS_INODE_LAYOUT_COMPACT
		? sizeof(struct grub_erofs_inode_compact)
		: sizeof(struct grub_erofs_inode_extended);
}

static inline grub_uint64_t
erofs_inode_file_size(grub_fshelp_node_t node)
{
	struct grub_erofs_inode_compact* dic =
		(struct grub_erofs_inode_compact*)&node->inode;

	return node->inode_type == EROFS_INODE_LAYOUT_COMPACT
		? grub_le_to_cpu32(dic->i_size)
		: grub_le_to_cpu64(node->inode.i_size);
}

static inline grub_uint32_t
erofs_inode_xattr_ibody_size(grub_fshelp_node_t node)
{
	grub_uint16_t cnt = grub_le_to_cpu16(node->inode.i_xattr_icount);

	return cnt ? sizeof(struct grub_erofs_xattr_ibody_header) +
		(cnt - 1) * sizeof(grub_uint32_t)
		: 0;
}

static inline grub_uint64_t
erofs_inode_nblocks(grub_fshelp_node_t node)
{
	return (erofs_inode_file_size(node) + erofs_blocksz(node->data) - 1) >>
		node->data->sb.log2_blksz;
}

static inline grub_uint64_t
erofs_inode_mtime(grub_fshelp_node_t node)
{
	return node->inode_type == EROFS_INODE_LAYOUT_COMPACT
		? grub_le_to_cpu64(node->data->sb.build_time)
		: grub_le_to_cpu64(node->inode.i_mtime);
}

static grub_err_t
grub_erofs_map_blocks_flatmode(grub_fshelp_node_t node,
	struct grub_erofs_map_blocks* map)
{
	grub_off_t nblocks, lastblk, file_size;
	grub_off_t tailendpacking =
		(node->inode_datalayout == EROFS_INODE_FLAT_INLINE) ? 1 : 0;
	grub_uint32_t blocksz = erofs_blocksz(node->data);

	file_size = erofs_inode_file_size(node);
	nblocks = erofs_inode_nblocks(node);
	lastblk = nblocks - tailendpacking;

	map->m_flags = EROFS_MAP_MAPPED;

	if (map->m_la < (lastblk * blocksz))
	{
		map->m_pa =
			grub_le_to_cpu32(node->inode.i_u.raw_blkaddr) * blocksz + map->m_la;
		map->m_plen = lastblk * blocksz - map->m_la;
	}
	else if (tailendpacking)
	{
		map->m_pa = erofs_iloc(node) + erofs_inode_size(node) +
			erofs_inode_xattr_ibody_size(node) + (map->m_la % blocksz);
		map->m_plen = file_size - map->m_la;

		if (((map->m_pa % blocksz) + map->m_plen) > blocksz)
			return grub_error(
				GRUB_ERR_BAD_FS,
				"inline data cross block boundary @ inode %" PRIuGRUB_UINT64_T,
				node->ino);
	}
	else
		return grub_error(GRUB_ERR_BAD_FS,
			"invalid map->m_la=%" PRIuGRUB_UINT64_T
			" @ inode %" PRIuGRUB_UINT64_T,
			map->m_la, node->ino);

	map->m_llen = map->m_plen;
	return GRUB_ERR_NONE;
}

static grub_err_t
grub_erofs_map_blocks_chunkmode(grub_fshelp_node_t node,
	struct grub_erofs_map_blocks* map)
{
	grub_uint16_t chunk_format = grub_le_to_cpu16(node->inode.i_u.c.format);
	grub_off_t unit, pos;
	grub_uint64_t chunknr;
	grub_uint8_t chunkbits;
	grub_err_t err;

	if (chunk_format & EROFS_CHUNK_FORMAT_INDEXES)
		unit = sizeof(struct grub_erofs_inode_chunk_index);
	else
		unit = EROFS_BLOCK_MAP_ENTRY_SIZE;

	chunkbits = node->data->sb.log2_blksz +
		(chunk_format & EROFS_CHUNK_FORMAT_BLKBITS_MASK);

	chunknr = map->m_la >> chunkbits;
	pos = ALIGN_UP(erofs_iloc(node) + erofs_inode_size(node) +
		erofs_inode_xattr_ibody_size(node),
		unit);
	pos += chunknr * unit;

	map->m_la = chunknr << chunkbits;
	map->m_plen = grub_min(1ULL << chunkbits,
		ALIGN_UP(erofs_inode_file_size(node) - map->m_la,
			erofs_blocksz(node->data)));

	if (chunk_format & EROFS_CHUNK_FORMAT_INDEXES)
	{
		struct grub_erofs_inode_chunk_index idx;
		grub_uint32_t blkaddr;

		err = grub_disk_read(node->data->disk, pos >> GRUB_DISK_SECTOR_BITS,
			pos & (GRUB_DISK_SECTOR_SIZE - 1), unit, &idx);
		if (err)
			return err;

		blkaddr = grub_le_to_cpu32(idx.blkaddr);

		if (blkaddr == (grub_uint32_t)EROFS_NULL_ADDR)
		{
			map->m_pa = 0;
			map->m_flags = 0;
		}
		else
		{
			map->m_pa = blkaddr << node->data->sb.log2_blksz;
			map->m_flags = EROFS_MAP_MAPPED;
		}
	}
	else
	{
		grub_uint32_t blkaddr_le, blkaddr;

		err =
			grub_disk_read(node->data->disk, pos >> GRUB_DISK_SECTOR_BITS,
				pos & (GRUB_DISK_SECTOR_SIZE - 1), unit, &blkaddr_le);
		if (err)
			return err;

		blkaddr = grub_le_to_cpu32(blkaddr_le);
		if (blkaddr == (grub_uint32_t)EROFS_NULL_ADDR)
		{
			map->m_pa = 0;
			map->m_flags = 0;
		}
		else
		{
			map->m_pa = blkaddr << node->data->sb.log2_blksz;
			map->m_flags = EROFS_MAP_MAPPED;
		}
	}

	map->m_llen = map->m_plen;
	return GRUB_ERR_NONE;
}

static grub_err_t
grub_erofs_map_blocks(grub_fshelp_node_t node,
	struct grub_erofs_map_blocks* map)
{
	if (map->m_la >= erofs_inode_file_size(node))
	{
		map->m_llen = map->m_plen = 0;
		map->m_pa = 0;
		map->m_flags = 0;
		return GRUB_ERR_NONE;
	}

	if (node->inode_datalayout != EROFS_INODE_CHUNK_BASED)
		return grub_erofs_map_blocks_flatmode(node, map);
	else
		return grub_erofs_map_blocks_chunkmode(node, map);
}

static grub_err_t
grub_erofs_read_raw_data(grub_fshelp_node_t node, char* buf, grub_off_t size,
	grub_off_t offset, grub_off_t* bytes)
{
	struct grub_erofs_map_blocks map = { .index = GRUB_UINT_MAX };
	grub_err_t err;

	if (bytes)
		*bytes = 0;

	grub_memset(&map, 0, sizeof(map));

	grub_off_t cur = offset;
	while (cur < offset + size)
	{
		char* const estart = buf + cur - offset;
		grub_off_t eend, moff = 0;

		map.m_la = cur;
		err = grub_erofs_map_blocks(node, &map);
		if (err)
			return err;

		eend = grub_min(offset + size, map.m_la + map.m_llen);
		if (!(map.m_flags & EROFS_MAP_MAPPED))
		{
			if (!map.m_llen)
			{
				/* reached EOF */
				grub_memset(estart, 0, offset + size - cur);
				cur = offset + size;
				continue;
			}

			/* Hole */
			grub_memset(estart, 0, eend - cur);
			cur = eend;
			if (bytes)
				*bytes += eend - cur;
			continue;
		}

		if (cur > map.m_la)
		{
			moff = cur - map.m_la;
			map.m_la = cur;
		}

		err = grub_disk_read(node->data->disk,
			(map.m_pa + moff) >> GRUB_DISK_SECTOR_BITS,
			(map.m_pa + moff) & (GRUB_DISK_SECTOR_SIZE - 1),
			eend - map.m_la, estart);
		if (err)
			return err;

		if (bytes)
			*bytes += eend - map.m_la;

		cur = eend;
	}

	return GRUB_ERR_NONE;
}

static grub_err_t
grub_erofs_zip_do_map_blocks(grub_fshelp_node_t node,
	struct grub_erofs_map_blocks* map, int flags);

static grub_err_t
grub_erofs_zip_read_header(grub_fshelp_node_t node)
{
	grub_off_t pos;
	struct grub_erofs_zip_header h;
	grub_err_t err = GRUB_ERR_NONE;

	if (node->z_header_read)
		return 0;

	pos = ALIGN_UP(erofs_iloc(node) + erofs_inode_size(node) +
		erofs_inode_xattr_ibody_size(node),
		8);
	err = grub_disk_read(node->data->disk, pos >> GRUB_DISK_SECTOR_BITS,
		pos & (GRUB_DISK_SECTOR_SIZE - 1),
		sizeof(struct grub_erofs_zip_header), &h);
	if (err)
		return err;

	/* the whole file is stored in the packed inode */
	if (h.h_clusterbits >> EROFS_ZIP_FRAGMENT_INODE_BIT)
	{
		node->z_advise = EROFS_ZIP_ADVISE_FRAGMENT_PCLUSTER;
		node->fragment_off =
			grub_le_to_cpu64(*((grub_uint64_t*)(void*)&h) ^ (1ULL << 63));
		node->z_tailextent_headlcn = 0;
		goto out;
	}

	node->z_advise = grub_le_to_cpu16(h.h_advise);
	node->z_algorithmtype[0] = h.h_algorithmtype & 0xF;
	node->z_algorithmtype[1] = (h.h_algorithmtype >> 4) & 0xF;

	if (node->z_algorithmtype[0] >= EROFS_COMPRESSION_MAX)
		return grub_error(GRUB_ERR_BAD_FS, "unsupported compression algorithm %u",
			node->z_algorithmtype[0]);

	node->z_log2_lclustersize =
		node->data->sb.log2_blksz + (h.h_clusterbits & 0x7);

	if (node->z_advise & EROFS_ZIP_ADVISE_INLINE_PCLUSTER)
	{
		struct grub_erofs_map_blocks map = { .index = GRUB_UINT_MAX };

		node->z_idatasize = grub_le_to_cpu16(h.h_idata_size);
		err = grub_erofs_zip_do_map_blocks(node, &map,
			EROFS_ZIP_GET_BLOCKS_FINDTAIL);
		if (err)
			return err;
	}

	if (node->z_advise & EROFS_ZIP_ADVISE_FRAGMENT_PCLUSTER &&
		!(h.h_clusterbits >> EROFS_ZIP_FRAGMENT_INODE_BIT))
	{
		struct grub_erofs_map_blocks map = { .index = GRUB_UINT_MAX };

		node->fragment_off = grub_le_to_cpu32(h.h_fragmentoff);
		err = grub_erofs_zip_do_map_blocks(node, &map,
			EROFS_ZIP_GET_BLOCKS_FINDTAIL);
		if (err)
			return err;
	}

out:
	node->z_header_read = true;
	return err;
}

static grub_err_t
grub_erofs_zip_load_cluster_index(struct grub_erofs_zip_maprecorder* m,
	grub_off_t blkno)
{
	struct grub_erofs_map_blocks* map = m->map;
	const struct grub_erofs_data* data = m->node->data;
	grub_off_t addr = blkno << data->sb.log2_blksz;
	grub_err_t err;

	if (map->index == blkno)
		return GRUB_ERR_NONE;

	err = grub_disk_read(data->disk, addr >> GRUB_DISK_SECTOR_BITS,
		addr & (GRUB_DISK_SECTOR_SIZE - 1),
		erofs_blocksz(data), (void*)map->mpage);
	if (err)
		return err;

	map->index = blkno;

	return GRUB_ERR_NONE;
}

static grub_err_t
grub_erofs_zip_load_cluster_full(struct grub_erofs_zip_maprecorder* m,
	grub_uint64_t lcn)
{
	grub_fshelp_node_t node = m->node;
	grub_off_t pos =
		EROFS_ZIP_FULL_INDEX_ALIGN(erofs_iloc(node) + erofs_inode_size(node) +
			erofs_inode_xattr_ibody_size(node)) +
		lcn * sizeof(struct grub_erofs_zip_lcluster_index);
	struct grub_erofs_zip_lcluster_index* di;
	grub_uint16_t advise, type;
	grub_err_t err;

	err = grub_erofs_zip_load_cluster_index(m, pos >> node->data->sb.log2_blksz);
	if (err)
		return err;

	m->nextpackoff = pos + sizeof(struct grub_erofs_zip_lcluster_index);
	m->lcn = lcn;
	di = (void*)(m->map->mpage + (pos & (erofs_blocksz(node->data) - 1)));

	advise = grub_cpu_to_le16(di->di_advise);
	type = (advise >> EROFS_ZIP_LI_LCLUSTER_TYPE_BIT) &
		EROFS_ZIP_LI_LCLUSTER_TYPE_MASKS;
	switch (type)
	{
	case EROFS_ZIP_LCLUSTER_TYPE_NONHEAD:
		m->clusterofs = 1 << node->z_log2_lclustersize;
		m->delta[0] = grub_cpu_to_le16(di->di_u.delta[0]);
		if (m->delta[0] & EROFS_ZIP_LI_D0_CBLKCNT)
		{
			if (!(advise & EROFS_ZIP_ADVISE_BIG_PCLUSTER_1))
				return grub_error(GRUB_ERR_BAD_FS, "bogus big pcluster");
			m->compressedblks = m->delta[0] & ~EROFS_ZIP_LI_D0_CBLKCNT;
			m->delta[0] = 1;
		}
		m->delta[1] = grub_cpu_to_le16(di->di_u.delta[1]);
		break;
	case EROFS_ZIP_LCLUSTER_TYPE_PLAIN:
	case EROFS_ZIP_LCLUSTER_TYPE_HEAD1:
		if (advise & EROFS_ZIP_LI_PARTIAL_REF)
			m->partialref = true;
		m->clusterofs = grub_cpu_to_le16(di->di_clusterofs);
		m->pblk = grub_cpu_to_le32(di->di_u.blkaddr);
		break;
	default:
		return grub_error(GRUB_ERR_BAD_FS, "unsupported cluster type %u", type);
	}
	m->type = type;
	return GRUB_ERR_NONE;
}

static unsigned int
grub_erofs_zip_decode_compactedbits(unsigned int lobits, unsigned int lomask,
	grub_uint8_t* in, unsigned int pos,
	grub_uint8_t* type)
{
	const unsigned int v =
		grub_le_to_cpu32(grub_get_unaligned32(in + pos / 8)) >> (pos & 7);
	const unsigned int lo = v & lomask;

	*type = (v >> lobits) & 3;
	return lo;
}

static grub_err_t
grub_erofs_zip_unpack_compacted_index(struct grub_erofs_zip_maprecorder* m,
	unsigned int amortizedshift,
	grub_off_t pos)
{
	grub_fshelp_node_t node = m->node;
	const unsigned int lclusterbits = node->z_log2_lclustersize;
	const unsigned int lomask = (1 << lclusterbits) - 1;
	unsigned int vcnt, base, lo, encodebits, nblk, eofs;
	int i;
	grub_uint8_t* in, type;
	bool big_pcluster;

	if (1 << amortizedshift == 4)
		vcnt = 2;
	else if (1 << amortizedshift == 2 && lclusterbits == 12)
		vcnt = 16;
	else
		return GRUB_ERR_BAD_FS;

	m->nextpackoff =
		ALIGN_DOWN(pos, vcnt << amortizedshift) + (vcnt << amortizedshift);
	big_pcluster = node->z_advise & EROFS_ZIP_ADVISE_BIG_PCLUSTER_1;
	encodebits = ((vcnt << amortizedshift) - sizeof(grub_uint32_t)) * 8 / vcnt;
	eofs = pos & (erofs_blocksz(node->data) - 1);
	base = ALIGN_DOWN(eofs, vcnt << amortizedshift);
	in = (void*)(m->map->mpage + base);

	i = (eofs - base) >> amortizedshift;

	lo = grub_erofs_zip_decode_compactedbits(lclusterbits, lomask, in,
		encodebits * i, &type);
	m->type = type;
	if (type == EROFS_ZIP_LCLUSTER_TYPE_NONHEAD)
	{
		m->clusterofs = 1 << lclusterbits;

		if (lo & EROFS_ZIP_LI_D0_CBLKCNT)
		{
			if (!big_pcluster)
				return GRUB_ERR_BAD_FS;
			m->compressedblks = lo & ~EROFS_ZIP_LI_D0_CBLKCNT;
			m->delta[0] = 1;
			return GRUB_ERR_NONE;
		}
		else if (i + 1 != (int)vcnt)
		{
			m->delta[0] = lo;
			return GRUB_ERR_NONE;
		}

		lo = grub_erofs_zip_decode_compactedbits(lclusterbits, lomask, in,
			encodebits * (i - 1), &type);
		if (type != EROFS_ZIP_LCLUSTER_TYPE_NONHEAD)
			lo = 0;
		else if (lo & EROFS_ZIP_LI_D0_CBLKCNT)
			lo = 1;
		m->delta[0] = lo + 1;
		return GRUB_ERR_NONE;
	}

	m->clusterofs = lo;
	m->delta[0] = 0;
	if (!big_pcluster)
	{
		nblk = 1;
		while (i > 0)
		{
			--i;
			lo = grub_erofs_zip_decode_compactedbits(lclusterbits, lomask, in,
				encodebits * i, &type);
			if (type == EROFS_ZIP_LCLUSTER_TYPE_NONHEAD)
				i -= lo;

			if (i >= 0)
				++nblk;
		}
	}
	else
	{
		nblk = 0;
		while (i > 0)
		{
			--i;
			lo = grub_erofs_zip_decode_compactedbits(lclusterbits, lomask, in,
				encodebits * i, &type);
			if (type == EROFS_ZIP_LCLUSTER_TYPE_NONHEAD)
			{
				if (lo & EROFS_ZIP_LI_D0_CBLKCNT)
				{
					--i;
					nblk += lo & ~EROFS_ZIP_LI_D0_CBLKCNT;
					continue;
				}
				if (lo <= 1)
					return GRUB_ERR_BAD_FS;
				i -= lo - 2;
				continue;
			}
			++nblk;
		}
	}
	in += (vcnt << amortizedshift) - sizeof(grub_uint32_t);
	m->pblk = grub_le_to_cpu32(*(grub_uint32_t*)(void*)in) + nblk;
	return 0;
}

static grub_err_t
grub_erofs_zip_load_cluster_compact(struct grub_erofs_zip_maprecorder* m,
	grub_uint64_t lcn)
{
	grub_fshelp_node_t node = m->node;
	const grub_off_t ebase =
		ALIGN_UP(erofs_iloc(node) + erofs_inode_size(node) +
			erofs_inode_xattr_ibody_size(node),
			8) +
		sizeof(struct grub_erofs_zip_header);
	const unsigned int totalidx = erofs_inode_nblocks(node);
	const unsigned int lclusterbits = node->z_log2_lclustersize;
	unsigned int compacted_4b_initial, compacted_2b;
	unsigned int amortizedshift;
	grub_off_t pos;
	grub_err_t err;

	if (lclusterbits != 12 || lcn >= totalidx)
		return GRUB_ERR_BAD_FS;

	m->lcn = lcn;

	compacted_4b_initial = (32 - ebase % 32) / 4;
	if (compacted_4b_initial == 32 / 4)
		compacted_4b_initial = 0;

	if ((node->z_advise & EROFS_ZIP_ADVISE_COMPACTED_2B) &&
		compacted_4b_initial < totalidx)
		compacted_2b = ALIGN_DOWN(totalidx - compacted_4b_initial, 16);
	else
		compacted_2b = 0;

	pos = ebase;
	if (lcn < compacted_4b_initial)
	{
		amortizedshift = 2;
		goto out;
	}
	pos += compacted_4b_initial * 4;
	lcn -= compacted_4b_initial;

	if (lcn < compacted_2b)
	{
		amortizedshift = 1;
		goto out;
	}
	pos += compacted_2b * 2;
	lcn -= compacted_2b;
	amortizedshift = 2;

out:
	pos += lcn * (1 << amortizedshift);
	err = grub_erofs_zip_load_cluster_index(m, pos >> node->data->sb.log2_blksz);
	if (err)
		return err;
	return grub_erofs_zip_unpack_compacted_index(m, amortizedshift, pos);
}

static grub_err_t
grub_erofs_zip_load_cluster(struct grub_erofs_zip_maprecorder* m,
	grub_uint64_t lcn)
{
	grub_uint8_t datalayout = m->node->inode_datalayout;

	if (datalayout == EROFS_INODE_COMPRESSED_FULL)
		return grub_erofs_zip_load_cluster_full(m, lcn);

	if (datalayout == EROFS_INODE_COMPRESSED_COMPACT)
		return grub_erofs_zip_load_cluster_compact(m, lcn);

	return GRUB_ERR_BAD_FS;
}

static grub_err_t
grub_erofs_zip_extent_lookback(struct grub_erofs_zip_maprecorder* m,
	grub_uint64_t lookback_distance)
{
	grub_uint64_t lcn = m->lcn;
	grub_err_t err;

	if (lcn < lookback_distance)
		return grub_error(GRUB_ERR_BAD_FS,
			"bogus lookback distance @ inode %" PRIuGRUB_UINT64_T,
			m->node->ino);

	lcn -= lookback_distance;
	err = grub_erofs_zip_load_cluster(m, lcn);
	if (err)
		return err;

	switch (m->type)
	{
	case EROFS_ZIP_LCLUSTER_TYPE_NONHEAD:
		if (!m->delta[0])
			return grub_error(
				GRUB_ERR_BAD_FS,
				"invalid lookback distance 0 @ inode %" PRIuGRUB_UINT64_T,
				m->node->ino);
		return grub_erofs_zip_extent_lookback(m, m->delta[0]);
	case EROFS_ZIP_LCLUSTER_TYPE_PLAIN:
	case EROFS_ZIP_LCLUSTER_TYPE_HEAD1:
		m->headtype = m->type;
		m->map->m_la = (lcn << m->node->z_log2_lclustersize) | m->clusterofs;
		break;
	default:
		return grub_error(GRUB_ERR_BAD_FS,
			"unknown lcluster type %u @ inode %" PRIuGRUB_UINT64_T,
			m->type, m->node->ino);
	}
	return GRUB_ERR_NONE;
}

static grub_err_t
grub_erofs_zip_get_extent_compressedlen(struct grub_erofs_zip_maprecorder* m)
{
	struct grub_fshelp_node* node = m->node;
	struct grub_erofs_map_blocks* map = m->map;
	grub_uint8_t lclusterbits = node->z_log2_lclustersize;
	grub_uint64_t lcn;
	grub_err_t err;

	if (m->headtype == EROFS_ZIP_LCLUSTER_TYPE_PLAIN ||
		!(node->z_advise & EROFS_ZIP_ADVISE_BIG_PCLUSTER_1))
	{
		map->m_plen = 1 << lclusterbits;
		return GRUB_ERR_NONE;
	}

	lcn = m->lcn + 1;
	if (m->compressedblks)
		goto out;

	err = grub_erofs_zip_load_cluster(m, lcn);
	if (err)
		return err;

	switch (m->type)
	{
	case EROFS_ZIP_LCLUSTER_TYPE_PLAIN:
	case EROFS_ZIP_LCLUSTER_TYPE_HEAD1:
		m->compressedblks = 1 << (lclusterbits - node->data->sb.log2_blksz);
		break;
	case EROFS_ZIP_LCLUSTER_TYPE_NONHEAD:
		if (m->delta[0] != 1)
			return grub_error(GRUB_ERR_BAD_FS,
				"bogus CBLKCNT of lcn %" PRIuGRUB_UINT64_T
				" @ inode %" PRIuGRUB_UINT64_T,
				lcn, node->ino);
		if (m->compressedblks)
			break;
		/* fallthrough */
	default:
		return grub_error(GRUB_ERR_BAD_FS,
			"cannot found CBLKCNT of lcn %" PRIuGRUB_UINT64_T
			" @ inode %" PRIuGRUB_UINT64_T,
			lcn, node->ino);
	}

out:
	map->m_plen = m->compressedblks << lclusterbits;
	return GRUB_ERR_NONE;
}

static grub_err_t
grub_erofs_zip_do_map_blocks(grub_fshelp_node_t node,
	struct grub_erofs_map_blocks* map, int flags)
{
	struct grub_erofs_zip_maprecorder m = {
		.node = node,
		.map = map,
	};
	bool ztailpacking = node->z_advise & EROFS_ZIP_ADVISE_INLINE_PCLUSTER;
	bool fragment = node->z_advise & EROFS_ZIP_ADVISE_FRAGMENT_PCLUSTER;
	grub_uint64_t file_size = erofs_inode_file_size(node);
	grub_uint8_t lclusterbits;
	grub_uint64_t initial_lcn, ofs, end, endoff;
	grub_err_t err;

	lclusterbits = node->z_log2_lclustersize;
	ofs = flags & EROFS_ZIP_GET_BLOCKS_FINDTAIL ? file_size - 1 : map->m_la;
	initial_lcn = ofs >> lclusterbits;
	endoff = ofs & ((1 << lclusterbits) - 1);

	err = grub_erofs_zip_load_cluster(&m, initial_lcn);
	if (err)
		return err;

	if (ztailpacking && (flags & EROFS_ZIP_GET_BLOCKS_FINDTAIL))
		node->z_idataoff = m.nextpackoff;

	map->m_flags = EROFS_MAP_MAPPED;
	end = (m.lcn + 1ULL) << lclusterbits;
	switch (m.type)
	{
	case EROFS_ZIP_LCLUSTER_TYPE_PLAIN:
	case EROFS_ZIP_LCLUSTER_TYPE_HEAD1:
		if (endoff >= m.clusterofs)
		{
			m.headtype = m.type;
			map->m_la = (m.lcn << lclusterbits) | m.clusterofs;

			if (ztailpacking && end > file_size)
				end = file_size;
			break;
		}
		if (!m.lcn)
			return grub_error(
				GRUB_ERR_BAD_FS,
				"invalid logical cluster 0 @ inode %" PRIuGRUB_UINT64_T, node->ino);
		end = (m.lcn << lclusterbits) | m.clusterofs;
		map->m_flags |= EROFS_MAP_FULL_MAPPED;
		m.delta[0] = 1;
		/* fallthrough */
	case EROFS_ZIP_LCLUSTER_TYPE_NONHEAD:
		err = grub_erofs_zip_extent_lookback(&m, m.delta[0]);
		if (err)
			return err;
		break;
	default:
		return grub_error(GRUB_ERR_BAD_FS,
			"unknown lcluster type %u @ inode %" PRIuGRUB_UINT64_T,
			m.type, node->ino);
	}

	if (m.partialref)
		map->m_flags |= EROFS_MAP_PARTIAL_REF;
	map->m_llen = end - map->m_la;

	if (flags & EROFS_ZIP_GET_BLOCKS_FINDTAIL)
	{
		node->z_tailextent_headlcn = m.lcn;
		if (fragment && node->inode_datalayout == EROFS_INODE_COMPRESSED_FULL)
			node->fragment_off |= m.pblk << 32;
	}

	if (ztailpacking && m.lcn == node->z_tailextent_headlcn)
	{
		map->m_pa = node->z_idataoff;
		map->m_plen = node->z_idatasize;
	}
	else if (fragment && m.lcn == node->z_tailextent_headlcn)
		map->m_flags |= EROFS_MAP_FRAGMENT;
	else
	{
		map->m_pa = m.pblk << node->data->sb.log2_blksz;
		err = grub_erofs_zip_get_extent_compressedlen(&m);
		if (err)
			return err;
	}

	if (m.headtype == EROFS_ZIP_LCLUSTER_TYPE_PLAIN)
	{
		if (map->m_llen > map->m_plen)
			return grub_error(GRUB_ERR_BAD_FS,
				"invalid extent length @ inode %" PRIuGRUB_UINT64_T,
				node->ino);

		map->m_algorithmformat =
			(node->z_advise & EROFS_ZIP_ADVISE_INTERLACED_PCLUSTER)
			? EROFS_COMPRESSION_INTERLACED
			: EROFS_COMPRESSION_SHIFTED;
	}
	else
		map->m_algorithmformat = node->z_algorithmtype[0];

	return err;
}

static grub_err_t
grub_erofs_zip_map_blocks_iter(grub_fshelp_node_t node,
	struct grub_erofs_map_blocks* map)
{
	grub_uint64_t file_size = erofs_inode_file_size(node);
	grub_err_t err = GRUB_ERR_NONE;

	if (map->m_la >= file_size)
	{
		map->m_llen = map->m_la + 1 - file_size;
		map->m_la = file_size;
		map->m_flags = 0;
		return GRUB_ERR_NONE;
	}

	err = grub_erofs_zip_read_header(node);
	if (err)
		return err;

	if ((node->z_advise & EROFS_ZIP_ADVISE_FRAGMENT_PCLUSTER) &&
		!node->z_tailextent_headlcn)
	{
		map->m_la = 0;
		map->m_llen = file_size;
		map->m_flags =
			EROFS_MAP_MAPPED | EROFS_MAP_FULL_MAPPED | EROFS_MAP_FRAGMENT;
		return GRUB_ERR_NONE;
	}

	err = grub_erofs_zip_do_map_blocks(node, map, 0);

	return err;
}

static grub_err_t
grub_erofs_zip_decompress_lz4(struct grub_erofs_zip_decompress_req* rq)
{
	int ret = 0;
	char* dest = rq->out, * src = rq->in;
	char* buff = NULL;
	bool support_0padding = false;
	grub_uint32_t inputmargin = 0;
	grub_err_t err = GRUB_ERR_NONE;

	if (grub_le_to_cpu32(rq->data->sb.feature_incompat) &
		EROFS_FEATURE_INCOMPAT_ZERO_PADDING)
	{
		support_0padding = true;

		while (!src[inputmargin & (erofs_blocksz(rq->data) - 1)])
			if (!(++inputmargin & (erofs_blocksz(rq->data) - 1)))
				break;

		if (inputmargin >= rq->inputsize)
			return grub_error(GRUB_ERR_BAD_FS, "invalid lz4 inputmargin %u",
				inputmargin);
	}

	if (rq->decodedskip)
	{
		buff = grub_malloc(rq->decodedlength);
		if (!buff)
			return grub_error(GRUB_ERR_OUT_OF_MEMORY, "out of memory");
		dest = buff;
	}

	if (rq->partial_decoding || !support_0padding)
		ret = LZ4_decompress_safe_partial(src + inputmargin, dest,
			rq->inputsize - inputmargin,
			rq->decodedlength, rq->decodedlength);
	else
		ret = LZ4_decompress_safe(src + inputmargin, dest,
			rq->inputsize - inputmargin, rq->decodedlength);

	if (ret != (int)rq->decodedlength)
	{
		err = grub_error(GRUB_ERR_BAD_FS,
			"lz4 decompress failed: ret=%d, expect=%d", ret,
			(int)rq->decodedlength);
		goto out;
	}

	if (rq->decodedskip)
		grub_memcpy(rq->out, dest + rq->decodedskip,
			rq->decodedlength - rq->decodedskip);

out:
	if (buff)
		grub_free(buff);

	return err;
}

static grub_err_t
grub_erofs_zip_decompress(struct grub_erofs_zip_decompress_req* rq)
{
	if (rq->alg == EROFS_COMPRESSION_SHIFTED)
	{
		if (rq->decodedlength > rq->inputsize)
			return grub_error(GRUB_ERR_BAD_FS, "invalid decompress request");

		grub_memcpy(rq->out, rq->in + rq->decodedskip,
			rq->decodedlength - rq->decodedskip);
		return GRUB_ERR_NONE;
	}

	if (rq->alg == EROFS_COMPRESSION_INTERLACED)
	{
		grub_uint32_t count, rightpart, skip;

		if (rq->inputsize > erofs_blocksz(rq->data) ||
			rq->decodedlength > erofs_blocksz(rq->data))
			return grub_error(GRUB_ERR_BAD_FS, "invalid decompress request");

		count = rq->decodedlength - rq->decodedskip;
		skip = (rq->interlaced_offset + rq->decodedskip) &
			(erofs_blocksz(rq->data) - 1);
		rightpart = grub_min(erofs_blocksz(rq->data) - skip, count);

		grub_memcpy(rq->out, rq->in + skip, rightpart);
		grub_memcpy(rq->out + rightpart, rq->in, count - rightpart);
		return GRUB_ERR_NONE;
	}

	if (rq->alg == EROFS_COMPRESSION_LZ4)
		return grub_erofs_zip_decompress_lz4(rq);

	return grub_error(GRUB_ERR_BAD_FS, "unknown compression alg %u", rq->alg);
}

static grub_err_t
grub_erofs_pread(grub_fshelp_node_t node, char* buf, grub_off_t size,
	grub_off_t offset, grub_off_t* bytes);

static grub_err_t
grub_erofs_zip_read_data(grub_fshelp_node_t node, char* buf, grub_off_t size,
	grub_off_t offset, grub_off_t* bytes)
{
	struct grub_erofs_map_blocks map = { .index = GRUB_UINT_MAX };
	grub_off_t end, length, skip;
	bool trimmed;
	char* raw = NULL;
	unsigned int bufsize = 0;
	struct grub_erofs_zip_decompress_req req;
	grub_err_t err = GRUB_ERR_NONE;

	if (bytes)
		*bytes = 0;

	end = offset + size;
	while (end > offset)
	{
		map.m_la = end - 1;

		err = grub_erofs_zip_map_blocks_iter(node, &map);
		if (err)
			break;

		if (end < map.m_la + map.m_llen)
		{
			length = end - map.m_la;
			trimmed = true;
		}
		else
		{
			/* assert(end > map.m_la + map.m_llen) */
			length = map.m_llen;
			trimmed = false;
		}

		if (map.m_la < offset)
		{
			skip = offset - map.m_la;
			end = offset;
		}
		else
		{
			skip = 0;
			end = map.m_la;
		}

		if (!(map.m_flags & EROFS_MAP_MAPPED))
		{
			grub_memset(buf + end - offset, 0, length);
			end = map.m_la;
			continue;
		}

		if (map.m_plen > bufsize)
		{
			bufsize = map.m_plen;
			raw = grub_realloc(raw, bufsize);
			if (!raw)
			{
				err = grub_error(GRUB_ERR_OUT_OF_MEMORY, "out of memory");
				break;
			}
		}

		if (map.m_flags & EROFS_MAP_FRAGMENT)
		{
			struct grub_fshelp_node packed_node = {
				.data = node->data,
				.ino = grub_le_to_cpu64(node->data->sb.packed_nid),
				.inode_read = false,
				.z_header_read = false,
			};

			err = grub_erofs_read_inode(node->data, &packed_node);
			if (err)
				break;

			err =
				grub_erofs_pread(&packed_node, buf + end - offset, length - skip,
					node->fragment_off + skip, NULL);
			if (err)
				break;
		}
		else
		{
			err = grub_disk_read(
				node->data->disk, map.m_pa >> GRUB_DISK_SECTOR_BITS,
				map.m_pa & (GRUB_DISK_SECTOR_SIZE - 1), map.m_plen, raw);
			if (err)
				break;

			req = (struct grub_erofs_zip_decompress_req){
				.data = node->data,
				.in = raw,
				.out = buf + end - offset,
				.decodedskip = skip,
				.interlaced_offset =
				map.m_algorithmformat == EROFS_COMPRESSION_INTERLACED
					? (map.m_la & (erofs_blocksz(node->data) - 1))
					: 0,
				.inputsize = map.m_plen,
				.decodedlength = length,
				.alg = map.m_algorithmformat,
				.partial_decoding =
				trimmed ? true
					: !(map.m_flags & EROFS_MAP_FULL_MAPPED) ||
					  (map.m_flags & EROFS_MAP_PARTIAL_REF),
			};

			err = grub_erofs_zip_decompress(&req);
			if (err)
				break;
		}

		if (bytes)
			*bytes += length - skip;
	}

	if (raw)
		grub_free(raw);
	return err;
}

static grub_err_t
grub_erofs_pread(grub_fshelp_node_t node, char* buf, grub_off_t size,
	grub_off_t offset, grub_off_t* bytes)
{
	grub_err_t err;
	if (!node->inode_read)
	{
		err = grub_erofs_read_inode(node->data, node);
		if (err)
			return err;
	}

	switch (node->inode_datalayout)
	{
	case EROFS_INODE_FLAT_PLAIN:
	case EROFS_INODE_FLAT_INLINE:
	case EROFS_INODE_CHUNK_BASED:
		return grub_erofs_read_raw_data(node, buf, size, offset, bytes);
	case EROFS_INODE_COMPRESSED_FULL:
	case EROFS_INODE_COMPRESSED_COMPACT:
		return grub_erofs_zip_read_data(node, buf, size, offset, bytes);
	default:
		return grub_error(GRUB_ERR_BAD_FS, "unknown data layout %u",
			node->inode_datalayout);
	}
}

static int
grub_erofs_iterate_dir(grub_fshelp_node_t dir,
	grub_fshelp_iterate_dir_hook_t hook, void* hook_data)
{
	grub_off_t offset = 0, file_size;
	grub_err_t err;
	grub_uint32_t blocksz = erofs_blocksz(dir->data);
	char* buf;

	if (!dir->inode_read)
	{
		err = grub_erofs_read_inode(dir->data, dir);
		if (err)
			return 0;
	}

	file_size = erofs_inode_file_size(dir);
	buf = grub_malloc(blocksz);
	if (!buf)
	{
		grub_error(GRUB_ERR_OUT_OF_MEMORY, "out of memory");
		return 0;
	}

	while (offset < file_size)
	{
		grub_off_t maxsize = grub_min(blocksz, file_size - offset);
		struct grub_erofs_dirent* de = (void*)buf, * end;
		grub_uint16_t nameoff;

		err = grub_erofs_pread(dir, buf, maxsize, offset, NULL);
		if (err)
			goto not_found;

		nameoff = grub_le_to_cpu16(de->nameoff);
		if (nameoff < sizeof(struct grub_erofs_dirent) || nameoff > blocksz)
		{
			grub_error(GRUB_ERR_BAD_FS,
				"invalid de[0].nameoff %u @ inode %" PRIuGRUB_UINT64_T,
				nameoff, dir->ino);
			goto not_found;
		}

		end = (struct grub_erofs_dirent*)((char*)de + nameoff);
		while (de < end)
		{
			struct grub_fshelp_node* fdiro;
			enum grub_fshelp_filetype type;
			char filename[EROFS_NAME_LEN + 1];
			unsigned int de_namelen;
			const char* de_name;

			fdiro = grub_malloc(sizeof(struct grub_fshelp_node));
			if (!fdiro)
			{
				grub_error(GRUB_ERR_OUT_OF_MEMORY, "out of memory");
				goto not_found;
			}

			fdiro->data = dir->data;
			fdiro->ino = grub_le_to_cpu64(de->nid);
			fdiro->inode_read = fdiro->z_header_read = false;

			nameoff = grub_le_to_cpu16(de->nameoff);
			de_name = buf + nameoff;
			if (de + 1 >= end)
				de_namelen = grub_strnlen(de_name, maxsize - nameoff);
			else
				de_namelen = grub_le_to_cpu16(de[1].nameoff) - nameoff;

			grub_memcpy(filename, de_name, de_namelen);
			filename[de_namelen] = '\0';

			switch (grub_le_to_cpu16(de->file_type))
			{
			case EROFS_FT_REG_FILE:
			case EROFS_FT_BLKDEV:
			case EROFS_FT_CHRDEV:
			case EROFS_FT_FIFO:
			case EROFS_FT_SOCK:
				type = GRUB_FSHELP_REG;
				break;
			case EROFS_FT_DIR:
				type = GRUB_FSHELP_DIR;
				break;
			case EROFS_FT_SYMLINK:
				type = GRUB_FSHELP_SYMLINK;
				break;
			case EROFS_FT_UNKNOWN:
			default:
				type = GRUB_FSHELP_UNKNOWN;
				break;
			}

			if (hook(filename, type, fdiro, hook_data))
			{
				grub_free(buf);
				return 1;
			}

			++de;
		}

		offset += maxsize;
	}

not_found:
	grub_free(buf);
	return 0;
}

static char*
grub_erofs_read_symlink(grub_fshelp_node_t node)
{
	char* symlink;
	grub_size_t sz;

	if (!node->inode_read)
	{
		grub_erofs_read_inode(node->data, node);
		if (grub_errno)
			return NULL;
	}

	if (grub_add(erofs_inode_file_size(node), 1, &sz))
	{
		grub_error(GRUB_ERR_OUT_OF_RANGE, N_("overflow is detected"));
		return NULL;
	}

	symlink = grub_malloc(sz);
	if (!symlink)
		return NULL;

	grub_erofs_pread(node, symlink, sz - 1, 0, NULL);
	if (grub_errno)
	{
		grub_free(symlink);
		return NULL;
	}

	symlink[sz - 1] = '\0';
	return symlink;
}

static struct grub_erofs_data*
grub_erofs_mount(grub_disk_t disk, bool read_root)
{
	struct grub_erofs_super sb;
	grub_err_t err;
	struct grub_erofs_data* data;
	grub_uint32_t feature;

	err = grub_disk_read(disk, EROFS_SUPER_OFFSET >> GRUB_DISK_SECTOR_BITS, 0,
		sizeof(sb), &sb);
	if (grub_errno == GRUB_ERR_OUT_OF_RANGE)
		grub_error(GRUB_ERR_BAD_FS, "not a valid erofs filesystem");
	if (err)
		return NULL;
	if (sb.magic != grub_cpu_to_le32_compile_time(EROFS_MAGIC))
	{
		grub_error(GRUB_ERR_BAD_FS, "not a valid erofs filesystem");
		return NULL;
	}

	feature = grub_le_to_cpu32(sb.feature_incompat);
	if (feature & ~EROFS_ALL_FEATURE_INCOMPAT)
	{
		grub_error(GRUB_ERR_BAD_FS, "unsupported features: 0x%x",
			feature & ~EROFS_ALL_FEATURE_INCOMPAT);
		return NULL;
	}

	data = grub_malloc(sizeof(*data));
	if (!data)
	{
		grub_error(GRUB_ERR_OUT_OF_MEMORY, "out of memory");
		return NULL;
	}

	data->disk = disk;
	data->sb = sb;
	data->inode.inode_read = data->inode.z_header_read = false;

	if (read_root)
	{
		data->inode.data = data;
		data->inode.ino = grub_le_to_cpu16(sb.root_nid);
		err = grub_erofs_read_inode(data, &data->inode);
		if (err)
		{
			grub_free(data);
			return NULL;
		}
	}

	return data;
}

/* Context for grub_erofs_dir. */
struct grub_erofs_dir_ctx
{
	grub_fs_dir_hook_t hook;
	void* hook_data;
	struct grub_erofs_data* data;
};

/* Helper for grub_erofs_dir. */
static int
grub_erofs_dir_iter(const char* filename, enum grub_fshelp_filetype filetype,
	grub_fshelp_node_t node, void* data)
{
	struct grub_erofs_dir_ctx* ctx = data;
	struct grub_dirhook_info info;

	grub_memset(&info, 0, sizeof(info));
	if (!node->inode_read)
	{
		grub_erofs_read_inode(ctx->data, node);
		grub_errno = GRUB_ERR_NONE;
	}

	if (node->inode_read)
	{
		info.mtimeset = 1;
		info.mtime = erofs_inode_mtime(node);
	}

	info.dir = ((filetype & GRUB_FSHELP_TYPE_MASK) == GRUB_FSHELP_DIR);
	info.symlink = ((filetype & GRUB_FSHELP_TYPE_MASK) == GRUB_FSHELP_SYMLINK);
	grub_free(node);
	return ctx->hook(filename, &info, ctx->hook_data);
}

static grub_err_t
grub_erofs_dir(grub_disk_t disk, const char* path, grub_fs_dir_hook_t hook,
	void* hook_data)
{
	grub_fshelp_node_t fdiro = NULL;
	struct grub_erofs_dir_ctx ctx =
	{
		.hook = hook,
		.hook_data = hook_data,
	};

	ctx.data = grub_erofs_mount(disk, true);
	if (!ctx.data)
		goto fail;

	grub_fshelp_find_file(path, &ctx.data->inode, &fdiro, grub_erofs_iterate_dir,
		grub_erofs_read_symlink, GRUB_FSHELP_DIR);
	if (grub_errno)
		goto fail;

	grub_erofs_iterate_dir(fdiro, grub_erofs_dir_iter, &ctx);

fail:
	if (fdiro != &ctx.data->inode)
		grub_free(fdiro);
	grub_free(ctx.data);

	return grub_errno;
}

static grub_err_t
grub_erofs_open(grub_file_t file, const char* name)
{
	struct grub_erofs_data* data;
	struct grub_fshelp_node* fdiro = 0;
	grub_err_t err;

	data = grub_erofs_mount(file->disk, true);
	if (!data)
	{
		err = grub_errno;
		goto fail;
	}

	err =
		grub_fshelp_find_file(name, &data->inode, &fdiro, grub_erofs_iterate_dir,
			grub_erofs_read_symlink, GRUB_FSHELP_REG);
	if (err)
		goto fail;

	if (!fdiro->inode_read)
	{
		err = grub_erofs_read_inode(data, fdiro);
		if (err)
			goto fail;
	}

	grub_memcpy(&data->inode, fdiro, sizeof(*fdiro));
	grub_free(fdiro);

	file->data = data;
	file->size = erofs_inode_file_size(&data->inode);

	return GRUB_ERR_NONE;

fail:
	if (fdiro != &data->inode)
		grub_free(fdiro);
	grub_free(data);

	return err;
}

static grub_ssize_t
grub_erofs_read(grub_file_t file, char* buf, grub_size_t len)
{
	struct grub_erofs_data* data = file->data;
	struct grub_fshelp_node* inode = &data->inode;
	grub_off_t off = file->offset, ret = 0;
	grub_off_t file_size;

	if (!inode->inode_read)
	{
		grub_erofs_read_inode(data, inode);
		if (grub_errno)
			goto end;
	}

	file_size = erofs_inode_file_size(inode);

	if (off >= file_size)
		goto end;

	if (off + len > file_size)
		len = file_size - off;

	grub_erofs_pread(inode, buf, len, off, &ret);
	if (grub_errno)
	{
		ret = 0;
		goto end;
	}

end:
	return ret;
}

static grub_err_t
grub_erofs_close(grub_file_t file)
{
	grub_free(file->data);

	return GRUB_ERR_NONE;
}

static grub_err_t
grub_erofs_uuid(grub_disk_t disk, char** uuid)
{
	struct grub_erofs_data* data;

	grub_errno = GRUB_ERR_NONE;
	data = grub_erofs_mount(disk, false);

	if (data)
		*uuid = grub_xasprintf(
			"%02x%02x%02x%02x-%02x%02x-%02x%02x-%02x%02x-%02x%02x%02x%02x%02x%02x",
			data->sb.uuid[0], data->sb.uuid[1], data->sb.uuid[2], data->sb.uuid[3],
			data->sb.uuid[4], data->sb.uuid[5], data->sb.uuid[6], data->sb.uuid[7],
			data->sb.uuid[8], data->sb.uuid[9], data->sb.uuid[10],
			data->sb.uuid[11], data->sb.uuid[12], data->sb.uuid[13],
			data->sb.uuid[14], data->sb.uuid[15]);
	else
		*uuid = NULL;

	grub_free(data);

	return grub_errno;
}

static grub_err_t
grub_erofs_label(grub_disk_t disk, char** label)
{
	struct grub_erofs_data* data;

	grub_errno = GRUB_ERR_NONE;
	data = grub_erofs_mount(disk, false);

	if (data)
		*label = grub_strndup((char*)data->sb.volume_name,
			sizeof(data->sb.volume_name));
	else
		*label = NULL;

	grub_free(data);

	return grub_errno;
}

static grub_err_t
grub_erofs_mtime(grub_disk_t disk, grub_int64_t* tm)
{
	struct grub_erofs_data* data;

	grub_errno = GRUB_ERR_NONE;
	data = grub_erofs_mount(disk, false);

	if (!data)
		*tm = 0;
	else
		*tm = grub_le_to_cpu64(data->sb.build_time);

	grub_free(data);

	return grub_errno;
}

static struct grub_fs grub_erofs_fs =
{
	.name = "erofs",
	.fs_dir = grub_erofs_dir,
	.fs_open = grub_erofs_open,
	.fs_read = grub_erofs_read,
	.fs_close = grub_erofs_close,
	.fs_uuid = grub_erofs_uuid,
	.fs_label = grub_erofs_label,
	.fs_mtime = grub_erofs_mtime,
	.next = 0,
};

GRUB_MOD_INIT(erofs)
{
	grub_fs_register(&grub_erofs_fs);
}

GRUB_MOD_FINI(erofs)
{
	grub_fs_unregister(&grub_erofs_fs);
}
