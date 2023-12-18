/*
 * Unicode character functions
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

#include "libuna_base64_stream.h"
#include "libuna_codepage_iso_8859_2.h"
#include "libuna_codepage_iso_8859_3.h"
#include "libuna_codepage_iso_8859_4.h"
#include "libuna_codepage_iso_8859_5.h"
#include "libuna_codepage_iso_8859_6.h"
#include "libuna_codepage_iso_8859_7.h"
#include "libuna_codepage_iso_8859_8.h"
#include "libuna_codepage_iso_8859_9.h"
#include "libuna_codepage_iso_8859_10.h"
#include "libuna_codepage_iso_8859_13.h"
#include "libuna_codepage_iso_8859_14.h"
#include "libuna_codepage_iso_8859_15.h"
#include "libuna_codepage_iso_8859_16.h"
#include "libuna_codepage_koi8_r.h"
#include "libuna_codepage_koi8_u.h"
#include "libuna_codepage_mac_arabic.h"
#include "libuna_codepage_mac_celtic.h"
#include "libuna_codepage_mac_centraleurroman.h"
#include "libuna_codepage_mac_croatian.h"
#include "libuna_codepage_mac_cyrillic.h"
#include "libuna_codepage_mac_dingbats.h"
#include "libuna_codepage_mac_farsi.h"
#include "libuna_codepage_mac_gaelic.h"
#include "libuna_codepage_mac_greek.h"
#include "libuna_codepage_mac_icelandic.h"
#include "libuna_codepage_mac_inuit.h"
#include "libuna_codepage_mac_roman.h"
#include "libuna_codepage_mac_romanian.h"
#include "libuna_codepage_mac_russian.h"
#include "libuna_codepage_mac_symbol.h"
#include "libuna_codepage_mac_thai.h"
#include "libuna_codepage_mac_turkish.h"
#include "libuna_codepage_mac_ukrainian.h"
#include "libuna_codepage_windows_874.h"
#include "libuna_codepage_windows_932.h"
#include "libuna_codepage_windows_936.h"
#include "libuna_codepage_windows_949.h"
#include "libuna_codepage_windows_950.h"
#include "libuna_codepage_windows_1250.h"
#include "libuna_codepage_windows_1251.h"
#include "libuna_codepage_windows_1252.h"
#include "libuna_codepage_windows_1253.h"
#include "libuna_codepage_windows_1254.h"
#include "libuna_codepage_windows_1255.h"
#include "libuna_codepage_windows_1256.h"
#include "libuna_codepage_windows_1257.h"
#include "libuna_codepage_windows_1258.h"
#include "libuna_definitions.h"
#include "libuna_types.h"
#include "libuna_unicode_character.h"
#include "libuna_unused.h"

/* Valid directly encoded characters: A-Z, a-z, 0-9, '\', '(', ')', ',', '-', '.', '/', ':', '?'
 * Valid directly encoded whitespace: '\t', '\n', '\r', ' '
 * Valid optional directly encoded characters: '!', '"', '#', '$', '%', '&', '*', ';', '<', '=', '>', '@', '[', ']', '^', '_', '`', '{', '|', '}'
 */
uint8_t libuna_unicode_character_utf7_valid_directly_encoded_character[ 256 ] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

/* Valid UTF-7 base64 characters: A-Z, a-z, 0-9, '+' and '/'
 */
uint8_t libuna_unicode_character_utf7_valid_base64_character[ 256 ] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,
	0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
	0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

/* Determines the size of a byte stream character from an Unicode character
 * Adds the size to the byte stream character size value
 * Returns 1 if successful, 0 if the byte stream character is valid but not supported since it requires special handling or -1 on error
 */
int libuna_unicode_character_size_to_byte_stream(
     libuna_unicode_character_t unicode_character,
     int codepage,
     size_t *byte_stream_character_size,
     libcerror_error_t **error )
{
	static char *function                  = "libuna_unicode_character_size_to_byte_stream";
	size_t safe_byte_stream_character_size = 0;
	int result                             = 1;

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
	safe_byte_stream_character_size = *byte_stream_character_size;

	switch( codepage )
	{
		case LIBUNA_CODEPAGE_ASCII:
		case LIBUNA_CODEPAGE_ISO_8859_1:
		case LIBUNA_CODEPAGE_ISO_8859_2:
		case LIBUNA_CODEPAGE_ISO_8859_3:
		case LIBUNA_CODEPAGE_ISO_8859_4:
		case LIBUNA_CODEPAGE_ISO_8859_5:
		case LIBUNA_CODEPAGE_ISO_8859_6:
		case LIBUNA_CODEPAGE_ISO_8859_7:
		case LIBUNA_CODEPAGE_ISO_8859_8:
		case LIBUNA_CODEPAGE_ISO_8859_9:
		case LIBUNA_CODEPAGE_ISO_8859_10:
		case LIBUNA_CODEPAGE_ISO_8859_11:
		case LIBUNA_CODEPAGE_ISO_8859_13:
		case LIBUNA_CODEPAGE_ISO_8859_14:
		case LIBUNA_CODEPAGE_ISO_8859_15:
		case LIBUNA_CODEPAGE_ISO_8859_16:
		case LIBUNA_CODEPAGE_KOI8_R:
		case LIBUNA_CODEPAGE_KOI8_U:
		case LIBUNA_CODEPAGE_MAC_ARABIC:
		case LIBUNA_CODEPAGE_MAC_CELTIC:
		case LIBUNA_CODEPAGE_MAC_CENTRALEURROMAN:
		case LIBUNA_CODEPAGE_MAC_CROATIAN:
		case LIBUNA_CODEPAGE_MAC_CYRILLIC:
		case LIBUNA_CODEPAGE_MAC_DINGBATS:
		case LIBUNA_CODEPAGE_MAC_FARSI:
		case LIBUNA_CODEPAGE_MAC_GAELIC:
		case LIBUNA_CODEPAGE_MAC_GREEK:
		case LIBUNA_CODEPAGE_MAC_ICELANDIC:
		case LIBUNA_CODEPAGE_MAC_INUIT:
		case LIBUNA_CODEPAGE_MAC_ROMAN:
		case LIBUNA_CODEPAGE_MAC_ROMANIAN:
		case LIBUNA_CODEPAGE_MAC_RUSSIAN:
		case LIBUNA_CODEPAGE_MAC_TURKISH:
		case LIBUNA_CODEPAGE_MAC_UKRAINIAN:
		case LIBUNA_CODEPAGE_WINDOWS_874:
		case LIBUNA_CODEPAGE_WINDOWS_1250:
		case LIBUNA_CODEPAGE_WINDOWS_1251:
		case LIBUNA_CODEPAGE_WINDOWS_1252:
		case LIBUNA_CODEPAGE_WINDOWS_1253:
		case LIBUNA_CODEPAGE_WINDOWS_1254:
		case LIBUNA_CODEPAGE_WINDOWS_1255:
		case LIBUNA_CODEPAGE_WINDOWS_1256:
		case LIBUNA_CODEPAGE_WINDOWS_1257:
		case LIBUNA_CODEPAGE_WINDOWS_1258:
			safe_byte_stream_character_size += 1;
			break;

		case LIBUNA_CODEPAGE_MAC_SYMBOL:
			result = libuna_codepage_mac_symbol_unicode_character_size_to_byte_stream(
			          unicode_character,
			          &safe_byte_stream_character_size,
			          error );
			break;

		case LIBUNA_CODEPAGE_MAC_THAI:
			result = libuna_codepage_mac_thai_unicode_character_size_to_byte_stream(
			          unicode_character,
			          &safe_byte_stream_character_size,
			          error );
			break;

		case LIBUNA_CODEPAGE_WINDOWS_932:
			result = libuna_codepage_windows_932_unicode_character_size_to_byte_stream(
			          unicode_character,
			          &safe_byte_stream_character_size,
			          error );
			break;

		case LIBUNA_CODEPAGE_WINDOWS_936:
			result = libuna_codepage_windows_936_unicode_character_size_to_byte_stream(
			          unicode_character,
			          &safe_byte_stream_character_size,
			          error );
			break;

		case LIBUNA_CODEPAGE_WINDOWS_949:
			result = libuna_codepage_windows_949_unicode_character_size_to_byte_stream(
			          unicode_character,
			          &safe_byte_stream_character_size,
			          error );
			break;

		case LIBUNA_CODEPAGE_WINDOWS_950:
			result = libuna_codepage_windows_950_unicode_character_size_to_byte_stream(
			          unicode_character,
			          &safe_byte_stream_character_size,
			          error );
			break;

		default:
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported codepage: %d.",
			 function,
			 codepage );

			return( -1 );
	}
	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine byte stream character size.",
		 function );

		return( -1 );
	}
	*byte_stream_character_size = safe_byte_stream_character_size;

	return( result );
}

/* Copies an Unicode character from a byte stream
 * Returns 1 if successful, 0 if the byte stream character is valid but not supported since it requires special handling or -1 on error
 */
int libuna_unicode_character_copy_from_byte_stream(
     libuna_unicode_character_t *unicode_character,
     const uint8_t *byte_stream,
     size_t byte_stream_size,
     size_t *byte_stream_index,
     int codepage,
     libcerror_error_t **error )
{
	static char *function                             = "libuna_unicode_character_copy_from_byte_stream";
	libuna_unicode_character_t safe_unicode_character = 0;
	size_t safe_byte_stream_index                     = 0;
	uint8_t byte_stream_character                     = 0;
	int result                                        = 1;

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
	byte_stream_character = byte_stream[ safe_byte_stream_index ];

	switch( codepage )
	{
		case LIBUNA_CODEPAGE_ASCII:
			if( byte_stream_character < 0x80 )
			{
				safe_unicode_character = byte_stream_character;
			}
			else
			{
				safe_unicode_character = 0xfffd;
			}
			safe_byte_stream_index += 1;

			break;

		case LIBUNA_CODEPAGE_ISO_8859_1:
			safe_unicode_character = byte_stream_character;

			safe_byte_stream_index += 1;

			break;

		case LIBUNA_CODEPAGE_ISO_8859_2:
			if( byte_stream_character < 0xa0 )
			{
				safe_unicode_character = byte_stream_character;
			}
			else
			{
				byte_stream_character -= 0xa0;

				safe_unicode_character = libuna_codepage_iso_8859_2_byte_stream_to_unicode_base_0xa0[ byte_stream_character ];
			}
			safe_byte_stream_index += 1;

			break;

		case LIBUNA_CODEPAGE_ISO_8859_3:
			if( byte_stream_character < 0xa0 )
			{
				safe_unicode_character = byte_stream_character;
			}
			else
			{
				byte_stream_character -= 0xa0;

				safe_unicode_character = libuna_codepage_iso_8859_3_byte_stream_to_unicode_base_0xa0[ byte_stream_character ];
			}
			safe_byte_stream_index += 1;

			break;

		case LIBUNA_CODEPAGE_ISO_8859_4:
			if( byte_stream_character < 0xa0 )
			{
				safe_unicode_character = byte_stream_character;
			}
			else
			{
				byte_stream_character -= 0xa0;

				safe_unicode_character = libuna_codepage_iso_8859_4_byte_stream_to_unicode_base_0xa0[ byte_stream_character ];
			}
			safe_byte_stream_index += 1;

			break;

		case LIBUNA_CODEPAGE_ISO_8859_5:
			if( byte_stream_character < 0xa0 )
			{
				safe_unicode_character = byte_stream_character;
			}
			else
			{
				byte_stream_character -= 0xa0;

				safe_unicode_character = libuna_codepage_iso_8859_5_byte_stream_to_unicode_base_0xa0[ byte_stream_character ];
			}
			safe_byte_stream_index += 1;

			break;

		case LIBUNA_CODEPAGE_ISO_8859_6:
			if( byte_stream_character < 0xa0 )
			{
				safe_unicode_character = byte_stream_character;
			}
			else
			{
				byte_stream_character -= 0xa0;

				safe_unicode_character = libuna_codepage_iso_8859_6_byte_stream_to_unicode_base_0xa0[ byte_stream_character ];
			}
			safe_byte_stream_index += 1;

			break;

		case LIBUNA_CODEPAGE_ISO_8859_7:
			if( byte_stream_character < 0xa0 )
			{
				safe_unicode_character = byte_stream_character;
			}
			else
			{
				byte_stream_character -= 0xa0;

				safe_unicode_character = libuna_codepage_iso_8859_7_byte_stream_to_unicode_base_0xa0[ byte_stream_character ];
			}
			safe_byte_stream_index += 1;

			break;

		case LIBUNA_CODEPAGE_ISO_8859_8:
			if( byte_stream_character < 0xa0 )
			{
				safe_unicode_character = byte_stream_character;
			}
			else
			{
				byte_stream_character -= 0xa0;

				safe_unicode_character = libuna_codepage_iso_8859_8_byte_stream_to_unicode_base_0xa0[ byte_stream_character ];
			}
			safe_byte_stream_index += 1;

			break;

		case LIBUNA_CODEPAGE_ISO_8859_9:
			if( byte_stream_character < 0xd0 )
			{
				safe_unicode_character = byte_stream_character;
			}
			else
			{
				byte_stream_character -= 0xd0;

				safe_unicode_character = libuna_codepage_iso_8859_9_byte_stream_to_unicode_base_0xd0[ byte_stream_character ];
			}
			safe_byte_stream_index += 1;

			break;

		case LIBUNA_CODEPAGE_ISO_8859_10:
			if( byte_stream_character < 0xa0 )
			{
				safe_unicode_character = byte_stream_character;
			}
			else
			{
				byte_stream_character -= 0xa0;

				safe_unicode_character = libuna_codepage_iso_8859_10_byte_stream_to_unicode_base_0xa0[ byte_stream_character ];
			}
			safe_byte_stream_index += 1;

			break;

		case LIBUNA_CODEPAGE_ISO_8859_11:
			if( byte_stream_character < 0xa1 )
			{
				safe_unicode_character = byte_stream_character;
			}
			else if( byte_stream_character < 0xdb )
			{
				safe_unicode_character = byte_stream_character + 0x0d60;
			}
			else if( byte_stream_character < 0xdf )
			{
				safe_unicode_character = 0xfffd;
			}
			else if( byte_stream_character < 0xfc )
			{
				safe_unicode_character = byte_stream_character + 0x0d60;
			}
			else
			{
				safe_unicode_character = 0xfffd;
			}
			safe_byte_stream_index += 1;

			break;

		case LIBUNA_CODEPAGE_ISO_8859_13:
			if( byte_stream_character < 0xa0 )
			{
				safe_unicode_character = byte_stream_character;
			}
			else
			{
				byte_stream_character -= 0xa0;

				safe_unicode_character = libuna_codepage_iso_8859_13_byte_stream_to_unicode_base_0xa0[ byte_stream_character ];
			}
			safe_byte_stream_index += 1;

			break;

		case LIBUNA_CODEPAGE_ISO_8859_14:
			if( byte_stream_character < 0xa0 )
			{
				safe_unicode_character = byte_stream_character;
			}
			else
			{
				byte_stream_character -= 0xa0;

				safe_unicode_character = libuna_codepage_iso_8859_14_byte_stream_to_unicode_base_0xa0[ byte_stream_character ];
			}
			safe_byte_stream_index += 1;

			break;

		case LIBUNA_CODEPAGE_ISO_8859_15:
			if( ( byte_stream_character >= 0xa0 )
			 && ( byte_stream_character < 0xc0 ) )
			{
				byte_stream_character -= 0xa0;

				safe_unicode_character = libuna_codepage_iso_8859_15_byte_stream_to_unicode_base_0xa0[ byte_stream_character ];
			}
			else
			{
				safe_unicode_character = byte_stream_character;
			}
			safe_byte_stream_index += 1;

			break;

		case LIBUNA_CODEPAGE_ISO_8859_16:
			if( byte_stream_character < 0xa0 )
			{
				safe_unicode_character = byte_stream_character;
			}
			else
			{
				byte_stream_character -= 0xa0;

				safe_unicode_character = libuna_codepage_iso_8859_16_byte_stream_to_unicode_base_0xa0[ byte_stream_character ];
			}
			safe_byte_stream_index += 1;

			break;

		case LIBUNA_CODEPAGE_KOI8_R:
			result = libuna_codepage_koi8_r_copy_from_byte_stream(
			          &safe_unicode_character,
			          byte_stream,
			          byte_stream_size,
			          &safe_byte_stream_index,
			          error );
			break;

		case LIBUNA_CODEPAGE_KOI8_U:
			result = libuna_codepage_koi8_u_copy_from_byte_stream(
			          &safe_unicode_character,
			          byte_stream,
			          byte_stream_size,
			          &safe_byte_stream_index,
			          error );
			break;

		case LIBUNA_CODEPAGE_MAC_ARABIC:
			result = libuna_codepage_mac_arabic_copy_from_byte_stream(
			          &safe_unicode_character,
			          byte_stream,
			          byte_stream_size,
			          &safe_byte_stream_index,
			          error );
			break;

		case LIBUNA_CODEPAGE_MAC_CELTIC:
			result = libuna_codepage_mac_celtic_copy_from_byte_stream(
			          &safe_unicode_character,
			          byte_stream,
			          byte_stream_size,
			          &safe_byte_stream_index,
			          error );
			break;

		case LIBUNA_CODEPAGE_MAC_CENTRALEURROMAN:
			result = libuna_codepage_mac_centraleurroman_copy_from_byte_stream(
			          &safe_unicode_character,
			          byte_stream,
			          byte_stream_size,
			          &safe_byte_stream_index,
			          error );
			break;

		case LIBUNA_CODEPAGE_MAC_CROATIAN:
			result = libuna_codepage_mac_croatian_copy_from_byte_stream(
			          &safe_unicode_character,
			          byte_stream,
			          byte_stream_size,
			          &safe_byte_stream_index,
			          error );
			break;

		case LIBUNA_CODEPAGE_MAC_CYRILLIC:
			result = libuna_codepage_mac_cyrillic_copy_from_byte_stream(
			          &safe_unicode_character,
			          byte_stream,
			          byte_stream_size,
			          &safe_byte_stream_index,
			          error );
			break;

		case LIBUNA_CODEPAGE_MAC_DINGBATS:
			result = libuna_codepage_mac_dingbats_copy_from_byte_stream(
			          &safe_unicode_character,
			          byte_stream,
			          byte_stream_size,
			          &safe_byte_stream_index,
			          error );
			break;

		case LIBUNA_CODEPAGE_MAC_FARSI:
			result = libuna_codepage_mac_farsi_copy_from_byte_stream(
			          &safe_unicode_character,
			          byte_stream,
			          byte_stream_size,
			          &safe_byte_stream_index,
			          error );
			break;

		case LIBUNA_CODEPAGE_MAC_GAELIC:
			result = libuna_codepage_mac_gaelic_copy_from_byte_stream(
			          &safe_unicode_character,
			          byte_stream,
			          byte_stream_size,
			          &safe_byte_stream_index,
			          error );
			break;

		case LIBUNA_CODEPAGE_MAC_GREEK:
			result = libuna_codepage_mac_greek_copy_from_byte_stream(
			          &safe_unicode_character,
			          byte_stream,
			          byte_stream_size,
			          &safe_byte_stream_index,
			          error );
			break;

		case LIBUNA_CODEPAGE_MAC_ICELANDIC:
			result = libuna_codepage_mac_icelandic_copy_from_byte_stream(
			          &safe_unicode_character,
			          byte_stream,
			          byte_stream_size,
			          &safe_byte_stream_index,
			          error );
			break;

		case LIBUNA_CODEPAGE_MAC_INUIT:
			result = libuna_codepage_mac_inuit_copy_from_byte_stream(
			          &safe_unicode_character,
			          byte_stream,
			          byte_stream_size,
			          &safe_byte_stream_index,
			          error );
			break;

		case LIBUNA_CODEPAGE_MAC_ROMAN:
			result = libuna_codepage_mac_roman_copy_from_byte_stream(
			          &safe_unicode_character,
			          byte_stream,
			          byte_stream_size,
			          &safe_byte_stream_index,
			          error );
			break;

		case LIBUNA_CODEPAGE_MAC_ROMANIAN:
			result = libuna_codepage_mac_romanian_copy_from_byte_stream(
			          &safe_unicode_character,
			          byte_stream,
			          byte_stream_size,
			          &safe_byte_stream_index,
			          error );
			break;

		case LIBUNA_CODEPAGE_MAC_RUSSIAN:
			result = libuna_codepage_mac_russian_copy_from_byte_stream(
			          &safe_unicode_character,
			          byte_stream,
			          byte_stream_size,
			          &safe_byte_stream_index,
			          error );
			break;

		case LIBUNA_CODEPAGE_MAC_SYMBOL:
			result = libuna_codepage_mac_symbol_copy_from_byte_stream(
			          &safe_unicode_character,
			          byte_stream,
			          byte_stream_size,
			          &safe_byte_stream_index,
			          error );
			break;

		case LIBUNA_CODEPAGE_MAC_THAI:
			result = libuna_codepage_mac_thai_copy_from_byte_stream(
			          &safe_unicode_character,
			          byte_stream,
			          byte_stream_size,
			          &safe_byte_stream_index,
			          error );
			break;

		case LIBUNA_CODEPAGE_MAC_TURKISH:
			result = libuna_codepage_mac_turkish_copy_from_byte_stream(
			          &safe_unicode_character,
			          byte_stream,
			          byte_stream_size,
			          &safe_byte_stream_index,
			          error );
			break;

		case LIBUNA_CODEPAGE_MAC_UKRAINIAN:
			result = libuna_codepage_mac_ukrainian_copy_from_byte_stream(
			          &safe_unicode_character,
			          byte_stream,
			          byte_stream_size,
			          &safe_byte_stream_index,
			          error );
			break;

		case LIBUNA_CODEPAGE_WINDOWS_874:
			result = libuna_codepage_windows_874_copy_from_byte_stream(
			          &safe_unicode_character,
			          byte_stream,
			          byte_stream_size,
			          &safe_byte_stream_index,
			          error );
			break;

		case LIBUNA_CODEPAGE_WINDOWS_932:
			result = libuna_codepage_windows_932_copy_from_byte_stream(
			          &safe_unicode_character,
			          byte_stream,
			          byte_stream_size,
			          &safe_byte_stream_index,
			          error );
			break;

		case LIBUNA_CODEPAGE_WINDOWS_936:
			result = libuna_codepage_windows_936_copy_from_byte_stream(
			          &safe_unicode_character,
			          byte_stream,
			          byte_stream_size,
			          &safe_byte_stream_index,
			          error );
			break;

		case LIBUNA_CODEPAGE_WINDOWS_949:
			result = libuna_codepage_windows_949_copy_from_byte_stream(
			          &safe_unicode_character,
			          byte_stream,
			          byte_stream_size,
			          &safe_byte_stream_index,
			          error );
			break;

		case LIBUNA_CODEPAGE_WINDOWS_950:
			result = libuna_codepage_windows_950_copy_from_byte_stream(
			          &safe_unicode_character,
			          byte_stream,
			          byte_stream_size,
			          &safe_byte_stream_index,
			          error );
			break;

		case LIBUNA_CODEPAGE_WINDOWS_1250:
			result = libuna_codepage_windows_1250_copy_from_byte_stream(
			          &safe_unicode_character,
			          byte_stream,
			          byte_stream_size,
			          &safe_byte_stream_index,
			          error );
			break;

		case LIBUNA_CODEPAGE_WINDOWS_1251:
			result = libuna_codepage_windows_1251_copy_from_byte_stream(
			          &safe_unicode_character,
			          byte_stream,
			          byte_stream_size,
			          &safe_byte_stream_index,
			          error );
			break;

		case LIBUNA_CODEPAGE_WINDOWS_1252:
			result = libuna_codepage_windows_1252_copy_from_byte_stream(
			          &safe_unicode_character,
			          byte_stream,
			          byte_stream_size,
			          &safe_byte_stream_index,
			          error );
			break;

		case LIBUNA_CODEPAGE_WINDOWS_1253:
			result = libuna_codepage_windows_1253_copy_from_byte_stream(
			          &safe_unicode_character,
			          byte_stream,
			          byte_stream_size,
			          &safe_byte_stream_index,
			          error );
			break;

		case LIBUNA_CODEPAGE_WINDOWS_1254:
			result = libuna_codepage_windows_1254_copy_from_byte_stream(
			          &safe_unicode_character,
			          byte_stream,
			          byte_stream_size,
			          &safe_byte_stream_index,
			          error );
			break;

		case LIBUNA_CODEPAGE_WINDOWS_1255:
			result = libuna_codepage_windows_1255_copy_from_byte_stream(
			          &safe_unicode_character,
			          byte_stream,
			          byte_stream_size,
			          &safe_byte_stream_index,
			          error );
			break;

		case LIBUNA_CODEPAGE_WINDOWS_1256:
			result = libuna_codepage_windows_1256_copy_from_byte_stream(
			          &safe_unicode_character,
			          byte_stream,
			          byte_stream_size,
			          &safe_byte_stream_index,
			          error );
			break;

		case LIBUNA_CODEPAGE_WINDOWS_1257:
			result = libuna_codepage_windows_1257_copy_from_byte_stream(
			          &safe_unicode_character,
			          byte_stream,
			          byte_stream_size,
			          &safe_byte_stream_index,
			          error );
			break;

		case LIBUNA_CODEPAGE_WINDOWS_1258:
			result = libuna_codepage_windows_1258_copy_from_byte_stream(
			          &safe_unicode_character,
			          byte_stream,
			          byte_stream_size,
			          &safe_byte_stream_index,
			          error );
			break;

		default:
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			"%s: unsupported codepage: %d.",
			 function,
			 codepage );

			return( -1 );
	}
	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy Unicode character from byte stream.",
		 function );

		return( -1 );
	}
	*unicode_character = safe_unicode_character;
	*byte_stream_index = safe_byte_stream_index;

	return( result );
}

