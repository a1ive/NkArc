// SPDX-License-Identifier: Unlicense

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR

#pragma warning(disable:4996)
#pragma warning(disable:4244)
#pragma warning(disable:4116)

#include <assert.h>
#define NK_ASSERT(expr) assert(expr)

#define NK_MEMSET memset
#define NK_MEMCPY memcpy
#define NK_SIN sinf
#define NK_COS cosf
#define NK_STRTOD strtod

#define NK_IMPLEMENTATION
#include <nuklear.h>

#pragma warning(disable:4244)
#pragma comment(lib, "gdiplus.lib")
#pragma comment(lib, "shlwapi.lib")

#define NK_GDIP_IMPLEMENTATION
#include <nuklear_gdip.h>

#include <VersionHelpers.h>

GdipFont*
nk_gdip_load_font(LPCWSTR name, int size)
{
	GdipFont* font = (GdipFont*)calloc(1, sizeof(GdipFont));
	GpFontFamily* family;

	if (!font)
		goto fail;

	if (GdipCreateFontFamilyFromName(name, NULL, &family))
	{
		UINT len = IsWindowsVistaOrGreater() ? sizeof(NONCLIENTMETRICSW) : sizeof(NONCLIENTMETRICSW) - sizeof(int);
		NONCLIENTMETRICSW metrics = { .cbSize = len };
		if (SystemParametersInfoW(SPI_GETNONCLIENTMETRICS, len, &metrics, 0))
		{
			if (GdipCreateFontFamilyFromName(metrics.lfMessageFont.lfFaceName, NULL, &family))
				goto fail;
		}
		else
			goto fail;
	}

	GdipCreateFont(family, (REAL)size, FontStyleRegular, UnitPixel, &font->handle);
	GdipDeleteFontFamily(family);

	return font;
fail:
	MessageBoxW(NULL, L"Failed to load font", L"Error", MB_OK);
	exit(1);
}

static inline nk_bool
nk_hover_begin(struct nk_context* ctx, float width)
{
	int x, y, w, h;
	struct nk_window* win;
	const struct nk_input* in;
	struct nk_rect bounds;
	int ret;

	/* make sure that no nonblocking popup is currently active */
	win = ctx->current;
	in = &ctx->input;

	w = nk_iceilf(width);
	h = nk_iceilf(nk_null_rect.h);
	x = nk_ifloorf(in->mouse.pos.x + 1) - (int)win->layout->clip.x;
	if (x > (int)win->bounds.w / 2) x -= w;
	y = nk_ifloorf(in->mouse.pos.y + 1) - (int)win->layout->clip.y;

	bounds.x = (float)x;
	bounds.y = (float)y;
	bounds.w = (float)w;
	bounds.h = (float)h;

	ret = nk_popup_begin(ctx, NK_POPUP_DYNAMIC,
		"__##Tooltip##__", NK_WINDOW_NO_SCROLLBAR | NK_WINDOW_BORDER, bounds);
	if (ret) win->layout->flags &= ~(nk_flags)NK_WINDOW_ROM;
	win->popup.type = NK_PANEL_TOOLTIP;
	ctx->current->layout->type = NK_PANEL_TOOLTIP;
	return ret;
}

static inline void
nk_hover_end(struct nk_context* ctx)
{
	ctx->current->seq--;
	nk_popup_close(ctx);
	nk_popup_end(ctx);
}

static void
nk_hover_colored(struct nk_context* ctx, const char* text, int text_len, struct nk_color color)
{
	if (ctx->current == ctx->active // only show tooltip if the window is active
		// make sure that no nonblocking popup is currently active
		&& !(ctx->current->popup.win && (ctx->current->popup.type & NK_PANEL_SET_NONBLOCK)))
	{
		/* calculate size of the text and tooltip */
		float text_width = ctx->style.font->width(ctx->style.font->userdata, ctx->style.font->height, text, text_len)
			+ (4 * ctx->style.window.padding.x);
		float text_height = (ctx->style.font->height + 2 * ctx->style.window.padding.y);

		/* execute tooltip and fill with text */
		if (nk_hover_begin(ctx, text_width))
		{
			nk_layout_row_dynamic(ctx, text_height, 1);
			nk_text_colored(ctx, text, text_len, NK_TEXT_LEFT, color);
			nk_hover_end(ctx);
		}
	}
}

