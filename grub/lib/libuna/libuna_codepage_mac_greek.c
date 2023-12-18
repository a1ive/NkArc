/*
 * MacGreek codepage functions
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

#include "libuna_codepage_mac_greek.h"
#include "libuna_types.h"

/* Extended ASCII to Unicode character lookup table for the MacGreek codepage
 * Unknown are filled with the Unicode replacement character 0xfffd
 */
const uint16_t libuna_codepage_mac_greek_byte_stream_to_unicode_base_0x80[ 128 ] = {
	0x00c4, 0x00b9, 0x00b2, 0x00c9, 0x00b3, 0x00d6, 0x00dc, 0x0385,
	0x00e0, 0x00e2, 0x00e4, 0x0384, 0x00a8, 0x00e7, 0x00e9, 0x00e8,
	0x00ea, 0x00eb, 0x00a3, 0x2122, 0x00ee, 0x00ef, 0x2022, 0x00bd,
	0x2030, 0x00f4, 0x00f6, 0x00a6, 0x20ac, 0x00f9, 0x00fb, 0x00fc,
	0x2020, 0x0393, 0x0394, 0x0398, 0x039b, 0x039e, 0x03a0, 0x00df,
	0x00ae, 0x00a9, 0x03a3, 0x03aa, 0x00a7, 0x2260, 0x00b0, 0x00b7,
	0x0391, 0x00b1, 0x2264, 0x2265, 0x00a5, 0x0392, 0x0395, 0x0396,
	0x0397, 0x0399, 0x039a, 0x039c, 0x03a6, 0x03ab, 0x03a8, 0x03a9,
	0x03ac, 0x039d, 0x00ac, 0x039f, 0x03a1, 0x2248, 0x03a4, 0x00ab,
	0x00bb, 0x2026, 0x00a0, 0x03a5, 0x03a7, 0x0386, 0x0388, 0x0153,
	0x2013, 0x2015, 0x201c, 0x201d, 0x2018, 0x2019, 0x00f7, 0x0389,
	0x038a, 0x038c, 0x038e, 0x03ad, 0x03ae, 0x03af, 0x03cc, 0x038f,
	0x03cd, 0x03b1, 0x03b2, 0x03c8, 0x03b4, 0x03b5, 0x03c6, 0x03b3,
	0x03b7, 0x03b9, 0x03be, 0x03ba, 0x03bb, 0x03bc, 0x03bd, 0x03bf,
	0x03c0, 0x03ce, 0x03c1, 0x03c3, 0x03c4, 0x03b8, 0x03c9, 0x03c2,
	0x03c7, 0x03c5, 0x03b6, 0x03ca, 0x03cb, 0x0390, 0x03b0, 0x00ad
};

/* Unicode to ASCII character lookup tables for the MacGreek codepage
 * Unknown are filled with the ASCII replacement character 0x1a
 */
const uint8_t libuna_codepage_mac_greek_unicode_to_byte_stream_base_0x00a0[ 96 ] = {
	0xca, 0x1a, 0x1a, 0x92, 0x1a, 0xb4, 0x9b, 0xac,
	0x8c, 0xa9, 0x1a, 0xc7, 0xc2, 0xff, 0xa8, 0x1a,
	0xae, 0xb1, 0x82, 0x84, 0x1a, 0x1a, 0x1a, 0xaf,
	0x1a, 0x81, 0x1a, 0xc8, 0x1a, 0x97, 0x1a, 0x1a,
	0x1a, 0x1a, 0x1a, 0x1a, 0x80, 0x1a, 0x1a, 0x1a,
	0x1a, 0x83, 0x1a, 0x1a, 0x1a, 0x1a, 0x1a, 0x1a,
	0x1a, 0x1a, 0x1a, 0x1a, 0x1a, 0x1a, 0x85, 0x1a,
	0x1a, 0x1a, 0x1a, 0x1a, 0x86, 0x1a, 0x1a, 0xa7,
	0x88, 0x1a, 0x89, 0x1a, 0x8a, 0x1a, 0x1a, 0x8d,
	0x8f, 0x8e, 0x90, 0x91, 0x1a, 0x1a, 0x94, 0x95,
	0x1a, 0x1a, 0x1a, 0x1a, 0x99, 0x1a, 0x9a, 0xd6,
	0x1a, 0x9d, 0x1a, 0x9e, 0x9f, 0x1a, 0x1a, 0x1a
};