/* Copies an Unicode character to a byte stream
 * Returns 1 if successful, 0 if the Unicode character is valid but not supported since it requires special handling or -1 on error
 */
int libuna_unicode_character_copy_to_byte_stream(
     libuna_unicode_character_t unicode_character,
     uint8_t *byte_stream,
     size_t byte_stream_size,
     size_t *byte_stream_index,
     int codepage,
     libcerror_error_t **error )
{
	static char *function         = "libuna_unicode_character_copy_to_byte_stream";
	size_t safe_byte_stream_index = 0;
	int result                    = 1;

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
	switch( codepage )
	{
		case LIBUNA_CODEPAGE_ASCII:
			if( unicode_character < 0x0080 )
			{
				byte_stream[ safe_byte_stream_index ] = (uint8_t) unicode_character;
			}
			else
			{
				byte_stream[ safe_byte_stream_index ] = 0x1a;
			}
			safe_byte_stream_index += 1;

			break;

		case LIBUNA_CODEPAGE_ISO_8859_1:
			if( unicode_character < 0x0100 )
			{
				byte_stream[ safe_byte_stream_index ] = (uint8_t) unicode_character;
			}
			else
			{
				byte_stream[ safe_byte_stream_index ] = 0x1a;
			}
			safe_byte_stream_index += 1;

			break;

		case LIBUNA_CODEPAGE_ISO_8859_2:
			if( unicode_character < 0x00a0 )
			{
				byte_stream[ safe_byte_stream_index ] = (uint8_t) unicode_character;
			}
			else if( ( unicode_character >= 0x00a0 )
			      && ( unicode_character < 0x0120 ) )
			{
				unicode_character -= 0x00a0;

				byte_stream[ safe_byte_stream_index ] = libuna_codepage_iso_8859_2_unicode_to_byte_stream_base_0x00a0[ unicode_character ];
			}
			else if( ( unicode_character >= 0x0138 )
			      && ( unicode_character < 0x0180 ) )
			{
				unicode_character -= 0x0138;

				byte_stream[ safe_byte_stream_index ] = libuna_codepage_iso_8859_2_unicode_to_byte_stream_base_0x0138[ unicode_character ];
			}
			else if( ( unicode_character >= 0x02d8 )
			      && ( unicode_character < 0x02e0 ) )
			{
				unicode_character -= 0x02d8;

				byte_stream[ safe_byte_stream_index ] = libuna_codepage_iso_8859_2_unicode_to_byte_stream_base_0x02d8[ unicode_character ];
			}
			else if( unicode_character == 0x02c7 )
			{
				byte_stream[ safe_byte_stream_index ] = 0xb7;
			}
			else
			{
				byte_stream[ safe_byte_stream_index ] = 0x1a;
			}
			safe_byte_stream_index += 1;

			break;

		case LIBUNA_CODEPAGE_ISO_8859_3:
			if( unicode_character < 0x00a0 )
			{
				byte_stream[ safe_byte_stream_index ] = (uint8_t) unicode_character;
			}
			else if( ( unicode_character >= 0x00a0 )
			      && ( unicode_character < 0x0100 ) )
			{
				unicode_character -= 0x00a0;

				byte_stream[ safe_byte_stream_index ] = libuna_codepage_iso_8859_3_unicode_to_byte_stream_base_0x00a0[ unicode_character ];
			}
			else if( ( unicode_character >= 0x0108 )
			      && ( unicode_character < 0x0110 ) )
			{
				unicode_character -= 0x0108;

				byte_stream[ safe_byte_stream_index ] = libuna_codepage_iso_8859_3_unicode_to_byte_stream_base_0x0108[ unicode_character ];
			}
			else if( ( unicode_character >= 0x0118 )
			      && ( unicode_character < 0x0128 ) )
			{
				unicode_character -= 0x0118;

				byte_stream[ safe_byte_stream_index ] = libuna_codepage_iso_8859_3_unicode_to_byte_stream_base_0x0118[ unicode_character ];
			}
			else if( ( unicode_character >= 0x0130 )
			      && ( unicode_character < 0x0138 ) )
			{
				unicode_character -= 0x0130;

				byte_stream[ safe_byte_stream_index ] = libuna_codepage_iso_8859_3_unicode_to_byte_stream_base_0x0130[ unicode_character ];
			}
			else if( ( unicode_character >= 0x0158 )
			      && ( unicode_character < 0x0160 ) )
			{
				unicode_character -= 0x0158;

				byte_stream[ safe_byte_stream_index ] = libuna_codepage_iso_8859_3_unicode_to_byte_stream_base_0x0158[ unicode_character ];
			}
			else switch( unicode_character )
			{
				case 0x016c:
					byte_stream[ safe_byte_stream_index ] = 0xdd;
					break;

				case 0x016d:
					byte_stream[ safe_byte_stream_index ] = 0xfd;
					break;

				case 0x017b:
					byte_stream[ safe_byte_stream_index ] = 0xaf;
					break;

				case 0x017c:
					byte_stream[ safe_byte_stream_index ] = 0xbf;
					break;

				case 0x02d8:
					byte_stream[ safe_byte_stream_index ] = 0xa2;
					break;

				case 0x02d9:
					byte_stream[ safe_byte_stream_index ] = 0xff;
					break;

				default:
					byte_stream[ safe_byte_stream_index ] = 0x1a;
					break;
			}
			safe_byte_stream_index += 1;

			break;

		case LIBUNA_CODEPAGE_ISO_8859_4:
			if( unicode_character < 0x00a0 )
			{
				byte_stream[ safe_byte_stream_index ] = (uint8_t) unicode_character;
			}
			else if( ( unicode_character >= 0x00a0 )
			      && ( unicode_character < 0x0158 ) )
			{
				unicode_character -= 0x00a0;

				byte_stream[ safe_byte_stream_index ] = libuna_codepage_iso_8859_4_unicode_to_byte_stream_base_0x00a0[ unicode_character ];
			}
			else if( ( unicode_character >= 0x0160 )
			      && ( unicode_character < 0x0180 ) )
			{
				unicode_character -= 0x0160;

				byte_stream[ safe_byte_stream_index ] = libuna_codepage_iso_8859_4_unicode_to_byte_stream_base_0x0160[ unicode_character ];
			}
			else switch( unicode_character )
			{
				case 0x02c7:
					byte_stream[ safe_byte_stream_index ] = 0xb7;
					break;

				case 0x02d9:
					byte_stream[ safe_byte_stream_index ] = 0xff;
					break;

				case 0x02db:
					byte_stream[ safe_byte_stream_index ] = 0xb2;
					break;

				default:
					byte_stream[ safe_byte_stream_index ] = 0x1a;
					break;
			}
			safe_byte_stream_index += 1;

			break;

		case LIBUNA_CODEPAGE_ISO_8859_5:
			if( unicode_character < 0x00a1 )
			{
				byte_stream[ safe_byte_stream_index ] = (uint8_t) unicode_character;
			}
			else if( ( unicode_character >= 0x0400 )
			      && ( unicode_character < 0x0460 ) )
			{
				unicode_character -= 0x0400;

				byte_stream[ safe_byte_stream_index ] = libuna_codepage_iso_8859_5_unicode_to_byte_stream_base_0x0400[ unicode_character ];
			}
			else switch( unicode_character )
			{
				case 0x00a7:
					byte_stream[ safe_byte_stream_index ] = 0xfd;
					break;

				case 0x00ad:
					byte_stream[ safe_byte_stream_index ] = 0xad;
					break;

				case 0x2116:
					byte_stream[ safe_byte_stream_index ] = 0xf0;
					break;

				default:
					byte_stream[ safe_byte_stream_index ] = 0x1a;
					break;
			}
			safe_byte_stream_index += 1;

			break;

		case LIBUNA_CODEPAGE_ISO_8859_6:
			if( unicode_character < 0x00a1 )
			{
				byte_stream[ safe_byte_stream_index ] = (uint8_t) unicode_character;
			}
			else if( ( unicode_character >= 0x0618 )
			      && ( unicode_character < 0x658 ) )
			{
				unicode_character -= 0x0618;

				byte_stream[ safe_byte_stream_index ] = libuna_codepage_iso_8859_6_unicode_to_byte_stream_base_0x0618[ unicode_character ];
			}
			else switch( unicode_character )
			{
				case 0x00a4:
					byte_stream[ safe_byte_stream_index ] = 0xa4;
					break;

				case 0x00ad:
					byte_stream[ safe_byte_stream_index ] = 0xad;
					break;

				case 0x060c:
					byte_stream[ safe_byte_stream_index ] = 0xac;
					break;

				default:
					byte_stream[ safe_byte_stream_index ] = 0x1a;
					break;
			}
			safe_byte_stream_index += 1;

			break;

		case LIBUNA_CODEPAGE_ISO_8859_7:
			if( unicode_character < 0x00a0 )
			{
				byte_stream[ safe_byte_stream_index ] = (uint8_t) unicode_character;
			}
			else if( ( unicode_character >= 0x00a0 )
			      && ( unicode_character < 0x00b8 ) )
			{
				unicode_character -= 0x00a0;

				byte_stream[ safe_byte_stream_index ] = libuna_codepage_iso_8859_7_unicode_to_byte_stream_base_0x00a0[ unicode_character ];
			}
			else if( ( unicode_character >= 0x0380 )
			      && ( unicode_character < 0x03d0 ) )
			{
				unicode_character -= 0x0380;

				byte_stream[ safe_byte_stream_index ] = libuna_codepage_iso_8859_7_unicode_to_byte_stream_base_0x0380[ unicode_character ];
			}
			else switch( unicode_character )
			{
				case 0x00bb:
					byte_stream[ safe_byte_stream_index ] = 0xbb;
					break;

				case 0x00bd:
					byte_stream[ safe_byte_stream_index ] = 0xbd;
					break;

				case 0x037a:
					byte_stream[ safe_byte_stream_index ] = 0xaa;
					break;

				case 0x2015:
					byte_stream[ safe_byte_stream_index ] = 0xaf;
					break;

				case 0x2018:
					byte_stream[ safe_byte_stream_index ] = 0xa1;
					break;

				case 0x2019:
					byte_stream[ safe_byte_stream_index ] = 0xa2;
					break;

				case 0x20ac:
					byte_stream[ safe_byte_stream_index ] = 0xa4;
					break;

				case 0x20af:
					byte_stream[ safe_byte_stream_index ] = 0xa5;
					break;

				default:
					byte_stream[ safe_byte_stream_index ] = 0x1a;
					break;
			}
			safe_byte_stream_index += 1;

			break;

		case LIBUNA_CODEPAGE_ISO_8859_8:
			if( unicode_character < 0x00a0 )
			{
				byte_stream[ safe_byte_stream_index ] = (uint8_t) unicode_character;
			}
			else if( ( unicode_character >= 0x00a0 )
			      && ( unicode_character < 0x00c0 ) )
			{
				unicode_character -= 0x00a0;

				byte_stream[ safe_byte_stream_index ] = libuna_codepage_iso_8859_8_unicode_to_byte_stream_base_0x00a0[ unicode_character ];
			}
			else if( ( unicode_character >= 0x05d0 )
			      && ( unicode_character < 0x05f0 ) )
			{
				unicode_character -= 0x05d0;

				byte_stream[ safe_byte_stream_index ] = libuna_codepage_iso_8859_8_unicode_to_byte_stream_base_0x05d0[ unicode_character ];
			}
			else switch( unicode_character )
			{
				case 0x00d7:
					byte_stream[ safe_byte_stream_index ] = 0xaa;
					break;

				case 0x00f7:
					byte_stream[ safe_byte_stream_index ] = 0xba;
					break;

				case 0x200e:
					byte_stream[ safe_byte_stream_index ] = 0xfd;
					break;

				case 0x200f:
					byte_stream[ safe_byte_stream_index ] = 0xfe;
					break;

				case 0x2017:
					byte_stream[ safe_byte_stream_index ] = 0xdf;
					break;

				default:
					byte_stream[ safe_byte_stream_index ] = 0x1a;
					break;
			}
			safe_byte_stream_index += 1;

			break;

		case LIBUNA_CODEPAGE_ISO_8859_9:
			if( unicode_character < 0x00d0 )
			{
				byte_stream[ safe_byte_stream_index ] = (uint8_t) unicode_character;
			}
			else if( ( unicode_character >= 0x00d0 )
			      && ( unicode_character < 0x0100 ) )
			{
				unicode_character -= 0x00d0;

				byte_stream[ safe_byte_stream_index ] = libuna_codepage_iso_8859_9_unicode_to_byte_stream_base_0x00d0[ unicode_character ];
			}
			else switch( unicode_character )
			{
				case 0x011e:
					byte_stream[ safe_byte_stream_index ] = 0xd0;
					break;

				case 0x011f:
					byte_stream[ safe_byte_stream_index ] = 0xf0;
					break;

				case 0x0130:
					byte_stream[ safe_byte_stream_index ] = 0xdd;
					break;

				case 0x0131:
					byte_stream[ safe_byte_stream_index ] = 0xfd;
					break;

				case 0x015e:
					byte_stream[ safe_byte_stream_index ] = 0xde;
					break;

				case 0x015f:
					byte_stream[ safe_byte_stream_index ] = 0xfe;
					break;

				default:
					byte_stream[ safe_byte_stream_index ] = 0x1a;
					break;
			}
			safe_byte_stream_index += 1;

			break;

		case LIBUNA_CODEPAGE_ISO_8859_10:
			if( unicode_character < 0x00a1 )
			{
				byte_stream[ safe_byte_stream_index ] = (uint8_t) unicode_character;
			}
			else if( ( unicode_character >= 0x00c0 )
			      && ( unicode_character < 0x0150 ) )
			{
				unicode_character -= 0x00c0;

				byte_stream[ safe_byte_stream_index ] = libuna_codepage_iso_8859_10_unicode_to_byte_stream_base_0x00c0[ unicode_character ];
			}
			else if( ( unicode_character >= 0x0160 )
			      && ( unicode_character < 0x0170 ) )
			{
				unicode_character -= 0x0160;

				byte_stream[ safe_byte_stream_index ] = libuna_codepage_iso_8859_10_unicode_to_byte_stream_base_0x0160[ unicode_character ];
			}
			else switch( unicode_character )
			{
				case 0x00a7:
					byte_stream[ safe_byte_stream_index ] = 0xa7;
					break;

				case 0x00ad:
					byte_stream[ safe_byte_stream_index ] = 0xad;
					break;

				case 0x00b0:
					byte_stream[ safe_byte_stream_index ] = 0xb0;
					break;

				case 0x00b7:
					byte_stream[ safe_byte_stream_index ] = 0xb7;
					break;

				case 0x0172:
					byte_stream[ safe_byte_stream_index ] = 0xd9;
					break;

				case 0x0173:
					byte_stream[ safe_byte_stream_index ] = 0xf9;
					break;

				case 0x017d:
					byte_stream[ safe_byte_stream_index ] = 0xac;
					break;

				case 0x017e:
					byte_stream[ safe_byte_stream_index ] = 0xbc;
					break;

				case 0x2015:
					byte_stream[ safe_byte_stream_index ] = 0xbd;
					break;

				default:
					byte_stream[ safe_byte_stream_index ] = 0x1a;
					break;
			}
			safe_byte_stream_index += 1;

			break;

		case LIBUNA_CODEPAGE_ISO_8859_11:
			if( unicode_character < 0x00a1 )
			{
				byte_stream[ safe_byte_stream_index ] = (uint8_t) unicode_character;
			}
			else if( ( unicode_character >= 0x0e01 )
			      && ( unicode_character < 0x0e3b ) )
			{
				byte_stream[ safe_byte_stream_index ] = (uint8_t) ( unicode_character - 0x0d60 );
			}
			else if( ( unicode_character >= 0x0e3f )
			      && ( unicode_character < 0x0e5c ) )
			{
				byte_stream[ safe_byte_stream_index ] = (uint8_t) ( unicode_character - 0x0d60 );
			}
			else
			{
				byte_stream[ safe_byte_stream_index ] = 0x1a;
			}
			safe_byte_stream_index += 1;

			break;

		case LIBUNA_CODEPAGE_ISO_8859_13:
			if( unicode_character < 0x00a0 )
			{
				byte_stream[ safe_byte_stream_index ] = (uint8_t) unicode_character;
			}
			else if( ( unicode_character >= 0x00a0 )
			      && ( unicode_character < 0x0180 ) )
			{
				unicode_character -= 0x00a0;

				byte_stream[ safe_byte_stream_index ] = libuna_codepage_iso_8859_13_unicode_to_byte_stream_base_0x00a0[ unicode_character ];
			}
			else if( ( unicode_character >= 0x2018 )
			       && ( unicode_character < 0x2020 ) )
			{
				unicode_character -= 0x2018;

				byte_stream[ safe_byte_stream_index ] = libuna_codepage_iso_8859_13_unicode_to_byte_stream_base_0x2018[ unicode_character ];
			}
			else
			{
				byte_stream[ safe_byte_stream_index ] = 0x1a;
			}
			safe_byte_stream_index += 1;

			break;

		case LIBUNA_CODEPAGE_ISO_8859_14:
			if( unicode_character < 0x00a1 )
			{
				byte_stream[ safe_byte_stream_index ] = (uint8_t) unicode_character;
			}
			else if( ( unicode_character >= 0x00c0 )
			      && ( unicode_character < 0x0100 ) )
			{
				unicode_character -= 0x00c0;

				byte_stream[ safe_byte_stream_index ] = libuna_codepage_iso_8859_14_unicode_to_byte_stream_base_0x00c0[ unicode_character ];
			}
			else if( ( unicode_character >= 0x0170 )
			      && ( unicode_character < 0x0178 ) )
			{
				unicode_character -= 0x0170;

				byte_stream[ safe_byte_stream_index ] = libuna_codepage_iso_8859_14_unicode_to_byte_stream_base_0x0170[ unicode_character ];
			}
			else if( ( unicode_character >= 0x1e80 )
			      && ( unicode_character < 0x1e88 ) )
			{
				unicode_character -= 0x1e80;

				byte_stream[ safe_byte_stream_index ] = libuna_codepage_iso_8859_14_unicode_to_byte_stream_base_0x1e80[ unicode_character ];
			}
			else switch( unicode_character )
			{
				case 0x00a3:
					byte_stream[ safe_byte_stream_index ] = 0xa3;
					break;

				case 0x00a7:
					byte_stream[ safe_byte_stream_index ] = 0xa7;
					break;

				case 0x00a9:
					byte_stream[ safe_byte_stream_index ] = 0xa9;
					break;

				case 0x00ad:
					byte_stream[ safe_byte_stream_index ] = 0xad;
					break;

				case 0x00ae:
					byte_stream[ safe_byte_stream_index ] = 0xae;
					break;

				case 0x00b6:
					byte_stream[ safe_byte_stream_index ] = 0xb6;
					break;

				case 0x010a:
					byte_stream[ safe_byte_stream_index ] = 0xa4;
					break;

				case 0x010b:
					byte_stream[ safe_byte_stream_index ] = 0xa5;
					break;

				case 0x0120:
					byte_stream[ safe_byte_stream_index ] = 0xb2;
					break;

				case 0x0121:
					byte_stream[ safe_byte_stream_index ] = 0xb3;
					break;

				case 0x0178:
					byte_stream[ safe_byte_stream_index ] = 0xaf;
					break;

				case 0x1e02:
					byte_stream[ safe_byte_stream_index ] = 0xa1;
					break;

				case 0x1e03:
					byte_stream[ safe_byte_stream_index ] = 0xa2;
					break;

				case 0x1e0a:
					byte_stream[ safe_byte_stream_index ] = 0xa6;
					break;

				case 0x1e0b:
					byte_stream[ safe_byte_stream_index ] = 0xab;
					break;

				case 0x1e1e:
					byte_stream[ safe_byte_stream_index ] = 0xb0;
					break;

				case 0x1e1f:
					byte_stream[ safe_byte_stream_index ] = 0xb1;
					break;

				case 0x1e40:
					byte_stream[ safe_byte_stream_index ] = 0xb4;
					break;

				case 0x1e41:
					byte_stream[ safe_byte_stream_index ] = 0xb5;
					break;

				case 0x1e56:
					byte_stream[ safe_byte_stream_index ] = 0xb7;
					break;

				case 0x1e57:
					byte_stream[ safe_byte_stream_index ] = 0xb9;
					break;

				case 0x1e60:
					byte_stream[ safe_byte_stream_index ] = 0xbb;
					break;

				case 0x1e61:
					byte_stream[ safe_byte_stream_index ] = 0xbf;
					break;

				case 0x1e6a:
					byte_stream[ safe_byte_stream_index ] = 0xd7;
					break;

				case 0x1e6b:
					byte_stream[ safe_byte_stream_index ] = 0xf7;
					break;

				case 0x1ef2:
					byte_stream[ safe_byte_stream_index ] = 0xac;
					break;

				case 0x1ef3:
					byte_stream[ safe_byte_stream_index ] = 0xbc;
					break;

				default:
					byte_stream[ safe_byte_stream_index ] = 0x1a;
					break;
			}
			safe_byte_stream_index += 1;

			break;

		case LIBUNA_CODEPAGE_ISO_8859_15:
			if( unicode_character < 0x00a0 )
			{
				byte_stream[ safe_byte_stream_index ] = (uint8_t) unicode_character;
			}
			else if( ( unicode_character >= 0x00a0 )
			      && ( unicode_character < 0x00c0 ) )
			{
				unicode_character -= 0x00a0;

				byte_stream[ safe_byte_stream_index ] = libuna_codepage_iso_8859_15_unicode_to_byte_stream_base_0x00a0[ unicode_character ];
			}
			else if( unicode_character < 0x0100 )
			{
				byte_stream[ safe_byte_stream_index ] = (uint8_t) unicode_character;
			}
			else switch( unicode_character )
			{
				case 0x0152:
					byte_stream[ safe_byte_stream_index ] = 0xbc;
					break;

				case 0x0153:
					byte_stream[ safe_byte_stream_index ] = 0xbd;
					break;

				case 0x0160:
					byte_stream[ safe_byte_stream_index ] = 0xa6;
					break;

				case 0x0161:
					byte_stream[ safe_byte_stream_index ] = 0xa8;
					break;

				case 0x0178:
					byte_stream[ safe_byte_stream_index ] = 0xbe;
					break;

				case 0x017d:
					byte_stream[ safe_byte_stream_index ] = 0xb4;
					break;

				case 0x017e:
					byte_stream[ safe_byte_stream_index ] = 0xb8;
					break;

				case 0x20ac:
					byte_stream[ safe_byte_stream_index ] = 0xa4;
					break;

				default:
					byte_stream[ safe_byte_stream_index ] = 0x1a;
					break;
			}
			safe_byte_stream_index += 1;

			break;

		case LIBUNA_CODEPAGE_ISO_8859_16:
			if( unicode_character < 0x00a1 )
			{
				byte_stream[ safe_byte_stream_index ] = (uint8_t) unicode_character;
			}
			else if( ( unicode_character >= 0x00a8 )
			      && ( unicode_character < 0x0108 ) )
			{
				unicode_character -= 0x00a8;

				byte_stream[ safe_byte_stream_index ] = libuna_codepage_iso_8859_16_unicode_to_byte_stream_base_0x00a8[ unicode_character ];
			}
			else if( ( unicode_character >= 0x0140 )
			      && ( unicode_character < 0x0148 ) )
			{
				unicode_character -= 0x0140;

				byte_stream[ safe_byte_stream_index ] = libuna_codepage_iso_8859_16_unicode_to_byte_stream_base_0x0140[ unicode_character ];
			}
			else if( ( unicode_character >= 0x0150 )
			      && ( unicode_character < 0x0158 ) )
			{
				unicode_character -= 0x0150;

				byte_stream[ safe_byte_stream_index ] = libuna_codepage_iso_8859_16_unicode_to_byte_stream_base_0x0150[ unicode_character ];
			}
			else if( ( unicode_character >= 0x0178 )
			      && ( unicode_character < 0x0180 ) )
			{
				unicode_character -= 0x0178;

				byte_stream[ safe_byte_stream_index ] = libuna_codepage_iso_8859_16_unicode_to_byte_stream_base_0x0178[ unicode_character ];
			}
			else if( ( unicode_character >= 0x0218 )
			      && ( unicode_character < 0x0220 ) )
			{
				unicode_character -= 0x0218;

				byte_stream[ safe_byte_stream_index ] = libuna_codepage_iso_8859_16_unicode_to_byte_stream_base_0x0218[ unicode_character ];
			}
			else switch( unicode_character )
			{
				case 0x00a7:
					byte_stream[ safe_byte_stream_index ] = 0xa7;
					break;

				case 0x010c:
					byte_stream[ safe_byte_stream_index ] = 0xb2;
					break;

				case 0x010d:
					byte_stream[ safe_byte_stream_index ] = 0xb9;
					break;

				case 0x0110:
					byte_stream[ safe_byte_stream_index ] = 0xd0;
					break;

				case 0x0111:
					byte_stream[ safe_byte_stream_index ] = 0xf0;
					break;

				case 0x0118:
					byte_stream[ safe_byte_stream_index ] = 0xdd;
					break;

				case 0x0119:
					byte_stream[ safe_byte_stream_index ] = 0xfd;
					break;

				case 0x015a:
					byte_stream[ safe_byte_stream_index ] = 0xd7;
					break;

				case 0x015b:
					byte_stream[ safe_byte_stream_index ] = 0xf7;
					break;

				case 0x0160:
					byte_stream[ safe_byte_stream_index ] = 0xa6;
					break;

				case 0x0161:
					byte_stream[ safe_byte_stream_index ] = 0xa8;
					break;

				case 0x0170:
					byte_stream[ safe_byte_stream_index ] = 0xd8;
					break;

				case 0x0171:
					byte_stream[ safe_byte_stream_index ] = 0xf8;
					break;

				case 0x201d:
					byte_stream[ safe_byte_stream_index ] = 0xb5;
					break;

				case 0x201e:
					byte_stream[ safe_byte_stream_index ] = 0xa5;
					break;

				case 0x20ac:
					byte_stream[ safe_byte_stream_index ] = 0xa4;
					break;

				default:
					byte_stream[ safe_byte_stream_index ] = 0x1a;
					break;
			}
			safe_byte_stream_index += 1;

			break;

		case LIBUNA_CODEPAGE_KOI8_R:
			result = libuna_codepage_koi8_r_copy_to_byte_stream(
			          unicode_character,
			          byte_stream,
			          byte_stream_size,
			          &safe_byte_stream_index,
			          error );
			break;

		case LIBUNA_CODEPAGE_KOI8_U:
			result = libuna_codepage_koi8_u_copy_to_byte_stream(
			          unicode_character,
			          byte_stream,
			          byte_stream_size,
			          &safe_byte_stream_index,
			          error );
			break;

		case LIBUNA_CODEPAGE_MAC_ARABIC:
			result = libuna_codepage_mac_arabic_copy_to_byte_stream(
			          unicode_character,
			          byte_stream,
			          byte_stream_size,
			          &safe_byte_stream_index,
			          error );
			break;

		case LIBUNA_CODEPAGE_MAC_CELTIC:
			result = libuna_codepage_mac_celtic_copy_to_byte_stream(
			          unicode_character,
			          byte_stream,
			          byte_stream_size,
			          &safe_byte_stream_index,
			          error );
			break;

		case LIBUNA_CODEPAGE_MAC_CENTRALEURROMAN:
			result = libuna_codepage_mac_centraleurroman_copy_to_byte_stream(
			          unicode_character,
			          byte_stream,
			          byte_stream_size,
			          &safe_byte_stream_index,
			          error );
			break;

		case LIBUNA_CODEPAGE_MAC_CROATIAN:
			result = libuna_codepage_mac_croatian_copy_to_byte_stream(
			          unicode_character,
			          byte_stream,
			          byte_stream_size,
			          &safe_byte_stream_index,
			          error );
			break;

		case LIBUNA_CODEPAGE_MAC_CYRILLIC:
			result = libuna_codepage_mac_cyrillic_copy_to_byte_stream(
			          unicode_character,
			          byte_stream,
			          byte_stream_size,
			          &safe_byte_stream_index,
			          error );
			break;

		case LIBUNA_CODEPAGE_MAC_DINGBATS:
			result = libuna_codepage_mac_dingbats_copy_to_byte_stream(
			          unicode_character,
			          byte_stream,
			          byte_stream_size,
			          &safe_byte_stream_index,
			          error );
			break;

		case LIBUNA_CODEPAGE_MAC_FARSI:
			result = libuna_codepage_mac_farsi_copy_to_byte_stream(
			          unicode_character,
			          byte_stream,
			          byte_stream_size,
			          &safe_byte_stream_index,
			          error );
			break;

		case LIBUNA_CODEPAGE_MAC_GAELIC:
			result = libuna_codepage_mac_gaelic_copy_to_byte_stream(
			          unicode_character,
			          byte_stream,
			          byte_stream_size,
			          &safe_byte_stream_index,
			          error );
			break;

		case LIBUNA_CODEPAGE_MAC_GREEK:
			result = libuna_codepage_mac_greek_copy_to_byte_stream(
			          unicode_character,
			          byte_stream,
			          byte_stream_size,
			          &safe_byte_stream_index,
			          error );
			break;

		case LIBUNA_CODEPAGE_MAC_ICELANDIC:
			result = libuna_codepage_mac_icelandic_copy_to_byte_stream(
			          unicode_character,
			          byte_stream,
			          byte_stream_size,
			          &safe_byte_stream_index,
			          error );
			break;

		case LIBUNA_CODEPAGE_MAC_INUIT:
			result = libuna_codepage_mac_inuit_copy_to_byte_stream(
			          unicode_character,
			          byte_stream,
			          byte_stream_size,
			          &safe_byte_stream_index,
			          error );
			break;

		case LIBUNA_CODEPAGE_MAC_ROMAN:
			result = libuna_codepage_mac_roman_copy_to_byte_stream(
			          unicode_character,
			          byte_stream,
			          byte_stream_size,
			          &safe_byte_stream_index,
			          error );
			break;

		case LIBUNA_CODEPAGE_MAC_ROMANIAN:
			result = libuna_codepage_mac_romanian_copy_to_byte_stream(
			          unicode_character,
			          byte_stream,
			          byte_stream_size,
			          &safe_byte_stream_index,
			          error );
			break;

		case LIBUNA_CODEPAGE_MAC_RUSSIAN:
			result = libuna_codepage_mac_russian_copy_to_byte_stream(
			          unicode_character,
			          byte_stream,
			          byte_stream_size,
			          &safe_byte_stream_index,
			          error );
			break;

		case LIBUNA_CODEPAGE_MAC_SYMBOL:
			result = libuna_codepage_mac_symbol_copy_to_byte_stream(
			          unicode_character,
			          byte_stream,
			          byte_stream_size,
			          &safe_byte_stream_index,
			          error );
			break;

		case LIBUNA_CODEPAGE_MAC_THAI:
			result = libuna_codepage_mac_thai_copy_to_byte_stream(
			          unicode_character,
			          byte_stream,
			          byte_stream_size,
			          &safe_byte_stream_index,
			          error );
			break;

		case LIBUNA_CODEPAGE_MAC_TURKISH:
			result = libuna_codepage_mac_turkish_copy_to_byte_stream(
			          unicode_character,
			          byte_stream,
			          byte_stream_size,
			          &safe_byte_stream_index,
			          error );
			break;

		case LIBUNA_CODEPAGE_MAC_UKRAINIAN:
			result = libuna_codepage_mac_ukrainian_copy_to_byte_stream(
			          unicode_character,
			          byte_stream,
			          byte_stream_size,
			          &safe_byte_stream_index,
			          error );
			break;

		case LIBUNA_CODEPAGE_WINDOWS_874:
			result = libuna_codepage_windows_874_copy_to_byte_stream(
			          unicode_character,
			          byte_stream,
			          byte_stream_size,
			          &safe_byte_stream_index,
			          error );
			break;

		case LIBUNA_CODEPAGE_WINDOWS_932:
			result = libuna_codepage_windows_932_copy_to_byte_stream(
			          unicode_character,
			          byte_stream,
			          byte_stream_size,
			          &safe_byte_stream_index,
			          error );
			break;

		case LIBUNA_CODEPAGE_WINDOWS_936:
			result = libuna_codepage_windows_936_copy_to_byte_stream(
			          unicode_character,
			          byte_stream,
			          byte_stream_size,
			          &safe_byte_stream_index,
			          error );
			break;

		case LIBUNA_CODEPAGE_WINDOWS_949:
			result = libuna_codepage_windows_949_copy_to_byte_stream(
			          unicode_character,
			          byte_stream,
			          byte_stream_size,
			          &safe_byte_stream_index,
			          error );
			break;

		case LIBUNA_CODEPAGE_WINDOWS_950:
			result = libuna_codepage_windows_950_copy_to_byte_stream(
			          unicode_character,
			          byte_stream,
			          byte_stream_size,
			          &safe_byte_stream_index,
			          error );
			break;

		case LIBUNA_CODEPAGE_WINDOWS_1250:
			result = libuna_codepage_windows_1250_copy_to_byte_stream(
			          unicode_character,
			          byte_stream,
			          byte_stream_size,
			          &safe_byte_stream_index,
			          error );
			break;

		case LIBUNA_CODEPAGE_WINDOWS_1251:
			result = libuna_codepage_windows_1251_copy_to_byte_stream(
			          unicode_character,
			          byte_stream,
			          byte_stream_size,
			          &safe_byte_stream_index,
			          error );
			break;

		case LIBUNA_CODEPAGE_WINDOWS_1252:
			result = libuna_codepage_windows_1252_copy_to_byte_stream(
			          unicode_character,
			          byte_stream,
			          byte_stream_size,
			          &safe_byte_stream_index,
			          error );
			break;

		case LIBUNA_CODEPAGE_WINDOWS_1253:
			result = libuna_codepage_windows_1253_copy_to_byte_stream(
			          unicode_character,
			          byte_stream,
			          byte_stream_size,
			          &safe_byte_stream_index,
			          error );
			break;

		case LIBUNA_CODEPAGE_WINDOWS_1254:
			result = libuna_codepage_windows_1254_copy_to_byte_stream(
			          unicode_character,
			          byte_stream,
			          byte_stream_size,
			          &safe_byte_stream_index,
			          error );
			break;

		case LIBUNA_CODEPAGE_WINDOWS_1255:
			result = libuna_codepage_windows_1255_copy_to_byte_stream(
			          unicode_character,
			          byte_stream,
			          byte_stream_size,
			          &safe_byte_stream_index,
			          error );
			break;

		case LIBUNA_CODEPAGE_WINDOWS_1256:
			result = libuna_codepage_windows_1256_copy_to_byte_stream(
			          unicode_character,
			          byte_stream,
			          byte_stream_size,
			          &safe_byte_stream_index,
			          error );
			break;

		case LIBUNA_CODEPAGE_WINDOWS_1257:
			result = libuna_codepage_windows_1257_copy_to_byte_stream(
			          unicode_character,
			          byte_stream,
			          byte_stream_size,
			          &safe_byte_stream_index,
			          error );
			break;

		case LIBUNA_CODEPAGE_WINDOWS_1258:
			result = libuna_codepage_windows_1258_copy_to_byte_stream(
			          unicode_character,
			          byte_stream,
			          byte_stream_size,
			          &safe_byte_stream_index,
			          error );
			break;

		default:
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			"%s: unsupported codepage: %d.",
			 function,
		         codepage );

			return( -1 );
	}
	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy Unicode character to byte stream.",
		 function );

		return( -1 );
	}
	*byte_stream_index = safe_byte_stream_index;

	return( result );
}

