/*
 * MacSymbol codepage functions
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

#include "libuna_codepage_mac_symbol.h"
#include "libuna_types.h"

/* Extended ASCII to Unicode character lookup table for the MacSymbol codepage
 * Unknown are filled with the Unicode replacement character 0xfffd
 */
const uint16_t libuna_codepage_mac_symbol_byte_stream_to_unicode_base_0x20[ 224 ] = {
	0x0020, 0x0021, 0x2200, 0x0023, 0x2203, 0x0025, 0x0026, 0x220d,
	0x0028, 0x0029, 0x2217, 0x002b, 0x002c, 0x2212, 0x002e, 0x002f,
	0x0030, 0x0031, 0x0032, 0x0033, 0x0034, 0x0035, 0x0036, 0x0037,
	0x0038, 0x0039, 0x003a, 0x003b, 0x003c, 0x003d, 0x003e, 0x003f,
	0x2245, 0x0391, 0x0392, 0x03a7, 0x0394, 0x0395, 0x03a6, 0x0393,
	0x0397, 0x0399, 0x03d1, 0x039a, 0x039b, 0x039c, 0x039d, 0x039f,
	0x03a0, 0x0398, 0x03a1, 0x03a3, 0x03a4, 0x03a5, 0x03c2, 0x03a9,
	0x039e, 0x03a8, 0x0396, 0x005b, 0x2234, 0x005d, 0x22a5, 0x005f,
	0xf8e5, 0x03b1, 0x03b2, 0x03c7, 0x03b4, 0x03b5, 0x03c6, 0x03b3,
	0x03b7, 0x03b9, 0x03d5, 0x03ba, 0x03bb, 0x03bc, 0x03bd, 0x03bf,
	0x03c0, 0x03b8, 0x03c1, 0x03c3, 0x03c4, 0x03c5, 0x03d6, 0x03c9,
	0x03be, 0x03c8, 0x03b6, 0x007b, 0x007c, 0x007d, 0x223c, 0x007f,
	0xfffd, 0xfffd, 0xfffd, 0xfffd, 0xfffd, 0xfffd, 0xfffd, 0xfffd,
	0xfffd, 0xfffd, 0xfffd, 0xfffd, 0xfffd, 0xfffd, 0xfffd, 0xfffd,
	0xfffd, 0xfffd, 0xfffd, 0xfffd, 0xfffd, 0xfffd, 0xfffd, 0xfffd,
	0xfffd, 0xfffd, 0xfffd, 0xfffd, 0xfffd, 0xfffd, 0xfffd, 0xfffd,
	0x20ac, 0x03d2, 0x2032, 0x2264, 0x2044, 0x221e, 0x0192, 0x2663,
	0x2666, 0x2665, 0x2660, 0x2194, 0x2190, 0x2191, 0x2192, 0x2193,
	0x00b0, 0x00b1, 0x2033, 0x2265, 0x00d7, 0x221d, 0x2202, 0x2022,
	0x00f7, 0x2260, 0x2261, 0x2248, 0x2026, 0x23d0, 0x23af, 0x21b5,
	0x2135, 0x2111, 0x211c, 0x2118, 0x2297, 0x2295, 0x2205, 0x2229,
	0x222a, 0x2283, 0x2287, 0x2284, 0x2282, 0x2286, 0x2208, 0x2209,
	0x2220, 0x2207, 0x00ae, 0x00a9, 0x2122, 0x220f, 0x221a, 0x22c5,
	0x00ac, 0x2227, 0x2228, 0x21d4, 0x21d0, 0x21d1, 0x21d2, 0x21d3,
	0x25ca, 0x3008, 0x00ae, 0x00a9, 0x2122, 0x2211, 0x239b, 0x239c,
	0x239d, 0x23a1, 0x23a2, 0x23a3, 0x23a7, 0x23a8, 0x23a9, 0x23aa,
	0xf8ff, 0x3009, 0x222b, 0x2320, 0x23ae, 0x2321, 0x239e, 0x239f,
	0x23a0, 0x23a4, 0x23a5, 0x23a6, 0x23ab, 0x23ac, 0x23ad, 0xfffd
};

