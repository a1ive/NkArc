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

#include <libyal_wrap.h>

#include "libbfio_definitions.h"
#include "libbfio_memory_range_io_handle.h"

/* Creates a memory range IO handle
 * Make sure the value memory_range_io_handle is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libbfio_memory_range_io_handle_initialize(
     libbfio_memory_range_io_handle_t **memory_range_io_handle,
     libcerror_error_t **error )
{
	static char *function = "libbfio_memory_range_io_handle_initialize";

	if( memory_range_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid memory range IO handle.",
		 function );

		return( -1 );
	}
	if( *memory_range_io_handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid memory range IO handle value already set.",
		 function );

		return( -1 );
	}
	*memory_range_io_handle = memory_allocate_structure(
	                           libbfio_memory_range_io_handle_t );

	if( *memory_range_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create memory range IO handle.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *memory_range_io_handle,
	     0,
	     sizeof( libbfio_memory_range_io_handle_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear memory range IO handle.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *memory_range_io_handle != NULL )
	{
		memory_free(
		 *memory_range_io_handle );

		*memory_range_io_handle = NULL;
	}
	return( -1 );
}

/* Frees a memory range IO handle
 * Returns 1 if succesful or -1 on error
 */
int libbfio_memory_range_io_handle_free(
     libbfio_memory_range_io_handle_t **memory_range_io_handle,
     libcerror_error_t **error )
{
	static char *function = "libbfio_memory_range_io_handle_free";

	if( memory_range_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid memory range IO handle.",
		 function );

		return( -1 );
	}
	if( *memory_range_io_handle != NULL )
	{
		memory_free(
		 *memory_range_io_handle );

		*memory_range_io_handle = NULL;
	}
	return( 1 );
}

/* Clones (duplicates) the memory range IO handle and its attributes
 * Returns 1 if succesful or -1 on error
 */
int libbfio_memory_range_io_handle_clone(
     libbfio_memory_range_io_handle_t **destination_memory_range_io_handle,
     libbfio_memory_range_io_handle_t *source_memory_range_io_handle,
     libcerror_error_t **error )
{
	static char *function = "libbfio_memory_range_io_handle_clone";

	if( destination_memory_range_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid destination memory range IO handle.",
		 function );

		return( -1 );
	}
	if( *destination_memory_range_io_handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: destination memory range IO handle already set.",
		 function );

		return( -1 );
	}
	if( source_memory_range_io_handle == NULL )
	{
		*destination_memory_range_io_handle = NULL;

		return( 1 );
	}
	if( libbfio_memory_range_io_handle_initialize(
	     destination_memory_range_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create memory range IO handle.",
		 function );

		return( -1 );
	}
	if( *destination_memory_range_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing destination memory range IO handle.",
		 function );

		return( -1 );
	}
	( *destination_memory_range_io_handle )->range_start  = source_memory_range_io_handle->range_start;
	( *destination_memory_range_io_handle )->range_size   = source_memory_range_io_handle->range_size;
	( *destination_memory_range_io_handle )->range_offset = source_memory_range_io_handle->range_offset;
	( *destination_memory_range_io_handle )->access_flags = source_memory_range_io_handle->access_flags;

	return( 1 );
}

/* Retrieves the range of the memory range IO handle
 * Returns 1 if succesful or -1 on error
 */
int libbfio_memory_range_io_handle_get(
     libbfio_memory_range_io_handle_t *memory_range_io_handle,
     uint8_t **range_start,
     size_t *range_size,
     libcerror_error_t **error )
{
	static char *function = "libbfio_memory_range_io_handle_get";

	if( memory_range_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid memory range IO handle.",
		 function );

		return( -1 );
	}
	if( range_start == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid range start.",
		 function );

		return( -1 );
	}
	if( range_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid range size.",
		 function );

		return( -1 );
	}
	*range_start = memory_range_io_handle->range_start;
	*range_size  = memory_range_io_handle->range_size;

	return( 1 );
}

/* Sets the range of the memory range IO handle
 * Returns 1 if succesful or -1 on error
 */
int libbfio_memory_range_io_handle_set(
     libbfio_memory_range_io_handle_t *memory_range_io_handle,
     uint8_t *range_start,
     size_t range_size,
     libcerror_error_t **error )
{
	static char *function = "libbfio_memory_range_io_handle_set";

	if( memory_range_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid memory range IO handle.",
		 function );

		return( -1 );
	}
	if( range_start == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid range start.",
		 function );

		return( -1 );
	}
	if( range_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid range size value exceeds maximum.",
		 function );

		return( -1 );
	}
	memory_range_io_handle->range_start = range_start;
	memory_range_io_handle->range_size  = range_size;

	return( 1 );
}

/* Opens the memory range IO handle
 * Returns 1 if successful or -1 on error
 */
int libbfio_memory_range_io_handle_open(
     libbfio_memory_range_io_handle_t *memory_range_io_handle,
     int access_flags,
     libcerror_error_t **error )
{
	static char *function = "libbfio_memory_range_io_handle_open";

	if( memory_range_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid memory range IO handle.",
		 function );

		return( -1 );
	}
	if( memory_range_io_handle->range_start == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid memory range IO handle - missing range start.",
		 function );

		return( -1 );
	}
	if( memory_range_io_handle->is_open != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid memory range IO handle - already open.",
		 function );

		return( -1 );
	}
	memory_range_io_handle->range_offset = 0;
	memory_range_io_handle->access_flags = access_flags;
	memory_range_io_handle->is_open      = 1;

	return( 1 );
}

/* Closes the memory range IO handle
 * Returns 0 if successful or -1 on error
 */