/* Determines the size of an UCS-2 character from an Unicode character
 * Adds the size to the UCS-2 character size value
 * Returns 1 if successful or -1 on error
 */
int libuna_unicode_character_size_to_ucs2(
     libuna_unicode_character_t unicode_character,
     size_t *ucs2_character_size,
     libcerror_error_t **error )
{
	static char *function = "libuna_unicode_character_size_to_ucs2";

	if( ucs2_character_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UCS-2 character size.",
		 function );

		return( -1 );
	}
	/* Determine if the Unicode character is valid
	 * UCS-2 with surrogate pairs supports upto 0x10ffff characters
	 */
	if( unicode_character > LIBUNA_UNICODE_CHARACTER_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported Unicode character.",
		 function );

		return( -1 );
	}
	if( unicode_character > LIBUNA_UNICODE_BASIC_MULTILINGUAL_PLANE_MAX )
	{
		*ucs2_character_size += 2;
	}
	else
	{
		*ucs2_character_size += 1;
	}
	return( 1 );
}

/* Copies an Unicode character from an UCS-2 string
 * Returns 1 if successful or -1 on error
 */
int libuna_unicode_character_copy_from_ucs2(
     libuna_unicode_character_t *unicode_character,
     const libuna_utf16_character_t *ucs2_string,
     size_t ucs2_string_size,
     size_t *ucs2_string_index,
     libcerror_error_t **error )
{
	static char *function                             = "libuna_unicode_character_copy_from_ucs2";
	libuna_utf16_character_t ucs2_surrogate           = 0;
	libuna_unicode_character_t safe_unicode_character = 0;
	size_t safe_ucs2_string_index                     = 0;

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
	if( ucs2_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UCS-2 string.",
		 function );

		return( -1 );
	}
	if( ucs2_string_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid UCS-2 string size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( ucs2_string_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UCS-2 string index.",
		 function );

		return( -1 );
	}
	safe_ucs2_string_index = *ucs2_string_index;

	if( safe_ucs2_string_index >= ucs2_string_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: UCS-2 string too small.",
		 function );

		return( -1 );
	}
	safe_unicode_character  = ucs2_string[ safe_ucs2_string_index ];
	safe_ucs2_string_index += 1;

	/* Determine if the UCS-2 character is within the high surrogate range
	 */
	if( ( safe_unicode_character >= LIBUNA_UNICODE_SURROGATE_HIGH_RANGE_START )
	 && ( safe_unicode_character <= LIBUNA_UNICODE_SURROGATE_HIGH_RANGE_END ) )
	{
		if( safe_ucs2_string_index >= ucs2_string_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: missing surrogate UCS-2 character bytes.",
			 function );

			return( -1 );
		}
		ucs2_surrogate = ucs2_string[ safe_ucs2_string_index ];

		/* Determine if the UCS-2 character is within the low surrogate range
		 */
		if( ( ucs2_surrogate >= LIBUNA_UNICODE_SURROGATE_LOW_RANGE_START )
		 && ( ucs2_surrogate <= LIBUNA_UNICODE_SURROGATE_LOW_RANGE_END ) )
		{
			safe_unicode_character  -= LIBUNA_UNICODE_SURROGATE_HIGH_RANGE_START;
			safe_unicode_character <<= 10;
			safe_unicode_character  += ucs2_surrogate - LIBUNA_UNICODE_SURROGATE_LOW_RANGE_START;
			safe_unicode_character  += 0x010000;

			safe_ucs2_string_index += 1;
		}
	}
	/* Determine if the Unicode character is valid
	 * UCS-2 with surrogate pairs supports upto 0x10ffff characters
	 */
	if( safe_unicode_character > LIBUNA_UNICODE_CHARACTER_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported Unicode character.",
		 function );

		return( -1 );
	}
	*unicode_character = safe_unicode_character;
	*ucs2_string_index = safe_ucs2_string_index;

	return( 1 );
}

