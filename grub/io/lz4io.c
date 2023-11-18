/* lz4io.c - decompression support for lzop */
/*
 *  GRUB  --  GRand Unified Bootloader
 *  Copyright (C) 2011  Free Software Foundation, Inc.
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

#include <grub/err.h>
#include <grub/mm.h>
#include <grub/file.h>
#include <grub/fs.h>

#include "../lib/lz4/lz4_decompress.h"

GRUB_MOD_LICENSE("GPLv3+");

#define LZ4_MAGIC "\x04\x22\x4d\x18"

#define LZ4_MAX_BLOCK_SIZE 0x400000 // 4MB

GRUB_PACKED_START
struct lz4_header
{
	grub_uint8_t magic[4]; // 0x184D2204
	grub_uint8_t flg;
	grub_uint8_t bd;
	grub_uint8_t data[13];
};
GRUB_PACKED_END

struct grub_lz4io
{
	grub_file_t file;
	int has_b_indep;
	int has_b_checksum;
	int has_c_size;
	int has_c_checksum;
	int has_dict_id;
	grub_size_t header_size;
	grub_uint64_t content_size;
	grub_uint64_t dict_id;
	grub_uint64_t max_block_size;
	grub_off_t saved_off;
	grub_off_t start_block_off;
	char* ubuf;
	grub_size_t u_size;
	char* cbuf;
	grub_size_t c_size;
};

typedef struct grub_lz4io* grub_lz4io_t;
static struct grub_fs grub_lz4io_fs;

static int
read_block(grub_lz4io_t lz4io)
{
	grub_ssize_t sz;
	grub_uint32_t c_size;
	int no_compress = 0;

	lz4io->start_block_off = grub_file_tell(lz4io->file);
	sz = grub_file_read(lz4io->file, &c_size, sizeof(c_size));
	if (sz != sizeof(c_size))
		return -1;
	c_size = grub_le_to_cpu32(c_size);
	if (c_size == 0)
		return 0;
	if (c_size & (1U << 31))
	{
		c_size &= ~(1U << 31);
		no_compress = 1;
	}
	if (c_size > lz4io->max_block_size)
		return -1;
	lz4io->c_size = c_size;
	sz = grub_file_read(lz4io->file, lz4io->cbuf, c_size);
	if (sz != c_size)
		return -1;
	if (no_compress)
	{
		grub_memcpy(lz4io->ubuf, lz4io->cbuf, c_size);
		lz4io->u_size = c_size;
	}
	else
	{
		sz = LZ4_decompress_safe(lz4io->cbuf, lz4io->ubuf, c_size, lz4io->max_block_size);
		if (sz <= 0)
			return -1;
		lz4io->u_size = sz;
	}
	if (lz4io->has_b_checksum)
	{
		grub_uint32_t b_checksum;
		grub_file_read(lz4io->file, &b_checksum, sizeof(grub_uint32_t));
		// TODO: verify block checksum
	}
	lz4io->start_block_off = grub_file_tell(lz4io->file);
	lz4io->saved_off += lz4io->u_size;

	return 1;
}

static int
test_header(grub_file_t file)
{
	grub_lz4io_t lz4io = file->data;
	struct lz4_header header;

	if (grub_file_read(lz4io->file, &header, sizeof(header)) != sizeof(header))
		return 0;

	if (grub_memcmp(header.magic, LZ4_MAGIC, sizeof(header.magic)) != 0)
		return 0;

	if ((header.flg & 0xC0) != 0x40)
		return 0;

	if (header.bd & 0x8F)
		return 0;
	lz4io->max_block_size = 1 << (header.bd / 8 + 8);
	if (lz4io->max_block_size > LZ4_MAX_BLOCK_SIZE)
		return 0;

	lz4io->header_size = 7;

	if (header.flg & 0x20)
		lz4io->has_b_indep = 1;

	if (header.flg & 0x10)
		lz4io->has_b_checksum = 1;

	if (header.flg & 0x08)
	{
		lz4io->has_c_size = 1;
		lz4io->header_size += 8;
		grub_memcpy(&lz4io->content_size, header.data + 2, sizeof(grub_uint64_t));
		lz4io->content_size = grub_le_to_cpu64(lz4io->content_size);
	}

	if (header.flg & 0x04)
		lz4io->has_c_checksum = 1;

	if (header.flg & 0x01)
	{
		lz4io->has_dict_id = 1;
		lz4io->header_size += 8;
		grub_memcpy(&lz4io->dict_id,
			header.data + (lz4io->has_c_size ? 10 : 2), sizeof(grub_uint64_t));
		lz4io->dict_id = grub_le_to_cpu64(lz4io->dict_id);
	}

	lz4io->ubuf = grub_malloc(lz4io->max_block_size);
	if (!lz4io->ubuf)
		return 0;
	lz4io->cbuf = grub_malloc(lz4io->max_block_size);
	if (!lz4io->cbuf)
	{
		grub_free(lz4io->ubuf);
		return 0;
	}

	grub_file_seek(lz4io->file, lz4io->header_size);
	lz4io->saved_off = 0;
	if (read_block(lz4io) <= 0)
	{
		grub_free(lz4io->ubuf);
		grub_free(lz4io->cbuf);
		return 0;
	}
	return 1;
}

static grub_file_t
grub_lz4io_open(grub_file_t io, enum grub_file_type type)
{
	grub_file_t file;
	grub_lz4io_t lz4io;

	if (type & GRUB_FILE_TYPE_NO_DECOMPRESS)
		return io;

	file = (grub_file_t)grub_zalloc(sizeof(*file));
	if (!file)
		return 0;

	lz4io = grub_zalloc(sizeof(*lz4io));
	if (!lz4io)
	{
		grub_free(file);
		return 0;
	}

	lz4io->file = io;

	file->disk = io->disk;
	file->data = lz4io;
	file->fs = &grub_lz4io_fs;
	file->size = GRUB_FILE_SIZE_UNKNOWN;
	file->not_easily_seekable = 1;

	if (grub_file_tell(lz4io->file) != 0)
		grub_file_seek(lz4io->file, 0);

	if (!test_header(file))
	{
		grub_errno = GRUB_ERR_NONE;
		grub_file_seek(io, 0);
		grub_free(lz4io);
		grub_free(file);

		return io;
	}

	if (lz4io->has_c_size)
		file->size = lz4io->content_size;

	return file;
}

static grub_ssize_t
grub_lz4io_read(grub_file_t file, char* buf, grub_size_t len)
{
	grub_lz4io_t lz4io = file->data;
	grub_ssize_t ret = 0;
	grub_off_t off = grub_file_tell(file);

	if (off < lz4io->saved_off - lz4io->u_size)
	{
		lz4io->saved_off = 0;
		grub_file_seek(lz4io->file, lz4io->header_size);
		do
		{
			if (read_block(lz4io) <= 0)
				goto CORRUPTED;
		} while (off >= lz4io->saved_off - lz4io->u_size);
	}

	if (off >= lz4io->saved_off - lz4io->u_size
		&& off < lz4io->saved_off)
	{
		grub_size_t off_in_block = off - (lz4io->saved_off - lz4io->u_size);
		grub_size_t to_copy = grub_min(len, lz4io->u_size - off_in_block);
		grub_memcpy(buf, lz4io->ubuf + off_in_block, to_copy);
		ret += to_copy;
		len -= to_copy;
		buf += to_copy;
		off += to_copy;
	}

	if (len == 0)
		return ret;

	while (len)
	{
		int rc = read_block(lz4io);
		if (rc < 0)
			goto CORRUPTED;
		if (rc == 0)
			break;
		grub_size_t to_copy = grub_min(len, lz4io->u_size);
		grub_memcpy(buf, lz4io->ubuf, to_copy);
		ret += to_copy;
		len -= to_copy;
		buf += to_copy;
		off += to_copy;
	}
	
	return ret;

CORRUPTED:
	grub_error(GRUB_ERR_BAD_COMPRESSED_DATA, N_("lz4 file corrupted"));
	return -1;
}

/* Release everything, including the underlying file object.  */
static grub_err_t
grub_lz4io_close(grub_file_t file)
{
	grub_lz4io_t lz4io = file->data;

	grub_file_close(lz4io->file);

	grub_free(lz4io->ubuf);
	grub_free(lz4io->cbuf);
	grub_free(lz4io);

	/* Device must not be closed twice.  */
	file->disk = 0;
	file->name = 0;
	return grub_errno;
}

static struct grub_fs grub_lz4io_fs =
{
	.name = "lz4io",
	.fs_dir = 0,
	.fs_open = 0,
	.fs_read = grub_lz4io_read,
	.fs_close = grub_lz4io_close,
	.fs_label = 0,
	.next = 0
};

GRUB_MOD_INIT(lz4io)
{
	grub_file_filter_register(GRUB_FILE_FILTER_LZ4IO, grub_lz4io_open);
}

GRUB_MOD_FINI(lz4io)
{
	grub_file_filter_unregister(GRUB_FILE_FILTER_LZ4IO);
}
