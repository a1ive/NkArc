/* progress.c - show loading progress */
/*
 *  GRUB  --  GRand Unified Bootloader
 *  Copyright (C) 2013  Free Software Foundation, Inc.
 *
 *  GRUB is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  GRUB is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with GRUB.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <grub/types.h>
#include <grub/time.h>
#include <grub/file.h>
#include <grub/misc.h>
#include <grub/charset.h>

#include <nkctx.h>

GRUB_MOD_LICENSE("GPLv3+");

#define UPDATE_INTERVAL 800

static grub_err_t
grub_file_progress_hook_real(grub_disk_addr_t sector __attribute__((unused)),
	unsigned offset __attribute__((unused)),
	unsigned length,
	char* buf __attribute__((unused)), void* data)
{
	static int call_depth = 0;
	grub_uint64_t now;
	static grub_uint64_t last_progress_update_time;
	grub_file_t file = data;

	file->progress_offset += length;

	if (call_depth)
		return GRUB_ERR_NONE;

	if (!nk.progress)
		return GRUB_ERR_NONE;

	call_depth = 1;
	now = grub_get_time_ms();

	if (((now - last_progress_update_time > UPDATE_INTERVAL) &&
		(file->progress_offset - file->offset > 0)) ||
		(file->progress_offset == file->size))
	{
		const char* partial_file_name;

		unsigned long long percent;
		grub_uint64_t current_speed;

		if (now - file->last_progress_time < 10)
			current_speed = 0;
		else
			current_speed = grub_divmod64((file->progress_offset
				- file->last_progress_offset)
				* 100ULL * 1000ULL,
				now - file->last_progress_time, 0);

		if (file->size == 0)
			percent = 100;
		else
			percent = grub_divmod64(100 * file->progress_offset,
				file->size, 0);

		/* grub_net_fs_open() saves off partial file structure before name is initialized.
		   It already saves passed file name in net structure so just use it in this case.
		 */
		if (file->name) /* grub_file_open() may leave it as NULL */
			partial_file_name = grub_strrchr(file->name, '/');
		else
			partial_file_name = NULL;
		if (partial_file_name)
			partial_file_name++;
		else
			partial_file_name = "";

		file->estimated_speed = (file->estimated_speed + current_speed) >> 1;

		static char buffer[80];
		grub_snprintf(buffer, sizeof(buffer), "[ %.20s  %s  %llu%%  ",
			partial_file_name,
			grub_get_human_size(file->progress_offset, GRUB_HUMAN_SIZE_NORMAL),
			(unsigned long long) percent);
		char* ptr = buffer + grub_strlen(buffer);
		grub_snprintf(ptr, sizeof(buffer) - (ptr - buffer), "%s ]",
			grub_get_human_size(file->estimated_speed, GRUB_HUMAN_SIZE_SPEED));
		memset(nk.progress_info, 0, sizeof(nk.progress_info));
		grub_utf8_to_utf16(nk.progress_info, 95, (grub_uint8_t*)buffer, -1, NULL);
		SetWindowTextW(GetDlgItem(nk.progress_wnd, IDC_PROG_TEXT), nk.progress_info);

		file->last_progress_offset = file->progress_offset;
		file->last_progress_time = now;
		last_progress_update_time = now;
	}
	call_depth = 0;

	return GRUB_ERR_NONE;
}

GRUB_MOD_INIT(progress)
{
	grub_file_progress_hook = grub_file_progress_hook_real;
}

GRUB_MOD_FINI(progress)
{
	grub_file_progress_hook = 0;
}
