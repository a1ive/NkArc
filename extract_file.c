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

#include <shlobj.h>
#include <shlobj_core.h>

#include <grub/types.h>
#include <grub/err.h>
#include <grub/misc.h>
#include <grub/file.h>
#include <grub/charset.h>

#define GRUB_MAX_PATH_LEN 4095

static WCHAR m_u16_buf[GRUB_MAX_PATH_LEN + 1];

static LPCWSTR
get_u16_path(LPCWSTR dir, const char* file)
{
	size_t len;
	wcscpy_s(m_u16_buf, GRUB_MAX_PATH_LEN - 1, dir);
	len = wcslen(m_u16_buf);
	if (len >= 1 && m_u16_buf[len - 1] != L'\\')
		m_u16_buf[len++] = L'\\';
	grub_utf8_to_utf16(m_u16_buf + len, GRUB_MAX_PATH_LEN - len, (const grub_uint8_t*)file, -1, NULL);
	return m_u16_buf;
}

BOOL
nkctx_extract_file(LPCWSTR target_dir, const char* source_file)
{
	BOOL ret = FALSE;
	HANDLE hf = INVALID_HANDLE_VALUE;
	grub_file_t file = NULL;
	const char* target_file = grub_strrchr(source_file, '/');
	if (!target_file)
		goto fail;
	target_file++;
	file = grub_file_open(source_file, GRUB_FILE_TYPE_CAT | GRUB_FILE_TYPE_NO_DECOMPRESS);
	if (!file)
		goto fail;

	hf = CreateFileW(get_u16_path(target_dir, target_file),
		GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hf == NULL || hf == INVALID_HANDLE_VALUE)
		goto fail;

	nkctx_show_progress();
	while (1)
	{
		DWORD w;
		grub_ssize_t r = grub_file_read(file, nk.copy_buf, nk.copy_size);
		if (r < 0)
			goto fail;
		if (r == 0)
			break;
		BOOL b = WriteFile(hf, nk.copy_buf, r, &w, NULL);
		if (!b || w != (DWORD)r)
			goto fail;
	}
	nkctx_hide_progress();
	ret = TRUE;

fail:
	if (hf != NULL && hf != INVALID_HANDLE_VALUE)
		CloseHandle(hf);
	if (file)
		grub_file_close(file);
	grub_errno = GRUB_ERR_NONE;
	return ret;
}

WCHAR*
nkctx_select_dir(void)
{
	LPMALLOC pMalloc = NULL;
	PIDLIST_ABSOLUTE pidl = NULL;
	BROWSEINFOW bi = { 0 };
	WCHAR* buf = calloc(MAX_PATH, sizeof(WCHAR));
	if (!buf)
		goto fail;

	bi.hwndOwner = nk.wnd;
	bi.pszDisplayName = m_u16_buf;
	bi.lpszTitle = L"Select target folder";
	bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_DONTGOBELOWDOMAIN | BIF_EDITBOX | BIF_NEWDIALOGSTYLE;

	pidl = SHBrowseForFolderW(&bi);
	if (pidl == NULL)
		goto fail;

	BOOL rc = SHGetPathFromIDListW(pidl, buf);

	if (SUCCEEDED(SHGetMalloc(&pMalloc)))
	{
		pMalloc->lpVtbl->Free(pMalloc, pidl);
		pMalloc->lpVtbl->Release(pMalloc);
	}

	if (rc == FALSE)
		goto fail;

	return buf;
fail:
	free(buf);
	return NULL;
}

