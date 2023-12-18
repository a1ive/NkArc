/*
 * Range list
 *
 * Copyright (C) 2006-2023, Joachim Metz <joachim.metz@gmail.com>
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

#include "libcdata_definitions.h"
#include "libcdata_list_element.h"
#include "libcdata_range_list.h"
#include "libcdata_range_list_value.h"
#include "libcdata_types.h"
#include "libcdata_unused.h"

/* Creates a range list
 * Make sure the value range_list is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libcdata_range_list_initialize(
     libcdata_range_list_t **range_list,
     libcerror_error_t **error )
{
	libcdata_internal_range_list_t *internal_range_list = NULL;
	static char *function                               = "libcdata_range_list_initialize";

	if( range_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid range list.",
		 function );

		return( -1 );
	}
	if( *range_list != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid range list value already set.",
		 function );

		return( -1 );
	}
	internal_range_list = memory_allocate_structure(
	                       libcdata_internal_range_list_t );

	if( internal_range_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create range list.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     internal_range_list,
	     0,
	     sizeof( libcdata_internal_range_list_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear range list.",
		 function );

		memory_free(
		 internal_range_list );

		return( -1 );
	}
#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	if( libcthreads_read_write_lock_initialize(
	     &( internal_range_list->read_write_lock ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize read/write lock.",
		 function );

		goto on_error;
	}
#endif
	*range_list = (libcdata_range_list_t *) internal_range_list;

	return( 1 );

on_error:
	if( internal_range_list != NULL )
	{
		memory_free(
		 internal_range_list );
	}
	return( -1 );
}

/* Frees a range list including the elements
 * Uses the value_free_function to free the element value
 * Returns 1 if successful or -1 on error
 */
int libcdata_range_list_free(
     libcdata_range_list_t **range_list,
     int (*value_free_function)(
            intptr_t **value,
            libcerror_error_t **error ),
     libcerror_error_t **error )
{
	libcdata_internal_range_list_t *internal_range_list = NULL;
	static char *function                               = "libcdata_range_list_free";
	int result                                          = 1;

	if( range_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid range list.",
		 function );

		return( -1 );
	}
	if( *range_list != NULL )
	{
		internal_range_list = (libcdata_internal_range_list_t *) *range_list;
		*range_list         = NULL;

		if( libcdata_range_list_empty(
		     (libcdata_range_list_t *) internal_range_list,
		     value_free_function,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to empty range list.",
			 function );

			result = -1;
		}
#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
		if( libcthreads_read_write_lock_free(
		     &( internal_range_list->read_write_lock ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free read/write lock.",
			 function );

			result = -1;
		}
#endif
		memory_free(
		 internal_range_list );
	}
	return( result );
}

/* Frees a range list element
 * Uses the value_free_function to free the element value
 * Returns 1 if successful or -1 on error
 */
int libcdata_internal_range_list_free_element(
     libcdata_internal_range_list_t *internal_range_list,
     libcdata_list_element_t **range_list_element,
     int (*value_free_function)(
            intptr_t **value,
            libcerror_error_t **error ),
     libcerror_error_t **error )
{
	libcdata_range_list_value_t *range_list_value = NULL;
	static char *function                         = "libcdata_internal_range_list_free_element";
	int result                                    = 1;

	if( internal_range_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid range list.",
		 function );

		return( -1 );
	}
	if( range_list_element == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid range list element.",
		 function );

		return( -1 );
	}
	if( *range_list_element != NULL )
	{
		if( libcdata_list_element_get_value(
		     *range_list_element,
		     (intptr_t **) &range_list_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value from range list element.",
			 function );

			return( -1 );
		}
		if( libcdata_range_list_value_free(
		     &range_list_value,
		     value_free_function,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free range list value.",
			 function );

			result = -1;
		}
		if( libcdata_list_element_free(
		     range_list_element,
		     NULL,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free list element.",
			 function );

			result = -1;
		}
	}
	return( result );
}

/* Empties a range list and frees the elements
 * Uses the value_free_function to free the element value
 * Returns 1 if successful or -1 on error
 */
int libcdata_range_list_empty(
     libcdata_range_list_t *range_list,
     int (*value_free_function)(
            intptr_t **value,
            libcerror_error_t **error ),
     libcerror_error_t **error )
{
	libcdata_internal_range_list_t *internal_range_list = NULL;
	libcdata_list_element_t *list_element               = NULL;
	libcdata_list_element_t *next_element               = NULL;
	static char *function                               = "libcdata_range_list_empty";
	int element_index                                   = 0;
	int number_of_elements                              = 0;
	int result                                          = 1;

	if( range_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid range list.",
		 function );

		return( -1 );
	}
	internal_range_list = (libcdata_internal_range_list_t *) range_list;

#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_range_list->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( internal_range_list->number_of_elements > 0 )
	{
		number_of_elements = internal_range_list->number_of_elements;
		list_element       = internal_range_list->first_element;

		for( element_index = 0;
		     element_index < number_of_elements;
		     element_index++ )
		{
			if( libcdata_list_element_get_next_element(
			     list_element,
			     &next_element,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve next element from list element: %d.",
				 function,
				 element_index );

				result = -1;

				break;
			}
			internal_range_list->first_element = next_element;

			if( internal_range_list->last_element == list_element )
			{
				internal_range_list->last_element = next_element;
			}
			internal_range_list->number_of_elements -= 1;

			if( next_element != NULL )
			{
				if( libcdata_list_element_set_previous_element(
				     next_element,
				     NULL,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
					 "%s: unable to set previous element of list element: %d.",
					 function,
					 element_index + 1 );

					result = -1;
				}
			}
			if( libcdata_list_element_set_next_element(
			     list_element,
			     NULL,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set next element of list element: %d.",
				 function,
				 element_index );

				result = -1;
			}
			if( libcdata_internal_range_list_free_element(
			     internal_range_list,
			     &list_element,
			     value_free_function,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free range list element: %d.",
				 function,
				 element_index );

				result = -1;
			}
			list_element = next_element;
		}
		internal_range_list->current_element       = NULL;
		internal_range_list->current_element_index = 0;
	}
#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_range_list->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		result = -1;
	}
#endif
	return( result );
}

/* Clones the range list
 *
 * The values are cloned using the value_clone_function
 * On error the values are freed using the value_free_function
 *
 * Returns 1 if successful or -1 on error
 */
int libcdata_range_list_clone(
     libcdata_range_list_t **destination_range_list,
     libcdata_range_list_t *source_range_list,
     int (*value_free_function)(
            intptr_t **value,
            libcerror_error_t **error ),
     int (*value_clone_function)(
            intptr_t **destination,
            intptr_t *source,
            libcerror_error_t **error ),
     libcerror_error_t **error )
{
	libcdata_internal_list_element_t *internal_source_list_element  = NULL;
	libcdata_internal_range_list_t *internal_destination_range_list = NULL;
	libcdata_internal_range_list_t *internal_source_range_list      = NULL;
	libcdata_range_list_value_t *destination_range_list_value       = NULL;
	static char *function                                           = "libcdata_range_list_clone";
	int element_index                                               = 0;

	if( destination_range_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid destination range list.",
		 function );

		return( -1 );
	}
	if( *destination_range_list != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid destination range list value already set.",
		 function );

		return( -1 );
	}
	if( source_range_list == NULL )
	{
		*destination_range_list = NULL;

		return( 1 );
	}
	internal_source_range_list = (libcdata_internal_range_list_t *) source_range_list;

#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_source_range_list->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( libcdata_range_list_initialize(
	     (libcdata_range_list_t **) &internal_destination_range_list,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create destination range list.",
		 function );

		goto on_error;
	}
	if( internal_destination_range_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing destination range list.",
		 function );

		goto on_error;
	}
	if( internal_source_range_list->first_element != NULL )
	{
		internal_source_list_element = (libcdata_internal_list_element_t *) internal_source_range_list->first_element;

		for( element_index = 0;
		     element_index < internal_source_range_list->number_of_elements;
		     element_index++ )
		{
			if( internal_source_list_element == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: missing source list element: %d.",
				 function,
				 element_index );

				goto on_error;
			}
			if( libcdata_range_list_value_clone(
			     &destination_range_list_value,
			     (libcdata_range_list_value_t *) internal_source_list_element->value,
			     value_free_function,
			     value_clone_function,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create destination range list value: %d.",
				 function,
				 element_index );

				goto on_error;
			}
			if( libcdata_internal_range_list_append_value(
			     internal_destination_range_list,
			     destination_range_list_value,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to append value: %d to destination range list.",
				 function,
				 element_index );

				goto on_error;
			}
			destination_range_list_value = NULL;

			internal_source_list_element = (libcdata_internal_list_element_t *) internal_source_list_element->next_element;
		}
	}
#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_source_range_list->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		libcdata_range_list_free(
		 (libcdata_range_list_t **) &internal_destination_range_list,
		 value_free_function,
		 NULL );

		return( -1 );
	}
#endif
	*destination_range_list = (libcdata_range_list_t *) internal_destination_range_list;

	return( 1 );

