/*
	*  NkArc
	*  Copyright (C) 2023 A1ive
	*
	*  This program is free software: you can redistribute it and/or modify
	*  it under the terms of the GNU General Public License as published by
	*  the Free Software Foundation, either version 3 of the License, or
	*  (at your option) any later version.
	*
	*  This program is distributed in the hope that it will be useful,
	*  but WITHOUT ANY WARRANTY; without even the implied warranty of
	*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	*  GNU General Public License for more details.
	*
	*  You should have received a copy of the GNU General Public License
	*  along with this program.  If not, see <http://www.gnu.org/licenses/>.
	*/

#include <grub/types.h>
#include <grub/misc.h>
#include <grub/disk.h>
#include <grub/fs.h>
#include <grub/file.h>
#include <grub/datetime.h>

#include <nkctx.h>
#include <stdlib.h>
#include <lang.h>

BOOL
nkctx_is_hidden_file(const char* filename)
{
	if (strcmp(filename, ".") == 0)
		return TRUE;
	if (strcmp(filename, "..") == 0)
		return TRUE;
	return FALSE;
}

static int
callback_count_file(const char* filename,
	const struct grub_dirhook_info* info,
	void* data)
{
	if (nkctx_is_hidden_file(filename))
		return 0;
	if (info->dir)
		nk.dir_count++;
	nk.file_count++;
	return 0;
}

static void
check_extension(struct nkctx_file* info)
{
	const char* ext = grub_strrchr(info->name, '.');
	info->type = NKCTX_FILE_FILE;
	info->icon = IDR_PNG_FILE;
	if (!ext)
		return;
	ext++;
	//grub_dprintf("enum", "ext: %s", ext);
	if (grub_strcasecmp(ext, "bmp") == 0 || grub_strcasecmp(ext, "gif") == 0 || grub_strcasecmp(ext, "tiff") == 0
		|| grub_strcasecmp(ext, "jpg") == 0 || grub_strcasecmp(ext, "jpeg") == 0 || grub_strcasecmp(ext, "png") == 0
		|| grub_strcasecmp(ext, "ico") == 0)
	{
		info->type = NKCTX_FILE_IMAGE;
		info->icon = IDR_PNG_IMAGE;
	}
	else if (grub_strcasecmp(ext, "tga") == 0 || grub_strcasecmp(ext, "svg") == 0)
		info->icon = IDR_PNG_IMAGE;
	else if (grub_strcasecmp(ext, "txt") == 0 || grub_strcasecmp(ext, "ini") == 0 || grub_strcasecmp(ext, "inf") == 0
		|| grub_strcasecmp(ext, "log") == 0 || grub_strcasecmp(ext, "bat") == 0 || grub_strcasecmp(ext, "cmd") == 0
		|| grub_strcasecmp(ext, "sh") == 0 || grub_strcasecmp(ext, "ps1") == 0)
	{
		info->type = NKCTX_FILE_TEXT;
		info->icon = IDR_PNG_TXT;
	}
	else if (grub_strcasecmp(ext, "iso") == 0)
		info->icon = IDR_PNG_ISO;
	else if (grub_strcasecmp(ext, "img") == 0)
		info->icon = IDR_PNG_FDD;
	else if (grub_strcasecmp(ext, "vhd") == 0 || grub_strcasecmp(ext, "vhdx") == 0 || grub_strcasecmp(ext, "vmdk") == 0
		|| grub_strcasecmp(ext, "vdi") == 0)
		info->icon = IDR_PNG_HDD;
	else if (grub_strcasecmp(ext, "7z") == 0 || grub_strcasecmp(ext, "zip") == 0 || grub_strcasecmp(ext, "rar") == 0
		|| grub_strcasecmp(ext, "lzma") == 0 || grub_strcasecmp(ext, "xz") == 0 || grub_strcasecmp(ext, "gz") == 0
		|| grub_strcasecmp(ext, "tar") == 0 || grub_strcasecmp(ext, "txz") == 0 || grub_strcasecmp(ext, "tgz") == 0
		|| grub_strcasecmp(ext, "zst") == 0 || grub_strcasecmp(ext, "sqfs") == 0 || grub_strcasecmp(ext, "cpio") == 0)
		info->icon = IDR_PNG_ARC;
	else if (grub_strcasecmp(ext, "doc") == 0 || grub_strcasecmp(ext, "docx") == 0 || grub_strcasecmp(ext, "xls") == 0
		|| grub_strcasecmp(ext, "xlsx") == 0 || grub_strcasecmp(ext, "ppt") == 0 || grub_strcasecmp(ext, "pptx") == 0
		|| grub_strcasecmp(ext, "md") == 0)
		info->icon = IDR_PNG_DOC;
	else if (grub_strcasecmp(ext, "pdf") == 0)
		info->icon = IDR_PNG_PDF;
	else if (grub_strcasecmp(ext, "ttf") == 0)
		info->icon = IDR_PNG_FONT;
	else if (grub_strcasecmp(ext, "bin") == 0 || grub_strcasecmp(ext, "efi") == 0 || grub_strcasecmp(ext, "bak") == 0)
		info->icon = IDR_PNG_BIN;
	else if (grub_strcasecmp(ext, "mp3") == 0 || grub_strcasecmp(ext, "wav") == 0 || grub_strcasecmp(ext, "flac") == 0
		|| grub_strcasecmp(ext, "ogg") == 0 || grub_strcasecmp(ext, "aac") == 0)
		info->icon = IDR_PNG_MUSIC;
	else if (grub_strcasecmp(ext, "mp4") == 0 || grub_strcasecmp(ext, "mkv") == 0 || grub_strcasecmp(ext, "mov") == 0
		|| grub_strcasecmp(ext, "flv") == 0 || grub_strcasecmp(ext, "wmv") == 0 || grub_strcasecmp(ext, "avi") == 0)
		info->icon = IDR_PNG_VIDEO;
	else if (grub_strcasecmp(ext, "lnk") == 0)
		info->icon = IDR_PNG_LINK;
	else if (grub_strcasecmp(ext, "wim") == 0 || grub_strcasecmp(ext, "esd") == 0 || grub_strcasecmp(ext, "swm") == 0
		|| grub_strcasecmp(ext, "exe") == 0 || grub_strcasecmp(ext, "dll") == 0 || grub_strcasecmp(ext, "sys") == 0)
		info->icon = IDR_PNG_WIN;
}

