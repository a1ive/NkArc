/*
 * Value functions
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

#if !defined( _LIBFVALUE_INTERNAL_VALUE_H )
#define _LIBFVALUE_INTERNAL_VALUE_H

#include <libyal_wrap.h>

#include <stdio.h>

#include "libfvalue_extern.h"
#include "libfvalue_libcdata.h"
#include "libfvalue_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfvalue_internal_value libfvalue_internal_value_t;

struct libfvalue_internal_value
{
	/* The type
	 */
	int type;

	/* The type string
	 */
	const char *type_string;

	/* The type description
	 */
	const char *type_description;

	/* The identifier
	 */
	uint8_t *identifier;

	/* The identifier size
	 */
	size_t identifier_size;

	/* The data handle
	 */
	libfvalue_data_handle_t *data_handle;

	/* The value instances array
	 */
	libcdata_array_t *value_instances;

	/* The initialize instance function
	 */
	int (*initialize_instance)(
	      intptr_t **instance,
	      libcerror_error_t **error );

	/* The free instance function
	 */
	int (*free_instance)(
	      intptr_t **instance,
	      libcerror_error_t **error );

	/* The clone (duplicate) instance function
	 */
	int (*clone_instance)(
	      intptr_t **destination_instance,
	      intptr_t *source_instance,
	      libcerror_error_t **error );

	/* The instance copy from byte stream
	 */
	int (*copy_from_byte_stream)(
	      intptr_t *instance,
	      const uint8_t *byte_stream,
	      size_t byte_stream_size,
	      int encoding,
	      libcerror_error_t **error );

	/* The instance copy to byte stream
	 */
	int (*copy_to_byte_stream)(
	      intptr_t *instance,
	      uint8_t *byte_stream,
	      size_t byte_stream_size,
	      int encoding,
	      libcerror_error_t **error );

	/* The instance copy from integer function
	 */
	int (*copy_from_integer)(
	      intptr_t *instance,
	      uint64_t integer_value,
	      size_t integer_value_size,
	      libcerror_error_t **error );

	/* The instance copy to integer function
	 */
	int (*copy_to_integer)(
	      intptr_t *instance,
	      uint64_t *integer_value,
	      size_t *integer_value_size,
	      libcerror_error_t **error );

	/* The instance copy from floating point function
	 */
	int (*copy_from_floating_point)(
	      intptr_t *instance,
	      double floating_point_value,
	      size_t floating_point_value_size,
	      libcerror_error_t **error );

	/* The instance copy to floating point function
	 */
	int (*copy_to_floating_point)(
	      intptr_t *instance,
	      double *floating_point_value,
	      size_t *floating_point_value_size,
	      libcerror_error_t **error );

	/* The instance copy from UTF-8 string with index function
	 */
	int (*copy_from_utf8_string_with_index)(
	      intptr_t *instance,
	      const uint8_t *utf8_string,
	      size_t utf8_string_length,
	      size_t *utf8_string_index,
	      uint32_t string_format_flags,
	      libcerror_error_t **error );

	/* The instance get UTF-8 string size function
	 */
	int (*get_utf8_string_size)(
	      intptr_t *instance,
	      size_t *utf8_string_size,
	      uint32_t string_format_flags,
	      libcerror_error_t **error );

	/* The instance copy to UTF-8 string with index function
	 */
	int (*copy_to_utf8_string_with_index)(
	      intptr_t *instance,
	      uint8_t *utf8_string,
	      size_t utf8_string_size,
	      size_t *utf8_string_index,
	      uint32_t string_format_flags,
	      libcerror_error_t **error );

	/* The instance copy from UTF-16 string with index function
	 */
	int (*copy_from_utf16_string_with_index)(
	      intptr_t *instance,
	      const uint16_t *utf16_string,
	      size_t utf16_string_length,
	      size_t *utf16_string_index,
	      uint32_t string_format_flags,
	      libcerror_error_t **error );

	/* The instance get UTF-16 string size function
	 */
	int (*get_utf16_string_size)(
	      intptr_t *instance,
	      size_t *utf16_string_size,
	      uint32_t string_format_flags,
	      libcerror_error_t **error );

	/* The instance copy to UTF-16 string with index function
	 */
	int (*copy_to_utf16_string_with_index)(
	      intptr_t *instance,
	      uint16_t *utf16_string,
	      size_t utf16_string_size,
	      size_t *utf16_string_index,
	      uint32_t string_format_flags,
	      libcerror_error_t **error );

	/* The instance copy from UTF-32 string with index function
	 */
	int (*copy_from_utf32_string_with_index)(
	      intptr_t *instance,
	      const uint32_t *utf32_string,
	      size_t utf32_string_length,
	      size_t *utf32_string_index,
	      uint32_t string_format_flags,
	      libcerror_error_t **error );

	/* The instance get UTF-32 string size function
	 */
	int (*get_utf32_string_size)(
	      intptr_t *instance,
	      size_t *utf32_string_size,
	      uint32_t string_format_flags,
	      libcerror_error_t **error );

	/* The instance copy to UTF-32 string with index function
	 */
	int (*copy_to_utf32_string_with_index)(
	      intptr_t *instance,
	      uint32_t *utf32_string,
	      size_t utf32_string_size,
	      size_t *utf32_string_index,
	      uint32_t string_format_flags,
	      libcerror_error_t **error );

	/* The format flags
	 */
	uint32_t format_flags;

	/* The flags
	 */
	uint8_t flags;
};