on_error:
#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	libcthreads_read_write_lock_release_for_read(
	 internal_source_range_list->read_write_lock,
	 NULL );
#endif
	if( destination_range_list_value != NULL )
	{
		libcdata_range_list_value_free(
		 &destination_range_list_value,
		 value_free_function,
		 NULL );
	}
	if( internal_destination_range_list != NULL )
	{
		libcdata_range_list_free(
		 (libcdata_range_list_t **) &internal_destination_range_list,
		 value_free_function,
		 NULL );
	}
	return( -1 );
}

/* Retrieves the number of elements in the range list
 * Returns 1 if successful or -1 on error
 */
int libcdata_range_list_get_number_of_elements(
     libcdata_range_list_t *range_list,
     int *number_of_elements,
     libcerror_error_t **error )
{
	libcdata_internal_range_list_t *internal_range_list = NULL;
	static char *function                               = "libcdata_range_list_get_number_of_elements";

	if( range_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid range list.",
		 function );

		return( -1 );
	}
	internal_range_list = (libcdata_internal_range_list_t *) range_list;

	if( number_of_elements == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid number of elements.",
		 function );

		return( -1 );
	}
#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_range_list->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	*number_of_elements = internal_range_list->number_of_elements;

#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_range_list->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );
}

/* Retrieves the first elements in the range list
 * Returns 1 if successful or -1 on error
 */
int libcdata_range_list_get_first_element(
     libcdata_range_list_t *range_list,
     libcdata_list_element_t **element,
     libcerror_error_t **error )
{
	libcdata_internal_range_list_t *internal_range_list = NULL;
	static char *function                               = "libcdata_range_list_get_first_element";

	if( range_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid range list.",
		 function );

		return( -1 );
	}
	internal_range_list = (libcdata_internal_range_list_t *) range_list;

	if( element == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid element.",
		 function );

		return( -1 );
	}
#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_range_list->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	*element = internal_range_list->first_element;

#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_range_list->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );
}

/* Sets the first element in the range list
 * Returns 1 if successful or -1 on error
 */
int libcdata_internal_range_list_set_first_element(
     libcdata_internal_range_list_t *internal_range_list,
     libcdata_list_element_t *element,
     libcerror_error_t **error )
{
	libcdata_list_element_t *backup_first_element = NULL;
	libcdata_list_element_t *backup_next_element  = NULL;
	static char *function                         = "libcdata_internal_range_list_set_first_element";

	if( internal_range_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid range list.",
		 function );

		return( -1 );
	}
	if( element != NULL )
	{
		if( libcdata_list_element_get_next_element(
		     element,
		     &backup_next_element,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve next element of element.",
			 function );

			return( -1 );
		}
	}
	backup_first_element = internal_range_list->first_element;

	if( element != NULL )
	{
		if( libcdata_list_element_set_next_element(
		     element,
		     internal_range_list->first_element,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set next element of element.",
			 function );

			goto on_error;
		}
	}
	if( internal_range_list->first_element != NULL )
	{
		if( libcdata_list_element_set_previous_element(
		     internal_range_list->first_element,
		     element,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set previous element of first element.",
			 function );

			goto on_error;
		}
	}
	internal_range_list->first_element = element;

	return( 1 );

on_error:
	if( element != NULL )
	{
		libcdata_list_element_set_next_element(
		 element,
		 backup_next_element,
		 NULL );
	}
	if( backup_first_element != NULL )
	{
		libcdata_list_element_set_next_element(
		 backup_first_element,
		 NULL,
		 NULL );
	}
	internal_range_list->first_element = backup_first_element;

	return( -1 );
}

/* Retrieves the last elements in the range list
 * Returns 1 if successful or -1 on error
 */
int libcdata_range_list_get_last_element(
     libcdata_range_list_t *range_list,
     libcdata_list_element_t **element,
     libcerror_error_t **error )
{
	libcdata_internal_range_list_t *internal_range_list = NULL;
	static char *function                               = "libcdata_range_list_get_last_element";

	if( range_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid range list.",
		 function );

		return( -1 );
	}
	internal_range_list = (libcdata_internal_range_list_t *) range_list;

	if( element == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid element.",
		 function );

		return( -1 );
	}
#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_range_list->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	*element = internal_range_list->last_element;

#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_range_list->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );
}

/* Sets the last element in the list
 * Returns 1 if successful or -1 on error
 */
int libcdata_internal_range_list_set_last_element(
     libcdata_internal_range_list_t *internal_range_list,
     libcdata_list_element_t *element,
     libcerror_error_t **error )
{
	libcdata_list_element_t *backup_last_element     = NULL;
	libcdata_list_element_t *backup_previous_element = NULL;
	static char *function                            = "libcdata_internal_range_list_set_last_element";

	if( internal_range_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid range list.",
		 function );

		return( -1 );
	}
	if( element != NULL )
	{
		if( libcdata_list_element_get_previous_element(
		     element,
		     &backup_previous_element,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve previous element of element.",
			 function );

			return( -1 );
		}
	}
	backup_last_element = internal_range_list->last_element;

	if( element != NULL )
	{
		if( libcdata_list_element_set_previous_element(
		     element,
		     internal_range_list->last_element,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set previous element of element.",
			 function );

			goto on_error;
		}
	}
	if( internal_range_list->last_element != NULL )
	{
		if( libcdata_list_element_set_next_element(
		     internal_range_list->last_element,
		     element,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set next element of last element.",
			 function );

			goto on_error;
		}
	}
	internal_range_list->last_element = element;

	return( 1 );

on_error:
	if( element != NULL )
	{
		libcdata_list_element_set_previous_element(
		 element,
		 backup_previous_element,
		 NULL );
	}
	if( backup_last_element != NULL )
	{
		libcdata_list_element_set_next_element(
		 backup_last_element,
		 NULL,
		 NULL );
	}
	internal_range_list->last_element = backup_last_element;

	return( -1 );
}

/* Append a list element to the list
 * Returns 1 if successful or -1 on error
 */
int libcdata_internal_range_list_append_element(
     libcdata_internal_range_list_t *internal_range_list,
     libcdata_list_element_t *element,
     libcerror_error_t **error )
{
	static char *function = "libcdata_internal_range_list_append_element";

	if( internal_range_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid range list.",
		 function );

		return( -1 );
	}
	if( element == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid list element.",
		 function );

		return( -1 );
	}
	if( internal_range_list->first_element == NULL )
	{
		internal_range_list->first_element = element;
	}
	if( libcdata_internal_range_list_set_last_element(
	     internal_range_list,
	     element,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set last element.",
		 function );

		return( -1 );
	}
	internal_range_list->number_of_elements += 1;

	return( 1 );
}

/* Append a value to the list
 * Creates a new list element
 * Returns 1 if successful or -1 on error
 */
int libcdata_internal_range_list_append_value(
     libcdata_internal_range_list_t *internal_range_list,
     libcdata_range_list_value_t *value,
     libcerror_error_t **error )
{
	libcdata_list_element_t *list_element = NULL;
	static char *function                 = "libcdata_internal_range_list_append_value";

	if( libcdata_list_element_initialize(
	     &list_element,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create list element.",
		 function );

		goto on_error;
	}
	if( libcdata_list_element_set_value(
	     list_element,
	     (intptr_t *) value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to set value of list element.",
		 function );

		goto on_error;
	}
	if( libcdata_internal_range_list_append_element(
	     internal_range_list,
	     list_element,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append element to range list.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( list_element != NULL )
	{
		libcdata_list_element_free(
		 &list_element,
		 NULL,
		 NULL );
	}
	return( -1 );
}

/* Removes an element from the range list
 * Returns 1 if successful, or -1 on error
 */
int libcdata_internal_range_list_remove_element(
     libcdata_internal_range_list_t *internal_range_list,
     libcdata_list_element_t *range_list_element,
     libcerror_error_t **error )
{
	libcdata_list_element_t *next_element     = NULL;
	libcdata_list_element_t *previous_element = NULL;
	static char *function                     = "libcdata_internal_range_list_remove_element";

	if( internal_range_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid range list.",
		 function );

		return( -1 );
	}
	if( libcdata_list_element_get_elements(
	     range_list_element,
	     &previous_element,
	     &next_element,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve previous and next element from range list element.",
		 function );

		return( -1 );
	}
	if( range_list_element == internal_range_list->first_element )
	{
		internal_range_list->first_element = next_element;
	}
	if( range_list_element == internal_range_list->last_element )
	{
		internal_range_list->last_element = previous_element;
	}
	if( next_element != NULL )
	{
		if( libcdata_list_element_set_previous_element(
		     next_element,
		     previous_element,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set previous element of next element.",
			 function );

			return( -1 );
		}
	}
	if( previous_element != NULL )
	{
		if( libcdata_list_element_set_next_element(
		     previous_element,
		     next_element,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set next element of previous element.",
			 function );

			return( -1 );
		}
	}
	if( libcdata_list_element_set_elements(
	     range_list_element,
	     NULL,
	     NULL,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set previous and next element of range list element.",
		 function );

		return( -1 );
	}
	internal_range_list->current_element       = NULL;
	internal_range_list->current_element_index = 0;

	internal_range_list->number_of_elements -= 1;

	return( 1 );
}

/* Removes an element from the range list and frees the element
 * Returns 1 if successful, or -1 on error
 */
int libcdata_internal_range_list_remove_range_value(
     libcdata_internal_range_list_t *internal_range_list,
     libcdata_list_element_t **range_list_element,
     int (*value_free_function)(
            intptr_t **value,
            libcerror_error_t **error ),
     libcerror_error_t **error )
{
	libcdata_range_list_value_t *range_list_value = NULL;
	static char *function                         = "libcdata_internal_range_list_remove_range_value";
	int result                                    = 1;

	if( internal_range_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid range list.",
		 function );

		return( -1 );
	}
	if( range_list_element == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid range list element.",
		 function );

		return( -1 );
	}
	if( libcdata_internal_range_list_remove_element(
	     internal_range_list,
	     *range_list_element,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_REMOVE_FAILED,
		 "%s: unable to remove range list element.",
		 function );

		return( -1 );
	}
	if( libcdata_list_element_get_value(
	     *range_list_element,
	     (intptr_t **) &range_list_value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve range list value from range list element.",
		 function );

		return( -1 );
	}
	if( libcdata_list_element_free(
	     range_list_element,
	     NULL,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free range list element.",
		 function );

		result = -1;
	}
	if( libcdata_range_list_value_free(
	     &range_list_value,
	     value_free_function,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free range list value.",
		 function );

		result = -1;
	}
	return( result );
}

