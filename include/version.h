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

#pragma once

#define QUOTE_(x) #x
#define QUOTE(x) QUOTE_(x)

#define NKGUI_MAJOR_VERSION 0
#define NKGUI_MINOR_VERSION 1
#define NKGUI_MICRO_VERSION 0
#define NKGUI_BUILD_VERSION 0

#define NKGUI_VERSION      NKGUI_MAJOR_VERSION,NKGUI_MINOR_VERSION,NKGUI_MICRO_VERSION,NKGUI_BUILD_VERSION
#define NKGUI_VERSION_STR  QUOTE(NKGUI_MAJOR_VERSION.NKGUI_MINOR_VERSION.NKGUI_MICRO_VERSION.NKGUI_BUILD_VERSION)

#define NKGUI_COMPANY      "A1ive"
#define NKGUI_COPYRIGHT    "Copyright (c) 2023 A1ive"
#define NKGUI_FILEDESC     "NkArc"

#define NKGUI_NAME         "NkArc"
