/*
 * MacArabic codepage functions
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

#include "libuna_codepage_mac_arabic.h"
#include "libuna_types.h"

/* Extended ASCII to Unicode character lookup table for the MacArabic codepage
 * Unknown are filled with the Unicode replacement character 0xfffd
 */
const uint16_t libuna_codepage_mac_arabic_byte_stream_to_unicode_base_0x80[ 128 ] = {
	0x00c4, 0x00a0, 0x00c7, 0x00c9, 0x00d1, 0x00d6, 0x00dc, 0x00e1,
	0x00e0, 0x00e2, 0x00e4, 0x06ba, 0x00ab, 0x00e7, 0x00e9, 0x00e8,
	0x00ea, 0x00eb, 0x00ed, 0x2026, 0x00ee, 0x00ef, 0x00f1, 0x00f3,
	0x00bb, 0x00f4, 0x00f6, 0x00f7, 0x00fa, 0x00f9, 0x00fb, 0x00fc,
	0x0020, 0x0021, 0x0022, 0x0023, 0x0024, 0x066a, 0x0026, 0x0027,
	0x0028, 0x0029, 0x002a, 0x002b, 0x060c, 0x002d, 0x002e, 0x002f,
	0x0660, 0x0661, 0x0662, 0x0663, 0x0664, 0x0665, 0x0666, 0x0667,
	0x0668, 0x0669, 0x003a, 0x061b, 0x003c, 0x003d, 0x003e, 0x061f,
	0x274a, 0x0621, 0x0622, 0x0623, 0x0624, 0x0625, 0x0626, 0x0627,
	0x0628, 0x0629, 0x062a, 0x062b, 0x062c, 0x062d, 0x062e, 0x062f,
	0x0630, 0x0631, 0x0632, 0x0633, 0x0634, 0x0635, 0x0636, 0x0637,
	0x0638, 0x0639, 0x063a, 0x005b, 0x005c, 0x005d, 0x005e, 0x005f,
	0x0640, 0x0641, 0x0642, 0x0643, 0x0644, 0x0645, 0x0646, 0x0647,
	0x0648, 0x0649, 0x064a, 0x064b, 0x064c, 0x064d, 0x064e, 0x064f,
	0x0650, 0x0651, 0x0652, 0x067e, 0x0679, 0x0686, 0x06d5, 0x06a4,
	0x06af, 0x0688, 0x0691, 0x007b, 0x007c, 0x007d, 0x0698, 0x06d2
};

/* Unicode to ASCII character lookup tables for the MacArabic codepage
 * Unknown are filled with the ASCII replacement character 0x1a
 */
const uint8_t libuna_codepage_mac_arabic_unicode_to_byte_stream_base_0x0020[ 32 ] = {
	0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0x25, 0xa6, 0xa7,
	0xa8, 0xa9, 0xaa, 0xab, 0x2c, 0xad, 0xae, 0xaf,
	0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
	0x38, 0x39, 0xba, 0x3b, 0xbc, 0xbd, 0xbe, 0x3f
};

const uint8_t libuna_codepage_mac_arabic_unicode_to_byte_stream_base_0x00a0[ 96 ] = {
	0x81, 0x1a, 0x1a, 0x1a, 0x1a, 0x1a, 0x1a, 0x1a,
	0x1a, 0x1a, 0x1a, 0x8c, 0x1a, 0x1a, 0x1a, 0x1a,
	0x1a, 0x1a, 0x1a, 0x1a, 0x1a, 0x1a, 0x1a, 0x1a,
	0x1a, 0x1a, 0x1a, 0x98, 0x1a, 0x1a, 0x1a, 0x1a,
	0x1a, 0x1a, 0x1a, 0x1a, 0x80, 0x1a, 0x1a, 0x82,
	0x1a, 0x83, 0x1a, 0x1a, 0x1a, 0x1a, 0x1a, 0x1a,
	0x1a, 0x84, 0x1a, 0x1a, 0x1a, 0x1a, 0x85, 0x1a,
	0x1a, 0x1a, 0x1a, 0x1a, 0x86, 0x1a, 0x1a, 0x1a,
	0x88, 0x87, 0x89, 0x1a, 0x8a, 0x1a, 0x1a, 0x8d,
	0x8f, 0x8e, 0x90, 0x91, 0x1a, 0x92, 0x94, 0x95,
	0x1a, 0x96, 0x1a, 0x97, 0x99, 0x1a, 0x9a, 0x9b,
	0x1a, 0x9d, 0x9c, 0x9e, 0x9f, 0x1a, 0x1a, 0x1a
};