/* Copies an Unicode character into a UCS-2 string
 * Returns 1 if successful or -1 on error
 */
int libuna_unicode_character_copy_to_ucs2(
     libuna_unicode_character_t unicode_character,
     libuna_utf16_character_t *ucs2_string,
     size_t ucs2_string_size,
     size_t *ucs2_string_index,
     libcerror_error_t **error )
{
	static char *function         = "libuna_unicode_character_copy_to_ucs2";
	size_t safe_ucs2_string_index = 0;

	if( ucs2_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UCS-2 string.",
		 function );

		return( -1 );
	}
	if( ucs2_string_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid UCS-2 string size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( ucs2_string_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UCS-2 string index.",
		 function );

		return( -1 );
	}
	safe_ucs2_string_index = *ucs2_string_index;

	if( safe_ucs2_string_index >= ucs2_string_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: UCS-2 string too small.",
		 function );

		return( -1 );
	}
	/* Determine if the Unicode character is valid
	 */
	if( unicode_character > LIBUNA_UCS_CHARACTER_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported Unicode character.",
		 function );

		return( -1 );
	}
	if( unicode_character <= LIBUNA_UNICODE_BASIC_MULTILINGUAL_PLANE_MAX )
	{
		ucs2_string[ safe_ucs2_string_index++ ] = (libuna_utf16_character_t) unicode_character;
	}
	else
	{
		if( ( ucs2_string_size < 2 )
		 || ( safe_ucs2_string_index > ( ucs2_string_size - 2 ) ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
			 "%s: UCS-2 string too small.",
			 function );

			return( -1 );
		}
		unicode_character                      -= 0x010000;
		ucs2_string[ safe_ucs2_string_index++ ] = (libuna_utf16_character_t) ( ( unicode_character >> 10 ) + LIBUNA_UNICODE_SURROGATE_HIGH_RANGE_START );
		ucs2_string[ safe_ucs2_string_index++ ] = (libuna_utf16_character_t) ( ( unicode_character & 0x03ff ) + LIBUNA_UNICODE_SURROGATE_LOW_RANGE_START );
	}
	*ucs2_string_index = safe_ucs2_string_index;

	return( 1 );
}

/* Determines the size of an UCS-4 character from an Unicode character
 * Adds the size to the UCS-4 character size value
 * Returns 1 if successful or -1 on error
 */
int libuna_unicode_character_size_to_ucs4(
     libuna_unicode_character_t unicode_character,
     size_t *ucs4_character_size,
     libcerror_error_t **error )
{
	static char *function = "libuna_unicode_character_size_to_ucs4";

	LIBUNA_UNREFERENCED_PARAMETER( unicode_character )

	if( ucs4_character_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UCS-4 character size.",
		 function );

		return( -1 );
	}
	if( unicode_character > LIBUNA_UCS_CHARACTER_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported Unicode character.",
		 function );

		return( -1 );
	}
	*ucs4_character_size += 1;

	return( 1 );
}

/* Copies an Unicode character from an UCS-4 string
 * Returns 1 if successful or -1 on error
 */
int libuna_unicode_character_copy_from_ucs4(
     libuna_unicode_character_t *unicode_character,
     const libuna_utf32_character_t *ucs4_string,
     size_t ucs4_string_size,
     size_t *ucs4_string_index,
     libcerror_error_t **error )
{
	static char *function                             = "libuna_unicode_character_copy_from_ucs4";
	libuna_unicode_character_t safe_unicode_character = 0;
	size_t safe_ucs4_string_index                     = 0;

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
	if( ucs4_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UCS-4 string.",
		 function );

		return( -1 );
	}
	if( ucs4_string_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid UCS-4 string size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( ucs4_string_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UCS-4 string index.",
		 function );

		return( -1 );
	}
	safe_ucs4_string_index = *ucs4_string_index;

	if( safe_ucs4_string_index >= ucs4_string_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: UCS-4 string too small.",
		 function );

		return( -1 );
	}
	safe_unicode_character = ucs4_string[ safe_ucs4_string_index ];

	/* Determine if the Unicode character is valid
	 */
	if( safe_unicode_character > LIBUNA_UNICODE_CHARACTER_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported Unicode character.",
		 function );

		return( -1 );
	}
	*unicode_character = safe_unicode_character;
	*ucs4_string_index = safe_ucs4_string_index + 1;

	return( 1 );
}

/* Copies an Unicode character into a UCS-4 string
 * Returns 1 if successful or -1 on error
 */
int libuna_unicode_character_copy_to_ucs4(
     libuna_unicode_character_t unicode_character,
     libuna_utf32_character_t *ucs4_string,
     size_t ucs4_string_size,
     size_t *ucs4_string_index,
     libcerror_error_t **error )
{
	static char *function         = "libuna_unicode_character_copy_to_ucs4";
	size_t safe_ucs4_string_index = 0;

	if( ucs4_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UCS-4 string.",
		 function );

		return( -1 );
	}
	if( ucs4_string_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid UCS-4 string size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( ucs4_string_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UCS-4 string index.",
		 function );

		return( -1 );
	}
	safe_ucs4_string_index = *ucs4_string_index;

	if( safe_ucs4_string_index >= ucs4_string_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: UCS-4 string too small.",
		 function );

		return( -1 );
	}
	/* Determine if the Unicode character is valid
	 */
	if( unicode_character > LIBUNA_UCS_CHARACTER_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported Unicode character.",
		 function );

		return( -1 );
	}
	ucs4_string[ safe_ucs4_string_index ] = (libuna_utf32_character_t) unicode_character;

	*ucs4_string_index = safe_ucs4_string_index + 1;

	return( 1 );
}

/* Determines the size of an UTF-7 stream character from an Unicode character
 * Adds the size to the UTF-7 stream character size value
 * Returns 1 if successful or -1 on error
 */
