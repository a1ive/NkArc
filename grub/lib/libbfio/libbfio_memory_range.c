/*
 * Memory range functions
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

#include <libyal_wrap.h>

#include "libbfio_definitions.h"
#include "libbfio_handle.h"
#include "libbfio_memory_range.h"
#include "libbfio_memory_range_io_handle.h"
#include "libbfio_types.h"

/* Creates a memory range handle
 * Make sure the value handle is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libbfio_memory_range_initialize(
     libbfio_handle_t **handle,
     libcerror_error_t **error )
{
	libbfio_memory_range_io_handle_t *memory_range_io_handle = NULL;
	static char *function                                    = "libbfio_memory_range_initialize";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	if( *handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid handle value already set.",
		 function );

		return( -1 );
	}
	if( libbfio_memory_range_io_handle_initialize(
	     &memory_range_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create memory range IO handle.",
		 function );

		goto on_error;
	}
	if( libbfio_handle_initialize(
	     handle,
	     (intptr_t *) memory_range_io_handle,
	     (int (*)(intptr_t **, libcerror_error_t **)) libbfio_memory_range_io_handle_free,
	     (int (*)(intptr_t **, intptr_t *, libcerror_error_t **)) libbfio_memory_range_io_handle_clone,
	     (int (*)(intptr_t *, int, libcerror_error_t **)) libbfio_memory_range_io_handle_open,
	     (int (*)(intptr_t *, libcerror_error_t **)) libbfio_memory_range_io_handle_close,
	     (ssize_t (*)(intptr_t *, uint8_t *, size_t, libcerror_error_t **)) libbfio_memory_range_io_handle_read_buffer,
	     (off64_t (*)(intptr_t *, off64_t, int, libcerror_error_t **)) libbfio_memory_range_io_handle_seek_offset,
	     (int (*)(intptr_t *, libcerror_error_t **)) libbfio_memory_range_io_handle_exists,
	     (int (*)(intptr_t *, libcerror_error_t **)) libbfio_memory_range_io_handle_is_open,
	     (int (*)(intptr_t *, size64_t *, libcerror_error_t **)) libbfio_memory_range_io_handle_get_size,
	     LIBBFIO_FLAG_IO_HANDLE_MANAGED | LIBBFIO_FLAG_IO_HANDLE_CLONE_BY_FUNCTION,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create handle.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( memory_range_io_handle != NULL )
	{
		libbfio_memory_range_io_handle_free(
		 &memory_range_io_handle,
		 NULL );
	}
	return( -1 );
}

/* Retrieves the range of the memory range handle
 * Returns 1 if succesful or -1 on error
 */
int libbfio_memory_range_get(
     libbfio_handle_t *handle,
     uint8_t **range_start,
     size_t *range_size,
     libcerror_error_t **error )
{
	libbfio_internal_handle_t *internal_handle = NULL;
	static char *function                      = "libbfio_memory_range_get";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libbfio_internal_handle_t *) handle;

	if( libbfio_memory_range_io_handle_get(
	     (libbfio_memory_range_io_handle_t *) internal_handle->io_handle,
	     range_start,
	     range_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve range from file IO handle.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Sets the range of the memory range handle
 * Returns 1 if succesful or -1 on error
 */
int libbfio_memory_range_set(
     libbfio_handle_t *handle,
     uint8_t *range_start,
     size_t range_size,
     libcerror_error_t **error )
{
	libbfio_internal_handle_t *internal_handle = NULL;
	static char *function                      = "libbfio_memory_range_set";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libbfio_internal_handle_t *) handle;

	if( libbfio_memory_range_io_handle_set(
	     (libbfio_memory_range_io_handle_t *) internal_handle->io_handle,
	     range_start,
	     range_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set range in file IO handle.",
		 function );

		return( -1 );
	}
	return( 1 );
}

