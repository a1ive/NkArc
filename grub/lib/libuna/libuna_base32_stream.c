/*
 * Base32 stream functions
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

#include "libuna_base32_stream.h"
#include "libuna_definitions.h"
#include "libuna_types.h"

static uint8_t libuna_base32_quintet_to_character_table[ 32 ] = {
	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
	'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '2', '3', '4', '5', '6', '7' };

static uint8_t libuna_base32hex_quintet_to_character_table[ 32 ] = {
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F',
	'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V' };

/* Copies a base32 character to a base32 quintet
 * Returns 1 if successful or -1 on error
 */
int libuna_base32_character_copy_to_quintet(
     uint8_t base32_character,
     uint8_t *base32_quintet,
     uint32_t base32_variant,
     libcerror_error_t **error )
{
	static char *function       = "libuna_base32_character_copy_to_quintet";
	uint8_t safe_base32_quintet = 0;

	if( base32_quintet == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid base32 quintet.",
		 function );

		return( -1 );
	}
	switch( base32_variant & 0x000f0000UL )
	{
		case LIBUNA_BASE32_VARIANT_ALPHABET_NORMAL:
			/* A-Z is not a continous range on an EBCDIC based system
			 * it consists of the ranges: A-I, J-R, S-Z
			 */
			if( ( base32_character >= (uint8_t) 'A' )
			 && ( base32_character <= (uint8_t) 'I' ) )
			{
				safe_base32_quintet = base32_character - (uint8_t) 'A';
			}
			else if( ( base32_character >= (uint8_t) 'J' )
			      && ( base32_character <= (uint8_t) 'R' ) )
			{
				safe_base32_quintet = base32_character - (uint8_t) 'J' + 9;
			}
			else if( ( base32_character >= (uint8_t) 'S' )
			      && ( base32_character <= (uint8_t) 'Z' ) )
			{
				safe_base32_quintet = base32_character - (uint8_t) 'S' + 18;
			}
			else if( ( base32_character >= (uint8_t) '2' )
			      && ( base32_character <= (uint8_t) '7' ) )
			{
				safe_base32_quintet = base32_character - (uint8_t) '2' + 26;
			}
			else
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
				 "%s: invalid base32 character: 0x%02" PRIx8 ".",
				 function,
				 base32_character );

				return( -1 );
			}
			break;

		case LIBUNA_BASE32_VARIANT_ALPHABET_HEX:
			if( ( base32_character >= (uint8_t) '0' )
			 && ( base32_character <= (uint8_t) '9' ) )
			{
				safe_base32_quintet = base32_character - (uint8_t) '0';
			}
			/* A-V is not a continous range on an EBCDIC based system
			 * it consists of the ranges: A-I, J-R, S-V
			 */
			else if( ( base32_character >= (uint8_t) 'A' )
			      && ( base32_character <= (uint8_t) 'I' ) )
			{
				safe_base32_quintet = base32_character - (uint8_t) 'A' + 10;
			}
			else if( ( base32_character >= (uint8_t) 'J' )
			      && ( base32_character <= (uint8_t) 'R' ) )
			{
				safe_base32_quintet = base32_character - (uint8_t) 'J' + 19;
			}
			else if( ( base32_character >= (uint8_t) 'S' )
			      && ( base32_character <= (uint8_t) 'V' ) )
			{
				safe_base32_quintet = base32_character - (uint8_t) 'S' + 27;
			}
			else
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
				 "%s: invalid base32 character: 0x%02" PRIx8 ".",
				 function,
				 base32_character );

				return( -1 );
			}
			break;

		default:
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported base32 variant.",
			 function );

			return( -1 );
	}
	*base32_quintet = safe_base32_quintet;

	return( 1 );
}

/* Copies a base32 quintuplet from a base32 stream
 *
 * The padding size will still be set to indicate the number of
 * quintets in the quintuplet
 *
 * Returns 1 if successful or -1 on error
 */
