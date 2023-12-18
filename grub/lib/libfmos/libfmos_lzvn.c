/*
 * LZVN (un)compression functions
 *
 * Copyright (C) 2019-2023, Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <libyal_wrap.h>

#include "libfmos_lzvn.h"

enum LIBFMOS_LZVN_OPPCODE_TYPES
{
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_LARGE,
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_MEDIUM,
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_PREVIOUS,
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,
	LIBFMOS_LZVN_OPPCODE_TYPE_END_OF_STREAM,
	LIBFMOS_LZVN_OPPCODE_TYPE_INVALID,
	LIBFMOS_LZVN_OPPCODE_TYPE_LITERAL_LARGE,
	LIBFMOS_LZVN_OPPCODE_TYPE_LITERAL_SMALL,
	LIBFMOS_LZVN_OPPCODE_TYPE_MATCH_LARGE,
	LIBFMOS_LZVN_OPPCODE_TYPE_MATCH_SMALL,
	LIBFMOS_LZVN_OPPCODE_TYPE_NONE,
};

/* Lookup table to map an oppcode to its type
 */
uint8_t libfmos_lzvn_oppcode_types[ 256 ] = {
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x00 */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x01 */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x02 */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x03 */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x04 */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x05 */
	LIBFMOS_LZVN_OPPCODE_TYPE_END_OF_STREAM,	/* 0x06 */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_LARGE,	/* 0x07 */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x08 */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x09 */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x0a */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x0b */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x0c */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x0d */
	LIBFMOS_LZVN_OPPCODE_TYPE_NONE,			/* 0x0e */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_LARGE,	/* 0x0f */

	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x10 */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x11 */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x12 */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x13 */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x14 */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x15 */
	LIBFMOS_LZVN_OPPCODE_TYPE_NONE,			/* 0x16 */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_LARGE,	/* 0x17 */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x18 */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x19 */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x1a */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x1b */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x1c */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x1d */
	LIBFMOS_LZVN_OPPCODE_TYPE_INVALID,		/* 0x1e */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_LARGE,	/* 0x1f */

	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x20 */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x21 */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x22 */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x23 */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x24 */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x25 */
	LIBFMOS_LZVN_OPPCODE_TYPE_INVALID,		/* 0x26 */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_LARGE,	/* 0x27 */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x28 */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x29 */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x2a */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x2b */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x2c */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x2d */
	LIBFMOS_LZVN_OPPCODE_TYPE_INVALID,		/* 0x2e */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_LARGE,	/* 0x2f */

	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x30 */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x31 */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x32 */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x33 */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x34 */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x35 */
	LIBFMOS_LZVN_OPPCODE_TYPE_INVALID,		/* 0x36 */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_LARGE,	/* 0x37 */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x38 */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x39 */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x3a */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x3b */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x3c */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x3d */
	LIBFMOS_LZVN_OPPCODE_TYPE_INVALID,		/* 0x3e */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_LARGE,	/* 0x3f */

	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x40 */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x41 */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x42 */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x43 */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x44 */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x45 */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_PREVIOUS,	/* 0x46 */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_LARGE,	/* 0x47 */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x48 */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x49 */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x4a */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x4b */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x4c */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x4d */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_PREVIOUS,	/* 0x4e */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_LARGE,	/* 0x4f */

	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x50 */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x51 */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x52 */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x53 */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x54 */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x55 */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_PREVIOUS,	/* 0x56 */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_LARGE,	/* 0x57 */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x58 */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x59 */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x5a */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x5b */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x5c */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x5d */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_PREVIOUS,	/* 0x5e */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_LARGE,	/* 0x5f */

	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x60 */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x61 */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x62 */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x63 */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x64 */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x65 */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_PREVIOUS,	/* 0x66 */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_LARGE,	/* 0x67 */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x68 */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x69 */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x6a */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x6b */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x6c */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x6d */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_PREVIOUS,	/* 0x6e */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_LARGE,	/* 0x6f */

	LIBFMOS_LZVN_OPPCODE_TYPE_INVALID,		/* 0x70 */
	LIBFMOS_LZVN_OPPCODE_TYPE_INVALID,		/* 0x71 */
	LIBFMOS_LZVN_OPPCODE_TYPE_INVALID,		/* 0x72 */
	LIBFMOS_LZVN_OPPCODE_TYPE_INVALID,		/* 0x73 */
	LIBFMOS_LZVN_OPPCODE_TYPE_INVALID,		/* 0x74 */
	LIBFMOS_LZVN_OPPCODE_TYPE_INVALID,		/* 0x75 */
	LIBFMOS_LZVN_OPPCODE_TYPE_INVALID,		/* 0x76 */
	LIBFMOS_LZVN_OPPCODE_TYPE_INVALID,		/* 0x77 */
	LIBFMOS_LZVN_OPPCODE_TYPE_INVALID,		/* 0x78 */
	LIBFMOS_LZVN_OPPCODE_TYPE_INVALID,		/* 0x79 */
	LIBFMOS_LZVN_OPPCODE_TYPE_INVALID,		/* 0x7a */
	LIBFMOS_LZVN_OPPCODE_TYPE_INVALID,		/* 0x7b */
	LIBFMOS_LZVN_OPPCODE_TYPE_INVALID,		/* 0x7c */
	LIBFMOS_LZVN_OPPCODE_TYPE_INVALID,		/* 0x7d */
	LIBFMOS_LZVN_OPPCODE_TYPE_INVALID,		/* 0x7e */
	LIBFMOS_LZVN_OPPCODE_TYPE_INVALID,		/* 0x7f */

	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x80 */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x81 */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x82 */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x83 */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x84 */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x85 */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_PREVIOUS,	/* 0x86 */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_LARGE,	/* 0x87 */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x88 */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x89 */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x8a */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x8b */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x8c */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x8d */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_PREVIOUS,	/* 0x8e */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_LARGE,	/* 0x8f */

	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x90 */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x91 */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x92 */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x93 */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x94 */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x95 */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_PREVIOUS,	/* 0x96 */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_LARGE,	/* 0x97 */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x98 */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x99 */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x9a */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x9b */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x9c */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x9d */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_PREVIOUS,	/* 0x9e */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_LARGE,	/* 0x9f */

	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_MEDIUM,	/* 0xa0 */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_MEDIUM,	/* 0xa1 */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_MEDIUM,	/* 0xa2 */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_MEDIUM,	/* 0xa3 */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_MEDIUM,	/* 0xa4 */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_MEDIUM,	/* 0xa5 */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_MEDIUM,	/* 0xa6 */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_MEDIUM,	/* 0xa7 */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_MEDIUM,	/* 0xa8 */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_MEDIUM,	/* 0xa9 */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_MEDIUM,	/* 0xaa */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_MEDIUM,	/* 0xab */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_MEDIUM,	/* 0xac */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_MEDIUM,	/* 0xad */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_MEDIUM,	/* 0xae */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_MEDIUM,	/* 0xaf */

	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_MEDIUM,	/* 0xb0 */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_MEDIUM,	/* 0xb1 */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_MEDIUM,	/* 0xb2 */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_MEDIUM,	/* 0xb3 */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_MEDIUM,	/* 0xb4 */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_MEDIUM,	/* 0xb5 */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_MEDIUM,	/* 0xb6 */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_MEDIUM,	/* 0xb7 */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_MEDIUM,	/* 0xb8 */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_MEDIUM,	/* 0xb9 */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_MEDIUM,	/* 0xba */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_MEDIUM,	/* 0xbb */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_MEDIUM,	/* 0xbc */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_MEDIUM,	/* 0xbd */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_MEDIUM,	/* 0xbe */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_MEDIUM,	/* 0xbf */

	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0xc0 */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0xc1 */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0xc2 */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0xc3 */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0xc4 */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0xc5 */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_PREVIOUS,	/* 0xc6 */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_LARGE,	/* 0xc7 */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0xc8 */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0xc9 */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0xca */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0xcb */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0xcc */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0xcd */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_PREVIOUS,	/* 0xce */
	LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_LARGE,	/* 0xcf */

	LIBFMOS_LZVN_OPPCODE_TYPE_INVALID,		/* 0xd0 */
	LIBFMOS_LZVN_OPPCODE_TYPE_INVALID,		/* 0xd1 */
	LIBFMOS_LZVN_OPPCODE_TYPE_INVALID,		/* 0xd2 */
	LIBFMOS_LZVN_OPPCODE_TYPE_INVALID,		/* 0xd3 */
	LIBFMOS_LZVN_OPPCODE_TYPE_INVALID,		/* 0xd4 */
	LIBFMOS_LZVN_OPPCODE_TYPE_INVALID,		/* 0xd5 */
	LIBFMOS_LZVN_OPPCODE_TYPE_INVALID,		/* 0xd6 */
	LIBFMOS_LZVN_OPPCODE_TYPE_INVALID,		/* 0xd7 */
	LIBFMOS_LZVN_OPPCODE_TYPE_INVALID,		/* 0xd8 */
	LIBFMOS_LZVN_OPPCODE_TYPE_INVALID,		/* 0xd9 */
	LIBFMOS_LZVN_OPPCODE_TYPE_INVALID,		/* 0xda */
	LIBFMOS_LZVN_OPPCODE_TYPE_INVALID,		/* 0xdb */
	LIBFMOS_LZVN_OPPCODE_TYPE_INVALID,		/* 0xdc */
	LIBFMOS_LZVN_OPPCODE_TYPE_INVALID,		/* 0xdd */
	LIBFMOS_LZVN_OPPCODE_TYPE_INVALID,		/* 0xde */
	LIBFMOS_LZVN_OPPCODE_TYPE_INVALID,		/* 0xdf */

	LIBFMOS_LZVN_OPPCODE_TYPE_LITERAL_LARGE,	/* 0xe0 */
	LIBFMOS_LZVN_OPPCODE_TYPE_LITERAL_SMALL,	/* 0xe1 */
	LIBFMOS_LZVN_OPPCODE_TYPE_LITERAL_SMALL,	/* 0xe2 */
	LIBFMOS_LZVN_OPPCODE_TYPE_LITERAL_SMALL,	/* 0xe3 */
	LIBFMOS_LZVN_OPPCODE_TYPE_LITERAL_SMALL,	/* 0xe4 */
	LIBFMOS_LZVN_OPPCODE_TYPE_LITERAL_SMALL,	/* 0xe5 */
	LIBFMOS_LZVN_OPPCODE_TYPE_LITERAL_SMALL,	/* 0xe6 */
	LIBFMOS_LZVN_OPPCODE_TYPE_LITERAL_SMALL,	/* 0xe7 */
	LIBFMOS_LZVN_OPPCODE_TYPE_LITERAL_SMALL,	/* 0xe8 */
	LIBFMOS_LZVN_OPPCODE_TYPE_LITERAL_SMALL,	/* 0xe9 */
	LIBFMOS_LZVN_OPPCODE_TYPE_LITERAL_SMALL,	/* 0xea */
	LIBFMOS_LZVN_OPPCODE_TYPE_LITERAL_SMALL,	/* 0xeb */
	LIBFMOS_LZVN_OPPCODE_TYPE_LITERAL_SMALL,	/* 0xec */
	LIBFMOS_LZVN_OPPCODE_TYPE_LITERAL_SMALL,	/* 0xed */
	LIBFMOS_LZVN_OPPCODE_TYPE_LITERAL_SMALL,	/* 0xee */
	LIBFMOS_LZVN_OPPCODE_TYPE_LITERAL_SMALL,	/* 0xef */

	LIBFMOS_LZVN_OPPCODE_TYPE_MATCH_LARGE,		/* 0xf0 */
	LIBFMOS_LZVN_OPPCODE_TYPE_MATCH_SMALL,		/* 0xf1 */
	LIBFMOS_LZVN_OPPCODE_TYPE_MATCH_SMALL,		/* 0xf2 */
	LIBFMOS_LZVN_OPPCODE_TYPE_MATCH_SMALL,		/* 0xf3 */
	LIBFMOS_LZVN_OPPCODE_TYPE_MATCH_SMALL,		/* 0xf4 */
	LIBFMOS_LZVN_OPPCODE_TYPE_MATCH_SMALL,		/* 0xf5 */
	LIBFMOS_LZVN_OPPCODE_TYPE_MATCH_SMALL,		/* 0xf6 */
	LIBFMOS_LZVN_OPPCODE_TYPE_MATCH_SMALL,		/* 0xf7 */
	LIBFMOS_LZVN_OPPCODE_TYPE_MATCH_SMALL,		/* 0xf8 */
	LIBFMOS_LZVN_OPPCODE_TYPE_MATCH_SMALL,		/* 0xf9 */
	LIBFMOS_LZVN_OPPCODE_TYPE_MATCH_SMALL,		/* 0xfa */
	LIBFMOS_LZVN_OPPCODE_TYPE_MATCH_SMALL,		/* 0xfb */
	LIBFMOS_LZVN_OPPCODE_TYPE_MATCH_SMALL,		/* 0xfc */
	LIBFMOS_LZVN_OPPCODE_TYPE_MATCH_SMALL,		/* 0xfd */
	LIBFMOS_LZVN_OPPCODE_TYPE_MATCH_SMALL,		/* 0xfe */
	LIBFMOS_LZVN_OPPCODE_TYPE_MATCH_SMALL,		/* 0xff */
};

