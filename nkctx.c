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
#include <resource.h>

#include <windowsx.h>

#include "dl.h"
#include <grub/misc.h>
#include <grub/mm.h>

NK_GUI_CTX nk;

#define COPY_SIZE 65536 // 64K

#define REGION_MASK_LEFT    (1 << 0)
#define REGION_MASK_RIGHT   (1 << 1)
#define REGION_MASK_TOP     (1 << 2)
#define REGION_MASK_BOTTOM  (1 << 3)

static INT_PTR CALLBACK
nkctx_proc_progress(HWND dlg, UINT message, WPARAM wparam, LPARAM lparam)
{
	UNREFERENCED_PARAMETER(lparam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;
	}
	return (INT_PTR)FALSE;
}

void
nkctx_show_progress(void)
{
	nk.progress = TRUE;
	ShowWindow(nk.progress_wnd, SW_SHOW);
}

void
nkctx_hide_progress(void)
{
	nk.progress = FALSE;
	ShowWindow(nk.progress_wnd, SW_HIDE);
}

static LRESULT CALLBACK
nkctx_window_proc(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_DPICHANGED:
		break;
	case WM_NCHITTEST:
	{
		RECT rect = { 0 };
		LONG result = 0;
		LONG x = GET_X_LPARAM(lparam);
		LONG y = GET_Y_LPARAM(lparam);
		LONG w = GetSystemMetricsForDpi(SM_CXFRAME, USER_DEFAULT_SCREEN_DPI)
			+ GetSystemMetricsForDpi(SM_CXPADDEDBORDER, USER_DEFAULT_SCREEN_DPI);
		LONG h = GetSystemMetricsForDpi(SM_CYFRAME, USER_DEFAULT_SCREEN_DPI)
			+ GetSystemMetricsForDpi(SM_CXPADDEDBORDER, USER_DEFAULT_SCREEN_DPI);
		GetWindowRect(wnd, &rect);
		result = REGION_MASK_LEFT * (x < (rect.left + w)) |
			REGION_MASK_RIGHT * (x >= (rect.right - w)) |
			REGION_MASK_TOP * (y < (rect.top + h)) |
			REGION_MASK_BOTTOM * (y >= (rect.bottom - h));
		switch (result)
		{
		case REGION_MASK_LEFT: return HTLEFT;
		case REGION_MASK_RIGHT: return HTRIGHT;
		case REGION_MASK_TOP: return HTTOP;
		case REGION_MASK_BOTTOM: return HTBOTTOM;
		case REGION_MASK_TOP | REGION_MASK_LEFT: return HTTOPLEFT;
		case REGION_MASK_TOP | REGION_MASK_RIGHT: return HTTOPRIGHT;
		case REGION_MASK_BOTTOM | REGION_MASK_LEFT: return HTBOTTOMLEFT;
		case REGION_MASK_BOTTOM | REGION_MASK_RIGHT: return HTBOTTOMRIGHT;
		}
		if (y <= (LONG)(rect.top + nk.title_height) &&
			x <= (LONG)(rect.right - 3 * nk.title_height))
			return HTCAPTION;
	}
		break;
	case WM_SIZE:
		nk.height = HIWORD(lparam);
		nk.width = LOWORD(lparam);
		break;
	}
	if (nk_gdip_handle_event(wnd, msg, wparam, lparam))
		return 0;
	return DefWindowProcW(wnd, msg, wparam, lparam);
}

