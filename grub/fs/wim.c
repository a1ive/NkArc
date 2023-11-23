/*
 *  GRUB  --  GRand Unified Bootloader
 *  Copyright (C) 2023  Free Software Foundation, Inc.
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

#include <grub/types.h>
#include <grub/fs.h>
#include <grub/mm.h>
#include <grub/disk.h>
#include <grub/file.h>
#include <grub/misc.h>
#include <grub/charset.h>
#include <grub/fshelp.h>

#include "../lib/mscompress/mscompress.h"

GRUB_MOD_LICENSE("GPLv3+");

// todo: support big endian

GRUB_PACKED_START
struct wim_resource_header
{
	/* The compressed size of the file and flags. */
#define WIM_RESHDR_ZLEN_MASK      0x00ffffffffffffffULL
#define WIM_RESHDR_FREE           (0x01ULL << 56)
#define WIM_RESHDR_METADATA       (0x02ULL << 56)
#define WIM_RESHDR_COMPRESSED     (0x04ULL << 56)
#define WIM_RESHDR_SPANNED        (0x08ULL << 56)
#define WIM_RESHDR_PACKED_STREAMS (0x10ULL << 56)
	grub_uint64_t zlen__flags;
	/* The location of the resource within the .wim file. */
	grub_uint64_t offset;
	/* The original uncompressed size of the file. */
	/* In a compressed file resource, the chunk table is placed
	 * at the beginning of the resource.
	 * An uncompressed file is divided into 32K block sizes,
	 * compressed and then stored sequentially after the chunk table.
	 * The number of chunk entries will be
	 * (((len + 32768 - 1) / 32768) - 1)
	 * If the original file is >4GB, then the chunk entry is a LARGE_INTEGER;
	 * otherwise it is a DWORD.
	 * Each value is the offset of the next chunk,
	 * so you can compute what the block compressed to.
	 * For example, if the entries are 0x4000, 0x6000, 0x12000,
	 * then you know that block #1 compressed to 0x4000 bytes,
	 * block #2 compressed to 0x2000 bytes,
	 * and block #3 compressed to 0x6000 bytes.
	 * All three should expand to 32768, unless it is the end block,
	 * which will uncompress to whatever is left over.
	 * The block decompressor is determined by the WIM header's flag value. */
	grub_uint64_t len;
};
GRUB_PACKED_END

#define WIM_CHUNK_LEN 32768

GRUB_PACKED_START
struct wim_header
{
	grub_uint8_t signature[8]; // "MSWIM\0\0"
	grub_uint32_t header_len;
	grub_uint32_t version;
	/* Flags */
#define WIM_HDR_RESERVED          0x00000001
#define WIM_HDR_COMPRESS          0x00000002
#define WIM_HDR_READONLY          0x00000004
#define WIM_HDR_SPANNED           0x00000008
#define WIM_HDR_RESOURCE_ONLY     0x00000010
#define WIM_HDR_METADATA_ONLY     0x00000020

#define WIM_HDR_COMPRESS_RESERVED 0x00010000
#define WIM_HDR_COMPRESS_XPRESS   0x00020000
#define WIM_HDR_COMPRESS_LZX      0x00040000
#define WIM_HDR_COMPRESS_LZMS     0x00080000
	grub_uint32_t flags;
	/** Size of the compressed .wim file in bytes. */
	grub_uint32_t chunk_len;
	/* GUID */
	grub_packed_guid_t guid;
	/* The part number of the current .wim file in a spanned set.
	 * This value is 1,
	 * unless the data of the .wim file was split into
	 * multiple parts (.swm). */
	grub_uint16_t part;
	/* The total number of .wim file parts in a spanned set. */
	grub_uint16_t parts;
	/* The number of images contained in the .wim file. */
	grub_uint32_t images;
	/* The location of the resource lookup table. */
	struct wim_resource_header lookup;
	/* The location of the XML data. */
	struct wim_resource_header xml;
	/* The location of the metadata resource. */
	struct wim_resource_header boot;
	/* The index of the bootable image in the .wim file.
	 * If this is zero, then there are no bootable images available. */
	grub_uint32_t boot_index;
	/* The location of integrity table used to verify files. */
	struct wim_resource_header integrity;
	/** Reserved */
	grub_uint8_t reserved[60];
};
GRUB_PACKED_END

GRUB_PACKED_START
struct wim_hash
{
	/** SHA-1 hash */
	grub_uint8_t sha1[20];
};
GRUB_PACKED_END

