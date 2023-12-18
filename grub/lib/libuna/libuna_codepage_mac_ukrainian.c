/*
 * MacUkrainian codepage functions
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

#include "libuna_codepage_mac_ukrainian.h"
#include "libuna_types.h"

/* Extended ASCII to Unicode character lookup table for the MacUkrainian codepage
 * Unknown are filled with the Unicode replacement character 0xfffd
 */
const uint16_t libuna_codepage_mac_ukrainian_byte_stream_to_unicode_base_0x80[ 128 ] = {
	0x0410, 0x0411, 0x0412, 0x0413, 0x0414, 0x0415, 0x0416, 0x0417,
	0x0418, 0x0419, 0x041a, 0x041b, 0x041c, 0x041d, 0x041e, 0x041f,
	0x0420, 0x0421, 0x0422, 0x0423, 0x0424, 0x0425, 0x0426, 0x0427,
	0x0428, 0x0429, 0x042a, 0x042b, 0x042c, 0x042d, 0x042e, 0x042f,
	0x2020, 0x00b0, 0x0490, 0x00a3, 0x00a7, 0x2022, 0x00b6, 0x0406,
	0x00ae, 0x00a9, 0x2122, 0x0402, 0x0452, 0x2260, 0x0403, 0x0453,
	0x221e, 0x00b1, 0x2264, 0x2265, 0x0456, 0x00b5, 0x0491, 0x0408,
	0x0404, 0x0454, 0x0407, 0x0457, 0x0409, 0x0459, 0x040a, 0x045a,
	0x0458, 0x0405, 0x00ac, 0x221a, 0x0192, 0x2248, 0x2206, 0x00ab,
	0x00bb, 0x2026, 0x00a0, 0x040b, 0x045b, 0x040c, 0x045c, 0x0455,
	0x2013, 0x2014, 0x201c, 0x201d, 0x2018, 0x2019, 0x00f7, 0x201e,
	0x040e, 0x045e, 0x040f, 0x045f, 0x2116, 0x0401, 0x0451, 0x044f,
	0x0430, 0x0431, 0x0432, 0x0433, 0x0434, 0x0435, 0x0436, 0x0437,
	0x0438, 0x0439, 0x043a, 0x043b, 0x043c, 0x043d, 0x043e, 0x043f,
	0x0440, 0x0441, 0x0442, 0x0443, 0x0444, 0x0445, 0x0446, 0x0447,
	0x0448, 0x0449, 0x044a, 0x044b, 0x044c, 0x044d, 0x044e, 0x00a4
};

/* Unicode to ASCII character lookup tables for the MacUkrainian codepage
 * Unknown are filled with the ASCII replacement character 0x1a
 */
const uint8_t libuna_codepage_mac_ukrainian_unicode_to_byte_stream_base_0x00a0[ 32 ] = {
	0xca, 0x1a, 0x1a, 0xa3, 0xff, 0x1a, 0x1a, 0xa4,
	0x1a, 0xa9, 0x1a, 0xc7, 0xc2, 0x1a, 0xa8, 0x1a,
	0xa1, 0xb1, 0x1a, 0x1a, 0x1a, 0xb5, 0xa6, 0x1a,
	0x1a, 0x1a, 0x1a, 0xc8, 0x1a, 0x1a, 0x1a, 0x1a
};

const uint8_t libuna_codepage_mac_ukrainian_unicode_to_byte_stream_base_0x0400[ 96 ] = {
	0x1a, 0xdd, 0xab, 0xae, 0xb8, 0xc1, 0xa7, 0xba,
	0xb7, 0xbc, 0xbe, 0xcb, 0xcd, 0x1a, 0xd8, 0xda,
	0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
	0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f,
	0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97,
	0x98, 0x99, 0x9a, 0x9b, 0x9c, 0x9d, 0x9e, 0x9f,
	0xe0, 0xe1, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7,
	0xe8, 0xe9, 0xea, 0xeb, 0xec, 0xed, 0xee, 0xef,
	0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7,
	0xf8, 0xf9, 0xfa, 0xfb, 0xfc, 0xfd, 0xfe, 0xdf,
	0x1a, 0xde, 0xac, 0xaf, 0xb9, 0xcf, 0xb4, 0xbb,
	0xc0, 0xbd, 0xbf, 0xcc, 0xce, 0x1a, 0xd9, 0xdb
};