LIBFVALUE_EXTERN \
int libfvalue_value_initialize(
     libfvalue_value_t **value,
     const char *type_string,
     const char *type_description,
     libfvalue_data_handle_t *data_handle,
     int (*initialize_instance)(
           intptr_t **instance,
           libcerror_error_t **error ),
     int (*free_instance)(
           intptr_t **instance,
           libcerror_error_t **error ),
     int (*clone_instance)(
           intptr_t **destination_instance,
           intptr_t *source_instance,
           libcerror_error_t **error ),
     int (*copy_from_byte_stream)(
           intptr_t *instance,
           const uint8_t *byte_stream,
           size_t byte_stream_size,
           int encoding,
           libcerror_error_t **error ),
     int (*copy_to_byte_stream)(
           intptr_t *instance,
           uint8_t *byte_stream,
           size_t byte_stream_size,
           int encoding,
           libcerror_error_t **error ),
     int (*copy_from_integer)(
           intptr_t *instance,
           uint64_t integer_value,
           size_t integer_value_size,
           libcerror_error_t **error ),
     int (*copy_to_integer)(
           intptr_t *instance,
           uint64_t *integer_value,
           size_t *integer_value_size,
           libcerror_error_t **error ),
     int (*copy_from_floating_point)(
           intptr_t *instance,
           double floating_point_value,
           size_t floating_point_value_size,
           libcerror_error_t **error ),
     int (*copy_to_floating_point)(
           intptr_t *instance,
           double *floating_point_value,
           size_t *floating_point_value_size,
           libcerror_error_t **error ),
     int (*copy_from_utf8_string_with_index)(
           intptr_t *instance,
           const uint8_t *utf8_string,
           size_t utf8_string_length,
           size_t *utf8_string_index,
           uint32_t string_format_flags,
           libcerror_error_t **error ),
     int (*get_utf8_string_size)(
           intptr_t *instance,
           size_t *utf8_string_size,
           uint32_t string_format_flags,
           libcerror_error_t **error ),
     int (*copy_to_utf8_string_with_index)(
           intptr_t *instance,
           uint8_t *utf8_string,
           size_t utf8_string_size,
           size_t *utf8_string_index,
           uint32_t string_format_flags,
           libcerror_error_t **error ),
     int (*copy_from_utf16_string_with_index)(
           intptr_t *instance,
           const uint16_t *utf16_string,
           size_t utf16_string_length,
           size_t *utf16_string_index,
           uint32_t string_format_flags,
           libcerror_error_t **error ),
     int (*get_utf16_string_size)(
           intptr_t *instance,
           size_t *utf16_string_size,
           uint32_t string_format_flags,
           libcerror_error_t **error ),
     int (*copy_to_utf16_string_with_index)(
           intptr_t *instance,
           uint16_t *utf16_string,
           size_t utf16_string_size,
           size_t *utf16_string_index,
           uint32_t string_format_flags,
           libcerror_error_t **error ),
     int (*copy_from_utf32_string_with_index)(
           intptr_t *instance,
           const uint32_t *utf32_string,
           size_t utf32_string_length,
           size_t *utf32_string_index,
           uint32_t string_format_flags,
           libcerror_error_t **error ),
     int (*get_utf32_string_size)(
           intptr_t *instance,
           size_t *utf32_string_size,
           uint32_t string_format_flags,
           libcerror_error_t **error ),
     int (*copy_to_utf32_string_with_index)(
           intptr_t *instance,
           uint32_t *utf32_string,
           size_t utf32_string_size,
           size_t *utf32_string_index,
           uint32_t string_format_flags,
           libcerror_error_t **error ),
     uint8_t flags,
     libcerror_error_t **error );

