/*
 * Support functions
 *
 * Copyright (C) 2008-2021, Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <libyal_wrap.h>

#include <grub/file.h>

#include "libcfile_definitions.h"
#include "libcfile_libuna.h"
#include "libcfile_support.h"

 /* Context for test_parse.  */
struct find_file_ctx
{
	int file_exists;
	struct grub_dirhook_info file_info;
	char* filename;
};

/* A hook for iterating directories. */
static int
find_file(const char* cur_filename, const struct grub_dirhook_info* info,
	void* data)
{
	struct find_file_ctx* ctx = data;

	if ((info->case_insensitive ? grub_strcasecmp(cur_filename, ctx->filename)
		: grub_strcmp(cur_filename, ctx->filename)) == 0)
	{
		ctx->file_info = *info;
		ctx->file_exists = 1;
		return 1;
	}
	return 0;
}

static void
get_fileinfo(const char* path, struct find_file_ctx* ctx)
{
	char* pathname;
	char* device_name;
	grub_fs_t fs;
	grub_disk_t dev;

	char* path_copy = grub_strdup(path);

	ctx->file_exists = 0;
	device_name = grub_file_get_disk_name(path_copy);
	dev = grub_disk_open(device_name);
	if (!dev)
	{
		grub_free(device_name);
		grub_free(path_copy);
		return;
	}

	fs = grub_fs_probe(dev);
	if (!fs)
	{
		grub_free(device_name);
		grub_disk_close(dev);
		grub_free(path_copy);
		return;
	}

	pathname = grub_strchr(path_copy, ')');
	if (!pathname)
		pathname = path_copy;
	else
		pathname++;

	/* Remove trailing '/'. */
	while (*pathname && pathname[grub_strlen(pathname) - 1] == '/')
		pathname[grub_strlen(pathname) - 1] = 0;

	/* Split into path_copy and filename. */
	ctx->filename = grub_strrchr(pathname, '/');
	if (!ctx->filename)
	{
		path_copy = grub_strdup("/");
		ctx->filename = pathname;
	}
	else
	{
		ctx->filename++;
		path_copy = grub_strdup(pathname);
		path_copy[ctx->filename - pathname] = 0;
	}

	/* It's the whole device. */
	if (!*pathname)
	{
		ctx->file_exists = 1;
		grub_memset(&ctx->file_info, 0, sizeof(ctx->file_info));
		/* Root is always a directory. */
		ctx->file_info.dir = 1;

		ctx->file_info.mtimeset = 0;
	}
	else
		(fs->fs_dir) (dev, path_copy, find_file, ctx);

	grub_disk_close(dev);
	grub_free(path_copy);
	grub_free(device_name);
}

/* Determines if a file exists
 * This function uses the POSIX stat function or equivalent
 * Returns 1 if the file exists, 0 if not or -1 on error
 */
int libcfile_file_exists(
     const char *filename,
     libcerror_error_t **error )
{
	static char *function = "libcfile_file_exists";
	struct find_file_ctx ctx = { 0 };

	if( filename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filename.",
		 function );

		return( -1 );
	}

	grub_errno = GRUB_ERR_NONE;
	get_fileinfo( filename, &ctx );
	grub_errno = GRUB_ERR_NONE;

	return( ctx.file_exists );
}
