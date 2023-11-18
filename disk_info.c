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

#include <nkctx.h>
#include <loopback.h>
#include <lang.h>

#include <grub/types.h>
#include <grub/disk.h>
#include <grub/fs.h>
#include <grub/misc.h>
#include <grub/file.h>
#include <grub/diskfilter.h>

static struct
{
	char* name;
	const char* driver;
	enum grub_disk_dev_id id;
	char* parent;
	grub_uint64_t size;
	const char* fs;
	char* label;
	char* uuid;
} m_ctx;

static const char*
get_disk_parent(grub_disk_t disk)
{
	switch (disk->dev->id)
	{
	case GRUB_DISK_DEVICE_LOOPBACK_ID:
		return ((struct grub_loopback*) disk->data)->file->name;
	//case GRUB_DISK_DEVICE_DISKFILTER_ID:
		//return ((struct grub_diskfilter_lv*)disk->data)->name;
	}
	return NULL;
}

void
nkctx_disk_info_init(const char* name)
{
	grub_memset(&m_ctx, 0, sizeof(m_ctx));
	grub_disk_t disk = grub_disk_open(name);
	if (!disk)
	{
		grub_errno = GRUB_ERR_NONE;
		return;
	}
	m_ctx.name = grub_strdup(name);
	m_ctx.driver = disk->dev->name;
	m_ctx.id = disk->dev->id;
	const char* p = get_disk_parent(disk);
	if (p)
		m_ctx.parent = grub_strdup(p);
	m_ctx.size = grub_disk_native_sectors(disk) << GRUB_DISK_SECTOR_BITS;
	grub_errno = GRUB_ERR_NONE;
	grub_fs_t fs = grub_fs_probe(disk);
	grub_errno = GRUB_ERR_NONE;
	if (fs)
	{
		m_ctx.fs = fs->name;
		if (fs->fs_label)
			fs->fs_label(disk, &m_ctx.label);
		if (fs->fs_uuid)
			fs->fs_uuid(disk, &m_ctx.uuid);
	}
	grub_disk_close(disk);
}

void
nkctx_disk_info_fini(void)
{
	grub_free(m_ctx.name);
	grub_free(m_ctx.parent);
	grub_memset(&m_ctx, 0, sizeof(m_ctx));
}

void
nkctx_disk_info_window(struct nk_context* ctx, float width, float height)
{
	if (!m_ctx.name)
		return;
	if (!nk_begin(ctx, "Disk Info",
		nk_rect(width * 0.1f, height / 3.0f, width * 0.8f, height / 3.0f),
		NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE | NK_WINDOW_CLOSABLE))
	{
		nkctx_disk_info_fini();
		goto out;
	}

	nk_layout_row(ctx, NK_DYNAMIC, 0, 2, (float[2]) { 0.4f, 0.6f });

	nk_label(ctx, GET_STR(LANG_STR_NAME), NK_TEXT_LEFT);
	nk_label(ctx, m_ctx.name, NK_TEXT_LEFT);

	nk_label(ctx, GET_STR(LANG_STR_DRIVER), NK_TEXT_LEFT);
	nk_label(ctx, m_ctx.driver, NK_TEXT_LEFT);

	nk_label(ctx, GET_STR(LANG_STR_SIZE), NK_TEXT_LEFT);
	nk_label(ctx, grub_get_human_size(m_ctx.size, GRUB_HUMAN_SIZE_NORMAL), NK_TEXT_LEFT);

	if (m_ctx.parent)
	{
		nk_label(ctx, GET_STR(LANG_STR_PARENT), NK_TEXT_LEFT);
		nk_label(ctx, m_ctx.parent, NK_TEXT_LEFT);
	}

	if (m_ctx.fs)
	{
		nk_label(ctx, GET_STR(LANG_STR_FS), NK_TEXT_LEFT);
		nk_label(ctx, m_ctx.fs, NK_TEXT_LEFT);
	}

	if (m_ctx.label)
	{
		nk_label(ctx, GET_STR(LANG_STR_VOL_LABEL), NK_TEXT_LEFT);
		nk_label(ctx, m_ctx.label, NK_TEXT_LEFT);
	}

	if (m_ctx.uuid)
	{
		nk_label(ctx, GET_STR(LANG_STR_FS_UUID), NK_TEXT_LEFT);
		nk_label(ctx, m_ctx.uuid, NK_TEXT_LEFT);
	}

out:
	nk_end(ctx);
}
