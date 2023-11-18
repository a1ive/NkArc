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

#include <grub/types.h>
#include <grub/misc.h>
#include <grub/mm.h>

#define MAX_LOOPBACK_NAME 16

static struct
{
	unsigned long id;
	char* path;
	int decompress;
	char name[MAX_LOOPBACK_NAME];
} m_ctx;

void
nkctx_mount_init(const char* path)
{
	m_ctx.path = grub_strdup(path);
	m_ctx.decompress = 1;
	grub_snprintf(m_ctx.name, MAX_LOOPBACK_NAME, "ld%lu", m_ctx.id);
}

void
nkctx_mount_fini(void)
{
	grub_free(m_ctx.path);
	m_ctx.path = NULL;
}

static nk_bool
name_filter(const struct nk_text_edit* box, nk_rune unicode)
{
	NK_UNUSED(box);
	// A - Z, a - z, 0 - 9
	if (unicode >= 'A' && unicode <= 'Z')
		return nk_true;
	if (unicode >= 'a' && unicode <= 'z')
		return nk_true;
	if (unicode >= '0' && unicode <= '9')
		return nk_true;
	return nk_false;
}

static void
mount_file(void)
{
	grub_loopback_add(m_ctx.name, m_ctx.path, m_ctx.decompress);
	m_ctx.id++;
	nkctx_free_disk();
	nkctx_enum_disk();
	for (struct nkctx_disk* info = nk.disks; info; info = info->next)
	{
		if (grub_strcmp(m_ctx.name, info->name) != 0)
			continue;
		nk.disk_index = info->index;
		nkctx_enum_file(info->path);
		break;
	}
	nkctx_mount_fini();
}

void
nkctx_mount_window(struct nk_context* ctx, float width, float height)
{
	if (!m_ctx.path)
		return;
	if (!nk_begin(ctx, "Mount",
		nk_rect(width / 4.0f, height / 3.0f, width / 2.0f, height / 3.0f),
		NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE | NK_WINDOW_CLOSABLE))
	{
		nkctx_mount_fini();
		goto out;
	}

	nk_layout_row_dynamic(ctx, 0, 1);
	nk_label(ctx, m_ctx.path, NK_TEXT_LEFT);

	nk_checkbox_label(ctx, "No decompress", &m_ctx.decompress);

	nk_edit_string_zero_terminated(ctx, NK_EDIT_FIELD, m_ctx.name, MAX_LOOPBACK_NAME, name_filter);

	if (nk_button_label(ctx, "Mount") && m_ctx.name[0])
		mount_file();

out:
	nk_end(ctx);
}
