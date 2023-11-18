/*
 *  GRUB  --  GRand Unified Bootloader
 *  Copyright (C) 2022  Free Software Foundation, Inc.
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
#include <grub/misc.h>
#include <grub/file.h>
#include <grub/fs.h>

#define ZSTD_STATIC_LINKING_ONLY
#include "../lib/zstd/zstd.h"

GRUB_MOD_LICENSE("GPLv3");

#define ZSBUFSIZ 0x20000

struct grub_zstd
{
	grub_file_t file;
	ZSTD_inBuffer in;
	ZSTD_outBuffer out;
	ZSTD_DStream* zds;
	grub_uint8_t inbuf[ZSBUFSIZ];
	grub_uint8_t outbuf[ZSBUFSIZ];
	grub_off_t saved_offset;
	ZSTD_frameHeader zfh;
};

typedef struct grub_zstd* grub_zstd_t;
static struct grub_fs grub_zstd_fs;

static int
test_header(grub_file_t file)
{
	grub_zstd_t zstd = file->data;
	size_t zsret;

	zstd->in.size = grub_file_read(zstd->file, zstd->inbuf,
		ZSTD_FRAMEHEADERSIZE_MAX);

	if (zstd->in.size != ZSTD_FRAMEHEADERSIZE_MAX)
		return 0;

	if (ZSTD_getFrameHeader(&zstd->zfh, zstd->inbuf, zstd->in.size) != 0)
		return 0;

	if (zstd->zfh.frameContentSize != ZSTD_CONTENTSIZE_UNKNOWN)
		file->size = zstd->zfh.frameContentSize;

	zstd->zds = ZSTD_createDStream();
	if (!zstd->zds)
		return 0;

	zstd->in.src = zstd->inbuf;
	zstd->out.dst = zstd->outbuf;
	zstd->out.size = ZSBUFSIZ;

	zsret = ZSTD_decompressStream(zstd->zds, &zstd->out, &zstd->in);
	if (ZSTD_isError(zsret))
	{
		ZSTD_freeDStream(zstd->zds);
		return 0;
	}

	ZSTD_initDStream(zstd->zds);
	grub_file_seek(zstd->file, 0);

	return 1;
}

static grub_file_t
grub_zstd_open(grub_file_t io, enum grub_file_type type)
{
	grub_file_t file;
	grub_zstd_t zstd;

	if (type & GRUB_FILE_TYPE_NO_DECOMPRESS)
		return io;

	file = (grub_file_t)grub_zalloc(sizeof(*file));
	if (!file)
		return 0;

	zstd = grub_zalloc(sizeof(*zstd));
	if (!zstd)
	{
		grub_free(file);
		return 0;
	}

	zstd->file = io;

	file->disk = io->disk;
	file->data = zstd;
	file->fs = &grub_zstd_fs;
	file->size = GRUB_FILE_SIZE_UNKNOWN;
	file->not_easily_seekable = 1;

	if (grub_file_tell(zstd->file) != 0)
		grub_file_seek(zstd->file, 0);

	/* FIXME: don't test footer on not easily seekable files.  */
	if (!test_header(file))
	{
		grub_errno = GRUB_ERR_NONE;
		grub_file_seek(io, 0);
		grub_free(zstd);
		grub_free(file);
		return io;
	}

	return file;
}

static grub_ssize_t
grub_zstd_read(grub_file_t file, char* buf, grub_size_t len)
{
	grub_ssize_t ret = 0;
	grub_ssize_t readret;
	size_t zsret;
	grub_zstd_t zstd = file->data;
	grub_off_t current_offset;

	if (file->offset < zstd->saved_offset)
	{
		ZSTD_initDStream(zstd->zds);
		zstd->saved_offset = 0;
		zstd->out.pos = 0;
		zstd->in.pos = 0;
		zstd->in.size = 0;
		grub_file_seek(zstd->file, 0);
	}

	current_offset = zstd->saved_offset;

	while (len > 0)
	{
		zstd->out.size = file->offset + ret + len - current_offset;
		if (zstd->out.size > ZSBUFSIZ)
			zstd->out.size = ZSBUFSIZ;
		/* Feed input.  */
		if (zstd->in.pos == zstd->in.size)
		{
			readret = grub_file_read(zstd->file, zstd->inbuf, ZSBUFSIZ);
			if (readret < 0)
				return -1;
			zstd->in.size = readret;
			zstd->in.pos = 0;
		}

		zsret = ZSTD_decompressStream(zstd->zds, &zstd->out, &zstd->in);
		if (ZSTD_isError(zsret))
		{
			grub_error(GRUB_ERR_BAD_COMPRESSED_DATA,
				N_("zst file corrupted"));
			return -1;
		}

		{
			grub_off_t new_offset = current_offset + zstd->out.pos;

			if (file->offset <= new_offset)
				/* Store first chunk of data in buffer.  */
			{
				grub_size_t delta = new_offset - (file->offset + ret);
				grub_memmove(buf, zstd->outbuf + (zstd->out.pos - delta), delta);
				len -= delta;
				buf += delta;
				ret += delta;
			}
			current_offset = new_offset;
		}
		zstd->out.pos = 0;

		if (zsret == 0) /* Stream end, EOF.  */
			break;
	}

	if (ret >= 0)
		zstd->saved_offset = file->offset + ret;

	return ret;
}

/* Release everything, including the underlying file object.  */
static grub_err_t
grub_zstd_close(grub_file_t file)
{
	grub_zstd_t zstd = file->data;
	ZSTD_freeDStream(zstd->zds);
	grub_file_close(zstd->file);
	grub_free(zstd);
	/* Device must not be closed twice.  */
	file->disk = 0;
	file->name = 0;
	return grub_errno;
}

static struct grub_fs grub_zstd_fs =
{
	.name = "zstd",
	.fs_dir = 0,
	.fs_open = 0,
	.fs_read = grub_zstd_read,
	.fs_close = grub_zstd_close,
	.fs_label = 0,
	.next = 0
};

GRUB_MOD_INIT(zstd)
{
	grub_file_filter_register(GRUB_FILE_FILTER_ZSTDIO, grub_zstd_open);
}

GRUB_MOD_FINI(zstd)
{
	grub_file_filter_unregister(GRUB_FILE_FILTER_ZSTDIO);
}