/* Retrieves the element which the range should be inserted before or merged within
 *
 * On return element will be set to NULL if the range should be inserted at the end of the list.
 *
 * Returns 1 if successful or -1 on error
 */
int libcdata_internal_range_list_insert_range_find_element(
     libcdata_internal_range_list_t *internal_range_list,
     uint64_t range_start,
     uint64_t range_end LIBCDATA_ATTRIBUTE_UNUSED,
     int *element_index,
     libcdata_list_element_t **element,
     libcerror_error_t **error )
{
	libcdata_list_element_t *current_element      = NULL;
	libcdata_list_element_t *previous_element     = NULL;
	libcdata_range_list_value_t *range_list_value = NULL;
	static char *function                         = "libcdata_internal_range_list_insert_range_find_element";
	int current_element_index                     = 0;
	int last_element_index                        = 0;

	LIBCDATA_UNREFERENCED_PARAMETER( range_end )

	if( internal_range_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid range list.",
		 function );

		return( -1 );
	}
	if( element == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid element.",
		 function );

		return( -1 );
	}
	if( element_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid element index.",
		 function );

		return( -1 );
	}
	current_element       = internal_range_list->last_element;
	current_element_index = internal_range_list->number_of_elements;

	if( internal_range_list->number_of_elements > 0 )
	{
		/* Check the last element first, most often the list will be filled linear
		 */
		current_element_index--;

		if( libcdata_list_element_get_value(
		     current_element,
		     (intptr_t **) &range_list_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value from list element: %d.",
			 function,
			 current_element_index );

			return( -1 );
		}
		if( range_list_value == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing range list value element: %d.",
			 function,
			 current_element_index );

			return( -1 );
		}
		if( range_start > range_list_value->end )
		{
			current_element = NULL;

			current_element_index++;
		}
		else if( internal_range_list->number_of_elements > 1 )
		{
			last_element_index = current_element_index;

			if( ( current_element_index != internal_range_list->current_element_index )
			 && ( internal_range_list->current_element != NULL ) )
			{
				/* Check the current element
				 */
				current_element       = internal_range_list->current_element;
				current_element_index = internal_range_list->current_element_index;

				if( libcdata_list_element_get_value(
				     current_element,
				     (intptr_t **) &range_list_value,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve value from list element: %d.",
					 function,
					 current_element_index );

					return( -1 );
				}
				if( range_list_value == NULL )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
					 "%s: missing range list value element: %d.",
					 function,
					 current_element_index );

					return( -1 );
				}
			}
			if( range_start > range_list_value->end )
			{
				while( current_element_index < last_element_index )
				{
					if( libcdata_list_element_get_next_element(
					     current_element,
					     &current_element,
					     error ) != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
						 "%s: unable to retrieve next element from list element: %d.",
						 function,
						 current_element_index );

						return( -1 );
					}
					current_element_index++;

					if( libcdata_list_element_get_value(
					     current_element,
					     (intptr_t **) &range_list_value,
					     error ) != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
						 "%s: unable to retrieve value from list element: %d.",
						 function,
						 current_element_index );

						return( -1 );
					}
					if( range_list_value == NULL )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
						 "%s: missing range list value element: %d.",
						 function,
						 current_element_index );

						return( -1 );
					}
					if( range_start <= range_list_value->end )
					{
						break;
					}
				}
			}
			else
			{
				while( current_element_index > 0 )
				{
					previous_element = current_element;

					if( libcdata_list_element_get_previous_element(
					     current_element,
					     &current_element,
					     error ) != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
						 "%s: unable to retrieve previous element from list element: %d.",
						 function,
						 current_element_index );

						return( -1 );
					}
					current_element_index--;

					if( libcdata_list_element_get_value(
					     current_element,
					     (intptr_t **) &range_list_value,
					     error ) != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
						 "%s: unable to retrieve value from list element: %d.",
						 function,
						 current_element_index );

						return( -1 );
					}
					if( range_list_value == NULL )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
						 "%s: missing range list value element: %d.",
						 function,
						 current_element_index );

						return( -1 );
					}
					if( range_start > range_list_value->end )
					{
						current_element = previous_element;

						current_element_index++;

						break;
					}
				}
			}
		}
	}
	if( current_element != NULL )
	{
		internal_range_list->current_element       = current_element;
		internal_range_list->current_element_index = current_element_index;
	}
	*element       = current_element;
	*element_index = current_element_index;

	return( 1 );
}

/* Checks if the range overlaps with the value of the range list element
 * Returns 1 if the range overlaps, 0 if not or -1 on error
 */
int libcdata_internal_range_list_check_range_overlap(
     libcdata_internal_range_list_t *internal_range_list,
     libcdata_list_element_t *range_list_element,
     uint64_t range_start,
     uint64_t range_end,
     libcerror_error_t **error )
{
	libcdata_range_list_value_t *range_list_value = NULL;
	static char *function                         = "libcdata_internal_range_list_check_range_overlap";
	int result                                    = 0;

	if( internal_range_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid range list.",
		 function );

		return( -1 );
	}
	if( range_list_element != NULL )
	{
		if( libcdata_list_element_get_value(
		     range_list_element,
		     (intptr_t **) &range_list_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value from range list element.",
			 function );

			return( -1 );
		}
		result = libcdata_range_list_value_check_range_overlap(
		          range_list_value,
		          range_start,
		          range_end,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine if range overlaps with range list value.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Inserts the range before the range list element
 * If range_list_element is NULL the element is inserted before or as the first element in the list
 * Returns 1 if successful, or -1 on error
 */
int libcdata_internal_range_list_insert_range_before_element(
     libcdata_internal_range_list_t *internal_range_list,
     libcdata_list_element_t *range_list_element,
     uint64_t range_start,
     uint64_t range_end,
     intptr_t *value,
     libcdata_list_element_t **new_range_list_element,
     libcerror_error_t **error )
{
	libcdata_list_element_t *backup_first_element     = NULL;
	libcdata_list_element_t *backup_last_element      = NULL;
	libcdata_list_element_t *new_element              = NULL;
	libcdata_list_element_t *previous_element         = NULL;
	libcdata_range_list_value_t *new_range_list_value = NULL;
	static char *function                             = "libcdata_internal_range_list_insert_range_before_element";

	if( internal_range_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid range list.",
		 function );

		return( -1 );
	}
	if( new_range_list_element == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid new range list element.",
		 function );

		return( -1 );
	}
	if( range_list_element != NULL )
	{
		if( libcdata_list_element_get_previous_element(
		     range_list_element,
		     &previous_element,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve previous element from range list element.",
			 function );

			return( -1 );
		}
	}
	if( libcdata_range_list_value_initialize(
	     &new_range_list_value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create range list value.",
		 function );

		goto on_error;
	}
	backup_first_element = internal_range_list->first_element;
	backup_last_element  = internal_range_list->last_element;

	if( new_range_list_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing range list value.",
		 function );

		goto on_error;
	}
	new_range_list_value->start = range_start;
	new_range_list_value->size  = range_end - range_start;
	new_range_list_value->end   = range_end;
	new_range_list_value->value = value;

	if( libcdata_list_element_initialize(
	     &new_element,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create list element.",
		 function );

		goto on_error;
	}
	if( libcdata_list_element_set_value(
	     new_element,
	     (intptr_t *) new_range_list_value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set value in list element.",
		 function );

		goto on_error;
	}
	if( internal_range_list->number_of_elements == 0 )
	{
		internal_range_list->first_element = new_element;
		internal_range_list->last_element  = new_element;
	}
	else if( range_list_element == NULL )
	{
		if( libcdata_internal_range_list_set_last_element(
		     internal_range_list,
		     new_element,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set last element.",
			 function );

			goto on_error;
		}
	}
	else
	{
		if( libcdata_list_element_set_elements(
		     new_element,
		     previous_element,
		     range_list_element,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set previous and next element of element.",
			 function );

			goto on_error;
		}
		if( internal_range_list->first_element == range_list_element )
		{
			internal_range_list->first_element = new_element;
		}
		else
		{
			if( libcdata_list_element_set_next_element(
			     previous_element,
			     new_element,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set next element of previous element.",
				 function );

				goto on_error;
			}
		}
		if( libcdata_list_element_set_previous_element(
		     range_list_element,
		     new_element,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set previous element of range list element.",
			 function );

			goto on_error;
		}
	}
	internal_range_list->current_element       = NULL;
	internal_range_list->current_element_index = 0;

	internal_range_list->number_of_elements += 1;

	*new_range_list_element = new_element;

	return( 1 );

