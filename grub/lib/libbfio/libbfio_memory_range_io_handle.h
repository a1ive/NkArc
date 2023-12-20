/*
 * Memory range IO handle functions
 *
 * Copyright (C) 2009-2022, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _LIBBFIO_MEMORY_RANGE_IO_HANDLE_H )
#define _LIBBFIO_MEMORY_RANGE_IO_HANDLE_H

#include <libyal_wrap.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libbfio_memory_range_io_handle libbfio_memory_range_io_handle_t;

struct libbfio_memory_range_io_handle
{
	/* The start of the memory range
	 */
	uint8_t *range_start;

	/* The size of the memory range
	 */
	size_t range_size;

	/* The current offset in the memory range
	 */
	size_t range_offset;

	/* Value to indicate the memory range is open
	 */
	uint8_t is_open;

	/* The current access flags
	 */
	int access_flags;
};

int libbfio_memory_range_io_handle_initialize(
     libbfio_memory_range_io_handle_t **memory_range_io_handle,
     libcerror_error_t **error );

int libbfio_memory_range_io_handle_free(
     libbfio_memory_range_io_handle_t **memory_range_io_handle,
     libcerror_error_t **error );

int libbfio_memory_range_io_handle_clone(
     libbfio_memory_range_io_handle_t **destination_memory_range_io_handle,
     libbfio_memory_range_io_handle_t *source_memory_range_io_handle,
     libcerror_error_t **error );

int libbfio_memory_range_io_handle_get(
     libbfio_memory_range_io_handle_t *memory_range_io_handle,
     uint8_t **range_start,
     size_t *range_size,
     libcerror_error_t **error );

int libbfio_memory_range_io_handle_set(
     libbfio_memory_range_io_handle_t *memory_range_io_handle,
     uint8_t *range_start,
     size_t range_size,
     libcerror_error_t **error );

int libbfio_memory_range_io_handle_open(
     libbfio_memory_range_io_handle_t *memory_range_io_handle,
     int access_flags,
     libcerror_error_t **error );

int libbfio_memory_range_io_handle_close(
     libbfio_memory_range_io_handle_t *memory_range_io_handle,
     libcerror_error_t **error );

ssize_t libbfio_memory_range_io_handle_read_buffer(
         libbfio_memory_range_io_handle_t *memory_range_io_handle,
         uint8_t *buffer,
         size_t size,
         libcerror_error_t **error );

off64_t libbfio_memory_range_io_handle_seek_offset(
         libbfio_memory_range_io_handle_t *memory_range_io_handle,
         off64_t offset,
         int whence,
         libcerror_error_t **error );

int libbfio_memory_range_io_handle_exists(
     libbfio_memory_range_io_handle_t *memory_range_io_handle,
     libcerror_error_t **error );

int libbfio_memory_range_io_handle_is_open(
     libbfio_memory_range_io_handle_t *memory_range_io_handle,
     libcerror_error_t **error );

int libbfio_memory_range_io_handle_get_size(
     libbfio_memory_range_io_handle_t *memory_range_io_handle,
     size64_t *size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBBFIO_MEMORY_RANGE_IO_HANDLE_H ) */

