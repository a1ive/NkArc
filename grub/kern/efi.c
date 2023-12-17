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

#include <grub/types.h>
#include <grub/misc.h>
#include <grub/mm.h>
#include <grub/efi.h>

#include <windows.h>

typedef struct _UNICODE_STRING
{
	USHORT Length;
	USHORT MaximumLength;
	PWSTR Buffer;
} UNICODE_STRING, *PUNICODE_STRING;

typedef enum _SYSTEM_ENVIRONMENT_INFORMATION_CLASS
{
	SystemEnvironmentNameInformation = 1, // q: VARIABLE_NAME
	SystemEnvironmentValueInformation = 2, // q: VARIABLE_NAME_AND_VALUE
	MaxSystemEnvironmentInfoClass
} SYSTEM_ENVIRONMENT_INFORMATION_CLASS;

#define NT_SUCCESS(Status) (((NTSTATUS)(Status)) >= 0)

GUID EFI_GV_GUID = { 0x8BE4DF61UL, 0x93CA, 0x11D2, { 0xAA, 0x0D, 0x00, 0xE0, 0x98, 0x03, 0x2B, 0x8C } };
GUID EFI_EMPTY_GUID = { 0 };

DWORD
grub_efi_get_variable(LPCWSTR name, LPGUID guid,
	PVOID buf, DWORD size, PDWORD attr)
{
	NTSTATUS rc;
	UNICODE_STRING str;
	ULONG(NTAPI * OsRtlNtStatusToDosError)(NTSTATUS) = NULL;
	VOID(NTAPI * OsRtlInitUnicodeString)(PUNICODE_STRING, PCWSTR) = NULL;
	NTSTATUS(NTAPI * OsQuerySystemEnvironmentValueEx)(PUNICODE_STRING, LPGUID, PVOID, PULONG, PULONG) = NULL;
	HMODULE mod = GetModuleHandleW(L"ntdll");
	if (!mod)
		goto fail;
	*(FARPROC*)&OsRtlNtStatusToDosError = GetProcAddress(mod, "RtlNtStatusToDosError");
	if (!OsRtlNtStatusToDosError)
		goto fail;
	*(FARPROC*)&OsRtlInitUnicodeString = GetProcAddress(mod, "RtlInitUnicodeString");
	if (!OsRtlInitUnicodeString)
		goto fail;
	*(FARPROC*)&OsQuerySystemEnvironmentValueEx = GetProcAddress(mod, "NtQuerySystemEnvironmentValueEx");
	if (!OsQuerySystemEnvironmentValueEx)
		goto fail;
	OsRtlInitUnicodeString(&str, name);
	if (!guid)
		guid = &EFI_GV_GUID;
	rc = OsQuerySystemEnvironmentValueEx(&str, guid, buf, &size, attr);
	SetLastError(OsRtlNtStatusToDosError(rc));
	return size;
fail:
	SetLastError(ERROR_INVALID_FUNCTION);
	return 0;
}

VOID*
grub_efi_get_variable_alloc(const char* name, LPGUID guid, PDWORD size, PDWORD attr)
{
	DWORD n_size = 0;
	PVOID buf = NULL;
	WCHAR* name16 = NULL;
	*size = 0;
	if (grub_utf8_to_utf16_alloc(name, &name16, NULL) < 0)
		return NULL;
	n_size = grub_efi_get_variable(name16, guid, NULL, 0, attr);
	if (!n_size)
	{
		grub_free(name16);
		return NULL;
	}
	buf = grub_calloc(n_size, 1);
	if (!buf)
	{
		grub_free(name16);
		return NULL;
	}
	if (grub_efi_get_variable(name16, guid, buf, n_size, attr) != n_size)
	{
		grub_free(name16);
		grub_free(buf);
		return NULL;
	}
	*size = n_size;
	grub_free(name16);
	return buf;
}

static BOOL
efi_var_iter(ULONG info_class, PVOID buf, PULONG len)
{
	NTSTATUS rc;
	NTSTATUS(NTAPI * OsEnumerateSystemEnvironmentValuesEx)(ULONG, PVOID, PULONG) = NULL;
	HMODULE mod = GetModuleHandleW(L"ntdll");
	if (!mod)
		goto fail;
	*(FARPROC*)&OsEnumerateSystemEnvironmentValuesEx = GetProcAddress(mod, "NtEnumerateSystemEnvironmentValuesEx");
	if (!OsEnumerateSystemEnvironmentValuesEx)
		goto fail;
	rc = OsEnumerateSystemEnvironmentValuesEx(info_class, buf, len);
	return NT_SUCCESS(rc);
fail:
	*len = 0;
	return FALSE;
}

PVARIABLE_NAME
grub_efi_enum_variable(PULONG size)
{
	PVOID name = NULL;
	efi_var_iter(SystemEnvironmentNameInformation, NULL, size);
	if (*size == 0)
		goto fail;
	name = grub_calloc(*size, 1);
	if (!name)
		goto fail;
	if (!efi_var_iter(SystemEnvironmentNameInformation, name, size))
		goto fail;
	return name;
fail:
	if (name)
		grub_free(name);
	*size = 0;
	return NULL;
}

BOOL
grub_is_efi_boot(VOID)
{
	grub_efi_get_variable(L"", &EFI_EMPTY_GUID, NULL, 0, NULL);
	return (GetLastError() != ERROR_INVALID_FUNCTION);
}
