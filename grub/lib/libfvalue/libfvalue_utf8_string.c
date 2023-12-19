/*
 * UTF-8 string value functions
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

#include "libfvalue_split_utf8_string.h"
#include "libfvalue_types.h"

/* Splits an UTF-8 string
 * Returns 1 if successful or -1 on error
 */
int libfvalue_utf8_string_split(
     const uint8_t *utf8_string,
     size_t utf8_string_size,
     uint8_t delimiter,
     libfvalue_split_utf8_string_t **split_string,
     libcerror_error_t **error )
{
	uint8_t *segment_start = NULL;
	uint8_t *segment_end   = NULL;
	uint8_t *string_end    = NULL;
	static char *function  = "libfvalue_utf8_string_split";
	size_t string_size     = 0;
	ssize_t segment_length = 0;
	int number_of_segments = 0;
	int segment_index      = 0;

	if( utf8_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-8 string.",
		 function );

		return( -1 );
	}
	if( utf8_string_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid UTF-8 string size value exceeds maximum.",
		 function );

		return( -1 );
	}
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
		 "%s: invalid split string already set.",
		 function );

		return( -1 );
	}
	/* An empty string has no segments
	 */
	if( ( utf8_string_size == 0 )
	 || ( utf8_string[ 0 ] == 0 ) )
	{
		return( 1 );
	}
	/* Determine the number of segments
	 */
	segment_start = (uint8_t *) utf8_string;
	string_end    = (uint8_t *) &( utf8_string[ utf8_string_size - 1 ] );

	do
	{
		segment_end = segment_start;

		while( segment_end <= string_end )
		{
			if( ( segment_end == string_end )
			 || ( *segment_end == 0 ) )
			{
				segment_end = NULL;

				break;
			}
			else if( *segment_end == delimiter )
			{
				break;
			}
			segment_end++;
		}
		if( segment_end > string_end )
		{
			break;
		}
		segment_index++;

		if( segment_end == NULL )
		{
			break;
		}
		if( segment_end == segment_start )
		{
			segment_start++;
		}
		else if( segment_end != utf8_string )
		{
			segment_start = segment_end + 1;
		}
	}
	while( segment_end != NULL );

	number_of_segments = segment_index;

	if( libfvalue_split_utf8_string_initialize(
	     split_string,
	     utf8_string,
	     utf8_string_size,
	     number_of_segments,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize split string.",
		 function );

		goto on_error;
	}
	if( *split_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing split string.",
		 function );

		goto on_error;
	}
	/* Do not bother splitting empty strings
	 */
	if( number_of_segments == 0 )
	{
		return( 1 );
	}
	/* Determine the segments
	 * empty segments are stored as strings only containing the end of character
	 */
	if( libfvalue_split_utf8_string_get_string(
	     *split_string,
	     &segment_start,
	     &string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve split UTF-8 string.",
		 function );

		goto on_error;
	}
	if( segment_start == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing segment start.",
		 function );

		goto on_error;
	}
	if( string_size < 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid string size value out of bounds.",
		 function );

		goto on_error;
	}
	string_end = &( segment_start[ string_size - 1 ] );

	for( segment_index = 0;
	     segment_index < number_of_segments;
	     segment_index++ )
	{
		segment_end = segment_start;

		while( segment_end <= string_end )
		{
			if( ( segment_end == string_end )
			 || ( *segment_end == 0 ) )
			{
				segment_end = NULL;

				break;
			}
			else if( *segment_end == delimiter )
			{
				break;
			}
			segment_end++;
		}
		if( segment_end == NULL )
		{
			segment_length = (ssize_t) ( string_end - segment_start );
		}
		else
		{
			segment_length = (ssize_t) ( segment_end - segment_start );
		}
		if( segment_length >= 0 )
		{
			segment_start[ segment_length ] = 0;

			if( libfvalue_split_utf8_string_set_segment_by_index(
			     *split_string,
			     segment_index,
			     segment_start,
			     segment_length + 1,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set split UTF-8 string segment: %d.",
				 function,
				 segment_index );

				goto on_error;
			}
		}
		if( segment_end == NULL )
		{
			break;
		}
		if( segment_end == string_end )
		{
			segment_start++;
		}
		if( segment_end != string_end )
		{
			segment_start = segment_end + 1;
		}
	}
	return( 1 );

on_error:
	if( *split_string != NULL )
	{
		libfvalue_split_utf8_string_free(
		 split_string,
		 NULL );
	}
	return( -1 );
}

