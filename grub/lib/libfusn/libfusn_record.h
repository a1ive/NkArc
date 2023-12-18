/*
 * Record functions
 *
 * Copyright (C) 2011-2022, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _LIBFUSN_RECORD_H )
#define _LIBFUSN_RECORD_H

#include <libyal_wrap.h>

#include "libfusn_extern.h"
#include "libfusn_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfusn_internal_record libfusn_internal_record_t;

struct libfusn_internal_record
{
	/* The size
	 */
	uint32_t size;

	/* The name
	 */
	uint8_t *name;

	/* The name size
	 */
	size_t name_size;

	/* The major version
	 */
	uint16_t major_version;

	/* The minor version
	 */
	uint16_t minor_version;

	/* The update time
	 */
	uint64_t update_time;

	/* The file reference
	 */
	uint64_t file_reference;

	/* The parent file reference
	 */
	uint64_t parent_file_reference;

	/* The update sequence number
	 */
	uint64_t update_sequence_number;

	/* The update reason flags
	 */
	uint32_t update_reason_flags;

	/* The update source flags
	 */
	uint32_t update_source_flags;

	/* The file attribute flags
	 */
	uint32_t file_attribute_flags;
};

LIBFUSN_EXTERN \
int libfusn_record_initialize(
     libfusn_record_t **usn_record,
     libcerror_error_t **error );

LIBFUSN_EXTERN \
int libfusn_record_free(
     libfusn_record_t **usn_record,
     libcerror_error_t **error );

LIBFUSN_EXTERN \
int libfusn_record_copy_from_byte_stream(
     libfusn_record_t *usn_record,
     const uint8_t *byte_stream,
     size_t byte_stream_size,
     libcerror_error_t **error );

LIBFUSN_EXTERN \
int libfusn_record_get_size(
     libfusn_record_t *usn_record,
     uint32_t *size,
     libcerror_error_t **error );

LIBFUSN_EXTERN \
int libfusn_record_get_update_time(
     libfusn_record_t *record,
     uint64_t *update_time,
     libcerror_error_t **error );

LIBFUSN_EXTERN \
int libfusn_record_get_file_reference(
     libfusn_record_t *record,
     uint64_t *file_reference,
     libcerror_error_t **error );

LIBFUSN_EXTERN \
int libfusn_record_get_parent_file_reference(
     libfusn_record_t *record,
     uint64_t *parent_file_reference,
     libcerror_error_t **error );

LIBFUSN_EXTERN \
int libfusn_record_get_update_sequence_number(
     libfusn_record_t *record,
     uint64_t *update_sequence_number,
     libcerror_error_t **error );

LIBFUSN_EXTERN \
int libfusn_record_get_update_reason_flags(
     libfusn_record_t *record,
     uint32_t *update_reason_flags,
     libcerror_error_t **error );

LIBFUSN_EXTERN \
int libfusn_record_get_update_source_flags(
     libfusn_record_t *record,
     uint32_t *update_source_flags,
     libcerror_error_t **error );

LIBFUSN_EXTERN \
int libfusn_record_get_file_attribute_flags(
     libfusn_record_t *record,
     uint32_t *file_attribute_flags,
     libcerror_error_t **error );

LIBFUSN_EXTERN \
int libfusn_record_get_utf8_name_size(
     libfusn_record_t *record,
     size_t *utf8_string_size,
     libcerror_error_t **error );

LIBFUSN_EXTERN \
int libfusn_record_get_utf8_name(
     libfusn_record_t *record,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

LIBFUSN_EXTERN \
int libfusn_record_get_utf16_name_size(
     libfusn_record_t *record,
     size_t *utf16_string_size,
     libcerror_error_t **error );

LIBFUSN_EXTERN \
int libfusn_record_get_utf16_name(
     libfusn_record_t *record,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFUSN_RECORD_H ) */

