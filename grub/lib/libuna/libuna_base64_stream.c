/*
 * Base64 stream functions
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
#include "libuna_definitions.h"
#include "libuna_types.h"

static uint8_t libuna_base64_sixtet_to_character_table[ 64 ] = {
	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
	'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
	'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
	'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/' };

static uint8_t libuna_base64url_sixtet_to_character_table[ 64 ] = {
	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
	'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
	'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
	'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '-', '_' };

/* Copies a base64 character to a base64 sixtet
 * Returns 1 if successful, 0 if not a valid base64 character or -1 on error
 */
int libuna_base64_character_copy_to_sixtet(
     uint32_t base64_character,
     uint8_t *base64_sixtet,
     uint32_t base64_variant,
     libcerror_error_t **error )
{
	static char *function       = "libuna_base64_character_copy_to_sixtet";
	uint32_t safe_base64_sixtet = 0;
	uint8_t base64_character_62 = 0;
	uint8_t base64_character_63 = 0;

	if( base64_sixtet == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid base64 sixtet.",
		 function );

		return( -1 );
	}
	switch( base64_variant & 0x000f0000UL )
	{
		case LIBUNA_BASE64_VARIANT_ALPHABET_NORMAL:
			base64_character_62 = (uint8_t) '+';
			base64_character_63 = (uint8_t) '/';
			break;

		case LIBUNA_BASE64_VARIANT_ALPHABET_URL:
			base64_character_62 = (uint8_t) '-';
			base64_character_63 = (uint8_t) '_';
			break;

		default:
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported base64 variant.",
			 function );

			return( -1 );
	}
	if( ( base64_character & 0xffffff00UL ) != 0 )
	{
		return( 0 );
	}
	/* A-Z is not a continous range on an EBCDIC based system
	 * it consists of the ranges: A-I, J-R, S-Z
	 */
	if( ( base64_character >= (uint32_t) 'A' )
	 && ( base64_character <= (uint32_t) 'I' ) )
	{
		safe_base64_sixtet = base64_character - (uint32_t) 'A';
	}
	else if( ( base64_character >= (uint32_t) 'J' )
	      && ( base64_character <= (uint32_t) 'R' ) )
	{
		safe_base64_sixtet = base64_character - (uint32_t) 'J' + 9;
	}
	else if( ( base64_character >= (uint32_t) 'S' )
	      && ( base64_character <= (uint32_t) 'Z' ) )
	{
		safe_base64_sixtet = base64_character - (uint32_t) 'S' + 18;
	}
	/* a-z is not a continous range on an EBCDIC based system
	 * it consists of the ranges: a-i, j-r, s-z
	 */
	else if( ( base64_character >= (uint32_t) 'a' )
	      && ( base64_character <= (uint32_t) 'i' ) )
	{
		safe_base64_sixtet = base64_character - (uint32_t) 'a' + 26;
	}
	else if( ( base64_character >= (uint32_t) 'j' )
	      && ( base64_character <= (uint32_t) 'r' ) )
	{
		safe_base64_sixtet = base64_character - (uint32_t) 'j' + 35;
	}
	else if( ( base64_character >= (uint32_t) 's' )
	      && ( base64_character <= (uint32_t) 'z' ) )
	{
		safe_base64_sixtet = base64_character - (uint32_t) 's' + 44;
	}
	else if( ( base64_character >= (uint32_t) '0' )
	      && ( base64_character <= (uint32_t) '9' ) )
	{
		safe_base64_sixtet = base64_character - (uint32_t) '0' + 52;
	}
	else if( base64_character == base64_character_62 )
	{
		safe_base64_sixtet = 62;
	}
	else if( base64_character == base64_character_63 )
	{
		safe_base64_sixtet = 63;
	}
	else
	{
		return( 0 );
	}
	*base64_sixtet = (uint8_t) ( safe_base64_sixtet & 0xff );

	return( 1 );
}

/* Copies a base64 triplet from a base64 stream
 *
 * The padding size will still be set to indicate the number of
 * sixtets in the triplet
 *
 * Returns 1 if successful or -1 on error
 */