/* Decompresses LZVN compressed data
 * Returns 1 on success or -1 on error
 */
int libfmos_lzvn_decompress(
     const uint8_t *compressed_data,
     size_t compressed_data_size,
     uint8_t *uncompressed_data,
     size_t *uncompressed_data_size,
     libcerror_error_t **error )
{
	static char *function              = "libfmos_lzvn_decompress";
	size_t compressed_data_offset      = 0;
	size_t match_offset                = 0;
	size_t safe_uncompressed_data_size = 0;
	size_t uncompressed_data_offset    = 0;
	uint16_t distance                  = 0;
	uint16_t literal_size              = 0;
	uint16_t match_size                = 0;
	uint8_t oppcode                    = 0;
	uint8_t oppcode_type               = 0;
	uint8_t oppcode_value              = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	size_t debug_match_offset          = 0;
	size_t oppcode_data_offset         = 0;
	size_t oppcode_data_size           = 0;
	uint16_t debug_match_size          = 0;
#endif

	if( compressed_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid compressed data.",
		 function );

		return( -1 );
	}
	if( compressed_data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid compressed data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( uncompressed_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid uncompressed data.",
		 function );

		return( -1 );
	}
	if( uncompressed_data_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid uncompressed data size.",
		 function );

		return( -1 );
	}
	safe_uncompressed_data_size = *uncompressed_data_size;

	if( safe_uncompressed_data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid uncompressed data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	while( compressed_data_offset < compressed_data_size )
	{
		if( uncompressed_data_offset >= safe_uncompressed_data_size )
		{
			break;
		}
		if( compressed_data_offset >= compressed_data_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
			 "%s: compressed data size value too small.",
			 function );

			return( -1 );
		}
#if defined( HAVE_DEBUG_OUTPUT )
		oppcode_data_offset = compressed_data_offset;
		oppcode_data_size   = 1;
#endif
		oppcode = compressed_data[ compressed_data_offset++ ];

		oppcode_type = libfmos_lzvn_oppcode_types[ oppcode ];

		literal_size = 0;
		match_size   = 0;

		switch( oppcode_type )
		{
			case LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_LARGE:
#if defined( HAVE_DEBUG_OUTPUT )
				oppcode_data_size += 2;
#endif
				if( ( compressed_data_offset + 1 ) >= compressed_data_size )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
					 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
					 "%s: compressed data size value too small.",
					 function );

					return( -1 );
				}
				oppcode_value = compressed_data[ compressed_data_offset++ ];

				literal_size = ( oppcode & 0xc0 ) >> 6;
				match_size   = ( ( oppcode & 0x38 ) >> 3 ) + 3;
				distance     = ( (uint16_t) compressed_data[ compressed_data_offset++ ] << 8 ) | oppcode_value;

				break;

			case LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_MEDIUM:
