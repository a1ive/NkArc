/*
 * Path functions
 *
 * Copyright (C) 2008-2022, Joachim Metz <joachim.metz@gmail.com>
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

#include "libcpath_definitions.h"
#include "libcpath_libcsplit.h"
#include "libcpath_path.h"
#include "libcpath_system_string.h"

/* Retrieves the size of a sanitized version of the path character
 * Returns 1 if successful or -1 on error
 */
int libcpath_path_get_sanitized_character_size(
     char character,
     size_t *sanitized_character_size,
     libcerror_error_t **error )
{
	static char *function = "libcpath_path_get_sanitized_character_size";

	if( sanitized_character_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sanitized character size.",
		 function );

		return( -1 );
	}
	if( ( character >= 0x00 )
	 && ( character <= 0x1f ) )
	{
		*sanitized_character_size = 4;
	}
	else if( character == LIBCPATH_ESCAPE_CHARACTER )
	{
		*sanitized_character_size = 2;
	}
	else if( ( character == '!' )
	      || ( character == '$' )
	      || ( character == '%' )
	      || ( character == '&' )
	      || ( character == '*' )
	      || ( character == '+' )
	      || ( character == ':' )
	      || ( character == ';' )
	      || ( character == '<' )
	      || ( character == '>' )
	      || ( character == '?' )
	      || ( character == '|' )
	      || ( character == 0x7f ) )
	{
		*sanitized_character_size = 4;
	}
	else
	{
		*sanitized_character_size = 1;
	}
	return( 1 );
}

/* Retrieves a sanitized version of the path character
 * Returns 1 if successful or -1 on error
 */
int libcpath_path_get_sanitized_character(
     char character,
     size_t sanitized_character_size,
     char *sanitized_path,
     size_t sanitized_path_size,
     size_t *sanitized_path_index,
     libcerror_error_t **error )
{
	static char *function            = "libcpath_path_get_sanitized_character";
	size_t safe_sanitized_path_index = 0;
	char lower_nibble                = 0;
	char upper_nibble                = 0;

	if( ( sanitized_character_size != 1 )
	 && ( sanitized_character_size != 2 )
	 && ( sanitized_character_size != 4 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid sanitized character size value out of bounds.",
		 function );

		return( -1 );
	}
	if( sanitized_path == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sanitized path.",
		 function );

		return( -1 );
	}
	if( sanitized_path_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid sanitized path size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( sanitized_path_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sanitized path index.",
		 function );

		return( -1 );
	}
	safe_sanitized_path_index = *sanitized_path_index;

	if( safe_sanitized_path_index > sanitized_path_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid sanitized path index value out of bounds.",
		 function );

		return( -1 );
	}
	if( ( sanitized_character_size > sanitized_path_size )
	 || ( safe_sanitized_path_index > ( sanitized_path_size - sanitized_character_size ) ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: invalid sanitized path size value too small.",
		 function );

		return( -1 );
	}
	if( sanitized_character_size == 1 )
	{
		sanitized_path[ safe_sanitized_path_index++ ] = character;
	}
	else if( sanitized_character_size == 2 )
	{
		sanitized_path[ safe_sanitized_path_index++ ] = LIBCPATH_ESCAPE_CHARACTER;
		sanitized_path[ safe_sanitized_path_index++ ] = LIBCPATH_ESCAPE_CHARACTER;
	}
	else if( sanitized_character_size == 4 )
	{
		lower_nibble = character & 0x0f;
		upper_nibble = ( character >> 4 ) & 0x0f;

		if( lower_nibble > 10 )
		{
			lower_nibble += 'a' - 10;
		}
		else
		{
			lower_nibble += '0';
		}
		if( upper_nibble > 10 )
		{
			upper_nibble += 'a' - 10;
		}
		else
		{
			upper_nibble += '0';
		}
		sanitized_path[ safe_sanitized_path_index++ ] = LIBCPATH_ESCAPE_CHARACTER;
		sanitized_path[ safe_sanitized_path_index++ ] = 'x';
		sanitized_path[ safe_sanitized_path_index++ ] = upper_nibble;
		sanitized_path[ safe_sanitized_path_index++ ] = lower_nibble;
	}
	*sanitized_path_index = safe_sanitized_path_index;

	return( 1 );
}

