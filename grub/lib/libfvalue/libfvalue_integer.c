/*
 * Integer value functions
 *
 * Copyright (C) 2010-2022, Joachim Metz <joachim.metz@gmail.com>
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

#include "libfvalue_definitions.h"
#include "libfvalue_integer.h"

/* Creates an integer
 * Make sure the value integer is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfvalue_integer_initialize(
     libfvalue_integer_t **integer,
     libcerror_error_t **error )
{
	static char *function = "libfvalue_integer_initialize";

	if( integer == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid integer.",
		 function );

		return( -1 );
	}
	if( *integer != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid integer value already set.",
		 function );

		return( -1 );
	}
	*integer = memory_allocate_structure(
	            libfvalue_integer_t );

	if( *integer == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create integer.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *integer,
	     0,
	     sizeof( libfvalue_integer_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear integer.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *integer != NULL )
	{
		memory_free(
		 *integer );

		*integer = NULL;
	}
	return( -1 );
}

/* Frees an integer
 * Returns 1 if successful or -1 on error
 */
int libfvalue_integer_free(
     libfvalue_integer_t **integer,
     libcerror_error_t **error )
{
	static char *function = "libfvalue_integer_free";

	if( integer == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid integer.",
		 function );

		return( -1 );
	}
	if( *integer != NULL )
	{
		memory_free(
		 *integer );

		*integer = NULL;
	}
	return( 1 );
}

/* Clones an integer
 * Returns 1 if successful or -1 on error
 */
int libfvalue_integer_clone(
     libfvalue_integer_t **destination_integer,
     libfvalue_integer_t *source_integer,
     libcerror_error_t **error )
{
	static char *function = "libfvalue_integer_clone";

	if( destination_integer == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid destination integer.",
		 function );

		return( -1 );
	}
	if( *destination_integer != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: destination integer already set.",
		 function );

		return( -1 );
	}
	if( source_integer == NULL )
	{
		*destination_integer = NULL;

		return( 1 );
	}
	*destination_integer = memory_allocate_structure(
	                        libfvalue_integer_t );

	if( *destination_integer == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create destination integer.",
		 function );

		goto on_error;
	}
	if( memory_copy(
	     *destination_integer,
	     source_integer,
	     sizeof( libfvalue_integer_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy integer.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *destination_integer != NULL )
	{
		memory_free(
		 *destination_integer );

		*destination_integer = NULL;
	}
	return( -1 );
}

/* Copies the integer from a byte stream
 * Returns 1 if successful or -1 on error
 */
int libfvalue_integer_copy_from_byte_stream(
     libfvalue_integer_t *integer,
     const uint8_t *byte_stream,
     size_t byte_stream_size,
     int encoding,
     libcerror_error_t **error )
{
	static char *function = "libfvalue_integer_copy_from_byte_stream";

	if( integer == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid integer.",
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
	if( ( encoding != LIBFVALUE_ENDIAN_BIG )
	 && ( encoding != LIBFVALUE_ENDIAN_LITTLE )
	 && ( encoding != LIBFVALUE_ENDIAN_NATIVE ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported encoding.",
		 function );

		return( -1 );
	}
	switch( byte_stream_size )
	{
		case 1:
			integer->value = (uint64_t) *( (uint8_t *) byte_stream );

			break;

		case 2:
			if( encoding == LIBFVALUE_ENDIAN_BIG )
			{
				byte_stream_copy_to_uint16_big_endian(
				 byte_stream,
				 integer->value );
			}
			else if( encoding == LIBFVALUE_ENDIAN_LITTLE )
			{
				byte_stream_copy_to_uint16_little_endian(
				 byte_stream,
				 integer->value );
			}
			else
			{
				integer->value = (uint64_t) *( (uint16_t *) byte_stream );
			}
			break;

		case 4:
			if( encoding == LIBFVALUE_ENDIAN_BIG )
			{
				byte_stream_copy_to_uint32_big_endian(
				 byte_stream,
				 integer->value );
			}
			else if( encoding == LIBFVALUE_ENDIAN_LITTLE )
			{
				byte_stream_copy_to_uint32_little_endian(
				 byte_stream,
				 integer->value );
			}
			else
			{
				integer->value = (uint64_t) *( (uint32_t *) byte_stream );
			}
			break;

		case 8:
			if( encoding == LIBFVALUE_ENDIAN_BIG )
			{
				byte_stream_copy_to_uint64_big_endian(
				 byte_stream,
				 integer->value );
			}
			else if( encoding == LIBFVALUE_ENDIAN_LITTLE )
			{
				byte_stream_copy_to_uint64_little_endian(
				 byte_stream,
				 integer->value );
			}
			else
			{
				integer->value = (uint64_t) *( (uint64_t *) byte_stream );
			}
			break;

		default:
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported byte stream size: %" PRIzd ".",
			 function,
			 byte_stream_size );

			return( -1 );
	}
	integer->value_size = byte_stream_size * 8;

	return( 1 );
}

/* Copies the integer from an integer value
 * Returns 1 if successful or -1 on error
 */
int libfvalue_integer_copy_from_integer(
     libfvalue_integer_t *integer,
     uint64_t integer_value,
     size_t integer_value_size,
     libcerror_error_t **error )
{
	static char *function = "libfvalue_integer_copy_from_integer";

	if( integer == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid integer.",
		 function );

		return( -1 );
	}
	if( ( integer_value_size != 8 )
	 && ( integer_value_size != 16 )
	 && ( integer_value_size != 32 )
	 && ( integer_value_size != 64 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported integer value size.",
		 function );

		return( -1 );
	}
	integer->value      = integer_value;
	integer->value_size = integer_value_size;

	return( 1 );
}

/* Copies the integer to an integer value
 * Returns 1 if successful or -1 on error
 */
int libfvalue_integer_copy_to_integer(
     libfvalue_integer_t *integer,
     uint64_t *integer_value,
     size_t *integer_value_size,
     libcerror_error_t **error )
{
	static char *function = "libfvalue_integer_copy_to_integer";

	if( integer == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid integer.",
		 function );

		return( -1 );
	}
	if( integer_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid integer value.",
		 function );

		return( -1 );
	}
	if( integer_value_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid integer value size.",
		 function );

		return( -1 );
	}
	*integer_value      = integer->value;
	*integer_value_size = integer->value_size;

	return( 1 );
}

/* Retrieves the size of a string of the integer
 * Returns 1 if successful or -1 on error
 */
int libfvalue_integer_get_string_size(
     libfvalue_integer_t *integer,
     size_t *string_size,
     uint32_t string_format_flags,
     libcerror_error_t **error )
{
	static char *function = "libfvalue_integer_get_string_size";

	if( integer == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid integer.",
		 function );

		return( -1 );
	}
	if( libfvalue_string_size_from_integer(
	     string_size,
	     integer->value,
	     integer->value_size,
	     string_format_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to determine size of string of %" PRIzd "-bit integer.",
		 function,
		 integer->value_size );

		return( -1 );
	}
	return( 1 );
}

/* Copies the integer from an UTF-8 encoded string
 * Returns 1 if successful or -1 on error
 */
int libfvalue_integer_copy_from_utf8_string_with_index(
     libfvalue_integer_t *integer,
     uint8_t *utf8_string,
     size_t utf8_string_length,
     size_t *utf8_string_index,
     uint32_t string_format_flags,
     libcerror_error_t **error )
{
	static char *function = "libfvalue_integer_copy_from_utf8_string_with_index";

	if( integer == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid integer.",
		 function );

		return( -1 );
	}
	if( libfvalue_utf8_string_with_index_copy_to_integer(
	     utf8_string,
	     utf8_string_length,
	     utf8_string_index,
	     &( integer->value ),
	     integer->value_size,
	     string_format_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy %" PRIzd "-bit integer from UTF-8 string.",
		 function,
		 integer->value_size );

		return( -1 );
	}
	return( 1 );
}