static void
set_style(struct nk_context* ctx)
{
	nk.table[NK_COLOR_TEXT] = nk_rgba(10, 10, 10, 255);
	nk.table[NK_COLOR_WINDOW] = nk_rgba(175, 175, 175, 255);
	nk.table[NK_COLOR_HEADER] = nk_rgba(175, 175, 175, 255);
	nk.table[NK_COLOR_BORDER] = nk_rgba(0, 0, 0, 255);
	nk.table[NK_COLOR_BUTTON] = nk_rgba(185, 185, 185, 255);
	nk.table[NK_COLOR_BUTTON_HOVER] = nk_rgba(170, 170, 170, 255);
	nk.table[NK_COLOR_BUTTON_ACTIVE] = nk_rgba(160, 160, 160, 255);
	nk.table[NK_COLOR_TOGGLE] = nk_rgba(150, 150, 150, 255);
	nk.table[NK_COLOR_TOGGLE_HOVER] = nk_rgba(120, 120, 120, 255);
	nk.table[NK_COLOR_TOGGLE_CURSOR] = nk_rgba(175, 175, 175, 255);
	nk.table[NK_COLOR_SELECT] = nk_rgba(190, 190, 190, 255);
	nk.table[NK_COLOR_SELECT_ACTIVE] = nk_rgba(175, 175, 175, 255);
	nk.table[NK_COLOR_SLIDER] = nk_rgba(190, 190, 190, 255);
	nk.table[NK_COLOR_SLIDER_CURSOR] = nk_rgba(80, 80, 80, 255);
	nk.table[NK_COLOR_SLIDER_CURSOR_HOVER] = nk_rgba(70, 70, 70, 255);
	nk.table[NK_COLOR_SLIDER_CURSOR_ACTIVE] = nk_rgba(60, 60, 60, 255);
	nk.table[NK_COLOR_PROPERTY] = nk_rgba(175, 175, 175, 255);
	nk.table[NK_COLOR_EDIT] = nk_rgba(150, 150, 150, 255);
	nk.table[NK_COLOR_EDIT_CURSOR] = nk_rgba(0, 0, 0, 255);
	nk.table[NK_COLOR_COMBO] = nk_rgba(175, 175, 175, 255);
	nk.table[NK_COLOR_CHART] = nk_rgba(160, 160, 160, 255);
	nk.table[NK_COLOR_CHART_COLOR] = nk_rgba(45, 45, 45, 255);
	nk.table[NK_COLOR_CHART_COLOR_HIGHLIGHT] = nk_rgba(255, 0, 0, 255);
	nk.table[NK_COLOR_SCROLLBAR] = nk_rgba(180, 180, 180, 255);
	nk.table[NK_COLOR_SCROLLBAR_CURSOR] = nk_rgba(140, 140, 140, 255);
	nk.table[NK_COLOR_SCROLLBAR_CURSOR_HOVER] = nk_rgba(150, 150, 150, 255);
	nk.table[NK_COLOR_SCROLLBAR_CURSOR_ACTIVE] = nk_rgba(160, 160, 160, 255);
	nk.table[NK_COLOR_TAB_HEADER] = nk_rgba(180, 180, 180, 255);

	nk_style_from_table(ctx, nk.table);

	ctx->style.window.min_row_height_padding = 2;

	ctx->style.button.rounding = 0;
	ctx->style.button.border = 1.0f;
	ctx->style.button.padding = nk_vec2(0.0f, 0.0f);

	ctx->style.progress.padding = nk_vec2(4.0f, 4.0f);
	ctx->style.progress.border = 1.0f;

	//ctx->style.edit.padding = nk_vec2(0.0f, 0.0f);

	nk.title_height = nk.font_size
		+ ctx->style.window.header.padding.y
		+ ctx->style.window.header.label_padding.y;

	nk.style_button = ctx->style.button;
	nk.style_button.border = 0;
	nk.style_button.normal = nk_style_item_color(nk.table[NK_COLOR_WINDOW]);
	nk.style_button.hover = nk_style_item_color(nk.table[NK_COLOR_BUTTON_ACTIVE]);
	nk.style_button.active = nk_style_item_color(nk_rgb(63, 98, 126));
	nk.style_button.text_alignment = NK_TEXT_LEFT;
}

static struct nk_image
load_png(WORD id)
{
	HRSRC res = FindResourceW(NULL, MAKEINTRESOURCEW(id), RT_RCDATA);
	if (!res)
		goto fail;
	HGLOBAL mem = LoadResource(NULL, res);
	if (!mem)
		goto fail;
	DWORD size = SizeofResource(NULL, res);
	if (!size)
		goto fail;
	void* data = LockResource(mem);
	if (!data)
		goto fail;
	return nk_gdip_load_image_from_memory(data, size);
fail:
	return nk_image_id(0);
}