int libuna_base32_quintuplet_copy_from_base32_stream(
     uint64_t *base32_quintuplet,
     const uint8_t *base32_stream,
     size_t base32_stream_size,
     size_t *base32_stream_index,
     uint8_t *padding_size,
     uint32_t base32_variant,
     libcerror_error_t **error )
{
	static char *function           = "libuna_base32_quintuplet_copy_from_base32_stream";
	size_t base32_character_size    = 0;
	size_t safe_base32_stream_index = 0;
	uint64_t safe_base32_quintuplet = 0;
	uint32_t base32_character1      = 0;
	uint32_t base32_character2      = 0;
	uint8_t quintet1                = 0;
	uint8_t quintet2                = 0;
	uint8_t quintet3                = 0;
	uint8_t quintet4                = 0;
	uint8_t quintet5                = 0;
	uint8_t quintet6                = 0;
	uint8_t quintet7                = 0;
	uint8_t quintet8                = 0;
	uint8_t safe_padding_size       = 0;

	if( base32_quintuplet == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid base32 quintuplet.",
		 function );

		return( -1 );
	}
	if( base32_stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid base32 stream.",
		 function );

		return( -1 );
	}
	if( base32_stream_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid base32 stream size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( base32_stream_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid base32 stream index.",
		 function );

		return( -1 );
	}
	if( *base32_stream_index >= base32_stream_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: base32 stream string too small.",
		 function );

		return( -1 );
	}
	if( padding_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid padding size.",
		 function );

		return( -1 );
	}
	switch( base32_variant & 0x0f000000UL )
	{
		case LIBUNA_BASE32_VARIANT_PADDING_NONE:
		case LIBUNA_BASE32_VARIANT_PADDING_OPTIONAL:
		case LIBUNA_BASE32_VARIANT_PADDING_REQUIRED:
			break;

		default:
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported base32 variant.",
			 function );

			return( -1 );
	}
	switch( base32_variant & 0xf0000000UL )
	{
		case LIBUNA_BASE32_VARIANT_ENCODING_BYTE_STREAM:
			base32_character_size = 1;
			break;

		case LIBUNA_BASE32_VARIANT_ENCODING_UTF16_BIG_ENDIAN:
		case LIBUNA_BASE32_VARIANT_ENCODING_UTF16_LITTLE_ENDIAN:
			base32_character_size = 2;
			break;

		case LIBUNA_BASE32_VARIANT_ENCODING_UTF32_BIG_ENDIAN:
		case LIBUNA_BASE32_VARIANT_ENCODING_UTF32_LITTLE_ENDIAN:
			base32_character_size = 4;
			break;

		default:
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported base32 variant.",
			 function );

			return( -1 );
	}
	safe_base32_stream_index = *base32_stream_index;

	if( base32_character_size > ( base32_stream_size - safe_base32_stream_index ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: missing 1st base32 character.",
		 function );

		return( -1 );
	}
	switch( base32_variant & 0xf0000000UL )
	{
		case LIBUNA_BASE32_VARIANT_ENCODING_BYTE_STREAM:
			base32_character1 = base32_stream[ safe_base32_stream_index ];
			break;

		case LIBUNA_BASE32_VARIANT_ENCODING_UTF16_BIG_ENDIAN:
			byte_stream_copy_to_uint16_big_endian(
			 &( base32_stream[ safe_base32_stream_index ] ),
			 base32_character1 );
			break;

		case LIBUNA_BASE32_VARIANT_ENCODING_UTF16_LITTLE_ENDIAN:
			byte_stream_copy_to_uint16_little_endian(
			 &( base32_stream[ safe_base32_stream_index ] ),
			 base32_character1 );
			break;

		case LIBUNA_BASE32_VARIANT_ENCODING_UTF32_BIG_ENDIAN:
			byte_stream_copy_to_uint32_big_endian(
			 &( base32_stream[ safe_base32_stream_index ] ),
			 base32_character1 );
			break;

		case LIBUNA_BASE32_VARIANT_ENCODING_UTF32_LITTLE_ENDIAN:
			byte_stream_copy_to_uint32_little_endian(
			 &( base32_stream[ safe_base32_stream_index ] ),
			 base32_character1 );
			break;
	}
	safe_base32_stream_index += base32_character_size;

	if( ( base32_character1 & 0xffffff00UL ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid 1st base32 character.",
		 function );

		return( -1 );
	}
	if( libuna_base32_character_copy_to_quintet(
	     (uint8_t) base32_character1,
	     &quintet1,
	     base32_variant,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_INPUT_FAILED,
		 "%s: unable to copy base32 character to 1st quintet.",
		 function );

		return( -1 );
	}
	if( base32_character_size > ( base32_stream_size - safe_base32_stream_index ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: missing 2nd base32 character.",
		 function );

		return( -1 );
	}
	switch( base32_variant & 0xf0000000UL )
	{
		case LIBUNA_BASE32_VARIANT_ENCODING_BYTE_STREAM:
			base32_character1 = base32_stream[ safe_base32_stream_index ];
			break;

		case LIBUNA_BASE32_VARIANT_ENCODING_UTF16_BIG_ENDIAN:
			byte_stream_copy_to_uint16_big_endian(
			 &( base32_stream[ safe_base32_stream_index ] ),
			 base32_character1 );
			break;

		case LIBUNA_BASE32_VARIANT_ENCODING_UTF16_LITTLE_ENDIAN:
			byte_stream_copy_to_uint16_little_endian(
			 &( base32_stream[ safe_base32_stream_index ] ),
			 base32_character1 );
			break;

		case LIBUNA_BASE32_VARIANT_ENCODING_UTF32_BIG_ENDIAN:
			byte_stream_copy_to_uint32_big_endian(
			 &( base32_stream[ safe_base32_stream_index ] ),
			 base32_character1 );
			break;

		case LIBUNA_BASE32_VARIANT_ENCODING_UTF32_LITTLE_ENDIAN:
			byte_stream_copy_to_uint32_little_endian(
			 &( base32_stream[ safe_base32_stream_index ] ),
			 base32_character1 );
			break;
	}
	safe_base32_stream_index += base32_character_size;

	if( ( base32_character1 & 0xffffff00UL ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid 2nd base32 character.",
		 function );

		return( -1 );
	}
	if( libuna_base32_character_copy_to_quintet(
	     (uint8_t) base32_character1,
	     &quintet2,
	     base32_variant,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_INPUT_FAILED,
		 "%s: unable to copy base32 character to 2nd quintet.",
		 function );

		return( -1 );
	}
	safe_padding_size = 6;

	if( ( 2 * base32_character_size ) <= ( base32_stream_size - safe_base32_stream_index ) )
	{
		switch( base32_variant & 0xf0000000UL )
		{
			case LIBUNA_BASE32_VARIANT_ENCODING_BYTE_STREAM:
				base32_character1 = base32_stream[ safe_base32_stream_index ];
				base32_character2 = base32_stream[ safe_base32_stream_index + 1 ];
				break;

			case LIBUNA_BASE32_VARIANT_ENCODING_UTF16_BIG_ENDIAN:
				byte_stream_copy_to_uint16_big_endian(
				 &( base32_stream[ safe_base32_stream_index ] ),
				 base32_character1 );

				byte_stream_copy_to_uint16_big_endian(
				 &( base32_stream[ safe_base32_stream_index + 2 ] ),
				 base32_character2 );
				break;

			case LIBUNA_BASE32_VARIANT_ENCODING_UTF16_LITTLE_ENDIAN:
				byte_stream_copy_to_uint16_little_endian(
				 &( base32_stream[ safe_base32_stream_index ] ),
				 base32_character1 );

				byte_stream_copy_to_uint16_little_endian(
				 &( base32_stream[ safe_base32_stream_index + 2 ] ),
				 base32_character2 );
				break;

			case LIBUNA_BASE32_VARIANT_ENCODING_UTF32_BIG_ENDIAN:
				byte_stream_copy_to_uint32_big_endian(
				 &( base32_stream[ safe_base32_stream_index ] ),
				 base32_character1 );

				byte_stream_copy_to_uint32_big_endian(
				 &( base32_stream[ safe_base32_stream_index + 4 ] ),
				 base32_character2 );
				break;

			case LIBUNA_BASE32_VARIANT_ENCODING_UTF32_LITTLE_ENDIAN:
				byte_stream_copy_to_uint32_little_endian(
				 &( base32_stream[ safe_base32_stream_index ] ),
				 base32_character1 );

				byte_stream_copy_to_uint32_little_endian(
				 &( base32_stream[ safe_base32_stream_index + 4 ] ),
				 base32_character2 );
				break;
		}
		safe_base32_stream_index += 2 * base32_character_size;

		if( ( base32_character1 & 0xffffff00UL ) != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: invalid 3rd base32 character.",
			 function );

			return( -1 );
		}
		if( ( base32_character2 & 0xffffff00UL ) != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: invalid 4th base32 character.",
			 function );

			return( -1 );
		}
		if( ( base32_character1 == (uint32_t) '=' )
		 || ( base32_character2 == (uint32_t) '=' ) )
		{
			if( ( base32_variant & 0x0f000000UL ) == LIBUNA_BASE32_VARIANT_PADDING_NONE )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
				 "%s: padding where not supposed to - invalid 3rd or 4th base32 character.",
				 function );

				return( -1 );
			}
			if( base32_character1 != (uint32_t) '=' )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
				 "%s: invalid 3rd base32 character.",
				 function );

				return( -1 );
			}
			if( base32_character2 != (uint32_t) '=' )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
				 "%s: invalid 4th base32 character.",
				 function );

				return( -1 );
			}
		}
		else
		{
			if( libuna_base32_character_copy_to_quintet(
			     (uint8_t) base32_character1,
			     &quintet3,
			     base32_variant,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
				 "%s: invalid 3rd base32 quintet.",
				 function );

				return( -1 );
			}
			if( libuna_base32_character_copy_to_quintet(
			     (uint8_t) base32_character2,
			     &quintet4,
			     base32_variant,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
				 "%s: invalid 4th base32 quintet.",
				 function );

				return( -1 );
			}
			safe_padding_size -= 2;
		}
	}
	else if( ( base32_variant & 0x0f000000UL ) != LIBUNA_BASE32_VARIANT_PADDING_NONE )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: missing 3rd or 4th base32 character.",
		 function );

		return( -1 );
	}
	if( base32_character_size <= ( base32_stream_size - safe_base32_stream_index ) )
	{
		switch( base32_variant & 0xf0000000UL )
		{
			case LIBUNA_BASE32_VARIANT_ENCODING_BYTE_STREAM:
				base32_character1 = base32_stream[ safe_base32_stream_index ];
				break;

			case LIBUNA_BASE32_VARIANT_ENCODING_UTF16_BIG_ENDIAN:
				byte_stream_copy_to_uint16_big_endian(
				 &( base32_stream[ safe_base32_stream_index ] ),
				 base32_character1 );
				break;

			case LIBUNA_BASE32_VARIANT_ENCODING_UTF16_LITTLE_ENDIAN:
				byte_stream_copy_to_uint16_little_endian(
				 &( base32_stream[ safe_base32_stream_index ] ),
				 base32_character1 );
				break;

			case LIBUNA_BASE32_VARIANT_ENCODING_UTF32_BIG_ENDIAN:
				byte_stream_copy_to_uint32_big_endian(
				 &( base32_stream[ safe_base32_stream_index ] ),
				 base32_character1 );
				break;

			case LIBUNA_BASE32_VARIANT_ENCODING_UTF32_LITTLE_ENDIAN:
				byte_stream_copy_to_uint32_little_endian(
				 &( base32_stream[ safe_base32_stream_index ] ),
				 base32_character1 );
				break;
		}
		safe_base32_stream_index += base32_character_size;

		if( ( base32_character1 & 0xffffff00UL ) != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: invalid 5th base32 character.",
			 function );

			return( -1 );
		}
		if( base32_character1 == (uint32_t) '=' )
		{
			if( ( base32_variant & 0x0f000000UL ) == LIBUNA_BASE32_VARIANT_PADDING_NONE )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
				 "%s: padding where not supposed to - invalid 5th base32 character.",
				 function );

				return( -1 );
			}
		}
		else
		{
			if( safe_padding_size > 4 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
				 "%s: non-padding where not supposed to - invalid 5th base32 character.",
				 function );

				return( -1 );
			}
			if( libuna_base32_character_copy_to_quintet(
			     (uint8_t) base32_character1,
			     &quintet5,
			     base32_variant,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
				 "%s: invalid 5th base32 quintet.",
				 function );

				return( -1 );
			}
			safe_padding_size -= 1;
		}
	}
	else if( ( base32_variant & 0x0f000000UL ) != LIBUNA_BASE32_VARIANT_PADDING_NONE )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: missing 5th base32 character.",
		 function );

		return( -1 );
	}
	if( ( 2 * base32_character_size ) <= ( base32_stream_size - safe_base32_stream_index ) )
	{
		switch( base32_variant & 0xf0000000UL )
		{
			case LIBUNA_BASE32_VARIANT_ENCODING_BYTE_STREAM:
				base32_character1 = base32_stream[ safe_base32_stream_index ];
				base32_character2 = base32_stream[ safe_base32_stream_index + 1 ];
				break;

			case LIBUNA_BASE32_VARIANT_ENCODING_UTF16_BIG_ENDIAN:
				byte_stream_copy_to_uint16_big_endian(
				 &( base32_stream[ safe_base32_stream_index ] ),
				 base32_character1 );

				byte_stream_copy_to_uint16_big_endian(
				 &( base32_stream[ safe_base32_stream_index + 2 ] ),
				 base32_character2 );
				break;

			case LIBUNA_BASE32_VARIANT_ENCODING_UTF16_LITTLE_ENDIAN:
				byte_stream_copy_to_uint16_little_endian(
				 &( base32_stream[ safe_base32_stream_index ] ),
				 base32_character1 );

				byte_stream_copy_to_uint16_little_endian(
				 &( base32_stream[ safe_base32_stream_index + 2 ] ),
				 base32_character2 );
				break;

			case LIBUNA_BASE32_VARIANT_ENCODING_UTF32_BIG_ENDIAN:
				byte_stream_copy_to_uint32_big_endian(
				 &( base32_stream[ safe_base32_stream_index ] ),
				 base32_character1 );

				byte_stream_copy_to_uint32_big_endian(
				 &( base32_stream[ safe_base32_stream_index + 4 ] ),
				 base32_character2 );
				break;

			case LIBUNA_BASE32_VARIANT_ENCODING_UTF32_LITTLE_ENDIAN:
				byte_stream_copy_to_uint32_little_endian(
				 &( base32_stream[ safe_base32_stream_index ] ),
				 base32_character1 );

				byte_stream_copy_to_uint32_little_endian(
				 &( base32_stream[ safe_base32_stream_index + 4 ] ),
				 base32_character2 );
				break;
		}
		safe_base32_stream_index += 2 * base32_character_size;

		if( ( base32_character1 & 0xffffff00UL ) != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: invalid 6th base32 character.",
			 function );

			return( -1 );
		}
		if( ( base32_character2 & 0xffffff00UL ) != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: invalid 7th base32 character.",
			 function );

			return( -1 );
		}
		if( ( base32_character1 == (uint32_t) '=' )
		 || ( base32_character2 == (uint32_t) '=' ) )
		{
			if( ( base32_variant & 0x0f000000UL ) == LIBUNA_BASE32_VARIANT_PADDING_NONE )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
				 "%s: padding where not supposed to - invalid 6th or 7th base32 character.",
				 function );

				return( -1 );
			}
			if( base32_character1 != (uint32_t) '=' )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
				 "%s: invalid 6th base32 quintet.",
				 function );

				return( -1 );
			}
			if( base32_character2 != (uint32_t) '=' )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
				 "%s: invalid 7th base32 quintet.",
				 function );

				return( -1 );
			}
		}
		else
		{
			if( safe_padding_size > 3 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
				 "%s: non-padding where not supposed to - invalid 6th or 7th base32 character.",
				 function );

				return( -1 );
			}
			if( libuna_base32_character_copy_to_quintet(
			     (uint8_t) base32_character1,
			     &quintet6,
			     base32_variant,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
				 "%s: invalid 6th base32 quintet.",
				 function );

				return( -1 );
			}
			if( libuna_base32_character_copy_to_quintet(
			     (uint8_t) base32_character2,
			     &quintet7,
			     base32_variant,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
				 "%s: invalid 7th base32 quintet.",
				 function );

				return( -1 );
			}
			safe_padding_size -= 2;
		}
	}
	else if( ( base32_variant & 0x0f000000UL ) != LIBUNA_BASE32_VARIANT_PADDING_NONE )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: missing 6th or 7th base32 character.",
		 function );

		return( -1 );
	}
	if( base32_character_size <= ( base32_stream_size - safe_base32_stream_index ) )
	{
		switch( base32_variant & 0xf0000000UL )
		{
			case LIBUNA_BASE32_VARIANT_ENCODING_BYTE_STREAM:
				base32_character1 = base32_stream[ safe_base32_stream_index ];
				break;

			case LIBUNA_BASE32_VARIANT_ENCODING_UTF16_BIG_ENDIAN:
				byte_stream_copy_to_uint16_big_endian(
				 &( base32_stream[ safe_base32_stream_index ] ),
				 base32_character1 );
				break;

			case LIBUNA_BASE32_VARIANT_ENCODING_UTF16_LITTLE_ENDIAN:
				byte_stream_copy_to_uint16_little_endian(
				 &( base32_stream[ safe_base32_stream_index ] ),
				 base32_character1 );
				break;

			case LIBUNA_BASE32_VARIANT_ENCODING_UTF32_BIG_ENDIAN:
				byte_stream_copy_to_uint32_big_endian(
				 &( base32_stream[ safe_base32_stream_index ] ),
				 base32_character1 );
				break;

			case LIBUNA_BASE32_VARIANT_ENCODING_UTF32_LITTLE_ENDIAN:
				byte_stream_copy_to_uint32_little_endian(
				 &( base32_stream[ safe_base32_stream_index ] ),
				 base32_character1 );
				break;
		}
		safe_base32_stream_index += base32_character_size;

		if( ( base32_character1 & 0xffffff00UL ) != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: invalid 8th base32 character.",
			 function );

			return( -1 );
		}
		if( base32_character1 == (uint32_t) '=' )
		{
			if( ( base32_variant & 0x0f000000UL ) == LIBUNA_BASE32_VARIANT_PADDING_NONE )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
				 "%s: padding where not supposed to - invalid 8th base32 character.",
				 function );

				return( -1 );
			}
		}
		else
		{
			if( safe_padding_size > 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
				 "%s: non-padding where not supposed to - invalid 8th base32 character.",
				 function );

				return( -1 );
			}
			if( libuna_base32_character_copy_to_quintet(
			     (uint8_t) base32_character1,
			     &quintet8,
			     base32_variant,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
				 "%s: invalid 8th base32 quintet.",
				 function );

				return( -1 );
			}
			safe_padding_size -= 1;
		}
	}
	else if( ( base32_variant & 0x0f000000UL ) != LIBUNA_BASE32_VARIANT_PADDING_NONE )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: missing 8th base32 character.",
		 function );

		return( -1 );
	}
	safe_base32_quintuplet   = quintet1;
	safe_base32_quintuplet <<= 5;
	safe_base32_quintuplet  |= quintet2;
	safe_base32_quintuplet <<= 5;
	safe_base32_quintuplet  |= quintet3;
	safe_base32_quintuplet <<= 5;
	safe_base32_quintuplet  |= quintet4;
	safe_base32_quintuplet <<= 5;
	safe_base32_quintuplet  |= quintet5;
	safe_base32_quintuplet <<= 5;
	safe_base32_quintuplet  |= quintet6;
	safe_base32_quintuplet <<= 5;
	safe_base32_quintuplet  |= quintet7;
	safe_base32_quintuplet <<= 5;
	safe_base32_quintuplet  |= quintet8;

	*base32_stream_index = safe_base32_stream_index;
	*base32_quintuplet   = safe_base32_quintuplet;
	*padding_size        = safe_padding_size;

	return( 1 );
}