/* Unicode to ASCII character lookup tables for the MacSymbol codepage
 * Unknown are filled with the ASCII replacement character 0x1a
 */
const uint8_t libuna_codepage_mac_symbol_unicode_to_byte_stream_base_0x0390[ 72 ] = {
	0x1a, 0x41, 0x42, 0x47, 0x44, 0x45, 0x5a, 0x48,
	0x51, 0x49, 0x4b, 0x4c, 0x4d, 0x4e, 0x58, 0x4f,
	0x50, 0x52, 0x1a, 0x53, 0x54, 0x55, 0x46, 0x43,
	0x59, 0x57, 0x1a, 0x1a, 0x1a, 0x1a, 0x1a, 0x1a,
	0x1a, 0x61, 0x62, 0x67, 0x64, 0x65, 0x7a, 0x68,
	0x71, 0x69, 0x6b, 0x6c, 0x6d, 0x6e, 0x78, 0x6f,
	0x70, 0x72, 0x56, 0x73, 0x74, 0x75, 0x66, 0x63,
	0x79, 0x77, 0x1a, 0x1a, 0x1a, 0x1a, 0x1a, 0x1a,
	0x1a, 0x4a, 0xa1, 0x1a, 0x1a, 0x6a, 0x76, 0x1a
};

const uint8_t libuna_codepage_mac_symbol_unicode_to_byte_stream_base_0x2200[ 80 ] = {
	0x22, 0x1a, 0xb6, 0x24, 0x1a, 0xc6, 0x1a, 0xd1,
	0xce, 0xcf, 0x1a, 0x1a, 0x1a, 0x27, 0x1a, 0xd5,
	0x1a, 0xe5, 0x2d, 0x1a, 0x1a, 0x1a, 0x1a, 0x2a,
	0x1a, 0x1a, 0xd6, 0x1a, 0x1a, 0xb5, 0xa5, 0x1a,
	0xd0, 0x1a, 0x1a, 0x1a, 0x1a, 0x1a, 0x1a, 0xd9,
	0xda, 0xc7, 0xc8, 0xf2, 0x1a, 0x1a, 0x1a, 0x1a,
	0x1a, 0x1a, 0x1a, 0x1a, 0x5c, 0x1a, 0x1a, 0x1a,
	0x1a, 0x1a, 0x1a, 0x1a, 0x7e, 0x1a, 0x1a, 0x1a,
	0x1a, 0x1a, 0x1a, 0x1a, 0x1a, 0x40, 0x1a, 0x1a,
	0xbb, 0x1a, 0x1a, 0x1a, 0x1a, 0x1a, 0x1a, 0x1a
};

const uint8_t libuna_codepage_mac_symbol_unicode_to_byte_stream_base_0x2280[ 40 ] = {
	0x1a, 0x1a, 0xcc, 0xc9, 0xcb, 0x1a, 0xcd, 0xca,
	0x1a, 0x1a, 0x1a, 0x1a, 0x1a, 0x1a, 0x1a, 0x1a,
	0x1a, 0x1a, 0x1a, 0x1a, 0x1a, 0xc5, 0x1a, 0xc4,
	0x1a, 0x1a, 0x1a, 0x1a, 0x1a, 0x1a, 0x1a, 0x1a,
	0x1a, 0x1a, 0x1a, 0x1a, 0x1a, 0x5e, 0x1a, 0x1a
};

const uint8_t libuna_codepage_mac_symbol_unicode_to_byte_stream_base_0x2398[ 24 ] = {
	0x1a, 0x1a, 0x1a, 0xe6, 0xe7, 0xe8, 0xf6, 0xf7,
	0xf8, 0xe9, 0xea, 0xeb, 0xf9, 0xfa, 0xfb, 0xec,
	0xed, 0xee, 0xef, 0xfc, 0xfd, 0xfe, 0xf4, 0xbe
};