/* Retrieves a sanitized version of the filename
 * Returns 1 if successful or -1 on error
 */
int libcpath_path_get_sanitized_filename(
     const char *filename,
     size_t filename_length,
     char **sanitized_filename,
     size_t *sanitized_filename_size,
     libcerror_error_t **error )
{
	static char *function               = "libcpath_path_get_sanitized_filename";
	char *safe_sanitized_filename       = NULL;
	size_t filename_index               = 0;
	size_t sanitized_character_size     = 0;
	size_t safe_sanitized_filename_size = 0;
	size_t sanitized_filename_index     = 0;

	if( filename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filename.",
		 function );

		return( -1 );
	}
	if( filename_length == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_ZERO_OR_LESS,
		 "%s: invalid filename length is zero.",
		 function );

		return( -1 );
	}
	if( filename_length > (size_t) ( SSIZE_MAX - 1 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid filename length value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( sanitized_filename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sanitized filename.",
		 function );

		return( -1 );
	}
	if( *sanitized_filename != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid sanitized filename value already set.",
		 function );

		return( -1 );
	}
	if( sanitized_filename_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sanitized filename size.",
		 function );

		return( -1 );
	}
	safe_sanitized_filename_size = 1;

	for( filename_index = 0;
	     filename_index < filename_length;
	     filename_index++ )
	{
		if( filename[ filename_index ] == LIBCPATH_SEPARATOR )
		{
			sanitized_character_size = 4;
		}
		else if( libcpath_path_get_sanitized_character_size(
		          filename[ filename_index ],
		          &sanitized_character_size,
		          error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine sanitize character size.",
			 function );

			goto on_error;
		}
		safe_sanitized_filename_size += sanitized_character_size;
	}
	if( safe_sanitized_filename_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid sanitized filename size value exceeds maximum.",
		 function );

		goto on_error;
	}
	safe_sanitized_filename = narrow_string_allocate(
	                           safe_sanitized_filename_size );

	if( safe_sanitized_filename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create sanitized filename.",
		 function );

		goto on_error;
	}
	for( filename_index = 0;
	     filename_index < filename_length;
	     filename_index++ )
	{
		if( filename[ filename_index ] == LIBCPATH_SEPARATOR )
		{
			sanitized_character_size = 4;
		}
		else if( libcpath_path_get_sanitized_character_size(
		          filename[ filename_index ],
		          &sanitized_character_size,
		          error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine sanitize character size.",
			 function );

			goto on_error;
		}
		if( libcpath_path_get_sanitized_character(
		     filename[ filename_index ],
		     sanitized_character_size,
		     safe_sanitized_filename,
		     safe_sanitized_filename_size,
		     &sanitized_filename_index,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine sanitize character size.",
			 function );

			goto on_error;
		}
	}
	safe_sanitized_filename[ sanitized_filename_index ] = 0;

	*sanitized_filename      = safe_sanitized_filename;
	*sanitized_filename_size = safe_sanitized_filename_size;

	return( 1 );

on_error:
	if( safe_sanitized_filename != NULL )
	{
		memory_free(
		 safe_sanitized_filename );
	}
	return( -1 );
}

/* Retrieves a sanitized version of the path
 * Returns 1 if successful or -1 on error
 */
