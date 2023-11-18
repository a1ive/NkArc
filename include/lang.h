#pragma once

enum
{
	LANG_STR_UP = 0,
	LANG_STR_REFRESH_FILES,
	LANG_STR_THIS_PC,
	LANG_STR_UMOUNT_DISK,
	LANG_STR_DISK_INFO,
	LANG_STR_CANCEL,
	LANG_STR_REFRESH_DISKS,
	LANG_STR_VIEW_IMAGE,
	LANG_STR_MOUNT_DISK,
	LANG_STR_GET_CHECKSUM,
	LANG_STR_EXTRACT_FILE,
	LANG_STR_NAME,
	LANG_STR_DRIVER,
	LANG_STR_SIZE,
	LANG_STR_PARENT,
	LANG_STR_FS,
	LANG_STR_VOL_LABEL,
	LANG_STR_FS_UUID,
	LANG_STR_UNKNOWN,
	LANG_STR_DIR,
	LANG_STR_CALC,
	LANG_STR_NO_DECOMP,
	LANG_STR_MOUNT,

	LANG_STRMAX
};

const char* GET_STR(int id);

enum
{
	LANG_WCS_INFO = 0,
	LANG_WCS_DONE,
	LANG_WCS_FAIL,
	LANG_WCS_SELECT_DIR,
	LANG_WCS_ERROR,
	LANG_WCS_CANNOT_OPEN_FILE,
	LANG_WCS_FILE_TOO_LARGE,
	LANG_WCS_OUT_OF_MEM,

	LANG_WCSMAX
};

const wchar_t* GET_WCS(int id);
