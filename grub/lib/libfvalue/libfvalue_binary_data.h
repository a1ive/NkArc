/*
 * Binary data value functions
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

#if !defined( _LIBFVALUE_BINARY_DATA_H )
#define _LIBFVALUE_BINARY_DATA_H

#include <libyal_wrap.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfvalue_binary_data libfvalue_binary_data_t;

struct libfvalue_binary_data
{
	/* The data
	 */
	const uint8_t *data;

	/* The data size
	 */
	size_t data_size;
};

int libfvalue_binary_data_initialize(
     libfvalue_binary_data_t **binary_data,
     libcerror_error_t **error );

int libfvalue_binary_data_free(
     libfvalue_binary_data_t **binary_data,
     libcerror_error_t **error );

int libfvalue_binary_data_clone(
     libfvalue_binary_data_t **destination_binary_data,
     libfvalue_binary_data_t *source_binary_data,
     libcerror_error_t **error );

int libfvalue_binary_data_copy_from_byte_stream(
     libfvalue_binary_data_t *binary_data,
     const uint8_t *byte_stream,
     size_t byte_stream_size,
     int encoding,
     libcerror_error_t **error );

int libfvalue_binary_data_get_utf8_string_size(
     libfvalue_binary_data_t *binary_data,
     size_t *utf8_string_size,
     uint32_t string_format_flags,
     libcerror_error_t **error );

int libfvalue_binary_data_copy_to_utf8_string_with_index(
     libfvalue_binary_data_t *binary_data,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     size_t *utf8_string_index,
     uint32_t string_format_flags,
     libcerror_error_t **error );

int libfvalue_binary_data_get_utf16_string_size(
     libfvalue_binary_data_t *binary_data,
     size_t *utf16_string_size,
     uint32_t string_format_flags,
     libcerror_error_t **error );

int libfvalue_binary_data_copy_to_utf16_string_with_index(
     libfvalue_binary_data_t *binary_data,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     size_t *utf16_string_index,
     uint32_t string_format_flags,
     libcerror_error_t **error );

int libfvalue_binary_data_get_utf32_string_size(
     libfvalue_binary_data_t *binary_data,
     size_t *utf32_string_size,
     uint32_t string_format_flags,
     libcerror_error_t **error );

int libfvalue_binary_data_copy_to_utf32_string_with_index(
     libfvalue_binary_data_t *binary_data,
     uint32_t *utf32_string,
     size_t utf32_string_size,
     size_t *utf32_string_index,
     uint32_t string_format_flags,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFVALUE_BINARY_DATA_H ) */