int libuna_unicode_character_size_to_utf7_stream(
     libuna_unicode_character_t unicode_character,
     size_t *utf7_stream_character_size,
     uint32_t *utf7_stream_base64_data,
     libcerror_error_t **error )
{
	static char *function                    = "libuna_unicode_character_size_to_utf7_stream";
	libuna_utf16_character_t utf16_surrogate = 0;
	size_t safe_utf7_stream_character_size   = 0;
	uint32_t base64_triplet                  = 0;
	uint32_t safe_utf7_stream_base64_data    = 0;
	uint8_t base64_encode_character          = 0;
	uint8_t byte_bit_shift                   = 0;
	uint8_t current_byte                     = 0;
	uint8_t number_of_bytes                  = 0;

	if( utf7_stream_character_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-7 stream character size.",
		 function );

		return( -1 );
	}
	if( utf7_stream_base64_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-7 stream base64 data.",
		 function );

		return( -1 );
	}
	safe_utf7_stream_character_size = *utf7_stream_character_size;
	safe_utf7_stream_base64_data    = *utf7_stream_base64_data;

	/* Determine if the Unicode character is valid
	 */
	if( ( ( unicode_character >= LIBUNA_UNICODE_SURROGATE_HIGH_RANGE_START )
	  &&  ( unicode_character <= LIBUNA_UNICODE_SURROGATE_LOW_RANGE_END ) )
	 || ( unicode_character > LIBUNA_UNICODE_CHARACTER_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported Unicode character.",
		 function );

		return( -1 );
	}
	/* The + character must be escaped
	 */
	if( unicode_character == (libuna_unicode_character_t) '+' )
	{
	}
	/* Allow for the end of string character
	 */
	else if( unicode_character == 0 )
	{
	}
	else if( ( unicode_character >= 256 )
	      || ( libuna_unicode_character_utf7_valid_directly_encoded_character[ (uint8_t) unicode_character ] == 0 ) )
	{
		base64_encode_character = 1;
	}
	if( base64_encode_character == 0 )
	{
		if( ( safe_utf7_stream_base64_data & LIBUNA_UTF7_IS_BASE64_ENCODED ) != 0 )
		{
			safe_utf7_stream_base64_data = 0;
		}
		safe_utf7_stream_character_size += 1;

		/* The + character must be escaped
		 */
		if( unicode_character == (libuna_unicode_character_t) '+' )
		{
			safe_utf7_stream_character_size += 1;
		}
	}
	else
	{
		/* Escape the base64 encoded characters with a +
		 */
		if( ( safe_utf7_stream_base64_data & LIBUNA_UTF7_IS_BASE64_ENCODED ) == 0 )
		{
			safe_utf7_stream_character_size += 1;
		}
		/* Otherwise continue the previously base64 encoded characters
		 */
		else
		{
			base64_triplet  = safe_utf7_stream_base64_data & 0x00ffffff;
			number_of_bytes = ( safe_utf7_stream_base64_data >> 24 ) & 0x03;
			current_byte    = ( safe_utf7_stream_base64_data >> 28 ) & 0x03;

			if( number_of_bytes > 0 )
			{
				if( safe_utf7_stream_character_size < (size_t) ( number_of_bytes + 1 ) )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
					 "%s: invalid UTF-7 stream character size value out of bounds.",
					 function );

					return( -1 );
				}
				/* Correct the size for the last partial base64 stream
				 */
				safe_utf7_stream_character_size -= number_of_bytes + 1;
			}
			if( safe_utf7_stream_character_size < 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: invalid UTF-7 stream character size value out of bounds.",
				 function );

				return( -1 );
			}
			/* Correct the size for the base64 stream termination character
			 */
			safe_utf7_stream_character_size -= 1;
		}
		safe_utf7_stream_base64_data = LIBUNA_UTF7_IS_BASE64_ENCODED;

		if( unicode_character > LIBUNA_UNICODE_BASIC_MULTILINGUAL_PLANE_MAX )
		{
			unicode_character -= 0x010000;

			utf16_surrogate = (libuna_utf16_character_t) ( ( unicode_character >> 10 ) + LIBUNA_UNICODE_SURROGATE_HIGH_RANGE_START );

			byte_bit_shift   = 16 - ( current_byte * 8 );
			base64_triplet  += (uint32_t) ( ( utf16_surrogate >> 8 ) & 0xff ) << byte_bit_shift;
			current_byte    += 1;
			number_of_bytes += 1;

			if( number_of_bytes == 3 )
			{
				safe_utf7_stream_character_size += 4;
				number_of_bytes                  = 0;
				current_byte                     = 0;
				base64_triplet                   = 0;
			}
			byte_bit_shift   = 16 - ( current_byte * 8 );
			base64_triplet  += (uint32_t) ( utf16_surrogate & 0xff ) << byte_bit_shift;
			current_byte    += 1;
			number_of_bytes += 1;

			if( number_of_bytes == 3 )
			{
				safe_utf7_stream_character_size += 4;
				number_of_bytes                  = 0;
				current_byte                     = 0;
				base64_triplet                   = 0;
			}
			unicode_character = (libuna_utf16_character_t) ( ( unicode_character & 0x03ff ) + LIBUNA_UNICODE_SURROGATE_LOW_RANGE_START );
		}
		byte_bit_shift   = 16 - ( current_byte * 8 );
		base64_triplet  += (uint32_t) ( ( unicode_character >> 8 ) & 0xff ) << byte_bit_shift;
		current_byte    += 1;
		number_of_bytes += 1;

		if( number_of_bytes == 3 )
		{
			safe_utf7_stream_character_size += 4;
			number_of_bytes                  = 0;
			current_byte                     = 0;
			base64_triplet                   = 0;
		}
		byte_bit_shift   = 16 - ( current_byte * 8 );
		base64_triplet  += (uint32_t) ( unicode_character & 0xff ) << byte_bit_shift;
		current_byte    += 1;
		number_of_bytes += 1;

		if( number_of_bytes == 3 )
		{
			safe_utf7_stream_character_size += 4;
			number_of_bytes                  = 0;
			current_byte                     = 0;
			base64_triplet                   = 0;
		}
		/* Terminate the base64 encoded characters
		 */
		if( number_of_bytes > 0 )
		{
			safe_utf7_stream_character_size += number_of_bytes + 1;
		}
		safe_utf7_stream_character_size += 1;
	}
	if( ( safe_utf7_stream_base64_data & LIBUNA_UTF7_IS_BASE64_ENCODED ) != 0 )
	{
		safe_utf7_stream_base64_data  = LIBUNA_UTF7_IS_BASE64_ENCODED;
		safe_utf7_stream_base64_data |= (uint32_t) current_byte << 28;
		safe_utf7_stream_base64_data |= (uint32_t) number_of_bytes << 24;
		safe_utf7_stream_base64_data |= base64_triplet & 0x00ffffff;
	}
	*utf7_stream_character_size = safe_utf7_stream_character_size;
	*utf7_stream_base64_data    = safe_utf7_stream_base64_data;

	return( 1 );
}

/* Copies an Unicode character from an UTF-7 stream
 * The bits of the base64 data contain:
 *   0 - 23 the base64 triplet
 *  24 - 25 the number of bytes in the triplet
 *  26 - 27 unused
 *  28 - 29 the current byte
 *       30 unused
 *       31 flag to indicate the current UTF-7 characters are (modified) base64 encoded
 *
 * Returns 1 if successful or -1 on error
 */
int libuna_unicode_character_copy_from_utf7_stream(
     libuna_unicode_character_t *unicode_character,
     const uint8_t *utf7_stream,
     size_t utf7_stream_size,
     size_t *utf7_stream_index,
     uint32_t *utf7_stream_base64_data,
     libcerror_error_t **error )
{
	static char *function                             = "libuna_unicode_character_copy_from_utf7_stream";
	libuna_unicode_character_t safe_unicode_character = 0;
	libuna_utf16_character_t utf16_surrogate          = 0;
	size_t safe_utf7_stream_index                     = 0;
	uint32_t base64_triplet                           = 0;
	uint32_t safe_utf7_stream_base64_data             = 0;
	uint8_t byte_bit_shift                            = 0;
	uint8_t current_byte                              = 0;
	uint8_t number_of_bytes                           = 0;
	uint8_t padding_size                              = 0;
	uint8_t utf7_character_value                      = 0;

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
	if( utf7_stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-7 stream.",
		 function );

		return( -1 );
	}
	if( utf7_stream_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid UTF-7 stream size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( utf7_stream_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-7 stream index.",
		 function );

		return( -1 );
	}
	if( utf7_stream_base64_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-7 base64 data.",
		 function );

		return( -1 );
	}
	safe_utf7_stream_index       = *utf7_stream_index;
	safe_utf7_stream_base64_data = *utf7_stream_base64_data;

	if( safe_utf7_stream_index >= utf7_stream_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: UTF-7 stream too small.",
		 function );

		return( -1 );
	}
	if( ( safe_utf7_stream_base64_data & LIBUNA_UTF7_IS_BASE64_ENCODED ) != 0 )
	{
		base64_triplet  = safe_utf7_stream_base64_data & 0x00ffffff;
		number_of_bytes = ( safe_utf7_stream_base64_data >> 24 ) & 0x03;
		current_byte    = ( safe_utf7_stream_base64_data >> 28 ) & 0x03;

		if( current_byte >= number_of_bytes )
		{
			if( safe_utf7_stream_index >= utf7_stream_size )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: invalid UTF-7 stream character size value out of bounds.",
				 function );

				return( -1 );
			}
			utf7_character_value = utf7_stream[ safe_utf7_stream_index ];

			/* Any character not in the modified base64 alphabet terminates the base64 encoded sequence
			 */
			if( libuna_unicode_character_utf7_valid_base64_character[ utf7_character_value ] == 0 )
			{
				safe_utf7_stream_base64_data = 0;
			}
		}
	}
	if( ( safe_utf7_stream_base64_data & LIBUNA_UTF7_IS_BASE64_ENCODED ) == 0 )
	{
		if( safe_utf7_stream_index >= utf7_stream_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid UTF-7 stream character size value out of bounds.",
			 function );

			return( -1 );
		}
		utf7_character_value = utf7_stream[ safe_utf7_stream_index ];

		/* Determine if the character is modified base64 encoded
		 * or a + character
		 */
		if( utf7_character_value == (uint8_t) '+' )
		{
			if( ( safe_utf7_stream_index + 1 ) >= utf7_stream_size )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: invalid UTF-7 stream character size value out of bounds.",
				 function );

				return( -1 );
			}
			if( utf7_stream[ safe_utf7_stream_index + 1 ] != (uint8_t) '-' )
			{
				safe_utf7_stream_base64_data = LIBUNA_UTF7_IS_BASE64_ENCODED;

				safe_utf7_stream_index++;
			}
		}
		/* Allow for the end of string character
		 */
		else if( utf7_character_value == 0 )
		{
		}
		else if( libuna_unicode_character_utf7_valid_directly_encoded_character[ utf7_character_value ] == 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: invalid directly encoded UTF-7 character byte: 0x%02" PRIx8 ".",
			 function,
			 utf7_character_value );

			return( -1 );
		}
	}
	if( ( safe_utf7_stream_base64_data & LIBUNA_UTF7_IS_BASE64_ENCODED ) == 0 )
	{
		safe_unicode_character = utf7_stream[ safe_utf7_stream_index++ ];

		if( ( safe_unicode_character == (libuna_unicode_character_t) '+' )
		 && ( utf7_stream[ safe_utf7_stream_index ] == (uint8_t) '-' ) )
		{
			safe_utf7_stream_index++;
		}
	}
	else if( ( number_of_bytes == 0 )
	      || ( current_byte >= number_of_bytes ) )
	{
		if( libuna_base64_triplet_copy_from_base64_stream(
		     &base64_triplet,
		     utf7_stream,
		     utf7_stream_size - 1,
		     &safe_utf7_stream_index,
		     &padding_size,
		     LIBUNA_BASE64_VARIANT_UTF7,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_CONVERSION,
			 LIBCERROR_CONVERSION_ERROR_OUTPUT_FAILED,
			 "%s: unable to copy base64 encoded UTF-7 characters.",
			 function );

			return( -1 );
		}
		if( padding_size > 2 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported padding in base64 encoded UTF-7 characters.",
			 function );

			return( -1 );
		}
		number_of_bytes = 3 - padding_size;
		current_byte    = 0;
	}
	if( ( safe_utf7_stream_base64_data & LIBUNA_UTF7_IS_BASE64_ENCODED ) != 0 )
	{
		byte_bit_shift         = 16 - ( current_byte * 8 );
		safe_unicode_character = ( ( base64_triplet >> byte_bit_shift ) & 0x000000ffUL ) << 8;
		current_byte          += 1;

		if( current_byte >= number_of_bytes )
		{
			if( libuna_base64_triplet_copy_from_base64_stream(
			     &base64_triplet,
			     utf7_stream,
			     utf7_stream_size - 1,
			     &safe_utf7_stream_index,
			     &padding_size,
			     LIBUNA_BASE64_VARIANT_UTF7,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_CONVERSION,
				 LIBCERROR_CONVERSION_ERROR_OUTPUT_FAILED,
				 "%s: unable to copy base64 encoded UTF-7 characters.",
				 function );

				return( -1 );
			}
			if( padding_size > 2 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
				 "%s: unsupported padding in base64 encoded UTF-7 characters.",
				 function );

				return( -1 );
			}
			number_of_bytes = 3 - padding_size;
			current_byte    = 0;
		}
		byte_bit_shift          = 16 - ( current_byte * 8 );
		safe_unicode_character += ( base64_triplet >> byte_bit_shift ) & 0x000000ffUL;
		current_byte           += 1;

		if( ( safe_unicode_character >= LIBUNA_UNICODE_SURROGATE_HIGH_RANGE_START )
		 && ( safe_unicode_character <= LIBUNA_UNICODE_SURROGATE_HIGH_RANGE_END ) )
		{
			if( current_byte >= number_of_bytes )
			{
				if( libuna_base64_triplet_copy_from_base64_stream(
				     &base64_triplet,
				     utf7_stream,
				     utf7_stream_size - 1,
				     &safe_utf7_stream_index,
				     &padding_size,
				     LIBUNA_BASE64_VARIANT_UTF7,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_CONVERSION,
					 LIBCERROR_CONVERSION_ERROR_OUTPUT_FAILED,
					 "%s: unable to copy base64 encoded UTF-7 characters.",
					 function );

					return( -1 );
				}
				if( padding_size > 2 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
					 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
					 "%s: unsupported padding in base64 encoded UTF-7 characters.",
					 function );

					return( -1 );
				}
				number_of_bytes = 3 - padding_size;
				current_byte    = 0;
			}
			byte_bit_shift  = 16 - ( current_byte * 8 );
			utf16_surrogate = ( ( base64_triplet >> byte_bit_shift ) & 0x000000ffUL ) << 8;
			current_byte   += 1;

			if( current_byte >= number_of_bytes )
			{
				if( libuna_base64_triplet_copy_from_base64_stream(
				     &base64_triplet,
				     utf7_stream,
				     utf7_stream_size - 1,
				     &safe_utf7_stream_index,
				     &padding_size,
				     LIBUNA_BASE64_VARIANT_UTF7,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve base64 encoded UTF-7 characters.",
					 function );

					return( -1 );
				}
				if( padding_size > 2 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
					 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
					 "%s: unsupported padding in base64 encoded UTF-7 characters.",
					 function );

					return( -1 );
				}
				number_of_bytes = 3 - padding_size;
				current_byte    = 0;
			}
			byte_bit_shift   = 16 - ( current_byte * 8 );
			utf16_surrogate += ( base64_triplet >> byte_bit_shift ) & 0x000000ffUL;
			current_byte    += 1;

			/* Determine if the UTF-16 character is within the low surrogate range
			 */
			if( ( utf16_surrogate >= LIBUNA_UNICODE_SURROGATE_LOW_RANGE_START )
			 && ( utf16_surrogate <= LIBUNA_UNICODE_SURROGATE_LOW_RANGE_END ) )
			{
				safe_unicode_character  -= LIBUNA_UNICODE_SURROGATE_HIGH_RANGE_START;
				safe_unicode_character <<= 10;
				safe_unicode_character  += utf16_surrogate - LIBUNA_UNICODE_SURROGATE_LOW_RANGE_START;
				safe_unicode_character  += 0x010000;
			}
			else
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
				 "%s: unsupported low surrogate UTF-16 character.",
				 function );

				return( -1 );
			}
		}
		if( safe_utf7_stream_index >= utf7_stream_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
			 "%s: UTF-7 stream too small.",
			 function );

			return( -1 );
		}
		if( ( current_byte >= number_of_bytes )
		 && ( utf7_stream[ safe_utf7_stream_index ] == (uint8_t) '-' ) )
		{
			safe_utf7_stream_base64_data = 0;

			safe_utf7_stream_index++;
		}
	}
	if( ( safe_utf7_stream_base64_data & LIBUNA_UTF7_IS_BASE64_ENCODED ) != 0 )
	{
		safe_utf7_stream_base64_data  = LIBUNA_UTF7_IS_BASE64_ENCODED;
		safe_utf7_stream_base64_data |= (uint32_t) current_byte << 28;
		safe_utf7_stream_base64_data |= (uint32_t) number_of_bytes << 24;
		safe_utf7_stream_base64_data |= base64_triplet & 0x00ffffff;
	}
	*unicode_character       = safe_unicode_character;
	*utf7_stream_index       = safe_utf7_stream_index;
	*utf7_stream_base64_data = safe_utf7_stream_base64_data;

	return( 1 );
}

/* Copies an Unicode character into a UTF-7 stream
 * The bits of the base64 data contain:
 *   0 - 23 the base64 triplet
 *  24 - 25 the number of bytes in the triplet
 *  26 - 27 unused
 *  28 - 29 the current byte
 *       30 unused
 *       31 flag to indicate the current UTF-7 characters are (modified) base64 encoded
 *
 * Returns 1 if successful or -1 on error
 */