/* Copies a base32 quintuplet to a base32 stream
 * Returns 1 if successful or -1 on error
 */
int libuna_base32_quintuplet_copy_to_base32_stream(
     uint64_t base32_quintuplet,
     uint8_t *base32_stream,
     size_t base32_stream_size,
     size_t *base32_stream_index,
     uint8_t padding_size,
     uint32_t base32_variant,
     libcerror_error_t **error )
{
	uint8_t *quintet_to_character_table = NULL;
	static char *function               = "libuna_base32_quintuplet_copy_to_base32_stream";
	size_t base32_character_size        = 0;
	size_t safe_base32_stream_index     = 0;
	uint32_t base32_character1          = 0;
	uint32_t base32_character2          = 0;
	uint8_t quintet1                    = 0;
	uint8_t quintet2                    = 0;
	uint8_t quintet3                    = 0;
	uint8_t quintet4                    = 0;
	uint8_t quintet5                    = 0;
	uint8_t quintet6                    = 0;
	uint8_t quintet7                    = 0;
	uint8_t quintet8                    = 0;

	if( base32_stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid base32 stream.",
		 function );

		return( -1 );
	}
	if( base32_stream_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid base32 stream size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( base32_stream_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid base32 stream index.",
		 function );

		return( -1 );
	}
	if( *base32_stream_index >= base32_stream_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: base32 stream string too small.",
		 function );

		return( -1 );
	}
	if( padding_size > 6 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid padding size value out of bounds.",
		 function );

		return( -1 );
	}
	switch( base32_variant & 0x000f0000UL )
	{
		case LIBUNA_BASE32_VARIANT_ALPHABET_NORMAL:
			quintet_to_character_table = libuna_base32_quintet_to_character_table;
			break;

		case LIBUNA_BASE32_VARIANT_ALPHABET_HEX:
			quintet_to_character_table = libuna_base32hex_quintet_to_character_table;
			break;

		default:
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported base32 variant.",
			 function );

			return( -1 );
	}
	switch( base32_variant & 0x0f000000UL )
	{
		case LIBUNA_BASE32_VARIANT_PADDING_NONE:
		case LIBUNA_BASE32_VARIANT_PADDING_OPTIONAL:
		case LIBUNA_BASE32_VARIANT_PADDING_REQUIRED:
			break;

		default:
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported base32 variant.",
			 function );

			return( -1 );
	}
	switch( base32_variant & 0xf0000000UL )
	{
		case LIBUNA_BASE32_VARIANT_ENCODING_BYTE_STREAM:
			base32_character_size = 1;
			break;

		case LIBUNA_BASE32_VARIANT_ENCODING_UTF16_BIG_ENDIAN:
		case LIBUNA_BASE32_VARIANT_ENCODING_UTF16_LITTLE_ENDIAN:
			base32_character_size = 2;
			break;

		case LIBUNA_BASE32_VARIANT_ENCODING_UTF32_BIG_ENDIAN:
		case LIBUNA_BASE32_VARIANT_ENCODING_UTF32_LITTLE_ENDIAN:
			base32_character_size = 4;
			break;

		default:
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported base32 variant.",
			 function );

			return( -1 );
	}
	safe_base32_stream_index = *base32_stream_index;

	/* Separate the 3 bytes value into 8 x 5 bit values
	 */
	quintet8          = (uint8_t) ( base32_quintuplet & 0x1f );
	base32_quintuplet >>= 5;
	quintet7          = (uint8_t) ( base32_quintuplet & 0x1f );
	base32_quintuplet >>= 5;
	quintet6          = (uint8_t) ( base32_quintuplet & 0x1f );
	base32_quintuplet >>= 5;
	quintet5          = (uint8_t) ( base32_quintuplet & 0x1f );
	base32_quintuplet >>= 5;
	quintet4          = (uint8_t) ( base32_quintuplet & 0x1f );
	base32_quintuplet >>= 5;
	quintet3          = (uint8_t) ( base32_quintuplet & 0x1f );
	base32_quintuplet >>= 5;
	quintet2          = (uint8_t) ( base32_quintuplet & 0x1f );
	base32_quintuplet >>= 5;
	quintet1          = (uint8_t) ( base32_quintuplet & 0x1f );

	/* Spread the encoding over 2 characters if 1 byte is available
	 */
	if( ( 2 * base32_character_size ) > ( base32_stream_size - safe_base32_stream_index ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: base32 stream is too small - insufficient space for 1st and 2nd base32 characters.",
		 function );

		return( -1 );
	}
	base32_character1 = (uint32_t) quintet_to_character_table[ quintet1 ];
	base32_character2 = (uint32_t) quintet_to_character_table[ quintet2 ];

	switch( base32_variant & 0xf0000000UL )
	{
		case LIBUNA_BASE32_VARIANT_ENCODING_BYTE_STREAM:
			base32_stream[ safe_base32_stream_index     ] = (uint8_t) base32_character1;
			base32_stream[ safe_base32_stream_index + 1 ] = (uint8_t) base32_character2;
			break;

		case LIBUNA_BASE32_VARIANT_ENCODING_UTF16_BIG_ENDIAN:
			byte_stream_copy_from_uint16_big_endian(
			 &( base32_stream[ safe_base32_stream_index ] ),
			 base32_character1 );

			byte_stream_copy_from_uint16_big_endian(
			 &( base32_stream[ safe_base32_stream_index + 2 ] ),
			 base32_character2 );
			break;

		case LIBUNA_BASE32_VARIANT_ENCODING_UTF16_LITTLE_ENDIAN:
			byte_stream_copy_from_uint16_little_endian(
			 &( base32_stream[ safe_base32_stream_index ] ),
			 base32_character1 );

			byte_stream_copy_from_uint16_little_endian(
			 &( base32_stream[ safe_base32_stream_index + 2 ] ),
			 base32_character2 );
			break;

		case LIBUNA_BASE32_VARIANT_ENCODING_UTF32_BIG_ENDIAN:
			byte_stream_copy_from_uint32_big_endian(
			 &( base32_stream[ safe_base32_stream_index ] ),
			 base32_character1 );

			byte_stream_copy_from_uint32_big_endian(
			 &( base32_stream[ safe_base32_stream_index + 4 ] ),
			 base32_character2 );
			break;

		case LIBUNA_BASE32_VARIANT_ENCODING_UTF32_LITTLE_ENDIAN:
			byte_stream_copy_from_uint32_little_endian(
			 &( base32_stream[ safe_base32_stream_index ] ),
			 base32_character1 );

			byte_stream_copy_from_uint32_little_endian(
			 &( base32_stream[ safe_base32_stream_index + 4 ] ),
			 base32_character2 );
			break;
	}
	safe_base32_stream_index += 2 * base32_character_size;

	/* Spread the encoding over 4 characters if 2 bytes are available
	 * Otherwise pad the remaining bytes if required
	 */
	if( ( padding_size < 6 )
	 || ( ( base32_variant & 0x0f000000UL ) != LIBUNA_BASE32_VARIANT_PADDING_NONE ) )
	{
		if( ( 2 * base32_character_size ) > ( base32_stream_size - safe_base32_stream_index ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
			 "%s: base32 stream is too small - insufficient space for 3rd and 4th base32 characters.",
			 function );

			return( -1 );
		}
		if( padding_size < 6 )
		{
			base32_character1 = (uint32_t) quintet_to_character_table[ quintet3 ];
			base32_character2 = (uint32_t) quintet_to_character_table[ quintet4 ];
		}
		else
		{
			base32_character1 = (uint32_t) '=';
			base32_character2 = (uint32_t) '=';
		}
		switch( base32_variant & 0xf0000000UL )
		{
			case LIBUNA_BASE32_VARIANT_ENCODING_BYTE_STREAM:
				base32_stream[ safe_base32_stream_index     ] = (uint8_t) base32_character1;
				base32_stream[ safe_base32_stream_index + 1 ] = (uint8_t) base32_character2;
				break;

			case LIBUNA_BASE32_VARIANT_ENCODING_UTF16_BIG_ENDIAN:
				byte_stream_copy_from_uint16_big_endian(
				 &( base32_stream[ safe_base32_stream_index ] ),
				 base32_character1 );

				byte_stream_copy_from_uint16_big_endian(
				 &( base32_stream[ safe_base32_stream_index + 2 ] ),
				 base32_character2 );
				break;

			case LIBUNA_BASE32_VARIANT_ENCODING_UTF16_LITTLE_ENDIAN:
				byte_stream_copy_from_uint16_little_endian(
				 &( base32_stream[ safe_base32_stream_index ] ),
				 base32_character1 );

				byte_stream_copy_from_uint16_little_endian(
				 &( base32_stream[ safe_base32_stream_index + 2 ] ),
				 base32_character2 );
				break;

			case LIBUNA_BASE32_VARIANT_ENCODING_UTF32_BIG_ENDIAN:
				byte_stream_copy_from_uint32_big_endian(
				 &( base32_stream[ safe_base32_stream_index ] ),
				 base32_character1 );

				byte_stream_copy_from_uint32_big_endian(
				 &( base32_stream[ safe_base32_stream_index + 4 ] ),
				 base32_character2 );
				break;

			case LIBUNA_BASE32_VARIANT_ENCODING_UTF32_LITTLE_ENDIAN:
				byte_stream_copy_from_uint32_little_endian(
				 &( base32_stream[ safe_base32_stream_index ] ),
				 base32_character1 );

				byte_stream_copy_from_uint32_little_endian(
				 &( base32_stream[ safe_base32_stream_index + 4 ] ),
				 base32_character2 );
				break;
		}
		safe_base32_stream_index += 2 * base32_character_size;
	}
	/* Spread the encoding over 5 characters if 3 bytes are available
	 * Otherwise pad the remaining bytes if required
	 */
	if( ( padding_size < 4 )
	 || ( ( base32_variant & 0x0f000000UL ) != LIBUNA_BASE32_VARIANT_PADDING_NONE ) )
	{
		if( base32_character_size > ( base32_stream_size - safe_base32_stream_index ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
			 "%s: base32 stream is too small - insufficient space for 5th base32 character.",
			 function );

			return( -1 );
		}
		if( padding_size < 4 )
		{
			base32_character1 = (uint32_t) quintet_to_character_table[ quintet5 ];
		}
		else
		{
			base32_character1 = (uint32_t) '=';
		}
		switch( base32_variant & 0xf0000000UL )
		{
			case LIBUNA_BASE32_VARIANT_ENCODING_BYTE_STREAM:
				base32_stream[ safe_base32_stream_index ] = (uint8_t) base32_character1;
				break;

			case LIBUNA_BASE32_VARIANT_ENCODING_UTF16_BIG_ENDIAN:
				byte_stream_copy_from_uint16_big_endian(
				 &( base32_stream[ safe_base32_stream_index ] ),
				 base32_character1 );
				break;

			case LIBUNA_BASE32_VARIANT_ENCODING_UTF16_LITTLE_ENDIAN:
				byte_stream_copy_from_uint16_little_endian(
				 &( base32_stream[ safe_base32_stream_index ] ),
				 base32_character1 );
				break;

			case LIBUNA_BASE32_VARIANT_ENCODING_UTF32_BIG_ENDIAN:
				byte_stream_copy_from_uint32_big_endian(
				 &( base32_stream[ safe_base32_stream_index ] ),
				 base32_character1 );
				break;

			case LIBUNA_BASE32_VARIANT_ENCODING_UTF32_LITTLE_ENDIAN:
				byte_stream_copy_from_uint32_little_endian(
				 &( base32_stream[ safe_base32_stream_index ] ),
				 base32_character1 );
				break;
		}
		safe_base32_stream_index += base32_character_size;
	}
	/* Spread the encoding over 7 characters if 4 bytes are available
	 * Otherwise pad the remaining bytes if required
	 */
	if( ( padding_size < 3 )
	 || ( ( base32_variant & 0x0f000000UL ) != LIBUNA_BASE32_VARIANT_PADDING_NONE ) )
	{
		if( ( 2 * base32_character_size ) > ( base32_stream_size - safe_base32_stream_index ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
			 "%s: base32 stream is too small - insufficient space for 6th and 7th base32 characters.",
			 function );

			return( -1 );
		}
		if( padding_size < 3 )
		{
			base32_character1 = (uint32_t) quintet_to_character_table[ quintet6 ];
			base32_character2 = (uint32_t) quintet_to_character_table[ quintet7 ];
		}
		else
		{
			base32_character1 = (uint32_t) '=';
			base32_character2 = (uint32_t) '=';
		}
		switch( base32_variant & 0xf0000000UL )
		{
			case LIBUNA_BASE32_VARIANT_ENCODING_BYTE_STREAM:
				base32_stream[ safe_base32_stream_index     ] = (uint8_t) base32_character1;
				base32_stream[ safe_base32_stream_index + 1 ] = (uint8_t) base32_character2;
				break;

			case LIBUNA_BASE32_VARIANT_ENCODING_UTF16_BIG_ENDIAN:
				byte_stream_copy_from_uint16_big_endian(
				 &( base32_stream[ safe_base32_stream_index ] ),
				 base32_character1 );

				byte_stream_copy_from_uint16_big_endian(
				 &( base32_stream[ safe_base32_stream_index + 2 ] ),
				 base32_character2 );
				break;

			case LIBUNA_BASE32_VARIANT_ENCODING_UTF16_LITTLE_ENDIAN:
				byte_stream_copy_from_uint16_little_endian(
				 &( base32_stream[ safe_base32_stream_index ] ),
				 base32_character1 );

				byte_stream_copy_from_uint16_little_endian(
				 &( base32_stream[ safe_base32_stream_index + 2 ] ),
				 base32_character2 );
				break;

			case LIBUNA_BASE32_VARIANT_ENCODING_UTF32_BIG_ENDIAN:
				byte_stream_copy_from_uint32_big_endian(
				 &( base32_stream[ safe_base32_stream_index ] ),
				 base32_character1 );

				byte_stream_copy_from_uint32_big_endian(
				 &( base32_stream[ safe_base32_stream_index + 4 ] ),
				 base32_character2 );
				break;

			case LIBUNA_BASE32_VARIANT_ENCODING_UTF32_LITTLE_ENDIAN:
				byte_stream_copy_from_uint32_little_endian(
				 &( base32_stream[ safe_base32_stream_index ] ),
				 base32_character1 );

				byte_stream_copy_from_uint32_little_endian(
				 &( base32_stream[ safe_base32_stream_index + 4 ] ),
				 base32_character2 );
				break;
		}
		safe_base32_stream_index += 2 * base32_character_size;
	}
	/* Spread the encoding over 8 characters if 5 bytes are available
	 * Otherwise pad the remaining bytes if required
	 */
	if( ( padding_size < 1 )
	 || ( ( base32_variant & 0x0f000000UL ) != LIBUNA_BASE32_VARIANT_PADDING_NONE ) )
	{
		if( base32_character_size > ( base32_stream_size - safe_base32_stream_index ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
			 "%s: base32 stream is too small - insufficient space for 8th base32 character.",
			 function );

			return( -1 );
		}
		if( padding_size < 1 )
		{
			base32_character1 = (uint32_t) quintet_to_character_table[ quintet8 ];
		}
		else
		{
			base32_character1 = (uint32_t) '=';
		}
		switch( base32_variant & 0xf0000000UL )
		{
			case LIBUNA_BASE32_VARIANT_ENCODING_BYTE_STREAM:
				base32_stream[ safe_base32_stream_index ] = (uint8_t) base32_character1;
				break;

			case LIBUNA_BASE32_VARIANT_ENCODING_UTF16_BIG_ENDIAN:
				byte_stream_copy_from_uint16_big_endian(
				 &( base32_stream[ safe_base32_stream_index ] ),
				 base32_character1 );
				break;

			case LIBUNA_BASE32_VARIANT_ENCODING_UTF16_LITTLE_ENDIAN:
				byte_stream_copy_from_uint16_little_endian(
				 &( base32_stream[ safe_base32_stream_index ] ),
				 base32_character1 );
				break;

			case LIBUNA_BASE32_VARIANT_ENCODING_UTF32_BIG_ENDIAN:
				byte_stream_copy_from_uint32_big_endian(
				 &( base32_stream[ safe_base32_stream_index ] ),
				 base32_character1 );
				break;

			case LIBUNA_BASE32_VARIANT_ENCODING_UTF32_LITTLE_ENDIAN:
				byte_stream_copy_from_uint32_little_endian(
				 &( base32_stream[ safe_base32_stream_index ] ),
				 base32_character1 );
				break;
		}
		safe_base32_stream_index += base32_character_size;
	}
	*base32_stream_index = safe_base32_stream_index;

	return( 1 );
}