int libuna_base64_triplet_copy_from_base64_stream(
     uint32_t *base64_triplet,
     const uint8_t *base64_stream,
     size_t base64_stream_size,
     size_t *base64_stream_index,
     uint8_t *padding_size,
     uint32_t base64_variant,
     libcerror_error_t **error )
{
	static char *function           = "libuna_base64_triplet_copy_from_base64_stream";
	size_t base64_character_size    = 0;
	size_t safe_base64_stream_index = 0;
	uint32_t base64_character       = 0;
	uint32_t safe_base64_triplet    = 0;
	uint8_t safe_padding_size       = 0;
	uint8_t sixtet1                 = 0;
	uint8_t sixtet2                 = 0;
	uint8_t sixtet3                 = 0;
	uint8_t sixtet4                 = 0;
	int result                      = 0;

	if( base64_triplet == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid base64 triplet.",
		 function );

		return( -1 );
	}
	if( base64_stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid base64 stream.",
		 function );

		return( -1 );
	}
	if( base64_stream_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid base64 stream size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( base64_stream_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid base64 stream index.",
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
	switch( base64_variant & 0x0f000000UL )
	{
		case LIBUNA_BASE64_VARIANT_PADDING_NONE:
		case LIBUNA_BASE64_VARIANT_PADDING_OPTIONAL:
		case LIBUNA_BASE64_VARIANT_PADDING_REQUIRED:
		case LIBUNA_BASE64_VARIANT_PADDING_INVALID:
			break;

		default:
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported base64 variant.",
			 function );

			return( -1 );
	}
	switch( base64_variant & 0xf0000000UL )
	{
		case LIBUNA_BASE64_VARIANT_ENCODING_BYTE_STREAM:
			base64_character_size = 1;
			break;

		case LIBUNA_BASE64_VARIANT_ENCODING_UTF16_BIG_ENDIAN:
		case LIBUNA_BASE64_VARIANT_ENCODING_UTF16_LITTLE_ENDIAN:
			base64_character_size = 2;
			break;

		case LIBUNA_BASE64_VARIANT_ENCODING_UTF32_BIG_ENDIAN:
		case LIBUNA_BASE64_VARIANT_ENCODING_UTF32_LITTLE_ENDIAN:
			base64_character_size = 4;
			break;

		default:
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported base64 variant.",
			 function );

			return( -1 );
	}
	safe_base64_stream_index = *base64_stream_index;

	if( ( base64_character_size > base64_stream_size )
	 || ( safe_base64_stream_index > ( base64_stream_size - base64_character_size ) ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: base64 stream string too small - missing 1st base64 character.",
		 function );

		return( -1 );
	}
	switch( base64_variant & 0xf0000000UL )
	{
		case LIBUNA_BASE64_VARIANT_ENCODING_BYTE_STREAM:
			base64_character = base64_stream[ safe_base64_stream_index ];
			break;

		case LIBUNA_BASE64_VARIANT_ENCODING_UTF16_BIG_ENDIAN:
			byte_stream_copy_to_uint16_big_endian(
			 &( base64_stream[ safe_base64_stream_index ] ),
			 base64_character );
			break;

		case LIBUNA_BASE64_VARIANT_ENCODING_UTF16_LITTLE_ENDIAN:
			byte_stream_copy_to_uint16_little_endian(
			 &( base64_stream[ safe_base64_stream_index ] ),
			 base64_character );
			break;

		case LIBUNA_BASE64_VARIANT_ENCODING_UTF32_BIG_ENDIAN:
			byte_stream_copy_to_uint32_big_endian(
			 &( base64_stream[ safe_base64_stream_index ] ),
			 base64_character );
			break;

		case LIBUNA_BASE64_VARIANT_ENCODING_UTF32_LITTLE_ENDIAN:
			byte_stream_copy_to_uint32_little_endian(
			 &( base64_stream[ safe_base64_stream_index ] ),
			 base64_character );
			break;
	}
	safe_base64_stream_index += base64_character_size;

	result = libuna_base64_character_copy_to_sixtet(
	          base64_character,
	          &sixtet1,
	          base64_variant,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_INPUT_FAILED,
		 "%s: unable to copy base64 character to 1st sixtet.",
		 function );

		return( -1 );
	}
	else if( result == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid 1st base64 character.",
		 function );

		return( -1 );
	}
	if( ( base64_character_size > base64_stream_size )
	 || ( safe_base64_stream_index > ( base64_stream_size - base64_character_size ) ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: base64 stream string too small - missing 2nd base64 character.",
		 function );

		return( -1 );
	}
	switch( base64_variant & 0xf0000000UL )
	{
		case LIBUNA_BASE64_VARIANT_ENCODING_BYTE_STREAM:
			base64_character = base64_stream[ safe_base64_stream_index ];
			break;

		case LIBUNA_BASE64_VARIANT_ENCODING_UTF16_BIG_ENDIAN:
			byte_stream_copy_to_uint16_big_endian(
			 &( base64_stream[ safe_base64_stream_index ] ),
			 base64_character );
			break;

		case LIBUNA_BASE64_VARIANT_ENCODING_UTF16_LITTLE_ENDIAN:
			byte_stream_copy_to_uint16_little_endian(
			 &( base64_stream[ safe_base64_stream_index ] ),
			 base64_character );
			break;

		case LIBUNA_BASE64_VARIANT_ENCODING_UTF32_BIG_ENDIAN:
			byte_stream_copy_to_uint32_big_endian(
			 &( base64_stream[ safe_base64_stream_index ] ),
			 base64_character );
			break;

		case LIBUNA_BASE64_VARIANT_ENCODING_UTF32_LITTLE_ENDIAN:
			byte_stream_copy_to_uint32_little_endian(
			 &( base64_stream[ safe_base64_stream_index ] ),
			 base64_character );
			break;
	}
	safe_base64_stream_index += base64_character_size;

	result = libuna_base64_character_copy_to_sixtet(
	          base64_character,
	          &sixtet2,
	          base64_variant,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_INPUT_FAILED,
		 "%s: unable to copy base64 character to 2nd sixtet.",
		 function );

		return( -1 );
	}
	else if( result == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid 2nd base64 character.",
		 function );

		return( -1 );
	}
	safe_padding_size = 2;

	if( ( base64_character_size > base64_stream_size )
	 || ( safe_base64_stream_index > ( base64_stream_size - base64_character_size ) ) )
	{
		if( ( base64_variant & 0x0f000000UL ) == LIBUNA_BASE64_VARIANT_PADDING_REQUIRED )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
			 "%s: base64 stream string too small - missing 3rd base64 character.",
			 function );

			return( -1 );
		}
	}
	else
	{
		switch( base64_variant & 0xf0000000UL )
		{
			case LIBUNA_BASE64_VARIANT_ENCODING_BYTE_STREAM:
				base64_character = base64_stream[ safe_base64_stream_index ];
				break;

			case LIBUNA_BASE64_VARIANT_ENCODING_UTF16_BIG_ENDIAN:
				byte_stream_copy_to_uint16_big_endian(
				 &( base64_stream[ safe_base64_stream_index ] ),
				 base64_character );
				break;

			case LIBUNA_BASE64_VARIANT_ENCODING_UTF16_LITTLE_ENDIAN:
				byte_stream_copy_to_uint16_little_endian(
				 &( base64_stream[ safe_base64_stream_index ] ),
				 base64_character );
				break;

			case LIBUNA_BASE64_VARIANT_ENCODING_UTF32_BIG_ENDIAN:
				byte_stream_copy_to_uint32_big_endian(
				 &( base64_stream[ safe_base64_stream_index ] ),
				 base64_character );
				break;

			case LIBUNA_BASE64_VARIANT_ENCODING_UTF32_LITTLE_ENDIAN:
				byte_stream_copy_to_uint32_little_endian(
				 &( base64_stream[ safe_base64_stream_index ] ),
				 base64_character );
				break;
		}
		safe_base64_stream_index += base64_character_size;

		if( ( base64_character & 0xffffff00UL ) != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: invalid 3rd base64 character.",
			 function );

			return( -1 );
		}
		if( base64_character == (uint32_t) '=' )
		{
			if( ( base64_variant & 0x0f000000UL ) == LIBUNA_BASE64_VARIANT_PADDING_NONE )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
				 "%s: padding where not supposed to - invalid 3rd base64 character.",
				 function );

				return( -1 );
			}
		}
		else
		{
			result = libuna_base64_character_copy_to_sixtet(
			          base64_character,
			          &sixtet3,
			          base64_variant,
			          error );

			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_CONVERSION,
				 LIBCERROR_CONVERSION_ERROR_INPUT_FAILED,
				 "%s: unable to copy base64 character to 3rd sixtet.",
				 function );

				return( -1 );
			}
			else if( result == 0 )
			{
				if( ( base64_variant & 0x0f000000UL ) != LIBUNA_BASE64_VARIANT_PADDING_INVALID )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
					 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
					 "%s: invalid 3rd base64 sixtet.",
					 function );

					return( -1 );
				}
			}
			else
			{
				safe_padding_size -= 1;
			}
		}
	}
	if( ( base64_character_size > base64_stream_size )
	 || ( safe_base64_stream_index > ( base64_stream_size - base64_character_size ) ) )
	{
		if( ( base64_variant & 0x0f000000UL ) == LIBUNA_BASE64_VARIANT_PADDING_REQUIRED )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
			 "%s: base64 stream string too small - missing 4th base64 character.",
			 function );

			return( -1 );
		}
	}
	else
	{
		switch( base64_variant & 0xf0000000UL )
		{
			case LIBUNA_BASE64_VARIANT_ENCODING_BYTE_STREAM:
				base64_character = base64_stream[ safe_base64_stream_index ];
				break;

			case LIBUNA_BASE64_VARIANT_ENCODING_UTF16_BIG_ENDIAN:
				byte_stream_copy_to_uint16_big_endian(
				 &( base64_stream[ safe_base64_stream_index ] ),
				 base64_character );
				break;

			case LIBUNA_BASE64_VARIANT_ENCODING_UTF16_LITTLE_ENDIAN:
				byte_stream_copy_to_uint16_little_endian(
				 &( base64_stream[ safe_base64_stream_index ] ),
				 base64_character );
				break;

			case LIBUNA_BASE64_VARIANT_ENCODING_UTF32_BIG_ENDIAN:
				byte_stream_copy_to_uint32_big_endian(
				 &( base64_stream[ safe_base64_stream_index ] ),
				 base64_character );
				break;

			case LIBUNA_BASE64_VARIANT_ENCODING_UTF32_LITTLE_ENDIAN:
				byte_stream_copy_to_uint32_little_endian(
				 &( base64_stream[ safe_base64_stream_index ] ),
				 base64_character );
				break;
		}
		safe_base64_stream_index += base64_character_size;

		if( ( base64_character & 0xffffff00UL ) != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: invalid 4th base64 character.",
			 function );

			return( -1 );
		}
		if( base64_character == (uint32_t) '=' )
		{
			if( ( base64_variant & 0x0f000000UL ) == LIBUNA_BASE64_VARIANT_PADDING_NONE )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
				 "%s: padding where not supposed to - invalid 4th base64 character.",
				 function );

				return( -1 );
			}
		}
		else
		{
			result = libuna_base64_character_copy_to_sixtet(
			          base64_character,
			          &sixtet4,
			          base64_variant,
			          error );

			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_CONVERSION,
				 LIBCERROR_CONVERSION_ERROR_INPUT_FAILED,
				 "%s: unable to copy base64 character to 4th sixtet.",
				 function );

				return( -1 );
			}
			else if( result == 0 )
			{
				if( ( base64_variant & 0x0f000000UL ) != LIBUNA_BASE64_VARIANT_PADDING_INVALID )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
					 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
					 "%s: invalid 4th base64 sixtet.",
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
					 "%s: non-padding where not supposed to - invalid 4th base64 character.",
					 function );

					return( -1 );
				}
				safe_padding_size -= 1;
			}
		}
	}
	safe_base64_triplet   = sixtet1;
	safe_base64_triplet <<= 6;
	safe_base64_triplet  |= sixtet2;
	safe_base64_triplet <<= 6;
	safe_base64_triplet  |= sixtet3;
	safe_base64_triplet <<= 6;
	safe_base64_triplet  |= sixtet4;

	*base64_triplet      = safe_base64_triplet;
	*base64_stream_index = safe_base64_stream_index;
	*padding_size        = safe_padding_size;

	return( 1 );
}

/* Copies a base64 triplet to a base64 stream
 * Returns 1 if successful or -1 on error
 */