struct
ctx_enum_file
{
	DWORD index;
};

static int
callback_enum_file(const char* filename,
	const struct grub_dirhook_info* info,
	void* data)
{
	if (nkctx_is_hidden_file(filename))
		return 0;

	struct ctx_enum_file* ctx = (struct ctx_enum_file*)data;
	struct nkctx_file* p = &nk.files[ctx->index++];

	p->name = grub_strdup(filename);

	if (info->dir)
	{
		p->is_dir = TRUE;
		p->icon = IDR_PNG_DIR;
		p->path = grub_xasprintf("%s%s/", nk.path, filename);
		strcpy_s(p->human_size, ARRAY_SIZE(p->human_size), GET_STR(LANG_STR_DIR));
	}
	else
	{
		p->is_dir = FALSE;
		check_extension(p);
		p->path = grub_xasprintf("%s%s", nk.path, filename);
		grub_file_t file = grub_file_open(p->path, GRUB_FILE_TYPE_GET_SIZE | GRUB_FILE_TYPE_NO_DECOMPRESS);
		if (file)
		{
			p->size = grub_file_size(file);
			strcpy_s(p->human_size, ARRAY_SIZE(p->human_size), grub_get_human_size(p->size, GRUB_HUMAN_SIZE_SHORT));
			grub_file_close(file);
		}
		else
			strcpy_s(p->human_size, ARRAY_SIZE(p->human_size), GET_STR(LANG_STR_UNKNOWN));
	}
	p->time[0] = '\0';
	if (info->mtimeset)
	{
		struct grub_datetime datetime;
		grub_unixtime2datetime(info->mtime, &datetime);
		if (datetime.year < 3000)
			grub_snprintf(p->time, ARRAY_SIZE(p->time), "%04d-%02d-%02d %02d:%02d:%02d",
				datetime.year, datetime.month, datetime.day,
				datetime.hour, datetime.minute, datetime.second);
	}
	if (info->symlink)
		p->icon = IDR_PNG_LINK;
	grub_errno = GRUB_ERR_NONE;
	return 0;
}

static int
callback_sort_file(const void* a, const void* b)
{
	struct nkctx_file* pa = (struct nkctx_file*)a;
	struct nkctx_file* pb = (struct nkctx_file*)b;
	if (pa->is_dir && !pb->is_dir)
		return -1;
	if (!pa->is_dir && pb->is_dir)
		return 1;
	return strcmp(pa->name, pb->name);
}

static void
remove_duplicated_dir(void)
{
	const char* prev = "";
	for (DWORD i = 0; i < nk.dir_count; i++)
	{
		if (strcmp(prev, nk.files[i].name) == 0)
		{
			free(nk.files[i].name);
			free(nk.files[i].path);
			nk.files[i].name = NULL;
			nk.files[i].path = NULL;
		}
		else
			prev = nk.files[i].name;
	}
}

void
nkctx_enum_file(const char* dir)
{
	char* disk_name = NULL;
	grub_fs_t fs;
	const char* path;
	grub_disk_t disk;

	nkctx_set_path(dir);
	nkctx_free_file();

	disk_name = grub_file_get_disk_name(nk.path);
	disk = grub_disk_open(disk_name);
	grub_free(disk_name);
	if (!disk)
		goto fail;

	fs = grub_fs_probe(disk);
	grub_errno = GRUB_ERR_NONE;
	path = grub_strchr(nk.path, ')');
	if (!path)
		path = nk.path;
	else
		path++;

	if (!path || !fs)
		goto fail;

	nk.file_count = 0;
	nk.dir_count = 0;
	fs->fs_dir(disk, path, callback_count_file, NULL);
	grub_errno = GRUB_ERR_NONE;
	if (nk.file_count != 0)
	{
		nk.files = grub_calloc(nk.file_count, sizeof(struct nkctx_file));
		if (!nk.files)
			goto fail;
		struct ctx_enum_file ctx = { 0 };
		fs->fs_dir(disk, path, callback_enum_file, &ctx);
		grub_errno = GRUB_ERR_NONE;
		qsort(nk.files, nk.file_count, sizeof(struct nkctx_file), callback_sort_file);
		remove_duplicated_dir();
	}
	grub_disk_close(disk);
	grub_snprintf(nk.status, ARRAY_SIZE(nk.status), "Found %lu dir(s), %lu item(s)", nk.dir_count, nk.file_count);
	return;

fail:
	if (disk)
		grub_disk_close(disk);
	nk.file_count = 0;
	nk.dir_count = 0;
	grub_snprintf(nk.status, ARRAY_SIZE(nk.status), "Can't open %s", nk.path);
}

void
nkctx_free_file(void)
{
	for (DWORD i = 0; i < nk.file_count; i++)
	{
		free(nk.files[i].name);
		free(nk.files[i].path);
	}
	free(nk.files);
	nk.files = NULL;
	nk.file_count = 0;
	nk.dir_count = 0;
}