/* Copies a base32 quintuplet from a byte stream
 * Returns 1 if successful or -1 on error
 */
int libuna_base32_quintuplet_copy_from_byte_stream(
     uint64_t *base32_quintuplet,
     const uint8_t *byte_stream,
     size_t byte_stream_size,
     size_t *byte_stream_index,
     uint8_t *padding_size,
     libcerror_error_t **error )
{
	static char *function           = "libuna_base32_quintuplet_copy_from_byte_stream";
	size_t safe_byte_stream_index   = 0;
	uint64_t safe_base32_quintuplet = 0;
	uint8_t safe_padding_size       = 0;

	if( base32_quintuplet == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid base32 quintuplet.",
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
	if( *byte_stream_index >= byte_stream_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: byte stream string too small.",
		 function );

		return( -1 );
	}
	if( padding_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid padding size.",
		 function );

		return( -1 );
	}
	/* Determine the value of 5 bytes (40 bits)
	 */
	safe_byte_stream_index = *byte_stream_index;

	safe_base32_quintuplet = byte_stream[ safe_byte_stream_index++ ];
	safe_padding_size      = 6;

	safe_base32_quintuplet <<= 8;

	if( safe_byte_stream_index < byte_stream_size )
	{
		safe_base32_quintuplet |= byte_stream[ safe_byte_stream_index++ ];
		safe_padding_size      -= 2;
	}
	safe_base32_quintuplet <<= 8;

	if( safe_byte_stream_index < byte_stream_size )
	{
		safe_base32_quintuplet |= byte_stream[ safe_byte_stream_index++ ];
		safe_padding_size      -= 1;
	}
	safe_base32_quintuplet <<= 8;

	if( safe_byte_stream_index < byte_stream_size )
	{
		safe_base32_quintuplet |= byte_stream[ safe_byte_stream_index++ ];
		safe_padding_size      -= 2;
	}
	safe_base32_quintuplet <<= 8;

	if( safe_byte_stream_index < byte_stream_size )
	{
		safe_base32_quintuplet |= byte_stream[ safe_byte_stream_index++ ];
		safe_padding_size      -= 1;
	}
	*base32_quintuplet = safe_base32_quintuplet;
	*byte_stream_index = safe_byte_stream_index;
	*padding_size      = safe_padding_size;

	return( 1 );
}

