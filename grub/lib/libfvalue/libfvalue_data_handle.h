/*
 * Data handle functions
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

#if !defined( _LIBFVALUE_INTERNAL_DATA_HANDLE_H )
#define _LIBFVALUE_INTERNAL_DATA_HANDLE_H

#include <libyal_wrap.h>

#include "libfvalue_extern.h"
#include "libfvalue_libcdata.h"
#include "libfvalue_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfvalue_internal_data_handle libfvalue_internal_data_handle_t;

struct libfvalue_internal_data_handle
{
	/* The data
	 */
	uint8_t *data;

	/* The data size
	 */
	size_t data_size;

	/* The encoding
	 */
	int encoding;

	/* The data flags
	 */
	uint32_t data_flags;

	/* The value entries array
	 */
	libcdata_array_t *value_entries;

	/* The read value entries function
	 */
	int (*read_value_entries)(
	       libfvalue_data_handle_t *data_handle,
	       const uint8_t *data,
	       size_t data_size,
	       int encoding,
	       uint32_t data_flags,
	       libcerror_error_t **error );

	/* The flags
	 */
	uint8_t flags;
};

LIBFVALUE_EXTERN \
int libfvalue_data_handle_initialize(
     libfvalue_data_handle_t **data_handle,
     int (*read_value_entries)(
            libfvalue_data_handle_t *data_handle,
            const uint8_t *data,
            size_t data_size,
            int encoding,
            uint32_t data_flags,
            libcerror_error_t **error ),
     libcerror_error_t **error );

LIBFVALUE_EXTERN \
int libfvalue_data_handle_free(
     libfvalue_data_handle_t **data_handle,
     libcerror_error_t **error );

LIBFVALUE_EXTERN \
int libfvalue_data_handle_clone(
     libfvalue_data_handle_t **destination_data_handle,
     libfvalue_data_handle_t *source_data_handle,
     libcerror_error_t **error );

LIBFVALUE_EXTERN \
int libfvalue_data_handle_clear(
     libfvalue_data_handle_t *data_handle,
     libcerror_error_t **error );

LIBFVALUE_EXTERN \
int libfvalue_data_handle_get_data(
     libfvalue_data_handle_t *data_handle,
     uint8_t **data,
     size_t *data_size,
     int *encoding,
     libcerror_error_t **error );

LIBFVALUE_EXTERN \
int libfvalue_data_handle_set_data(
     libfvalue_data_handle_t *data_handle,
     const uint8_t *data,
     size_t data_size,
     int encoding,
     uint8_t flags,
     libcerror_error_t **error );

LIBFVALUE_EXTERN \
int libfvalue_data_handle_get_data_flags(
     libfvalue_data_handle_t *data_handle,
     uint32_t *data_flags,
     libcerror_error_t **error );

LIBFVALUE_EXTERN \
int libfvalue_data_handle_set_data_flags(
     libfvalue_data_handle_t *data_handle,
     uint32_t data_flags,
     libcerror_error_t **error );

LIBFVALUE_EXTERN \
int libfvalue_data_handle_get_number_of_value_entries(
     libfvalue_data_handle_t *data_handle,
     int *number_of_value_entries,
     libcerror_error_t **error );

LIBFVALUE_EXTERN \
int libfvalue_data_handle_get_value_entry(
     libfvalue_data_handle_t *data_handle,
     int value_entry_index,
     size_t *value_entry_offset,
     size_t *value_entry_size,
     libcerror_error_t **error );

LIBFVALUE_EXTERN \
int libfvalue_data_handle_set_value_entry(
     libfvalue_data_handle_t *data_handle,
     int value_entry_index,
     size_t value_entry_offset,
     size_t value_entry_size,
     libcerror_error_t **error );

LIBFVALUE_EXTERN \
int libfvalue_data_handle_append_value_entry(
     libfvalue_data_handle_t *data_handle,
     int *value_entry_index,
     size_t value_entry_offset,
     size_t value_entry_size,
     libcerror_error_t **error );

LIBFVALUE_EXTERN \
int libfvalue_data_handle_get_value_entry_data(
     libfvalue_data_handle_t *data_handle,
     int value_entry_index,
     uint8_t **value_entry_data,
     size_t *value_entry_data_size,
     int *encoding,
     libcerror_error_t **error );

LIBFVALUE_EXTERN \
int libfvalue_data_handle_set_value_entry_data(
     libfvalue_data_handle_t *data_handle,
     int value_entry_index,
     const uint8_t *value_entry_data,
     size_t value_entry_data_size,
     int encoding,
     libcerror_error_t **error );

LIBFVALUE_EXTERN \
int libfvalue_data_handle_append_value_entry_data(
     libfvalue_data_handle_t *data_handle,
     int *value_entry_index,
     const uint8_t *value_entry_data,
     size_t value_entry_data_size,
     int encoding,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFVALUE_INTERNAL_DATA_HANDLE_H ) */

