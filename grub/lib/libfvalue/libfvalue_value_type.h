/*
 * Value type functions
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

#if !defined( _LIBFVALUE_INTERNAL_VALUE_TYPE_H )
#define _LIBFVALUE_INTERNAL_VALUE_TYPE_H

#include <libyal_wrap.h>

#include "libfvalue_extern.h"
#include "libfvalue_libfdatetime.h"
#include "libfvalue_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

LIBFVALUE_EXTERN \
int libfvalue_value_type_initialize(
     libfvalue_value_t **value,
     int type,
     libcerror_error_t **error );

LIBFVALUE_EXTERN \
int libfvalue_value_type_initialize_with_data_handle(
     libfvalue_value_t **value,
     int type,
     libfvalue_data_handle_t *data_handle,
     uint8_t flags,
     libcerror_error_t **error );

ssize_t libfvalue_value_type_get_string_size(
         libfvalue_value_t *value,
         const uint8_t *data,
         size_t data_size,
         libcerror_error_t **error );

LIBFVALUE_EXTERN \
ssize_t libfvalue_value_type_set_data_string(
         libfvalue_value_t *value,
         const uint8_t *data,
         size_t data_size,
         int encoding,
         uint8_t flags,
         libcerror_error_t **error );

LIBFVALUE_EXTERN \
ssize_t libfvalue_value_type_append_data_string(
         libfvalue_value_t *value,
         const uint8_t *data,
         size_t data_size,
         int encoding,
         libcerror_error_t **error );

LIBFVALUE_EXTERN \
ssize_t libfvalue_value_type_set_data_strings_array(
         libfvalue_value_t *value,
         const uint8_t *data,
         size_t data_size,
         int encoding,
         libcerror_error_t **error );

#if defined( HAVE_LIBFDATETIME ) || defined( HAVE_LOCAL_LIBFDATETIME )

int libfvalue_value_type_posix_time_copy_from_byte_stream(
     libfdatetime_posix_time_t *posix_time,
     const uint8_t *byte_stream,
     size_t byte_stream_size,
     int encoding,
     libcerror_error_t **error );

#endif /* defined( HAVE_LIBFDATETIME ) || defined( HAVE_LOCAL_LIBFDATETIME ) */

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFVALUE_INTERNAL_VALUE_TYPE_H ) */