int libcpath_path_get_sanitized_path(
     const char *path,
     size_t path_length,
     char **sanitized_path,
     size_t *sanitized_path_size,
     libcerror_error_t **error )
{
	static char *function                    = "libcpath_path_get_sanitized_path";
	char *safe_sanitized_path                = NULL;
	size_t path_index                        = 0;
	size_t safe_sanitized_path_size          = 0;
	size_t sanitized_character_size          = 0;
	size_t sanitized_path_index              = 0;

	if( path == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid path.",
		 function );

		return( -1 );
	}
	if( path_length == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_ZERO_OR_LESS,
		 "%s: invalid path length is zero.",
		 function );

		return( -1 );
	}
	if( path_length > (size_t) ( SSIZE_MAX - 1 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid path length value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( sanitized_path == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sanitized path.",
		 function );

		return( -1 );
	}
	if( *sanitized_path != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid sanitized path value already set.",
		 function );

		return( -1 );
	}
	if( sanitized_path_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sanitized path size.",
		 function );

		return( -1 );
	}
	safe_sanitized_path_size = 1;

	for( path_index = 0;
	     path_index < path_length;
	     path_index++ )
	{
		if( libcpath_path_get_sanitized_character_size(
		     path[ path_index ],
		     &sanitized_character_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine sanitize character size.",
			 function );

			goto on_error;
		}
		safe_sanitized_path_size += sanitized_character_size;
	}
	if( safe_sanitized_path_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid sanitized path size value exceeds maximum.",
		 function );

		goto on_error;
	}

	safe_sanitized_path = narrow_string_allocate(
	                       safe_sanitized_path_size );

	if( safe_sanitized_path == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create sanitized path.",
		 function );

		goto on_error;
	}
	for( path_index = 0;
	     path_index < path_length;
	     path_index++ )
	{
		if( libcpath_path_get_sanitized_character_size(
		     path[ path_index ],
		     &sanitized_character_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine sanitize character size.",
			 function );

			goto on_error;
		}
		if( libcpath_path_get_sanitized_character(
		     path[ path_index ],
		     sanitized_character_size,
		     safe_sanitized_path,
		     safe_sanitized_path_size,
		     &sanitized_path_index,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine sanitize character size.",
			 function );

			goto on_error;
		}
	}
	safe_sanitized_path[ sanitized_path_index ] = 0;

	*sanitized_path      = safe_sanitized_path;
	*sanitized_path_size = safe_sanitized_path_size;

	return( 1 );

on_error:
	if( safe_sanitized_path != NULL )
	{
		memory_free(
		 safe_sanitized_path );
	}
	return( -1 );
}

/* Combines the directory name and filename into a path
 * Returns 1 if successful or -1 on error
 */
int libcpath_path_join(
     char **path,
     size_t *path_size,
     const char *directory_name,
     size_t directory_name_length,
     const char *filename,
     size_t filename_length,
     libcerror_error_t **error )
{
	static char *function = "libcpath_path_join";
	size_t filename_index = 0;
	size_t path_index     = 0;

	if( path == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid path.",
		 function );

		return( -1 );
	}
	if( *path != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid path value already set.",
		 function );

		return( -1 );
	}
	if( path_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid path size.",
		 function );

		return( -1 );
	}
	if( directory_name == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory name.",
		 function );

		return( -1 );
	}
	if( directory_name_length > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid directory name length value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( filename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filename.",
		 function );

		return( -1 );
	}
	if( filename_length > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid filename length value exceeds maximum.",
		 function );

		return( -1 );
	}
/* TODO strip other patterns like /./ */
	while( directory_name_length > 0 )
	{
		if( directory_name[ directory_name_length - 1 ] != (char) LIBCPATH_SEPARATOR )
		{
			break;
		}
		directory_name_length--;
	}
	while( filename_length > 0 )
	{
		if( filename[ filename_index ] != (char) LIBCPATH_SEPARATOR )
		{
			break;
		}
		filename_index++;
		filename_length--;
	}
	*path_size = directory_name_length + filename_length + 2;

	*path = narrow_string_allocate(
	         *path_size );

	if( *path == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create path.",
		 function );

		goto on_error;
	}
	if( narrow_string_copy(
	     *path,
	     directory_name,
	     directory_name_length ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy directory name to path.",
		 function );

		goto on_error;
	}
	path_index = directory_name_length;

	( *path )[ path_index++ ] = (char) LIBCPATH_SEPARATOR;

	if( narrow_string_copy(
	     &( ( *path )[ path_index ] ),
	     &( filename[ filename_index ] ),
	     filename_length ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy filename to path.",
		 function );

		goto on_error;
	}
	path_index += filename_length;

	( *path )[ path_index ] = 0;

	return( 1 );

on_error:
	if( *path != NULL )
	{
		memory_free(
		 *path );

		*path = NULL;
	}
	*path_size = 0;

	return( -1 );
}
