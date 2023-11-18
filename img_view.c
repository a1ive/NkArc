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
#include <nuklear_gdip.h>

#include <grub/types.h>
#include <grub/misc.h>
#include <grub/mm.h>
#include <grub/file.h>

#define IMG_MAX_SIZE 0x4000000 // 64MB

static struct
{
	char* path;
	grub_size_t size;
	struct nk_image img;
	struct nk_rect bounds;
	float ratio;
} m_ctx;

void
nkctx_image_init(const char* path)
{
	void* data = NULL;
	grub_file_t file = grub_file_open(path, GRUB_FILE_TYPE_THEME);
	if (!file)
	{
		MessageBoxW(nk.wnd, L"CANNOT OPEN FILE", L"ERROR", MB_OK | MB_ICONERROR);
		goto fail;
	}
	m_ctx.size = grub_file_size(file);
	if (m_ctx.size > IMG_MAX_SIZE)
	{
		MessageBoxW(nk.wnd, L"FILE TOO LARGE", L"ERROR", MB_OK | MB_ICONERROR);
		goto fail;
	}
	data = grub_malloc(m_ctx.size);
	if (!data)
	{
		MessageBoxW(nk.wnd, L"OUT OF MEMORY", L"ERROR", MB_OK | MB_ICONERROR);
		goto fail;
	}
	grub_file_read(file, data, m_ctx.size);
	m_ctx.img = nk_gdip_load_image_from_memory(data, m_ctx.size);
	if (m_ctx.img.w == 0 || m_ctx.img.h == 0)
		goto fail;
	m_ctx.path = grub_strdup(path);
	grub_free(data);
	grub_file_close(file);

	m_ctx.ratio = (float)m_ctx.img.w / (float)m_ctx.img.h;
	m_ctx.bounds.w = (float)nk.width * 0.95f - 2 * nk.title_height;
	m_ctx.bounds.h = (float)nk.height * 0.95f - 2 * nk.title_height;
	if ((float)m_ctx.img.w <= m_ctx.bounds.w
		&& (float)m_ctx.img.h <= m_ctx.bounds.h)
	{
		m_ctx.bounds.w = max(m_ctx.img.w, 96) * m_ctx.ratio;
		m_ctx.bounds.h = max(m_ctx.img.h, 96) * m_ctx.ratio;
	}
	else
	{
		if (m_ctx.ratio > 1.0f)
			m_ctx.bounds.h = m_ctx.bounds.w / m_ctx.ratio;
		else
			m_ctx.bounds.w = m_ctx.bounds.h * m_ctx.ratio;
	}
	m_ctx.bounds.w += 2 * nk.title_height;
	m_ctx.bounds.h += 2 * nk.title_height;
	m_ctx.bounds.x = ((float)nk.width - m_ctx.bounds.w) / 2.0f;
	m_ctx.bounds.y = ((float)nk.height - m_ctx.bounds.h) / 2.0f;

	return;

fail:
	m_ctx.size = 0;
	m_ctx.img = nk_image_id(0);
	if (data)
		grub_free(data);
	if (file)
		grub_file_close(file);
	return;
}

void
nkctx_image_fini(void)
{
	nk_gdip_image_free(m_ctx.img);
	m_ctx.img = nk_image_id(0);
	grub_free(m_ctx.path);
	m_ctx.path = NULL;
	m_ctx.size = 0;
}

static void
draw_image(struct nk_context* ctx, struct nk_image img, struct nk_color col)
{
	struct nk_window* win = ctx->current;
	struct nk_rect bounds;
	struct nk_rect image;
	if (!nk_widget(&bounds, ctx))
		return;
	if (m_ctx.ratio > 1.0f)
	{
		image.w = bounds.w;
		image.h = bounds.w / m_ctx.ratio;
		image.x = bounds.x;
		image.y = bounds.y + (bounds.h - image.h) / 2.0f;
	}
	else
	{
		image.w = bounds.h * m_ctx.ratio;
		image.h = bounds.h;
		image.x = bounds.x + (bounds.w - image.w) / 2.0f;
		image.y = bounds.y;
	}
	nk_draw_image(&win->buffer, image, &img, col);
}

void
nkctx_image_window(struct nk_context* ctx, float width, float height)
{
	if (m_ctx.img.w == 0 || m_ctx.img.h == 0)
		return;

	if (!nk_begin(ctx, "Image Viewer", m_ctx.bounds,
		NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE | NK_WINDOW_CLOSABLE))
	{
		nkctx_image_fini();
		goto out;
	}

	nk_layout_row_dynamic(ctx, m_ctx.bounds.h - 2 * nk.title_height, 1);

	draw_image(ctx, m_ctx.img, nk_rgb(255, 255, 255));

out:
	nk_end(ctx);
}
