/*
 * Split UTF-16 string functions
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

#include <libyal_wrap.h>

#include "libfvalue_split_utf16_string.h"
#include "libfvalue_types.h"

/* Creates a split UTF-16 string
 * Make sure the value split_string is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfvalue_split_utf16_string_initialize(
     libfvalue_split_utf16_string_t **split_string,
     const uint16_t *utf16_string,
     size_t utf16_string_size,
     int number_of_segments,
     libcerror_error_t **error )
{
	libfvalue_internal_split_utf16_string_t *internal_split_string = NULL;
	static char *function                                          = "libfvalue_split_utf16_string_initialize";

	if( split_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid split string.",
		 function );

		return( -1 );
	}
	if( *split_string != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid split string value already set.",
		 function );

		return( -1 );
	}
	if( number_of_segments < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_LESS_THAN_ZERO,
		 "%s: invalid number of segments less than zero.",
		 function );

		return( -1 );
	}
	internal_split_string = memory_allocate_structure(
	                         libfvalue_internal_split_utf16_string_t );

	if( internal_split_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create split string.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     internal_split_string,
	     0,
	     sizeof( libfvalue_internal_split_utf16_string_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear split string.",
		 function );

		memory_free(
		 internal_split_string );

		return( -1 );
	}
	if( ( utf16_string != NULL )
	 && ( utf16_string_size > 0 ) )
	{
		internal_split_string->string = (uint16_t *) memory_allocate(
		                                              sizeof( uint16_t ) * utf16_string_size );

		if( internal_split_string->string == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create string.",
			 function );

			goto on_error;
		}
		if( memory_copy(
		     internal_split_string->string,
		     utf16_string,
		     sizeof( uint16_t ) * ( utf16_string_size - 1 ) ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to copy string.",
			 function );

			goto on_error;
		}
		internal_split_string->string[ utf16_string_size - 1 ] = 0;
		internal_split_string->string_size                     = utf16_string_size;
	}
	if( number_of_segments > 0 )
	{
		internal_split_string->segments = (uint16_t **) memory_allocate(
		                                                 sizeof( uint16_t * ) * number_of_segments );

		if( internal_split_string->segments == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create segments.",
			 function );

			goto on_error;
		}
		if( memory_set(
		     internal_split_string->segments,
		     0,
		     sizeof( uint16_t * ) * number_of_segments ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear segments.",
			 function );

			goto on_error;
		}
		internal_split_string->segment_sizes = (size_t *) memory_allocate(
		                                                   sizeof( size_t ) * number_of_segments );

		if( internal_split_string->segment_sizes == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create segment sizes.",
			 function );

			goto on_error;
		}
		if( memory_set(
		     internal_split_string->segment_sizes,
		     0,
		     sizeof( size_t ) * number_of_segments ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear segment sizes.",
			 function );

			goto on_error;
		}
	}
	internal_split_string->number_of_segments = number_of_segments;

	*split_string = (libfvalue_split_utf16_string_t *) internal_split_string;

	return( 1 );

on_error:
	if( internal_split_string != NULL )
	{
		if( internal_split_string->segment_sizes != NULL )
		{
			memory_free(
			 internal_split_string->segment_sizes );
		}
		if( internal_split_string->segments != NULL )
		{
			memory_free(
			 internal_split_string->segments );
		}
		if( internal_split_string->string != NULL )
		{
			memory_free(
			 internal_split_string->string );
		}
		memory_free(
		 internal_split_string );
	}
	return( -1 );
}

/* Frees a split UTF-16 string
 * Returns 1 if successful or -1 on error
 */
int libfvalue_split_utf16_string_free(
     libfvalue_split_utf16_string_t **split_string,
     libcerror_error_t **error )
{
	libfvalue_internal_split_utf16_string_t *internal_split_string = NULL;
	static char *function                                          = "libfvalue_split_utf16_string_free";

	if( split_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid split string.",
		 function );

		return( -1 );
	}
	if( *split_string != NULL )
	{
		internal_split_string = (libfvalue_internal_split_utf16_string_t *) *split_string;
		*split_string         = NULL;

		if( internal_split_string->string != NULL )
		{
			memory_free(
			 internal_split_string->string );
		}
		if( internal_split_string->segments != NULL )
		{
			memory_free(
			 internal_split_string->segments );
		}
		if( internal_split_string->segment_sizes != NULL )
		{
			memory_free(
			 internal_split_string->segment_sizes );
		}
		memory_free(
		 internal_split_string );
	}
	return( 1 );
}

/* Retrieves the UTF-16 string
 * Returns 1 if successful or -1 on error
 */
