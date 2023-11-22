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
#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <time.h>

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR

#include <assert.h>
#define NK_ASSERT(expr) assert(expr)

#include <nuklear.h>
#include <nuklear_gdip.h>

#include "resource.h"

GdipFont*
nk_gdip_load_font(LPCWSTR name, int size);

void
nk_image_label(struct nk_context* ctx, struct nk_image img,
	const char* str, nk_flags align, struct nk_color color);

nk_bool
nk_hb_image_label_styled(struct nk_context* ctx,
	const struct nk_style_button* style, struct nk_image img,
	const char* str, const char* hover);

nk_bool
nk_hb_image_label(struct nk_context* ctx, struct nk_image img,
	const char* label, const char* hover);

nk_bool
nk_hb_image(struct nk_context* ctx, struct nk_image img, const char* hover);

nk_bool
nk_hc_begin(struct nk_context* ctx, float width, float height,
	struct nk_rect trigger_bounds);

#define NK_COLOR_YELLOW     {0xFF, 0xEA, 0x00, 0xff}
#define NK_COLOR_RED        {0xFF, 0x17, 0x44, 0xff}
#define NK_COLOR_GREEN      {0x00, 0xE6, 0x76, 0xff}
#define NK_COLOR_CYAN       {0x03, 0xDA, 0xC6, 0xff}
#define NK_COLOR_BLUE       {0x29, 0x79, 0xFF, 0xff}
#define NK_COLOR_WHITE      {0xFF, 0xFF, 0xFF, 0xff}
#define NK_COLOR_BLACK      {0x00, 0x00, 0x00, 0xff}
#define NK_COLOR_GRAY       {0x1E, 0x1E, 0x1E, 0xff}
#define NK_COLOR_LIGHT      {0xBF, 0xBF, 0xBF, 0xff}
#define NK_COLOR_DARK       {0x2D, 0x2D, 0x2D, 0xFF}

struct nkctx_disk
{
	struct nkctx_disk* next;
	char* name;
	char* path;
	char desc[256];
	DWORD index;
	BOOL is_partition;
	BOOL is_loopback;
	UINT64 size;
	const char* fs;
	char* label;
	WORD icon;
};

enum nkctx_file_type
{
	NKCTX_FILE_FILE = 0,
	NKCTX_FILE_IMAGE,
	NKCTX_FILE_TEXT,
};

struct nkctx_file
{
	char* name;
	char* path;
	char human_size[12];
	char time[20];
	BOOL is_dir;
	BOOL selected;
	UINT64 size;
	WORD icon;
	enum nkctx_file_type type;
};

typedef struct _NK_GUI_CTX
{
	HINSTANCE inst;
	HWND wnd;
	WNDCLASSW wc;
	BOOL needs_refresh;
	BOOL running;
	GdipFont* font;
	struct nk_context* ctx;
	unsigned width;
	unsigned height;
	BYTE alpha;
	LANGID lang;
	unsigned font_size;
	float title_height;
	float gui_ratio;
	struct nk_color table[NK_COLOR_COUNT];
	struct nk_style_button style_button;

	struct nk_image image[IDR_PNG_MAX - IDR_PNG_MIN];

	char status[512];

	BOOL progress;
	WCHAR progress_info[96];
	HWND progress_wnd;

	char* path;
	DWORD disk_count;
	DWORD disk_index;
	struct nkctx_disk* disks;
	DWORD file_count;
	DWORD dir_count;
	struct nkctx_file* files;

	DWORD copy_size;
	char* copy_buf;
} NK_GUI_CTX;
extern NK_GUI_CTX nk;

#define GET_PNG(x) nk.image[x - IDR_PNG_MIN]

void
nkctx_init(HINSTANCE inst,
	int x, int y, unsigned width, unsigned height,
	LPCWSTR class_name, LPCWSTR title,
	LPCWSTR font_name, int font_size);

void
nkctx_loop(void);

_Noreturn void
nkctx_fini(int code);

void
nkctx_set_path(const char* path);

void
nkctx_unset_path(void);

void
nkctx_enum_disk(void);

void
nkctx_free_disk(void);

void
nkctx_enum_file(const char* dir);

void
nkctx_free_file(void);

BOOL
nkctx_is_hidden_file(const char* filename);

struct nkctx_window
{
	void (*init)(const char* path);
	void (*fini)(void);
	void (*window)(struct nk_context* ctx, float width, float height);
};

extern struct nkctx_window nkctx_hash;
extern struct nkctx_window nkctx_mount;
extern struct nkctx_window nkctx_disk_info;
extern struct nkctx_window nkctx_hex;
extern struct nkctx_window nkctx_image;

void
nkctx_main_window(struct nk_context* ctx, float width, float height);

BOOL
nkctx_about_popup(struct nk_context* ctx, float width, float height);

void
nkctx_show_progress(void);

void
nkctx_hide_progress(void);

BOOL
nkctx_extract_file(LPCWSTR target_dir, const char* source_file);

void
nkctx_extract_dir(LPCWSTR target_dir);

WCHAR*
nkctx_select_dir(void);