/* Determines the size of a MacSymbol encoded byte stream from an Unicode character
 * Adds the size to the byte stream character size value
 * Returns 1 if successful, 0 if the byte stream character is valid but not supported since it requires special handling or -1 on error
 */
int libuna_codepage_mac_symbol_unicode_character_size_to_byte_stream(
     libuna_unicode_character_t unicode_character,
     size_t *byte_stream_character_size,
     libcerror_error_t **error )
{
	static char *function = "libuna_codepage_mac_symbol_unicode_character_size_to_byte_stream";
	int result            = 0;

	if( byte_stream_character_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid byte stream character size.",
		 function );

		return( -1 );
	}
	switch( unicode_character )
	{
		case 0x000000aeUL:
		case 0x000000a9UL:
		case 0x00002122UL:
			result = 0;
			break;

		default:
			*byte_stream_character_size += 1;

			result = 1;
			break;
	}
	return( result );
}

/* Copies an Unicode character from a MacSymbol encoded byte stream
 * Returns 1 if successful or -1 on error
 */
int libuna_codepage_mac_symbol_copy_from_byte_stream(
     libuna_unicode_character_t *unicode_character,
     const uint8_t *byte_stream,
     size_t byte_stream_size,
     size_t *byte_stream_index,
     libcerror_error_t **error )
{
	static char *function                             = "libuna_codepage_mac_symbol_copy_from_byte_stream";
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

	if( byte_stream_character < 0x20 )
	{
		safe_unicode_character = byte_stream_character;
	}
	else
	{
		byte_stream_character -= 0x20;

		safe_unicode_character = libuna_codepage_mac_symbol_byte_stream_to_unicode_base_0x20[ byte_stream_character ];
	}
	*unicode_character = safe_unicode_character;
	*byte_stream_index = safe_byte_stream_index;

	return( 1 );
}

/* Copies an Unicode character to a MacSymbol encoded byte stream
 * Returns 1 if successful or -1 on error
 */