/* Copies the integer to an UTF-8 encoded string
 * Returns 1 if successful or -1 on error
 */
int libfvalue_integer_copy_to_utf8_string_with_index(
     libfvalue_integer_t *integer,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     size_t *utf8_string_index,
     uint32_t string_format_flags,
     libcerror_error_t **error )
{
	static char *function = "libfvalue_integer_copy_to_utf8_string_with_index";

	if( integer == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid integer.",
		 function );

		return( -1 );
	}
	if( libfvalue_utf8_string_with_index_copy_from_integer(
	     utf8_string,
	     utf8_string_size,
	     utf8_string_index,
	     integer->value,
	     integer->value_size,
	     string_format_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy %" PRIzd "-bit integer to UTF-8 string.",
		 function,
		 integer->value_size );

		return( -1 );
	}
	return( 1 );
}

/* Copies the integer from an UTF-16 encoded string
 * Returns 1 if successful or -1 on error
 */
int libfvalue_integer_copy_from_utf16_string_with_index(
     libfvalue_integer_t *integer,
     uint16_t *utf16_string,
     size_t utf16_string_length,
     size_t *utf16_string_index,
     uint32_t string_format_flags,
     libcerror_error_t **error )
{
	static char *function = "libfvalue_integer_copy_from_utf16_string_with_index";

	if( integer == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid integer.",
		 function );

		return( -1 );
	}
	if( libfvalue_utf16_string_with_index_copy_to_integer(
	     utf16_string,
	     utf16_string_length,
	     utf16_string_index,
	     &( integer->value ),
	     integer->value_size,
	     string_format_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy %" PRIzd "-bit integer from UTF-16 string.",
		 function,
		 integer->value_size );

		return( -1 );
	}
	return( 1 );
}

/* Copies the integer to an UTF-16 encoded string
 * Returns 1 if successful or -1 on error
 */
int libfvalue_integer_copy_to_utf16_string_with_index(
     libfvalue_integer_t *integer,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     size_t *utf16_string_index,
     uint32_t string_format_flags,
     libcerror_error_t **error )
{
	static char *function = "libfvalue_integer_copy_to_utf16_string_with_index";

	if( integer == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid integer.",
		 function );

		return( -1 );
	}
	if( libfvalue_utf16_string_with_index_copy_from_integer(
	     utf16_string,
	     utf16_string_size,
	     utf16_string_index,
	     integer->value,
	     integer->value_size,
	     string_format_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy %" PRIzd "-bit integer to UTF-16 string.",
		 function,
		 integer->value_size );

		return( -1 );
	}
	return( 1 );
}

/* Copies the integer from an UTF-32 encoded string
 * Returns 1 if successful or -1 on error
 */
int libfvalue_integer_copy_from_utf32_string_with_index(
     libfvalue_integer_t *integer,
     uint32_t *utf32_string,
     size_t utf32_string_length,
     size_t *utf32_string_index,
     uint32_t string_format_flags,
     libcerror_error_t **error )
{
	static char *function = "libfvalue_integer_copy_from_utf32_string_with_index";

	if( integer == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid integer.",
		 function );

		return( -1 );
	}
	if( libfvalue_utf32_string_with_index_copy_to_integer(
	     utf32_string,
	     utf32_string_length,
	     utf32_string_index,
	     &( integer->value ),
	     integer->value_size,
	     string_format_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy %" PRIzd "-bit integer from UTF-32 string.",
		 function,
		 integer->value_size );

		return( -1 );
	}
	return( 1 );
}

/* Copies the integer to an UTF-32 encoded string
 * Returns 1 if successful or -1 on error
 */
int libfvalue_integer_copy_to_utf32_string_with_index(
     libfvalue_integer_t *integer,
     uint32_t *utf32_string,
     size_t utf32_string_size,
     size_t *utf32_string_index,
     uint32_t string_format_flags,
     libcerror_error_t **error )
{
	static char *function = "libfvalue_integer_copy_to_utf32_string_with_index";

	if( integer == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid integer.",
		 function );

		return( -1 );
	}
	if( libfvalue_utf32_string_with_index_copy_from_integer(
	     utf32_string,
	     utf32_string_size,
	     utf32_string_index,
	     integer->value,
	     integer->value_size,
	     string_format_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy %" PRIzd "-bit integer to UTF-32 string.",
		 function,
		 integer->value_size );

		return( -1 );
	}
	return( 1 );
}

/* Determines the size of a string of an integer value
 * The integer value size is in bits
 * Returns 1 if successful or -1 on error
 */
int libfvalue_string_size_from_integer(
     size_t *string_size,
     uint64_t integer_value,
     size_t integer_value_size,
     uint32_t string_format_flags,
     libcerror_error_t **error )
{
	static char *function       = "libfvalue_string_size_from_integer";
	uint64_t divider            = 0;
	uint32_t string_format_type = 0;
	uint32_t supported_flags    = 0;
	uint8_t is_signed           = 0;
	int8_t bit_shift            = 0;

	if( string_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid string size.",
		 function );

		return( -1 );
	}
	if( ( integer_value_size != 8 )
	 && ( integer_value_size != 16 )
	 && ( integer_value_size != 32 )
	 && ( integer_value_size != 64 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported integer value size.",
		 function );

		return( -1 );
	}
	supported_flags = 0x000000ffUL
	                | LIBFVALUE_INTEGER_FORMAT_FLAG_SIGNED
	                | LIBFVALUE_INTEGER_FORMAT_FLAG_UNSIGNED
	                | LIBFVALUE_INTEGER_FORMAT_FLAG_NO_BASE_INDICATOR;

	if( ( string_format_flags & ~( supported_flags ) ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported string format flags: 0x%08" PRIx32 ".",
		 function,
		 string_format_flags );

		return( -1 );
	}
	string_format_type = string_format_flags & 0x000000ffUL;

	if( ( string_format_type != LIBFVALUE_INTEGER_FORMAT_TYPE_DECIMAL )
	 && ( string_format_type != LIBFVALUE_INTEGER_FORMAT_TYPE_HEXADECIMAL )
	 && ( string_format_type != LIBFVALUE_INTEGER_FORMAT_TYPE_BOOLEAN ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported string format type.",
		 function );

		return( -1 );
	}
	if( string_format_type == LIBFVALUE_INTEGER_FORMAT_TYPE_BOOLEAN )
	{
		if( integer_value == 0 )
		{
			*string_size = 6;
		}
		else
		{
			*string_size = 5;
		}
	}
	else if( string_format_type == LIBFVALUE_INTEGER_FORMAT_TYPE_HEXADECIMAL )
	{
		*string_size = ( integer_value_size >> 2 ) + 1;

		if( ( string_format_flags & LIBFVALUE_INTEGER_FORMAT_FLAG_NO_BASE_INDICATOR ) == 0 )
		{
			*string_size += 2;
		}
	}
	else
	{
		/* The string is at least a single digit with an end of string character
		 */
		*string_size = 2;

		bit_shift = (uint8_t) ( integer_value_size - 1 );

		if( ( string_format_flags & LIBFVALUE_INTEGER_FORMAT_FLAG_SIGNED ) != 0 )
		{
			is_signed = (uint8_t) ( integer_value >> bit_shift );

			if( is_signed != 0 )
			{
				*string_size += 1;

				integer_value &= ~( (uint64_t) 1 << bit_shift );

				if( integer_value == 0 )
				{
					integer_value |= (uint64_t) 1 << bit_shift;
				}
			}
		}
		divider = 1;

		while( ( integer_value / divider ) >= 10 )
		{
			divider *= 10;

			*string_size += 1;
		}
	}
	return( 1 );
}