void
nk_image_label(struct nk_context* ctx, struct nk_image img,
	const char* str, nk_flags align, struct nk_color color)
{
	struct nk_window* win;
	const struct nk_style* style;
	struct nk_rect bounds;
	struct nk_rect icon;
	struct nk_text text;
	int len;

	NK_ASSERT(ctx);
	NK_ASSERT(ctx->current);
	NK_ASSERT(ctx->current->layout);
	if (!ctx || !ctx->current || !ctx->current->layout) return;

	win = ctx->current;
	style = &ctx->style;
	len = nk_strlen(str);
	if (!nk_widget(&bounds, ctx))
		return;

	icon.w = icon.h = bounds.h;
	icon.x = bounds.x;
	icon.y = bounds.y;

	nk_draw_image(&win->buffer, icon, &img, nk_white);

	bounds.x = icon.x + icon.w + style->window.padding.x + style->window.border;
	bounds.w -= icon.w + style->window.padding.x + style->window.border;

	text.padding.x = style->text.padding.x;
	text.padding.y = style->text.padding.y;
	text.background = style->window.background;
	text.text = color;
	nk_widget_text(&win->buffer, bounds, str, len, &text, align, style->font);

	if (nk_input_is_mouse_hovering_rect(&ctx->input, bounds))
		nk_hover_colored(ctx, str, len, color);
}

static void
nk_draw_hb_text_image(struct nk_command_buffer* out,
	const struct nk_rect* bounds, const struct nk_rect* label,
	const struct nk_rect* image, nk_flags state, const struct nk_style_button* style,
	const char* str, const struct nk_user_font* font,
	const struct nk_image* img)
{
	struct nk_text text;
	const struct nk_style_item* background;
	background = nk_draw_button(out, bounds, state, style);

	/* select correct colors */
	if (background->type == NK_STYLE_ITEM_COLOR)
		text.background = background->data.color;
	else text.background = style->text_background;
	if (state & NK_WIDGET_STATE_HOVER)
		text.text = style->text_hover;
	else if (state & NK_WIDGET_STATE_ACTIVED)
		text.text = style->text_active;
	else text.text = style->text_normal;

	text.padding = nk_vec2(0, 0);
	nk_widget_text(out, *label, str, nk_strlen(str), &text, NK_TEXT_LEFT, font);
	nk_draw_image(out, *image, img, nk_white);
}

static nk_bool
nk_do_hb_text_image(nk_flags* state,
	struct nk_command_buffer* out, struct nk_rect bounds,
	struct nk_image img, const char* str,
	enum nk_button_behavior behavior, const struct nk_style_button* style,
	const struct nk_user_font* font, const struct nk_input* in)
{
	int ret;
	struct nk_rect icon;
	struct nk_rect label;
	struct nk_rect content;

	NK_ASSERT(style);
	NK_ASSERT(state);
	NK_ASSERT(font);
	NK_ASSERT(out);
	if (!out || !font || !style || !str)
		return nk_false;

	ret = nk_do_button(state, out, bounds, style, in, behavior, &content);
	icon.y = bounds.y + style->padding.y;
	icon.w = icon.h = bounds.h - 2 * style->padding.y;
	icon.x = bounds.x + 2 * style->padding.x;

	icon.x += style->image_padding.x;
	icon.y += style->image_padding.y;
	icon.w -= 2 * style->image_padding.x;
	icon.h -= 2 * style->image_padding.y;

	label.x = content.x + icon.w;
	label.y = content.y;
	label.w = content.w - icon.w;
	label.h = content.h;

	if (style->draw_begin) style->draw_begin(out, style->userdata);
	nk_draw_hb_text_image(out, &bounds, &label, &icon, *state, style, str, font, &img);
	if (style->draw_end) style->draw_end(out, style->userdata);
	return ret;
}

