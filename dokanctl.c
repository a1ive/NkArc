/*
  Dokan : user-mode file system library for Windows

  Copyright (C) 2020 - 2022 Google, Inc.
  Copyright (C) 2015 - 2019 Adrien J. <liryna.stark@gmail.com> and Maxime C. <maxime@islog.com>
  Copyright (C) 2007 - 2011 Hiroki Asakawa <info@dokan-dev.net>

  http://dokan-dev.github.io

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#include <stdio.h>
#include <stdlib.h>

#include "dokan/dokan.h"
#include "dokan/dokanc.h"
#include <shlobj.h>
#include <pathcch.h>

BOOL
DokanCheckService(VOID)
{
	SC_HANDLE scm = NULL;
	SC_HANDLE svc = NULL;
	SERVICE_STATUS ss;
	BOOL rc = FALSE;

	scm = OpenSCManagerW(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (scm == NULL)
		goto out;
	svc = OpenServiceW(scm, DOKAN_DRIVER_SERVICE, SERVICE_QUERY_STATUS);
	if (svc == NULL)
		goto out;
	if (QueryServiceStatus(svc, &ss) == FALSE)
		goto out;
	if (ss.dwCurrentState == SERVICE_RUNNING)
		rc = TRUE;
out:
	if (svc)
		CloseServiceHandle(svc);
	if (scm)
		CloseServiceHandle(scm);
	return rc;
}
