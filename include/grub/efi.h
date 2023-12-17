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

#ifndef GRUB_EFI_HEADER
#define GRUB_EFI_HEADER	1

#include <windows.h>

#define EFI_VARIABLE_NON_VOLATILE                           0x00000001
#define EFI_VARIABLE_BOOTSERVICE_ACCESS                     0x00000002
#define EFI_VARIABLE_RUNTIME_ACCESS                         0x00000004
#define EFI_VARIABLE_HARDWARE_ERROR_RECORD                  0x00000008
#define EFI_VARIABLE_AUTHENTICATED_WRITE_ACCESS             0x00000010
#define EFI_VARIABLE_TIME_BASED_AUTHENTICATED_WRITE_ACCESS  0x00000020
#define EFI_VARIABLE_APPEND_WRITE                           0x00000040
#define EFI_VARIABLE_ENHANCED_AUTHENTICATED_ACCESS          0x00000080

typedef struct _VARIABLE_NAME
{
	ULONG NextEntryOffset;
	GUID VendorGuid;
	WCHAR Name[ANYSIZE_ARRAY];
} VARIABLE_NAME, *PVARIABLE_NAME;

GUID EFI_GV_GUID;
GUID EFI_EMPTY_GUID;

DWORD
grub_efi_get_variable(LPCWSTR name, LPGUID guid,
	PVOID buf, DWORD size, PDWORD attr);

VOID*
grub_efi_get_variable_alloc(const char* name, LPGUID guid, PDWORD size, PDWORD attr);

PVARIABLE_NAME
grub_efi_enum_variable(PULONG size);

BOOL
grub_is_efi_boot(VOID);

#endif /* ! GRUB_EFI_HEADER */