int libuna_unicode_character_copy_to_utf7_stream(
     libuna_unicode_character_t unicode_character,
     uint8_t *utf7_stream,
     size_t utf7_stream_size,
     size_t *utf7_stream_index,
     uint32_t *utf7_stream_base64_data,
     libcerror_error_t **error )
{
	static char *function                    = "libuna_unicode_character_copy_to_utf7_stream";
	libuna_utf16_character_t utf16_surrogate = 0;
	size_t safe_utf7_stream_index            = 0;
	uint32_t base64_triplet                  = 0;
	uint32_t safe_utf7_stream_base64_data    = 0;
	uint8_t base64_encode_character          = 0;
	uint8_t byte_bit_shift                   = 0;
	uint8_t current_byte                     = 0;
	uint8_t number_of_bytes                  = 0;

	if( utf7_stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-7 stream.",
		 function );

		return( -1 );
	}
	if( utf7_stream_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid UTF-7 stream size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( utf7_stream_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-7 stream index.",
		 function );

		return( -1 );
	}
	if( utf7_stream_base64_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-7 stream base64 data.",
		 function );

		return( -1 );
	}
	safe_utf7_stream_index       = *utf7_stream_index;
	safe_utf7_stream_base64_data = *utf7_stream_base64_data;

	/* Determine if the Unicode character is valid
	 */
	if( ( ( unicode_character >= LIBUNA_UNICODE_SURROGATE_HIGH_RANGE_START )
	  &&  ( unicode_character <= LIBUNA_UNICODE_SURROGATE_LOW_RANGE_END ) )
	 || ( unicode_character > LIBUNA_UNICODE_CHARACTER_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported Unicode character.",
		 function );

		return( -1 );
	}
	/* A-Z is not a continous range on an EBCDIC based system
	 * it consists of the ranges: A-I, J-R, S-Z
	 */
	if( ( unicode_character >= 0x41 )
	 && ( unicode_character <= 0x49 ) )
	{
		unicode_character = ( unicode_character - 0x41 ) + (libuna_unicode_character_t) 'A';
	}
	else if( ( unicode_character >= 0x4a )
	      && ( unicode_character <= 0x52 ) )
	{
		unicode_character = ( unicode_character - 0x4a ) + (libuna_unicode_character_t) 'J';
	}
	else if( ( unicode_character >= 0x53 )
	      && ( unicode_character <= 0x5a ) )
	{
		unicode_character = ( unicode_character - 0x53 ) + (libuna_unicode_character_t) 'S';
	}
	/* a-z is not a continous range on an EBCDIC based system
	 * it consists of the ranges: a-i, j-r, s-z
	 */
	else if( ( unicode_character >= 0x61 )
	      && ( unicode_character <= 0x69 ) )
	{
		unicode_character = ( unicode_character - 0x61 ) + (libuna_unicode_character_t) 'a';
	}
	else if( ( unicode_character >= 0x6a )
	      && ( unicode_character <= 0x72 ) )
	{
		unicode_character = ( unicode_character - 0x6a ) + (libuna_unicode_character_t) 'j';
	}
	else if( ( unicode_character >= 0x73 )
	      && ( unicode_character <= 0x7a ) )
	{
		unicode_character = ( unicode_character - 0x73 ) + (libuna_unicode_character_t) 's';
	}
	/* 0-9
	 */
	else if( ( unicode_character >= 0x30 )
	      && ( unicode_character <= 0x39 ) )
	{
		unicode_character = ( unicode_character - 0x30 ) + (libuna_unicode_character_t) '0';
	}
	/* The + character must be escaped
	 */
	else if( unicode_character == (libuna_unicode_character_t) '+' )
	{
	}
	/* Allow for the end of string character
	 */
	else if( unicode_character == 0 )
	{
	}
	else if( ( unicode_character >= 256 )
	      || ( libuna_unicode_character_utf7_valid_directly_encoded_character[ (uint8_t) unicode_character ] == 0 ) )
	{
		base64_encode_character = 1;
	}
	if( base64_encode_character == 0 )
	{
		if( ( safe_utf7_stream_base64_data & LIBUNA_UTF7_IS_BASE64_ENCODED ) != 0 )
		{
			safe_utf7_stream_base64_data = 0;
		}
		if( safe_utf7_stream_index >= utf7_stream_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
			 "%s: UTF-7 stream too small.",
			 function );

			return( -1 );
		}
		utf7_stream[ safe_utf7_stream_index++ ] = (uint8_t) unicode_character;

		/* The + character must be escaped
		 */
		if( unicode_character == (libuna_unicode_character_t) '+' )
		{
			if( safe_utf7_stream_index >= utf7_stream_size )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
				 "%s: UTF-7 stream too small.",
				 function );

				return( -1 );
			}
			utf7_stream[ safe_utf7_stream_index++ ] = (uint8_t) '-';
		}
	}
	else
	{
		/* Escape the base64 encoded chracters with a +
		 */
		if( ( safe_utf7_stream_base64_data & LIBUNA_UTF7_IS_BASE64_ENCODED ) == 0 )
		{
			if( safe_utf7_stream_index >= utf7_stream_size )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
				 "%s: UTF-7 stream too small.",
				 function );

				return( -1 );
			}
			utf7_stream[ safe_utf7_stream_index++ ] = (uint8_t) '+';
		}
		/* Otherwise continue the previously base64 encoded characters
		 */
		else
		{
			base64_triplet  = safe_utf7_stream_base64_data & 0x00ffffff;
			number_of_bytes = ( safe_utf7_stream_base64_data >> 24 ) & 0x03;
			current_byte    = ( safe_utf7_stream_base64_data >> 28 ) & 0x03;

			if( number_of_bytes > 0 )
			{
				/* Correct the index for the last partial base64 stream
				 */
				safe_utf7_stream_index -= number_of_bytes + 1;
			}
			/* Correct the index for the base64 stream termination character
			 */
			safe_utf7_stream_index -= 1;
		}
		safe_utf7_stream_base64_data = LIBUNA_UTF7_IS_BASE64_ENCODED;

		if( unicode_character > LIBUNA_UNICODE_BASIC_MULTILINGUAL_PLANE_MAX )
		{
			unicode_character -= 0x010000;

			utf16_surrogate = (libuna_utf16_character_t) ( ( unicode_character >> 10 )
			                + LIBUNA_UNICODE_SURROGATE_HIGH_RANGE_START );

			byte_bit_shift   = 16 - ( current_byte * 8 );
			base64_triplet  += (uint32_t) ( ( utf16_surrogate >> 8 ) & 0xff ) << byte_bit_shift;
			current_byte    += 1;
			number_of_bytes += 1;

			if( number_of_bytes == 3 )
			{
				if( libuna_base64_triplet_copy_to_base64_stream(
				     base64_triplet,
				     utf7_stream,
				     utf7_stream_size,
				     &safe_utf7_stream_index,
				     0,
				     LIBUNA_BASE64_VARIANT_UTF7,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
					 "%s: unable to set base64 encoded UTF-7 characters.",
					 function );

					return( -1 );
				}
				number_of_bytes = 0;
				current_byte    = 0;
				base64_triplet  = 0;
			}
			byte_bit_shift   = 16 - ( current_byte * 8 );
			base64_triplet  += (uint32_t) ( utf16_surrogate & 0xff ) << byte_bit_shift;
			current_byte    += 1;
			number_of_bytes += 1;

			if( number_of_bytes == 3 )
			{
				if( libuna_base64_triplet_copy_to_base64_stream(
				     base64_triplet,
				     utf7_stream,
				     utf7_stream_size,
				     &safe_utf7_stream_index,
				     0,
				     LIBUNA_BASE64_VARIANT_UTF7,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
					 "%s: unable to set base64 encoded UTF-7 characters.",
					 function );

					return( -1 );
				}
				number_of_bytes = 0;
				current_byte    = 0;
				base64_triplet  = 0;
			}
			unicode_character = (libuna_utf16_character_t) ( ( unicode_character & 0x03ff )
			                  + LIBUNA_UNICODE_SURROGATE_LOW_RANGE_START );
		}
		byte_bit_shift   = 16 - ( current_byte * 8 );
		base64_triplet  += (uint32_t) ( ( unicode_character >> 8 ) & 0xff ) << byte_bit_shift;
		current_byte    += 1;
		number_of_bytes += 1;

		if( number_of_bytes == 3 )
		{
			if( libuna_base64_triplet_copy_to_base64_stream(
			     base64_triplet,
			     utf7_stream,
			     utf7_stream_size,
			     &safe_utf7_stream_index,
			     0,
			     LIBUNA_BASE64_VARIANT_UTF7,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set base64 encoded UTF-7 characters.",
				 function );

				return( -1 );
			}
			number_of_bytes = 0;
			current_byte    = 0;
			base64_triplet  = 0;
		}
		byte_bit_shift   = 16 - ( current_byte * 8 );
		base64_triplet  += (uint32_t) ( unicode_character & 0xff ) << byte_bit_shift;
		current_byte    += 1;
		number_of_bytes += 1;

		if( number_of_bytes == 3 )
		{
			if( libuna_base64_triplet_copy_to_base64_stream(
			     base64_triplet,
			     utf7_stream,
			     utf7_stream_size,
			     &safe_utf7_stream_index,
			     0,
			     LIBUNA_BASE64_VARIANT_UTF7,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set base64 encoded UTF-7 characters.",
				 function );

				return( -1 );
			}
			number_of_bytes = 0;
			current_byte    = 0;
			base64_triplet  = 0;
		}
		/* Terminate the base64 encoded characters
		 */
		if( number_of_bytes > 0 )
		{
			if( libuna_base64_triplet_copy_to_base64_stream(
			     base64_triplet,
			     utf7_stream,
			     utf7_stream_size,
			     &safe_utf7_stream_index,
			     3 - number_of_bytes,
			     LIBUNA_BASE64_VARIANT_UTF7,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set base64 encoded UTF-7 characters.",
				 function );

				return( -1 );
			}
		}
		if( safe_utf7_stream_index >= utf7_stream_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
			 "%s: UTF-7 stream too small.",
			 function );

			return( -1 );
		}
		utf7_stream[ safe_utf7_stream_index++ ] = (uint8_t) '-';
	}
	if( ( safe_utf7_stream_base64_data & LIBUNA_UTF7_IS_BASE64_ENCODED ) != 0 )
	{
		safe_utf7_stream_base64_data  = LIBUNA_UTF7_IS_BASE64_ENCODED;
		safe_utf7_stream_base64_data |= (uint32_t) current_byte << 28;
		safe_utf7_stream_base64_data |= (uint32_t) number_of_bytes << 24;
		safe_utf7_stream_base64_data |= base64_triplet & 0x00ffffff;
	}
	*utf7_stream_index       = safe_utf7_stream_index;
	*utf7_stream_base64_data = safe_utf7_stream_base64_data;

	return( 1 );
}

/* Determines the size of an UTF-8 character from an Unicode character
 * This function supports upto U+10FFFF (4 byte UTF-8 characters)
 * Adds the size to the UTF-8 character size value
 * Returns 1 if successful or -1 on error
 */
int libuna_unicode_character_size_to_utf8(
     libuna_unicode_character_t unicode_character,
     size_t *utf8_character_size,
     libcerror_error_t **error )
{
	static char *function           = "libuna_unicode_character_size_to_utf8";
	size_t safe_utf8_character_size = 0;

	if( utf8_character_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-8 character size.",
		 function );

		return( -1 );
	}
	/* Determine if the Unicode character is valid
	 */
	if( ( ( unicode_character >= LIBUNA_UNICODE_SURROGATE_HIGH_RANGE_START )
	  &&  ( unicode_character <= LIBUNA_UNICODE_SURROGATE_LOW_RANGE_END ) )
	 || ( unicode_character > LIBUNA_UNICODE_CHARACTER_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported Unicode character.",
		 function );

		return( -1 );
	}
	/* RFC 3629 limits the UTF-8 character to consist of a maximum of 4 bytes
	 * while its predecessor RFC 2279 allowed up to 6 bytes
	 */
	if( unicode_character < 0x00000080UL )
	{
		safe_utf8_character_size += 1;
	}
	else if( unicode_character < 0x00000800UL )
	{
		safe_utf8_character_size += 2;
	}
	else if( unicode_character < 0x00010000UL )
	{
		safe_utf8_character_size += 3;
	}
	else
	{
		safe_utf8_character_size += 4;
	}
	*utf8_character_size += safe_utf8_character_size;

	return( 1 );
}

/* Copies an Unicode character from an UTF-8 string
 * This function supports upto U+10FFFF (4 byte UTF-8 characters)
 * Returns 1 if successful or -1 on error
 */
int libuna_unicode_character_copy_from_utf8(
     libuna_unicode_character_t *unicode_character,
     const libuna_utf8_character_t *utf8_string,
     size_t utf8_string_size,
     size_t *utf8_string_index,
     libcerror_error_t **error )
{
	static char *function                             = "libuna_unicode_character_copy_from_utf8";
	libuna_unicode_character_t safe_unicode_character = 0;
	size_t safe_utf8_string_index                     = 0;
	uint8_t byte_value1                               = 0;
	uint8_t byte_value2                               = 0;
	uint8_t byte_value3                               = 0;
	uint8_t byte_value4                               = 0;
	uint8_t utf8_character_additional_bytes           = 0;
	int result                                        = 0;

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
	if( utf8_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-8 string.",
		 function );

		return( -1 );
	}
	if( utf8_string_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid UTF-8 string size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( utf8_string_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-8 string index.",
		 function );

		return( -1 );
	}
	safe_utf8_string_index = *utf8_string_index;

	if( safe_utf8_string_index >= utf8_string_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: UTF-8 string too small.",
		 function );

		return( -1 );
	}
	/* Determine the number of additional bytes of the UTF-8 character
	 */
	byte_value1 = utf8_string[ safe_utf8_string_index ];

	/* Determine the UTF-8 character and make sure it is valid
	 * RFC 3629 limits the UTF-8 character to consist of a maximum of 4 bytes
	 * while its predecessor RFC 2279 allowed up to 6 bytes
	 */
	if( byte_value1 > 0xf4 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid 1st UTF-8 character byte: 0x%02" PRIx8 ".",
		 function,
		 byte_value1 );

		return( -1 );
	}
	if( byte_value1 < 0xc0 )
	{
		utf8_character_additional_bytes = 0;
	}
	else if( byte_value1 < 0xe0 )
	{
		utf8_character_additional_bytes = 1;
	}
	else if( byte_value1 < 0xf0 )
	{
		utf8_character_additional_bytes = 2;
	}
	else
	{
		utf8_character_additional_bytes = 3;
	}
	if( ( ( (size_t) utf8_character_additional_bytes + 1 ) > utf8_string_size )
	 || ( safe_utf8_string_index > ( utf8_string_size - ( utf8_character_additional_bytes + 1 ) ) ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: missing UTF-8 character bytes.",
		 function );

		return( -1 );
	}
	safe_unicode_character = byte_value1;

	if( utf8_character_additional_bytes == 0 )
	{
		if( byte_value1 >= 0x80 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: invalid 1st UTF-8 character byte: 0x%02" PRIx8 ".",
			 function,
			 byte_value1 );

			return( -1 );
		}
	}
	if( utf8_character_additional_bytes >= 1 )
	{
		byte_value2 = utf8_string[ safe_utf8_string_index + 1 ];

		if( ( byte_value2 < 0x80 )
		 || ( byte_value2 > 0xbf ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: invalid 2nd UTF-8 character byte: 0x%02" PRIx8 ".",
			 function,
			 byte_value2 );

			return( -1 );
		}
		result = 1;

		switch( byte_value1 )
		{
			case 0xe0:
				if( ( byte_value2 < 0xa0 )
				 || ( byte_value2 > 0xbf ) )
				{
					result = 0;
				}
				break;

			case 0xed:
				if( ( byte_value2 < 0x80 )
				 || ( byte_value2 > 0x9f ) )
				{
					result = 0;
				}
				break;

			case 0xf0:
				if( ( byte_value2 < 0x90 )
				 || ( byte_value2 > 0xbf ) )
				{
					result = 0;
				}
				break;

			case 0xf4:
				if( ( byte_value2 < 0x80 )
				 || ( byte_value2 > 0xbf ) )
				{
					result = 0;
				}
				break;

			default:
				break;
		}
		if( result == 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: invalid 1st and 2nd UTF-8 character byte pair: 0x%02" PRIx8 " 0x%02" PRIx8 ".",
			 function,
			 byte_value1,
			 byte_value2 );

			return( -1 );
		}
		safe_unicode_character <<= 6;
		safe_unicode_character += byte_value2;

		if( utf8_character_additional_bytes == 1 )
		{
			safe_unicode_character -= 0x03080;
		}
	}
	if( utf8_character_additional_bytes >= 2 )
	{
		byte_value3 = utf8_string[ safe_utf8_string_index + 2 ];

		if( ( byte_value3 < 0x80 )
		 || ( byte_value3 > 0xbf ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: invalid 3rd UTF-8 character byte: 0x%02" PRIx8 ".",
			 function,
			 byte_value3 );

			return( -1 );
		}
		result = 1;

		switch( byte_value2 )
		{
			case 0xe0:
				if( ( byte_value2 < 0xa0 )
				 || ( byte_value2 > 0xbf ) )
				{
					result = 0;
				}
				break;

			case 0xed:
				if( ( byte_value2 < 0x80 )
				 || ( byte_value2 > 0x9f ) )
				{
					result = 0;
				}
				break;

			default:
				break;
		}
		if( result == 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: invalid 2nd and 3rd UTF-8 character byte pair: 0x%02" PRIx8 " 0x%02" PRIx8 ".",
			 function,
			 byte_value2,
			 byte_value3 );

			return( -1 );
		}
		safe_unicode_character <<= 6;
		safe_unicode_character += byte_value3;

		if( utf8_character_additional_bytes == 2 )
		{
			safe_unicode_character -= 0x0e2080;
		}
	}
	if( utf8_character_additional_bytes >= 3 )
	{
		byte_value4 = utf8_string[ safe_utf8_string_index + 3 ];

		if( ( byte_value4 < 0x80 )
		 || ( byte_value4 > 0xbf ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: invalid 4th UTF-8 character byte: 0x%02" PRIx8 ".",
			 function,
			 byte_value4 );

			return( -1 );
		}
		safe_unicode_character <<= 6;
		safe_unicode_character += byte_value4;

		if( utf8_character_additional_bytes == 3 )
		{
			safe_unicode_character -= 0x03c82080;
		}
	}
	/* Determine if the Unicode character is valid
	 */
	if( ( ( safe_unicode_character >= LIBUNA_UNICODE_SURROGATE_HIGH_RANGE_START )
	  &&  ( safe_unicode_character <= LIBUNA_UNICODE_SURROGATE_LOW_RANGE_END ) )
	 || ( safe_unicode_character > LIBUNA_UNICODE_CHARACTER_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported Unicode character.",
		 function );

		return( -1 );
	}
	*unicode_character = safe_unicode_character;
	*utf8_string_index = safe_utf8_string_index + 1 + utf8_character_additional_bytes;

	return( 1 );
}

/* Copies an Unicode character into a UTF-8 string
 * This function supports upto U+10FFFF (4 byte UTF-8 characters)
 * Returns 1 if successful or -1 on error
 */
int libuna_unicode_character_copy_to_utf8(
     libuna_unicode_character_t unicode_character,
     libuna_utf8_character_t *utf8_string,
     size_t utf8_string_size,
     size_t *utf8_string_index,
     libcerror_error_t **error )
{
	static char *function                   = "libuna_unicode_character_copy_to_utf8";
	size_t safe_utf8_string_index           = 0;
	size_t utf8_character_iterator          = 0;
	uint8_t utf8_character_additional_bytes = 0;
	uint8_t utf8_first_character_mark       = 0;

	if( utf8_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-8 string.",
		 function );

		return( -1 );
	}
	if( utf8_string_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid UTF-8 string size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( utf8_string_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-8 string index.",
		 function );

		return( -1 );
	}
	safe_utf8_string_index = *utf8_string_index;

	if( safe_utf8_string_index >= utf8_string_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: UTF-8 string too small.",
		 function );

		return( -1 );
	}
	/* Determine if the Unicode character is valid
	 */
	if( ( ( unicode_character >= LIBUNA_UNICODE_SURROGATE_HIGH_RANGE_START )
	  &&  ( unicode_character <= LIBUNA_UNICODE_SURROGATE_LOW_RANGE_END ) )
	 || ( unicode_character > LIBUNA_UNICODE_CHARACTER_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported Unicode character.",
		 function );

		return( -1 );
	}
	/* Determine how many UTF-8 character bytes are required
	 */
	if( unicode_character < 0x080 )
	{
		utf8_character_additional_bytes = 0;
		utf8_first_character_mark       = 0;
	}
	else if( unicode_character < 0x0800 )
	{
		utf8_character_additional_bytes = 1;
		utf8_first_character_mark       = 0x0c0;
	}
	else if( unicode_character < 0x010000 )
	{
		utf8_character_additional_bytes = 2;
		utf8_first_character_mark       = 0x0e0;
	}
	else
	{
		utf8_character_additional_bytes = 3;
		utf8_first_character_mark       = 0x0f0;
	}
	/* Convert Unicode character into UTF-8 character bytes
	 */
	if( ( utf8_character_additional_bytes > utf8_string_size )
	 || ( safe_utf8_string_index >= ( utf8_string_size - utf8_character_additional_bytes ) ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: UTF-8 string too small.",
		 function );

		return( -1 );
	}
	for( utf8_character_iterator = safe_utf8_string_index + utf8_character_additional_bytes;
	     utf8_character_iterator > safe_utf8_string_index;
	     utf8_character_iterator-- )
	{
		utf8_string[ utf8_character_iterator ] = (libuna_utf8_character_t) ( ( unicode_character & 0x0bf ) | 0x080 );

		unicode_character >>= 6;
	}
	utf8_string[ safe_utf8_string_index ] = (libuna_utf8_character_t) ( unicode_character | utf8_first_character_mark );

	*utf8_string_index = safe_utf8_string_index + 1 + utf8_character_additional_bytes;

	return( 1 );
}

/* Determines the size of an UTF-8 character from an Unicode character
 * This function supports upto U+7FFFFFF (6 byte UTF-8 characters)
 * Adds the size to the UTF-8 character size value
 * Returns 1 if successful or -1 on error
 */
