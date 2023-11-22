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

#include <grub/misc.h>
#include <grub/mm.h>

#define MAIN_WINDOW_NAME "NkArc"

void
nkctx_unset_path(void)
{
	grub_free(nk.path);
	nk.path = NULL;
}

void
nkctx_set_path(const char* path)
{
	nkctx_unset_path();
	nk.path = grub_strdup(path);
}

static void
go_up(void)
{
	if (!nk.path || nk.path[0] == '\0')
		return;
	char* path = grub_strdup(nk.path);
	path[strlen(path) - 1] = '\0';
	char* start = grub_strchr(path, ')');
	if (!start)
		goto fail;
	char* p = strrchr(start, '/');
	if (!p)
		goto fail;

	*(p + 1) = '\0';
	nkctx_enum_file(path);
	free(path);
	return;

fail:
	grub_free(path);
	nkctx_free_file();
	nk.disk_index = 0;
	nkctx_unset_path();
}

static void
draw_disk_menu(struct nk_context* ctx, struct nkctx_disk* info, struct nk_rect bounds)
{
	if (!nk_hc_begin(ctx, (float)nk.width / 4.0f, nk.title_height * 4.0f, bounds))
		return;

	nk.style_button.normal = nk_style_item_color(nk.table[NK_COLOR_WINDOW]);

	nk_layout_row_dynamic(ctx, 0, 1);

	if (info->is_loopback)
	{
		if (nk_hb_image_label_styled(ctx, &nk.style_button, GET_PNG(IDR_PNG_RM), GET_STR(LANG_STR_UMOUNT_DISK), NULL))
		{
			grub_loopback_delete(info->name);
			nkctx_free_file();
			nkctx_free_disk();
			nkctx_enum_disk();
			nk_contextual_close(ctx);
		}
	}

	if (nk_hb_image_label_styled(ctx, &nk.style_button, GET_PNG(IDR_PNG_EMPTY), GET_STR(LANG_STR_DISK_INFO), NULL))
	{
		nkctx_disk_info.init(info->name);
		nk_contextual_close(ctx);
	}

	if (nk_hb_image_label_styled(ctx, &nk.style_button, GET_PNG(IDR_PNG_ERROR), GET_STR(LANG_STR_CANCEL), NULL))
		nk_contextual_close(ctx);
	nk_contextual_end(ctx);
}

static void
draw_disk_button(struct nk_context* ctx, struct nkctx_disk* info)
{
	if (nk.disk_index == info->index)
		nk.style_button.normal = nk_style_item_color(nk_rgb(63, 98, 126));
	else
		nk.style_button.normal = nk_style_item_color(nk.table[NK_COLOR_WINDOW]);
	nk_layout_row(ctx, NK_DYNAMIC, 0, 2, (float[2]) { 0.1f, 0.9f });
	nk_spacer(ctx);
	struct nk_rect bounds = nk_widget_bounds(ctx);
	if (nk_hb_image_label_styled(ctx, &nk.style_button, GET_PNG(info->icon), info->name, info->desc))
	{
		nk.disk_index = info->index;
		nkctx_enum_file(info->path);
	}
	draw_disk_menu(ctx, info, bounds);
}

static void
draw_pc_menu(struct nk_context* ctx, struct nk_rect bounds)
{
	if (!nk_hc_begin(ctx, (float)nk.width / 4.0f, nk.title_height * 4.0f, bounds))
		return;

	nk.style_button.normal = nk_style_item_color(nk.table[NK_COLOR_WINDOW]);

	nk_layout_row_dynamic(ctx, 0, 1);

	if (nk_hb_image_label_styled(ctx, &nk.style_button, GET_PNG(IDR_PNG_REFRESH), GET_STR(LANG_STR_REFRESH_DISKS), NULL))
	{
		nkctx_free_file();
		nk.disk_index = 0;
		nkctx_free_disk();
		nkctx_enum_disk();
		nk_contextual_close(ctx);
	}

	if (nk_hb_image_label_styled(ctx, &nk.style_button, GET_PNG(IDR_PNG_ERROR), GET_STR(LANG_STR_CANCEL), NULL))
		nk_contextual_close(ctx);
	nk_contextual_end(ctx);
}

static void
draw_pc_button(struct nk_context* ctx)
{
	if (nk.disk_index == 0)
		nk.style_button.normal = nk_style_item_color(nk_rgb(63, 98, 126));
	else
		nk.style_button.normal = nk_style_item_color(nk.table[NK_COLOR_WINDOW]);
	nk_layout_row_dynamic(ctx, 0, 1);
	struct nk_rect bounds = nk_widget_bounds(ctx);
	if (nk_hb_image_label_styled(ctx, &nk.style_button, GET_PNG(IDR_PNG_PC), GET_STR(LANG_STR_THIS_PC), NULL))
	{
		nkctx_free_file();
		nk.disk_index = 0;
		nkctx_unset_path();
	}
	draw_pc_menu(ctx, bounds);
}