/* Copies a base32 quintuplet to a byte stream
 * Returns 1 if successful or -1 on error
 */
int libuna_base32_quintuplet_copy_to_byte_stream(
     uint64_t base32_quintuplet,
     uint8_t *byte_stream,
     size_t byte_stream_size,
     size_t *byte_stream_index,
     uint8_t padding_size,
     libcerror_error_t **error )
{
	static char *function         = "libuna_base32_quintuplet_copy_to_byte_stream";
	size_t safe_byte_stream_index = 0;

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
	if( *byte_stream_index >= byte_stream_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: byte stream string too small.",
		 function );

		return( -1 );
	}
	if( padding_size > 6 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid padding size value out of bounds.",
		 function );

		return( -1 );
	}
	safe_byte_stream_index = *byte_stream_index;

	byte_stream[ safe_byte_stream_index++ ] = (uint8_t) ( ( base32_quintuplet >> 32 ) & 0xff );

	if( padding_size <= 4 )
	{
		if( safe_byte_stream_index >= byte_stream_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
			 "%s: byte stream string too small.",
			 function );

			return( -1 );
		}
		byte_stream[ safe_byte_stream_index++ ] = (uint8_t) ( ( base32_quintuplet >> 24 ) & 0xff );
	}
	if( padding_size <= 3 )
	{
		if( safe_byte_stream_index >= byte_stream_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
			 "%s: byte stream string too small.",
			 function );

			return( -1 );
		}
		byte_stream[ safe_byte_stream_index++ ] = (uint8_t) ( ( base32_quintuplet >> 16 ) & 0xff );
	}
	if( padding_size <= 2 )
	{
		if( safe_byte_stream_index >= byte_stream_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
			 "%s: byte stream string too small.",
			 function );

			return( -1 );
		}
		byte_stream[ safe_byte_stream_index++ ] = (uint8_t) ( ( base32_quintuplet >> 8 ) & 0xff );
	}
	if( padding_size == 0 )
	{
		if( safe_byte_stream_index >= byte_stream_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
			 "%s: byte stream is too small.",
			 function );

			return( -1 );
		}
		byte_stream[ safe_byte_stream_index++ ] = (uint8_t) ( base32_quintuplet & 0xff );
	}
	*byte_stream_index = safe_byte_stream_index;

	return( 1 );
}

/* Determines the size of a byte stream from a base32 stream
 *
 * LIBUNA_BASE32_FLAG_STRIP_WHITESPACE removes leading space and tab characters,
 * and trailing space, tab and end of line characters
 *
 * Returns 1 if successful or -1 on error
 */