GRUB_PACKED_START
struct wim_security_header
{
	/* The total length of the table. */
	grub_uint32_t len;
	/* The number of directory entries. */
	grub_uint32_t count;
	/* An array of sizes of the actual security descriptors to follow, in bytes. */
	//grub_uint64_t sizes[0];
};
GRUB_PACKED_END

GRUB_PACKED_START
struct wim_directory_entry
{
	/* Size of directory entry in bytes. */
	grub_uint64_t len;
	/* The file attributes associated with this file. */
#define WIM_ATTR_READONLY            0x00000001
#define WIM_ATTR_HIDDEN              0x00000002
#define WIM_ATTR_SYSTEM              0x00000004
#define WIM_ATTR_DIRECTORY           0x00000010
#define WIM_ATTR_ARCHIVE             0x00000020
#define WIM_ATTR_DEVICE              0x00000040
#define WIM_ATTR_NORMAL              0x00000080
#define WIM_ATTR_TEMPORARY           0x00000100
#define WIM_ATTR_SPARSE_FILE         0x00000200
#define WIM_ATTR_REPARSE_POINT       0x00000400
#define WIM_ATTR_COMPRESSED          0x00000800
#define WIM_ATTR_OFFLINE             0x00001000
#define WIM_ATTR_NOT_CONTENT_INDEXED 0x00002000
#define WIM_ATTR_ENCRYPTED           0x00004000
#define WIM_ATTR_VIRTUAL             0x00010000
	grub_uint32_t attributes;
	/* The index of the node in the security table
	 * that contains this file's security information.
	 * If this field is set to -1, no security information exists
	 * for this file. */
	grub_uint32_t security;
	/* The offset, from the start of the metadata section,
	 * of this directory entry's child files. */
	grub_uint64_t subdir;
	/* Reserved */
	grub_uint8_t reserved1[16];
	/* Creation time */
	grub_uint64_t ctime;
	/* Last access time */
	grub_uint64_t atime;
	/* Last written time */
	grub_uint64_t mtime;
	/* SHA-1 Hash */
	struct wim_hash hash;
	/* Reserved */
	grub_uint8_t reserved2[12];
	/* The number of STREAMENTRY structures
	 * that follow this DIRENTRY structure. */
	grub_uint16_t streams;
	/* The size of the short filename in bytes. */
	grub_uint16_t short_name_len;
	/* The size of the filename in bytes. */
	grub_uint16_t name_len;
	/* The relative name of the file or directory. */
	//grub_uint16_t name[0];
};
GRUB_PACKED_END

GRUB_PACKED_START
struct wim_lookup_entry
{
	/* Resource header */
	struct wim_resource_header resource;
	/* The part number in a spanned set. */
	grub_uint16_t part;
	/* The total number of times this file resource is stored
	 * in all images within a single .wim file */
	grub_uint32_t refcnt;
	/* Hash */
	struct wim_hash hash;
};
GRUB_PACKED_END

struct grub_wim_data
{
	grub_disk_t disk;
	grub_off_t size;
	grub_uint64_t cached_chunk;
	grub_uint64_t cached_res_offset;
	grub_uint8_t chunk_data[WIM_CHUNK_LEN];
	struct wim_header header;
	grub_uint32_t index;
	grub_uint32_t count;
	grub_uint32_t boot;
	struct wim_resource_header res;
	struct wim_resource_header meta[0];
};

struct grub_fshelp_node
{
	struct grub_wim_data* data;
	grub_uint64_t offset;
	grub_uint64_t mtime;
	struct wim_directory_entry direntry;
	struct wim_security_header security;
	struct wim_lookup_entry entry;
};

static char*
get_utf8(grub_uint8_t* in, grub_size_t len)
{
	grub_uint8_t* buf;
	grub_uint16_t* tmp;
	grub_size_t i;

	buf = grub_calloc(len, GRUB_MAX_UTF8_PER_UTF16 + 1);
	tmp = grub_calloc(len, sizeof(tmp[0]));
	if (!buf || !tmp)
	{
		grub_free(buf);
		grub_free(tmp);
		return NULL;
	}
	for (i = 0; i < len; i++)
		tmp[i] = grub_le_to_cpu16(grub_get_unaligned16(in + 2 * i));
	*grub_utf16_to_utf8(buf, tmp, len) = '\0';
	grub_free(tmp);
	return (char*)buf;
}