static void
draw_file_menu(struct nk_context* ctx, struct nkctx_file* info, struct nk_rect bounds)
{
	if (!nk_hc_begin(ctx, (float)nk.width / 4.0f, nk.title_height * 6.0f, bounds))
		return;

	nk.style_button.normal = nk_style_item_color(nk.table[NK_COLOR_WINDOW]);

	nk_layout_row_dynamic(ctx, 0, 1);

	if (info->type == NKCTX_FILE_IMAGE)
	{
		if (nk_hb_image_label_styled(ctx, &nk.style_button, GET_PNG(IDR_PNG_IMAGE), GET_STR(LANG_STR_VIEW_IMAGE), NULL))
		{
			nkctx_image.init(info->path);
			nk_contextual_close(ctx);
		}
	}

	if (nk_hb_image_label_styled(ctx, &nk.style_button, GET_PNG(IDR_PNG_RM), GET_STR(LANG_STR_MOUNT_DISK), NULL))
	{
		nkctx_mount.init(info->path);
		nk_contextual_close(ctx);
	}

	if (nk_hb_image_label_styled(ctx, &nk.style_button, GET_PNG(IDR_PNG_OK), GET_STR(LANG_STR_GET_CHECKSUM), NULL))
	{
		nkctx_hash.init(info->path);
		nk_contextual_close(ctx);
	}

	if (nk_hb_image_label_styled(ctx, &nk.style_button, GET_PNG(IDR_PNG_COPY), GET_STR(LANG_STR_EXTRACT_FILE), NULL))
	{
		WCHAR* dir = nkctx_select_dir();
		if (dir)
		{
			if (nkctx_extract_file(dir, info->path))
				MessageBoxW(nk.wnd, GET_WCS(LANG_WCS_DONE), GET_WCS(LANG_WCS_INFO), MB_OK);
			else
				MessageBoxW(nk.wnd, dir, GET_WCS(LANG_WCS_FAIL), MB_OK | MB_ICONERROR);
			free(dir);
		}
		nk_contextual_close(ctx);
	}
	
	if (nk_hb_image_label_styled(ctx, &nk.style_button, GET_PNG(IDR_PNG_ERROR), GET_STR(LANG_STR_CANCEL), NULL))
		nk_contextual_close(ctx);
	nk_contextual_end(ctx);
}

static void
draw_file_info(struct nk_context* ctx, struct nkctx_file* info)
{
	struct nk_rect bounds;
	struct nk_color color = nk.table[NK_COLOR_WINDOW];
	if (!info->name)
		return;
#ifdef NKCTX_SELECT_FILE
	nk_layout_row(ctx, NK_DYNAMIC, 0, 4, (float[4]) { nk.gui_ratio, 0.6f, 0.3f, 0.1f - nk.gui_ratio });
	info->selected = !nk_check_text(ctx, "", 1, !info->selected);
	if (info->selected)
		color = nk_rgb(63, 98, 126);
#else
	nk_layout_row(ctx, NK_DYNAMIC, 0, 3, (float[3]) { 0.6f, 0.3f, 0.1f });
#endif
	nk.style_button.normal = nk_style_item_color(color);

	bounds = nk_widget_bounds(ctx);
	if (nk_hb_image_label_styled(ctx, &nk.style_button, GET_PNG(info->icon), info->name, info->name))
	{
		if (info->is_dir)
			nkctx_enum_file(info->path);
		else
			nkctx_hex.init(info->path);
	}

	if (!info->is_dir)
		draw_file_menu(ctx, info, bounds);

	nk_label(ctx, info->time, NK_TEXT_LEFT);
	nk_label(ctx, info->human_size, NK_TEXT_LEFT);
}

void
nkctx_main_window(struct nk_context* ctx, float width, float height)
{
	struct nk_rect total_space;

	if (!nk_begin(ctx, MAIN_WINDOW_NAME, nk_rect(0.0f, 0.0f, width, height),
		NK_WINDOW_BACKGROUND | NK_WINDOW_CLOSABLE | NK_WINDOW_TITLE))
	{
		nkctx_fini(0);
	}
	nk_layout_row_begin(ctx, NK_DYNAMIC, 0, 3);

	struct nk_rect rect = nk_layout_widget_bounds(ctx);
	nk.gui_ratio = rect.h / rect.w;

	nk_layout_row_push(ctx, nk.gui_ratio);
	if (nk_hb_image(ctx, GET_PNG(IDR_PNG_UP), GET_STR(LANG_STR_UP)))
		go_up();
	nk_layout_row_push(ctx, nk.gui_ratio);
	if (nk_hb_image(ctx, GET_PNG(IDR_PNG_REFRESH), GET_STR(LANG_STR_REFRESH_FILES)))
		nkctx_enum_file(nk.path);
	nk_layout_row_push(ctx, 1.0f - 2 * nk.gui_ratio);
	nk_image_label(ctx, GET_PNG(IDR_PNG_DIR), nk.path ? nk.path : GET_STR(LANG_STR_THIS_PC), NK_TEXT_LEFT, nk.table[NK_COLOR_TEXT]);
	nk_layout_row_end(ctx);

	total_space = nk_window_get_content_region(ctx);
	nk_layout_row(ctx, NK_DYNAMIC, total_space.h - nk.title_height * 2.2f, 2, (float[2]){ 0.2f, 0.8f });

	nk_group_begin(ctx, "DISK", NK_WINDOW_BORDER);
	draw_pc_button(ctx);
	for (struct nkctx_disk* info = nk.disks; info; info = info->next)
	{
		draw_disk_button(ctx, info);
	}
	nk_group_end(ctx);

	nk_group_begin(ctx, "FILE", NK_WINDOW_BORDER);
	for (DWORD i = 0; i < nk.file_count; i++)
	{
		struct nkctx_file* info = &nk.files[i];
		draw_file_info(ctx, info);
	}
	nk_group_end(ctx);

	nk_layout_row_dynamic(ctx, 0, 1);
	nk_label(ctx, nk.status, NK_TEXT_LEFT);

	nk_end(ctx);
}