/* Copies an UTF-8 encoded string from an integer value
 * The integer value size is in bits
 * Returns 1 if successful or -1 on error
 */
int libfvalue_utf8_string_copy_from_integer(
     uint8_t *utf8_string,
     size_t utf8_string_size,
     uint64_t integer_value,
     size_t integer_value_size,
     uint32_t string_format_flags,
     libcerror_error_t **error )
{
	static char *function    = "libfvalue_utf8_string_copy_from_integer";
	size_t utf8_string_index = 0;

	if( libfvalue_utf8_string_with_index_copy_from_integer(
	     utf8_string,
	     utf8_string_size,
	     &utf8_string_index,
	     integer_value,
	     integer_value_size,
	     string_format_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy integer value to UTF-8 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Copies an UTF-8 encoded string of from integer value
 * The integer value size is in bits
 * Returns 1 if successful or -1 on error
 */
int libfvalue_utf8_string_with_index_copy_from_integer(
     uint8_t *utf8_string,
     size_t utf8_string_size,
     size_t *utf8_string_index,
     uint64_t integer_value,
     size_t integer_value_size,
     uint32_t string_format_flags,
     libcerror_error_t **error )
{
	static char *function         = "libfvalue_utf8_string_with_index_copy_from_integer";
	size_t safe_utf8_string_index = 0;
	uint64_t divider              = 0;
	uint32_t string_format_type   = 0;
	uint32_t supported_flags      = 0;
	uint8_t byte_value            = 0;
	uint8_t is_signed             = 0;
	uint8_t number_of_characters  = 0;
	int8_t bit_shift              = 0;

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
	if( *utf8_string_index >= utf8_string_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid UTF-8 string index value out of bounds.",
		 function );

		return( -1 );
	}
	safe_utf8_string_index = *utf8_string_index;

	if( ( integer_value_size != 8 )
	 && ( integer_value_size != 16 )
	 && ( integer_value_size != 32 )
	 && ( integer_value_size != 64 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported integer value size.",
		 function );

		return( -1 );
	}
	supported_flags = 0x000000ffUL
	                | LIBFVALUE_INTEGER_FORMAT_FLAG_SIGNED
	                | LIBFVALUE_INTEGER_FORMAT_FLAG_UNSIGNED
	                | LIBFVALUE_INTEGER_FORMAT_FLAG_NO_BASE_INDICATOR;

	if( ( string_format_flags & ~( supported_flags ) ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported string format flags: 0x%08" PRIx32 ".",
		 function,
		 string_format_flags );

		return( -1 );
	}
	string_format_type = string_format_flags & 0x000000ffUL;

	if( ( string_format_type != LIBFVALUE_INTEGER_FORMAT_TYPE_DECIMAL )
	 && ( string_format_type != LIBFVALUE_INTEGER_FORMAT_TYPE_HEXADECIMAL )
	 && ( string_format_type != LIBFVALUE_INTEGER_FORMAT_TYPE_BOOLEAN ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported string format type.",
		 function );

		return( -1 );
	}
	if( string_format_type == LIBFVALUE_INTEGER_FORMAT_TYPE_BOOLEAN )
	{
		if( integer_value == 0 )
		{
			number_of_characters = 6;
		}
		else
		{
			number_of_characters = 5;
		}
	}
	else if( string_format_type == LIBFVALUE_INTEGER_FORMAT_TYPE_HEXADECIMAL )
	{
		number_of_characters = (uint8_t) ( integer_value_size >> 2 ) + 1;

		if( ( string_format_flags & LIBFVALUE_INTEGER_FORMAT_FLAG_NO_BASE_INDICATOR ) == 0 )
		{
			number_of_characters += 2;
		}
	}
	else
	{
		/* The string is at least a single digit with an end of string character
		 */
		number_of_characters = 2;

		bit_shift = (uint8_t) ( integer_value_size - 1 );

		if( ( string_format_flags & LIBFVALUE_INTEGER_FORMAT_FLAG_SIGNED ) != 0 )
		{
			is_signed = (uint8_t) ( integer_value >> bit_shift );

			if( is_signed != 0 )
			{
				number_of_characters += 1;

				integer_value &= ~( (uint64_t) 1 << bit_shift );

				if( integer_value == 0 )
				{
					integer_value |= (uint64_t) 1 << bit_shift;
				}
			}
		}
		divider = 1;

		while( ( integer_value / divider ) >= 10 )
		{
			divider *= 10;

			number_of_characters += 1;
		}
	}
	if( ( number_of_characters > utf8_string_size )
	 || ( safe_utf8_string_index > ( utf8_string_size - number_of_characters ) ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: UTF-8 string size too small.",
		 function );

		return( -1 );
	}
	if( string_format_type == LIBFVALUE_INTEGER_FORMAT_TYPE_BOOLEAN )
	{
		if( integer_value == 0 )
		{
			utf8_string[ safe_utf8_string_index++ ] = (uint8_t) 'f';
			utf8_string[ safe_utf8_string_index++ ] = (uint8_t) 'a';
			utf8_string[ safe_utf8_string_index++ ] = (uint8_t) 'l';
			utf8_string[ safe_utf8_string_index++ ] = (uint8_t) 's';
			utf8_string[ safe_utf8_string_index++ ] = (uint8_t) 'e';
		}
		else
		{
			utf8_string[ safe_utf8_string_index++ ] = (uint8_t) 't';
			utf8_string[ safe_utf8_string_index++ ] = (uint8_t) 'r';
			utf8_string[ safe_utf8_string_index++ ] = (uint8_t) 'u';
			utf8_string[ safe_utf8_string_index++ ] = (uint8_t) 'e';
		}
	}
	else if( string_format_type == LIBFVALUE_INTEGER_FORMAT_TYPE_HEXADECIMAL )
	{
		if( ( string_format_flags & LIBFVALUE_INTEGER_FORMAT_FLAG_NO_BASE_INDICATOR ) == 0 )
		{
			utf8_string[ safe_utf8_string_index++ ] = (uint8_t) '0';
			utf8_string[ safe_utf8_string_index++ ] = (uint8_t) 'x';
		}
		bit_shift = (uint8_t) ( integer_value_size - 4 );

		do
		{
			byte_value = (uint8_t) ( ( integer_value >> bit_shift ) & 0x0f );

			if( byte_value <= 9 )
			{
				utf8_string[ safe_utf8_string_index++ ] = (uint8_t) '0' + byte_value;
			}
			else
			{
				utf8_string[ safe_utf8_string_index++ ] = (uint8_t) 'a' + byte_value - 10;
			}
			bit_shift -= 4;
		}
		while( bit_shift >= 0 );
	}
	else
	{
		if( is_signed != 0 )
		{
			utf8_string[ safe_utf8_string_index++ ] = (uint8_t) '-';
		}
		while( divider > 1 )
		{
			utf8_string[ safe_utf8_string_index++ ] = (uint8_t) '0' + (uint8_t) ( integer_value / divider );

			integer_value %= divider;

			divider /= 10;
		}
		utf8_string[ safe_utf8_string_index++ ] = (uint8_t) '0' + (uint8_t) ( integer_value / divider );
	}
	utf8_string[ safe_utf8_string_index++ ] = 0;

	*utf8_string_index = safe_utf8_string_index;

	return( 1 );
}

/* Copies an UTF-8 encoded string to an integer value
 * The integer value size is in bits
 * Returns 1 if successful or -1 on error
 */
int libfvalue_utf8_string_copy_to_integer(
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     uint64_t *integer_value,
     size_t integer_value_size,
     uint32_t string_format_flags,
     libcerror_error_t **error )
{
	static char *function    = "libfvalue_utf8_string_copy_to_integer";
	size_t utf8_string_index = 0;

	if( libfvalue_utf8_string_with_index_copy_to_integer(
	     utf8_string,
	     utf8_string_length,
	     &utf8_string_index,
	     integer_value,
	     integer_value_size,
	     string_format_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy UTF-8 string to integer value.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Copies an UTF-8 encoded string to an integer value
 * The integer value size is in bits
 * Returns 1 if successful or -1 on error
 */
int libfvalue_utf8_string_with_index_copy_to_integer(
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     size_t *utf8_string_index,
     uint64_t *integer_value,
     size_t integer_value_size,
     uint32_t string_format_flags,
     libcerror_error_t **error )
{
	static char *function         = "libfvalue_utf8_string_with_index_copy_to_integer";
	size_t maximum_string_index   = 0;
	size_t safe_utf8_string_index = 0;
	uint64_t divider              = 0;
	uint64_t value_64bit          = 0;
	uint32_t string_format_type   = 0;
	uint32_t supported_flags      = 0;
	uint8_t byte_value            = 0;
	uint8_t character_value       = 0;
	int8_t bit_shift              = 0;
	int8_t sign                   = 1;

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
	if( utf8_string_length > (size_t) SSIZE_MAX )
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
	if( *utf8_string_index >= utf8_string_length )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid UTF-8 string index value out of bounds.",
		 function );

		return( -1 );
	}
	safe_utf8_string_index = *utf8_string_index;

	if( integer_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid integer value.",
		 function );

		return( -1 );
	}
	if( ( integer_value_size != 8 )
	 && ( integer_value_size != 16 )
	 && ( integer_value_size != 32 )
	 && ( integer_value_size != 64 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported integer value size.",
		 function );

		return( -1 );
	}
	supported_flags = 0x000000ffUL
	                | LIBFVALUE_INTEGER_FORMAT_FLAG_SIGNED
	                | LIBFVALUE_INTEGER_FORMAT_FLAG_UNSIGNED
	                | LIBFVALUE_INTEGER_FORMAT_FLAG_NO_BASE_INDICATOR;

	if( ( string_format_flags & ~( supported_flags ) ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported string format flags: 0x%08" PRIx32 ".",
		 function,
		 string_format_flags );

		return( -1 );
	}
	string_format_type = string_format_flags & 0x000000ffUL;

	if( ( string_format_type != LIBFVALUE_INTEGER_FORMAT_TYPE_DECIMAL )
	 && ( string_format_type != LIBFVALUE_INTEGER_FORMAT_TYPE_HEXADECIMAL )
	 && ( string_format_type != LIBFVALUE_INTEGER_FORMAT_TYPE_BOOLEAN ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported string format type.",
		 function );

		return( -1 );
	}
	if( string_format_type == LIBFVALUE_INTEGER_FORMAT_TYPE_BOOLEAN )
	{
		if( integer_value == 0 )
		{
			maximum_string_index = 5;
		}
		else
		{
			maximum_string_index = 4;
		}
	}
	else if( string_format_type == LIBFVALUE_INTEGER_FORMAT_TYPE_HEXADECIMAL )
	{
		maximum_string_index = (size_t) ( integer_value_size >> 2 );

		if( ( string_format_flags & LIBFVALUE_INTEGER_FORMAT_FLAG_NO_BASE_INDICATOR ) == 0 )
		{
			maximum_string_index += 2;
		}
	}
	else
	{
		/* The string is at least a single digit with an end of string character
		 */
		maximum_string_index = 2;

		bit_shift = (uint8_t) ( integer_value_size - 1 );

		divider = 1;

		value_64bit = ~( ( ~( (uint64_t) 1 << bit_shift ) >> bit_shift ) << bit_shift );

		while( ( value_64bit / divider ) >= 10 )
		{
			divider *= 10;

			maximum_string_index += 1;
		}
	}
	maximum_string_index += safe_utf8_string_index;

	if( maximum_string_index > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid maximum string index value exceeds maximum.",
		 function );

		return( -1 );
	}
	value_64bit = 0;

	if( string_format_type == LIBFVALUE_INTEGER_FORMAT_TYPE_BOOLEAN )
	{
/* TODO */
	}
	else if( string_format_type == LIBFVALUE_INTEGER_FORMAT_TYPE_HEXADECIMAL )
	{
		if( ( string_format_flags & LIBFVALUE_INTEGER_FORMAT_FLAG_NO_BASE_INDICATOR ) == 0 )
		{
			character_value = utf8_string[ safe_utf8_string_index++ ];

			if( character_value != (uint8_t) '0' )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
				 "%s: unsupported character value: 0x%02" PRIx8 " at index: %d.",
				 function,
				 character_value,
				 safe_utf8_string_index );

				return( -1 );
			}
			character_value = utf8_string[ safe_utf8_string_index++ ];

			if( character_value != (uint8_t) 'x' )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
				 "%s: unsupported character value: 0x%02" PRIx8 " at index: %d.",
				 function,
				 character_value,
				 safe_utf8_string_index );

				return( -1 );
			}
		}
		while( safe_utf8_string_index < utf8_string_length )
		{
			character_value = utf8_string[ safe_utf8_string_index ];

			if( character_value == 0 )
			{
				break;
			}
			if( safe_utf8_string_index > (size_t) maximum_string_index )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_LARGE,
				 "%s: string too large.",
				 function );

				return( -1 );
			}
			value_64bit <<= 4;

			if( ( character_value >= (uint8_t) '0' )
			 && ( character_value <= (uint8_t) '9' ) )
			{
				byte_value = character_value - (uint8_t) '0';
			}
			else if( ( character_value >= (uint8_t) 'A' )
			      && ( character_value <= (uint8_t) 'F' ) )
			{
				byte_value = character_value - (uint8_t) 'A' + 10;
			}
			else if( ( character_value >= (uint8_t) 'a' )
			      && ( character_value <= (uint8_t) 'f' ) )
			{
				byte_value = character_value - (uint8_t) 'a' + 10;
			}
			else
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
				 "%s: unsupported character value: 0x%02" PRIx8 " at index: %d.",
				 function,
				 character_value,
				 safe_utf8_string_index );

				return( -1 );
			}
			value_64bit += byte_value;

			safe_utf8_string_index++;
		}
	}
	else
	{
		if( ( string_format_flags & LIBFVALUE_INTEGER_FORMAT_FLAG_SIGNED ) != 0 )
		{
			character_value = utf8_string[ safe_utf8_string_index ];

			/* In the maximum possible string one character is substituted for the sign
			 */
			if( character_value == (uint8_t) '-' )
			{
				safe_utf8_string_index++;

				sign = -1;
			}
			else if( character_value == (uint8_t) '+' )
			{
				safe_utf8_string_index++;
			}
		}
		while( safe_utf8_string_index < utf8_string_length )
		{
			character_value = utf8_string[ safe_utf8_string_index ];

			if( character_value == 0 )
			{
				break;
			}
			if( safe_utf8_string_index > (size_t) maximum_string_index )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_LARGE,
				 "%s: string too large.",
				 function );

				return( -1 );
			}
			value_64bit *= 10;

			if( ( character_value < (uint8_t) '0' )
			 || ( character_value > (uint8_t) '9' ) )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
				 "%s: unsupported character value: 0x%02" PRIx8 " at index: %d.",
				 function,
				 character_value,
				 safe_utf8_string_index );

				return( -1 );
			}
			character_value -= (uint8_t) '0';

			value_64bit += character_value;

			safe_utf8_string_index++;
		}
		if( ( sign == -1 )
		 && ( value_64bit != 0 ) )
		{
			value_64bit = ~( value_64bit - 1 );
		}
	}
	*utf8_string_index = safe_utf8_string_index;
	*integer_value     = value_64bit;

	return( 1 );
}

