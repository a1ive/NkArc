/*
 * File IO handle functions
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
#include "libbfio_file_io_handle.h"
#include "libbfio_libcfile.h"
#include "libbfio_system_string.h"

/* Creates a file IO handle
 * Make sure the value file_io_handle is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libbfio_file_io_handle_initialize(
     libbfio_file_io_handle_t **file_io_handle,
     libcerror_error_t **error )
{
	static char *function = "libbfio_file_io_handle_initialize";

	if( file_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file IO handle.",
		 function );

		return( -1 );
	}
	if( *file_io_handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file IO handle value already set.",
		 function );

		return( -1 );
	}
	*file_io_handle = memory_allocate_structure(
	                   libbfio_file_io_handle_t );

	if( *file_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create file IO handle.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *file_io_handle,
	     0,
	     sizeof( libbfio_file_io_handle_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear file IO handle.",
		 function );

		goto on_error;
	}
	if( libcfile_file_initialize(
	     &( ( *file_io_handle )->file ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create file.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *file_io_handle != NULL )
	{
		memory_free(
		 *file_io_handle );

		*file_io_handle = NULL;
	}
	return( -1 );
}

/* Frees a file IO handle
 * Returns 1 if succesful or -1 on error
 */
int libbfio_file_io_handle_free(
     libbfio_file_io_handle_t **file_io_handle,
     libcerror_error_t **error )
{
	static char *function = "libbfio_file_io_handle_free";
	int result            = 1;

	if( file_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file IO handle.",
		 function );

		return( -1 );
	}
	if( *file_io_handle != NULL )
	{
		if( ( *file_io_handle )->name != NULL )
		{
			memory_free(
			 ( *file_io_handle )->name );
		}
		if( libcfile_file_free(
		     &( ( *file_io_handle )->file ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free file.",
			 function );

			result = -1;
		}
		memory_free(
		 *file_io_handle );

		*file_io_handle = NULL;
	}
	return( result );
}

/* Clones (duplicates) the file IO handle and its attributes
 * Returns 1 if succesful or -1 on error
 */
int libbfio_file_io_handle_clone(
     libbfio_file_io_handle_t **destination_file_io_handle,
     libbfio_file_io_handle_t *source_file_io_handle,
     libcerror_error_t **error )
{
	static char *function = "libbfio_file_io_handle_clone";

	if( destination_file_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid destination file IO handle.",
		 function );

		return( -1 );
	}
	if( *destination_file_io_handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: destination file IO handle already set.",
		 function );

		return( -1 );
	}
	if( source_file_io_handle == NULL )
	{
		*destination_file_io_handle = NULL;

		return( 1 );
	}
	if( libbfio_file_io_handle_initialize(
	     destination_file_io_handle,
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
	if( *destination_file_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing destination file IO handle.",
		 function );

		goto on_error;
	}
	if( source_file_io_handle->name_size > 0 )
	{
		if( source_file_io_handle->name == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid source file IO handle - missing name.",
			 function );

			goto on_error;
		}
		if( source_file_io_handle->name_size > ( (size_t) SSIZE_MAX / sizeof( system_character_t ) ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
			 "%s: invalid source file IO handle - name size value exceeds maximum.",
			 function );

			goto on_error;
		}
		( *destination_file_io_handle )->name = system_string_allocate(
		                                         source_file_io_handle->name_size );

		if( ( *destination_file_io_handle )->name == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create name.",
			 function );

			goto on_error;
		}
		if( source_file_io_handle->name_size > 1 )
		{
			if( system_string_copy(
			     ( *destination_file_io_handle )->name,
			     source_file_io_handle->name,
			     source_file_io_handle->name_size ) == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
				 "%s: unable to copy name.",
				 function );

				goto on_error;
			}
		}
		( *destination_file_io_handle )->name[ source_file_io_handle->name_size - 1 ] = 0;

		( *destination_file_io_handle )->name_size = source_file_io_handle->name_size;
	}
	return( 1 );

on_error:
	if( *destination_file_io_handle != NULL )
	{
		libbfio_file_io_handle_free(
		 destination_file_io_handle,
		 NULL );
	}
	return( -1 );
}