static int
grub_wim_get_chunk_offset(struct grub_wim_data* data,
	const struct wim_resource_header* res,
	grub_uint64_t chunk, grub_size_t* offset)
{
	grub_size_t zlen = res->zlen__flags & WIM_RESHDR_ZLEN_MASK;
	grub_uint64_t chunks;
	grub_size_t offset_offset;
	grub_size_t offset_len;
	grub_size_t chunks_len;
	union
	{
		grub_uint32_t offset_32;
		grub_uint64_t offset_64;
	} u;

	/* Special case: zero-length files have no chunks */
	if (!res->len)
	{
		*offset = 0;
		return 0;
	}

	/* Calculate chunk parameters */
	chunks = (res->len + WIM_CHUNK_LEN - 1) / WIM_CHUNK_LEN;
	offset_len = res->len > 0xffffffffULL ?
		sizeof(u.offset_64) : sizeof(u.offset_32);
	chunks_len = (chunks - 1) * offset_len;

	/* Sanity check */
	if (chunks_len > zlen)
		return -1;

	/* Special case: chunk 0 has no offset field */
	if (!chunk)
	{
		*offset = chunks_len;
		return 0;
	}

	/* Treat out-of-range chunks as being at the end of the
	 * resource, to allow for length calculation on the final
	 * chunk.
	 */
	if (chunk >= chunks)
	{
		*offset = zlen;
		return 0;
	}

	/* Otherwise, read the chunk offset */
	offset_offset = (chunk - 1) * offset_len;
	if (grub_disk_read(data->disk, 0,
		res->offset + offset_offset, offset_len, &u) != GRUB_ERR_NONE)
		return -1;
	*offset = chunks_len + ((offset_len == sizeof(u.offset_64)) ?
		u.offset_64 : u.offset_32);
	if (*offset > zlen)
		return -1;
	return 0;
}

// Read chunk from a compressed resource
static int
grub_wim_get_chunk(struct grub_wim_data* data,
	struct wim_resource_header* res, grub_uint64_t chunk)
{
	grub_uint64_t chunks;
	grub_size_t offset;
	grub_size_t next_offset;
	grub_size_t len;
	grub_size_t expected_out_len;
	grub_ssize_t out_len;

	/* Get chunk compressed data offset and length */
	if (grub_wim_get_chunk_offset(data, res, chunk, &offset) != 0)
		return -1;
	if (grub_wim_get_chunk_offset(data, res, chunk + 1, &next_offset) != 0)
		return -1;
	len = next_offset - offset;

	/* Calculate uncompressed length */
	chunks = (res->len + WIM_CHUNK_LEN - 1) / WIM_CHUNK_LEN;
	expected_out_len = WIM_CHUNK_LEN;
	if (chunk >= (chunks - 1))
		expected_out_len -= -res->len & (WIM_CHUNK_LEN - 1);

	/* Read possibly-compressed data */
	if (len == expected_out_len)
	{
		/* Chunk did not compress; read raw data */
		if (grub_disk_read(data->disk, 0,
			res->offset + offset, len, data->chunk_data) != GRUB_ERR_NONE)
			return -1;
	}
	else
	{
		/* Read compressed data into a temporary buffer */
		int rc = -1;
		grub_ssize_t(*decompress) (const void* src, grub_size_t len, void* dest);
		grub_uint8_t* zbuf = grub_malloc(len);
		if (!zbuf)
			return -1;
		if (grub_disk_read(data->disk, 0,
			res->offset + offset, len, zbuf) != GRUB_ERR_NONE)
			goto end;
		/* Identify decompressor */
		if (data->header.flags & WIM_HDR_COMPRESS_LZX)
			decompress = grub_lzx_decompress;
		else if (data->header.flags & WIM_HDR_COMPRESS_XPRESS)
			decompress = grub_xca_decompress;
		else
			goto end;
		/* Decompress data */
		out_len = decompress(zbuf, len, NULL);
		if (out_len < 0)
			goto end;
		if ((grub_size_t)out_len != expected_out_len)
			goto end;
		decompress(zbuf, len, data->chunk_data);
		rc = 0;
	end:
		grub_free(zbuf);
		return rc;
	}

	return 0;
}