#if defined( HAVE_DEBUG_OUTPUT )
				oppcode_data_size += 2;
#endif
				if( ( compressed_data_offset + 1 ) >= compressed_data_size )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
					 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
					 "%s: compressed data size value too small.",
					 function );

					return( -1 );
				}
				oppcode_value = compressed_data[ compressed_data_offset++ ];

				literal_size = ( oppcode & 0x18 ) >> 3;
				match_size   = ( ( ( oppcode & 0x07 ) << 2 ) | ( oppcode_value & 0x03 ) ) + 3;
				distance     = ( (uint16_t) compressed_data[ compressed_data_offset++ ] << 6 ) | ( ( oppcode_value & 0xfc ) >> 2 );

				break;

			case LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_PREVIOUS:
				literal_size = ( oppcode & 0xc0 ) >> 6;
				match_size   = ( ( oppcode & 0x38 ) >> 3 ) + 3;

				break;

			case LIBFMOS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL:
#if defined( HAVE_DEBUG_OUTPUT )
				oppcode_data_size += 1;
#endif
				if( compressed_data_offset >= compressed_data_size )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
					 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
					 "%s: compressed data size value too small.",
					 function );

					return( -1 );
				}
				literal_size = ( oppcode & 0xc0 ) >> 6;
				match_size   = ( ( oppcode & 0x38 ) >> 3 ) + 3;
				distance     = ( (uint16_t) ( oppcode & 0x07 ) << 8 ) | compressed_data[ compressed_data_offset++ ];

				break;

			case LIBFMOS_LZVN_OPPCODE_TYPE_LITERAL_LARGE:
#if defined( HAVE_DEBUG_OUTPUT )
				oppcode_data_size += 1;
#endif
				if( compressed_data_offset >= compressed_data_size )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
					 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
					 "%s: compressed data size value too small.",
					 function );

					return( -1 );
				}
				literal_size = (uint16_t) compressed_data[ compressed_data_offset++ ] + 16;

				break;

			case LIBFMOS_LZVN_OPPCODE_TYPE_LITERAL_SMALL:
				literal_size = oppcode & 0x0f;

				break;

			case LIBFMOS_LZVN_OPPCODE_TYPE_MATCH_LARGE:
#if defined( HAVE_DEBUG_OUTPUT )
				oppcode_data_size += 1;
#endif
				if( compressed_data_offset >= compressed_data_size )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
					 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
					 "%s: compressed data size value too small.",
					 function );

					return( -1 );
				}
				match_size = (uint16_t) compressed_data[ compressed_data_offset++ ] + 16;

				break;

			case LIBFMOS_LZVN_OPPCODE_TYPE_MATCH_SMALL:
				match_size = oppcode & 0x0f;

				break;

			case LIBFMOS_LZVN_OPPCODE_TYPE_END_OF_STREAM:
			case LIBFMOS_LZVN_OPPCODE_TYPE_NONE:
				break;

			case LIBFMOS_LZVN_OPPCODE_TYPE_INVALID:
			default:
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
				 "%s: invalid oppcode: 0x%02" PRIx8 ".",
				 function,
				 oppcode );

				return( -1 );
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: oppcode data:\n",
			 function );
			libcnotify_print_data(
			 &( compressed_data[ oppcode_data_offset ] ),
			 oppcode_data_size,
			 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );

			libcnotify_printf(
			 "%s: oppcode\t\t\t\t\t: 0x%02" PRIx8 "\n",
			 function,
			 oppcode );

			libcnotify_printf(
			 "%s: literal size\t\t\t\t\t: %" PRIu16 "\n",
			 function,
			 literal_size );

			libcnotify_printf(
			 "%s: match size\t\t\t\t\t: %" PRIu16 "\n",
			 function,
			 match_size );

			libcnotify_printf(
			 "%s: distance\t\t\t\t\t: %" PRIu16 "\n",
			 function,
			 distance );

			libcnotify_printf(
			 "\n" );
		}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

		if( oppcode_type == LIBFMOS_LZVN_OPPCODE_TYPE_END_OF_STREAM )
		{
			break;
		}
		if( literal_size > 0 )
		{
			if( ( (size_t) literal_size > compressed_data_size )
			 || ( compressed_data_offset > ( compressed_data_size - literal_size ) ) )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: literal size value exceeds compressed data size.",
				 function );

				return( -1 );
			}
			if( ( (size_t) literal_size > safe_uncompressed_data_size )
			 || ( uncompressed_data_offset > ( safe_uncompressed_data_size - literal_size ) ) )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: literal size value exceeds uncompressed data size.",
				 function );

				return( -1 );
			}
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: literal:\n",
				 function );
				libcnotify_print_data(
				 &( compressed_data[ compressed_data_offset ] ),
				 literal_size,
				 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
			}