int libuna_base32_stream_size_to_byte_stream(
     const uint8_t *base32_stream,
     size_t base32_stream_size,
     size_t *byte_stream_size,
     uint32_t base32_variant,
     uint8_t flags,
     libcerror_error_t **error )
{
	static char *function        = "libuna_base32_stream_size_to_byte_stream";
	size_t base32_character_size = 0;
	size_t base32_stream_index   = 0;
	size_t number_of_characters  = 0;
	size_t safe_byte_stream_size = 0;
	size_t whitespace_size       = 0;
	uint32_t base32_character1   = 0;
	uint32_t base32_character2   = 0;
	uint8_t character_limit      = 0;
	uint8_t padding_size         = 0;
	uint8_t strip_mode           = LIBUNA_STRIP_MODE_LEADING_WHITESPACE;

	if( base32_stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid base32 stream.",
		 function );

		return( -1 );
	}
	if( base32_stream_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid base32 stream size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( byte_stream_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid byte stream size.",
		 function );

		return( -1 );
	}
	switch( base32_variant & 0x000000ffUL )
	{
		case LIBUNA_BASE32_VARIANT_CHARACTER_LIMIT_NONE:
			character_limit = 0;
			break;

		case LIBUNA_BASE32_VARIANT_CHARACTER_LIMIT_64:
			character_limit = 64;
			break;

		default:
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported base32 variant.",
			 function );

			return( -1 );
	}
	switch( base32_variant & 0x000f0000UL )
	{
		case LIBUNA_BASE32_VARIANT_ALPHABET_NORMAL:
		case LIBUNA_BASE32_VARIANT_ALPHABET_HEX:
			break;

		default:
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported base32 variant.",
			 function );

			return( -1 );
	}
	switch( base32_variant & 0xf0000000UL )
	{
		case LIBUNA_BASE32_VARIANT_ENCODING_BYTE_STREAM:
			base32_character_size = 1;
			break;

		case LIBUNA_BASE32_VARIANT_ENCODING_UTF16_BIG_ENDIAN:
		case LIBUNA_BASE32_VARIANT_ENCODING_UTF16_LITTLE_ENDIAN:
			base32_character_size = 2;
			break;

		case LIBUNA_BASE32_VARIANT_ENCODING_UTF32_BIG_ENDIAN:
		case LIBUNA_BASE32_VARIANT_ENCODING_UTF32_LITTLE_ENDIAN:
			base32_character_size = 4;
			break;

		default:
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported base32 variant.",
			 function );

			return( -1 );
	}
	if( ( flags & ~( LIBUNA_BASE32_FLAG_STRIP_WHITESPACE ) ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported flags.",
		 function );

		return( -1 );
	}
	/* Ignore trailing whitespace
	 */
	if( base32_stream_size > base32_character_size )
	{
		base32_stream_index = base32_stream_size - base32_character_size;
		whitespace_size     = 0;

		while( base32_stream_index > base32_character_size )
		{
			switch( base32_variant & 0xf0000000UL )
			{
				case LIBUNA_BASE32_VARIANT_ENCODING_BYTE_STREAM:
					base32_character1 = base32_stream[ base32_stream_index ];
					break;

				case LIBUNA_BASE32_VARIANT_ENCODING_UTF16_BIG_ENDIAN:
					byte_stream_copy_to_uint16_big_endian(
					 &( base32_stream[ base32_stream_index ] ),
					 base32_character1 );
					break;

				case LIBUNA_BASE32_VARIANT_ENCODING_UTF16_LITTLE_ENDIAN:
					byte_stream_copy_to_uint16_little_endian(
					 &( base32_stream[ base32_stream_index ] ),
					 base32_character1 );
					break;

				case LIBUNA_BASE32_VARIANT_ENCODING_UTF32_BIG_ENDIAN:
					byte_stream_copy_to_uint32_big_endian(
					 &( base32_stream[ base32_stream_index ] ),
					 base32_character1 );
					break;

				case LIBUNA_BASE32_VARIANT_ENCODING_UTF32_LITTLE_ENDIAN:
					byte_stream_copy_to_uint32_little_endian(
					 &( base32_stream[ base32_stream_index ] ),
					 base32_character1 );
					break;
			}
			base32_stream_index -= base32_character_size;

			if( ( base32_character1 == (uint32_t) '\n' )
			 || ( base32_character1 == (uint32_t) '\r' ) )
			{
				whitespace_size += base32_character_size;

				continue;
			}
			else if( ( flags & LIBUNA_BASE32_FLAG_STRIP_WHITESPACE ) == 0 )
			{
				break;
			}
			if( ( base32_character1 == (uint32_t) ' ' )
			 || ( base32_character1 == (uint32_t) '\t' )
			 || ( base32_character1 == (uint32_t) '\v' ) )
			{
				whitespace_size += base32_character_size;
			}
			else
			{
				break;
			}
		}
		base32_stream_size -= whitespace_size;
	}
	/* Determine and ignore the padding
	 */
	if( base32_stream_size > base32_character_size )
	{
		base32_stream_index = base32_stream_size - base32_character_size;

		while( ( base32_stream_index > base32_character_size )
		    && ( padding_size <= 6 ) )
		{
			switch( base32_variant & 0xf0000000UL )
			{
				case LIBUNA_BASE32_VARIANT_ENCODING_BYTE_STREAM:
					base32_character1 = base32_stream[ base32_stream_index ];
					break;

				case LIBUNA_BASE32_VARIANT_ENCODING_UTF16_BIG_ENDIAN:
					byte_stream_copy_to_uint16_big_endian(
					 &( base32_stream[ base32_stream_index ] ),
					 base32_character1 );
					break;

				case LIBUNA_BASE32_VARIANT_ENCODING_UTF16_LITTLE_ENDIAN:
					byte_stream_copy_to_uint16_little_endian(
					 &( base32_stream[ base32_stream_index ] ),
					 base32_character1 );
					break;

				case LIBUNA_BASE32_VARIANT_ENCODING_UTF32_BIG_ENDIAN:
					byte_stream_copy_to_uint32_big_endian(
					 &( base32_stream[ base32_stream_index ] ),
					 base32_character1 );
					break;

				case LIBUNA_BASE32_VARIANT_ENCODING_UTF32_LITTLE_ENDIAN:
					byte_stream_copy_to_uint32_little_endian(
					 &( base32_stream[ base32_stream_index ] ),
					 base32_character1 );
					break;
			}
			base32_stream_index -= base32_character_size;

			if( base32_character1 == (uint32_t) '=' )
			{
				padding_size += 1;
			}
		}
		if( padding_size > 6 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_CONVERSION,
			 LIBCERROR_CONVERSION_ERROR_INPUT_FAILED,
			 "%s: invalid amount of padding - found more than 6 padding characters.",
			 function );

			return( -1 );
		}
		base32_stream_size -= padding_size * base32_character_size;
	}
	base32_stream_index = 0;
	whitespace_size     = 0;

	while( ( base32_stream_index + base32_character_size ) < base32_stream_size )
	{
		switch( base32_variant & 0xf0000000UL )
		{
			case LIBUNA_BASE32_VARIANT_ENCODING_BYTE_STREAM:
				base32_character1 = base32_stream[ base32_stream_index ];
				break;

			case LIBUNA_BASE32_VARIANT_ENCODING_UTF16_BIG_ENDIAN:
				byte_stream_copy_to_uint16_big_endian(
				 &( base32_stream[ base32_stream_index ] ),
				 base32_character1 );
				break;

			case LIBUNA_BASE32_VARIANT_ENCODING_UTF16_LITTLE_ENDIAN:
				byte_stream_copy_to_uint16_little_endian(
				 &( base32_stream[ base32_stream_index ] ),
				 base32_character1 );
				break;

			case LIBUNA_BASE32_VARIANT_ENCODING_UTF32_BIG_ENDIAN:
				byte_stream_copy_to_uint32_big_endian(
				 &( base32_stream[ base32_stream_index ] ),
				 base32_character1 );
				break;

			case LIBUNA_BASE32_VARIANT_ENCODING_UTF32_LITTLE_ENDIAN:
				byte_stream_copy_to_uint32_little_endian(
				 &( base32_stream[ base32_stream_index ] ),
				 base32_character1 );
				break;
		}
		if( ( base32_character1 == (uint32_t) '\n' )
		 || ( base32_character1 == (uint32_t) '\r' ) )
		{
			if( ( strip_mode != LIBUNA_STRIP_MODE_NON_WHITESPACE )
			 && ( strip_mode != LIBUNA_STRIP_MODE_TRAILING_WHITESPACE ) )
			{
				strip_mode = LIBUNA_STRIP_MODE_INVALID_CHARACTER;
			}
			else
			{
				/* Handle multi-character end-of-line
				 */
				if( ( base32_stream_index + base32_character_size ) < base32_stream_size )
				{
					switch( base32_variant & 0xf0000000UL )
					{
						case LIBUNA_BASE32_VARIANT_ENCODING_BYTE_STREAM:
							base32_character2 = base32_stream[ base32_stream_index + 1 ];
							break;

						case LIBUNA_BASE32_VARIANT_ENCODING_UTF16_BIG_ENDIAN:
							byte_stream_copy_to_uint16_big_endian(
							 &( base32_stream[ base32_stream_index + 2 ] ),
							 base32_character2 );
							break;

						case LIBUNA_BASE32_VARIANT_ENCODING_UTF16_LITTLE_ENDIAN:
							byte_stream_copy_to_uint16_little_endian(
							 &( base32_stream[ base32_stream_index + 2 ] ),
							 base32_character2 );
							break;

						case LIBUNA_BASE32_VARIANT_ENCODING_UTF32_BIG_ENDIAN:
							byte_stream_copy_to_uint32_big_endian(
							 &( base32_stream[ base32_stream_index + 4 ] ),
							 base32_character2 );
							break;

						case LIBUNA_BASE32_VARIANT_ENCODING_UTF32_LITTLE_ENDIAN:
							byte_stream_copy_to_uint32_little_endian(
							 &( base32_stream[ base32_stream_index + 4 ] ),
							 base32_character2 );
							break;
					}
					if( ( base32_character2 == (uint32_t) '\n' )
					 || ( base32_character2 == (uint32_t) '\r' ) )
					{
						base32_stream_index += base32_character_size;

						whitespace_size += base32_character_size;
					}
				}
				strip_mode = LIBUNA_STRIP_MODE_LEADING_WHITESPACE;
			}
			if( ( number_of_characters != 0 )
			 && ( character_limit != 0 ) )
			{
				if( number_of_characters != (size_t) character_limit )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_CONVERSION,
					 LIBCERROR_CONVERSION_ERROR_INPUT_FAILED,
					 "%s: number of characters in line: %" PRIzd " does not match character limit: %" PRIu8 ".",
					 function,
					 number_of_characters,
					 character_limit );

					return( -1 );
				}
				number_of_characters = 0;
			}
			whitespace_size += base32_character_size;
		}
		else if( ( base32_character1 == (uint32_t) ' ' )
		      || ( base32_character1 == (uint32_t) '\t' )
		      || ( base32_character1 == (uint32_t) '\v' ) )
		{
			if( ( flags & LIBUNA_BASE32_FLAG_STRIP_WHITESPACE ) == 0 )
			{
				strip_mode = LIBUNA_STRIP_MODE_INVALID_CHARACTER;
			}
			else
			{
				if( strip_mode == LIBUNA_STRIP_MODE_NON_WHITESPACE )
				{
					strip_mode = LIBUNA_STRIP_MODE_TRAILING_WHITESPACE;
				}
				if( ( strip_mode != LIBUNA_STRIP_MODE_LEADING_WHITESPACE )
				 && ( strip_mode != LIBUNA_STRIP_MODE_TRAILING_WHITESPACE ) )
				{
					strip_mode = LIBUNA_STRIP_MODE_INVALID_CHARACTER;
				}
				else
				{
					whitespace_size += base32_character_size;
				}
			}
		}
		else if( strip_mode == LIBUNA_STRIP_MODE_LEADING_WHITESPACE )
		{
			strip_mode = LIBUNA_STRIP_MODE_NON_WHITESPACE;
		}
		else if( strip_mode == LIBUNA_STRIP_MODE_TRAILING_WHITESPACE )
		{
			strip_mode = LIBUNA_STRIP_MODE_INVALID_CHARACTER;
		}
		if( strip_mode == LIBUNA_STRIP_MODE_NON_WHITESPACE )
		{
			switch( base32_variant & 0x000f0000UL )
			{
				case LIBUNA_BASE32_VARIANT_ALPHABET_NORMAL:
					/* A-Z is not a continous range on an EBCDIC based system
					 * it consists of the ranges: A-I, J-R, S-Z
					 */
					if( ( base32_character1 >= (uint32_t) 'A' )
					 && ( base32_character1 <= (uint32_t) 'I' ) )
					{
						number_of_characters++;
					}
					else if( ( base32_character1 >= (uint32_t) 'J' )
					      && ( base32_character1 <= (uint32_t) 'R' ) )
					{
						number_of_characters++;
					}
					else if( ( base32_character1 >= (uint32_t) 'S' )
					      && ( base32_character1 <= (uint32_t) 'Z' ) )
					{
						number_of_characters++;
					}
					else if( ( base32_character1 >= (uint32_t) '2' )
					      && ( base32_character1 <= (uint32_t) '7' ) )
					{
						number_of_characters++;
					}
					else
					{
						strip_mode = LIBUNA_STRIP_MODE_INVALID_CHARACTER;
					}
					break;

				case LIBUNA_BASE32_VARIANT_ALPHABET_HEX:
					if( ( base32_character1 >= (uint32_t) '0' )
					 && ( base32_character1 <= (uint32_t) '9' ) )
					{
						number_of_characters++;
					}
					/* A-V is not a continous range on an EBCDIC based system
					 * it consists of the ranges: A-I, J-R, S-V
					 */
					else if( ( base32_character1 >= (uint32_t) 'A' )
					      && ( base32_character1 <= (uint32_t) 'I' ) )
					{
						number_of_characters++;
					}
					else if( ( base32_character1 >= (uint32_t) 'J' )
					      && ( base32_character1 <= (uint32_t) 'R' ) )
					{
						number_of_characters++;
					}
					else if( ( base32_character1 >= (uint32_t) 'S' )
					      && ( base32_character1 <= (uint32_t) 'V' ) )
					{
						number_of_characters++;
					}
					else
					{
						strip_mode = LIBUNA_STRIP_MODE_INVALID_CHARACTER;
					}
					break;

				default:
					strip_mode = LIBUNA_STRIP_MODE_INVALID_CHARACTER;
			}
		}
		if( strip_mode == LIBUNA_STRIP_MODE_INVALID_CHARACTER )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_CONVERSION,
			 LIBCERROR_CONVERSION_ERROR_INPUT_FAILED,
			 "%s: invalid character in base32 stream at index: %d.",
			 function,
			 base32_stream_index );

			return( -1 );
		}
		base32_stream_index += base32_character_size;
	}
	if( character_limit != 0 )
	{
		if( number_of_characters > (size_t) character_limit )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_CONVERSION,
			 LIBCERROR_CONVERSION_ERROR_INPUT_FAILED,
			 "%s: number of characters in last line exceed maximum.",
			 function );

			return( -1 );
		}
	}
	base32_stream_size -= whitespace_size;

	/* Make sure the byte stream is able to hold
	 * at least 5 bytes for each 8 base32 characters
	 */
	safe_byte_stream_size = ( base32_stream_size  * 5 ) / ( base32_character_size * 8 );

	*byte_stream_size = safe_byte_stream_size * base32_character_size;

	return( 1 );
}