static int
grub_wim_get_resource(struct grub_wim_data* data,
	struct wim_resource_header* res, void* buf,
	grub_uint64_t offset, grub_size_t len)
{
	grub_uint64_t zlen = (res->zlen__flags & WIM_RESHDR_ZLEN_MASK);

	/* Sanity checks */
	if (offset + len > res->len)
		return -1;
	if (res->offset + zlen > data->size)
		return -1;

	/* If resource is uncompressed, just read the raw data */
	if (!(res->zlen__flags & (WIM_RESHDR_COMPRESSED | WIM_RESHDR_PACKED_STREAMS)))
	{
		if (grub_disk_read(data->disk, 0,
			res->offset + offset, len, buf) != GRUB_ERR_NONE)
			return -1;
		return 0;
	}
	/* Read from each chunk overlapping the target region */
	while (len)
	{
		/* Calculate chunk number */
		grub_size_t skip_len;
		grub_size_t frag_len;
		grub_uint64_t chunk = offset / WIM_CHUNK_LEN;
		
		/* Read chunk, if not already cached */
		if (res->offset != data->cached_res_offset
			|| chunk != data->cached_chunk)
		{
			/* Read chunk */
			if (grub_wim_get_chunk(data, res, chunk) != 0)
				return -1;

			/* Update cache */
			data->cached_res_offset = res->offset;
			data->cached_chunk = chunk;
		}

		/* Copy fragment from this chunk */
		skip_len = offset % WIM_CHUNK_LEN;
		frag_len = WIM_CHUNK_LEN - skip_len;
		if (frag_len > len)
			frag_len = len;
		grub_memcpy(buf, data->chunk_data + skip_len, frag_len);

		/* Move to next chunk */
		buf = (grub_uint8_t*)buf + frag_len;
		offset += frag_len;
		len -= frag_len;
	}

	return 0;
}

static int
grub_wim_get_metadata(struct grub_wim_data *data,
	struct wim_resource_header* meta)
{
	struct wim_lookup_entry entry;
	grub_uint64_t offset;
	grub_uint32_t found = 0;

	/* If no image index is specified, just use the boot metadata */
	if (data->index == 0)
	{
		grub_memcpy(meta, &data->header.boot, sizeof(*meta));
		return 0;
	}

	/* Look for metadata entry */
	for (offset = 0;
		offset + sizeof(entry) <= data->header.lookup.len;
		offset += sizeof(entry))
	{
		/* Read entry */
		if (grub_wim_get_resource(data, &data->header.lookup,
			&entry, offset, sizeof(entry)) != 0)
			return -1;

		/* Look for our target entry */
		if (entry.resource.zlen__flags & WIM_RESHDR_METADATA)
		{
			found++;
			if (found == data->index)
			{
				grub_memcpy(meta, &entry.resource, sizeof(*meta));
				return 0;
			}
		}
	}

	/* Fail if index was not found */
	return -1;
}

static struct grub_wim_data*
grub_wim_mount(grub_disk_t disk)
{
	struct wim_header header;
	struct grub_wim_data* data = NULL;

	if (grub_disk_read(disk, 0, 0, sizeof(struct wim_header), &header))
		goto fail;

	if (grub_memcmp(header.signature, "MSWIM\0\0", 8) != 0)
		goto fail;

	// SWM not supported
	if (header.part != 1 || header.parts != 1)
		goto fail;
	if (header.boot_index > header.images)
		goto fail;

	data = grub_zalloc(sizeof(struct grub_wim_data)
		+ (header.images + 1) * sizeof(struct wim_resource_header));
	if (!data)
		goto fail;

	grub_memcpy(&data->header, &header, sizeof(struct wim_header));
	data->disk = disk;
	data->size = grub_disk_native_sectors(disk) << GRUB_DISK_SECTOR_BITS;
	data->count = header.images + 1;
	data->boot = header.boot_index;
	for (data->index = 0; data->index < data->count; data->index++)
	{
		if (grub_wim_get_metadata(data, &data->meta[data->index]) != 0)
			goto fail;
	}
	return data;
fail:
	grub_free(data);
	grub_error(GRUB_ERR_BAD_FS, "not a wim filesystem");
	return NULL;
}

static char*
grub_wim_read_symlink(grub_fshelp_node_t node)
{
	return NULL;
}

