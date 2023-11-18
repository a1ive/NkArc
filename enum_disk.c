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

#include <nkctx.h>
#include <lang.h>

static int
callback_enum_disk(const char* name, void* data)
{
	grub_disk_t disk = NULL;
	grub_fs_t fs = NULL;
	struct nkctx_disk** tail = (struct nkctx_disk**)data;
	struct nkctx_disk* info = calloc(1, sizeof(struct nkctx_disk));

	if (!info)
		return 0;

	disk = grub_disk_open(name);
	if (!disk)
	{
		free(info);
		return 0;
	}

	info->index = ++nk.disk_count;
	info->name = grub_strdup(name);

	info->path = grub_xasprintf("(%s)/", name);
	info->icon = IDR_PNG_UNKNOWN;

	if (disk->partition)
		info->is_partition = TRUE;
	else
		info->icon = IDR_PNG_HDD;

	if (disk->dev->id == GRUB_DISK_DEVICE_LOOPBACK_ID)
		info->is_loopback = TRUE;

	info->size = grub_disk_native_sectors(disk);
	if (info->size == GRUB_DISK_SIZE_UNKNOWN)
		strcpy_s(info->desc, ARRAY_SIZE(info->desc), GET_STR(LANG_STR_UNKNOWN));
	else
		strcpy_s(info->desc, ARRAY_SIZE(info->desc),
			grub_get_human_size(info->size << GRUB_DISK_SECTOR_BITS, GRUB_HUMAN_SIZE_SHORT));

	fs = grub_fs_probe(disk);
	if (fs)
	{
		info->icon = IDR_PNG_HDD;
		info->fs = fs->name;
		strcat_s(info->desc, ARRAY_SIZE(info->desc), " ");
		strcat_s(info->desc, ARRAY_SIZE(info->desc), fs->name);
		if (fs->fs_label)
			fs->fs_label(disk, &info->label);
		if (info->label && info->label[0])
		{
			strcat_s(info->desc, ARRAY_SIZE(info->desc), " [");
			strcat_s(info->desc, ARRAY_SIZE(info->desc), info->label);
			strcat_s(info->desc, ARRAY_SIZE(info->desc), "]");
		}
	}

	switch (disk->dev->id)
	{
	case GRUB_DISK_DEVICE_CRYPTODISK_ID:
		info->icon = IDR_PNG_LOCK;
		break;
	case GRUB_DISK_DEVICE_DISKFILTER_ID:
		info->icon = IDR_PNG_SERVER;
		break;
	case GRUB_DISK_DEVICE_LOOPBACK_ID:
		info->icon = IDR_PNG_LINK;
		break;
	case GRUB_DISK_DEVICE_PROCFS_ID:
		info->icon = IDR_PNG_DIR;
		break;
	}

	grub_disk_close(disk);

	if (*tail)
		(*tail)->next = info;
	else
		nk.disks = info;
	*tail = info;

	grub_errno = GRUB_ERR_NONE;
	return 0;
}

void
nkctx_enum_disk(void)
{
	struct nkctx_disk* tail = NULL;
	nk.disk_count = 0;
	grub_disk_iterate(callback_enum_disk, &tail);
	grub_snprintf(nk.status, ARRAY_SIZE(nk.status), "Found %lu disk(s)", nk.disk_count);
}

void
nkctx_free_disk(void)
{
	struct nkctx_disk* p = nk.disks;
	while (p)
	{
		struct nkctx_disk* next = p->next;
		free(p->name);
		free(p->path);
		free(p->label);
		free(p);
		p = next;
	}
	nk.disks = NULL;
	nk.disk_count = 0;
	nk.disk_index = 0;
	nkctx_unset_path();
}

