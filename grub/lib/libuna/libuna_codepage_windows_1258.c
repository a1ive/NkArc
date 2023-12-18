/*
 * Windows 1258 codepage (Vietnamese) functions
 *
 * Copyright (C) 2008-2023, Joachim Metz <joachim.metz@gmail.com>
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

#include "libuna_codepage_windows_1258.h"
#include "libuna_types.h"

/* Extended ASCII to Unicode character lookup table for the Windows 1258 codepage
 * Unknown are filled with the Unicode replacement character 0xfffd
 */
const uint16_t libuna_codepage_windows_1258_byte_stream_to_unicode_base_0x80[ 128 ] = {
	0x20ac, 0xfffd, 0x201a, 0x0192, 0x201e, 0x2026, 0x2020, 0x2021,
	0x02c6, 0x2030, 0xfffd, 0x2039, 0x0152, 0xfffd, 0xfffd, 0xfffd,
	0xfffd, 0x2018, 0x2019, 0x201c, 0x201d, 0x2022, 0x2013, 0x2014,
	0x02dc, 0x2122, 0xfffd, 0x203a, 0x0153, 0xfffd, 0xfffd, 0x0178,
	0x00a0, 0x00a1, 0x00a2, 0x00a3, 0x00a4, 0x00a5, 0x00a6, 0x00a7,
	0x00a8, 0x00a9, 0x00aa, 0x00ab, 0x00ac, 0x00ad, 0x00ae, 0x00af,
	0x00b0, 0x00b1, 0x00b2, 0x00b3, 0x00b4, 0x00b5, 0x00b6, 0x00b7,
	0x00b8, 0x00b9, 0x00ba, 0x00bb, 0x00bc, 0x00bd, 0x00be, 0x00bf,
	0x00c0, 0x00c1, 0x00c2, 0x0102, 0x00c4, 0x00c5, 0x00c6, 0x00c7,
	0x00c8, 0x00c9, 0x00ca, 0x00cb, 0x0300, 0x00cd, 0x00ce, 0x00cf,
	0x0110, 0x00d1, 0x0309, 0x00d3, 0x00d4, 0x01a0, 0x00d6, 0x00d7,
	0x00d8, 0x00d9, 0x00da, 0x00db, 0x00dc, 0x01af, 0x0303, 0x00df,
	0x00e0, 0x00e1, 0x00e2, 0x0103, 0x00e4, 0x00e5, 0x00e6, 0x00e7,
	0x00e8, 0x00e9, 0x00ea, 0x00eb, 0x0301, 0x00ed, 0x00ee, 0x00ef,
	0x0111, 0x00f1, 0x0323, 0x00f3, 0x00f4, 0x01a1, 0x00f6, 0x00f7,
	0x00f8, 0x00f9, 0x00fa, 0x00fb, 0x00fc, 0x01b0, 0x20ab, 0x00ff
};

/* Unicode to ASCII character lookup tables for the Windows 1258 codepage
 * Unknown are filled with the ASCII replacement character 0x1a
 */
const uint8_t libuna_codepage_windows_1258_unicode_to_byte_stream_base_0x00c0[ 72 ] = {
	0xc0, 0xc1, 0xc2, 0x1a, 0xc4, 0xc5, 0xc6, 0xc7,
	0xc8, 0xc9, 0xca, 0xcb, 0x1a, 0xcd, 0xce, 0xcf,
	0x1a, 0xd1, 0x1a, 0xd3, 0xd4, 0x1a, 0xd6, 0xd7,
	0xd8, 0xd9, 0xda, 0xdb, 0xdc, 0x1a, 0x1a, 0xdf,
	0xe0, 0xe1, 0xe2, 0x1a, 0xe4, 0xe5, 0xe6, 0xe7,
	0xe8, 0xe9, 0xea, 0xeb, 0x1a, 0xed, 0xee, 0xef,
	0x1a, 0xf1, 0x1a, 0xf3, 0xf4, 0x1a, 0xf6, 0xf7,
	0xf8, 0xf9, 0xfa, 0xfb, 0xfc, 0x1a, 0x1a, 0xff,
	0x1a, 0x1a, 0xc3, 0xe3, 0x1a, 0x1a, 0x1a, 0x1a
};

const uint8_t libuna_codepage_windows_1258_unicode_to_byte_stream_base_0x2010[ 24 ] = {
	0x1a, 0x1a, 0x1a, 0x96, 0x97, 0x1a, 0x1a, 0x1a,
	0x91, 0x92, 0x82, 0x1a, 0x93, 0x94, 0x84, 0x1a,
	0x86, 0x87, 0x95, 0x1a, 0x1a, 0x1a, 0x85, 0x1a
};

/* Copies an Unicode character from a Windows 1258 encoded byte stream
 * Returns 1 if successful or -1 on error
 */
