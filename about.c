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

#include <dokan.h>
#include <nkctx.h>
#include <lang.h>
#include "version.h"

#include <grub/misc.h>
#include <grub/fs.h>
#include <grub/partition.h>
#include <grub/diskfilter.h>

BOOL
nkctx_about_popup(struct nk_context* ctx, float width, float height)
{
	if (!nk_popup_begin(ctx, NK_POPUP_STATIC, GET_STR(LANG_STR_ABOUT),
		NK_WINDOW_CLOSABLE,
		nk_rect(width / 3.0f, height / 3.0f, width / 3.0f, height / 3.0f)))
		return FALSE;

	nk_layout_row_dynamic(ctx, 0, 1);
	nk_label(ctx, NKGUI_NAME, NK_TEXT_CENTERED);
	nk_label(ctx, NKGUI_COPYRIGHT, NK_TEXT_CENTERED);
	nk_label(ctx, "v"NKGUI_VERSION_STR, NK_TEXT_CENTERED);
	nk_label(ctx, "Build. " __DATE__ " " __TIME__, NK_TEXT_CENTERED);
	nk_labelf(ctx, NK_TEXT_CENTERED, "Dokan v%lu", DokanVersion());

	nk_label(ctx, "Supported Filesystems:", NK_TEXT_LEFT);
	grub_fs_t fs;
	FOR_FILESYSTEMS(fs)
	{
		nk_label(ctx, fs->name, NK_TEXT_RIGHT);
	}
	grub_partition_map_t partmap;
	nk_label(ctx, "Supported Partition Maps:", NK_TEXT_LEFT);
	FOR_PARTITION_MAPS(partmap)
	{
		nk_label(ctx, partmap->name, NK_TEXT_RIGHT);
	}
	grub_diskfilter_t df;
	nk_label(ctx, "Supported Disk Filters:", NK_TEXT_LEFT);
	FOR_LIST_ELEMENTS(df, grub_diskfilter_list)
	{
		nk_label(ctx, df->name, NK_TEXT_RIGHT);
	}

	nk_popup_end(ctx);
	return TRUE;
}