on_error:
	if( new_element != NULL )
	{
		libcdata_list_element_set_elements(
		 new_element,
		 NULL,
		 NULL,
		 NULL );

		libcdata_list_element_free(
		 &new_element,
		 NULL,
		 NULL );
	}
	if( new_range_list_value != NULL )
	{
		libcdata_range_list_value_free(
		 &new_range_list_value,
		 NULL,
		 NULL );
	}
	if( previous_element != NULL )
	{
		libcdata_list_element_set_next_element(
		 previous_element,
		 range_list_element,
		 NULL );
	}
	if( range_list_element != NULL )
	{
		libcdata_list_element_set_previous_element(
		 range_list_element,
		 previous_element,
		 NULL );
	}
	internal_range_list->first_element = backup_first_element;
	internal_range_list->last_element  = backup_last_element;

	return( -1 );
}

/* Inserts the element in the range list after the range list element
 * If range_list_element is NULL the element is inserted as the first element in the list
 * Returns 1 if successful, or -1 on error
 */
int libcdata_internal_range_list_insert_element_after_element(
     libcdata_internal_range_list_t *internal_range_list,
     libcdata_list_element_t *range_list_element,
     libcdata_list_element_t *element,
     libcerror_error_t **error )
{
	libcdata_list_element_t *next_element     = NULL;
	libcdata_list_element_t *previous_element = NULL;
	static char *function                     = "libcdata_internal_range_list_insert_element_after_element";

	if( internal_range_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid range list.",
		 function );

		return( -1 );
	}
	if( libcdata_list_element_get_elements(
	     element,
	     &previous_element,
	     &next_element,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve previous and next element from list element.",
		 function );

		return( -1 );
	}
	if( ( previous_element != NULL )
	 || ( next_element != NULL ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: list element already part of a list.",
		 function );

		return( -1 );
	}
	if( internal_range_list->number_of_elements == 0 )
	{
		if( internal_range_list->first_element != NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid range list - first element already set.",
			 function );

			return( -1 );
		}
		if( internal_range_list->last_element != NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid range list - last element already set.",
			 function );

			return( -1 );
		}
		internal_range_list->first_element = element;
		internal_range_list->last_element  = element;
	}
	else
	{
		if( internal_range_list->first_element == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid range list - missing first element.",
			 function );

			return( -1 );
		}
		if( internal_range_list->last_element == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid range list - missing last element.",
			 function );

			return( -1 );
		}
		if( range_list_element == NULL )
		{
			if( libcdata_internal_range_list_set_first_element(
			     internal_range_list,
			     element,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set first element.",
				 function );

				return( -1 );
			}
		}
		else
		{
			if( libcdata_list_element_get_next_element(
			     range_list_element,
			     &next_element,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve next element from range list element.",
				 function );

				return( -1 );
			}
			if( libcdata_list_element_set_elements(
			     element,
			     range_list_element,
			     next_element,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set previous and next element of list element.",
				 function );

				return( -1 );
			}
			if( range_list_element == internal_range_list->last_element )
			{
				internal_range_list->last_element = element;
			}
			else if( next_element == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: invalid range list element - missing next element.",
				 function );

				return( -1 );
			}
			else
			{
				if( libcdata_list_element_set_previous_element(
				     next_element,
				     element,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
					 "%s: unable to set previous element of next element.",
					 function );

					return( -1 );
				}
			}
			if( libcdata_list_element_set_next_element(
			     range_list_element,
			     element,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set next element of range list element.",
				 function );

				return( -1 );
			}
		}
	}
	internal_range_list->number_of_elements += 1;

	return( 1 );
}

/* Inserts the range list value in the range list after the range list element
 * If range_list_element is NULL the value is inserted before the first element in the list
 * Returns 1 if successful, or -1 on error
 */
int libcdata_internal_range_list_insert_value_after_element(
     libcdata_internal_range_list_t *internal_range_list,
     libcdata_list_element_t *range_list_element,
     libcdata_range_list_value_t *value,
     libcerror_error_t **error )
{
	libcdata_list_element_t *list_element = NULL;
	static char *function                 = "libcdata_internal_range_list_insert_value_after_element";

	if( internal_range_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid range list.",
		 function );

		return( -1 );
	}
	if( libcdata_list_element_initialize(
	     &list_element,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create list element.",
		 function );

		goto on_error;
	}
	if( libcdata_list_element_set_value(
	     list_element,
	     (intptr_t *) value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set value in list element.",
		 function );

		goto on_error;
	}
	if( libcdata_internal_range_list_insert_element_after_element(
	     internal_range_list,
	     range_list_element,
	     list_element,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to insert element after range list element.",
		 function );

		goto on_error;
	}
	list_element = NULL;

	return( 1 );

on_error:
	if( list_element != NULL )
	{
		libcdata_list_element_free(
		 &list_element,
		 NULL,
		 NULL );
	}
	return( -1 );
}

/* Merges the range into the range list element
 * Returns 1 if successful, or -1 on error
 */
int libcdata_internal_range_list_merge_range(
     libcdata_internal_range_list_t *internal_range_list,
     libcdata_list_element_t *range_list_element,
     uint64_t range_start,
     uint64_t range_end,
     intptr_t *value,
     int (*value_merge_function)(
            intptr_t *destination_value,
            intptr_t *source_value,
            libcerror_error_t **error ),
     libcerror_error_t **error )
{
	libcdata_range_list_value_t *range_list_value = NULL;
	static char *function                         = "libcdata_internal_range_list_merge_range";

	if( internal_range_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid range list.",
		 function );

		return( -1 );
	}
	if( libcdata_list_element_get_value(
	     range_list_element,
	     (intptr_t **) &range_list_value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value from range list element.",
		 function );

		return( -1 );
	}
	if( range_list_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing range list value.",
		 function );

		return( -1 );
	}
	if( range_list_value->value != NULL )
	{
		if( value_merge_function == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid value merge function.",
			 function );

			return( -1 );
		}
	}
	if( range_list_value->value == NULL )
	{
		range_list_value->value = value;
	}
	else
	{
		if( value_merge_function(
		     range_list_value->value,
		     value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to merge value.",
			 function );

			return( -1 );
		}
	}
	if( range_start < range_list_value->start )
	{
		range_list_value->size += range_list_value->start - range_start;
		range_list_value->start = range_start;
	}
	if( range_end > range_list_value->end )
	{
		range_list_value->size += range_end - range_list_value->end;
		range_list_value->end   = range_end;
	}
	return( 1 );
}

/* Merges successive overlapping ranges
 * Returns 1 if successful, or -1 on error
 */