nk_bool
nk_hb_image_label_styled(struct nk_context* ctx,
	const struct nk_style_button* style, struct nk_image img,
	const char* str, const char* hover)
{
	struct nk_window* win;
	struct nk_panel* layout;
	const struct nk_input* in;

	struct nk_rect bounds;
	enum nk_widget_layout_states state;

	NK_ASSERT(ctx);
	NK_ASSERT(ctx->current);
	NK_ASSERT(ctx->current->layout);
	if (!ctx || !ctx->current || !ctx->current->layout)
		return 0;

	win = ctx->current;
	layout = win->layout;

	state = nk_widget(&bounds, ctx);
	if (!state) return 0;
	in = (state == NK_WIDGET_ROM || layout->flags & NK_WINDOW_ROM) ? 0 : &ctx->input;

	if (nk_input_is_mouse_hovering_rect(in, bounds) && hover)
		nk_hover_colored(ctx, hover, nk_strlen(hover), ctx->style.text.color);

	return nk_do_hb_text_image(&ctx->last_widget_state, &win->buffer,
		bounds, img, str, ctx->button_behavior,
		style, ctx->style.font, in);
}

nk_bool
nk_hb_image_label(struct nk_context* ctx, struct nk_image img,
	const char* label, const char* hover)
{
	return nk_hb_image_label_styled(ctx, &ctx->style.button, img, label, hover);
}

nk_bool
nk_hb_image(struct nk_context* ctx, struct nk_image img, const char* hover)
{
	struct nk_window* win;
	struct nk_panel* layout;
	const struct nk_input* in;

	struct nk_rect bounds;
	enum nk_widget_layout_states state;

	NK_ASSERT(ctx);
	NK_ASSERT(ctx->current);
	NK_ASSERT(ctx->current->layout);
	if (!ctx || !ctx->current || !ctx->current->layout)
		return 0;

	win = ctx->current;
	layout = win->layout;

	state = nk_widget(&bounds, ctx);
	if (!state) return 0;
	in = (state == NK_WIDGET_ROM || layout->flags & NK_WINDOW_ROM) ? 0 : &ctx->input;

	if (nk_input_is_mouse_hovering_rect(in, bounds) && hover)
		nk_hover_colored(ctx, hover, nk_strlen(hover), ctx->style.text.color);

	return nk_do_button_image(&ctx->last_widget_state, &win->buffer, bounds,
		img, ctx->button_behavior, &ctx->style.button, in);
}

nk_bool
nk_hc_begin(struct nk_context* ctx, float width, float height,
	struct nk_rect trigger_bounds)
{
	struct nk_window* win;
	struct nk_window* popup;
	struct nk_rect body;

	NK_STORAGE const struct nk_rect null_rect = { -1,-1,0,0 };
	int is_clicked = 0;
	int is_open = 0;
	int ret = 0;

	NK_ASSERT(ctx);
	NK_ASSERT(ctx->current);
	NK_ASSERT(ctx->current->layout);
	if (!ctx || !ctx->current || !ctx->current->layout)
		return 0;

	win = ctx->current;
	++win->popup.con_count;
	if (ctx->current != ctx->active)
		return 0;

	/* check if currently active contextual is active */
	popup = win->popup.win;
	is_open = (popup && win->popup.type == NK_PANEL_CONTEXTUAL);
	is_clicked = nk_input_mouse_clicked(&ctx->input, NK_BUTTON_RIGHT, trigger_bounds);
	if (win->popup.active_con && win->popup.con_count != win->popup.active_con)
		return 0;
	if (!is_open && win->popup.active_con)
		win->popup.active_con = 0;
	if ((!is_open && !is_clicked))
		return 0;

	/* calculate contextual position on click */
	win->popup.active_con = win->popup.con_count;
	body.w = width;
	body.h = height;
	if (is_clicked)
	{
		body.x = ctx->input.mouse.pos.x;
		if (body.x + body.w > win->bounds.w)
			body.x -= body.w * 0.95f;
		body.y = ctx->input.mouse.pos.y;
		if (body.y + body.h > win->bounds.h)
			body.y = win->bounds.h - body.h;
	}
	else
	{
		body.x = popup->bounds.x;
		body.y = popup->bounds.y;
	}

	/* start nonblocking contextual popup */
	ret = nk_nonblock_begin(ctx, NK_WINDOW_NO_SCROLLBAR, body,
		null_rect, NK_PANEL_CONTEXTUAL);
	if (ret) win->popup.type = NK_PANEL_CONTEXTUAL;
	else {
		win->popup.active_con = 0;
		win->popup.type = NK_PANEL_NONE;
		if (win->popup.win)
			win->popup.win->flags = 0;
	}
#if 1
	if (!nk_input_is_mouse_hovering_rect(&ctx->input, body))
	{
		nk_contextual_close(ctx);
		nk_contextual_end(ctx);
		ret = 0;
	}
#endif
	return ret;
}
