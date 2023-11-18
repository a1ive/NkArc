/* file.c - file I/O functions */
/*
 *  GRUB  --  GRand Unified Bootloader
 *  Copyright (C) 2002,2006,2007,2009  Free Software Foundation, Inc.
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

#include <grub/misc.h>
#include <grub/err.h>
#include <grub/file.h>
#include <grub/mm.h>
#include <grub/fs.h>
#include <grub/disk.h>

grub_file_filter_t grub_file_filters[GRUB_FILE_FILTER_MAX];

/* Get the disk part of the filename NAME. It is enclosed by parentheses.  */
char*
grub_file_get_disk_name(const char* name)
{
	if (name[0] == '(')
	{
		char* p = grub_strchr(name, ')');
		char* ret;

		if (!p)
		{
			grub_error(GRUB_ERR_BAD_FILENAME, N_("missing `%c' symbol"), ')');
			return 0;
		}

		ret = (char*)grub_malloc(p - name);
		if (!ret)
			return 0;

		grub_memcpy(ret, name + 1, p - name - 1);
		ret[p - name - 1] = '\0';
		return ret;
	}

	return 0;
}

grub_file_t
grub_file_open(const char* name, enum grub_file_type type)
{
	grub_disk_t disk = 0;
	grub_file_t file = 0, last_file = 0;
	char* disk_name;
	const char* file_name;
	grub_file_filter_id_t filter;

	/* Reset grub_errno before we start. */
	grub_errno = GRUB_ERR_NONE;

	disk_name = grub_file_get_disk_name(name);
	if (grub_errno)
		goto fail;

	/* Get the file part of NAME.  */
	file_name = (name[0] == '(') ? grub_strchr(name, ')') : NULL;
	if (file_name)
		file_name++;
	else
		file_name = name;

	disk = grub_disk_open(disk_name);
	grub_free(disk_name);
	disk_name = NULL;
	if (!disk)
		goto fail;

	file = (grub_file_t)grub_zalloc(sizeof(*file));
	if (!file)
		goto fail;

	file->disk = disk;

	/* In case of relative pathnames and non-Unix systems (like Windows)
	 * name of host files may not start with `/'. Blocklists for host files
	 * are meaningless as well (for a start, host disk does not allow any direct
	 * access - it is just a marker). So skip host disk in this case.
	 */
	if (file_name[0] != '/')
		/* This is a block list.  */
		file->fs = &grub_fs_blocklist;
	else
	{
		file->fs = grub_fs_probe(disk);
		if (!file->fs)
			goto fail;
	}

	if ((file->fs->fs_open) (file, file_name) != GRUB_ERR_NONE)
		goto fail;

	file->name = grub_strdup(name);
	grub_errno = GRUB_ERR_NONE;

	for (filter = 0; file && filter < ARRAY_SIZE(grub_file_filters);
		filter++)
		if (grub_file_filters[filter])
		{
			last_file = file;
			file = grub_file_filters[filter](file, type);
			if (file && file != last_file)
			{
				file->name = grub_strdup(name);
				grub_errno = GRUB_ERR_NONE;
			}
		}
	if (!file)
		grub_file_close(last_file);

	return file;

fail:
	grub_free(disk_name);
	if (disk)
		grub_disk_close(disk);

	/* if (net) grub_net_close (net);  */

	grub_free(file);

	return 0;
}

grub_disk_read_hook_t grub_file_progress_hook;

grub_ssize_t
grub_file_read(grub_file_t file, void* buf, grub_size_t len)
{
	grub_ssize_t res;
	grub_disk_read_hook_t read_hook;
	void* read_hook_data;

	if (file->offset > file->size)
	{
		grub_error(GRUB_ERR_OUT_OF_RANGE,
			N_("attempt to read past the end of file"));
		return -1;
	}

	if (len == 0)
		return 0;

	if (len > file->size - file->offset)
		len = file->size - file->offset;

	/* Prevent an overflow.  */
	if ((grub_ssize_t)len < 0)
		len >>= 1;

	if (len == 0)
		return 0;
	read_hook = file->read_hook;
	read_hook_data = file->read_hook_data;
	if (!file->read_hook)
	{
		file->read_hook = grub_file_progress_hook;
		file->read_hook_data = file;
		file->progress_offset = file->offset;
	}
	res = (file->fs->fs_read) (file, buf, len);
	file->read_hook = read_hook;
	file->read_hook_data = read_hook_data;
	if (res > 0)
		file->offset += res;

	return res;
}

grub_err_t
grub_file_close(grub_file_t file)
{
	if (file->fs->fs_close)
		(file->fs->fs_close) (file);

	if (file->disk)
		grub_disk_close(file->disk);
	grub_free(file->name);
	grub_free(file);
	return grub_errno;
}

grub_off_t
grub_file_seek(grub_file_t file, grub_off_t offset)
{
	grub_off_t old;

	if (offset > file->size)
	{
		grub_error(GRUB_ERR_OUT_OF_RANGE,
			N_("attempt to seek outside of the file"));
		return -1;
	}

	old = file->offset;
	file->offset = offset;

	return old;
}