int libuna_base64_triplet_copy_to_base64_stream(
     uint32_t base64_triplet,
     uint8_t *base64_stream,
     size_t base64_stream_size,
     size_t *base64_stream_index,
     uint8_t padding_size,
     uint32_t base64_variant,
     libcerror_error_t **error )
{
	uint8_t *sixtet_to_character_table = NULL;
	static char *function              = "libuna_base64_triplet_copy_to_base64_stream";
	size_t base64_character_size       = 0;
	size_t safe_base64_stream_index    = 0;
	uint32_t base64_character          = 0;
	uint8_t padding_character          = 0;
	uint8_t sixtet1                    = 0;
	uint8_t sixtet2                    = 0;
	uint8_t sixtet3                    = 0;
	uint8_t sixtet4                    = 0;

	if( base64_stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid base64 stream.",
		 function );

		return( -1 );
	}
	if( base64_stream_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid base64 stream size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( base64_stream_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid base64 stream index.",
		 function );

		return( -1 );
	}
	if( *base64_stream_index >= base64_stream_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: base64 stream string too small.",
		 function );

		return( -1 );
	}
	if( padding_size > 2 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid padding size value out of bounds.",
		 function );

		return( -1 );
	}
	switch( base64_variant & 0x000f0000UL )
	{
		case LIBUNA_BASE64_VARIANT_ALPHABET_NORMAL:
			sixtet_to_character_table = libuna_base64_sixtet_to_character_table;
			break;

		case LIBUNA_BASE64_VARIANT_ALPHABET_URL:
			sixtet_to_character_table = libuna_base64url_sixtet_to_character_table;
			break;

		default:
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported base64 variant.",
			 function );

			return( -1 );
	}
	switch( base64_variant & 0x0f000000UL )
	{
		case LIBUNA_BASE64_VARIANT_PADDING_NONE:
		case LIBUNA_BASE64_VARIANT_PADDING_OPTIONAL:
		case LIBUNA_BASE64_VARIANT_PADDING_INVALID:
			padding_character = 0;
			break;


		case LIBUNA_BASE64_VARIANT_PADDING_REQUIRED:
			padding_character = (uint8_t) '=';
			break;

		default:
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported base64 variant.",
			 function );

			return( -1 );
	}
	switch( base64_variant & 0xf0000000UL )
	{
		case LIBUNA_BASE64_VARIANT_ENCODING_BYTE_STREAM:
			base64_character_size = 1;
			break;

		case LIBUNA_BASE64_VARIANT_ENCODING_UTF16_BIG_ENDIAN:
		case LIBUNA_BASE64_VARIANT_ENCODING_UTF16_LITTLE_ENDIAN:
			base64_character_size = 2;
			break;

		case LIBUNA_BASE64_VARIANT_ENCODING_UTF32_BIG_ENDIAN:
		case LIBUNA_BASE64_VARIANT_ENCODING_UTF32_LITTLE_ENDIAN:
			base64_character_size = 4;
			break;

		default:
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported base64 variant.",
			 function );

			return( -1 );
	}
	safe_base64_stream_index = *base64_stream_index;

	/* Separate the 3 bytes value into 4 x 6 bit values
	 */
	sixtet4          = (uint8_t) ( base64_triplet & 0x3f );
	base64_triplet >>= 6;
	sixtet3          = (uint8_t) ( base64_triplet & 0x3f );
	base64_triplet >>= 6;
	sixtet2          = (uint8_t) ( base64_triplet & 0x3f );
	base64_triplet >>= 6;
	sixtet1          = (uint8_t) ( base64_triplet & 0x3f );

	/* Spread the encoding over 2 characters if 1 byte is available
	 */
	if( ( safe_base64_stream_index + ( base64_character_size * 2 ) ) > base64_stream_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: base64 stream is too small.",
		 function );

		return( -1 );
	}
	base64_character = sixtet_to_character_table[ sixtet1 ];

	switch( base64_variant & 0xf0000000UL )
	{
		case LIBUNA_BASE64_VARIANT_ENCODING_BYTE_STREAM:
			base64_stream[ safe_base64_stream_index ] = (uint8_t) base64_character;
			break;

		case LIBUNA_BASE64_VARIANT_ENCODING_UTF16_BIG_ENDIAN:
			byte_stream_copy_from_uint16_big_endian(
			 &( base64_stream[ safe_base64_stream_index ] ),
			 base64_character );
			break;

		case LIBUNA_BASE64_VARIANT_ENCODING_UTF16_LITTLE_ENDIAN:
			byte_stream_copy_from_uint16_little_endian(
			 &( base64_stream[ safe_base64_stream_index ] ),
			 base64_character );
			break;

		case LIBUNA_BASE64_VARIANT_ENCODING_UTF32_BIG_ENDIAN:
			byte_stream_copy_from_uint32_big_endian(
			 &( base64_stream[ safe_base64_stream_index ] ),
			 base64_character );
			break;

		case LIBUNA_BASE64_VARIANT_ENCODING_UTF32_LITTLE_ENDIAN:
			byte_stream_copy_from_uint32_little_endian(
			 &( base64_stream[ safe_base64_stream_index ] ),
			 base64_character );
			break;
	}
	safe_base64_stream_index += base64_character_size;

	base64_character = sixtet_to_character_table[ sixtet2 ];

	switch( base64_variant & 0xf0000000UL )
	{
		case LIBUNA_BASE64_VARIANT_ENCODING_BYTE_STREAM:
			base64_stream[ safe_base64_stream_index ] = (uint8_t) base64_character;
			break;

		case LIBUNA_BASE64_VARIANT_ENCODING_UTF16_BIG_ENDIAN:
			byte_stream_copy_from_uint16_big_endian(
			 &( base64_stream[ safe_base64_stream_index ] ),
			 base64_character );
			break;

		case LIBUNA_BASE64_VARIANT_ENCODING_UTF16_LITTLE_ENDIAN:
			byte_stream_copy_from_uint16_little_endian(
			 &( base64_stream[ safe_base64_stream_index ] ),
			 base64_character );
			break;

		case LIBUNA_BASE64_VARIANT_ENCODING_UTF32_BIG_ENDIAN:
			byte_stream_copy_from_uint32_big_endian(
			 &( base64_stream[ safe_base64_stream_index ] ),
			 base64_character );
			break;

		case LIBUNA_BASE64_VARIANT_ENCODING_UTF32_LITTLE_ENDIAN:
			byte_stream_copy_from_uint32_little_endian(
			 &( base64_stream[ safe_base64_stream_index ] ),
			 base64_character );
			break;
	}
	safe_base64_stream_index += base64_character_size;

	/* Spread the encoding over 3 characters if 2 bytes are available
	 * Otherwise pad the remaining bytes if required
	 */
	if( ( safe_base64_stream_index + base64_character_size ) > base64_stream_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: base64 stream is too small.",
		 function );

		return( -1 );
	}
	if( ( padding_size < 2 )
	 || ( padding_character != 0 ) )
	{
		if( padding_size < 2 )
		{
			base64_character = sixtet_to_character_table[ sixtet3 ];
		}
		else if( padding_character != 0 )
		{
			base64_character = padding_character;
		}
		switch( base64_variant & 0xf0000000UL )
		{
			case LIBUNA_BASE64_VARIANT_ENCODING_BYTE_STREAM:
				base64_stream[ safe_base64_stream_index ] = (uint8_t) base64_character;
				break;

			case LIBUNA_BASE64_VARIANT_ENCODING_UTF16_BIG_ENDIAN:
				byte_stream_copy_from_uint16_big_endian(
				 &( base64_stream[ safe_base64_stream_index ] ),
				 base64_character );
				break;

			case LIBUNA_BASE64_VARIANT_ENCODING_UTF16_LITTLE_ENDIAN:
				byte_stream_copy_from_uint16_little_endian(
				 &( base64_stream[ safe_base64_stream_index ] ),
				 base64_character );
				break;

			case LIBUNA_BASE64_VARIANT_ENCODING_UTF32_BIG_ENDIAN:
				byte_stream_copy_from_uint32_big_endian(
				 &( base64_stream[ safe_base64_stream_index ] ),
				 base64_character );
				break;

			case LIBUNA_BASE64_VARIANT_ENCODING_UTF32_LITTLE_ENDIAN:
				byte_stream_copy_from_uint32_little_endian(
				 &( base64_stream[ safe_base64_stream_index ] ),
				 base64_character );
				break;
		}
		safe_base64_stream_index += base64_character_size;
	}
	/* Spread the encoding over 4 characters if 3 bytes are available
	 * Otherwise pad the remaining bytes if required
	 */
	if( ( safe_base64_stream_index + base64_character_size ) > base64_stream_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: base64 stream is too small.",
		 function );

		return( -1 );
	}
	if( ( padding_size < 1 )
	 || ( padding_character != 0 ) )
	{
		if( padding_size < 1 )
		{
			base64_character = sixtet_to_character_table[ sixtet4 ];
		}
		else if( padding_character != 0 )
		{
			base64_character = padding_character;
		}
		switch( base64_variant & 0xf0000000UL )
		{
			case LIBUNA_BASE64_VARIANT_ENCODING_BYTE_STREAM:
				base64_stream[ safe_base64_stream_index ] = (uint8_t) base64_character;
				break;

			case LIBUNA_BASE64_VARIANT_ENCODING_UTF16_BIG_ENDIAN:
				byte_stream_copy_from_uint16_big_endian(
				 &( base64_stream[ safe_base64_stream_index ] ),
				 base64_character );
				break;

			case LIBUNA_BASE64_VARIANT_ENCODING_UTF16_LITTLE_ENDIAN:
				byte_stream_copy_from_uint16_little_endian(
				 &( base64_stream[ safe_base64_stream_index ] ),
				 base64_character );
				break;

			case LIBUNA_BASE64_VARIANT_ENCODING_UTF32_BIG_ENDIAN:
				byte_stream_copy_from_uint32_big_endian(
				 &( base64_stream[ safe_base64_stream_index ] ),
				 base64_character );
				break;

			case LIBUNA_BASE64_VARIANT_ENCODING_UTF32_LITTLE_ENDIAN:
				byte_stream_copy_from_uint32_little_endian(
				 &( base64_stream[ safe_base64_stream_index ] ),
				 base64_character );
				break;
		}
		safe_base64_stream_index += base64_character_size;
	}
	*base64_stream_index = safe_base64_stream_index;

	return( 1 );
}