const uint8_t libuna_codepage_mac_greek_unicode_to_byte_stream_base_0x0380[ 80 ] = {
	0x1a, 0x1a, 0x1a, 0x1a, 0x8b, 0x87, 0xcd, 0x1a,
	0xce, 0xd7, 0xd8, 0x1a, 0xd9, 0x1a, 0xda, 0xdf,
	0xfd, 0xb0, 0xb5, 0xa1, 0xa2, 0xb6, 0xb7, 0xb8,
	0xa3, 0xb9, 0xba, 0xa4, 0xbb, 0xc1, 0xa5, 0xc3,
	0xa6, 0xc4, 0x1a, 0xaa, 0xc6, 0xcb, 0xbc, 0xcc,
	0xbe, 0xbf, 0xab, 0xbd, 0xc0, 0xdb, 0xdc, 0xdd,
	0xfe, 0xe1, 0xe2, 0xe7, 0xe4, 0xe5, 0xfa, 0xe8,
	0xf5, 0xe9, 0xeb, 0xec, 0xed, 0xee, 0xea, 0xef,
	0xf0, 0xf2, 0xf7, 0xf3, 0xf4, 0xf9, 0xe6, 0xf8,
	0xe3, 0xf6, 0xfb, 0xfc, 0xde, 0xe0, 0xf1, 0x1a
};

const uint8_t libuna_codepage_mac_greek_unicode_to_byte_stream_base_0x2010[ 40 ] = {
	0x1a, 0x1a, 0x1a, 0xd0, 0x1a, 0xd1, 0x1a, 0x1a,
	0xd4, 0xd5, 0x1a, 0x1a, 0xd2, 0xd3, 0x1a, 0x1a,
	0xa0, 0x1a, 0x96, 0x1a, 0x1a, 0x1a, 0xc9, 0x1a,
	0x1a, 0x1a, 0x1a, 0x1a, 0x1a, 0x1a, 0x1a, 0x1a,
	0x98, 0x1a, 0x1a, 0x1a, 0x1a, 0x1a, 0x1a, 0x1a
};

/* Copies an Unicode character from a MacGreek encoded byte stream
 * Returns 1 if successful or -1 on error
 */
int libuna_codepage_mac_greek_copy_from_byte_stream(
     libuna_unicode_character_t *unicode_character,
     const uint8_t *byte_stream,
     size_t byte_stream_size,
     size_t *byte_stream_index,
     libcerror_error_t **error )
{
	static char *function                             = "libuna_codepage_mac_greek_copy_from_byte_stream";
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

		safe_unicode_character = libuna_codepage_mac_greek_byte_stream_to_unicode_base_0x80[ byte_stream_character ];
	}
	*unicode_character = safe_unicode_character;
	*byte_stream_index = safe_byte_stream_index;

	return( 1 );
}

/* Copies an Unicode character to a MacGreek encoded byte stream
 * Returns 1 if successful or -1 on error
 */
int libuna_codepage_mac_greek_copy_to_byte_stream(
     libuna_unicode_character_t unicode_character,
     uint8_t *byte_stream,
     size_t byte_stream_size,
     size_t *byte_stream_index,
     libcerror_error_t **error )
{
	static char *function         = "libuna_codepage_mac_greek_copy_to_byte_stream";
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
	      && ( unicode_character < 0x0100 ) )
	{
		unicode_character -= 0x00a0;

		byte_stream_value = libuna_codepage_mac_greek_unicode_to_byte_stream_base_0x00a0[ unicode_character ];
	}
	else if( ( unicode_character >= 0x0380 )
	      && ( unicode_character < 0x03d0 ) )
	{
		unicode_character -= 0x0380;

		byte_stream_value = libuna_codepage_mac_greek_unicode_to_byte_stream_base_0x0380[ unicode_character ];
	}
	else if( ( unicode_character >= 0x2010 )
	      && ( unicode_character < 0x2038 ) )
	{
		unicode_character -= 0x2010;

		byte_stream_value = libuna_codepage_mac_greek_unicode_to_byte_stream_base_0x2010[ unicode_character ];
	}
	else switch( unicode_character )
	{
		case 0x0153:
			byte_stream_value = 0xcf;
			break;

		case 0x20ac:
			byte_stream_value = 0x9c;
			break;

		case 0x2122:
			byte_stream_value = 0x93;
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

