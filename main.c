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

static DWORD
obtain_privileges(LPWSTR privilege)
{
	HANDLE hToken;
	TOKEN_PRIVILEGES tkp = { 0 };
	BOOL res;
	// Obtain required privileges
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
		return GetLastError();
	res = LookupPrivilegeValueW(NULL, privilege, &tkp.Privileges[0].Luid);
	if (!res)
		return GetLastError();
	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0);
	return GetLastError();
}

int APIENTRY
wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR lpCmdLine,
	_In_ int nCmdShow)
{
	obtain_privileges(SE_SYSTEM_ENVIRONMENT_NAME);
	nkctx_init(hInstance,
		100, 100, 900, 600,
		L"NkWindowClass", L"Demo",
		L"Courier New", 16);
	nkctx_loop();
	nkctx_fini(0);
	return 0;
}