/* Copies a byte stream from a base32 stream
 *
 * LIBUNA_BASE32_FLAG_STRIP_WHITESPACE removes leading space and tab characters,
 * and trailing space, tab and end of line characters
 *
 * Returns 1 if successful or -1 on error
 */
int libuna_base32_stream_copy_to_byte_stream(
     const uint8_t *base32_stream,
     size_t base32_stream_size,
     uint8_t *byte_stream,
     size_t byte_stream_size,
     uint32_t base32_variant,
     uint8_t flags,
     libcerror_error_t **error )
{
	static char *function        = "libuna_base32_stream_copy_to_byte_stream";
	size_t base32_character_size = 0;
	size_t base32_stream_index   = 0;
	size_t byte_stream_index     = 0;
	size_t number_of_characters  = 0;
	size_t whitespace_size       = 0;
	uint64_t base32_quintuplet   = 0;
	uint32_t base32_character1   = 0;
	uint32_t base32_character2   = 0;
	uint8_t character_limit      = 0;
	uint8_t padding_size         = 0;
	uint8_t strip_mode           = LIBUNA_STRIP_MODE_LEADING_WHITESPACE;

	if( base32_stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid base32 stream.",
		 function );

		return( -1 );
	}
	if( base32_stream_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid base32 stream size value exceeds maximum.",
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
	switch( base32_variant & 0x000000ffUL )
	{
		case LIBUNA_BASE32_VARIANT_CHARACTER_LIMIT_NONE:
			character_limit = 0;
			break;

		case LIBUNA_BASE32_VARIANT_CHARACTER_LIMIT_64:
			character_limit = 64;
			break;

		default:
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported base32 variant.",
			 function );

			return( -1 );
	}
	switch( base32_variant & 0xf0000000UL )
	{
		case LIBUNA_BASE32_VARIANT_ENCODING_BYTE_STREAM:
			base32_character_size = 1;
			break;

		case LIBUNA_BASE32_VARIANT_ENCODING_UTF16_BIG_ENDIAN:
		case LIBUNA_BASE32_VARIANT_ENCODING_UTF16_LITTLE_ENDIAN:
			base32_character_size = 2;
			break;

		case LIBUNA_BASE32_VARIANT_ENCODING_UTF32_BIG_ENDIAN:
		case LIBUNA_BASE32_VARIANT_ENCODING_UTF32_LITTLE_ENDIAN:
			base32_character_size = 4;
			break;

		default:
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported base32 variant.",
			 function );

			return( -1 );
	}
	if( ( flags & ~( LIBUNA_BASE32_FLAG_STRIP_WHITESPACE ) ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported flags.",
		 function );

		return( -1 );
	}
	/* Ignore trailing whitespace
	 */
	if( base32_stream_size > base32_character_size )
	{
		base32_stream_index = base32_stream_size - base32_character_size;
		whitespace_size     = 0;

		while( base32_stream_index > base32_character_size )
		{
			switch( base32_variant & 0xf0000000UL )
			{
				case LIBUNA_BASE32_VARIANT_ENCODING_BYTE_STREAM:
					base32_character1 = base32_stream[ base32_stream_index ];
					break;

				case LIBUNA_BASE32_VARIANT_ENCODING_UTF16_BIG_ENDIAN:
					byte_stream_copy_to_uint16_big_endian(
					 &( base32_stream[ base32_stream_index ] ),
					 base32_character1 );
					break;

				case LIBUNA_BASE32_VARIANT_ENCODING_UTF16_LITTLE_ENDIAN:
					byte_stream_copy_to_uint16_little_endian(
					 &( base32_stream[ base32_stream_index ] ),
					 base32_character1 );
					break;

				case LIBUNA_BASE32_VARIANT_ENCODING_UTF32_BIG_ENDIAN:
					byte_stream_copy_to_uint32_big_endian(
					 &( base32_stream[ base32_stream_index ] ),
					 base32_character1 );
					break;

				case LIBUNA_BASE32_VARIANT_ENCODING_UTF32_LITTLE_ENDIAN:
					byte_stream_copy_to_uint32_little_endian(
					 &( base32_stream[ base32_stream_index ] ),
					 base32_character1 );
					break;
			}
			base32_stream_index -= base32_character_size;

			if( ( base32_character1 == (uint32_t) '\n' )
			 || ( base32_character1 == (uint32_t) '\r' ) )
			{
				whitespace_size += base32_character_size;

				continue;
			}
			else if( ( flags & LIBUNA_BASE32_FLAG_STRIP_WHITESPACE ) == 0 )
			{
				break;
			}
			if( ( base32_character1 == (uint32_t) ' ' )
			 || ( base32_character1 == (uint32_t) '\t' )
			 || ( base32_character1 == (uint32_t) '\v' ) )
			{
				whitespace_size += base32_character_size;
			}
			else
			{
				break;
			}
		}
		base32_stream_size -= whitespace_size;
	}
	if( ( flags & LIBUNA_BASE32_FLAG_STRIP_WHITESPACE ) == 0 )
	{
		strip_mode = LIBUNA_STRIP_MODE_NON_WHITESPACE;
	}
	base32_stream_index = 0;

	while( ( base32_stream_index + base32_character_size ) < base32_stream_size )
	{
		switch( base32_variant & 0xf0000000UL )
		{
			case LIBUNA_BASE32_VARIANT_ENCODING_BYTE_STREAM:
				base32_character1 = base32_stream[ base32_stream_index ];
				break;

			case LIBUNA_BASE32_VARIANT_ENCODING_UTF16_BIG_ENDIAN:
				byte_stream_copy_to_uint16_big_endian(
				 &( base32_stream[ base32_stream_index ] ),
				 base32_character1 );
				break;

			case LIBUNA_BASE32_VARIANT_ENCODING_UTF16_LITTLE_ENDIAN:
				byte_stream_copy_to_uint16_little_endian(
				 &( base32_stream[ base32_stream_index ] ),
				 base32_character1 );
				break;

			case LIBUNA_BASE32_VARIANT_ENCODING_UTF32_BIG_ENDIAN:
				byte_stream_copy_to_uint32_big_endian(
				 &( base32_stream[ base32_stream_index ] ),
				 base32_character1 );
				break;

			case LIBUNA_BASE32_VARIANT_ENCODING_UTF32_LITTLE_ENDIAN:
				byte_stream_copy_to_uint32_little_endian(
				 &( base32_stream[ base32_stream_index ] ),
				 base32_character1 );
				break;
		}
		if( ( base32_character1 == (uint32_t) '\n' )
		 || ( base32_character1 == (uint32_t) '\r' ) )
		{
			if( ( strip_mode != LIBUNA_STRIP_MODE_NON_WHITESPACE )
			 && ( strip_mode != LIBUNA_STRIP_MODE_TRAILING_WHITESPACE ) )
			{
				strip_mode = LIBUNA_STRIP_MODE_INVALID_CHARACTER;
			}
			else
			{
				/* Handle multi-character end-of-line
				 */
				if( ( base32_stream_index + base32_character_size ) < base32_stream_size )
				{
					switch( base32_variant & 0xf0000000UL )
					{
						case LIBUNA_BASE32_VARIANT_ENCODING_BYTE_STREAM:
							base32_character2 = base32_stream[ base32_stream_index + 1 ];
							break;

						case LIBUNA_BASE32_VARIANT_ENCODING_UTF16_BIG_ENDIAN:
							byte_stream_copy_to_uint16_big_endian(
							 &( base32_stream[ base32_stream_index + 2 ] ),
							 base32_character2 );
							break;

						case LIBUNA_BASE32_VARIANT_ENCODING_UTF16_LITTLE_ENDIAN:
							byte_stream_copy_to_uint16_little_endian(
							 &( base32_stream[ base32_stream_index + 2 ] ),
							 base32_character2 );
							break;

						case LIBUNA_BASE32_VARIANT_ENCODING_UTF32_BIG_ENDIAN:
							byte_stream_copy_to_uint32_big_endian(
							 &( base32_stream[ base32_stream_index + 4 ] ),
							 base32_character2 );
							break;

						case LIBUNA_BASE32_VARIANT_ENCODING_UTF32_LITTLE_ENDIAN:
							byte_stream_copy_to_uint32_little_endian(
							 &( base32_stream[ base32_stream_index + 4 ] ),
							 base32_character2 );
							break;
					}
					if( ( base32_character2 == (uint32_t) '\n' )
					 || ( base32_character2 == (uint32_t) '\r' ) )
					{
						base32_stream_index += base32_character_size;
					}
				}
				strip_mode = LIBUNA_STRIP_MODE_LEADING_WHITESPACE;

				base32_stream_index += base32_character_size;
			}
			if( character_limit != 0 )
			{
				if( number_of_characters != (size_t) character_limit )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_CONVERSION,
					 LIBCERROR_CONVERSION_ERROR_INPUT_FAILED,
					 "%s: number of characters in line does not match character limit.",
					 function );

					return( -1 );
				}
				number_of_characters = 0;
			}
		}
		else if( ( base32_character1 == (uint32_t) ' ' )
		      || ( base32_character1 == (uint32_t) '\t' )
		      || ( base32_character1 == (uint32_t) '\v' ) )
		{
			if( ( flags & LIBUNA_BASE32_FLAG_STRIP_WHITESPACE ) != 0 )
			{
				if( strip_mode == LIBUNA_STRIP_MODE_NON_WHITESPACE )
				{
					strip_mode = LIBUNA_STRIP_MODE_TRAILING_WHITESPACE;
				}
				if( ( strip_mode != LIBUNA_STRIP_MODE_LEADING_WHITESPACE )
				 && ( strip_mode != LIBUNA_STRIP_MODE_TRAILING_WHITESPACE ) )
				{
					strip_mode = LIBUNA_STRIP_MODE_INVALID_CHARACTER;
				}
				base32_stream_index += base32_character_size;
			}
			else
			{
				strip_mode = LIBUNA_STRIP_MODE_INVALID_CHARACTER;
			}
		}
		else if( strip_mode == LIBUNA_STRIP_MODE_LEADING_WHITESPACE )
		{
			strip_mode = LIBUNA_STRIP_MODE_NON_WHITESPACE;
		}
		else if( strip_mode == LIBUNA_STRIP_MODE_TRAILING_WHITESPACE )
		{
			strip_mode = LIBUNA_STRIP_MODE_INVALID_CHARACTER;
		}
		if( strip_mode == LIBUNA_STRIP_MODE_INVALID_CHARACTER )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_CONVERSION,
			 LIBCERROR_CONVERSION_ERROR_INPUT_FAILED,
			 "%s: invalid character in base32 stream at index: %d.",
			 function,
			 base32_stream_index );

			return( -1 );
		}
		if( strip_mode == LIBUNA_STRIP_MODE_NON_WHITESPACE )
		{
			if( padding_size > 0 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
				 "%s: invalid 1st base32 quintet.",
				 function );

				return( -1 );
			}
			/* Convert the base32 stream into a base32 quintuplet
			 */
			if( libuna_base32_quintuplet_copy_from_base32_stream(
			     &base32_quintuplet,
			     base32_stream,
			     base32_stream_size,
			     &base32_stream_index,
			     &padding_size,
			     base32_variant,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_CONVERSION,
				 LIBCERROR_CONVERSION_ERROR_INPUT_FAILED,
				 "%s: unable to copy base32 quintuplet from base32 stream.",
				 function );

				return( -1 );
			}
			/* Convert the base32 quintuplet into a byte stream
			 */
			if( libuna_base32_quintuplet_copy_to_byte_stream(
			     base32_quintuplet,
			     byte_stream,
			     byte_stream_size,
			     &byte_stream_index,
			     padding_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_CONVERSION,
				 LIBCERROR_CONVERSION_ERROR_INPUT_FAILED,
				 "%s: unable to copy base32 quintuplet to byte stream.",
				 function );

				return( -1 );
			}
			number_of_characters += 8 - padding_size;
		}
	}
	if( character_limit != 0 )
	{
		if( number_of_characters > (size_t) character_limit )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_CONVERSION,
			 LIBCERROR_CONVERSION_ERROR_INPUT_FAILED,
			 "%s: number of characters in last line exceed maximum.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Determines the size of a base32 stream from a byte stream
 * Returns 1 if successful or -1 on error
 */
int libuna_base32_stream_size_from_byte_stream(
     const uint8_t *byte_stream,
     size_t byte_stream_size,
     size_t *base32_stream_size,
     uint32_t base32_variant,
     libcerror_error_t **error )
{
	static char *function          = "libuna_base32_stream_size_from_byte_stream";
	size_t base32_character_size   = 0;
	size_t safe_base32_stream_size = 0;
	size_t whitespace_size         = 0;
	uint8_t character_limit        = 0;

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
	if( base32_stream_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid base32 stream size.",
		 function );

		return( -1 );
	}
	switch( base32_variant & 0x000000ffUL )
	{
		case LIBUNA_BASE32_VARIANT_CHARACTER_LIMIT_NONE:
			character_limit = 0;
			break;

		case LIBUNA_BASE32_VARIANT_CHARACTER_LIMIT_64:
			character_limit = 64;
			break;

		default:
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported base32 variant.",
			 function );

			return( -1 );
	}
	switch( base32_variant & 0xf0000000UL )
	{
		case LIBUNA_BASE32_VARIANT_ENCODING_BYTE_STREAM:
			base32_character_size = 1;
			break;

		case LIBUNA_BASE32_VARIANT_ENCODING_UTF16_BIG_ENDIAN:
		case LIBUNA_BASE32_VARIANT_ENCODING_UTF16_LITTLE_ENDIAN:
			base32_character_size = 2;
			break;

		case LIBUNA_BASE32_VARIANT_ENCODING_UTF32_BIG_ENDIAN:
		case LIBUNA_BASE32_VARIANT_ENCODING_UTF32_LITTLE_ENDIAN:
			base32_character_size = 4;
			break;

		default:
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported base32 variant.",
			 function );

			return( -1 );
	}
	/* Make sure the base32 stream is able to hold
	 * at least 8 base32 characters for each 5 bytes
	 */
	safe_base32_stream_size = byte_stream_size / 5;

	if( ( byte_stream_size % 5 ) != 0 )
	{
		safe_base32_stream_size += 1;
	}
	safe_base32_stream_size *= 8;

	if( character_limit != 0 )
	{
		whitespace_size = safe_base32_stream_size / character_limit;

		if( ( safe_base32_stream_size % character_limit ) != 0 )
		{
			whitespace_size += 1;
		}
		safe_base32_stream_size += whitespace_size;
	}
	*base32_stream_size = safe_base32_stream_size * base32_character_size;

	return( 1 );
}