const uint8_t libuna_codepage_mac_arabic_unicode_to_byte_stream_base_0x0608[ 184 ] = {
	0x1a, 0x1a, 0x1a, 0x1a, 0xac, 0x1a, 0x1a, 0x1a,
	0x1a, 0x1a, 0x1a, 0x1a, 0x1a, 0x1a, 0x1a, 0x1a,
	0x1a, 0x1a, 0x1a, 0xbb, 0x1a, 0x1a, 0x1a, 0xbf,
	0x1a, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7,
	0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf,
	0xd0, 0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd7,
	0xd8, 0xd9, 0xda, 0x1a, 0x1a, 0x1a, 0x1a, 0x1a,
	0xe0, 0xe1, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7,
	0xe8, 0xe9, 0xea, 0xeb, 0xec, 0xed, 0xee, 0xef,
	0xf0, 0xf1, 0xf2, 0x1a, 0x1a, 0x1a, 0x1a, 0x1a,
	0x1a, 0x1a, 0x1a, 0x1a, 0x1a, 0x1a, 0x1a, 0x1a,
	0xb0, 0xb1, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7,
	0xb8, 0xb9, 0xa5, 0x1a, 0x1a, 0x1a, 0x1a, 0x1a,
	0x1a, 0x1a, 0x1a, 0x1a, 0x1a, 0x1a, 0x1a, 0x1a,
	0x1a, 0xf4, 0x1a, 0x1a, 0x1a, 0x1a, 0xf3, 0x1a,
	0x1a, 0x1a, 0x1a, 0x1a, 0x1a, 0x1a, 0xf5, 0x1a,
	0xf9, 0x1a, 0x1a, 0x1a, 0x1a, 0x1a, 0x1a, 0x1a,
	0x1a, 0xfa, 0x1a, 0x1a, 0x1a, 0x1a, 0x1a, 0x1a,
	0xfe, 0x1a, 0x1a, 0x1a, 0x1a, 0x1a, 0x1a, 0x1a,
	0x1a, 0x1a, 0x1a, 0x1a, 0xf7, 0x1a, 0x1a, 0x1a,
	0x1a, 0x1a, 0x1a, 0x1a, 0x1a, 0x1a, 0x1a, 0xf8,
	0x1a, 0x1a, 0x1a, 0x1a, 0x1a, 0x1a, 0x1a, 0x1a,
	0x1a, 0x1a, 0x8b, 0x1a, 0x1a, 0x1a, 0x1a, 0x1a
};

/* Copies an Unicode character from a MacArabic encoded byte stream
 * Returns 1 if successful or -1 on error
 */
int libuna_codepage_mac_arabic_copy_from_byte_stream(
     libuna_unicode_character_t *unicode_character,
     const uint8_t *byte_stream,
     size_t byte_stream_size,
     size_t *byte_stream_index,
     libcerror_error_t **error )
{
	static char *function                             = "libuna_codepage_mac_arabic_copy_from_byte_stream";
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

		safe_unicode_character = libuna_codepage_mac_arabic_byte_stream_to_unicode_base_0x80[ byte_stream_character ];
	}
	*unicode_character = safe_unicode_character;
	*byte_stream_index = safe_byte_stream_index;

	return( 1 );
}

/* Copies an Unicode character to a MacArabic encoded byte stream
 * Returns 1 if successful or -1 on error
 */
int libuna_codepage_mac_arabic_copy_to_byte_stream(
     libuna_unicode_character_t unicode_character,
     uint8_t *byte_stream,
     size_t byte_stream_size,
     size_t *byte_stream_index,
     libcerror_error_t **error )
{
	static char *function         = "libuna_codepage_mac_arabic_copy_to_byte_stream";
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
	if( unicode_character < 0x0020 )
	{
		byte_stream_value = (uint16_t) unicode_character;
	}
	else if( ( unicode_character >= 0x0020 )
	      && ( unicode_character < 0x0040 ) )
	{
		unicode_character -= 0x0020;

		byte_stream_value = libuna_codepage_mac_arabic_unicode_to_byte_stream_base_0x0020[ unicode_character ];
	}
	else if( ( unicode_character >= 0x0040 )
	      && ( unicode_character < 0x005b ) )
	{
		byte_stream_value = (uint16_t) unicode_character;
	}
	else if( ( unicode_character >= 0x0060 )
	      && ( unicode_character < 0x007b ) )
	{
		byte_stream_value = (uint16_t) unicode_character;
	}
	else if( ( unicode_character >= 0x007e )
	      && ( unicode_character < 0x0080 ) )
	{
		byte_stream_value = (uint16_t) unicode_character;
	}
	else if( ( unicode_character >= 0x00a0 )
	      && ( unicode_character < 0x0100 ) )
	{
		unicode_character -= 0x00a0;

		byte_stream_value = libuna_codepage_mac_arabic_unicode_to_byte_stream_base_0x00a0[ unicode_character ];
	}
	else if( ( unicode_character >= 0x0608 )
	      && ( unicode_character < 0x06c0 ) )
	{
		unicode_character -= 0x0608;

		byte_stream_value = libuna_codepage_mac_arabic_unicode_to_byte_stream_base_0x0608[ unicode_character ];
	}
	else switch( unicode_character )
	{
		case 0x005b:
			byte_stream_value = 0xdb;
			break;

		case 0x005c:
			byte_stream_value = 0xdc;
			break;

		case 0x005d:
			byte_stream_value = 0xdd;
			break;

		case 0x005e:
			byte_stream_value = 0xde;
			break;

		case 0x005f:
			byte_stream_value = 0xdf;
			break;

		case 0x007b:
			byte_stream_value = 0xfb;
			break;

		case 0x007c:
			byte_stream_value = 0xfc;
			break;

		case 0x007d:
			byte_stream_value = 0xfd;
			break;

		case 0x06d2:
			byte_stream_value = 0xff;
			break;

		case 0x06d5:
			byte_stream_value = 0xf6;
			break;

		case 0x2026:
			byte_stream_value = 0x93;
			break;

		case 0x274a:
			byte_stream_value = 0xc0;
			break;

		default:
			byte_stream_value = 0x1a;
			break;
	}
	byte_stream[ safe_byte_stream_index++ ] = (uint8_t) ( byte_stream_value & 0x00ff );

	*byte_stream_index = safe_byte_stream_index;

	return( 1 );
}