LIBFVALUE_EXTERN \
int libfvalue_value_free(
     libfvalue_value_t **value,
     libcerror_error_t **error );

LIBFVALUE_EXTERN \
int libfvalue_value_clone(
     libfvalue_value_t **destination_value,
     libfvalue_value_t *source_value,
     libcerror_error_t **error );

LIBFVALUE_EXTERN \
int libfvalue_value_clear(
     libfvalue_value_t *value,
     libcerror_error_t **error );

int libfvalue_compare_identifier(
     intptr_t *first_value,
     intptr_t *second_value,
     libcerror_error_t **error );

LIBFVALUE_EXTERN \
int libfvalue_value_get_type(
     libfvalue_value_t *value,
     int *value_type,
     libcerror_error_t **error );

LIBFVALUE_EXTERN \
int libfvalue_value_get_identifier(
     libfvalue_value_t *value,
     uint8_t **identifier,
     size_t *identifier_size,
     libcerror_error_t **error );

LIBFVALUE_EXTERN \
int libfvalue_value_set_identifier(
     libfvalue_value_t *value,
     const uint8_t *identifier,
     size_t identifier_size,
     uint8_t flags,
     libcerror_error_t **error );

LIBFVALUE_EXTERN \
int libfvalue_value_get_data_flags(
     libfvalue_value_t *value,
     uint32_t *data_flags,
     libcerror_error_t **error );

LIBFVALUE_EXTERN \
int libfvalue_value_set_data_flags(
     libfvalue_value_t *value,
     uint32_t data_flags,
     libcerror_error_t **error );

LIBFVALUE_EXTERN \
int libfvalue_value_has_data(
     libfvalue_value_t *value,
     libcerror_error_t **error );

LIBFVALUE_EXTERN \
int libfvalue_value_initialize_data(
     libfvalue_value_t *value,
     size_t data_size,
     libcerror_error_t **error );

LIBFVALUE_EXTERN \
int libfvalue_value_get_data_size(
     libfvalue_value_t *value,
     size_t *data_size,
     libcerror_error_t **error );

LIBFVALUE_EXTERN \
int libfvalue_value_get_data(
     libfvalue_value_t *value,
     uint8_t **data,
     size_t *data_size,
     int *encoding,
     libcerror_error_t **error );

LIBFVALUE_EXTERN \
int libfvalue_value_set_data(
     libfvalue_value_t *value,
     const uint8_t *data,
     size_t data_size,
     int encoding,
     uint8_t flags,
     libcerror_error_t **error );

