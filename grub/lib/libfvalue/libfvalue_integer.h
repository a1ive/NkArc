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

#if !defined( _LIBFVALUE_INTEGER_H )
#define _LIBFVALUE_INTEGER_H

#include <libyal_wrap.h>

#include "libfvalue_extern.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfvalue_integer libfvalue_integer_t;

struct libfvalue_integer
{
	/* The value
	 */
	uint64_t value;

	/* The value size
	 */
	size_t value_size;
};

int libfvalue_integer_initialize(
     libfvalue_integer_t **integer,
     libcerror_error_t **error );

int libfvalue_integer_free(
     libfvalue_integer_t **integer,
     libcerror_error_t **error );

int libfvalue_integer_clone(
     libfvalue_integer_t **destination_integer,
     libfvalue_integer_t *source_integer,
     libcerror_error_t **error );

int libfvalue_integer_copy_from_byte_stream(
     libfvalue_integer_t *integer,
     const uint8_t *byte_stream,
     size_t byte_stream_size,
     int encoding,
     libcerror_error_t **error );

int libfvalue_integer_copy_from_integer(
     libfvalue_integer_t *integer,
     uint64_t integer_value,
     size_t integer_value_size,
     libcerror_error_t **error );

int libfvalue_integer_copy_to_integer(
     libfvalue_integer_t *integer,
     uint64_t *integer_value,
     size_t *integer_value_size,
     libcerror_error_t **error );

int libfvalue_integer_get_string_size(
     libfvalue_integer_t *integer,
     size_t *string_size,
     uint32_t string_format_flags,
     libcerror_error_t **error );

int libfvalue_integer_copy_from_utf8_string_with_index(
     libfvalue_integer_t *integer,
     uint8_t *utf8_string,
     size_t utf8_string_length,
     size_t *utf8_string_index,
     uint32_t string_format_flags,
     libcerror_error_t **error );

int libfvalue_integer_copy_to_utf8_string_with_index(
     libfvalue_integer_t *integer,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     size_t *utf8_string_index,
     uint32_t string_format_flags,
     libcerror_error_t **error );

int libfvalue_integer_copy_from_utf16_string_with_index(
     libfvalue_integer_t *integer,
     uint16_t *utf16_string,
     size_t utf16_string_length,
     size_t *utf16_string_index,
     uint32_t string_format_flags,
     libcerror_error_t **error );

int libfvalue_integer_copy_to_utf16_string_with_index(
     libfvalue_integer_t *integer,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     size_t *utf16_string_index,
     uint32_t string_format_flags,
     libcerror_error_t **error );

int libfvalue_integer_copy_from_utf32_string_with_index(
     libfvalue_integer_t *integer,
     uint32_t *utf32_string,
     size_t utf32_string_length,
     size_t *utf32_string_index,
     uint32_t string_format_flags,
     libcerror_error_t **error );

int libfvalue_integer_copy_to_utf32_string_with_index(
     libfvalue_integer_t *integer,
     uint32_t *utf32_string,
     size_t utf32_string_size,
     size_t *utf32_string_index,
     uint32_t string_format_flags,
     libcerror_error_t **error );

LIBFVALUE_EXTERN \
int libfvalue_string_size_from_integer(
     size_t *string_size,
     uint64_t integer_value,
     size_t integer_value_size,
     uint32_t string_format_flags,
     libcerror_error_t **error );

LIBFVALUE_EXTERN \
int libfvalue_utf8_string_copy_from_integer(
     uint8_t *utf8_string,
     size_t utf8_string_size,
     uint64_t integer_value,
     size_t integer_value_size,
     uint32_t string_format_flags,
     libcerror_error_t **error );

LIBFVALUE_EXTERN \
int libfvalue_utf8_string_with_index_copy_from_integer(
     uint8_t *utf8_string,
     size_t utf8_string_size,
     size_t *utf8_string_index,
     uint64_t integer_value,
     size_t integer_value_size,
     uint32_t string_format_flags,
     libcerror_error_t **error );

LIBFVALUE_EXTERN \
int libfvalue_utf8_string_copy_to_integer(
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     uint64_t *integer_value,
     size_t integer_value_size,
     uint32_t string_format_flags,
     libcerror_error_t **error );

LIBFVALUE_EXTERN \
int libfvalue_utf8_string_with_index_copy_to_integer(
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     size_t *utf8_string_index,
     uint64_t *integer_value,
     size_t integer_value_size,
     uint32_t string_format_flags,
     libcerror_error_t **error );

LIBFVALUE_EXTERN \
int libfvalue_utf16_string_copy_from_integer(
     uint16_t *utf16_string,
     size_t utf16_string_size,
     uint64_t integer_value,
     size_t integer_value_size,
     uint32_t string_format_flags,
     libcerror_error_t **error );

LIBFVALUE_EXTERN \
int libfvalue_utf16_string_with_index_copy_from_integer(
     uint16_t *utf16_string,
     size_t utf16_string_size,
     size_t *utf16_string_index,
     uint64_t integer_value,
     size_t integer_value_size,
     uint32_t string_format_flags,
     libcerror_error_t **error );

LIBFVALUE_EXTERN \
int libfvalue_utf16_string_copy_to_integer(
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     uint64_t *integer_value,
     size_t integer_value_size,
     uint32_t string_format_flags,
     libcerror_error_t **error );

LIBFVALUE_EXTERN \
int libfvalue_utf16_string_with_index_copy_to_integer(
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     size_t *utf16_string_index,
     uint64_t *integer_value,
     size_t integer_value_size,
     uint32_t string_format_flags,
     libcerror_error_t **error );

LIBFVALUE_EXTERN \
int libfvalue_utf32_string_copy_from_integer(
     uint32_t *utf32_string,
     size_t utf32_string_size,
     uint64_t integer_value,
     size_t integer_value_size,
     uint32_t string_format_flags,
     libcerror_error_t **error );

LIBFVALUE_EXTERN \
int libfvalue_utf32_string_with_index_copy_from_integer(
     uint32_t *utf32_string,
     size_t utf32_string_size,
     size_t *utf32_string_index,
     uint64_t integer_value,
     size_t integer_value_size,
     uint32_t string_format_flags,
     libcerror_error_t **error );

LIBFVALUE_EXTERN \
int libfvalue_utf32_string_copy_to_integer(
     const uint32_t *utf32_string,
     size_t utf32_string_length,
     uint64_t *integer_value,
     size_t integer_value_size,
     uint32_t string_format_flags,
     libcerror_error_t **error );

LIBFVALUE_EXTERN \
int libfvalue_utf32_string_with_index_copy_to_integer(
     const uint32_t *utf32_string,
     size_t utf32_string_length,
     size_t *utf32_string_index,
     uint64_t *integer_value,
     size_t integer_value_size,
     uint32_t string_format_flags,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFVALUE_INTEGER_H ) */