/* Copies a base64 triplet from a byte stream
 * Returns 1 if successful or -1 on error
 */
int libuna_base64_triplet_copy_from_byte_stream(
     uint32_t *base64_triplet,
     const uint8_t *byte_stream,
     size_t byte_stream_size,
     size_t *byte_stream_index,
     uint8_t *padding_size,
     libcerror_error_t **error )
{
	static char *function         = "libuna_base64_triplet_copy_from_byte_stream";
	size_t safe_byte_stream_index = 0;
	uint32_t safe_base64_triplet  = 0;
	uint8_t safe_padding_size     = 0;

	if( base64_triplet == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid base64 triplet.",
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
	safe_byte_stream_index = *byte_stream_index;

	/* Determine the value of 3 bytes (24 bits)
	 */
	safe_base64_triplet = byte_stream[ safe_byte_stream_index++ ];
	safe_padding_size   = 2;

	safe_base64_triplet <<= 8;

	if( safe_byte_stream_index < byte_stream_size )
	{
		safe_base64_triplet |= byte_stream[ safe_byte_stream_index++ ];
		safe_padding_size   -= 1;
	}
	safe_base64_triplet <<= 8;

	if( safe_byte_stream_index < byte_stream_size )
	{
		safe_base64_triplet |= byte_stream[ safe_byte_stream_index++ ];
		safe_padding_size   -= 1;
	}
	*base64_triplet    = safe_base64_triplet;
	*byte_stream_index = safe_byte_stream_index;
	*padding_size      = safe_padding_size;

	return( 1 );
}

/* Copies a base64 triplet to a byte stream
 * Returns 1 if successful or -1 on error
 */
int libuna_base64_triplet_copy_to_byte_stream(
     uint32_t base64_triplet,
     uint8_t *byte_stream,
     size_t byte_stream_size,
     size_t *byte_stream_index,
     uint8_t padding_size,
     libcerror_error_t **error )
{
	static char *function         = "libuna_base64_triplet_copy_to_byte_stream";
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
	if( padding_size > 2 )
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

	byte_stream[ safe_byte_stream_index++ ] = (uint8_t) ( ( base64_triplet >> 16 ) & 0xff );

	if( padding_size <= 1 )
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
		byte_stream[ safe_byte_stream_index++ ] = (uint8_t) ( ( base64_triplet >> 8 ) & 0xff );
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
		byte_stream[ safe_byte_stream_index++ ] = (uint8_t) ( base64_triplet & 0xff );
	}
	*byte_stream_index = safe_byte_stream_index;

	return( 1 );
}

/* Determines the size of a byte stream from a base64 stream
 *
 * LIBUNA_BASE64_FLAG_STRIP_WHITESPACE removes leading space and tab characters,
 * and trailing space, tab and end of line characters
 *
 * Returns 1 if successful or -1 on error
 */