int libcdata_internal_range_list_merge_overlapping_ranges(
     libcdata_internal_range_list_t *internal_range_list,
     libcdata_list_element_t *range_list_element,
     int (*value_merge_function)(
            intptr_t *destination_value,
            intptr_t *source_value,
            libcerror_error_t **error ),
     libcdata_range_list_t *backup_range_list,
     libcerror_error_t **error )
{
	libcdata_list_element_t *list_element              = NULL;
	libcdata_list_element_t *next_element              = NULL;
	libcdata_range_list_value_t *next_range_list_value = NULL;
	static char *function                              = "libcdata_internal_range_list_merge_overlapping_ranges";
	int result                                         = 0;

	if( internal_range_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid range list.",
		 function );

		return( -1 );
	}
	if( libcdata_list_element_get_next_element(
	     range_list_element,
	     &next_element,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve next element from range list element.",
		 function );

		return( -1 );
	}
	while( next_element != NULL )
	{
		list_element = next_element;

		if( libcdata_list_element_get_next_element(
		     list_element,
		     &next_element,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve next element.",
			 function );

			return( -1 );
		}
		if( libcdata_list_element_get_value(
		     list_element,
		     (intptr_t **) &next_range_list_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value from next element.",
			 function );

			return( -1 );
		}
		if( next_range_list_value == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing next range list value.",
			 function );

			return( -1 );
		}
		result = libcdata_internal_range_list_check_range_overlap(
		          internal_range_list,
		          range_list_element,
		          next_range_list_value->start,
		          next_range_list_value->end,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine if next range overlaps with range list element.",
			 function );

			return( -1 );
		}
		else if( result == 0 )
		{
			break;
		}
		if( libcdata_internal_range_list_append_value(
		     (libcdata_internal_range_list_t *) backup_range_list,
		     next_range_list_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to append value to backup range list.",
			 function );

			return( -1 );
		}
		if( libcdata_internal_range_list_remove_element(
		     internal_range_list,
		     list_element,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_REMOVE_FAILED,
			 "%s: unable to remove list element.",
			 function );

			return( -1 );
		}
		if( libcdata_list_element_free(
		     &list_element,
		     NULL,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free list element.",
			 function );

			return( -1 );
		}
		if( libcdata_internal_range_list_merge_range(
		     internal_range_list,
		     range_list_element,
		     next_range_list_value->start,
		     next_range_list_value->end,
		     next_range_list_value->value,
		     value_merge_function,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to merge next range with range list element.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Inserts a range by merging the range into the range list element and successive overlapping ranges
 * Returns 1 if successful or -1 on error
 */
int libcdata_internal_range_list_insert_range_merge(
     libcdata_internal_range_list_t *internal_range_list,
     libcdata_list_element_t *range_list_element,
     uint64_t range_start,
     uint64_t range_end,
     intptr_t *value,
     int (*value_merge_function)(
            intptr_t *destination_value,
            intptr_t *source_value,
            libcerror_error_t **error ),
     libcdata_range_list_t *backup_range_list,
     libcerror_error_t **error )
{
	libcdata_range_list_value_t *backup_range_list_value = NULL;
	libcdata_range_list_value_t *range_list_value        = NULL;
	static char *function                                = "libcdata_internal_range_list_insert_range_merge";

	if( internal_range_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid range list.",
		 function );

		return( -1 );
	}
	if( libcdata_list_element_get_value(
	     range_list_element,
	     (intptr_t **) &range_list_value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value from range list element.",
		 function );

		goto on_error;
	}
	if( libcdata_range_list_value_initialize(
	     &backup_range_list_value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create backup range list value.",
		 function );

		goto on_error;
	}
	backup_range_list_value->start = range_list_value->start;
	backup_range_list_value->end   = range_list_value->end;
	backup_range_list_value->size  = range_list_value->size;

	if( libcdata_internal_range_list_append_value(
	     (libcdata_internal_range_list_t *) backup_range_list,
	     backup_range_list_value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to append value to backup range list.",
		 function );

		goto on_error;
	}
	backup_range_list_value = NULL;

	if( libcdata_internal_range_list_merge_range(
	     internal_range_list,
	     range_list_element,
	     range_start,
	     range_end,
	     value,
	     value_merge_function,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to merge range with range list element.",
		 function );

		goto on_error;
	}
	if( libcdata_internal_range_list_merge_overlapping_ranges(
	     internal_range_list,
	     range_list_element,
	     value_merge_function,
	     backup_range_list,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to merge range list element with overlapping ranges.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( backup_range_list_value != NULL )
	{
		libcdata_range_list_value_free(
		 &backup_range_list_value,
		 NULL,
		 NULL );
	}
	return( -1 );
}

/* Reverts a previously merge of the range list element and successive overlapping ranges
 * Returns 1 if successful or -1 on error
 */
int libcdata_internal_range_list_insert_range_revert_merge(
     libcdata_internal_range_list_t *internal_range_list,
     libcdata_list_element_t *range_list_element,
     libcdata_range_list_t *backup_range_list,
     libcerror_error_t **error )
{
	libcdata_internal_range_list_t *internal_backup_range_list = NULL;
	libcdata_list_element_t *backup_range_list_element         = NULL;
	libcdata_range_list_value_t *backup_range_list_value       = NULL;
	libcdata_range_list_value_t *range_list_value              = NULL;
	static char *function                                      = "libcdata_internal_range_list_insert_range_revert_merge";

	if( internal_range_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid range list.",
		 function );

		return( -1 );
	}
	if( backup_range_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid backup range list.",
		 function );

		return( -1 );
	}
	internal_backup_range_list = (libcdata_internal_range_list_t *) backup_range_list;

	if( internal_backup_range_list->first_element == NULL )
	{
		return( 1 );
	}
	if( libcdata_list_element_get_value(
	     internal_backup_range_list->first_element,
	     (intptr_t **) &backup_range_list_value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value from first backup range list element.",
		 function );

		return( -1 );
	}
	if( libcdata_list_element_get_value(
	     range_list_element,
	     (intptr_t **) &range_list_value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value from range list element.",
		 function );

		return( -1 );
	}
	range_list_value->start = backup_range_list_value->start;
	range_list_value->end   = backup_range_list_value->end;
	range_list_value->size  = backup_range_list_value->size;

	while( internal_backup_range_list->first_element != NULL )
	{
		if( libcdata_list_element_get_next_element(
		     internal_backup_range_list->first_element,
		     &backup_range_list_element,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve next element from first backup range list element.",
			 function );

			return( -1 );
		}
		if( backup_range_list_element == NULL )
		{
			break;
		}
		if( libcdata_internal_range_list_remove_element(
		     internal_backup_range_list,
		     backup_range_list_element,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_REMOVE_FAILED,
			 "%s: unable to remove backup range list element.",
			 function );

			return( -1 );
		}
		if( libcdata_internal_range_list_insert_element_after_element(
		     internal_range_list,
		     range_list_element,
		     backup_range_list_element,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to insert element after range list element.",
			 function );

			return( -1 );
		}
		backup_range_list_element = NULL;
	}
	return( 1 );
}

/* Inserts a range
 *
 * The values are merged using the value_merge_function.
 * If the source value is NULL the merge function is not called.
 *
 * After a merge and on error the values are freed using
 * the value_free_function
 *
 * Returns 1 if successful, 0 if the range was merged or -1 on error
 */
int libcdata_range_list_insert_range(
     libcdata_range_list_t *range_list,
     uint64_t range_start,
     uint64_t range_size,
     intptr_t *value,
     int (*value_free_function)(
            intptr_t **value,
            libcerror_error_t **error ),
     int (*value_merge_function)(
            intptr_t *destination_value,
            intptr_t *source_value,
            libcerror_error_t **error ),
     libcerror_error_t **error )
{
	libcdata_internal_range_list_t *internal_range_list = NULL;
	libcdata_list_element_t *list_element               = NULL;
	libcdata_list_element_t *new_element                = NULL;
	libcdata_range_list_t *backup_range_list            = NULL;
	static char *function                               = "libcdata_range_list_insert_range";
	uint64_t range_end                                  = 0;
	int element_index                                   = 0;
	int result                                          = 1;

	if( range_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid range list.",
		 function );

		return( -1 );
	}
	internal_range_list = (libcdata_internal_range_list_t *) range_list;

	if( range_start > (uint64_t) INT64_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid range start value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( range_size > (uint64_t) INT64_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid range size value exceeds maximum.",
		 function );

		return( -1 );
	}
	range_end = range_start + range_size;

	if( range_end < range_start )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid range end value out of bounds.",
		 function );

		return( -1 );
	}
	if( libcdata_range_list_initialize(
	     &backup_range_list,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create backup range list.",
		 function );

		return( -1 );
	}
#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_range_list->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		libcdata_range_list_free(
		 &backup_range_list,
		 NULL,
		 NULL );

		return( -1 );
	}
#endif
	if( libcdata_internal_range_list_insert_range_find_element(
	     internal_range_list,
	     range_start,
	     range_end,
	     &element_index,
	     &list_element,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to find element in list.",
		 function );

		result = -1;
	}
	if( result == 1 )
	{
		result = libcdata_internal_range_list_check_range_overlap(
		          internal_range_list,
		          list_element,
		          range_start,
		          range_end,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine if range overlaps with range list element: %d.",
			 function,
			 element_index );

			result = -1;
		}
	}
	if( result == 0 )
	{
		result = 1;

		if( libcdata_internal_range_list_insert_range_before_element(
		     internal_range_list,
		     list_element,
		     range_start,
		     range_end,
		     value,
		     &new_element,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to insert range before range list element.",
			 function );

			result = -1;
		}
	}
	else if( result != -1 )
	{
		result = 0;

		if( libcdata_internal_range_list_insert_range_merge(
		     internal_range_list,
		     list_element,
		     range_start,
		     range_end,
		     value,
		     value_merge_function,
		     backup_range_list,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to merge range with list element: %d.",
			 function,
			 element_index );

			result = -1;
		}
	}
#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_range_list->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		goto on_error;
	}
#endif
	libcdata_range_list_free(
	 &backup_range_list,
	 value_free_function,
	 NULL );

	return( result );

#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
on_error:
	if( result == 0 )
	{
		libcdata_internal_range_list_insert_range_revert_merge(
		 internal_range_list,
		 list_element,
		 backup_range_list,
		 NULL );
	}
	else if( result == 1 )
	{
		libcdata_internal_range_list_remove_range_value(
		 internal_range_list,
		 &new_element,
		 NULL,
		 NULL );
	}
	libcdata_range_list_free(
	 &backup_range_list,
	 NULL,
	 NULL );

	return( -1 );