void
nkctx_init(HINSTANCE inst,
	int x, int y, unsigned width, unsigned height,
	LPCWSTR class_name, LPCWSTR title,
	LPCWSTR font_name, int font_size)
{
	DWORD style = WS_POPUP | WS_VISIBLE;
	DWORD exstyle = WS_EX_LAYERED;

	CoInitialize(NULL);

	ZeroMemory(&nk, sizeof(nk));

	nk.inst = inst;
	nk.width = width;
	nk.height = height;
	nk.alpha = 255;
	nk.font_size = font_size;
	nk.lang = GetUserDefaultUILanguage();

	nk.copy_size = COPY_SIZE;
	nk.copy_buf = grub_malloc(nk.copy_size);

	nk.wc.style = CS_DBLCLKS;
	nk.wc.lpfnWndProc = nkctx_window_proc;
	nk.wc.hInstance = inst;
	nk.wc.hIcon = LoadIconW(inst, MAKEINTRESOURCEW(IDI_MAIN_ICON));
	nk.wc.hCursor = LoadCursorW(NULL, IDC_ARROW);
	nk.wc.lpszClassName = class_name;
	RegisterClassW(&nk.wc);

	nk.wnd = CreateWindowExW(exstyle, class_name, title, style,
		x, y, (int)width, (int)height,
		NULL, NULL, inst, NULL);

	SetLayeredWindowAttributes(nk.wnd, 0, nk.alpha, LWA_ALPHA);

	nk.ctx = nk_gdip_init(nk.wnd, nk.width, nk.height);

	nk.font = nk_gdip_load_font(font_name, nk.font_size);
	nk_gdip_set_font(nk.font);

	set_style(nk.ctx);
	for (WORD i = 0; i < ARRAY_SIZE(nk.image); i++)
		nk.image[i] = load_png(i + IDR_PNG_MIN);

	nk.progress_wnd = CreateDialogParamW(nk.inst, MAKEINTRESOURCE(IDD_PROG_DIALOG), nk.wnd, nkctx_proc_progress, 0);
	ShowWindow(nk.progress_wnd, SW_HIDE);

	grub_module_init();
	nk.path = NULL;
	nkctx_enum_disk();
}

void
nkctx_loop(void)
{
	nk.running = TRUE;
	nk.needs_refresh = TRUE;

	while (nk.running)
	{
		/* Input */
		MSG msg;
		nk_input_begin(nk.ctx);
		if (nk.needs_refresh == FALSE)
		{
			if (GetMessageW(&msg, NULL, 0, 0) <= 0)
				nk.running = FALSE;
			else
			{
				TranslateMessage(&msg);
				DispatchMessageW(&msg);
			}
			nk.needs_refresh = TRUE;
		}
		else
			nk.needs_refresh = FALSE;
		while (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				nk.running = FALSE;
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
			nk.needs_refresh = TRUE;
		}
		nk_input_end(nk.ctx);

		/* GUI */
		//set_style(nk.ctx);
		nkctx_main_window(nk.ctx, (float)nk.width, (float)nk.height);
		nkctx_hash_window(nk.ctx, (float)nk.width, (float)nk.height);
		nkctx_mount_window(nk.ctx, (float)nk.width, (float)nk.height);
		nkctx_disk_info_window(nk.ctx, (float)nk.width, (float)nk.height);
		nkctx_hex_window(nk.ctx, (float)nk.width, (float)nk.height);
		nkctx_image_window(nk.ctx, (float)nk.width, (float)nk.height);

		/* Draw */
		nk_gdip_render(NK_ANTI_ALIASING_ON, nk.table[NK_COLOR_WINDOW]);
	}
}

_Noreturn void
nkctx_fini(int code)
{
	for (WORD i = 0; i < ARRAY_SIZE(nk.image); i++)
		nk_gdip_image_free(nk.image[i]);
	nk_gdipfont_del(nk.font);
	nk_gdip_shutdown();
	UnregisterClassW(nk.wc.lpszClassName, nk.wc.hInstance);
	nkctx_free_file();
	nkctx_free_disk();
	nkctx_unset_path();
	grub_module_fini();
	free(nk.path);
	free(nk.copy_buf);
	CoUninitialize();
	exit(code);
}