int libuna_base64_stream_size_to_byte_stream(
     const uint8_t *base64_stream,
     size_t base64_stream_size,
     size_t *byte_stream_size,
     uint32_t base64_variant,
     uint8_t flags,
     libcerror_error_t **error )
{
	static char *function        = "libuna_base64_stream_size_to_byte_stream";
	size_t base64_character_size = 0;
	size_t base64_stream_index   = 0;
	size_t number_of_characters  = 0;
	size_t safe_byte_stream_size = 0;
	size_t whitespace_size       = 0;
	uint32_t base64_character1   = 0;
	uint32_t base64_character2   = 0;
	uint8_t base64_character_62  = 0;
	uint8_t base64_character_63  = 0;
	uint8_t character_limit      = 0;
	uint8_t padding_size         = 0;
	uint8_t strip_mode           = LIBUNA_STRIP_MODE_LEADING_WHITESPACE;

	if( base64_stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid base64 stream.",
		 function );

		return( -1 );
	}
	if( ( base64_stream_size == 0 )
	 || ( base64_stream_size > (size_t) SSIZE_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid base64 stream size value out of bounds.",
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
	switch( base64_variant & 0x000000ffUL )
	{
		case LIBUNA_BASE64_VARIANT_CHARACTER_LIMIT_NONE:
			character_limit = 0;

			break;

		case LIBUNA_BASE64_VARIANT_CHARACTER_LIMIT_64:
			character_limit = 64;

			break;

		case LIBUNA_BASE64_VARIANT_CHARACTER_LIMIT_76:
			character_limit = 76;

			break;

		default:
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported base64 variant.",
			 function );

			return( -1 );
	}
	switch( base64_variant & 0x000f0000UL )
	{
		case LIBUNA_BASE64_VARIANT_ALPHABET_NORMAL:
			base64_character_62 = (uint8_t) '+';
			base64_character_63 = (uint8_t) '/';
			break;

		case LIBUNA_BASE64_VARIANT_ALPHABET_URL:
			base64_character_62 = (uint8_t) '-';
			base64_character_63 = (uint8_t) '_';
			break;

		default:
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported base64 variant.",
			 function );

			return( -1 );
	}
	switch( base64_variant & 0xf0000000UL )
	{
		case LIBUNA_BASE64_VARIANT_ENCODING_BYTE_STREAM:
			base64_character_size = 1;
			break;

		case LIBUNA_BASE64_VARIANT_ENCODING_UTF16_BIG_ENDIAN:
		case LIBUNA_BASE64_VARIANT_ENCODING_UTF16_LITTLE_ENDIAN:
			base64_character_size = 2;
			break;

		case LIBUNA_BASE64_VARIANT_ENCODING_UTF32_BIG_ENDIAN:
		case LIBUNA_BASE64_VARIANT_ENCODING_UTF32_LITTLE_ENDIAN:
			base64_character_size = 4;
			break;

		default:
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported base64 variant.",
			 function );

			return( -1 );
	}
	if( base64_stream_size < base64_character_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: invalid base64 stream value too small.",
		 function );

		return( -1 );
	}
	if( ( flags & ~( LIBUNA_BASE64_FLAG_STRIP_WHITESPACE ) ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported flags.",
		 function );

		return( -1 );
	}
	base64_stream_index = base64_stream_size - base64_character_size;
	whitespace_size     = 0;

	while( base64_stream_index > base64_character_size )
	{
		switch( base64_variant & 0xf0000000UL )
		{
			case LIBUNA_BASE64_VARIANT_ENCODING_BYTE_STREAM:
				base64_character1 = base64_stream[ base64_stream_index ];
				break;

			case LIBUNA_BASE64_VARIANT_ENCODING_UTF16_BIG_ENDIAN:
				byte_stream_copy_to_uint16_big_endian(
				 &( base64_stream[ base64_stream_index ] ),
				 base64_character1 );
				break;

			case LIBUNA_BASE64_VARIANT_ENCODING_UTF16_LITTLE_ENDIAN:
				byte_stream_copy_to_uint16_little_endian(
				 &( base64_stream[ base64_stream_index ] ),
				 base64_character1 );
				break;

			case LIBUNA_BASE64_VARIANT_ENCODING_UTF32_BIG_ENDIAN:
				byte_stream_copy_to_uint32_big_endian(
				 &( base64_stream[ base64_stream_index ] ),
				 base64_character1 );
				break;

			case LIBUNA_BASE64_VARIANT_ENCODING_UTF32_LITTLE_ENDIAN:
				byte_stream_copy_to_uint32_little_endian(
				 &( base64_stream[ base64_stream_index ] ),
				 base64_character1 );
				break;
		}
		base64_stream_index -= base64_character_size;

		if( ( base64_character1 == (uint32_t) '\n' )
		 || ( base64_character1 == (uint32_t) '\r' ) )
		{
			whitespace_size += base64_character_size;
		}
		else if( ( flags & LIBUNA_BASE64_FLAG_STRIP_WHITESPACE ) == 0 )
		{
			break;
		}
		else if( ( base64_character1 == (uint32_t) ' ' )
		      || ( base64_character1 == (uint32_t) '\t' )
		      || ( base64_character1 == (uint32_t) '\v' ) )
		{
			whitespace_size += base64_character_size;
		}
		else
		{
			break;
		}
	}
	base64_stream_size -= whitespace_size;
	base64_stream_index = base64_stream_size - base64_character_size;

	if( base64_stream_size < base64_character_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: invalid base64 stream value too small.",
		 function );

		return( -1 );
	}
	if( base64_stream_index > base64_character_size )
	{
		switch( base64_variant & 0xf0000000UL )
		{
			case LIBUNA_BASE64_VARIANT_ENCODING_BYTE_STREAM:
				base64_character1 = base64_stream[ base64_stream_index ];
				break;

			case LIBUNA_BASE64_VARIANT_ENCODING_UTF16_BIG_ENDIAN:
				byte_stream_copy_to_uint16_big_endian(
				 &( base64_stream[ base64_stream_index ] ),
				 base64_character1 );
				break;

			case LIBUNA_BASE64_VARIANT_ENCODING_UTF16_LITTLE_ENDIAN:
				byte_stream_copy_to_uint16_little_endian(
				 &( base64_stream[ base64_stream_index ] ),
				 base64_character1 );
				break;

			case LIBUNA_BASE64_VARIANT_ENCODING_UTF32_BIG_ENDIAN:
				byte_stream_copy_to_uint32_big_endian(
				 &( base64_stream[ base64_stream_index ] ),
				 base64_character1 );
				break;

			case LIBUNA_BASE64_VARIANT_ENCODING_UTF32_LITTLE_ENDIAN:
				byte_stream_copy_to_uint32_little_endian(
				 &( base64_stream[ base64_stream_index ] ),
				 base64_character1 );
				break;
		}
		base64_stream_index -= base64_character_size;

		if( base64_character1 == (uint32_t) '=' )
		{
			padding_size += 1;
		}
	}
	if( base64_stream_index > base64_character_size )
	{
		switch( base64_variant & 0xf0000000UL )
		{
			case LIBUNA_BASE64_VARIANT_ENCODING_BYTE_STREAM:
				base64_character1 = base64_stream[ base64_stream_index ];
				break;

			case LIBUNA_BASE64_VARIANT_ENCODING_UTF16_BIG_ENDIAN:
				byte_stream_copy_to_uint16_big_endian(
				 &( base64_stream[ base64_stream_index ] ),
				 base64_character1 );
				break;

			case LIBUNA_BASE64_VARIANT_ENCODING_UTF16_LITTLE_ENDIAN:
				byte_stream_copy_to_uint16_little_endian(
				 &( base64_stream[ base64_stream_index ] ),
				 base64_character1 );
				break;

			case LIBUNA_BASE64_VARIANT_ENCODING_UTF32_BIG_ENDIAN:
				byte_stream_copy_to_uint32_big_endian(
				 &( base64_stream[ base64_stream_index ] ),
				 base64_character1 );
				break;

			case LIBUNA_BASE64_VARIANT_ENCODING_UTF32_LITTLE_ENDIAN:
				byte_stream_copy_to_uint32_little_endian(
				 &( base64_stream[ base64_stream_index ] ),
				 base64_character1 );
				break;
		}
		base64_stream_index -= base64_character_size;

		if( base64_character1 == (uint32_t) '=' )
		{
			padding_size += 1;
		}
	}
	if( base64_stream_index > base64_character_size )
	{
		switch( base64_variant & 0xf0000000UL )
		{
			case LIBUNA_BASE64_VARIANT_ENCODING_BYTE_STREAM:
				base64_character1 = base64_stream[ base64_stream_index ];
				break;

			case LIBUNA_BASE64_VARIANT_ENCODING_UTF16_BIG_ENDIAN:
				byte_stream_copy_to_uint16_big_endian(
				 &( base64_stream[ base64_stream_index ] ),
				 base64_character1 );
				break;

			case LIBUNA_BASE64_VARIANT_ENCODING_UTF16_LITTLE_ENDIAN:
				byte_stream_copy_to_uint16_little_endian(
				 &( base64_stream[ base64_stream_index ] ),
				 base64_character1 );
				break;

			case LIBUNA_BASE64_VARIANT_ENCODING_UTF32_BIG_ENDIAN:
				byte_stream_copy_to_uint32_big_endian(
				 &( base64_stream[ base64_stream_index ] ),
				 base64_character1 );
				break;

			case LIBUNA_BASE64_VARIANT_ENCODING_UTF32_LITTLE_ENDIAN:
				byte_stream_copy_to_uint32_little_endian(
				 &( base64_stream[ base64_stream_index ] ),
				 base64_character1 );
				break;
		}
		if( base64_character1 == (uint32_t) '=' )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_CONVERSION,
			 LIBCERROR_CONVERSION_ERROR_INPUT_FAILED,
			 "%s: invalid number of padding bytes.",
			 function );

			return( -1 );
		}
	}
	base64_stream_index = 0;
	whitespace_size     = 0;

	while( base64_stream_index < ( base64_stream_size - ( padding_size * base64_character_size ) ) )
	{
		switch( base64_variant & 0xf0000000UL )
		{
			case LIBUNA_BASE64_VARIANT_ENCODING_BYTE_STREAM:
				base64_character1 = base64_stream[ base64_stream_index ];
				break;

			case LIBUNA_BASE64_VARIANT_ENCODING_UTF16_BIG_ENDIAN:
				byte_stream_copy_to_uint16_big_endian(
				 &( base64_stream[ base64_stream_index ] ),
				 base64_character1 );
				break;

			case LIBUNA_BASE64_VARIANT_ENCODING_UTF16_LITTLE_ENDIAN:
				byte_stream_copy_to_uint16_little_endian(
				 &( base64_stream[ base64_stream_index ] ),
				 base64_character1 );
				break;

			case LIBUNA_BASE64_VARIANT_ENCODING_UTF32_BIG_ENDIAN:
				byte_stream_copy_to_uint32_big_endian(
				 &( base64_stream[ base64_stream_index ] ),
				 base64_character1 );
				break;

			case LIBUNA_BASE64_VARIANT_ENCODING_UTF32_LITTLE_ENDIAN:
				byte_stream_copy_to_uint32_little_endian(
				 &( base64_stream[ base64_stream_index ] ),
				 base64_character1 );
				break;
		}
		base64_stream_index += base64_character_size;

		if( ( base64_character1 == (uint32_t) '\n' )
		 || ( base64_character1 == (uint32_t) '\r' ) )
		{
			if( ( strip_mode != LIBUNA_STRIP_MODE_NON_WHITESPACE )
			 && ( strip_mode != LIBUNA_STRIP_MODE_TRAILING_WHITESPACE ) )
			{
				strip_mode = LIBUNA_STRIP_MODE_INVALID_CHARACTER;
			}
			else
			{
				if( ( base64_stream_index + base64_character_size ) < base64_stream_size )
				{
					switch( base64_variant & 0xf0000000UL )
					{
						case LIBUNA_BASE64_VARIANT_ENCODING_BYTE_STREAM:
							base64_character2 = base64_stream[ base64_stream_index ];
							break;

						case LIBUNA_BASE64_VARIANT_ENCODING_UTF16_BIG_ENDIAN:
							byte_stream_copy_to_uint16_big_endian(
							 &( base64_stream[ base64_stream_index ] ),
							 base64_character2 );
							break;

						case LIBUNA_BASE64_VARIANT_ENCODING_UTF16_LITTLE_ENDIAN:
							byte_stream_copy_to_uint16_little_endian(
							 &( base64_stream[ base64_stream_index ] ),
							 base64_character2 );
							break;

						case LIBUNA_BASE64_VARIANT_ENCODING_UTF32_BIG_ENDIAN:
							byte_stream_copy_to_uint32_big_endian(
							 &( base64_stream[ base64_stream_index ] ),
							 base64_character2 );
							break;

						case LIBUNA_BASE64_VARIANT_ENCODING_UTF32_LITTLE_ENDIAN:
							byte_stream_copy_to_uint32_little_endian(
							 &( base64_stream[ base64_stream_index ] ),
							 base64_character2 );
							break;
					}
					if( ( base64_character2 == (uint32_t) '\n' )
					 || ( base64_character2 == (uint32_t) '\r' ) )
					{
						base64_stream_index += base64_character_size;

						whitespace_size += base64_character_size;
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
					 "%s: number of characters in line does not match character limit.",
					 function );

					return( -1 );
				}
				number_of_characters = 0;
			}
			whitespace_size += base64_character_size;
		}
		else if( ( base64_character1 == (uint32_t) ' ' )
		      || ( base64_character1 == (uint32_t) '\t' )
		      || ( base64_character1 == (uint32_t) '\v' ) )
		{
			if( ( flags & LIBUNA_BASE64_FLAG_STRIP_WHITESPACE ) != 0 )
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
					whitespace_size += base64_character_size;
				}
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
		if( strip_mode == LIBUNA_STRIP_MODE_NON_WHITESPACE )
		{
			/* A-Z is not a continous range on an EBCDIC based system
			 * it consists of the ranges: A-I, J-R, S-Z
			 */
			if( ( base64_character1 >= (uint32_t) 'A' )
			 && ( base64_character1 <= (uint32_t) 'I' ) )
			{
				number_of_characters++;
			}
			else if( ( base64_character1 >= (uint32_t) 'J' )
			      && ( base64_character1 <= (uint32_t) 'R' ) )
			{
				number_of_characters++;
			}
			else if( ( base64_character1 >= (uint32_t) 'S' )
			      && ( base64_character1 <= (uint32_t) 'Z' ) )
			{
				number_of_characters++;
			}
			/* a-z is not a continous range on an EBCDIC based system
			 * it consists of the ranges: a-i, j-r, s-z
			 */
			else if( ( base64_character1 >= (uint32_t) 'a' )
			      && ( base64_character1 <= (uint32_t) 'i' ) )
			{
				number_of_characters++;
			}
			else if( ( base64_character1 >= (uint32_t) 'j' )
			      && ( base64_character1 <= (uint32_t) 'r' ) )
			{
				number_of_characters++;
			}
			else if( ( base64_character1 >= (uint32_t) 's' )
			      && ( base64_character1 <= (uint32_t) 'z' ) )
			{
				number_of_characters++;
			}
			else if( ( base64_character1 >= (uint32_t) '0' )
			      && ( base64_character1 <= (uint32_t) '9' ) )
			{
				number_of_characters++;
			}
			else if( ( base64_character1 == (uint32_t) base64_character_62 )
			      || ( base64_character1 == (uint32_t) base64_character_63 ) )
			{
				number_of_characters++;
			}
			else
			{
				strip_mode = LIBUNA_STRIP_MODE_INVALID_CHARACTER;
			}
		}
		if( strip_mode == LIBUNA_STRIP_MODE_INVALID_CHARACTER )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_CONVERSION,
			 LIBCERROR_CONVERSION_ERROR_INPUT_FAILED,
			 "%s: invalid character in base64 stream at index: %" PRIzd ".",
			 function,
			 base64_stream_index - base64_character_size );

			return( -1 );
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
	base64_stream_size -= whitespace_size;

	/* Make sure the byte stream is able to hold
	 * at least 3 bytes for each 4 base64 characters
	 */
	base64_character_size *= 4;

	safe_byte_stream_size = base64_stream_size / base64_character_size;

	if( ( base64_stream_size % base64_character_size ) != 0 )
	{
		safe_byte_stream_size += 1;
	}
	safe_byte_stream_size *= 3;

	/* The padding size contains the number of bytes to correct
	 */
	*byte_stream_size = safe_byte_stream_size - padding_size;

	return( 1 );
}