int libuna_unicode_character_size_to_utf8_rfc2279(
     libuna_unicode_character_t unicode_character,
     size_t *utf8_character_size,
     libcerror_error_t **error )
{
	static char *function           = "libuna_unicode_character_size_to_utf8_rfc2279";
	size_t safe_utf8_character_size = 0;

	if( utf8_character_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-8 character size.",
		 function );

		return( -1 );
	}
	if( unicode_character > LIBUNA_UCS_CHARACTER_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported Unicode character.",
		 function );

		return( -1 );
	}
	/* RFC 3629 limits the UTF-8 character to consist of a maximum of 4 bytes
	 * while its predecessor RFC 2279 allowed up to 6 bytes
	 */
	if( unicode_character < 0x00000080UL )
	{
		safe_utf8_character_size += 1;
	}
	else if( unicode_character < 0x00000800UL )
	{
		safe_utf8_character_size += 2;
	}
	else if( unicode_character < 0x00010000UL )
	{
		safe_utf8_character_size += 3;
	}
	else if( unicode_character < 0x00200000UL )
	{
		safe_utf8_character_size += 4;
	}
	else if( unicode_character < 0x04000000UL )
	{
		safe_utf8_character_size += 5;
	}
	else
	{
		safe_utf8_character_size += 6;
	}
	*utf8_character_size += safe_utf8_character_size;

	return( 1 );
}

/* Copies an Unicode character from an UTF-8 string
 * This function supports upto U+7FFFFFF (6 byte UTF-8 characters)
 * Returns 1 if successful or -1 on error
 */
int libuna_unicode_character_copy_from_utf8_rfc2279(
     libuna_unicode_character_t *unicode_character,
     const libuna_utf8_character_t *utf8_string,
     size_t utf8_string_size,
     size_t *utf8_string_index,
     libcerror_error_t **error )
{
	static char *function                             = "libuna_unicode_character_copy_from_utf8_rfc2279";
	libuna_unicode_character_t safe_unicode_character = 0;
	size_t safe_utf8_string_index                     = 0;
	uint8_t byte_value1                               = 0;
	uint8_t byte_value2                               = 0;
	uint8_t byte_value3                               = 0;
	uint8_t byte_value4                               = 0;
	uint8_t byte_value5                               = 0;
	uint8_t byte_value6                               = 0;
	uint8_t utf8_character_additional_bytes           = 0;

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
	if( utf8_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-8 string.",
		 function );

		return( -1 );
	}
	if( utf8_string_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid UTF-8 string size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( utf8_string_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-8 string index.",
		 function );

		return( -1 );
	}
	safe_utf8_string_index = *utf8_string_index;

	if( safe_utf8_string_index >= utf8_string_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: UTF-8 string too small.",
		 function );

		return( -1 );
	}
	/* Determine the number of additional bytes of the UTF-8 character
	 */
	byte_value1 = utf8_string[ safe_utf8_string_index ];

	/* Determine the UTF-8 character and make sure it is valid
	 * RFC 3629 limits the UTF-8 character to consist of a maximum of 4 bytes
	 * while its predecessor RFC 2279 allowed up to 6 bytes
	 */
	if( byte_value1 > 0xfd )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid 1st UTF-8 character byte: 0x%02" PRIx8 ".",
		 function,
		 byte_value1 );

		return( -1 );
	}
	if( byte_value1 < 0xc0 )
	{
		utf8_character_additional_bytes = 0;
	}
	else if( byte_value1 < 0xe0 )
	{
		utf8_character_additional_bytes = 1;
	}
	else if( byte_value1 < 0xf0 )
	{
		utf8_character_additional_bytes = 2;
	}
	else if( byte_value1 < 0xf8 )
	{
		utf8_character_additional_bytes = 3;
	}
	else if( byte_value1 < 0xfc )
	{
		utf8_character_additional_bytes = 4;
	}
	else
	{
		utf8_character_additional_bytes = 5;
	}
	if( ( ( (size_t) utf8_character_additional_bytes + 1 ) > utf8_string_size )
	 || ( safe_utf8_string_index > ( utf8_string_size - ( utf8_character_additional_bytes + 1 ) ) ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: missing UTF-8 character bytes.",
		 function );

		return( -1 );
	}
	/* Determine the UTF-8 character and make sure it is valid
	 * RFC 3629 limits the UTF-8 character to consist of a maximum of 4 bytes
	 * while its predecessor RFC 2279 allowed up to 6 bytes
	 */
	safe_unicode_character = byte_value1;

	if( utf8_character_additional_bytes == 0 )
	{
		if( byte_value1 >= 0x80 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: invalid 1st UTF-8 character byte: 0x%02" PRIx8 ".",
			 function,
			 byte_value1 );

			return( -1 );
		}
	}
	if( utf8_character_additional_bytes >= 1 )
	{
		byte_value2 = utf8_string[ safe_utf8_string_index + 1 ];

		if( ( byte_value2 < 0x80 )
		 || ( byte_value2 > 0xbf ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: invalid 2nd UTF-8 character byte: 0x%02" PRIx8 ".",
			 function,
			 byte_value2 );

			return( -1 );
		}
		safe_unicode_character <<= 6;
		safe_unicode_character += byte_value2;

		if( utf8_character_additional_bytes == 1 )
		{
			safe_unicode_character -= 0x03080;
		}
	}
	if( utf8_character_additional_bytes >= 2 )
	{
		byte_value3 = utf8_string[ safe_utf8_string_index + 2 ];

		if( ( byte_value3 < 0x80 )
		 || ( byte_value3 > 0xbf ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: invalid 3rd UTF-8 character byte: 0x%02" PRIx8 ".",
			 function,
			 byte_value3 );

			return( -1 );
		}
		safe_unicode_character <<= 6;
		safe_unicode_character += byte_value3;

		if( utf8_character_additional_bytes == 2 )
		{
			safe_unicode_character -= 0x0e2080;
		}
	}
	if( utf8_character_additional_bytes >= 3 )
	{
		byte_value4 = utf8_string[ safe_utf8_string_index + 3 ];

		if( ( byte_value4 < 0x80 )
		 || ( byte_value4 > 0xbf ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: invalid 4th UTF-8 character byte: 0x%02" PRIx8 ".",
			 function,
			 byte_value4 );

			return( -1 );
		}
		safe_unicode_character <<= 6;
		safe_unicode_character += byte_value4;

		if( utf8_character_additional_bytes == 3 )
		{
			safe_unicode_character -= 0x03c82080;
		}
	}
	if( utf8_character_additional_bytes >= 4 )
	{
		byte_value5 = utf8_string[ safe_utf8_string_index + 4 ];

		if( ( byte_value5 < 0x80 )
		 || ( byte_value5 > 0xbf ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: invalid 5th UTF-8 character byte: 0x%02" PRIx8 ".",
			 function,
			 byte_value5 );

			return( -1 );
		}
		safe_unicode_character <<= 6;
		safe_unicode_character += byte_value5;

		if( utf8_character_additional_bytes == 4 )
		{
			safe_unicode_character -= 0x0fa082080;
		}
	}
	if( utf8_character_additional_bytes == 5 )
	{
		byte_value6 = utf8_string[ safe_utf8_string_index + 5 ];

		if( ( byte_value6 < 0x80 )
		  || ( byte_value6 > 0xbf ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: invalid 6th UTF-8 character byte: 0x%02" PRIx8 ".",
			 function,
			 byte_value6 );

			return( -1 );
		}
		safe_unicode_character <<= 6;
		safe_unicode_character += byte_value6;
		safe_unicode_character -= 0x082082080;
	}
	/* Determine if the Unicode character is valid
	 */
	if( safe_unicode_character > LIBUNA_UCS_CHARACTER_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported Unicode character.",
		 function );

		return( -1 );
	}
	*unicode_character = safe_unicode_character;
	*utf8_string_index = safe_utf8_string_index + 1 + utf8_character_additional_bytes;

	return( 1 );
}

/* Copies an Unicode character into a UTF-8 string
 * This function supports upto U+7FFFFFF (6 byte UTF-8 characters)
 * Returns 1 if successful or -1 on error
 */
int libuna_unicode_character_copy_to_utf8_rfc2279(
     libuna_unicode_character_t unicode_character,
     libuna_utf8_character_t *utf8_string,
     size_t utf8_string_size,
     size_t *utf8_string_index,
     libcerror_error_t **error )
{
	static char *function                   = "libuna_unicode_character_copy_to_utf8_rfc2279";
	size_t safe_utf8_string_index           = 0;
	size_t utf8_character_iterator          = 0;
	uint8_t utf8_character_additional_bytes = 0;
	uint8_t utf8_first_character_mark       = 0;

	if( utf8_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-8 string.",
		 function );

		return( -1 );
	}
	if( utf8_string_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid UTF-8 string size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( utf8_string_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-8 string index.",
		 function );

		return( -1 );
	}
	safe_utf8_string_index = *utf8_string_index;

	if( safe_utf8_string_index >= utf8_string_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: UTF-8 string too small.",
		 function );

		return( -1 );
	}
	/* Determine if the Unicode character is valid
	 */
	if( unicode_character > LIBUNA_UCS_CHARACTER_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported Unicode character.",
		 function );

		return( -1 );
	}
	/* Determine how many UTF-8 character bytes are required
	 */
	if( unicode_character < 0x080 )
	{
		utf8_character_additional_bytes = 0;
		utf8_first_character_mark       = 0;
	}
	else if( unicode_character < 0x0800 )
	{
		utf8_character_additional_bytes = 1;
		utf8_first_character_mark       = 0x0c0;
	}
	else if( unicode_character < 0x010000 )
	{
		utf8_character_additional_bytes = 2;
		utf8_first_character_mark       = 0x0e0;
	}
	else if( unicode_character < 0x0200000 )
	{
		utf8_character_additional_bytes = 3;
		utf8_first_character_mark       = 0x0f0;
	}
	else if( unicode_character < 0x0400000 )
	{
		utf8_character_additional_bytes = 4;
		utf8_first_character_mark       = 0x0f8;
	}
	else
	{
		utf8_character_additional_bytes = 5;
		utf8_first_character_mark       = 0x0fc;
	}
	/* Convert Unicode character into UTF-8 character bytes
	 */
	if( ( utf8_character_additional_bytes > utf8_string_size )
	 || ( safe_utf8_string_index >= ( utf8_string_size - utf8_character_additional_bytes ) ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: UTF-8 string too small.",
		 function );

		return( -1 );
	}
	for( utf8_character_iterator = safe_utf8_string_index + utf8_character_additional_bytes;
	     utf8_character_iterator > safe_utf8_string_index;
	     utf8_character_iterator-- )
	{
		utf8_string[ utf8_character_iterator ] = (libuna_utf8_character_t) ( ( unicode_character & 0x0bf ) | 0x080 );

		unicode_character >>= 6;
	}
	utf8_string[ safe_utf8_string_index ] = (libuna_utf8_character_t) ( unicode_character | utf8_first_character_mark );

	*utf8_string_index = safe_utf8_string_index + 1 + utf8_character_additional_bytes;

	return( 1 );
}

/* Determines the size of an UTF-16 character from an Unicode character
 * Adds the size to the UTF-16 character size value
 * Returns 1 if successful or -1 on error
 */
int libuna_unicode_character_size_to_utf16(
     libuna_unicode_character_t unicode_character,
     size_t *utf16_character_size,
     libcerror_error_t **error )
{
	static char *function = "libuna_unicode_character_size_to_utf16";

	if( utf16_character_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-16 character size.",
		 function );

		return( -1 );
	}
	if( ( ( unicode_character >= LIBUNA_UNICODE_SURROGATE_HIGH_RANGE_START )
	  &&  ( unicode_character <= LIBUNA_UNICODE_SURROGATE_LOW_RANGE_END ) )
	 || ( unicode_character > LIBUNA_UNICODE_CHARACTER_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported Unicode character.",
		 function );

		return( -1 );
	}
	if( unicode_character > LIBUNA_UNICODE_BASIC_MULTILINGUAL_PLANE_MAX )
	{
		*utf16_character_size += 2;
	}
	else
	{
		*utf16_character_size += 1;
	}
	return( 1 );
}

/* Copies an Unicode character from an UTF-16 string
 * Returns 1 if successful or -1 on error
 */
int libuna_unicode_character_copy_from_utf16(
     libuna_unicode_character_t *unicode_character,
     const libuna_utf16_character_t *utf16_string,
     size_t utf16_string_size,
     size_t *utf16_string_index,
     libcerror_error_t **error )
{
	static char *function                             = "libuna_unicode_character_copy_from_utf16";
	libuna_unicode_character_t safe_unicode_character = 0;
	libuna_utf16_character_t utf16_surrogate          = 0;
	size_t safe_utf16_string_index                    = 0;

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
	if( utf16_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-16 string.",
		 function );

		return( -1 );
	}
	if( utf16_string_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid UTF-16 string size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( utf16_string_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-16 string index.",
		 function );

		return( -1 );
	}
	safe_utf16_string_index = *utf16_string_index;

	if( safe_utf16_string_index >= utf16_string_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: UTF-16 string too small.",
		 function );

		return( -1 );
	}
	safe_unicode_character   = utf16_string[ safe_utf16_string_index ];
	safe_utf16_string_index += 1;

	/* Determine if the UTF-16 character is within the high surrogate range
	 */
	if( ( safe_unicode_character >= LIBUNA_UNICODE_SURROGATE_HIGH_RANGE_START )
	 && ( safe_unicode_character <= LIBUNA_UNICODE_SURROGATE_HIGH_RANGE_END ) )
	{
		if( safe_utf16_string_index >= utf16_string_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: missing surrogate UTF-16 character bytes.",
			 function );

			return( -1 );
		}
		utf16_surrogate          = utf16_string[ safe_utf16_string_index ];
		safe_utf16_string_index += 1;

		/* Determine if the UTF-16 character is within the low surrogate range
		 */
		if( ( utf16_surrogate >= LIBUNA_UNICODE_SURROGATE_LOW_RANGE_START )
		 && ( utf16_surrogate <= LIBUNA_UNICODE_SURROGATE_LOW_RANGE_END ) )
		{
			safe_unicode_character  -= LIBUNA_UNICODE_SURROGATE_HIGH_RANGE_START;
			safe_unicode_character <<= 10;
			safe_unicode_character  += utf16_surrogate - LIBUNA_UNICODE_SURROGATE_LOW_RANGE_START;
			safe_unicode_character  += 0x010000;
		}
		else
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported low surrogate UTF-16 character.",
			 function );

			return( -1 );
		}
	}
	/* Determine if the Unicode character is valid
	 */
	if( ( ( safe_unicode_character >= LIBUNA_UNICODE_SURROGATE_HIGH_RANGE_START )
	  &&  ( safe_unicode_character <= LIBUNA_UNICODE_SURROGATE_LOW_RANGE_END ) )
	 || ( safe_unicode_character > LIBUNA_UNICODE_CHARACTER_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported Unicode character.",
		 function );

		return( -1 );
	}
	*unicode_character  = safe_unicode_character;
	*utf16_string_index = safe_utf16_string_index;

	return( 1 );
}

/* Copies an Unicode character into a UTF-16 string
 * Returns 1 if successful or -1 on error
 */
int libuna_unicode_character_copy_to_utf16(
     libuna_unicode_character_t unicode_character,
     libuna_utf16_character_t *utf16_string,
     size_t utf16_string_size,
     size_t *utf16_string_index,
     libcerror_error_t **error )
{
	static char *function          = "libuna_unicode_character_copy_to_utf16";
	size_t safe_utf16_string_index = 0;

	if( utf16_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-16 string.",
		 function );

		return( -1 );
	}
	if( utf16_string_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid UTF-16 string size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( utf16_string_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-16 string index.",
		 function );

		return( -1 );
	}
	safe_utf16_string_index = *utf16_string_index;

	if( safe_utf16_string_index >= utf16_string_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: UTF-16 string too small.",
		 function );

		return( -1 );
	}
	/* Determine if the Unicode character is valid
	 */
	if( ( ( unicode_character >= LIBUNA_UNICODE_SURROGATE_HIGH_RANGE_START )
	  &&  ( unicode_character <= LIBUNA_UNICODE_SURROGATE_LOW_RANGE_END ) )
	 || ( unicode_character > LIBUNA_UNICODE_CHARACTER_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported Unicode character.",
		 function );

		return( -1 );
	}
	if( unicode_character <= LIBUNA_UNICODE_BASIC_MULTILINGUAL_PLANE_MAX )
	{
		utf16_string[ safe_utf16_string_index++ ] = (libuna_utf16_character_t) unicode_character;
	}
	else
	{
		if( ( utf16_string_size < 2 )
		 || ( safe_utf16_string_index > ( utf16_string_size - 2 ) ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
			 "%s: UTF-16 string too small.",
			 function );

			return( -1 );
		}
		unicode_character                        -= 0x010000;
		utf16_string[ safe_utf16_string_index++ ] = (libuna_utf16_character_t) ( ( unicode_character >> 10 ) + LIBUNA_UNICODE_SURROGATE_HIGH_RANGE_START );
		utf16_string[ safe_utf16_string_index++ ] = (libuna_utf16_character_t) ( ( unicode_character & 0x03ff ) + LIBUNA_UNICODE_SURROGATE_LOW_RANGE_START );
	}
	*utf16_string_index = safe_utf16_string_index;

	return( 1 );
}

/* Copies an Unicode character from an UTF-16 stream
 * Returns 1 if successful or -1 on error
 */