/* Copies an UTF-16 encoded string of an integer value
 * The integer value size is in bits
 * Returns 1 if successful or -1 on error
 */
int libfvalue_utf16_string_copy_from_integer(
     uint16_t *utf16_string,
     size_t utf16_string_size,
     uint64_t integer_value,
     size_t integer_value_size,
     uint32_t string_format_flags,
     libcerror_error_t **error )
{
	static char *function     = "libfvalue_utf16_string_copy_from_integer";
	size_t utf16_string_index = 0;

	if( libfvalue_utf16_string_with_index_copy_from_integer(
	     utf16_string,
	     utf16_string_size,
	     &utf16_string_index,
	     integer_value,
	     integer_value_size,
	     string_format_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy integer value to UTF-16 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Copies an UTF-16 encoded string of an integer value
 * The integer value size is in bits
 * Returns 1 if successful or -1 on error
 */
int libfvalue_utf16_string_with_index_copy_from_integer(
     uint16_t *utf16_string,
     size_t utf16_string_size,
     size_t *utf16_string_index,
     uint64_t integer_value,
     size_t integer_value_size,
     uint32_t string_format_flags,
     libcerror_error_t **error )
{
	static char *function          = "libfvalue_utf16_string_with_index_copy_from_integer";
	size_t safe_utf16_string_index = 0;
	uint64_t divider               = 0;
	uint32_t string_format_type    = 0;
	uint32_t supported_flags       = 0;
	uint8_t byte_value             = 0;
	uint8_t is_signed              = 0;
	uint8_t number_of_characters   = 0;
	int8_t bit_shift               = 0;

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
	if( *utf16_string_index >= utf16_string_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid UTF-16 string index value out of bounds.",
		 function );

		return( -1 );
	}
	safe_utf16_string_index = *utf16_string_index;

	if( ( integer_value_size != 8 )
	 && ( integer_value_size != 16 )
	 && ( integer_value_size != 32 )
	 && ( integer_value_size != 64 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported integer value size.",
		 function );

		return( -1 );
	}
	supported_flags = 0x000000ffUL
	                | LIBFVALUE_INTEGER_FORMAT_FLAG_SIGNED
	                | LIBFVALUE_INTEGER_FORMAT_FLAG_UNSIGNED
	                | LIBFVALUE_INTEGER_FORMAT_FLAG_NO_BASE_INDICATOR;

	if( ( string_format_flags & ~( supported_flags ) ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported string format flags: 0x%08" PRIx32 ".",
		 function,
		 string_format_flags );

		return( -1 );
	}
	string_format_type = string_format_flags & 0x000000ffUL;

	if( ( string_format_type != LIBFVALUE_INTEGER_FORMAT_TYPE_DECIMAL )
	 && ( string_format_type != LIBFVALUE_INTEGER_FORMAT_TYPE_HEXADECIMAL )
	 && ( string_format_type != LIBFVALUE_INTEGER_FORMAT_TYPE_BOOLEAN ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported string format type.",
		 function );

		return( -1 );
	}
	if( string_format_type == LIBFVALUE_INTEGER_FORMAT_TYPE_BOOLEAN )
	{
		if( integer_value == 0 )
		{
			number_of_characters = 6;
		}
		else
		{
			number_of_characters = 5;
		}
	}
	else if( string_format_type == LIBFVALUE_INTEGER_FORMAT_TYPE_HEXADECIMAL )
	{
		number_of_characters = (uint8_t) ( integer_value_size >> 2 ) + 1;

		if( ( string_format_flags & LIBFVALUE_INTEGER_FORMAT_FLAG_NO_BASE_INDICATOR ) == 0 )
		{
			number_of_characters += 2;
		}
	}
	else
	{
		/* The string is at least a single digit with an end of string character
		 */
		number_of_characters = 2;

		bit_shift = (uint8_t) ( integer_value_size - 1 );

		if( ( string_format_flags & LIBFVALUE_INTEGER_FORMAT_FLAG_SIGNED ) != 0 )
		{
			is_signed = (uint8_t) ( integer_value >> bit_shift );

			if( is_signed != 0 )
			{
				number_of_characters += 1;

				integer_value &= ~( (uint64_t) 1 << bit_shift );

				if( integer_value == 0 )
				{
					integer_value |= (uint64_t) 1 << bit_shift;
				}
			}
		}
		divider = 1;

		while( ( integer_value / divider ) >= 10 )
		{
			divider *= 10;

			number_of_characters += 1;
		}
	}
	if( ( number_of_characters > utf16_string_size )
	 || ( safe_utf16_string_index > ( utf16_string_size - number_of_characters ) ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: UTF-16 string size too small.",
		 function );

		return( -1 );
	}
	if( string_format_type == LIBFVALUE_INTEGER_FORMAT_TYPE_BOOLEAN )
	{
		if( integer_value == 0 )
		{
			utf16_string[ safe_utf16_string_index++ ] = (uint16_t) 'f';
			utf16_string[ safe_utf16_string_index++ ] = (uint16_t) 'a';
			utf16_string[ safe_utf16_string_index++ ] = (uint16_t) 'l';
			utf16_string[ safe_utf16_string_index++ ] = (uint16_t) 's';
			utf16_string[ safe_utf16_string_index++ ] = (uint16_t) 'e';
		}
		else
		{
			utf16_string[ safe_utf16_string_index++ ] = (uint16_t) 't';
			utf16_string[ safe_utf16_string_index++ ] = (uint16_t) 'r';
			utf16_string[ safe_utf16_string_index++ ] = (uint16_t) 'u';
			utf16_string[ safe_utf16_string_index++ ] = (uint16_t) 'e';
		}
	}
	else if( string_format_type == LIBFVALUE_INTEGER_FORMAT_TYPE_HEXADECIMAL )
	{
		if( ( string_format_flags & LIBFVALUE_INTEGER_FORMAT_FLAG_NO_BASE_INDICATOR ) == 0 )
		{
			utf16_string[ safe_utf16_string_index++ ] = (uint16_t) '0';
			utf16_string[ safe_utf16_string_index++ ] = (uint16_t) 'x';
		}
		bit_shift = (uint8_t) ( integer_value_size - 4 );

		do
		{
			byte_value = (uint16_t) ( ( integer_value >> bit_shift ) & 0x0f );

			if( byte_value <= 9 )
			{
				utf16_string[ safe_utf16_string_index++ ] = (uint16_t) '0' + byte_value;
			}
			else
			{
				utf16_string[ safe_utf16_string_index++ ] = (uint16_t) 'a' + byte_value - 10;
			}
			bit_shift -= 4;
		}
		while( bit_shift >= 0 );
	}
	else
	{
		if( is_signed != 0 )
		{
			utf16_string[ safe_utf16_string_index++ ] = (uint16_t) '-';
		}
		while( divider > 1 )
		{
			utf16_string[ safe_utf16_string_index++ ] = (uint16_t) '0' + (uint16_t) ( integer_value / divider );

			integer_value %= divider;

			divider /= 10;
		}
		utf16_string[ safe_utf16_string_index++ ] = (uint16_t) '0' + (uint16_t) ( integer_value / divider );
	}
	utf16_string[ safe_utf16_string_index++ ] = 0;

	*utf16_string_index = safe_utf16_string_index;

	return( 1 );
}

/* Copies an UTF-16 encoded string to an integer value
 * The integer value size is in bits
 * Returns 1 if successful or -1 on error
 */
int libfvalue_utf16_string_copy_to_integer(
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     uint64_t *integer_value,
     size_t integer_value_size,
     uint32_t string_format_flags,
     libcerror_error_t **error )
{
	static char *function     = "libfvalue_utf16_string_copy_to_integer";
	size_t utf16_string_index = 0;

	if( libfvalue_utf16_string_with_index_copy_to_integer(
	     utf16_string,
	     utf16_string_length,
	     &utf16_string_index,
	     integer_value,
	     integer_value_size,
	     string_format_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy UTF-16 string to integer value.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Copies an UTF-16 encoded string to an integer value
 * The integer value size is in bits
 * Returns 1 if successful or -1 on error
 */
int libfvalue_utf16_string_with_index_copy_to_integer(
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     size_t *utf16_string_index,
     uint64_t *integer_value,
     size_t integer_value_size,
     uint32_t string_format_flags,
     libcerror_error_t **error )
{
	static char *function          = "libfvalue_utf16_string_with_index_copy_to_integer";
	size_t maximum_string_index    = 0;
	size_t safe_utf16_string_index = 0;
	uint64_t divider               = 0;
	uint64_t value_64bit           = 0;
	uint32_t string_format_type    = 0;
	uint32_t supported_flags       = 0;
	uint8_t byte_value             = 0;
	uint8_t character_value        = 0;
	int8_t bit_shift               = 0;
	int8_t sign                    = 1;

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
	if( utf16_string_length > (size_t) SSIZE_MAX )
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
	if( *utf16_string_index >= utf16_string_length )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid UTF-16 string index value out of bounds.",
		 function );

		return( -1 );
	}
	safe_utf16_string_index = *utf16_string_index;

	if( integer_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid integer value.",
		 function );

		return( -1 );
	}
	if( ( integer_value_size != 8 )
	 && ( integer_value_size != 16 )
	 && ( integer_value_size != 32 )
	 && ( integer_value_size != 64 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported integer value size.",
		 function );

		return( -1 );
	}
	supported_flags = 0x000000ffUL
	                | LIBFVALUE_INTEGER_FORMAT_FLAG_SIGNED
	                | LIBFVALUE_INTEGER_FORMAT_FLAG_UNSIGNED
	                | LIBFVALUE_INTEGER_FORMAT_FLAG_NO_BASE_INDICATOR;

	if( ( string_format_flags & ~( supported_flags ) ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported string format flags: 0x%08" PRIx32 ".",
		 function,
		 string_format_flags );

		return( -1 );
	}
	string_format_type = string_format_flags & 0x000000ffUL;

	if( ( string_format_type != LIBFVALUE_INTEGER_FORMAT_TYPE_DECIMAL )
	 && ( string_format_type != LIBFVALUE_INTEGER_FORMAT_TYPE_HEXADECIMAL )
	 && ( string_format_type != LIBFVALUE_INTEGER_FORMAT_TYPE_BOOLEAN ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported string format type.",
		 function );

		return( -1 );
	}
	if( string_format_type == LIBFVALUE_INTEGER_FORMAT_TYPE_BOOLEAN )
	{
		if( integer_value == 0 )
		{
			maximum_string_index = 5;
		}
		else
		{
			maximum_string_index = 4;
		}
	}
	else if( string_format_type == LIBFVALUE_INTEGER_FORMAT_TYPE_HEXADECIMAL )
	{
		maximum_string_index = (size_t) ( integer_value_size >> 2 );

		if( ( string_format_flags & LIBFVALUE_INTEGER_FORMAT_FLAG_NO_BASE_INDICATOR ) == 0 )
		{
			maximum_string_index += 2;
		}
	}
	else
	{
		/* The string is at least a single digit with an end of string character
		 */
		maximum_string_index = 2;

		bit_shift = (uint8_t) ( integer_value_size - 1 );

		divider = 1;

		value_64bit = ~( ( ~( (uint64_t) 1 << bit_shift ) >> bit_shift ) << bit_shift );

		while( ( value_64bit / divider ) >= 10 )
		{
			divider *= 10;

			maximum_string_index += 1;
		}
	}
	maximum_string_index += safe_utf16_string_index;

	if( maximum_string_index > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid maximum string index value exceeds maximum.",
		 function );

		return( -1 );
	}
	value_64bit = 0;

	if( string_format_type == LIBFVALUE_INTEGER_FORMAT_TYPE_BOOLEAN )
	{
/* TODO */
	}
	else if( string_format_type == LIBFVALUE_INTEGER_FORMAT_TYPE_HEXADECIMAL )
	{
		if( ( string_format_flags & LIBFVALUE_INTEGER_FORMAT_FLAG_NO_BASE_INDICATOR ) == 0 )
		{
			character_value = utf16_string[ safe_utf16_string_index++ ];

			if( character_value != (uint16_t) '0' )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
				 "%s: unsupported character value: 0x04%" PRIx16 " at index: %d.",
				 function,
				 character_value,
				 safe_utf16_string_index );

				return( -1 );
			}
			character_value = utf16_string[ safe_utf16_string_index++ ];

			if( character_value != (uint16_t) 'x' )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
				 "%s: unsupported character value: 0x04%" PRIx16 " at index: %d.",
				 function,
				 character_value,
				 safe_utf16_string_index );

				return( -1 );
			}
		}
		while( safe_utf16_string_index < utf16_string_length )
		{
			character_value = utf16_string[ safe_utf16_string_index ];

			if( character_value == 0 )
			{
				break;
			}
			if( safe_utf16_string_index > (size_t) maximum_string_index )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_LARGE,
				 "%s: string too large.",
				 function );

				return( -1 );
			}
			value_64bit <<= 4;

			if( ( character_value >= (uint16_t) '0' )
			 && ( character_value <= (uint16_t) '9' ) )
			{
				byte_value = (uint8_t) ( character_value - (uint16_t) '0' );
			}
			else if( ( character_value >= (uint16_t) 'A' )
			      && ( character_value <= (uint16_t) 'F' ) )
			{
				byte_value = (uint8_t) ( character_value - (uint16_t) 'A' + 10 );
			}
			else if( ( character_value >= (uint16_t) 'a' )
			      && ( character_value <= (uint16_t) 'f' ) )
			{
				byte_value = (uint8_t) ( character_value - (uint16_t) 'a' + 10 );
			}
			else
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
				 "%s: unsupported character value: 0x04%" PRIx16 " at index: %d.",
				 function,
				 character_value,
				 safe_utf16_string_index );

				return( -1 );
			}
			value_64bit += byte_value;

			safe_utf16_string_index++;
		}
	}
	else
	{
		if( ( string_format_flags & LIBFVALUE_INTEGER_FORMAT_FLAG_SIGNED ) != 0 )
		{
			character_value = utf16_string[ safe_utf16_string_index ];

			/* In the maximum possible string one character is substituted for the sign
			 */
			if( character_value == (uint16_t) '-' )
			{
				safe_utf16_string_index++;

				sign = -1;
			}
			else if( character_value == (uint16_t) '+' )
			{
				safe_utf16_string_index++;
			}
		}
		while( safe_utf16_string_index < utf16_string_length )
		{
			character_value = utf16_string[ safe_utf16_string_index ];

			if( character_value == 0 )
			{
				break;
			}
			if( safe_utf16_string_index > (size_t) maximum_string_index )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_LARGE,
				 "%s: string too large.",
				 function );

				return( -1 );
			}
			value_64bit *= 10;

			if( ( character_value < (uint16_t) '0' )
			 || ( character_value > (uint16_t) '9' ) )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
				 "%s: unsupported character value: 0x04%" PRIx16 " at index: %d.",
				 function,
				 character_value,
				 safe_utf16_string_index );

				return( -1 );
			}
			character_value = (uint8_t) ( character_value - (uint16_t) '0' );

			value_64bit += character_value;

			safe_utf16_string_index++;
		}
		if( ( sign == -1 )
		 && ( value_64bit != 0 ) )
		{
			value_64bit = ~( value_64bit - 1 );
		}
	}
	*utf16_string_index = safe_utf16_string_index;
	*integer_value      = value_64bit;

	return( 1 );
}