static int
grub_wim_iterate_dir(grub_fshelp_node_t dir,
	grub_fshelp_iterate_dir_hook_t hook, void* hook_data)
{
	struct grub_wim_data* data = dir->data;

	for (; ; dir->offset += dir->direntry.len)
	{
		char* name = NULL;
		grub_uint8_t* buf = NULL;
		struct grub_fshelp_node* node = NULL;
		enum grub_fshelp_filetype filetype = GRUB_FSHELP_UNKNOWN;

		/* Read length field */
		if (grub_wim_get_resource(data, &data->meta[data->index],
			&dir->direntry, dir->offset, sizeof(dir->direntry.len)) != 0)
			return 1;
		
		/* Check for end of this directory */
		if (!dir->direntry.len)
			break;

		/* Read fixed-length portion of directory entry */
		if (grub_wim_get_resource(data, &data->meta[data->index],
			&dir->direntry, dir->offset, sizeof(struct wim_directory_entry)) != 0)
			return 1;
		
		if (dir->direntry.name_len < sizeof(grub_uint16_t))
			continue;

		buf = grub_malloc(dir->direntry.name_len);
		if (!buf)
			return 1;

		/* Read name */
		if (grub_wim_get_resource(data, &data->meta[data->index], buf,
		dir->offset + sizeof(struct wim_directory_entry),
			dir->direntry.name_len) != 0)
			goto end;

		name = get_utf8(buf, dir->direntry.name_len / sizeof(grub_uint16_t));

		if (!name)
			goto end;

		node = grub_malloc(sizeof(*node));
		if (!node)
			goto end;
		node->data = data;
		node->offset = dir->direntry.subdir;
		node->mtime = dir->direntry.mtime;

		grub_memcpy(&node->direntry, &dir->direntry, sizeof(struct wim_directory_entry));
		grub_memcpy(&node->security, &dir->security, sizeof(struct wim_security_header));
		if (dir->direntry.attributes & WIM_ATTR_DIRECTORY)
			filetype = GRUB_FSHELP_DIR;
		else
			filetype = GRUB_FSHELP_REG;
		if (hook(name, filetype, node, hook_data))
			goto end;
		grub_free(name);
		grub_free(buf);
		continue;
	end:
		grub_free(name);
		grub_free(buf);
		return 1;
	}

	return 0;
}

struct grub_wim_dir_ctx
{
	grub_fs_dir_hook_t hook;
	void* hook_data;
};

static int
grub_wim_dir_iter(const char* filename, enum grub_fshelp_filetype filetype,
	grub_fshelp_node_t node, void* data)
{
	struct grub_wim_dir_ctx* ctx = data;
	struct grub_dirhook_info info;

	grub_memset(&info, 0, sizeof(info));

	info.dir = ((filetype & GRUB_FSHELP_TYPE_MASK) == GRUB_FSHELP_DIR);
	info.case_insensitive = 1;
	info.mtimeset = 1;
	info.mtime = grub_divmod64(node->mtime, 10000000, 0)
		- 86400ULL * 365 * (1970 - 1601)
		- 86400ULL * ((1970 - 1601) / 4) + 86400ULL * ((1970 - 1601) / 100);
	grub_free(node);
	return ctx->hook(filename, &info, ctx->hook_data);
}

static grub_err_t
grub_wim_dir_image(struct grub_wim_data* data,
	grub_fs_dir_hook_t hook, void* hook_data)
{
	grub_uint32_t i = 0;
	char name[32];
	struct grub_dirhook_info info = { 0 };
	if (data->header.boot_index == 0)
		i = 1;
	for ( ; i < data->count; i++)
	{
		info.dir = 1;
		info.case_insensitive = 1;
		grub_snprintf(name, sizeof(name), "%u", i);
		if (hook(name, &info, hook_data))
			break;
	}
	return GRUB_ERR_NONE;
}

static int
grub_wim_get_root(struct grub_wim_data* data, grub_fshelp_node_t root)
{
	if (data->index == 0 && data->header.boot_index == 0)
		return -1;
	if (grub_wim_get_resource(data, &data->meta[data->index],
		&root->security, 0, sizeof(struct wim_security_header)) != 0)
		return -1;

	root->offset = (root->security.len + sizeof(grub_uint64_t) - 1) & ~(sizeof(grub_uint64_t) - 1);
	root->data = data;
	if (grub_wim_get_resource(data, &data->meta[data->index],
		&root->direntry, root->offset, sizeof(root->direntry.len)) != 0)
		return -1;
	if (!root->direntry.len)
		return -1;
	if (grub_wim_get_resource(data, &data->meta[data->index],
		&root->direntry, root->offset, sizeof(struct wim_directory_entry)) != 0)
		return -1;
	root->offset = root->direntry.subdir;
	return 0;
}

