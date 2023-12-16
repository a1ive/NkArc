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

#include <grub/types.h>
#include <grub/misc.h>
#include <grub/mm.h>
#include <grub/file.h>

#include <lang.h>

#define HEX_VIEW_STEP 0x100

static struct
{
	grub_file_t file;
	grub_off_t offset;
	grub_off_t size;
	grub_uint8_t buf[HEX_VIEW_STEP];
} m_ctx;

static void
nkctx_hex_init(const char* path)
{
	m_ctx.file = grub_file_open(path, GRUB_FILE_TYPE_HEXCAT | GRUB_FILE_TYPE_NO_DECOMPRESS);
	if (!m_ctx.file)
	{
		MessageBoxW(nk.wnd, GET_WCS(LANG_WCS_CANNOT_OPEN_FILE), GET_WCS(LANG_WCS_ERROR), MB_OK | MB_ICONERROR);
		return;
	}
	m_ctx.offset = 0;
	m_ctx.size = grub_file_size(m_ctx.file);
	grub_memset(m_ctx.buf, 0, HEX_VIEW_STEP);
	grub_file_read(m_ctx.file, m_ctx.buf, HEX_VIEW_STEP);
}

static void
nkctx_hex_fini(void)
{
	grub_file_close(m_ctx.file);
	m_ctx.file = NULL;
	m_ctx.offset = 0;
	m_ctx.size = 0;
}

static void
update_buf(int forward)
{
	if (!forward && m_ctx.offset >= HEX_VIEW_STEP)
		m_ctx.offset -= HEX_VIEW_STEP;
	else if (forward && m_ctx.offset + HEX_VIEW_STEP < m_ctx.size)
		m_ctx.offset += HEX_VIEW_STEP;
	else
		return;
	grub_memset(m_ctx.buf, 0, HEX_VIEW_STEP);
	grub_file_seek(m_ctx.file, m_ctx.offset);
	grub_file_read(m_ctx.file, m_ctx.buf, HEX_VIEW_STEP);
}

static void
draw_row(struct nk_context* ctx, grub_off_t base)
{
	grub_size_t i;
	struct nk_color c1 = NK_COLOR_BLUE;
	if (base < 0xFFFFFFFFULL)
		nk_labelf_colored(ctx, NK_TEXT_LEFT, c1, "%08llX| ", m_ctx.offset + base);
	else
		nk_labelf_colored(ctx, NK_TEXT_LEFT, c1, "%016llX| ", m_ctx.offset + base);
	char buf[50] = { 0 };
	for (i = 0; i < 0x10; i++)
		grub_snprintf(buf + i * 3, 4, "%02X ", m_ctx.buf[base + i]);
	nk_label(ctx, buf, NK_TEXT_LEFT);
	grub_strcpy(buf, "|");
	for (i = 0; i < 0x10; i++)
		buf[1 + i] = grub_isprint(m_ctx.buf[base + i]) ? m_ctx.buf[base + i] : '.';
	buf[0x11] = 0;
	nk_label(ctx, buf, NK_TEXT_LEFT);
}

static void
nkctx_hex_window(struct nk_context* ctx, float width, float height)
{
	if (!m_ctx.file)
		return;
	if (!nk_begin(ctx, "Hex Viewer",
		nk_rect(2.0f, height * 0.1f, width * 0.95f, height * 0.8f),
		NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE | NK_WINDOW_CLOSABLE))
	{
		nkctx_hex_fini();
		goto out;
	}

	nk_layout_row(ctx, NK_DYNAMIC, 0, 3, (float[3]){ nk.gui_ratio, 1.0f - 2 * nk.gui_ratio, nk.gui_ratio });
	if (nk_button_symbol(ctx, NK_SYMBOL_TRIANGLE_LEFT))
		update_buf(0);
	if (m_ctx.file->name)
		nk_label(ctx, m_ctx.file->name, NK_TEXT_LEFT);
	else
		nk_spacer(ctx);
	if (nk_button_symbol(ctx, NK_SYMBOL_TRIANGLE_RIGHT))
		update_buf(1);

	nk_layout_row(ctx, NK_DYNAMIC, 0, 3, (float[3]){ 0.12f, 0.66f, 0.22f });
	for (grub_size_t i = 0; i < HEX_VIEW_STEP; i += 0x10)
		draw_row(ctx, i);

out:
	nk_end(ctx);
}

struct nkctx_window nkctx_hex =
{
	.init = nkctx_hex_init,
	.fini = nkctx_hex_fini,
	.window = nkctx_hex_window,
};