int libuna_unicode_character_copy_from_utf16_stream(
     libuna_unicode_character_t *unicode_character,
     const uint8_t *utf16_stream,
     size_t utf16_stream_size,
     size_t *utf16_stream_index,
     int byte_order,
     libcerror_error_t **error )
{
	static char *function                             = "libuna_unicode_character_copy_from_utf16_stream";
	libuna_unicode_character_t safe_unicode_character = 0;
	libuna_utf16_character_t utf16_surrogate          = 0;
	size_t safe_utf16_stream_index                    = 0;
	int byte_order_without_flags                      = 0;

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
	if( utf16_stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-16 stream.",
		 function );

		return( -1 );
	}
	if( utf16_stream_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid UTF-16 stream size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( utf16_stream_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-16 stream index.",
		 function );

		return( -1 );
	}
	byte_order_without_flags = byte_order & 0xff;

	if( ( byte_order_without_flags != LIBUNA_ENDIAN_BIG )
	 && ( byte_order_without_flags != LIBUNA_ENDIAN_LITTLE ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported byte order.",
		 function );

		return( -1 );
	}
	safe_utf16_stream_index = *utf16_stream_index;

	if( ( utf16_stream_size < 2 )
	 || ( safe_utf16_stream_index > ( utf16_stream_size - 2 ) ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: UTF-16 stream too small.",
		 function );

		return( -1 );
	}
	if( byte_order_without_flags == LIBUNA_ENDIAN_BIG )
	{
		safe_unicode_character   = utf16_stream[ safe_utf16_stream_index ];
		safe_unicode_character <<= 8;
		safe_unicode_character  += utf16_stream[ safe_utf16_stream_index + 1 ];
	}
	else if( byte_order_without_flags == LIBUNA_ENDIAN_LITTLE )
	{
		safe_unicode_character   = utf16_stream[ safe_utf16_stream_index + 1 ];
		safe_unicode_character <<= 8;
		safe_unicode_character  += utf16_stream[ safe_utf16_stream_index ];
	}
	safe_utf16_stream_index += 2;

	/* Determine if the Unicode character is valid
	 */
	if( ( safe_unicode_character >= LIBUNA_UNICODE_SURROGATE_LOW_RANGE_START )
	 && ( safe_unicode_character <= LIBUNA_UNICODE_SURROGATE_LOW_RANGE_END ) )
	{
		if( ( byte_order & LIBUNA_UTF16_STREAM_ALLOW_UNPAIRED_SURROGATE ) == 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported UTF-16 character.",
			 function );

			return( -1 );
		}
	}
	/* Determine if the UTF-16 character is within the high surrogate range
	 */
	if( ( safe_unicode_character >= LIBUNA_UNICODE_SURROGATE_HIGH_RANGE_START )
	 && ( safe_unicode_character <= LIBUNA_UNICODE_SURROGATE_HIGH_RANGE_END ) )
	{
		if( safe_utf16_stream_index > ( utf16_stream_size - 2 ) )
		{
			if( ( byte_order & LIBUNA_UTF16_STREAM_ALLOW_UNPAIRED_SURROGATE ) == 0 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
				 "%s: missing surrogate UTF-16 character bytes.",
				 function );

				return( -1 );
			}
		}
		else
		{
			if( byte_order_without_flags == LIBUNA_ENDIAN_BIG )
			{
				utf16_surrogate   = utf16_stream[ safe_utf16_stream_index ];
				utf16_surrogate <<= 8;
				utf16_surrogate  += utf16_stream[ safe_utf16_stream_index + 1 ];
			}
			else if( byte_order_without_flags == LIBUNA_ENDIAN_LITTLE )
			{
				utf16_surrogate   = utf16_stream[ safe_utf16_stream_index + 1 ];
				utf16_surrogate <<= 8;
				utf16_surrogate  += utf16_stream[ safe_utf16_stream_index ];
			}
			/* Determine if the UTF-16 character is within the low surrogate range
			 */
			if( ( utf16_surrogate >= LIBUNA_UNICODE_SURROGATE_LOW_RANGE_START )
			 && ( utf16_surrogate <= LIBUNA_UNICODE_SURROGATE_LOW_RANGE_END ) )
			{
				safe_unicode_character  -= LIBUNA_UNICODE_SURROGATE_HIGH_RANGE_START;
				safe_unicode_character <<= 10;
				safe_unicode_character  += utf16_surrogate - LIBUNA_UNICODE_SURROGATE_LOW_RANGE_START;
				safe_unicode_character  += 0x010000;

				safe_utf16_stream_index += 2;
			}
			else if( ( byte_order & LIBUNA_UTF16_STREAM_ALLOW_UNPAIRED_SURROGATE ) == 0 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
				 "%s: unsupported low surrogate UTF-16 character.",
				 function );

				return( -1 );
			}
		}
	}
	if( safe_unicode_character > LIBUNA_UNICODE_CHARACTER_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported Unicode character.",
		 function );

		return( -1 );
	}
	*unicode_character  = safe_unicode_character;
	*utf16_stream_index = safe_utf16_stream_index;

	return( 1 );
}

/* Copies an Unicode character to an UTF-16 stream
 * Returns 1 if successful or -1 on error
 */
int libuna_unicode_character_copy_to_utf16_stream(
     libuna_unicode_character_t unicode_character,
     uint8_t *utf16_stream,
     size_t utf16_stream_size,
     size_t *utf16_stream_index,
     int byte_order,
     libcerror_error_t **error )
{
	static char *function                    = "libuna_unicode_character_copy_to_utf16_stream";
	libuna_utf16_character_t utf16_surrogate = 0;
	size_t safe_utf16_stream_index           = 0;
	int byte_order_without_flags             = 0;

	if( utf16_stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-16 stream.",
		 function );

		return( -1 );
	}
	if( utf16_stream_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid UTF-16 stream size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( utf16_stream_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-16 stream index.",
		 function );

		return( -1 );
	}
	byte_order_without_flags = byte_order & 0xff;

	if( ( byte_order_without_flags != LIBUNA_ENDIAN_BIG )
	 && ( byte_order_without_flags != LIBUNA_ENDIAN_LITTLE ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported byte order.",
		 function );

		return( -1 );
	}
	safe_utf16_stream_index = *utf16_stream_index;

	/* Determine if the Unicode character is valid
	 */
	if( unicode_character > LIBUNA_UNICODE_CHARACTER_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported Unicode character.",
		 function );

		return( -1 );
	}
	if( ( unicode_character >= LIBUNA_UNICODE_SURROGATE_HIGH_RANGE_START )
	 && ( unicode_character <= LIBUNA_UNICODE_SURROGATE_LOW_RANGE_END ) )
	{
		if( ( byte_order & LIBUNA_UTF16_STREAM_ALLOW_UNPAIRED_SURROGATE ) == 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported Unicode character.",
			 function );

			return( -1 );
		}
	}
	if( unicode_character <= LIBUNA_UNICODE_BASIC_MULTILINGUAL_PLANE_MAX )
	{
		if( ( utf16_stream_size < 2 )
		 || ( safe_utf16_stream_index > ( utf16_stream_size - 2 ) ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
			 "%s: UTF-16 stream too small.",
			 function );

			return( -1 );
		}
		if( byte_order == LIBUNA_ENDIAN_BIG )
		{
			utf16_stream[ safe_utf16_stream_index + 1 ] = (uint8_t) ( unicode_character & 0xff );
			unicode_character                         >>= 8;
			utf16_stream[ safe_utf16_stream_index     ] = (uint8_t) ( unicode_character & 0xff );
		}
		else if( byte_order == LIBUNA_ENDIAN_LITTLE )
		{
			utf16_stream[ safe_utf16_stream_index     ] = (uint8_t) ( unicode_character & 0xff );
			unicode_character                         >>= 8;
			utf16_stream[ safe_utf16_stream_index + 1 ] = (uint8_t) ( unicode_character & 0xff );
		}
		safe_utf16_stream_index += 2;
	}
	else
	{
		if( ( utf16_stream_size < 4 )
		 || ( safe_utf16_stream_index > ( utf16_stream_size - 4 ) ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
			 "%s: UTF-16 stream too small.",
			 function );

			return( -1 );
		}
		unicode_character -= 0x010000;

		utf16_surrogate = (libuna_utf16_character_t) ( ( unicode_character >> 10 ) + LIBUNA_UNICODE_SURROGATE_HIGH_RANGE_START );

		if( byte_order == LIBUNA_ENDIAN_BIG )
		{
			utf16_stream[ safe_utf16_stream_index + 1 ] = (uint8_t) ( utf16_surrogate & 0xff );
			utf16_surrogate                           >>= 8;
			utf16_stream[ safe_utf16_stream_index     ] = (uint8_t) ( utf16_surrogate & 0xff );
		}
		else if( byte_order == LIBUNA_ENDIAN_LITTLE )
		{
			utf16_stream[ safe_utf16_stream_index     ] = (uint8_t) ( utf16_surrogate & 0xff );
			utf16_surrogate                           >>= 8;
			utf16_stream[ safe_utf16_stream_index + 1 ] = (uint8_t) ( utf16_surrogate & 0xff );
		}
		safe_utf16_stream_index += 2;

		utf16_surrogate = (libuna_utf16_character_t) ( ( unicode_character & 0x03ff ) + LIBUNA_UNICODE_SURROGATE_LOW_RANGE_START );

		if( byte_order == LIBUNA_ENDIAN_BIG )
		{
			utf16_stream[ safe_utf16_stream_index + 1 ] = (uint8_t) ( utf16_surrogate & 0xff );
			utf16_surrogate                           >>= 8;
			utf16_stream[ safe_utf16_stream_index     ] = (uint8_t) ( utf16_surrogate & 0xff );
		}
		else if( byte_order == LIBUNA_ENDIAN_LITTLE )
		{
			utf16_stream[ safe_utf16_stream_index     ] = (uint8_t) ( utf16_surrogate & 0xff );
			utf16_surrogate                           >>= 8;
			utf16_stream[ safe_utf16_stream_index + 1 ] = (uint8_t) ( utf16_surrogate & 0xff );
		}
		safe_utf16_stream_index += 2;
	}
	*utf16_stream_index = safe_utf16_stream_index;

	return( 1 );
}

/* Determines the size of an UTF-32 character from an Unicode character
 * Adds the size to the UTF-32 character size value
 * Returns 1 if successful or -1 on error
 */
int libuna_unicode_character_size_to_utf32(
     libuna_unicode_character_t unicode_character,
     size_t *utf32_character_size,
     libcerror_error_t **error )
{
	static char *function = "libuna_unicode_character_size_to_utf32";

	LIBUNA_UNREFERENCED_PARAMETER( unicode_character )

	if( utf32_character_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-32 character size.",
		 function );

		return( -1 );
	}
	if( ( ( unicode_character >= LIBUNA_UNICODE_SURROGATE_HIGH_RANGE_START )
	  &&  ( unicode_character <= LIBUNA_UNICODE_SURROGATE_LOW_RANGE_END ) )
	 || ( unicode_character > LIBUNA_UNICODE_CHARACTER_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported Unicode character.",
		 function );

		return( -1 );
	}
	*utf32_character_size += 1;

	return( 1 );
}

/* Copies an Unicode character from an UTF-32 string
 * Returns 1 if successful or -1 on error
 */
int libuna_unicode_character_copy_from_utf32(
     libuna_unicode_character_t *unicode_character,
     const libuna_utf32_character_t *utf32_string,
     size_t utf32_string_size,
     size_t *utf32_string_index,
     libcerror_error_t **error )
{
	static char *function                             = "libuna_unicode_character_copy_from_utf32";
	libuna_unicode_character_t safe_unicode_character = 0;
	size_t safe_utf32_string_index                    = 0;

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
	if( utf32_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-32 string.",
		 function );

		return( -1 );
	}
	if( utf32_string_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid UTF-32 string size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( utf32_string_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-32 string index.",
		 function );

		return( -1 );
	}
	safe_utf32_string_index = *utf32_string_index;

	if( safe_utf32_string_index >= utf32_string_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: UTF-32 string too small.",
		 function );

		return( -1 );
	}
	safe_unicode_character = utf32_string[ safe_utf32_string_index ];

	/* Determine if the Unicode character is valid
	 */
	if( ( ( safe_unicode_character >= LIBUNA_UNICODE_SURROGATE_HIGH_RANGE_START )
	  &&  ( safe_unicode_character <= LIBUNA_UNICODE_SURROGATE_LOW_RANGE_END ) )
	 || ( safe_unicode_character > LIBUNA_UNICODE_CHARACTER_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported Unicode character.",
		 function );

		return( -1 );
	}
	*unicode_character  = safe_unicode_character;
	*utf32_string_index = safe_utf32_string_index + 1;

	return( 1 );
}

/* Copies an Unicode character into a UTF-32 string
 * Returns 1 if successful or -1 on error
 */
int libuna_unicode_character_copy_to_utf32(
     libuna_unicode_character_t unicode_character,
     libuna_utf32_character_t *utf32_string,
     size_t utf32_string_size,
     size_t *utf32_string_index,
     libcerror_error_t **error )
{
	static char *function          = "libuna_unicode_character_copy_to_utf32";
	size_t safe_utf32_string_index = 0;

	if( utf32_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-32 string.",
		 function );

		return( -1 );
	}
	if( utf32_string_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid UTF-32 string size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( utf32_string_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-32 string index.",
		 function );

		return( -1 );
	}
	safe_utf32_string_index = *utf32_string_index;

	if( safe_utf32_string_index >= utf32_string_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: UTF-32 string too small.",
		 function );

		return( -1 );
	}
	/* Determine if the Unicode character is valid
	 */
	if( ( ( unicode_character >= LIBUNA_UNICODE_SURROGATE_HIGH_RANGE_START )
	  &&  ( unicode_character <= LIBUNA_UNICODE_SURROGATE_LOW_RANGE_END ) )
	 || ( unicode_character > LIBUNA_UNICODE_CHARACTER_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported Unicode character.",
		 function );

		return( -1 );
	}
	utf32_string[ safe_utf32_string_index ] = (libuna_utf32_character_t) unicode_character;

	*utf32_string_index = safe_utf32_string_index + 1;

	return( 1 );
}

/* Copies an Unicode character from an UTF-32 stream
 * Returns 1 if successful or -1 on error
 */
int libuna_unicode_character_copy_from_utf32_stream(
     libuna_unicode_character_t *unicode_character,
     const uint8_t *utf32_stream,
     size_t utf32_stream_size,
     size_t *utf32_stream_index,
     int byte_order,
     libcerror_error_t **error )
{
	static char *function                             = "libuna_unicode_character_copy_from_utf32_stream";
	libuna_unicode_character_t safe_unicode_character = 0;
	size_t safe_utf32_stream_index                    = 0;

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
	if( utf32_stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-32 stream.",
		 function );

		return( -1 );
	}
	if( utf32_stream_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid UTF-32 stream size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( utf32_stream_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-32 stream index.",
		 function );

		return( -1 );
	}
	safe_utf32_stream_index = *utf32_stream_index;

	if( ( utf32_stream_size < 4 )
	 || ( safe_utf32_stream_index > ( utf32_stream_size - 4 ) ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: UTF-32 stream too small.",
		 function );

		return( -1 );
	}
	if( ( byte_order != LIBUNA_ENDIAN_BIG )
	 && ( byte_order != LIBUNA_ENDIAN_LITTLE ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported byte order.",
		 function );

		return( -1 );
	}
	if( byte_order == LIBUNA_ENDIAN_BIG )
	{
		safe_unicode_character   = utf32_stream[ safe_utf32_stream_index ];
		safe_unicode_character <<= 8;
		safe_unicode_character  += utf32_stream[ safe_utf32_stream_index + 1 ];
		safe_unicode_character <<= 8;
		safe_unicode_character  += utf32_stream[ safe_utf32_stream_index + 2 ];
		safe_unicode_character <<= 8;
		safe_unicode_character  += utf32_stream[ safe_utf32_stream_index + 3 ];
	}
	else if( byte_order == LIBUNA_ENDIAN_LITTLE )
	{
		safe_unicode_character   = utf32_stream[ safe_utf32_stream_index + 3 ];
		safe_unicode_character <<= 8;
		safe_unicode_character  += utf32_stream[ safe_utf32_stream_index + 2 ];
		safe_unicode_character <<= 8;
		safe_unicode_character  += utf32_stream[ safe_utf32_stream_index + 1 ];
		safe_unicode_character <<= 8;
		safe_unicode_character  += utf32_stream[ safe_utf32_stream_index ];
	}
	/* Determine if the Unicode character is valid
	 */
	if( ( ( safe_unicode_character >= LIBUNA_UNICODE_SURROGATE_HIGH_RANGE_START )
	  &&  ( safe_unicode_character <= LIBUNA_UNICODE_SURROGATE_LOW_RANGE_END ) )
	 || ( safe_unicode_character > LIBUNA_UNICODE_CHARACTER_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported Unicode character.",
		 function );

		return( -1 );
	}
	*unicode_character  = safe_unicode_character;
	*utf32_stream_index = safe_utf32_stream_index + 4;

	return( 1 );
}

/* Copies an Unicode character to an UTF-32 stream
 * Returns 1 if successful or -1 on error
 */
int libuna_unicode_character_copy_to_utf32_stream(
     libuna_unicode_character_t unicode_character,
     uint8_t *utf32_stream,
     size_t utf32_stream_size,
     size_t *utf32_stream_index,
     int byte_order,
     libcerror_error_t **error )
{
	static char *function          = "libuna_unicode_character_copy_to_utf32_stream";
	size_t safe_utf32_stream_index = 0;

	if( utf32_stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-32 stream.",
		 function );

		return( -1 );
	}
	if( utf32_stream_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid UTF-32 stream size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( utf32_stream_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-32 stream index.",
		 function );

		return( -1 );
	}
	safe_utf32_stream_index = *utf32_stream_index;

	if( ( utf32_stream_size < 4 )
	 || ( safe_utf32_stream_index > ( utf32_stream_size - 4 ) ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: UTF-32 stream too small.",
		 function );

		return( -1 );
	}
	if( ( byte_order != LIBUNA_ENDIAN_BIG )
	 && ( byte_order != LIBUNA_ENDIAN_LITTLE ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported byte order.",
		 function );

		return( -1 );
	}
	/* Determine if the Unicode character is valid
	 */
	if( ( ( unicode_character >= LIBUNA_UNICODE_SURROGATE_HIGH_RANGE_START )
	  &&  ( unicode_character <= LIBUNA_UNICODE_SURROGATE_LOW_RANGE_END ) )
	 || ( unicode_character > LIBUNA_UNICODE_CHARACTER_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported Unicode character.",
		 function );

		return( -1 );
	}
	if( byte_order == LIBUNA_ENDIAN_BIG )
	{
		utf32_stream[ safe_utf32_stream_index + 3 ] = (uint8_t) ( unicode_character & 0xff );
		unicode_character                         >>= 8;
		utf32_stream[ safe_utf32_stream_index + 2 ] = (uint8_t) ( unicode_character & 0xff );
		unicode_character                         >>= 8;
		utf32_stream[ safe_utf32_stream_index + 1 ] = (uint8_t) ( unicode_character & 0xff );
		unicode_character                         >>= 8;
		utf32_stream[ safe_utf32_stream_index     ] = (uint8_t) ( unicode_character & 0xff );
	}
	else if( byte_order == LIBUNA_ENDIAN_LITTLE )
	{
		utf32_stream[ safe_utf32_stream_index     ] = (uint8_t) ( unicode_character & 0xff );
		unicode_character                         >>= 8;
		utf32_stream[ safe_utf32_stream_index + 1 ] = (uint8_t) ( unicode_character & 0xff );
		unicode_character                         >>= 8;
		utf32_stream[ safe_utf32_stream_index + 2 ] = (uint8_t) ( unicode_character & 0xff );
		unicode_character                        >>= 8;
		utf32_stream[ safe_utf32_stream_index + 3 ] = (uint8_t) ( unicode_character & 0xff );
	}
	*utf32_stream_index = safe_utf32_stream_index + 4;

	return( 1 );
}