#endif
}

/* Inserts a range list
 *
 * The values are merged using the value_merge_function.
 * If the source value is NULL the merge function is not called.
 *
 * After a merge and on error the values are freed using
 * the value_free_function
 *
 * The values in the source_range_list are not affected.
 *
 * Returns 1 if successful, or -1 on error
 */
int libcdata_range_list_insert_range_list(
     libcdata_range_list_t *range_list,
     libcdata_range_list_t *source_range_list,
     int (*value_free_function)(
            intptr_t **value,
            libcerror_error_t **error ),
     int (*value_merge_function)(
            intptr_t *destination_value,
            intptr_t *source_value,
            libcerror_error_t **error ),
     libcerror_error_t **error )
{
	libcdata_list_element_t *source_list_element         = NULL;
	libcdata_range_list_value_t *source_range_list_value = NULL;
	static char *function                                = "libcdata_range_list_insert_range_list";
	int element_index                                    = 0;
	int number_of_elements                               = 0;
	int result                                           = 0;

	if( range_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid range list.",
		 function );

		return( -1 );
	}
	if( source_range_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid source range list.",
		 function );

		return( -1 );
	}
	if( libcdata_range_list_get_number_of_elements(
	     source_range_list,
	     &number_of_elements,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of source list elements.",
		 function );

		return( -1 );
	}
	if( libcdata_range_list_get_first_element(
	     source_range_list,
	     &source_list_element,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve first element from source list.",
		 function );

		return( -1 );
	}
	for( element_index = 0;
	     element_index < number_of_elements;
	     element_index++ )
	{
		if( libcdata_list_element_get_value(
		     source_list_element,
		     (intptr_t **) &source_range_list_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value from source list element: %d.",
			 function,
			 element_index );

			return( -1 );
		}
		result = libcdata_range_list_insert_range(
		          range_list,
		          source_range_list_value->start,
		          source_range_list_value->size,
		          source_range_list_value->value,
		          value_free_function,
		          value_merge_function,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to insert source range list value: %d to range list.",
			 function,
			 element_index );

			return( -1 );
		}
		if( libcdata_list_element_get_next_element(
		     source_list_element,
		     &source_list_element,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve next element from source list element: %d.",
			 function,
			 element_index );

			return( -1 );
		}
	}
	return( 1 );
}

/* Shrinks a range
 *
 * A range that either overlaps the start or the end of the range list element
 * is removed by shrining the range of the range list element.
 *
 * Returns 1 if successful, or -1 on error
 */
int libcdata_internal_range_list_remove_shrink_range(
     libcdata_internal_range_list_t *internal_range_list,
     libcdata_list_element_t *range_list_element,
     libcdata_range_list_value_t *range_list_value,
     uint64_t range_start,
     uint64_t range_end,
     int (*value_free_function)(
            intptr_t **value,
            libcerror_error_t **error ),
     int (*value_split_function)(
            intptr_t **destination_value,
            intptr_t *source_value,
            uint64_t split_range_offset,
            libcerror_error_t **error ),
     libcerror_error_t **error )
{
	intptr_t *split_value = NULL;
	static char *function = "libcdata_internal_range_list_remove_shrink_range";
	uint64_t split_offset = 0;

	if( internal_range_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid range list.",
		 function );

		return( -1 );
	}
	if( range_list_element == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid range list element.",
		 function );

		return( -1 );
	}
	if( range_list_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid range list value.",
		 function );

		return( -1 );
	}
	if( ( range_start > range_list_value->start )
	 && ( range_end < range_list_value->end ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid range value out of bounds.",
		 function );

		return( -1 );
	}
	if( range_list_value->value != NULL )
	{
		if( value_free_function == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid value free function.",
			 function );

			return( -1 );
		}
		if( value_split_function == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid value split function.",
			 function );

			return( -1 );
		}
	}
	if( range_start <= range_list_value->start )
	{
		split_offset = range_end;
	}
	else
	{
		split_offset = range_start;
	}
	if( range_list_value->value != NULL )
	{
		if( value_split_function(
		     &split_value,
		     range_list_value->value,
		     split_offset,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to split value range offset: %" PRIu64 ".",
			 function,
			 split_offset );

			goto on_error;
		}
		if( value_free_function(
		     &( range_list_value->value ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to free value.",
			 function );

			range_list_value->value = split_value;

			goto on_error;
		}
		range_list_value->value = split_value;
	}
	if( split_offset > range_list_value->start )
	{
		range_list_value->start = split_offset;
		range_list_value->size  = range_list_value->end - split_offset;
	}
	else
	{
		range_list_value->end  = split_offset;
		range_list_value->size = split_offset - range_list_value->start;
	}
	return( 1 );

on_error:
	return( -1 );
}

/* Splits a range
 *
 * A range that either overlaps a part of the range list element
 * is removed by splitting the range of the range list element.
 *
 * Returns 1 if successful, or -1 on error
 */
int libcdata_internal_range_list_remove_split_range(
     libcdata_internal_range_list_t *internal_range_list,
     libcdata_list_element_t *range_list_element,
     libcdata_range_list_value_t *range_list_value,
     uint64_t range_start,
     uint64_t range_end,
     int (*value_free_function)(
            intptr_t **value,
            libcerror_error_t **error ),
     int (*value_split_function)(
            intptr_t **destination_value,
            intptr_t *source_value,
            uint64_t split_range_offset,
            libcerror_error_t **error ),
     libcerror_error_t **error )
{
	libcdata_range_list_value_t *split_range_list_value = NULL;
	static char *function                               = "libcdata_internal_range_list_remove_split_range";
	uint64_t split_offset                               = 0;

	if( internal_range_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid range list.",
		 function );

		return( -1 );
	}
	if( range_list_element == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid range list element.",
		 function );

		return( -1 );
	}
	if( range_list_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid range list value.",
		 function );

		return( -1 );
	}
	if( ( range_start <= range_list_value->start )
	 || ( range_end >= range_list_value->end ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid range value out of bounds.",
		 function );

		return( -1 );
	}
	if( range_list_value->value != NULL )
	{
		if( value_free_function == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid value free function.",
			 function );

			return( -1 );
		}
		if( value_split_function == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid value split function.",
			 function );

			return( -1 );
		}
	}
	split_offset = range_end;

	if( libcdata_range_list_value_initialize(
	     &split_range_list_value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create range list value.",
		 function );

		goto on_error;
	}
	if( split_range_list_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing range list value.",
		 function );

		goto on_error;
	}
	if( range_list_value->value != NULL )
	{
		if( value_split_function(
		     &( split_range_list_value->value ),
		     range_list_value->value,
		     split_offset,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to split value at range offset: %" PRIu64 ".",
			 function,
			 split_offset );

			goto on_error;
		}
	}
	split_range_list_value->start = split_offset;
	split_range_list_value->size  = range_list_value->end - split_offset;
	split_range_list_value->end   = range_list_value->end;

	range_list_value->size = split_offset - range_list_value->start;
	range_list_value->end  = split_offset;

	if( libcdata_internal_range_list_insert_value_after_element(
	     internal_range_list,
	     range_list_element,
	     split_range_list_value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to insert range list value after range list element.",
		 function );

		goto on_error;
	}
	split_range_list_value = NULL;

	if( libcdata_internal_range_list_remove_shrink_range(
	     internal_range_list,
	     range_list_element,
	     range_list_value,
	     range_start,
	     range_end,
	     value_free_function,
	     value_split_function,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to shrink range.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( split_range_list_value != NULL )
	{
		libcdata_range_list_value_free(
		 &split_range_list_value,
		 value_free_function,
		 NULL );
	}
	return( -1 );
}

/* Removes a range
 *
 * The values are split using the value_merge_function.
 * If the source value is NULL the split function is not called.
 * On return destination_value of the value_merge_function
 * should contain data greater equal to the split_range_offset.
 *
 * After a split and on error the values are freed using
 * the value_free_function
 *
 * Returns 1 if successful, or -1 on error
 */
int libcdata_range_list_remove_range(
     libcdata_range_list_t *range_list,
     uint64_t range_start,
     uint64_t range_size,
     int (*value_free_function)(
            intptr_t **value,
            libcerror_error_t **error ),
     int (*value_split_function)(
            intptr_t **destination_value,
            intptr_t *source_value,
            uint64_t split_range_offset,
            libcerror_error_t **error ),
     libcerror_error_t **error )
{
	libcdata_internal_range_list_t *internal_range_list = NULL;
	libcdata_list_element_t *list_element               = NULL;
	libcdata_range_list_value_t *range_list_value       = NULL;
	static char *function                               = "libcdata_range_list_remove_range";
	uint64_t next_range_start                           = 0;
	uint64_t range_end                                  = 0;
	int result                                          = 1;

	if( range_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid range list.",
		 function );

		return( -1 );
	}
	internal_range_list = (libcdata_internal_range_list_t *) range_list;

	if( range_start > (uint64_t) INT64_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid range start value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( range_size > (uint64_t) INT64_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid range size value exceeds maximum.",
		 function );

		return( -1 );
	}
	range_end = range_start + range_size;

	if( range_end < range_start )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid range end value out of bounds.",
		 function );

		return( -1 );
	}