int libuna_codepage_windows_1258_copy_from_byte_stream(
     libuna_unicode_character_t *unicode_character,
     const uint8_t *byte_stream,
     size_t byte_stream_size,
     size_t *byte_stream_index,
     libcerror_error_t **error )
{
	static char *function                             = "libuna_codepage_windows_1258_copy_from_byte_stream";
	libuna_unicode_character_t safe_unicode_character = 0xfffd;
	size_t safe_byte_stream_index                     = 0;
	uint8_t byte_stream_character                     = 0;

	if( unicode_character == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid Unicode character.",
		 function );

		return( -1 );
	}
	if( byte_stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid byte stream.",
		 function );

		return( -1 );
	}
	if( byte_stream_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid byte stream size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( byte_stream_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid byte stream index.",
		 function );

		return( -1 );
	}
	safe_byte_stream_index = *byte_stream_index;

	if( safe_byte_stream_index >= byte_stream_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: byte stream too small.",
		 function );

		return( -1 );
	}
	byte_stream_character = byte_stream[ safe_byte_stream_index++ ];

	if( byte_stream_character < 0x80 )
	{
		safe_unicode_character = byte_stream_character;
	}
	else
	{
		byte_stream_character -= 0x80;

		safe_unicode_character = libuna_codepage_windows_1258_byte_stream_to_unicode_base_0x80[ byte_stream_character ];
	}
	*unicode_character = safe_unicode_character;
	*byte_stream_index = safe_byte_stream_index;

	return( 1 );
}

/* Copies an Unicode character to a Windows 1258 encoded byte stream
 * Returns 1 if successful or -1 on error
 */
int libuna_codepage_windows_1258_copy_to_byte_stream(
     libuna_unicode_character_t unicode_character,
     uint8_t *byte_stream,
     size_t byte_stream_size,
     size_t *byte_stream_index,
     libcerror_error_t **error )
{
	static char *function         = "libuna_codepage_windows_1258_copy_to_byte_stream";
	size_t safe_byte_stream_index = 0;
	uint16_t byte_stream_value    = 0x001a;

	if( byte_stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid byte stream.",
		 function );

		return( -1 );
	}
	if( byte_stream_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid byte stream size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( byte_stream_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid byte stream index.",
		 function );

		return( -1 );
	}
	safe_byte_stream_index = *byte_stream_index;

	if( safe_byte_stream_index >= byte_stream_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: byte stream too small.",
		 function );

		return( -1 );
	}
	if( unicode_character < 0x0080 )
	{
		byte_stream_value = (uint16_t) unicode_character;
	}
	else if( ( unicode_character >= 0x00a0 )
	      && ( unicode_character < 0x00c0 ) )
	{
		byte_stream_value = (uint16_t) unicode_character;
	}
	else if( ( unicode_character >= 0x00c0 )
	      && ( unicode_character < 0x0108 ) )
	{
		unicode_character -= 0x00c0;

		byte_stream_value = libuna_codepage_windows_1258_unicode_to_byte_stream_base_0x00c0[ unicode_character ];
	}
	else if( ( unicode_character >= 0x2010 )
	      && ( unicode_character < 0x2028 ) )
	{
		unicode_character -= 0x2010;

		byte_stream_value = libuna_codepage_windows_1258_unicode_to_byte_stream_base_0x2010[ unicode_character ];
	}
	else switch( unicode_character )
	{
		case 0x0110:
			byte_stream_value = 0xd0;
			break;

		case 0x0111:
			byte_stream_value = 0xf0;
			break;

		case 0x0152:
			byte_stream_value = 0x8c;
			break;

		case 0x0153:
			byte_stream_value = 0x9c;
			break;

		case 0x0178:
			byte_stream_value = 0x9f;
			break;

		case 0x0192:
			byte_stream_value = 0x83;
			break;

		case 0x01a0:
			byte_stream_value = 0xd5;
			break;

		case 0x01a1:
			byte_stream_value = 0xf5;
			break;

		case 0x01af:
			byte_stream_value = 0xdd;
			break;

		case 0x01b0:
			byte_stream_value = 0xfd;
			break;

		case 0x02c6:
			byte_stream_value = 0x88;
			break;

		case 0x02dc:
			byte_stream_value = 0x98;
			break;

		case 0x0300:
			byte_stream_value = 0xcc;
			break;

		case 0x0301:
			byte_stream_value = 0xec;
			break;

		case 0x0303:
			byte_stream_value = 0xde;
			break;

		case 0x0309:
			byte_stream_value = 0xd2;
			break;

		case 0x0323:
			byte_stream_value = 0xf2;
			break;

		case 0x2030:
			byte_stream_value = 0x89;
			break;

		case 0x2039:
			byte_stream_value = 0x8b;
			break;

		case 0x203a:
			byte_stream_value = 0x9b;
			break;

		case 0x20ab:
			byte_stream_value = 0xfe;
			break;

		case 0x20ac:
			byte_stream_value = 0x80;
			break;

		case 0x2122:
			byte_stream_value = 0x99;
			break;

		default:
			byte_stream_value = 0x1a;
			break;
	}
	byte_stream[ safe_byte_stream_index++ ] = (uint8_t) ( byte_stream_value & 0x00ff );

	*byte_stream_index = safe_byte_stream_index;

	return( 1 );
}