/* Retrieves the name size of the file IO handle
 * The name size includes the end of string character
 * Returns 1 if succesful or -1 on error
 */
int libbfio_file_io_handle_get_name_size(
     libbfio_file_io_handle_t *file_io_handle,
     size_t *name_size,
     libcerror_error_t **error )
{
	static char *function = "libbfio_file_io_handle_get_name_size";

	if( file_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file IO handle.",
		 function );

		return( -1 );
	}
	if( libbfio_system_string_size_to_narrow_string(
	     file_io_handle->name,
	     file_io_handle->name_size,
	     name_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to determine name size.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the name of the file IO handle
 * The name size should include the end of string character
 * Returns 1 if succesful or -1 on error
 */
int libbfio_file_io_handle_get_name(
     libbfio_file_io_handle_t *file_io_handle,
     char *name,
     size_t name_size,
     libcerror_error_t **error )
{
	static char *function = "libbfio_file_io_handle_get_name";

	if( file_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file IO handle.",
		 function );

		return( -1 );
	}
	if( libbfio_system_string_copy_to_narrow_string(
	     file_io_handle->name,
	     file_io_handle->name_size,
	     name,
	     name_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to set name.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Sets the name for the file IO handle
 * Returns 1 if succesful or -1 on error
 */
int libbfio_file_io_handle_set_name(
     libbfio_file_io_handle_t *file_io_handle,
     const char *name,
     size_t name_length,
     libcerror_error_t **error )
{
	static char *function = "libbfio_file_io_handle_set_name";
	int result            = 0;

	if( file_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file IO handle.",
		 function );

		return( -1 );
	}
	if( name == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid name.",
		 function );

		return( -1 );
	}
	if( ( name_length == 0 )
	 || ( name_length > ( (size_t) SSIZE_MAX - 1 ) ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid name length value out of bounds.",
		 function );

		return( -1 );
	}
	if( file_io_handle->name != NULL )
	{
		result = libcfile_file_is_open(
		          file_io_handle->file,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
			 "%s: unable to determine if file is open.",
			 function );

			return( -1 );
		}
		if( result != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
			 "%s: unable to set name when file is open.",
			 function );

			return( -1 );
		}
		memory_free(
		  file_io_handle->name );

		file_io_handle->name      = NULL;
		file_io_handle->name_size = 0;
	}
	if( libbfio_system_string_size_from_narrow_string(
	     name,
	     name_length + 1,
	     &( file_io_handle->name_size ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to determine name size.",
		 function );

		goto on_error;
	}
	if( ( file_io_handle->name_size == 0 )
	 || ( file_io_handle->name_size > ( (size_t) SSIZE_MAX / sizeof( system_character_t ) ) ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid file IO handle - name size value out of bounds.",
		 function );

		goto on_error;
	}
	file_io_handle->name = system_string_allocate(
	                        file_io_handle->name_size );

	if( file_io_handle->name == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create name.",
		 function );

		goto on_error;
	}
	if( libbfio_system_string_copy_from_narrow_string(
	     file_io_handle->name,
	     file_io_handle->name_size,
	     name,
	     name_length + 1,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to set name.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( file_io_handle->name != NULL )
	{
		memory_free(
		 file_io_handle->name );

		file_io_handle->name = NULL;
	}
	file_io_handle->name_size = 0;

	return( -1 );
}

#if defined( HAVE_WIDE_CHARACTER_TYPE )

/* Retrieves the name size of the file IO handle
 * The name size includes the end of string character
 * Returns 1 if succesful or -1 on error
 */
int libbfio_file_io_handle_get_name_size_wide(
     libbfio_file_io_handle_t *file_io_handle,
     size_t *name_size,
     libcerror_error_t **error )
{
	static char *function = "libbfio_file_io_handle_get_name_size_wide";

	if( file_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file IO handle.",
		 function );

		return( -1 );
	}
	if( libbfio_system_string_size_to_wide_string(
	     file_io_handle->name,
	     file_io_handle->name_size,
	     name_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to determine name size.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the name of the file IO handle
 * The name size should include the end of string character
 * Returns 1 if succesful or -1 on error
 */
int libbfio_file_io_handle_get_name_wide(
     libbfio_file_io_handle_t *file_io_handle,
     wchar_t *name,
     size_t name_size,
     libcerror_error_t **error )
{
	static char *function = "libbfio_file_io_handle_get_name_wide";

	if( file_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file IO handle.",
		 function );

		return( -1 );
	}
	if( libbfio_system_string_copy_to_wide_string(
	     file_io_handle->name,
	     file_io_handle->name_size,
	     name,
	     name_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to set name.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Sets the name for the file IO handle
 * Returns 1 if succesful or -1 on error
 */
int libbfio_file_io_handle_set_name_wide(
     libbfio_file_io_handle_t *file_io_handle,
     const wchar_t *name,
     size_t name_length,
     libcerror_error_t **error )
{
	static char *function = "libbfio_file_io_handle_set_name_wide";
	int result            = 0;

	if( file_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file IO handle.",
		 function );

		return( -1 );
	}
	if( name == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid name.",
		 function );

		return( -1 );
	}
	if( ( name_length == 0 )
	 || ( name_length > ( (size_t) SSIZE_MAX - 1 ) ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid name length value out of bounds.",
		 function );

		return( -1 );
	}
	if( file_io_handle->name != NULL )
	{
		result = libcfile_file_is_open(
		          file_io_handle->file,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
			 "%s: unable to determine if file is open.",
			 function );

			return( -1 );
		}
		if( result != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
			 "%s: unable to set name when file is open.",
			 function );

			return( -1 );
		}
		memory_free(
		  file_io_handle->name );

		file_io_handle->name      = NULL;
		file_io_handle->name_size = 0;
	}
	if( libbfio_system_string_size_from_wide_string(
	     name,
	     name_length + 1,
	     &( file_io_handle->name_size ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to determine name size.",
		 function );

		goto on_error;
	}
	if( ( file_io_handle->name_size == 0 )
	 || ( file_io_handle->name_size > ( (size_t) SSIZE_MAX / sizeof( system_character_t ) ) ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid file IO handle - name size value out of bounds.",
		 function );

		goto on_error;
	}
	file_io_handle->name = system_string_allocate(
	                        file_io_handle->name_size );

	if( file_io_handle->name == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create name.",
		 function );

		goto on_error;
	}
	if( libbfio_system_string_copy_from_wide_string(
	     file_io_handle->name,
	     file_io_handle->name_size,
	     name,
	     name_length + 1,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to set name.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( file_io_handle->name != NULL )
	{
		memory_free(
		 file_io_handle->name );

		file_io_handle->name = NULL;
	}
	file_io_handle->name_size = 0;

	return( -1 );
}

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

/* Opens the file handle
 * Returns 1 if successful or -1 on error
 */
int libbfio_file_io_handle_open(
     libbfio_file_io_handle_t *file_io_handle,
     int access_flags,
     libcerror_error_t **error )
{
	static char *function = "libbfio_file_io_handle_open";
	int result            = 0;

	if( file_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file IO handle.",
		 function );

		return( -1 );
	}
	if( file_io_handle->name == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file IO handle - missing name.",
		 function );

		return( -1 );
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libcfile_file_open_wide(
	          file_io_handle->file,
	          file_io_handle->name,
	          access_flags,
	          error );
#else
	result = libcfile_file_open(
	          file_io_handle->file,
	          file_io_handle->name,
	          access_flags,
	          error );
#endif
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open file: %" PRIs_SYSTEM ".",
		 function,
		 file_io_handle->name );

		return( -1 );
	}
	file_io_handle->access_flags = access_flags;

	return( 1 );
}

/* Closes the file handle
 * Returns 0 if successful or -1 on error
 */
int libbfio_file_io_handle_close(
     libbfio_file_io_handle_t *file_io_handle,
     libcerror_error_t **error )
{
	static char *function = "libbfio_file_close";

	if( file_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file IO handle.",
		 function );

		return( -1 );
	}
	if( file_io_handle->name == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file IO handle - missing name.",
		 function );

		return( -1 );
	}
	if( libcfile_file_close(
	     file_io_handle->file,
	     error ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to close file: %" PRIs_SYSTEM ".",
		 function,
		 file_io_handle->name );

		return( -1 );
	}
	file_io_handle->access_flags = 0;

	return( 0 );
}