int libbfio_memory_range_io_handle_close(
     libbfio_memory_range_io_handle_t *memory_range_io_handle,
     libcerror_error_t **error )
{
	static char *function = "libbfio_memory_range_io_handle_close";

	if( memory_range_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid memory range IO handle.",
		 function );

		return( -1 );
	}
	if( memory_range_io_handle->range_start == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid memory range IO handle - missing range start.",
		 function );

		return( -1 );
	}
	if( memory_range_io_handle->is_open == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid memory range IO handle - not open.",
		 function );

		return( -1 );
	}
	memory_range_io_handle->is_open = 0;

	return( 0 );
}

/* Reads a buffer from the memory range IO handle
 * Returns the number of bytes read if successful, or -1 on error
 */
ssize_t libbfio_memory_range_io_handle_read_buffer(
         libbfio_memory_range_io_handle_t *memory_range_io_handle,
         uint8_t *buffer,
         size_t size,
         libcerror_error_t **error )
{
	static char *function = "libbfio_memory_range_io_handle_read_buffer";
	size_t read_size      = 0;

	if( memory_range_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid memory range IO handle.",
		 function );

		return( -1 );
	}
	if( memory_range_io_handle->range_start == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid memory range IO handle - invalid range start.",
		 function );

		return( -1 );
	}
	if( memory_range_io_handle->is_open == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid memory range IO handle - not open.",
		 function );

		return( -1 );
	}
	if( ( memory_range_io_handle->access_flags & LIBBFIO_ACCESS_FLAG_READ ) == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid memory range IO handle - no read access.",
		 function );

		return( -1 );
	}
	if( buffer == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid buffer.",
		 function );

		return( -1 );
	}
	if( size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( memory_range_io_handle->range_offset > memory_range_io_handle->range_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid range offset value out of bounds.",
		 function );

		return( -1 );
	}
	/* Check if the end of the data was reached
	 */
	if( memory_range_io_handle->range_offset >= memory_range_io_handle->range_size )
	{
		return( 0 );
	}
	/* Check the amount of data available
	 */
	read_size = memory_range_io_handle->range_size - memory_range_io_handle->range_offset;

	/* Cannot read more data than available
	 */
	if( read_size > size )
	{
		read_size = size;
	}
	if( memory_copy(
	     buffer,
	     &( memory_range_io_handle->range_start[ memory_range_io_handle->range_offset ] ),
	     read_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to read buffer from memory range.",
		 function );

		return( -1 );
	}
	memory_range_io_handle->range_offset += read_size;

	return( (ssize_t) read_size );
}

/* Seeks a certain offset within the memory range IO handle
 * Returns the offset if the seek is successful or -1 on error
 */
off64_t libbfio_memory_range_io_handle_seek_offset(
         libbfio_memory_range_io_handle_t *memory_range_io_handle,
         off64_t offset,
         int whence,
         libcerror_error_t **error )
{
	static char *function = "libbfio_memory_range_io_handle_seek_offset";

	if( memory_range_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid memory range IO handle.",
		 function );

		return( -1 );
	}
	if( memory_range_io_handle->range_start == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid memory range IO handle - invalid range start.",
		 function );

		return( -1 );
	}
	if( memory_range_io_handle->is_open == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid memory range IO handle - not open.",
		 function );

		return( -1 );
	}
	if( offset > (off64_t) INT64_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid offset value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( ( whence != SEEK_CUR )
	 && ( whence != SEEK_END )
	 && ( whence != SEEK_SET ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported whence.",
		 function );

		return( -1 );
	}
	if( whence == SEEK_CUR )
	{
		offset += memory_range_io_handle->range_offset;
	}
	else if( whence == SEEK_END )
	{
		offset += memory_range_io_handle->range_size;
	}
	if( offset < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek offset.",
		 function );

		return( -1 );
	}
	if( offset > (off64_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid offset value exceeds maximum.",
		 function );

		return( -1 );
	}
	memory_range_io_handle->range_offset = (size_t) offset;

	return( offset );
}

/* Function to determine if a memory range exists
 * Returns 1 if the memory range exists, 0 if not or -1 on error
 */
int libbfio_memory_range_io_handle_exists(
     libbfio_memory_range_io_handle_t *memory_range_io_handle,
     libcerror_error_t **error )
{
	static char *function = "libbfio_memory_range_io_handle_exists";

	if( memory_range_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid memory range IO handle.",
		 function );

		return( -1 );
	}
	if( memory_range_io_handle->range_start == NULL )
	{
		return( 0 );
	}
	return( 1 );
}

/* Check if the memory range is open
 * Returns 1 if open, 0 if not or -1 on error
 */
int libbfio_memory_range_io_handle_is_open(
     libbfio_memory_range_io_handle_t *memory_range_io_handle,
     libcerror_error_t **error )
{
	static char *function = "libbfio_memory_range_io_handle_is_open";

	if( memory_range_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid memory range IO handle.",
		 function );

		return( -1 );
	}
	if( ( memory_range_io_handle->range_start == NULL )
	 || ( memory_range_io_handle->is_open == 0 ) )
	{
		return( 0 );
	}
	return( 1 );
}

/* Retrieves the memory range size
 * Returns 1 if successful or -1 on error
 */
int libbfio_memory_range_io_handle_get_size(
     libbfio_memory_range_io_handle_t *memory_range_io_handle,
     size64_t *size,
     libcerror_error_t **error )
{
	static char *function = "libbfio_memory_range_io_handle_get_size";

	if( memory_range_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid memory range IO handle.",
		 function );

		return( -1 );
	}
	if( memory_range_io_handle->range_start == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid memory range IO handle - invalid range start.",
		 function );

		return( -1 );
	}
	if( size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid size.",
		 function );

		return( -1 );
	}
	*size = (size64_t) memory_range_io_handle->range_size;

	return( 1 );
}

