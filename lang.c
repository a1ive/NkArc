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
#include <lang.h>

static const char* lang_en_us[LANG_STRMAX] =
{
	u8"Up",
	u8"Refresh Files",
	u8"Extract Selected Files",
	u8"About",
	u8"[COMPUTER]",
	u8"Unmount Disk",
	u8"Disk Info",
	u8"Cancel",
	u8"Refresh Disks",
	u8"View Image",
	u8"Mount Disk",
	u8"Get Checksum",
	u8"Extract File",
	u8"Name",
	u8"Driver",
	u8"Size",
	u8"Parent",
	u8"Filesystem",
	u8"Label",
	u8"FS UUID",
	u8"UNKNOWN",
	u8"DIR",
	u8"CALC",
	u8"No decompress",
	u8"Mount",
};

static const wchar_t* langw_en_us[LANG_WCSMAX] =
{
	L"INFO",
	L"DONE",
	L"FAIL",
	L"Select target folder",
	L"ERROR",
	L"CANNOT OPEN FILE",
	L"FILE TOO LARGE",
	L"OUT OF MEMORY",
};

static const char* lang_zh_cn[LANG_STRMAX] =
{
	u8"向上",
	u8"刷新文件",
	u8"提取选中文件",
	u8"关于",
	u8"[计算机]",
	u8"卸载磁盘",
	u8"磁盘信息",
	u8"取消",
	u8"刷新磁盘",
	u8"查看图像",
	u8"挂载磁盘",
	u8"获取校验和",
	u8"提取文件",
	u8"名称",
	u8"驱动",
	u8"大小",
	u8"父级",
	u8"文件系统",
	u8"卷标",
	u8"文件系统 UUID",
	u8"未知",
	u8"文件夹",
	u8"计算",
	u8"不解压",
	u8"挂载",
};

static const wchar_t* langw_zh_cn[LANG_WCSMAX] =
{
	L"信息",
	L"完成",
	L"失败",
	L"选择目标文件夹",
	L"错误",
	L"无法打开文件",
	L"文件过大",
	L"内存不足",
};

static const char* lang_zh_tw[LANG_STRMAX] =
{
	u8"向上",
	u8"重新整理檔案",
	u8"提取選中檔案",
	u8"關於",
	u8"[計算機]",
	u8"解除安裝磁碟",
	u8"磁碟資訊",
	u8"取消",
	u8"重新整理磁碟",
	u8"檢視影象",
	u8"掛載磁碟",
	u8"獲取校驗和",
	u8"提取檔案",
};

static const wchar_t* langw_zh_tw[LANG_WCSMAX] =
{
	L"信息",
	L"完成",
	L"失敗",
	L"選擇目標資料夾",
};

const char*
GET_STR(int id)
{
	const char* text = NULL;
	switch (nk.lang)
	{
	case 2052: text = lang_zh_cn[id]; break;
	case 1028: text = lang_zh_tw[id]; break;
	}
	if (text == NULL)
		text = lang_en_us[id];
	return text;
}

const wchar_t*
GET_WCS(int id)
{
	const wchar_t* text = NULL;
	switch (nk.lang)
	{
	case 2052: text = langw_zh_cn[id]; break;
	case 1028: text = langw_zh_tw[id]; break;
	}
	if (text == NULL)
		text = langw_en_us[id];
	return text;
}
