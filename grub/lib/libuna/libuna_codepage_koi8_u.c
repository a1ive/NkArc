/*
 * KOI8-U codepage (Ukrainian Cyrillic) function
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

#include "libuna_codepage_koi8_u.h"
#include "libuna_types.h"

/* Extended ASCII to Unicode character lookup table for the KOI8-U codepage
 * Unknown are filled with the Unicode replacement character 0xfffd
 */
const uint16_t libuna_codepage_koi8_u_byte_stream_to_unicode_base_0x80[ 128 ] = {
	0x2500, 0x2502, 0x250c, 0x2510, 0x2514, 0x2518, 0x251c, 0x2524,
	0x252c, 0x2534, 0x253c, 0x2580, 0x2584, 0x2588, 0x258c, 0x2590,
	0x2591, 0x2592, 0x2593, 0x2320, 0x25a0, 0x2219, 0x221a, 0x2248,
	0x2264, 0x2265, 0x00a0, 0x2321, 0x00b0, 0x00b2, 0x00b7, 0x00f7,
	0x2550, 0x2551, 0x2552, 0x0451, 0x0454, 0x2554, 0x0456, 0x0457,
	0x2557, 0x2558, 0x2559, 0x255a, 0x255b, 0x0491, 0x255d, 0x255e,
	0x255f, 0x2560, 0x2561, 0x0401, 0x0404, 0x2563, 0x0406, 0x0407,
	0x2566, 0x2567, 0x2568, 0x2569, 0x256a, 0x0490, 0x256c, 0x00a9,
	0x044e, 0x0430, 0x0431, 0x0446, 0x0434, 0x0435, 0x0444, 0x0433,
	0x0445, 0x0438, 0x0439, 0x043a, 0x043b, 0x043c, 0x043d, 0x043e,
	0x043f, 0x044f, 0x0440, 0x0441, 0x0442, 0x0443, 0x0436, 0x0432,
	0x044c, 0x044b, 0x0437, 0x0448, 0x044d, 0x0449, 0x0447, 0x044a,
	0x042e, 0x0410, 0x0411, 0x0426, 0x0414, 0x0415, 0x0424, 0x0413,
	0x0425, 0x0418, 0x0419, 0x041a, 0x041b, 0x041c, 0x041d, 0x041e,
	0x041f, 0x042f, 0x0420, 0x0421, 0x0422, 0x0423, 0x0416, 0x0412,
	0x042c, 0x042b, 0x0417, 0x0428, 0x042d, 0x0429, 0x0427, 0x042a
};

/* Unicode to ASCII character lookup tables for the KOI8-U codepage
 * Unknown are filled with the ASCII replacement character 0x1a
 */
const uint8_t libuna_codepage_koi8_u_unicode_to_byte_stream_base_0x0410[ 64 ] = {
	0xe1, 0xe2, 0xf7, 0xe7, 0xe4, 0xe5, 0xf6, 0xfa,
	0xe9, 0xea, 0xeb, 0xec, 0xed, 0xee, 0xef, 0xf0,
	0xf2, 0xf3, 0xf4, 0xf5, 0xe6, 0xe8, 0xe3, 0xfe,
	0xfb, 0xfd, 0xff, 0xf9, 0xf8, 0xfc, 0xe0, 0xf1,
	0xc1, 0xc2, 0xd7, 0xc7, 0xc4, 0xc5, 0xd6, 0xda,
	0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf, 0xd0,
	0xd2, 0xd3, 0xd4, 0xd5, 0xc6, 0xc8, 0xc3, 0xde,
	0xdb, 0xdd, 0xdf, 0xd9, 0xd8, 0xdc, 0xc0, 0xd1
};

const uint8_t libuna_codepage_koi8_u_unicode_to_byte_stream_base_0x2550[ 32 ] = {
	0xa0, 0xa1, 0xa2, 0x1a, 0xa5, 0x1a, 0x1a, 0xa8,
	0xa9, 0xaa, 0xab, 0xac, 0x1a, 0xae, 0xaf, 0xb0,
	0xb1, 0xb2, 0x1a, 0xb5, 0x1a, 0x1a, 0xb8, 0xb9,
	0xba, 0xbb, 0xbc, 0x1a, 0xbe, 0x1a, 0x1a, 0x1a
};

/* Copies an Unicode character from a KOI8-U encoded byte stream
 * Returns 1 if successful or -1 on error
 */
int libuna_codepage_koi8_u_copy_from_byte_stream(
     libuna_unicode_character_t *unicode_character,
     const uint8_t *byte_stream,
     size_t byte_stream_size,
     size_t *byte_stream_index,
     libcerror_error_t **error )
{
	static char *function                             = "libuna_codepage_koi8_u_copy_from_byte_stream";
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

		safe_unicode_character = libuna_codepage_koi8_u_byte_stream_to_unicode_base_0x80[ byte_stream_character ];
	}
	*unicode_character = safe_unicode_character;
	*byte_stream_index = safe_byte_stream_index;

	return( 1 );
}