/* Reads a buffer from the file handle
 * Returns the number of bytes read if successful, or -1 on error
 */
ssize_t libbfio_file_io_handle_read_buffer(
         libbfio_file_io_handle_t *file_io_handle,
         uint8_t *buffer,
         size_t size,
         libcerror_error_t **error )
{
	static char *function = "libbfio_file_io_handle_read_buffer";
	ssize_t read_count    = 0;

	if( file_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file IO handle.",
		 function );

		return( -1 );
	}
	if( file_io_handle->name == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file IO handle - missing name.",
		 function );

		return( -1 );
	}
	read_count = libcfile_file_read_buffer(
	              file_io_handle->file,
	              buffer,
	              size,
	              error );

	if( read_count < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read from file: %" PRIs_SYSTEM ".",
		 function,
		 file_io_handle->name );

		return( -1 );
	}
	return( read_count );
}

/* Seeks a certain offset within the file handle
 * Returns the offset if the seek is successful or -1 on error
 */
off64_t libbfio_file_io_handle_seek_offset(
         libbfio_file_io_handle_t *file_io_handle,
         off64_t offset,
         int whence,
         libcerror_error_t **error )
{
	static char *function = "libbfio_file_seek_offset";
	off64_t seek_offset   = 0;

	if( file_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file IO handle.",
		 function );

		return( -1 );
	}
	if( file_io_handle->name == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file IO handle - missing name.",
		 function );

		return( -1 );
	}
	seek_offset = libcfile_file_seek_offset(
	               file_io_handle->file,
	               offset,
	               whence,
	               error );

	if( seek_offset == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek offset: %" PRIi64 " in file: %" PRIs_SYSTEM ".",
		 function,
		 offset,
		 file_io_handle->name );

		return( -1 );
	}
	return( seek_offset );
}