const uint8_t libuna_codepage_mac_ukrainian_unicode_to_byte_stream_base_0x2010[ 24 ] = {
	0x1a, 0x1a, 0x1a, 0xd0, 0xd1, 0x1a, 0x1a, 0x1a,
	0xd4, 0xd5, 0x1a, 0x1a, 0xd2, 0xd3, 0xd7, 0x1a,
	0xa0, 0x1a, 0xa5, 0x1a, 0x1a, 0x1a, 0xc9, 0x1a
};

/* Copies an Unicode character from a MacUkrainian encoded byte stream
 * Returns 1 if successful or -1 on error
 */
int libuna_codepage_mac_ukrainian_copy_from_byte_stream(
     libuna_unicode_character_t *unicode_character,
     const uint8_t *byte_stream,
     size_t byte_stream_size,
     size_t *byte_stream_index,
     libcerror_error_t **error )
{
	static char *function                             = "libuna_codepage_mac_ukrainian_copy_from_byte_stream";
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

		safe_unicode_character = libuna_codepage_mac_ukrainian_byte_stream_to_unicode_base_0x80[ byte_stream_character ];
	}
	*unicode_character = safe_unicode_character;
	*byte_stream_index = safe_byte_stream_index;

	return( 1 );
}

/* Copies an Unicode character to a MacUkrainian encoded byte stream
 * Returns 1 if successful or -1 on error
 */
int libuna_codepage_mac_ukrainian_copy_to_byte_stream(
     libuna_unicode_character_t unicode_character,
     uint8_t *byte_stream,
     size_t byte_stream_size,
     size_t *byte_stream_index,
     libcerror_error_t **error )
{
	static char *function         = "libuna_codepage_mac_ukrainian_copy_to_byte_stream";
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

		byte_stream_value = libuna_codepage_mac_ukrainian_unicode_to_byte_stream_base_0x00a0[ unicode_character ];
	}
	else if( ( unicode_character >= 0x0400 )
	      && ( unicode_character < 0x0460 ) )
	{
		unicode_character -= 0x0400;

		byte_stream_value = libuna_codepage_mac_ukrainian_unicode_to_byte_stream_base_0x0400[ unicode_character ];
	}
	else if( ( unicode_character >= 0x2010 )
	      && ( unicode_character < 0x2028 ) )
	{
		unicode_character -= 0x2010;

		byte_stream_value = libuna_codepage_mac_ukrainian_unicode_to_byte_stream_base_0x2010[ unicode_character ];
	}
	else switch( unicode_character )
	{
		case 0x00f7:
			byte_stream_value = 0xd6;
			break;

		case 0x0192:
			byte_stream_value = 0xc4;
			break;

		case 0x0490:
			byte_stream_value = 0xa2;
			break;

		case 0x0491:
			byte_stream_value = 0xb6;
			break;

		case 0x2116:
			byte_stream_value = 0xdc;
			break;

		case 0x2122:
			byte_stream_value = 0xaa;
			break;

		case 0x2206:
			byte_stream_value = 0xc6;
			break;

		case 0x221a:
			byte_stream_value = 0xc3;
			break;

		case 0x221e:
			byte_stream_value = 0xb0;
			break;

		case 0x2248:
			byte_stream_value = 0xc5;
			break;

		case 0x2260:
			byte_stream_value = 0xad;
			break;

		case 0x2264:
			byte_stream_value = 0xb2;
			break;

		case 0x2265:
			byte_stream_value = 0xb3;
			break;

		default:
			byte_stream_value = 0x1a;
			break;
	}
	byte_stream[ safe_byte_stream_index++ ] = (uint8_t) ( byte_stream_value & 0x00ff );

	*byte_stream_index = safe_byte_stream_index;

	return( 1 );
}

