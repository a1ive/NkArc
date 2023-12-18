/*
 * Windows 1256 codepage (Arabic) functions
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

#include "libuna_codepage_windows_1256.h"
#include "libuna_types.h"

/* Extended ASCII to Unicode character lookup table for the Windows 1256 codepage
 * Unknown are filled with the Unicode replacement character 0xfffd
 */
const uint16_t libuna_codepage_windows_1256_byte_stream_to_unicode_base_0x80[ 128 ] = {
	0x20ac, 0x067e, 0x201a, 0x0192, 0x201e, 0x2026, 0x2020, 0x2021,
	0x02c6, 0x2030, 0x0679, 0x2039, 0x0152, 0x0686, 0x0698, 0x0688,
	0x06af, 0x2018, 0x2019, 0x201c, 0x201d, 0x2022, 0x2013, 0x2014,
	0x06a9, 0x2122, 0x0691, 0x203a, 0x0153, 0x200c, 0x200d, 0x06ba,
	0x00a0, 0x060c, 0x00a2, 0x00a3, 0x00a4, 0x00a5, 0x00a6, 0x00a7,
	0x00a8, 0x00a9, 0x06be, 0x00ab, 0x00ac, 0x00ad, 0x00ae, 0x00af,
	0x00b0, 0x00b1, 0x00b2, 0x00b3, 0x00b4, 0x00b5, 0x00b6, 0x00b7,
	0x00b8, 0x00b9, 0x061b, 0x00bb, 0x00bc, 0x00bd, 0x00be, 0x061f,
	0x06c1, 0x0621, 0x0622, 0x0623, 0x0624, 0x0625, 0x0626, 0x0627,
	0x0628, 0x0629, 0x062a, 0x062b, 0x062c, 0x062d, 0x062e, 0x062f,
	0x0630, 0x0631, 0x0632, 0x0633, 0x0634, 0x0635, 0x0636, 0x00d7,
	0x0637, 0x0638, 0x0639, 0x063a, 0x0640, 0x0641, 0x0642, 0x0643,
	0x00e0, 0x0644, 0x00e2, 0x0645, 0x0646, 0x0647, 0x0648, 0x00e7,
	0x00e8, 0x00e9, 0x00ea, 0x00eb, 0x0649, 0x064a, 0x00ee, 0x00ef,
	0x064b, 0x064c, 0x064d, 0x064e, 0x00f4, 0x064f, 0x0650, 0x00f7,
	0x0651, 0x00f9, 0x0652, 0x00fb, 0x00fc, 0x200e, 0x200f, 0x06d2
};

/* Unicode to ASCII character lookup tables for the Windows 1256 codepage
 * Unknown are filled with the ASCII replacement character 0x1a
 */
const uint8_t libuna_codepage_windows_1256_unicode_to_byte_stream_base_0x00a0[ 32 ] = {
	0xa0, 0x1a, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7,
	0xa8, 0xa9, 0x1a, 0xab, 0xac, 0xad, 0xae, 0xaf,
	0xb0, 0xb1, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7,
	0xb8, 0xb9, 0x1a, 0xbb, 0xbc, 0xbd, 0xbe, 0x1a
};

const uint8_t libuna_codepage_windows_1256_unicode_to_byte_stream_base_0x00e0[ 32 ] = {
	0xe0, 0x1a, 0xe2, 0x1a, 0x1a, 0x1a, 0x1a, 0xe7,
	0xe8, 0xe9, 0xea, 0xeb, 0x1a, 0x1a, 0xee, 0xef,
	0x1a, 0x1a, 0x1a, 0x1a, 0xf4, 0x1a, 0x1a, 0xf7,
	0x1a, 0xf9, 0x1a, 0xfb, 0xfc, 0x1a, 0x1a, 0x1a
};

const uint8_t libuna_codepage_windows_1256_unicode_to_byte_stream_base_0x0618[ 64 ] = {
	0x1a, 0x1a, 0x1a, 0xba, 0x1a, 0x1a, 0x1a, 0xbf,
	0x1a, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7,
	0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf,
	0xd0, 0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd8,
	0xd9, 0xda, 0xdb, 0x1a, 0x1a, 0x1a, 0x1a, 0x1a,
	0xdc, 0xdd, 0xde, 0xdf, 0xe1, 0xe3, 0xe4, 0xe5,
	0xe6, 0xec, 0xed, 0xf0, 0xf1, 0xf2, 0xf3, 0xf5,
	0xf6, 0xf8, 0xfa, 0x1a, 0x1a, 0x1a, 0x1a, 0x1a
};