int libfvalue_split_utf16_string_get_string(
     libfvalue_split_utf16_string_t *split_string,
     uint16_t **utf16_string,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	libfvalue_internal_split_utf16_string_t *internal_split_string = NULL;
	static char *function                                          = "libfvalue_split_utf16_string_get_string";

	if( split_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid split string.",
		 function );

		return( -1 );
	}
	internal_split_string = (libfvalue_internal_split_utf16_string_t *) split_string;

	if( utf16_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-16 string.",
		 function );

		return( -1 );
	}
	if( utf16_string_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-16 string size.",
		 function );

		return( -1 );
	}
	*utf16_string      = internal_split_string->string;
	*utf16_string_size = internal_split_string->string_size;

	return( 1 );
}

/* Retrieves the number of segments
 * Returns 1 if successful or -1 on error
 */
int libfvalue_split_utf16_string_get_number_of_segments(
     libfvalue_split_utf16_string_t *split_string,
     int *number_of_segments,
     libcerror_error_t **error )
{
	libfvalue_internal_split_utf16_string_t *internal_split_string = NULL;
	static char *function                                          = "libfvalue_split_utf16_string_get_number_of_segments";

	if( split_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid split string.",
		 function );

		return( -1 );
	}
	internal_split_string = (libfvalue_internal_split_utf16_string_t *) split_string;

	if( number_of_segments == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid number of segments.",
		 function );

		return( -1 );
	}
	*number_of_segments = internal_split_string->number_of_segments;

	return( 1 );
}

/* Retrieves a specific segment
 * Returns 1 if successful or -1 on error
 */
int libfvalue_split_utf16_string_get_segment_by_index(
     libfvalue_split_utf16_string_t *split_string,
     int segment_index,
     uint16_t **utf16_string_segment,
     size_t *utf16_string_segment_size,
     libcerror_error_t **error )
{
	libfvalue_internal_split_utf16_string_t *internal_split_string = NULL;
	static char *function                                          = "libfvalue_split_utf16_string_get_segment_by_index";

	if( split_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid split string.",
		 function );

		return( -1 );
	}
	internal_split_string = (libfvalue_internal_split_utf16_string_t *) split_string;

	if( ( segment_index < 0 )
	 || ( segment_index >= internal_split_string->number_of_segments ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid segment index value out of bounds.",
		 function );

		return( -1 );
	}
	if( utf16_string_segment == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-16 string segment.",
		 function );

		return( -1 );
	}
	if( utf16_string_segment_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-16 string segment size.",
		 function );

		return( -1 );
	}
	*utf16_string_segment      = internal_split_string->segments[ segment_index ];
	*utf16_string_segment_size = internal_split_string->segment_sizes[ segment_index ];

	return( 1 );
}

/* Sets a specific segment
 * Returns 1 if successful or -1 on error
 */
int libfvalue_split_utf16_string_set_segment_by_index(
     libfvalue_split_utf16_string_t *split_string,
     int segment_index,
     uint16_t *utf16_string_segment,
     size_t utf16_string_segment_size,
     libcerror_error_t **error )
{
	libfvalue_internal_split_utf16_string_t *internal_split_string = NULL;
	static char *function                                          = "libfvalue_split_utf16_string_set_segment_by_index";
	size_t utf16_string_segment_offset                             = 0;

	if( split_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid split string.",
		 function );

		return( -1 );
	}
	internal_split_string = (libfvalue_internal_split_utf16_string_t *) split_string;

	if( ( segment_index < 0 )
	 || ( segment_index >= internal_split_string->number_of_segments ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid segment index value out of bounds.",
		 function );

		return( -1 );
	}
	if( utf16_string_segment_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid UTF-16 string segment size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( utf16_string_segment == NULL )
	{
		if( utf16_string_segment_size != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid UTF-16 string segment size value out of bounds.",
			 function );

			return( -1 );
		}
	}
	else
	{
		if( utf16_string_segment < internal_split_string->string )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid UTF-16 string segment value out of bounds.",
			 function );

			return( -1 );
		}
		utf16_string_segment_offset = (size_t) ( utf16_string_segment - internal_split_string->string );

		if( utf16_string_segment_offset > internal_split_string->string_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid UTF-16 string segment value out of bounds.",
			 function );

			return( -1 );
		}
		utf16_string_segment_offset += utf16_string_segment_size;

		if( utf16_string_segment_offset > internal_split_string->string_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid UTF-16 string segment value out of bounds.",
			 function );

			return( -1 );
		}
	}
	internal_split_string->segments[ segment_index ]      = utf16_string_segment;
	internal_split_string->segment_sizes[ segment_index ] = utf16_string_segment_size;

	return( 1 );
}