/* Copies an UTF-32 encoded string of an integer value
 * The integer value size is in bits
 * Returns 1 if successful or -1 on error
 */
int libfvalue_utf32_string_copy_from_integer(
     uint32_t *utf32_string,
     size_t utf32_string_size,
     uint64_t integer_value,
     size_t integer_value_size,
     uint32_t string_format_flags,
     libcerror_error_t **error )
{
	static char *function     = "libfvalue_utf32_string_copy_from_integer";
	size_t utf32_string_index = 0;

	if( libfvalue_utf32_string_with_index_copy_from_integer(
	     utf32_string,
	     utf32_string_size,
	     &utf32_string_index,
	     integer_value,
	     integer_value_size,
	     string_format_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy integer value to UTF-32 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Copies an UTF-32 encoded string of an integer value
 * The integer value size is in bits
 * Returns 1 if successful or -1 on error
 */
int libfvalue_utf32_string_with_index_copy_from_integer(
     uint32_t *utf32_string,
     size_t utf32_string_size,
     size_t *utf32_string_index,
     uint64_t integer_value,
     size_t integer_value_size,
     uint32_t string_format_flags,
     libcerror_error_t **error )
{
	static char *function          = "libfvalue_utf32_string_with_index_copy_from_integer";
	size_t safe_utf32_string_index = 0;
	uint64_t divider               = 0;
	uint32_t string_format_type    = 0;
	uint32_t supported_flags       = 0;
	uint8_t byte_value             = 0;
	uint8_t is_signed              = 0;
	uint8_t number_of_characters   = 0;
	int8_t bit_shift               = 0;

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
	if( *utf32_string_index >= utf32_string_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid UTF-32 string index value out of bounds.",
		 function );

		return( -1 );
	}
	safe_utf32_string_index = *utf32_string_index;

	if( ( integer_value_size != 8 )
	 && ( integer_value_size != 16 )
	 && ( integer_value_size != 32 )
	 && ( integer_value_size != 64 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported integer value size.",
		 function );

		return( -1 );
	}
	supported_flags = 0x000000ffUL
	                | LIBFVALUE_INTEGER_FORMAT_FLAG_SIGNED
	                | LIBFVALUE_INTEGER_FORMAT_FLAG_UNSIGNED
	                | LIBFVALUE_INTEGER_FORMAT_FLAG_NO_BASE_INDICATOR;

	if( ( string_format_flags & ~( supported_flags ) ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported string format flags: 0x%08" PRIx32 ".",
		 function,
		 string_format_flags );

		return( -1 );
	}
	string_format_type = string_format_flags & 0x000000ffUL;

	if( ( string_format_type != LIBFVALUE_INTEGER_FORMAT_TYPE_DECIMAL )
	 && ( string_format_type != LIBFVALUE_INTEGER_FORMAT_TYPE_HEXADECIMAL )
	 && ( string_format_type != LIBFVALUE_INTEGER_FORMAT_TYPE_BOOLEAN ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported string format type.",
		 function );

		return( -1 );
	}
	if( string_format_type == LIBFVALUE_INTEGER_FORMAT_TYPE_BOOLEAN )
	{
		if( integer_value == 0 )
		{
			number_of_characters = 6;
		}
		else
		{
			number_of_characters = 5;
		}
	}
	else if( string_format_type == LIBFVALUE_INTEGER_FORMAT_TYPE_HEXADECIMAL )
	{
		number_of_characters = (uint8_t) ( integer_value_size >> 2 ) + 1;

		if( ( string_format_flags & LIBFVALUE_INTEGER_FORMAT_FLAG_NO_BASE_INDICATOR ) == 0 )
		{
			number_of_characters += 2;
		}
	}
	else
	{
		/* The string is at least a single digit with an end of string character
		 */
		number_of_characters = 2;

		bit_shift = (uint8_t) ( integer_value_size - 1 );

		if( ( string_format_flags & LIBFVALUE_INTEGER_FORMAT_FLAG_SIGNED ) != 0 )
		{
			is_signed = (uint8_t) ( integer_value >> bit_shift );

			if( is_signed != 0 )
			{
				number_of_characters += 1;

				integer_value &= ~( (uint64_t) 1 << bit_shift );

				if( integer_value == 0 )
				{
					integer_value |= (uint64_t) 1 << bit_shift;
				}
			}
		}
		divider = 1;

		while( ( integer_value / divider ) >= 10 )
		{
			divider *= 10;

			number_of_characters += 1;
		}
	}
	if( ( number_of_characters > utf32_string_size )
	 || ( safe_utf32_string_index > ( utf32_string_size - number_of_characters ) ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: UTF-32 string size too small.",
		 function );

		return( -1 );
	}
	if( string_format_type == LIBFVALUE_INTEGER_FORMAT_TYPE_BOOLEAN )
	{
		if( integer_value == 0 )
		{
			utf32_string[ safe_utf32_string_index++ ] = (uint32_t) 'f';
			utf32_string[ safe_utf32_string_index++ ] = (uint32_t) 'a';
			utf32_string[ safe_utf32_string_index++ ] = (uint32_t) 'l';
			utf32_string[ safe_utf32_string_index++ ] = (uint32_t) 's';
			utf32_string[ safe_utf32_string_index++ ] = (uint32_t) 'e';
		}
		else
		{
			utf32_string[ safe_utf32_string_index++ ] = (uint32_t) 't';
			utf32_string[ safe_utf32_string_index++ ] = (uint32_t) 'r';
			utf32_string[ safe_utf32_string_index++ ] = (uint32_t) 'u';
			utf32_string[ safe_utf32_string_index++ ] = (uint32_t) 'e';
		}
	}
	else if( string_format_type == LIBFVALUE_INTEGER_FORMAT_TYPE_HEXADECIMAL )
	{
		if( ( string_format_flags & LIBFVALUE_INTEGER_FORMAT_FLAG_NO_BASE_INDICATOR ) == 0 )
		{
			utf32_string[ safe_utf32_string_index++ ] = (uint32_t) '0';
			utf32_string[ safe_utf32_string_index++ ] = (uint32_t) 'x';
		}
		bit_shift = (uint8_t) ( integer_value_size - 4 );

		do
		{
			byte_value = (uint32_t) ( ( integer_value >> bit_shift ) & 0x0f );

			if( byte_value <= 9 )
			{
				utf32_string[ safe_utf32_string_index++ ] = (uint32_t) '0' + byte_value;
			}
			else
			{
				utf32_string[ safe_utf32_string_index++ ] = (uint32_t) 'a' + byte_value - 10;
			}
			bit_shift -= 4;
		}
		while( bit_shift >= 0 );
	}
	else
	{
		if( is_signed != 0 )
		{
			utf32_string[ safe_utf32_string_index++ ] = (uint32_t) '-';
		}
		while( divider > 1 )
		{
			utf32_string[ safe_utf32_string_index++ ] = (uint32_t) '0' + (uint32_t) ( integer_value / divider );

			integer_value %= divider;

			divider /= 10;
		}
		utf32_string[ safe_utf32_string_index++ ] = (uint32_t) '0' + (uint32_t) ( integer_value / divider );
	}
	utf32_string[ safe_utf32_string_index++ ] = 0;

	*utf32_string_index = safe_utf32_string_index;

	return( 1 );
}

/* Copies an UTF-32 encoded string to an integer value
 * The integer value size is in bits
 * Returns 1 if successful or -1 on error
 */
int libfvalue_utf32_string_copy_to_integer(
     const uint32_t *utf32_string,
     size_t utf32_string_length,
     uint64_t *integer_value,
     size_t integer_value_size,
     uint32_t string_format_flags,
     libcerror_error_t **error )
{
	static char *function     = "libfvalue_utf32_string_copy_to_integer";
	size_t utf32_string_index = 0;

	if( libfvalue_utf32_string_with_index_copy_to_integer(
	     utf32_string,
	     utf32_string_length,
	     &utf32_string_index,
	     integer_value,
	     integer_value_size,
	     string_format_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy UTF-32 string to integer value.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Copies an UTF-32 encoded string to an integer value
 * The integer value size is in bits
 * Returns 1 if successful or -1 on error
 */
int libfvalue_utf32_string_with_index_copy_to_integer(
     const uint32_t *utf32_string,
     size_t utf32_string_length,
     size_t *utf32_string_index,
     uint64_t *integer_value,
     size_t integer_value_size,
     uint32_t string_format_flags,
     libcerror_error_t **error )
{
	static char *function          = "libfvalue_utf32_string_with_index_copy_to_integer";
	size_t maximum_string_index    = 0;
	size_t safe_utf32_string_index = 0;
	uint64_t divider               = 0;
	uint64_t value_64bit           = 0;
	uint32_t string_format_type    = 0;
	uint32_t supported_flags       = 0;
	uint8_t byte_value             = 0;
	uint8_t character_value        = 0;
	int8_t bit_shift               = 0;
	int8_t sign                    = 1;

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
	if( utf32_string_length > (size_t) SSIZE_MAX )
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
	if( *utf32_string_index >= utf32_string_length )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid UTF-32 string index value out of bounds.",
		 function );

		return( -1 );
	}
	safe_utf32_string_index = *utf32_string_index;

	if( integer_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid integer value.",
		 function );

		return( -1 );
	}
	if( ( integer_value_size != 8 )
	 && ( integer_value_size != 16 )
	 && ( integer_value_size != 32 )
	 && ( integer_value_size != 64 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported integer value size.",
		 function );

		return( -1 );
	}
	supported_flags = 0x000000ffUL
	                | LIBFVALUE_INTEGER_FORMAT_FLAG_SIGNED
	                | LIBFVALUE_INTEGER_FORMAT_FLAG_UNSIGNED
	                | LIBFVALUE_INTEGER_FORMAT_FLAG_NO_BASE_INDICATOR;

	if( ( string_format_flags & ~( supported_flags ) ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported string format flags: 0x%08" PRIx32 ".",
		 function,
		 string_format_flags );

		return( -1 );
	}
	string_format_type = string_format_flags & 0x000000ffUL;

	if( ( string_format_type != LIBFVALUE_INTEGER_FORMAT_TYPE_DECIMAL )
	 && ( string_format_type != LIBFVALUE_INTEGER_FORMAT_TYPE_HEXADECIMAL )
	 && ( string_format_type != LIBFVALUE_INTEGER_FORMAT_TYPE_BOOLEAN ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported string format type.",
		 function );

		return( -1 );
	}
	if( string_format_type == LIBFVALUE_INTEGER_FORMAT_TYPE_BOOLEAN )
	{
		if( integer_value == 0 )
		{
			maximum_string_index = 5;
		}
		else
		{
			maximum_string_index = 4;
		}
	}
	else if( string_format_type == LIBFVALUE_INTEGER_FORMAT_TYPE_HEXADECIMAL )
	{
		maximum_string_index = (size_t) ( integer_value_size >> 2 );

		if( ( string_format_flags & LIBFVALUE_INTEGER_FORMAT_FLAG_NO_BASE_INDICATOR ) == 0 )
		{
			maximum_string_index += 2;
		}
	}
	else
	{
		/* The string is at least a single digit with an end of string character
		 */
		maximum_string_index = 2;

		bit_shift = (uint8_t) ( integer_value_size - 1 );

		divider = 1;

		value_64bit = ~( ( ~( (uint64_t) 1 << bit_shift ) >> bit_shift ) << bit_shift );

		while( ( value_64bit / divider ) >= 10 )
		{
			divider *= 10;

			maximum_string_index += 1;
		}
	}
	maximum_string_index += safe_utf32_string_index;

	if( maximum_string_index > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid maximum string index value exceeds maximum.",
		 function );

		return( -1 );
	}
	value_64bit = 0;

	if( string_format_type == LIBFVALUE_INTEGER_FORMAT_TYPE_BOOLEAN )
	{
/* TODO */
	}
	else if( string_format_type == LIBFVALUE_INTEGER_FORMAT_TYPE_HEXADECIMAL )
	{
		if( ( string_format_flags & LIBFVALUE_INTEGER_FORMAT_FLAG_NO_BASE_INDICATOR ) == 0 )
		{
			character_value = utf32_string[ safe_utf32_string_index++ ];

			if( character_value != (uint32_t) '0' )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
				 "%s: unsupported character value: 0x08%" PRIx32 " at index: %d.",
				 function,
				 character_value,
				 safe_utf32_string_index );

				return( -1 );
			}
			character_value = utf32_string[ safe_utf32_string_index++ ];

			if( character_value != (uint32_t) 'x' )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
				 "%s: unsupported character value: 0x08%" PRIx32 " at index: %d.",
				 function,
				 character_value,
				 safe_utf32_string_index );

				return( -1 );
			}
		}
		while( safe_utf32_string_index < utf32_string_length )
		{
			character_value = utf32_string[ safe_utf32_string_index ];

			if( character_value == 0 )
			{
				break;
			}
			if( safe_utf32_string_index > (size_t) maximum_string_index )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_LARGE,
				 "%s: string too large.",
				 function );

				return( -1 );
			}
			value_64bit <<= 4;

			if( ( character_value >= (uint32_t) '0' )
			 && ( character_value <= (uint32_t) '9' ) )
			{
				byte_value = (uint8_t) ( character_value - (uint32_t) '0' );
			}
			else if( ( character_value >= (uint32_t) 'A' )
			      && ( character_value <= (uint32_t) 'F' ) )
			{
				byte_value = (uint8_t) ( character_value - (uint32_t) 'A' + 10 );
			}
			else if( ( character_value >= (uint32_t) 'a' )
			      && ( character_value <= (uint32_t) 'f' ) )
			{
				byte_value = (uint8_t) ( character_value - (uint32_t) 'a' + 10 );
			}
			else
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
				 "%s: unsupported character value: 0x08%" PRIx32 " at index: %d.",
				 function,
				 character_value,
				 safe_utf32_string_index );

				return( -1 );
			}
			value_64bit += byte_value;

			safe_utf32_string_index++;
		}
	}
	else
	{
		if( ( string_format_flags & LIBFVALUE_INTEGER_FORMAT_FLAG_SIGNED ) != 0 )
		{
			character_value = utf32_string[ safe_utf32_string_index ];

			/* In the maximum possible string one character is substituted for the sign
			 */
			if( character_value == (uint32_t) '-' )
			{
				safe_utf32_string_index++;

				sign = -1;
			}
			else if( character_value == (uint32_t) '+' )
			{
				safe_utf32_string_index++;
			}
		}
		while( safe_utf32_string_index < utf32_string_length )
		{
			character_value = utf32_string[ safe_utf32_string_index ];

			if( character_value == 0 )
			{
				break;
			}
			if( safe_utf32_string_index > (size_t) maximum_string_index )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_LARGE,
				 "%s: string too large.",
				 function );

				return( -1 );
			}
			value_64bit *= 10;

			if( ( character_value < (uint32_t) '0' )
			 || ( character_value > (uint32_t) '9' ) )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
				 "%s: unsupported character value: 0x08%" PRIx32 " at index: %d.",
				 function,
				 character_value,
				 safe_utf32_string_index );

				return( -1 );
			}
			character_value = (uint8_t) ( character_value - (uint32_t) '0' );

			value_64bit += character_value;

			safe_utf32_string_index++;
		}
		if( ( sign == -1 )
		 && ( value_64bit != 0 ) )
		{
			value_64bit = ~( value_64bit - 1 );
		}
	}
	*utf32_string_index = safe_utf32_string_index;
	*integer_value      = value_64bit;

	return( 1 );
}