/* Copies a byte stream from a base64 stream
 *
 * LIBUNA_BASE64_FLAG_STRIP_WHITESPACE removes leading space and tab characters,
 * and trailing space, tab and end of line characters
 *
 * Returns 1 if successful or -1 on error
 */
int libuna_base64_stream_copy_to_byte_stream(
     const uint8_t *base64_stream,
     size_t base64_stream_size,
     uint8_t *byte_stream,
     size_t byte_stream_size,
     uint32_t base64_variant,
     uint8_t flags,
     libcerror_error_t **error )
{
	static char *function        = "libuna_base64_stream_copy_to_byte_stream";
	size_t base64_character_size = 0;
	size_t base64_stream_index   = 0;
	size_t byte_stream_index     = 0;
	size_t number_of_characters  = 0;
	size_t whitespace_size       = 0;
	uint32_t base64_character1   = 0;
	uint32_t base64_character2   = 0;
	uint32_t base64_triplet      = 0;
	uint8_t character_limit      = 0;
	uint8_t padding_size         = 0;
	uint8_t strip_mode           = LIBUNA_STRIP_MODE_LEADING_WHITESPACE;

	if( base64_stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid base64 stream.",
		 function );

		return( -1 );
	}
	if( base64_stream_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid base64 stream size value exceeds maximum.",
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
	switch( base64_variant & 0x000000ffUL )
	{
		case LIBUNA_BASE64_VARIANT_CHARACTER_LIMIT_NONE:
			character_limit = 0;

			break;

		case LIBUNA_BASE64_VARIANT_CHARACTER_LIMIT_64:
			character_limit = 64;

			break;

		case LIBUNA_BASE64_VARIANT_CHARACTER_LIMIT_76:
			character_limit = 76;

			break;

		default:
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported base64 variant.",
			 function );

			return( -1 );
	}
	switch( base64_variant & 0xf0000000UL )
	{
		case LIBUNA_BASE64_VARIANT_ENCODING_BYTE_STREAM:
			base64_character_size = 1;
			break;

		case LIBUNA_BASE64_VARIANT_ENCODING_UTF16_BIG_ENDIAN:
		case LIBUNA_BASE64_VARIANT_ENCODING_UTF16_LITTLE_ENDIAN:
			base64_character_size = 2;
			break;

		case LIBUNA_BASE64_VARIANT_ENCODING_UTF32_BIG_ENDIAN:
		case LIBUNA_BASE64_VARIANT_ENCODING_UTF32_LITTLE_ENDIAN:
			base64_character_size = 4;
			break;

		default:
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported base64 variant.",
			 function );

			return( -1 );
	}
	if( base64_stream_size < base64_character_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: invalid base64 stream value too small.",
		 function );

		return( -1 );
	}
	if( ( flags & ~( LIBUNA_BASE64_FLAG_STRIP_WHITESPACE ) ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported flags.",
		 function );

		return( -1 );
	}
	base64_stream_index = base64_stream_size - base64_character_size;
	whitespace_size     = 0;

	while( base64_stream_index > base64_character_size )
	{
		switch( base64_variant & 0xf0000000UL )
		{
			case LIBUNA_BASE64_VARIANT_ENCODING_BYTE_STREAM:
				base64_character1 = base64_stream[ base64_stream_index ];
				break;

			case LIBUNA_BASE64_VARIANT_ENCODING_UTF16_BIG_ENDIAN:
				byte_stream_copy_to_uint16_big_endian(
				 &( base64_stream[ base64_stream_index ] ),
				 base64_character1 );
				break;

			case LIBUNA_BASE64_VARIANT_ENCODING_UTF16_LITTLE_ENDIAN:
				byte_stream_copy_to_uint16_little_endian(
				 &( base64_stream[ base64_stream_index ] ),
				 base64_character1 );
				break;

			case LIBUNA_BASE64_VARIANT_ENCODING_UTF32_BIG_ENDIAN:
				byte_stream_copy_to_uint32_big_endian(
				 &( base64_stream[ base64_stream_index ] ),
				 base64_character1 );
				break;

			case LIBUNA_BASE64_VARIANT_ENCODING_UTF32_LITTLE_ENDIAN:
				byte_stream_copy_to_uint32_little_endian(
				 &( base64_stream[ base64_stream_index ] ),
				 base64_character1 );
				break;
		}
		base64_stream_index -= base64_character_size;

		if( ( base64_character1 == (uint32_t) '\n' )
		 || ( base64_character1 == (uint32_t) '\r' ) )
		{
			whitespace_size += base64_character_size;
		}
		else if( ( flags & LIBUNA_BASE64_FLAG_STRIP_WHITESPACE ) == 0 )
		{
			break;
		}
		else if( ( base64_character1 == (uint32_t) ' ' )
		      || ( base64_character1 == (uint32_t) '\t' )
		      || ( base64_character1 == (uint32_t) '\v' ) )
		{
			whitespace_size += base64_character_size;
		}
		else
		{
			break;
		}
	}
	base64_stream_size -= whitespace_size;
	base64_stream_index = 0;

	if( ( flags & LIBUNA_BASE64_FLAG_STRIP_WHITESPACE ) == 0 )
	{
		strip_mode = LIBUNA_STRIP_MODE_NON_WHITESPACE;
	}
	while( base64_stream_index < base64_stream_size )
	{
		switch( base64_variant & 0xf0000000UL )
		{
			case LIBUNA_BASE64_VARIANT_ENCODING_BYTE_STREAM:
				base64_character1 = base64_stream[ base64_stream_index ];
				break;

			case LIBUNA_BASE64_VARIANT_ENCODING_UTF16_BIG_ENDIAN:
				byte_stream_copy_to_uint16_big_endian(
				 &( base64_stream[ base64_stream_index ] ),
				 base64_character1 );
				break;

			case LIBUNA_BASE64_VARIANT_ENCODING_UTF16_LITTLE_ENDIAN:
				byte_stream_copy_to_uint16_little_endian(
				 &( base64_stream[ base64_stream_index ] ),
				 base64_character1 );
				break;

			case LIBUNA_BASE64_VARIANT_ENCODING_UTF32_BIG_ENDIAN:
				byte_stream_copy_to_uint32_big_endian(
				 &( base64_stream[ base64_stream_index ] ),
				 base64_character1 );
				break;

			case LIBUNA_BASE64_VARIANT_ENCODING_UTF32_LITTLE_ENDIAN:
				byte_stream_copy_to_uint32_little_endian(
				 &( base64_stream[ base64_stream_index ] ),
				 base64_character1 );
				break;
		}
		base64_stream_index += base64_character_size;

		if( ( base64_character1 == (uint32_t) '\n' )
		 || ( base64_character1 == (uint32_t) '\r' ) )
		{
			if( ( strip_mode != LIBUNA_STRIP_MODE_NON_WHITESPACE )
			 && ( strip_mode != LIBUNA_STRIP_MODE_TRAILING_WHITESPACE ) )
			{
				strip_mode = LIBUNA_STRIP_MODE_INVALID_CHARACTER;
			}
			else
			{
				if( ( base64_stream_index + base64_character_size ) < base64_stream_size )
				{
					switch( base64_variant & 0xf0000000UL )
					{
						case LIBUNA_BASE64_VARIANT_ENCODING_BYTE_STREAM:
							base64_character2 = base64_stream[ base64_stream_index ];
							break;

						case LIBUNA_BASE64_VARIANT_ENCODING_UTF16_BIG_ENDIAN:
							byte_stream_copy_to_uint16_big_endian(
							 &( base64_stream[ base64_stream_index ] ),
							 base64_character2 );
							break;

						case LIBUNA_BASE64_VARIANT_ENCODING_UTF16_LITTLE_ENDIAN:
							byte_stream_copy_to_uint16_little_endian(
							 &( base64_stream[ base64_stream_index ] ),
							 base64_character2 );
							break;

						case LIBUNA_BASE64_VARIANT_ENCODING_UTF32_BIG_ENDIAN:
							byte_stream_copy_to_uint32_big_endian(
							 &( base64_stream[ base64_stream_index ] ),
							 base64_character2 );
							break;

						case LIBUNA_BASE64_VARIANT_ENCODING_UTF32_LITTLE_ENDIAN:
							byte_stream_copy_to_uint32_little_endian(
							 &( base64_stream[ base64_stream_index ] ),
							 base64_character2 );
							break;
					}
					if( ( base64_character2 == (uint32_t) '\n' )
					 || ( base64_character2 == (uint32_t) '\r' ) )
					{
						base64_stream_index += base64_character_size;
					}
				}
				strip_mode = LIBUNA_STRIP_MODE_LEADING_WHITESPACE;
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
		else if( ( base64_character1 == (uint32_t) ' ' )
		      || ( base64_character1 == (uint32_t) '\t' )
		      || ( base64_character1 == (uint32_t) '\v' ) )
		{
			if( ( flags & LIBUNA_BASE64_FLAG_STRIP_WHITESPACE ) != 0 )
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
			 "%s: invalid character in base64 stream at index: %" PRIzd ".",
			 function,
			 base64_stream_index - base64_character_size );

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
				 "%s: invalid 1st base64 sixtet.",
				 function );

				return( -1 );
			}
			base64_stream_index -= base64_character_size;

			/* Convert the base64 stream into a base64 triplet
			 */
			if( libuna_base64_triplet_copy_from_base64_stream(
			     &base64_triplet,
			     base64_stream,
			     base64_stream_size,
			     &base64_stream_index,
			     &padding_size,
			     base64_variant,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_CONVERSION,
				 LIBCERROR_CONVERSION_ERROR_INPUT_FAILED,
				 "%s: unable to copy base64 triplet from base64 stream.",
				 function );

				return( -1 );
			}
			/* Convert the base64 triplet into a byte stream
			 */
			if( libuna_base64_triplet_copy_to_byte_stream(
			     base64_triplet,
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
				 "%s: unable to copy base64 triplet to byte stream.",
				 function );

				return( -1 );
			}
			number_of_characters += 4 - padding_size;
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

/* Determines the size of a base64 stream from a byte stream
 * Returns 1 if successful or -1 on error
 */
int libuna_base64_stream_size_from_byte_stream(
     const uint8_t *byte_stream,
     size_t byte_stream_size,
     size_t *base64_stream_size,
     uint32_t base64_variant,
     libcerror_error_t **error )
{
	static char *function                = "libuna_base64_stream_size_from_byte_stream";
	size_t base64_character_size         = 0;
	size_t calculated_base64_stream_size = 0;
	size_t remaining_size                = 0;
	size_t whitespace_size               = 0;
	uint8_t character_limit              = 0;
	uint8_t padding_character            = 0;

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
	if( base64_stream_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid base64 stream size.",
		 function );

		return( -1 );
	}
	switch( base64_variant & 0x000000ffUL )
	{
		case LIBUNA_BASE64_VARIANT_CHARACTER_LIMIT_NONE:
			character_limit = 0;

			break;

		case LIBUNA_BASE64_VARIANT_CHARACTER_LIMIT_64:
			character_limit = 64;

			break;

		case LIBUNA_BASE64_VARIANT_CHARACTER_LIMIT_76:
			character_limit = 76;

			break;

		default:
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported base64 variant.",
			 function );

			return( -1 );
	}
	switch( base64_variant & 0x0f000000UL )
	{
		case LIBUNA_BASE64_VARIANT_PADDING_NONE:
			padding_character = 0;
			break;

		case LIBUNA_BASE64_VARIANT_PADDING_OPTIONAL:
			padding_character = (uint8_t) '=';
			break;

		case LIBUNA_BASE64_VARIANT_PADDING_REQUIRED:
			padding_character = (uint8_t) '=';
			break;

		default:
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported base64 variant.",
			 function );

			return( -1 );
	}
	switch( base64_variant & 0xf0000000UL )
	{
		case LIBUNA_BASE64_VARIANT_ENCODING_BYTE_STREAM:
			base64_character_size = 1;
			break;

		case LIBUNA_BASE64_VARIANT_ENCODING_UTF16_BIG_ENDIAN:
		case LIBUNA_BASE64_VARIANT_ENCODING_UTF16_LITTLE_ENDIAN:
			base64_character_size = 2;
			break;

		case LIBUNA_BASE64_VARIANT_ENCODING_UTF32_BIG_ENDIAN:
		case LIBUNA_BASE64_VARIANT_ENCODING_UTF32_LITTLE_ENDIAN:
			base64_character_size = 4;
			break;

		default:
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported base64 variant.",
			 function );

			return( -1 );
	}
	/* Make sure the base64 stream is able to hold
	 * at least 4 base64 characters for each 3 bytes
	 */
	calculated_base64_stream_size = ( byte_stream_size / 3 ) * 4;

	remaining_size = byte_stream_size % 3;

	if( remaining_size != 0 )
	{
		if( padding_character != 0 )
		{
			calculated_base64_stream_size += 4;
		}
		else if( remaining_size == 1 )
		{
			calculated_base64_stream_size += 2;
		}
		else if( remaining_size == 2 )
		{
			calculated_base64_stream_size += 3;
		}
	}
	if( character_limit != 0 )
	{
		if( calculated_base64_stream_size > character_limit )
		{
			whitespace_size = calculated_base64_stream_size / character_limit;

			if( ( calculated_base64_stream_size % character_limit ) != 0 )
			{
				whitespace_size += 1;
			}
			calculated_base64_stream_size += whitespace_size;
		}
		calculated_base64_stream_size += 1;
	}
	calculated_base64_stream_size *= base64_character_size;

	*base64_stream_size = calculated_base64_stream_size;

	return( 1 );
}

