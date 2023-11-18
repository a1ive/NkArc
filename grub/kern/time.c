/* time.c - kernel time functions */
/*
 *  GRUB  --  GRand Unified Bootloader
 *  Copyright (C) 2008  Free Software Foundation, Inc.
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

#include <grub/datetime.h>
#include <grub/time.h>
#include <time.h>

#include <windows.h>

grub_err_t
grub_get_datetime(struct grub_datetime* datetime)
{
	struct tm* mytm;
	time_t mytime;

	mytime = time(&mytime);
	if (gmtime_s(mytm, &mytime) == 0)
	{
		datetime->year = mytm->tm_year + 1900;
		datetime->month = mytm->tm_mon + 1;
		datetime->day = mytm->tm_mday;
		datetime->hour = mytm->tm_hour;
		datetime->minute = mytm->tm_min;
		datetime->second = mytm->tm_sec;
	}
	else
		memset(datetime, 0, sizeof(*datetime));

	return GRUB_ERR_NONE;
}

grub_err_t
grub_set_datetime(struct grub_datetime* datetime __attribute__((unused)))
{
	return grub_error(GRUB_ERR_NOT_IMPLEMENTED_YET,
		"no clock setting routine available");
}

grub_uint64_t
grub_get_time_ms(void)
{
	FILETIME cr, ex, ke, us;
	ULARGE_INTEGER us_ul;

	GetProcessTimes(GetCurrentProcess(), &cr, &ex, &ke, &us);
	us_ul.LowPart = us.dwLowDateTime;
	us_ul.HighPart = us.dwHighDateTime;

	return us_ul.QuadPart / 10000;
}

void
grub_millisleep(grub_uint32_t ms)
{
	Sleep(ms);
}
