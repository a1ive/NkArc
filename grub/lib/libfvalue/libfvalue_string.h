/*
 * String value functions
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

#if !defined( _LIBVALUE_STRING_H )
#define _LIBVALUE_STRING_H

#include <libyal_wrap.h>

#include "libfvalue_definitions.h"
#include "libfvalue_extern.h"
#include "libfvalue_types.h"

#if defined( _cplusplus )
extern "C" {
#endif

typedef struct libfvalue_string libfvalue_string_t;

struct libfvalue_string
{
	/* The data
	 */
	uint8_t *data;

	/* The data size
	 */
	size_t data_size;

	/* The codepage
	 */
	int codepage;

	/* The flags
	 */
	uint8_t flags;
};

int libfvalue_string_initialize(
     libfvalue_string_t **string,
     libcerror_error_t **error );

int libfvalue_string_free(
     libfvalue_string_t **string,
     libcerror_error_t **error );

int libfvalue_string_clone(
     libfvalue_string_t **destination_string,
     libfvalue_string_t *source_string,
     libcerror_error_t **error );

int libfvalue_string_copy_from_byte_stream(
     libfvalue_string_t *string,
     const uint8_t *byte_stream,
     size_t byte_stream_size,
     int encoding,
     libcerror_error_t **error );

int libfvalue_string_copy_from_utf8_string_with_index(
     libfvalue_string_t *string,
     const uint8_t *utf8_string,
     size_t utf8_string_size,
     size_t *utf8_string_index,
     uint32_t string_format_flags,
     libcerror_error_t **error );

int libfvalue_string_get_utf8_string_size(
     libfvalue_string_t *string,
     size_t *utf8_string_size,
     uint32_t string_format_flags,
     libcerror_error_t **error );

int libfvalue_string_copy_to_utf8_string_with_index(
     libfvalue_string_t *string,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     size_t *utf8_string_index,
     uint32_t string_format_flags,
     libcerror_error_t **error );

int libfvalue_string_copy_from_utf16_string_with_index(
     libfvalue_string_t *string,
     const uint16_t *utf16_string,
     size_t utf16_string_size,
     size_t *utf16_string_index,
     uint32_t string_format_flags,
     libcerror_error_t **error );

int libfvalue_string_get_utf16_string_size(
     libfvalue_string_t *string,
     size_t *utf16_string_size,
     uint32_t string_format_flags,
     libcerror_error_t **error );

int libfvalue_string_copy_to_utf16_string_with_index(
     libfvalue_string_t *string,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     size_t *utf16_string_index,
     uint32_t string_format_flags,
     libcerror_error_t **error );

int libfvalue_string_copy_from_utf32_string_with_index(
     libfvalue_string_t *string,
     const uint32_t *utf32_string,
     size_t utf32_string_size,
     size_t *utf32_string_index,
     uint32_t string_format_flags,
     libcerror_error_t **error );

int libfvalue_string_get_utf32_string_size(
     libfvalue_string_t *string,
     size_t *utf32_string_size,
     uint32_t string_format_flags,
     libcerror_error_t **error );

int libfvalue_string_copy_to_utf32_string_with_index(
     libfvalue_string_t *string,
     uint32_t *utf32_string,
     size_t utf32_string_size,
     size_t *utf32_string_index,
     uint32_t string_format_flags,
     libcerror_error_t **error );

LIBFVALUE_EXTERN \
int libfvalue_utf8_string_split(
     const uint8_t *utf8_string,
     size_t utf8_string_size,
     uint8_t delimiter,
     libfvalue_split_utf8_string_t **split_string,
     libcerror_error_t **error );

LIBFVALUE_EXTERN \
int libfvalue_utf16_string_split(
     const uint16_t *utf16_string,
     size_t utf16_string_size,
     uint16_t delimiter,
     libfvalue_split_utf16_string_t **split_string,
     libcerror_error_t **error );

#if defined( _cplusplus )
}
#endif

#endif /* !defined( _LIBVALUE_STRING_H ) */