int libuna_codepage_mac_symbol_copy_to_byte_stream(
     libuna_unicode_character_t unicode_character,
     uint8_t *byte_stream,
     size_t byte_stream_size,
     size_t *byte_stream_index,
     libcerror_error_t **error )
{
	static char *function         = "libuna_codepage_mac_symbol_copy_to_byte_stream";
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
	if( ( unicode_character <= 0x0021 )
	 || ( unicode_character == 0x0023 )
	 || ( unicode_character == 0x0025 )
	 || ( unicode_character == 0x0026 )
	 || ( unicode_character == 0x0028 )
	 || ( unicode_character == 0x0029 )
	 || ( unicode_character == 0x002b )
	 || ( unicode_character == 0x002c )
	 || ( ( unicode_character >= 0x002e )
	  &&  ( unicode_character <= 0x003f ) )
	 || ( unicode_character == 0x005b )
	 || ( unicode_character == 0x005d )
	 || ( unicode_character == 0x005f )
	 || ( unicode_character == 0x007b )
	 || ( unicode_character == 0x007c )
	 || ( unicode_character == 0x007d )
	 || ( unicode_character == 0x007f )
	 || ( unicode_character == 0x00b0 )
	 || ( unicode_character == 0x00b1 ) )
	{
		byte_stream_value = (uint16_t) unicode_character;
	}
	else if( ( unicode_character >= 0x0390 )
	      && ( unicode_character < 0x03d8 ) )
	{
		unicode_character -= 0x0390;

		byte_stream_value = libuna_codepage_mac_symbol_unicode_to_byte_stream_base_0x0390[ unicode_character ];
	}
	else if( ( unicode_character >= 0x2200 )
	      && ( unicode_character < 0x2250 ) )
	{
		unicode_character -= 0x2200;

		byte_stream_value = libuna_codepage_mac_symbol_unicode_to_byte_stream_base_0x2200[ unicode_character ];
	}
	else if( ( unicode_character >= 0x2280 )
	      && ( unicode_character < 0x22a8 ) )
	{
		unicode_character -= 0x2280;

		byte_stream_value = libuna_codepage_mac_symbol_unicode_to_byte_stream_base_0x2280[ unicode_character ];
	}
	else if( ( unicode_character >= 0x2398 )
	      && ( unicode_character < 0x23b0 ) )
	{
		unicode_character -= 0x2398;

		byte_stream_value = libuna_codepage_mac_symbol_unicode_to_byte_stream_base_0x2398[ unicode_character ];
	}
	else switch( unicode_character )
	{
		case 0x00a9:
			byte_stream_value = 0xd3;
			break;

		case 0x00ac:
			byte_stream_value = 0xd8;
			break;

		case 0x00ae:
			byte_stream_value = 0xd2;
			break;

		case 0x00d7:
			byte_stream_value = 0xb4;
			break;

		case 0x00f7:
			byte_stream_value = 0xb8;
			break;

		case 0x0192:
			byte_stream_value = 0xa6;
			break;

		case 0x2022:
			byte_stream_value = 0xb7;
			break;

		case 0x2026:
			byte_stream_value = 0xbc;
			break;

		case 0x2032:
			byte_stream_value = 0xa2;
			break;

		case 0x2033:
			byte_stream_value = 0xb2;
			break;

		case 0x2044:
			byte_stream_value = 0xa4;
			break;

		case 0x20ac:
			byte_stream_value = 0xa0;
			break;

		case 0x2111:
			byte_stream_value = 0xc1;
			break;

		case 0x2118:
			byte_stream_value = 0xc3;
			break;

		case 0x211c:
			byte_stream_value = 0xc2;
			break;

		case 0x2122:
			byte_stream_value = 0xd4;
			break;

		case 0x2135:
			byte_stream_value = 0xc0;
			break;

		case 0x2190:
			byte_stream_value = 0xac;
			break;

		case 0x2191:
			byte_stream_value = 0xad;
			break;

		case 0x2192:
			byte_stream_value = 0xae;
			break;

		case 0x2193:
			byte_stream_value = 0xaf;
			break;

		case 0x2194:
			byte_stream_value = 0xab;
			break;

		case 0x21b5:
			byte_stream_value = 0xbf;
			break;

		case 0x21d0:
			byte_stream_value = 0xdc;
			break;

		case 0x21d1:
			byte_stream_value = 0xdd;
			break;

		case 0x21d2:
			byte_stream_value = 0xde;
			break;

		case 0x21d3:
			byte_stream_value = 0xdf;
			break;

		case 0x21d4:
			byte_stream_value = 0xdb;
			break;

		case 0x2260:
			byte_stream_value = 0xb9;
			break;

		case 0x2261:
			byte_stream_value = 0xba;
			break;

		case 0x2264:
			byte_stream_value = 0xa3;
			break;

		case 0x2265:
			byte_stream_value = 0xb3;
			break;

		case 0x22c5:
			byte_stream_value = 0xd7;
			break;

		case 0x2320:
			byte_stream_value = 0xf3;
			break;

		case 0x2321:
			byte_stream_value = 0xf5;
			break;

		case 0x23d0:
			byte_stream_value = 0xbd;
			break;

		case 0x25ca:
			byte_stream_value = 0xe0;
			break;

		case 0x2660:
			byte_stream_value = 0xaa;
			break;

		case 0x2663:
			byte_stream_value = 0xa7;
			break;

		case 0x2665:
			byte_stream_value = 0xa9;
			break;

		case 0x2666:
			byte_stream_value = 0xa8;
			break;

		case 0x3008:
			byte_stream_value = 0xe1;
			break;

		case 0x3009:
			byte_stream_value = 0xf1;
			break;

		case 0xf8e5:
			byte_stream_value = 0x60;
			break;

		case 0xf8ff:
			byte_stream_value = 0xf0;
			break;

		default:
			byte_stream_value = 0x1a;
			break;
	}
	byte_stream[ safe_byte_stream_index++ ] = (uint8_t) ( byte_stream_value & 0x00ff );

	*byte_stream_index = safe_byte_stream_index;

	return( 1 );
}

