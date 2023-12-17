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

#include <grub/disk.h>
#include <grub/fs.h>
#include <grub/mm.h>
#include <grub/charset.h>
#include <grub/procfs.h>
#include <grub/efi.h>

GRUB_MOD_LICENSE("GPLv3+");

struct efivars_data
{
	DWORD size;
	char* name;
	void* data;
};

static struct grub_procfs_entry* efivars;
static grub_size_t efivars_count;

static char* efivars_get_contents (struct grub_procfs_entry* this, grub_size_t* sz)
{
	struct efivars_data* data = this->data;
	void* buf = grub_malloc(data->size);
	*sz = 0;
	if (!buf)
		return NULL;
	grub_memcpy(buf, data->data, data->size);
	*sz = data->size;
	return buf;
}

GRUB_MOD_INIT(efivars)
{
	efivars = NULL;
	efivars_count = 0;
	if (!grub_is_efi_boot())
		return;
	ULONG ptr_size = 0;
	grub_size_t i;
	PVARIABLE_NAME p;
	PVARIABLE_NAME data = grub_efi_enum_variable(&ptr_size);
	if (!data)
		return;
	for (PVARIABLE_NAME p = data;
		p->NextEntryOffset && ((LPBYTE)p < (LPBYTE)data + ptr_size);
		p = (PVARIABLE_NAME)((LPBYTE)p + p->NextEntryOffset))
	{
		efivars_count++;
	}
	efivars = grub_calloc(efivars_count, sizeof(struct grub_procfs_entry));
	for (p = data, i = 0;
		p->NextEntryOffset && ((LPBYTE)p < (LPBYTE)data + ptr_size);
		p = (PVARIABLE_NAME)((LPBYTE)p + p->NextEntryOffset), i++)
	{
		grub_size_t len = wcslen(p->Name) + 1;
		char* name = grub_calloc(4, len);
		*grub_utf16_to_utf8((grub_uint8_t*)name, p->Name, len) = 0;
		struct efivars_data* ctx = grub_malloc(sizeof(struct efivars_data));
		ctx->name = grub_xasprintf("efi/{%08lX-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}/%s",
			p->VendorGuid.Data1, p->VendorGuid.Data2, p->VendorGuid.Data3,
			p->VendorGuid.Data4[0], p->VendorGuid.Data4[1], p->VendorGuid.Data4[2], p->VendorGuid.Data4[3],
			p->VendorGuid.Data4[4], p->VendorGuid.Data4[5], p->VendorGuid.Data4[6], p->VendorGuid.Data4[7],
			name);
		grub_free(name);
		ctx->data = grub_efi_get_variable_alloc(name, &p->VendorGuid, &ctx->size, NULL);
		efivars[i].name = ctx->name;
		efivars[i].data = ctx;
		efivars[i].get_contents = efivars_get_contents;
		grub_procfs_register(efivars[i].name, &efivars[i]);
	}
}

GRUB_MOD_FINI(efivars)
{
	for (grub_size_t i = 0; i < efivars_count; i++)
	{
		grub_procfs_unregister(&efivars[i]);
		struct efivars_data* ctx = efivars[i].data;
		grub_free(ctx->name);
		grub_free(ctx->data);
	}
	grub_free(efivars);
	efivars = NULL;
	efivars_count = 0;
}
