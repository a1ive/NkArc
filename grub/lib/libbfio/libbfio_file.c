/*
 * File functions
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
#include "libbfio_file.h"
#include "libbfio_file_io_handle.h"
#include "libbfio_handle.h"
#include "libbfio_libcpath.h"
#include "libbfio_types.h"

/* Creates a file handle
 * Make sure the value handle is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libbfio_file_initialize(
     libbfio_handle_t **handle,
     libcerror_error_t **error )
{
	libbfio_file_io_handle_t *file_io_handle = NULL;
	static char *function                    = "libbfio_file_initialize";

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
	if( libbfio_file_io_handle_initialize(
	     &file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create file IO handle.",
		 function );

		goto on_error;
	}
	if( libbfio_handle_initialize(
	     handle,
	     (intptr_t *) file_io_handle,
	     (int (*)(intptr_t **, libcerror_error_t **)) libbfio_file_io_handle_free,
	     (int (*)(intptr_t **, intptr_t *, libcerror_error_t **)) libbfio_file_io_handle_clone,
	     (int (*)(intptr_t *, int, libcerror_error_t **)) libbfio_file_io_handle_open,
	     (int (*)(intptr_t *, libcerror_error_t **)) libbfio_file_io_handle_close,
	     (ssize_t (*)(intptr_t *, uint8_t *, size_t, libcerror_error_t **)) libbfio_file_io_handle_read_buffer,
	     (off64_t (*)(intptr_t *, off64_t, int, libcerror_error_t **)) libbfio_file_io_handle_seek_offset,
	     (int (*)(intptr_t *, libcerror_error_t **)) libbfio_file_io_handle_exists,
	     (int (*)(intptr_t *, libcerror_error_t **)) libbfio_file_io_handle_is_open,
	     (int (*)(intptr_t *, size64_t *, libcerror_error_t **)) libbfio_file_io_handle_get_size,
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
	if( file_io_handle != NULL )
	{
		libbfio_file_io_handle_free(
		 &file_io_handle,
		 NULL );
	}
	return( -1 );
}

/* Retrieves the name size of the file handle
 * The name size includes the end of string character
 * Returns 1 if succesful or -1 on error
 */
int libbfio_file_get_name_size(
     libbfio_handle_t *handle,
     size_t *name_size,
     libcerror_error_t **error )
{
	libbfio_internal_handle_t *internal_handle = NULL;
	static char *function                      = "libbfio_file_get_name_size";

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

	if( libbfio_file_io_handle_get_name_size(
	     (libbfio_file_io_handle_t *) internal_handle->io_handle,
	     name_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve name size.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the name of the file handle
 * The name size should include the end of string character
 * Returns 1 if succesful or -1 on error
 */
int libbfio_file_get_name(
     libbfio_handle_t *handle,
     char *name,
     size_t name_size,
     libcerror_error_t **error )
{
	libbfio_internal_handle_t *internal_handle = NULL;
	static char *function                      = "libbfio_file_get_name";

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

	if( libbfio_file_io_handle_get_name(
	     (libbfio_file_io_handle_t *) internal_handle->io_handle,
	     name,
	     name_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve name.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Sets the name for the file handle
 * Returns 1 if succesful or -1 on error
 */
int libbfio_file_set_name(
     libbfio_handle_t *handle,
     const char *name,
     size_t name_length,
     libcerror_error_t **error )
{
	libbfio_internal_handle_t *internal_handle = NULL;
	static char *function                      = "libbfio_file_set_name";

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

	if( libbfio_file_io_handle_set_name(
	     (libbfio_file_io_handle_t *) internal_handle->io_handle,
	     name,
	     name_length,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set name.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	return( -1 );
}

#if defined( HAVE_WIDE_CHARACTER_TYPE )

/* Retrieves the name size of the file handle
 * The name size includes the end of string character
 * Returns 1 if succesful or -1 on error
 */
int libbfio_file_get_name_size_wide(
     libbfio_handle_t *handle,
     size_t *name_size,
     libcerror_error_t **error )
{
	libbfio_internal_handle_t *internal_handle = NULL;
	static char *function                      = "libbfio_file_get_name_size_wide";

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

	if( libbfio_file_io_handle_get_name_size_wide(
	     (libbfio_file_io_handle_t *) internal_handle->io_handle,
	     name_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve name size.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the name of the file handle
 * The name size should include the end of string character
 * Returns 1 if succesful or -1 on error
 */
int libbfio_file_get_name_wide(
     libbfio_handle_t *handle,
     wchar_t *name,
     size_t name_size,
     libcerror_error_t **error )
{
	libbfio_internal_handle_t *internal_handle = NULL;
	static char *function                      = "libbfio_file_get_name_wide";

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

	if( libbfio_file_io_handle_get_name_wide(
	     (libbfio_file_io_handle_t *) internal_handle->io_handle,
	     name,
	     name_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve name.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Sets the name for the file handle
 * Returns 1 if succesful or -1 on error
 */
int libbfio_file_set_name_wide(
     libbfio_handle_t *handle,
     const wchar_t *name,
     size_t name_length,
     libcerror_error_t **error )
{
	libbfio_internal_handle_t *internal_handle = NULL;
	wchar_t *full_name                         = NULL;
	static char *function                      = "libbfio_file_set_name_wide";
	size_t full_name_size                      = 0;

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

	if( libcpath_path_get_full_path_wide(
	     name,
	     name_length,
	     &full_name,
	     &full_name_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to determine full path.",
		 function );

		goto on_error;
	}
	if( libbfio_file_io_handle_set_name_wide(
	     (libbfio_file_io_handle_t *) internal_handle->io_handle,
	     full_name,
	     full_name_size - 1,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set name.",
		 function );

		goto on_error;
	}
	if( ( full_name != NULL )
	 && ( full_name != name ) )
	{
		memory_free(
		 full_name );
	}
	return( 1 );

on_error:
	if( ( full_name != NULL )
	 && ( full_name != name ) )
	{
		memory_free(
		 full_name );
	}
	return( -1 );
}

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