/* Function to determine if a file exists
 * Returns 1 if file exists, 0 if not or -1 on error
 */
int libbfio_file_io_handle_exists(
     libbfio_file_io_handle_t *file_io_handle,
     libcerror_error_t **error )
{
	static char *function = "libbfio_file_exists";
	int result            = 0;

	if( file_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file IO handle.",
		 function );

		return( -1 );
	}
	if( file_io_handle->name == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file IO handle - missing name.",
		 function );

		return( -1 );
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libcfile_file_exists_wide(
	          file_io_handle->name,
	          error );
#else
	result = libcfile_file_exists(
	          file_io_handle->name,
	          error );
#endif
	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_GENERIC,
		 "%s: unable to determine if file: %" PRIs_SYSTEM " exists.",
		 function,
		 file_io_handle->name );

		return( -1 );
	}
	return( result );
}

/* Check if the file is open
 * Returns 1 if open, 0 if not or -1 on error
 */
int libbfio_file_io_handle_is_open(
     libbfio_file_io_handle_t *file_io_handle,
     libcerror_error_t **error )
{
	static char *function = "libbfio_file_is_open";
	int result            = 0;

	if( file_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file IO handle.",
		 function );

		return( -1 );
	}
	result = libcfile_file_is_open(
	          file_io_handle->file,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if file is open.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Retrieves the file size
 * Returns 1 if successful or -1 on error
 */
int libbfio_file_io_handle_get_size(
     libbfio_file_io_handle_t *file_io_handle,
     size64_t *size,
     libcerror_error_t **error )
{
	static char *function = "libbfio_file_get_size";

	if( file_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file IO handle.",
		 function );

		return( -1 );
	}
	if( file_io_handle->name == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file IO handle - missing name.",
		 function );

		return( -1 );
	}
	if( libcfile_file_get_size(
	     file_io_handle->file,
	     size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve size of file: %" PRIs_SYSTEM ".",
		 function,
		 file_io_handle->name );

		return( -1 );
	}
	return( 1 );
}