#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_range_list->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	while( range_start < range_end )
	{
		result = libcdata_internal_range_list_get_element_at_offset(
		          internal_range_list,
		          range_start,
		          &list_element,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve list element for range offset: %" PRIu64 ".",
			 function,
			 range_start );

			goto on_error;
		}
		else if( result == 0 )
		{
			/* The specified range is not defined in the range list
			 */
			if( list_element == NULL )
			{
				break;
			}
		}
		if( libcdata_list_element_get_value(
		     list_element,
		     (intptr_t **) &range_list_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value from list element for range offset: %" PRIu64 ".",
			 function,
			 range_start );

			goto on_error;
		}
		if( range_list_value == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing range list value element for range offset: %" PRIu64 ".",
			 function,
			 range_start );

			goto on_error;
		}
		if( result == 0 )
		{
			/* The specified range is not defined in the range list
			 */
			if( range_list_value->start >= range_end )
			{
				break;
			}
			range_start = range_list_value->start;
		}
		next_range_start = range_list_value->end;

		if( ( range_start <= range_list_value->start )
		 && ( range_end >= range_list_value->end ) )
		{
			if( libcdata_internal_range_list_remove_range_value(
			     internal_range_list,
			     &list_element,
			     value_free_function,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_REMOVE_FAILED,
				 "%s: unable to remove range value.",
				 function );

				goto on_error;
			}
		}
		else if( ( range_start > range_list_value->start )
		      && ( range_end < range_list_value->end ) )
		{
			if( libcdata_internal_range_list_remove_split_range(
			     internal_range_list,
			     list_element,
			     range_list_value,
			     range_start,
			     range_end,
			     value_free_function,
			     value_split_function,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to split range.",
				 function );

				goto on_error;
			}
		}
		else
		{
			if( libcdata_internal_range_list_remove_shrink_range(
			     internal_range_list,
			     list_element,
			     range_list_value,
			     range_start,
			     range_end,
			     value_free_function,
			     value_split_function,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to shrink range.",
				 function );

				goto on_error;
			}
		}
		range_start = next_range_start;
	}
#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_range_list->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );

on_error:
#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	libcthreads_read_write_lock_release_for_write(
	 internal_range_list->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

/* Retrieves a specific element from the range list
 * Returns 1 if successful or -1 on error
 */
int libcdata_internal_range_list_get_element_by_index(
     libcdata_internal_range_list_t *internal_range_list,
     int element_index,
     libcdata_list_element_t **element,
     libcerror_error_t **error )
{
	libcdata_list_element_t *current_element = NULL;
	static char *function                    = "libcdata_internal_range_list_get_element_by_index";
	int current_element_index                = 0;

	if( internal_range_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid range list.",
		 function );

		return( -1 );
	}
	if( ( element_index < 0 )
	 || ( element_index >= internal_range_list->number_of_elements ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid element index value out of bounds.",
		 function );

		return( -1 );
	}
	if( element == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid element.",
		 function );

		return( -1 );
	}
	current_element       = internal_range_list->current_element;
	current_element_index = internal_range_list->current_element_index;

	if( ( current_element != NULL )
	 && ( current_element_index != element_index ) )
	{
		if( element_index < current_element_index )
		{
			if( ( current_element_index - element_index ) < ( internal_range_list->number_of_elements / 2 ) )
			{
				while( current_element_index > element_index )
				{
					if( libcdata_list_element_get_previous_element(
					     current_element,
					     &current_element,
					     error ) != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
						 "%s: unable to retrieve previous element from list element: %d.",
						 function,
						 current_element_index );

						return( -1 );
					}
					current_element_index--;
				}
			}
		}
		else
		{
			if( ( element_index - current_element_index ) < ( internal_range_list->number_of_elements / 2 ) )
			{
				while( current_element_index < element_index )
				{
					if( libcdata_list_element_get_next_element(
					     current_element,
					     &current_element,
					     error ) != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
						 "%s: unable to retrieve next element from list element: %d.",
						 function,
						 current_element_index );

						return( -1 );
					}
					current_element_index++;
				}
			}
		}
	}
	if( ( current_element == NULL )
	 || ( current_element_index != element_index ) )
	{
		if( element_index < ( internal_range_list->number_of_elements / 2 ) )
		{
			current_element = internal_range_list->first_element;

			for( current_element_index = 0;
			     current_element_index < element_index;
			     current_element_index++ )
			{
				if( libcdata_list_element_get_next_element(
				     current_element,
				     &current_element,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve next element from list element: %d.",
					 function,
					 current_element_index );

					return( -1 );
				}
			}
		}
		else
		{
			current_element = internal_range_list->last_element;

			for( current_element_index = ( internal_range_list->number_of_elements - 1 );
			     current_element_index > element_index;
			     current_element_index-- )
			{
				if( libcdata_list_element_get_previous_element(
				     current_element,
				     &current_element,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve previous element from list element: %d.",
					 function,
					 current_element_index );

					return( -1 );
				}
			}
		}
	}
	if( current_element != NULL )
	{
		internal_range_list->current_element       = current_element;
		internal_range_list->current_element_index = current_element_index;
	}
	*element = current_element;

	return( 1 );
}

/* Retrieves the element that contains the range offset
 * If the function returns element can be set to contain the element
 * containing the next range list value.
 * Returns 1 if successful, 0 if not found or -1 on error
 */
int libcdata_internal_range_list_get_element_at_offset(
     libcdata_internal_range_list_t *internal_range_list,
     uint64_t range_offset,
     libcdata_list_element_t **element,
     libcerror_error_t **error )
{
	libcdata_list_element_t *range_list_element   = NULL;
	libcdata_range_list_value_t *range_list_value = NULL;
	static char *function                         = "libcdata_internal_range_list_get_element_at_offset";
	int element_index                             = 0;

	if( internal_range_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid range list.",
		 function );

		return( -1 );
	}
	if( range_offset > (uint64_t) INT64_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid range offset value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( element == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid element.",
		 function );

		return( -1 );
	}
/* TODO add optimization using current element */
	range_list_element = internal_range_list->first_element;

	for( element_index = 0;
	     element_index < internal_range_list->number_of_elements;
	     element_index++ )
	{
		if( libcdata_list_element_get_value(
		     range_list_element,
		     (intptr_t **) &range_list_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value from list element: %d.",
			 function,
			 element_index );

			return( -1 );
		}
		if( range_list_value == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing range list value element: %d.",
			 function,
			 element_index );

			return( -1 );
		}
		if( range_offset < range_list_value->start )
		{
			break;
		}
		if( ( range_offset >= range_list_value->start )
		 && ( range_offset < range_list_value->end ) )
		{
			*element = range_list_element;

			return( 1 );
		}
		if( libcdata_list_element_get_next_element(
		     range_list_element,
		     &range_list_element,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve next element from list element: %d.",
			 function,
			 element_index );

			return( -1 );
		}
	}
	*element = NULL;

	return( 0 );
}

/* Retrieves a specific value from the range list
 * Returns 1 if successful or -1 on error
 */
int libcdata_internal_range_list_get_value_by_index(
     libcdata_internal_range_list_t *internal_range_list,
     int element_index,
     libcdata_range_list_value_t **range_list_value,
     libcerror_error_t **error )
{
	libcdata_list_element_t *list_element = NULL;
	static char *function                 = "libcdata_internal_range_list_get_value_by_index";

	if( range_list_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid range list value.",
		 function );

		return( -1 );
	}
	if( libcdata_internal_range_list_get_element_by_index(
	     internal_range_list,
	     element_index,
	     &list_element,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve list element: %d.",
		 function,
		 element_index );

		return( -1 );
	}
	if( libcdata_list_element_get_value(
	     list_element,
	     (intptr_t **) range_list_value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value from list element: %d.",
		 function,
		 element_index );

		return( -1 );
	}
	if( range_list_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing range list value: %d .",
		 function,
		 element_index );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the value that contains the range offset
 * Returns 1 if successful, 0 if no value was found or -1 on error
 */