static grub_err_t
grub_wimfs_dir(grub_disk_t disk, const char* path,
	grub_fs_dir_hook_t hook, void* hook_data)
{
	struct grub_wim_dir_ctx ctx = { hook, hook_data };
	struct grub_wim_data* data = NULL;
	struct grub_fshelp_node* fdiro = NULL;
	struct grub_fshelp_node start;

	data = grub_wim_mount(disk);
	if (!data)
		goto fail;

	while (*path == '/')
		path++;
	if (*path == '\0')
	{
		grub_wim_dir_image(data, hook, hook_data);
		goto fail;
	}

	data->index = grub_strtoul(path, &path, 10);
	grub_wim_get_root(data, &start);

	grub_fshelp_find_file(path, &start, &fdiro, grub_wim_iterate_dir,
		grub_wim_read_symlink, GRUB_FSHELP_DIR);
	if (grub_errno)
		goto fail;

	grub_wim_iterate_dir(fdiro, grub_wim_dir_iter, &ctx);

fail:
	grub_free(data);

	return grub_errno;
}

static grub_err_t
grub_wimfs_open(struct grub_file* file, const char* name)
{
	struct grub_wim_data* data = NULL;
	struct grub_fshelp_node* fdiro = NULL;
	struct grub_fshelp_node start;
	grub_uint64_t offset;

	data = grub_wim_mount(file->disk);
	if (!data)
		goto fail;

	while (*name == '/')
		name++;
	if (*name == '\0')
	{
		grub_error(GRUB_ERR_FILE_NOT_FOUND, "file not found");
		goto fail;
	}

	data->index = grub_strtoul(name, &name, 10);
	grub_wim_get_root(data, &start);

	grub_fshelp_find_file(name, &start, &fdiro, grub_wim_iterate_dir,
		grub_wim_read_symlink, GRUB_FSHELP_REG);
	if (grub_errno)
		goto fail;

	for (offset = 0;
		offset + sizeof(struct wim_lookup_entry) <= data->header.lookup.len;
		offset += sizeof(struct wim_lookup_entry))
	{
		/* Read entry */
		if (grub_wim_get_resource(data, &data->header.lookup, &fdiro->entry,
			offset, sizeof(struct wim_lookup_entry)) != 0)
			goto fail;

		/* Look for our target entry */
		if (grub_memcmp(&fdiro->entry.hash, &fdiro->direntry.hash,
			sizeof(fdiro->entry.hash)) == 0)
		{
			file->size = fdiro->entry.resource.len;
			file->data = fdiro;
			return GRUB_ERR_NONE;
		}
	}

	grub_error(GRUB_ERR_FILE_NOT_FOUND, "file not found");

fail:
	grub_free(data);
	return grub_errno;
}

static grub_err_t
grub_wimfs_close(grub_file_t file)
{
	struct grub_fshelp_node* data = file->data;

	grub_free(data->data);
	grub_free(data);

	return GRUB_ERR_NONE;
}

static grub_ssize_t
grub_wimfs_read(grub_file_t file, char* buf, grub_size_t len)
{
	int rc;
	struct grub_fshelp_node* fdiro = file->data;
	struct grub_wim_data* data = fdiro->data;

	/* XXX: The file is stored in as a single extent.  */
	data->disk->read_hook = file->read_hook;
	data->disk->read_hook_data = file->read_hook_data;
	rc = grub_wim_get_resource(data, &fdiro->entry.resource,
		buf, file->offset, len);
	fdiro->data->disk->read_hook = NULL;

	if (rc)
		return -1;

	return len;
}

static grub_err_t
grub_wimfs_uuid(grub_disk_t disk, char** uuid)
{
	struct wim_header header;
	if (grub_disk_read(disk, 0, 0, sizeof(header), &header))
		*uuid = 0;
	else
	{
		*uuid = grub_xasprintf("%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x",
			grub_le_to_cpu32(header.guid.data1),
			grub_le_to_cpu16(header.guid.data2),
			grub_le_to_cpu16(header.guid.data3),
			header.guid.data4[0], header.guid.data4[1],
			header.guid.data4[2], header.guid.data4[3],
			header.guid.data4[4], header.guid.data4[5],
			header.guid.data4[6], header.guid.data4[7]);
		grub_errno = GRUB_ERR_NONE;
	}
	return grub_errno;
}

static struct grub_fs grub_wim_fs =
{
	.name = "wim",
	.fs_dir = grub_wimfs_dir,
	.fs_open = grub_wimfs_open,
	.fs_read = grub_wimfs_read,
	.fs_close = grub_wimfs_close,
	.fs_uuid = grub_wimfs_uuid,
	.next = 0
};

GRUB_MOD_INIT(wim)
{
	grub_fs_register(&grub_wim_fs);
}

GRUB_MOD_FINI(wim)
{
	grub_fs_unregister(&grub_wim_fs);
}