const uint8_t libuna_codepage_windows_1256_unicode_to_byte_stream_base_0x2008[ 32 ] = {
	0x1a, 0x1a, 0x1a, 0x1a, 0x9d, 0x9e, 0xfd, 0xfe,
	0x1a, 0x1a, 0x1a, 0x96, 0x97, 0x1a, 0x1a, 0x1a,
	0x91, 0x92, 0x82, 0x1a, 0x93, 0x94, 0x84, 0x1a,
	0x86, 0x87, 0x95, 0x1a, 0x1a, 0x1a, 0x85, 0x1a
};

/* Copies an Unicode character from a Windows 1256 encoded byte stream
 * Returns 1 if successful or -1 on error
 */
int libuna_codepage_windows_1256_copy_from_byte_stream(
     libuna_unicode_character_t *unicode_character,
     const uint8_t *byte_stream,
     size_t byte_stream_size,
     size_t *byte_stream_index,
     libcerror_error_t **error )
{
	static char *function                             = "libuna_codepage_windows_1256_copy_from_byte_stream";
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

		safe_unicode_character = libuna_codepage_windows_1256_byte_stream_to_unicode_base_0x80[ byte_stream_character ];
	}
	*unicode_character = safe_unicode_character;
	*byte_stream_index = safe_byte_stream_index;

	return( 1 );
}

/* Copies an Unicode character to a Windows 1256 encoded byte stream
 * Returns 1 if successful or -1 on error
 */
int libuna_codepage_windows_1256_copy_to_byte_stream(
     libuna_unicode_character_t unicode_character,
     uint8_t *byte_stream,
     size_t byte_stream_size,
     size_t *byte_stream_index,
     libcerror_error_t **error )
{
	static char *function         = "libuna_codepage_windows_1256_copy_to_byte_stream";
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
		unicode_character -= 0x00a0;

		byte_stream_value = libuna_codepage_windows_1256_unicode_to_byte_stream_base_0x00a0[ unicode_character ];
	}
	else if( ( unicode_character >= 0x00e0 )
	      && ( unicode_character < 0x0100 ) )
	{
		unicode_character -= 0x00e0;

		byte_stream_value = libuna_codepage_windows_1256_unicode_to_byte_stream_base_0x00e0[ unicode_character ];
	}
	else if( ( unicode_character >= 0x0618 )
	      && ( unicode_character < 0x0658 ) )
	{
		unicode_character -= 0x0618;

		byte_stream_value = libuna_codepage_windows_1256_unicode_to_byte_stream_base_0x0618[ unicode_character ];
	}
	else if( ( unicode_character >= 0x2008 )
	      && ( unicode_character < 0x2028 ) )
	{
		unicode_character -= 0x2008;

		byte_stream_value = libuna_codepage_windows_1256_unicode_to_byte_stream_base_0x2008[ unicode_character ];
	}
	else switch( unicode_character )
	{
		case 0x00d7:
			byte_stream_value = 0xd7;
			break;

		case 0x0152:
			byte_stream_value = 0x8c;
			break;

		case 0x0153:
			byte_stream_value = 0x9c;
			break;

		case 0x0192:
			byte_stream_value = 0x83;
			break;

		case 0x02c6:
			byte_stream_value = 0x88;
			break;

		case 0x060c:
			byte_stream_value = 0xa1;
			break;

		case 0x0679:
			byte_stream_value = 0x8a;
			break;

		case 0x067e:
			byte_stream_value = 0x81;
			break;

		case 0x0686:
			byte_stream_value = 0x8d;
			break;

		case 0x0688:
			byte_stream_value = 0x8f;
			break;

		case 0x0691:
			byte_stream_value = 0x9a;
			break;

		case 0x0698:
			byte_stream_value = 0x8e;
			break;

		case 0x06a9:
			byte_stream_value = 0x98;
			break;

		case 0x06af:
			byte_stream_value = 0x90;
			break;

		case 0x06ba:
			byte_stream_value = 0x9f;
			break;

		case 0x06be:
			byte_stream_value = 0xaa;
			break;

		case 0x06c1:
			byte_stream_value = 0xc0;
			break;

		case 0x06d2:
			byte_stream_value = 0xff;
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