/* Copies a base32 stream from a byte stream
 * Returns 1 if successful or -1 on error
 */
int libuna_base32_stream_copy_from_byte_stream(
     uint8_t *base32_stream,
     size_t base32_stream_size,
     const uint8_t *byte_stream,
     size_t byte_stream_size,
     uint32_t base32_variant,
     libcerror_error_t **error )
{
	static char *function      = "libuna_base32_stream_copy_from_byte_stream";
	size_t base32_stream_index = 0;

	if( libuna_base32_stream_with_index_copy_from_byte_stream(
	     base32_stream,
	     base32_stream_size,
	     &base32_stream_index,
	     byte_stream,
	     byte_stream_size,
	     base32_variant,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy base32 stream from byte stream.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Copies a base32 stream from a byte stream
 * Returns 1 if successful or -1 on error
 */
int libuna_base32_stream_with_index_copy_from_byte_stream(
     uint8_t *base32_stream,
     size_t base32_stream_size,
     size_t *base32_stream_index,
     const uint8_t *byte_stream,
     size_t byte_stream_size,
     uint32_t base32_variant,
     libcerror_error_t **error )
{
	static char *function                = "libuna_base32_stream_with_index_copy_from_byte_stream";
	size_t calculated_base32_stream_size = 0;
	size_t safe_base32_stream_index      = 0;
	size_t byte_stream_index             = 0;
	size_t number_of_characters          = 0;
	size_t whitespace_size               = 0;
	uint64_t base32_quintuplet           = 0;
	uint8_t character_limit              = 0;
	uint8_t padding_size                 = 0;

	if( base32_stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid base32 stream.",
		 function );

		return( -1 );
	}
	if( base32_stream_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid base32 stream size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( base32_stream_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid base32 stream index.",
		 function );

		return( -1 );
	}
	if( *base32_stream_index >= base32_stream_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: base32 stream string too small.",
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
	switch( base32_variant & 0x000000ffUL )
	{
		case LIBUNA_BASE32_VARIANT_CHARACTER_LIMIT_NONE:
			character_limit = 0;
			break;

		case LIBUNA_BASE32_VARIANT_CHARACTER_LIMIT_64:
			character_limit = 64;
			break;

		default:
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported base32 variant.",
			 function );

			return( -1 );
	}
	safe_base32_stream_index = *base32_stream_index;

	/* Make sure the base32 stream is able to hold
	 * at least 8 base32 characters for each 5 bytes
	 */
	calculated_base32_stream_size = byte_stream_size / 5;

	if( ( byte_stream_size % 5 ) != 0 )
	{
		calculated_base32_stream_size += 1;
	}
	calculated_base32_stream_size *= 8;

	if( character_limit != 0 )
	{
		whitespace_size = calculated_base32_stream_size / character_limit;

		if( ( calculated_base32_stream_size % character_limit ) != 0 )
		{
			whitespace_size += 1;
		}
		calculated_base32_stream_size += whitespace_size;
	}
	if( base32_stream_size < calculated_base32_stream_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: base32 stream is too small.",
		 function );

		return( -1 );
	}
	while( byte_stream_index < byte_stream_size )
	{
		/* Convert the byte stream into a base32 quintuplet
		 */
		if( libuna_base32_quintuplet_copy_from_byte_stream(
		     &base32_quintuplet,
		     byte_stream,
		     byte_stream_size,
		     &byte_stream_index,
		     &padding_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_CONVERSION,
			 LIBCERROR_CONVERSION_ERROR_INPUT_FAILED,
			 "%s: unable to copy base32 quintuplet from byte stream.",
			 function );

			return( -1 );
		}
		/* Convert the base32 quintuplet into a base32 stream
		 */
		if( libuna_base32_quintuplet_copy_to_base32_stream(
		     base32_quintuplet,
		     base32_stream,
		     base32_stream_size,
		     &safe_base32_stream_index,
		     padding_size,
		     base32_variant,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_CONVERSION,
			 LIBCERROR_CONVERSION_ERROR_INPUT_FAILED,
			 "%s: unable to copy base32 quintuplet to base32 stream.",
			 function );

			return( -1 );
		}
		if( character_limit != 0 )
		{
			number_of_characters += 8;

			if( number_of_characters >= (size_t) character_limit )
			{
				base32_stream[ safe_base32_stream_index++ ] = (uint8_t) '\n';

				number_of_characters = 0;
			}
		}
	}
	if( character_limit != 0 )
	{
		if( number_of_characters != 0 )
		{
			base32_stream[ safe_base32_stream_index++ ] = (uint8_t) '\n';
		}
	}
	*base32_stream_index = safe_base32_stream_index;

	return( 1 );
}