/* Copies a base64 stream from a byte stream
 * Returns 1 if successful or -1 on error
 */
int libuna_base64_stream_copy_from_byte_stream(
     uint8_t *base64_stream,
     size_t base64_stream_size,
     const uint8_t *byte_stream,
     size_t byte_stream_size,
     uint32_t base64_variant,
     libcerror_error_t **error )
{
	static char *function      = "libuna_base64_stream_copy_from_byte_stream";
	size_t base64_stream_index = 0;

	if( libuna_base64_stream_with_index_copy_from_byte_stream(
	     base64_stream,
	     base64_stream_size,
	     &base64_stream_index,
	     byte_stream,
	     byte_stream_size,
	     base64_variant,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy base64 stream from byte stream.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Copies a base64 stream from a byte stream
 * Returns 1 if successful or -1 on error
 */
int libuna_base64_stream_with_index_copy_from_byte_stream(
     uint8_t *base64_stream,
     size_t base64_stream_size,
     size_t *base64_stream_index,
     const uint8_t *byte_stream,
     size_t byte_stream_size,
     uint32_t base64_variant,
     libcerror_error_t **error )
{
	static char *function                = "libuna_base64_stream_with_index_copy_from_byte_stream";
	size_t base64_character_size         = 0;
	size_t calculated_base64_stream_size = 0;
	size_t byte_stream_index             = 0;
	size_t number_of_characters          = 0;
	size_t remaining_size                = 0;
	size_t safe_base64_stream_index      = 0;
	size_t whitespace_size               = 0;
	uint32_t base64_character            = 0;
	uint32_t base64_triplet              = 0;
	uint8_t character_limit              = 0;
	uint8_t padding_character            = 0;
	uint8_t padding_size                 = 0;

	if( base64_stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid base64 stream.",
		 function );

		return( -1 );
	}
	if( base64_stream_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid base64 stream size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( base64_stream_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid base64 stream index.",
		 function );

		return( -1 );
	}
	if( *base64_stream_index >= base64_stream_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: base64 stream string too small.",
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
	switch( base64_variant & 0x000000ffUL )
	{
		case LIBUNA_BASE64_VARIANT_CHARACTER_LIMIT_NONE:
			character_limit = 0;

			break;

		case LIBUNA_BASE64_VARIANT_CHARACTER_LIMIT_64:
			character_limit = 64;

			break;

		case LIBUNA_BASE64_VARIANT_CHARACTER_LIMIT_76:
			character_limit = 76;

			break;

		default:
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported base64 variant.",
			 function );

			return( -1 );
	}
	switch( base64_variant & 0x0f000000UL )
	{
		case LIBUNA_BASE64_VARIANT_PADDING_NONE:
			padding_character = 0;
			break;

		case LIBUNA_BASE64_VARIANT_PADDING_OPTIONAL:
			padding_character = (uint8_t) '=';
			break;

		case LIBUNA_BASE64_VARIANT_PADDING_REQUIRED:
			padding_character = (uint8_t) '=';
			break;

		default:
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported base64 variant.",
			 function );

			return( -1 );
	}
	switch( base64_variant & 0xf0000000UL )
	{
		case LIBUNA_BASE64_VARIANT_ENCODING_BYTE_STREAM:
			base64_character_size = 1;
			break;

		case LIBUNA_BASE64_VARIANT_ENCODING_UTF16_BIG_ENDIAN:
		case LIBUNA_BASE64_VARIANT_ENCODING_UTF16_LITTLE_ENDIAN:
			base64_character_size = 2;
			break;

		case LIBUNA_BASE64_VARIANT_ENCODING_UTF32_BIG_ENDIAN:
		case LIBUNA_BASE64_VARIANT_ENCODING_UTF32_LITTLE_ENDIAN:
			base64_character_size = 4;
			break;

		default:
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported base64 variant.",
			 function );

			return( -1 );
	}
	safe_base64_stream_index = *base64_stream_index;

	/* Make sure the base64 stream is able to hold
	 * at least 4 base64 characters for each 3 bytes
	 */
	calculated_base64_stream_size = ( byte_stream_size / 3 ) * 4;

	remaining_size = byte_stream_size % 3;

	if( remaining_size != 0 )
	{
		if( padding_character != 0 )
		{
			calculated_base64_stream_size += 4;
		}
		else if( remaining_size == 1 )
		{
			calculated_base64_stream_size += 2;
		}
		else if( remaining_size == 2 )
		{
			calculated_base64_stream_size += 3;
		}
	}
	if( character_limit != 0 )
	{
		if( calculated_base64_stream_size > character_limit )
		{
			whitespace_size = calculated_base64_stream_size / character_limit;

			if( ( calculated_base64_stream_size % character_limit ) != 0 )
			{
				whitespace_size += 1;
			}
			calculated_base64_stream_size += whitespace_size;
		}
		calculated_base64_stream_size += 1;
	}
	calculated_base64_stream_size *= base64_character_size;

	if( base64_stream_size < calculated_base64_stream_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: base64 stream is too small.",
		 function );

		return( -1 );
	}
	while( byte_stream_index < byte_stream_size )
	{
		/* Convert the byte stream into a base64 triplet
		 */
		if( libuna_base64_triplet_copy_from_byte_stream(
		     &base64_triplet,
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
			 "%s: unable to copy base64 triplet from byte stream.",
			 function );

			return( -1 );
		}
		/* Convert the base64 triplet into a base64 stream
		 */
		if( libuna_base64_triplet_copy_to_base64_stream(
		     base64_triplet,
		     base64_stream,
		     base64_stream_size,
		     &safe_base64_stream_index,
		     padding_size,
		     base64_variant,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_CONVERSION,
			 LIBCERROR_CONVERSION_ERROR_INPUT_FAILED,
			 "%s: unable to copy base64 triplet to base64 stream.",
			 function );

			return( -1 );
		}
		if( character_limit != 0 )
		{
			number_of_characters += 4;

			if( number_of_characters >= (size_t) character_limit )
			{
				base64_character = (uint32_t) '\n';

				switch( base64_variant & 0xf0000000UL )
				{
					case LIBUNA_BASE64_VARIANT_ENCODING_BYTE_STREAM:
						base64_stream[ safe_base64_stream_index ] = (uint8_t) base64_character;
						break;

					case LIBUNA_BASE64_VARIANT_ENCODING_UTF16_BIG_ENDIAN:
						byte_stream_copy_from_uint16_big_endian(
						 &( base64_stream[ safe_base64_stream_index ] ),
						 base64_character );
						break;

					case LIBUNA_BASE64_VARIANT_ENCODING_UTF16_LITTLE_ENDIAN:
						byte_stream_copy_from_uint16_little_endian(
						 &( base64_stream[ safe_base64_stream_index ] ),
						 base64_character );
						break;

					case LIBUNA_BASE64_VARIANT_ENCODING_UTF32_BIG_ENDIAN:
						byte_stream_copy_from_uint32_big_endian(
						 &( base64_stream[ safe_base64_stream_index ] ),
						 base64_character );
						break;

					case LIBUNA_BASE64_VARIANT_ENCODING_UTF32_LITTLE_ENDIAN:
						byte_stream_copy_from_uint32_little_endian(
						 &( base64_stream[ safe_base64_stream_index ] ),
						 base64_character );
						break;
				}
				safe_base64_stream_index += base64_character_size;

				number_of_characters = 0;
			}
		}
	}
	if( character_limit != 0 )
	{
		if( number_of_characters != 0 )
		{
			base64_character = (uint32_t) '\n';

			switch( base64_variant & 0xf0000000UL )
			{
				case LIBUNA_BASE64_VARIANT_ENCODING_BYTE_STREAM:
					base64_stream[ safe_base64_stream_index ] = (uint8_t) base64_character;
					break;

				case LIBUNA_BASE64_VARIANT_ENCODING_UTF16_BIG_ENDIAN:
					byte_stream_copy_from_uint16_big_endian(
					 &( base64_stream[ safe_base64_stream_index ] ),
					 base64_character );
					break;

				case LIBUNA_BASE64_VARIANT_ENCODING_UTF16_LITTLE_ENDIAN:
					byte_stream_copy_from_uint16_little_endian(
					 &( base64_stream[ safe_base64_stream_index ] ),
					 base64_character );
					break;

				case LIBUNA_BASE64_VARIANT_ENCODING_UTF32_BIG_ENDIAN:
					byte_stream_copy_from_uint32_big_endian(
					 &( base64_stream[ safe_base64_stream_index ] ),
					 base64_character );
					break;

				case LIBUNA_BASE64_VARIANT_ENCODING_UTF32_LITTLE_ENDIAN:
					byte_stream_copy_from_uint32_little_endian(
					 &( base64_stream[ safe_base64_stream_index ] ),
					 base64_character );
					break;
			}
			safe_base64_stream_index += base64_character_size;
		}
	}
	*base64_stream_index = safe_base64_stream_index;

	return( 1 );
}