/* Copies an Unicode character to a KOI8-U encoded byte stream
 * Returns 1 if successful or -1 on error
 */
int libuna_codepage_koi8_u_copy_to_byte_stream(
     libuna_unicode_character_t unicode_character,
     uint8_t *byte_stream,
     size_t byte_stream_size,
     size_t *byte_stream_index,
     libcerror_error_t **error )
{
	static char *function         = "libuna_codepage_koi8_u_copy_to_byte_stream";
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
	else if( ( unicode_character >= 0x0410 )
	      && ( unicode_character < 0x0450 ) )
	{
		unicode_character -= 0x0410;

		byte_stream_value = libuna_codepage_koi8_u_unicode_to_byte_stream_base_0x0410[ unicode_character ];
	}
	else if( ( unicode_character >= 0x2550 )
	      && ( unicode_character < 0x2570 ) )
	{
		unicode_character -= 0x2550;

		byte_stream_value = libuna_codepage_koi8_u_unicode_to_byte_stream_base_0x2550[ unicode_character ];
	}
	else switch( unicode_character )
	{
		case 0x00a0:
			byte_stream_value = 0x9a;
			break;

		case 0x00a9:
			byte_stream_value = 0xbf;
			break;

		case 0x00b0:
			byte_stream_value = 0x9c;
			break;

		case 0x00b2:
			byte_stream_value = 0x9d;
			break;

		case 0x00b7:
			byte_stream_value = 0x9e;
			break;

		case 0x00f7:
			byte_stream_value = 0x9f;
			break;

		case 0x0401:
			byte_stream_value = 0xb3;
			break;

		case 0x0404:
			byte_stream_value = 0xb4;
			break;

		case 0x0406:
			byte_stream_value = 0xb6;
			break;

		case 0x0407:
			byte_stream_value = 0xb7;
			break;

		case 0x0451:
			byte_stream_value = 0xa3;
			break;

		case 0x0454:
			byte_stream_value = 0xa4;
			break;

		case 0x0456:
			byte_stream_value = 0xa6;
			break;

		case 0x0457:
			byte_stream_value = 0xa7;
			break;

		case 0x0490:
			byte_stream_value = 0xbd;
			break;

		case 0x0491:
			byte_stream_value = 0xad;
			break;

		case 0x2219:
			byte_stream_value = 0x95;
			break;

		case 0x221a:
			byte_stream_value = 0x96;
			break;

		case 0x2248:
			byte_stream_value = 0x97;
			break;

		case 0x2264:
			byte_stream_value = 0x98;
			break;

		case 0x2265:
			byte_stream_value = 0x99;
			break;

		case 0x2320:
			byte_stream_value = 0x93;
			break;

		case 0x2321:
			byte_stream_value = 0x9b;
			break;

		case 0x2500:
			byte_stream_value = 0x80;
			break;

		case 0x2502:
			byte_stream_value = 0x81;
			break;

		case 0x250c:
			byte_stream_value = 0x82;
			break;

		case 0x2510:
			byte_stream_value = 0x83;
			break;

		case 0x2514:
			byte_stream_value = 0x84;
			break;

		case 0x2518:
			byte_stream_value = 0x85;
			break;

		case 0x251c:
			byte_stream_value = 0x86;
			break;

		case 0x2524:
			byte_stream_value = 0x87;
			break;

		case 0x252c:
			byte_stream_value = 0x88;
			break;

		case 0x2534:
			byte_stream_value = 0x89;
			break;

		case 0x253c:
			byte_stream_value = 0x8a;
			break;

		case 0x2580:
			byte_stream_value = 0x8b;
			break;

		case 0x2584:
			byte_stream_value = 0x8c;
			break;

		case 0x2588:
			byte_stream_value = 0x8d;
			break;

		case 0x258c:
			byte_stream_value = 0x8e;
			break;

		case 0x2590:
			byte_stream_value = 0x8f;
			break;

		case 0x2591:
			byte_stream_value = 0x90;
			break;

		case 0x2592:
			byte_stream_value = 0x91;
			break;

		case 0x2593:
			byte_stream_value = 0x92;
			break;

		case 0x25a0:
			byte_stream_value = 0x94;
			break;

		default:
			byte_stream_value = 0x1a;
			break;
	}
	byte_stream[ safe_byte_stream_index++ ] = (uint8_t) ( byte_stream_value & 0x00ff );

	*byte_stream_index = safe_byte_stream_index;

	return( 1 );
}