int libcdata_internal_range_list_get_value_at_offset(
     libcdata_internal_range_list_t *internal_range_list,
     uint64_t range_offset,
     libcdata_range_list_value_t **range_list_value,
     libcerror_error_t **error )
{
	libcdata_list_element_t *list_element = NULL;
	static char *function                 = "libcdata_internal_range_list_get_value_at_offset";
	int result                            = 0;

	if( range_list_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid range list value.",
		 function );

		return( -1 );
	}
	result = libcdata_internal_range_list_get_element_at_offset(
	          internal_range_list,
	          range_offset,
	          &list_element,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve list element for range offset: %" PRIu64 ".",
		 function,
		 range_offset );

		return( -1 );
	}
	else if( result != 0 )
	{
		if( libcdata_list_element_get_value(
		     list_element,
		     (intptr_t **) range_list_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value from list element for range offset: %" PRIu64 ".",
			 function,
			 range_offset );

			return( -1 );
		}
		if( *range_list_value == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing range list value for range offset: %" PRIu64 ".",
			 function,
			 range_offset );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves a specific range
 * Returns 1 if successful or -1 on error
 */
int libcdata_range_list_get_range_by_index(
     libcdata_range_list_t *range_list,
     int element_index,
     uint64_t *range_start,
     uint64_t *range_size,
     intptr_t **value,
     libcerror_error_t **error )
{
	libcdata_internal_range_list_t *internal_range_list = NULL;
	libcdata_range_list_value_t *range_list_value       = NULL;
	static char *function                               = "libcdata_range_list_get_range_by_index";

	if( range_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid range list.",
		 function );

		return( -1 );
	}
	internal_range_list = (libcdata_internal_range_list_t *) range_list;

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
	if( value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value.",
		 function );

		return( -1 );
	}
#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_range_list->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( libcdata_internal_range_list_get_value_by_index(
	     internal_range_list,
	     element_index,
	     &range_list_value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve range list value: %d.",
		 function,
		 element_index );

		goto on_error;
	}
	if( range_list_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing range list value: %d.",
		 function,
		 element_index );

		goto on_error;
	}
	*range_start = range_list_value->start;
	*range_size  = range_list_value->size;
	*value       = range_list_value->value;

#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_range_list->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );

on_error:
#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	libcthreads_read_write_lock_release_for_read(
	 internal_range_list->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

/* Retrieves a range for a specific range offset
 * Returns 1 if successful, 0 if no range was found or -1 on error
 */
int libcdata_range_list_get_range_at_offset(
     libcdata_range_list_t *range_list,
     uint64_t range_offset,
     uint64_t *range_start,
     uint64_t *range_size,
     intptr_t **value,
     libcerror_error_t **error )
{
	libcdata_internal_range_list_t *internal_range_list = NULL;
	libcdata_range_list_value_t *range_list_value       = NULL;
	static char *function                               = "libcdata_range_list_get_range_at_offset";
	int result                                          = 0;

	if( range_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid range list.",
		 function );

		return( -1 );
	}
	internal_range_list = (libcdata_internal_range_list_t *) range_list;

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
	if( value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value.",
		 function );

		return( -1 );
	}
#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_range_list->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	result = libcdata_internal_range_list_get_value_at_offset(
	          internal_range_list,
	          range_offset,
	          &range_list_value,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve range list value for range offset: %" PRIu64 ".",
		 function,
		 range_offset );

		goto on_error;
	}
	else if( result != 0 )
	{
		if( range_list_value == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing range list value for range offset: %" PRIu64 ".",
			 function,
			 range_offset );

			goto on_error;
		}
		*range_start = range_list_value->start;
		*range_size  = range_list_value->size;
		*value       = range_list_value->value;
	}
#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_range_list->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );

on_error:
#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	libcthreads_read_write_lock_release_for_read(
	 internal_range_list->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

/* Determines if a certain range is present in the list
 * This function does not check for partial overlapping ranges
 * Returns 1 if a range is present, 0 if not or -1 on error
 */
int libcdata_range_list_range_is_present(
     libcdata_range_list_t *range_list,
     uint64_t range_start,
     uint64_t range_size,
     libcerror_error_t **error )
{
	libcdata_internal_range_list_t *internal_range_list = NULL;
	libcdata_list_element_t *list_element               = NULL;
	libcdata_range_list_value_t *range_list_value       = NULL;
	static char *function                               = "libcdata_range_list_range_is_present";
	uint64_t range_end                                  = 0;
	int element_index                                   = 0;
	int result                                          = 0;

	if( range_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid range list.",
		 function );

		return( -1 );
	}
	internal_range_list = (libcdata_internal_range_list_t *) range_list;

	if( range_start > (uint64_t) INT64_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid range start value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( range_size > (uint64_t) INT64_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid size value exceeds maximum.",
		 function );

		return( -1 );
	}
	range_end = range_start + range_size;

	if( range_end < range_start )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid range end value out of bounds.",
		 function );

		return( -1 );
	}
#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_range_list->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
/* TODO add optimization using current element */
	list_element = internal_range_list->first_element;

	for( element_index = 0;
	     element_index < internal_range_list->number_of_elements;
	     element_index++ )
	{
		if( libcdata_list_element_get_value(
		     list_element,
		     (intptr_t **) &range_list_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value from list element: %d.",
			 function,
			 element_index );

			goto on_error;
		}
		if( range_end < range_list_value->start )
		{
			break;
		}
		if( ( range_start >= range_list_value->start )
		 && ( range_end <= range_list_value->end ) )
		{
			result = 1;

			break;
		}
		if( libcdata_list_element_get_next_element(
		     list_element,
		     &list_element,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve next element from list element: %d.",
			 function,
			 element_index );

			goto on_error;
		}
	}
#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_range_list->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );

on_error:
#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	libcthreads_read_write_lock_release_for_read(
	 internal_range_list->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

/* Determines if a certain range is present in the list
 * This function does not check for partial overlapping ranges
 * Returns 1 if a range is present, 0 if not or -1 on error
 */
int libcdata_range_list_range_has_overlapping_range(
     libcdata_range_list_t *range_list,
     uint64_t range_start,
     uint64_t range_size,
     libcerror_error_t **error )
{
	libcdata_internal_range_list_t *internal_range_list = NULL;
	libcdata_list_element_t *list_element               = NULL;
	libcdata_range_list_value_t *range_list_value       = NULL;
	static char *function                               = "libcdata_range_list_range_has_overlapping_range";
	uint64_t range_end                                  = 0;
	int element_index                                   = 0;
	int result                                          = 0;

	if( range_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid range list.",
		 function );

		return( -1 );
	}
	internal_range_list = (libcdata_internal_range_list_t *) range_list;

	if( range_start > (uint64_t) INT64_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid range start value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( range_size > (uint64_t) INT64_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid size value exceeds maximum.",
		 function );

		return( -1 );
	}
	range_end = range_start + range_size;

	if( range_end < range_start )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid range end value out of bounds.",
		 function );

		return( -1 );
	}
#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_range_list->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
/* TODO add optimization using current element */
	list_element = internal_range_list->first_element;

	for( element_index = 0;
	     element_index < internal_range_list->number_of_elements;
	     element_index++ )
	{
		if( libcdata_list_element_get_value(
		     list_element,
		     (intptr_t **) &range_list_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value from list element: %d.",
			 function,
			 element_index );

			goto on_error;
		}
		if( range_end < range_list_value->start )
		{
			break;
		}
		if( ( range_start >= range_list_value->start )
		 && ( range_start < range_list_value->end ) )
		{
			result = 1;

			break;
		}
		if( ( range_end > range_list_value->start )
		 && ( range_end <= range_list_value->end ) )
		{
			result = 1;

			break;
		}
		if( libcdata_list_element_get_next_element(
		     list_element,
		     &list_element,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve next element from list element: %d.",
			 function,
			 element_index );

			goto on_error;
		}
	}
#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_range_list->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );

on_error:
#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	libcthreads_read_write_lock_release_for_read(
	 internal_range_list->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

/* Retrieves the range spanning the ranges in the range list
 * Returns 1 if present, 0 if not present or -1 on error
 */
int libcdata_range_list_get_spanning_range(
     libcdata_range_list_t *range_list,
     uint64_t *range_start,
     uint64_t *range_size,
     libcerror_error_t **error )
{
	libcdata_internal_range_list_t *internal_range_list = NULL;
	libcdata_range_list_value_t *range_list_value       = NULL;
	static char *function                               = "libcdata_range_list_get_spanning_range";
	int element_index                                   = 0;
	int result                                          = 0;

	if( range_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid range list.",
		 function );

		return( -1 );
	}
	internal_range_list = (libcdata_internal_range_list_t *) range_list;

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
#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_range_list->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( internal_range_list->number_of_elements > 0 )
	{
		if( libcdata_internal_range_list_get_value_by_index(
		     internal_range_list,
		     0,
		     &range_list_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve range list value: 0.",
			 function );

			goto on_error;
		}
		if( range_list_value == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing range list value: 0.",
			 function );

			goto on_error;
		}
		*range_start = range_list_value->start;

		if( internal_range_list->number_of_elements > 1 )
		{
			element_index = internal_range_list->number_of_elements - 1;

			if( libcdata_internal_range_list_get_value_by_index(
			     internal_range_list,
			     element_index,
			     &range_list_value,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve range list value: %d.",
				 function,
				 element_index );

				goto on_error;
			}
			if( range_list_value == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: missing range list value: %d.",
				 function,
				 element_index );

				goto on_error;
			}
		}
		*range_size = range_list_value->end - *range_start;

		result = 1;
	}
#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_range_list->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );

on_error:
#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	libcthreads_read_write_lock_release_for_read(
	 internal_range_list->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