#endif
			if( memory_copy(
			     &( uncompressed_data[ uncompressed_data_offset ] ),
			     &( compressed_data[ compressed_data_offset ] ),
			     (size_t) literal_size ) == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
				 "%s: unable to copy literal to uncompressed data.",
				 function );

				return( -1 );
			}
			compressed_data_offset   += (size_t) literal_size;
			uncompressed_data_offset += (size_t) literal_size;
		}
		if( match_size > 0 )
		{
			if( (size_t) distance > uncompressed_data_offset )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: distance value exceeds uncompressed data offset.",
				 function );

				return( -1 );
			}
			match_offset = uncompressed_data_offset - distance;

			if( ( (size_t) match_size > safe_uncompressed_data_size )
			 || ( uncompressed_data_offset > ( safe_uncompressed_data_size - match_size ) ) )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: match size value exceeds uncompressed data size.",
				 function );

				return( -1 );
			}
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				debug_match_offset = match_offset;
				debug_match_size   = match_size;

				libcnotify_printf(
				 "%s: match offset\t\t\t\t\t: 0x%" PRIzx "\n",
				 function,
				 debug_match_offset );
			}
#endif
			while( match_size > 0 )
			{
				uncompressed_data[ uncompressed_data_offset++ ] = uncompressed_data[ match_offset++ ];

				match_size--;
			}
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: match:\n",
				 function );
				libcnotify_print_data(
				 &( uncompressed_data[ debug_match_offset ] ),
				 debug_match_size,
				 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
			}
#endif
		}
	}
	*uncompressed_data_size = uncompressed_data_offset;

	return( 1 );
}