LIBFVALUE_EXTERN \
int libfvalue_value_copy_data(
     libfvalue_value_t *value,
     uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

LIBFVALUE_EXTERN \
int libfvalue_value_get_format_flags(
     libfvalue_value_t *value,
     uint32_t *format_flags,
     libcerror_error_t **error );

LIBFVALUE_EXTERN \
int libfvalue_value_set_format_flags(
     libfvalue_value_t *value,
     uint32_t format_flags,
     libcerror_error_t **error );

/* Value instance functions
 */
int libfvalue_value_get_value_instance_by_index(
     libfvalue_value_t *value,
     int value_entry_index,
     intptr_t **value_instance,
     libcerror_error_t **error );

/* Value entry functions
 */
LIBFVALUE_EXTERN \
int libfvalue_value_get_number_of_value_entries(
     libfvalue_value_t *value,
     int *number_of_value_entries,
     libcerror_error_t **error );

LIBFVALUE_EXTERN \
int libfvalue_value_get_entry(
     libfvalue_value_t *value,
     int value_entry_index,
     size_t *entry_data_offset,
     size_t *entry_data_size,
     libcerror_error_t **error );

LIBFVALUE_EXTERN \
int libfvalue_value_set_entry(
     libfvalue_value_t *value,
     int value_entry_index,
     size_t entry_data_offset,
     size_t entry_data_size,
     libcerror_error_t **error );

LIBFVALUE_EXTERN \
int libfvalue_value_append_entry(
     libfvalue_value_t *value,
     int *value_entry_index,
     size_t entry_data_offset,
     size_t entry_data_size,
     libcerror_error_t **error );

LIBFVALUE_EXTERN \
int libfvalue_value_get_entry_data(
     libfvalue_value_t *value,
     int value_entry_index,
     uint8_t **entry_data,
     size_t *entry_data_size,
     int *encoding,
     libcerror_error_t **error );

LIBFVALUE_EXTERN \
int libfvalue_value_set_entry_data(
     libfvalue_value_t *value,
     int value_entry_index,
     const uint8_t *entry_data,
     size_t entry_data_size,
     int encoding,
     libcerror_error_t **error );

LIBFVALUE_EXTERN \
int libfvalue_value_append_entry_data(
     libfvalue_value_t *value,
     int *value_entry_index,
     const uint8_t *entry_data,
     size_t entry_data_size,
     int encoding,
     libcerror_error_t **error );

LIBFVALUE_EXTERN \
int libfvalue_value_copy_entry_data(
     libfvalue_value_t *value,
     int value_entry_index,
     uint8_t *entry_data,
     size_t entry_data_size,
     int *encoding,
     libcerror_error_t **error );

/* Boolean value functions
 */
LIBFVALUE_EXTERN \
int libfvalue_value_copy_from_boolean(
     libfvalue_value_t *value,
     int value_entry_index,
     uint8_t value_boolean,
     libcerror_error_t **error );

LIBFVALUE_EXTERN \
int libfvalue_value_copy_to_boolean(
     libfvalue_value_t *value,
     int value_entry_index,
     uint8_t *value_boolean,
     libcerror_error_t **error );

/* Integer value functions
 */
LIBFVALUE_EXTERN \
int libfvalue_value_copy_from_8bit(
     libfvalue_value_t *value,
     int value_entry_index,
     uint8_t value_8bit,
     libcerror_error_t **error );

LIBFVALUE_EXTERN \
int libfvalue_value_copy_to_8bit(
     libfvalue_value_t *value,
     int value_entry_index,
     uint8_t *value_8bit,
     libcerror_error_t **error );

LIBFVALUE_EXTERN \
int libfvalue_value_copy_from_16bit(
     libfvalue_value_t *value,
     int value_entry_index,
     uint16_t value_16bit,
     libcerror_error_t **error );

LIBFVALUE_EXTERN \
int libfvalue_value_copy_to_16bit(
     libfvalue_value_t *value,
     int value_entry_index,
     uint16_t *value_16bit,
     libcerror_error_t **error );

LIBFVALUE_EXTERN \
int libfvalue_value_copy_from_32bit(
     libfvalue_value_t *value,
     int value_entry_index,
     uint32_t value_32bit,
     libcerror_error_t **error );

LIBFVALUE_EXTERN \
int libfvalue_value_copy_to_32bit(
     libfvalue_value_t *value,
     int value_entry_index,
     uint32_t *value_32bit,
     libcerror_error_t **error );

LIBFVALUE_EXTERN \
int libfvalue_value_copy_from_64bit(
     libfvalue_value_t *value,
     int value_entry_index,
     uint64_t value_64bit,
     libcerror_error_t **error );

LIBFVALUE_EXTERN \
int libfvalue_value_copy_to_64bit(
     libfvalue_value_t *value,
     int value_entry_index,
     uint64_t *value_64bit,
     libcerror_error_t **error );

/* Floating point value functions
 */
LIBFVALUE_EXTERN \
int libfvalue_value_copy_from_float(
     libfvalue_value_t *value,
     int value_entry_index,
     float value_float,
     libcerror_error_t **error );

LIBFVALUE_EXTERN \
int libfvalue_value_copy_to_float(
     libfvalue_value_t *value,
     int value_entry_index,
     float *value_float,
     libcerror_error_t **error );

LIBFVALUE_EXTERN \
int libfvalue_value_copy_from_double(
     libfvalue_value_t *value,
     int value_entry_index,
     double value_double,
     libcerror_error_t **error );

LIBFVALUE_EXTERN \
int libfvalue_value_copy_to_double(
     libfvalue_value_t *value,
     int value_entry_index,
     double *value_double,
     libcerror_error_t **error );

/* String value functions
 */
LIBFVALUE_EXTERN \
int libfvalue_value_copy_from_utf8_string(
     libfvalue_value_t *value,
     int value_entry_index,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libcerror_error_t **error );

LIBFVALUE_EXTERN \
int libfvalue_value_get_utf8_string_size(
     libfvalue_value_t *value,
     int value_entry_index,
     size_t *utf8_string_size,
     libcerror_error_t **error );

LIBFVALUE_EXTERN \
int libfvalue_value_copy_to_utf8_string(
     libfvalue_value_t *value,
     int value_entry_index,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

LIBFVALUE_EXTERN \
int libfvalue_value_copy_to_utf8_string_with_index(
     libfvalue_value_t *value,
     int value_entry_index,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     size_t *utf8_string_index,
     libcerror_error_t **error );

LIBFVALUE_EXTERN \
int libfvalue_value_copy_from_utf16_string(
     libfvalue_value_t *value,
     int value_entry_index,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libcerror_error_t **error );

LIBFVALUE_EXTERN \
int libfvalue_value_get_utf16_string_size(
     libfvalue_value_t *value,
     int value_entry_index,
     size_t *utf16_string_size,
     libcerror_error_t **error );

LIBFVALUE_EXTERN \
int libfvalue_value_copy_to_utf16_string(
     libfvalue_value_t *value,
     int value_entry_index,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

LIBFVALUE_EXTERN \
int libfvalue_value_copy_to_utf16_string_with_index(
     libfvalue_value_t *value,
     int value_entry_index,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     size_t *utf16_string_index,
     libcerror_error_t **error );

LIBFVALUE_EXTERN \
int libfvalue_value_copy_from_utf32_string(
     libfvalue_value_t *value,
     int value_entry_index,
     const uint32_t *utf32_string,
     size_t utf32_string_length,
     libcerror_error_t **error );

LIBFVALUE_EXTERN \
int libfvalue_value_get_utf32_string_size(
     libfvalue_value_t *value,
     int value_entry_index,
     size_t *utf32_string_size,
     libcerror_error_t **error );

LIBFVALUE_EXTERN \
int libfvalue_value_copy_to_utf32_string(
     libfvalue_value_t *value,
     int value_entry_index,
     uint32_t *utf32_string,
     size_t utf32_string_size,
     libcerror_error_t **error );

LIBFVALUE_EXTERN \
int libfvalue_value_copy_to_utf32_string_with_index(
     libfvalue_value_t *value,
     int value_entry_index,
     uint32_t *utf32_string,
     size_t utf32_string_size,
     size_t *utf32_string_index,
     libcerror_error_t **error );

/* Marshalling functions
 */
LIBFVALUE_EXTERN \
ssize_t libfvalue_value_read_from_file_stream(
         libfvalue_value_t *value,
         FILE *file_stream,
         libcerror_error_t **error );

LIBFVALUE_EXTERN \
ssize_t libfvalue_value_write_to_file_stream(
         libfvalue_value_t *value,
         FILE *file_stream,
         libcerror_error_t **error );

/* Print functions
 */
int libfvalue_value_print(
     libfvalue_value_t *value,
     int value_entry_index,
     uint8_t flags,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFVALUE_INTERNAL_VALUE_H ) */

