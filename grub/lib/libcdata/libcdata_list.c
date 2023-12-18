/*
 * List functions
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
#include "libcdata_list.h"
#include "libcdata_list_element.h"
#include "libcdata_types.h"

/* Creates a list
 * Make sure the value list is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libcdata_list_initialize(
     libcdata_list_t **list,
     libcerror_error_t **error )
{
	libcdata_internal_list_t *internal_list = NULL;
	static char *function                   = "libcdata_list_initialize";

	if( list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid list.",
		 function );

		return( -1 );
	}
	if( *list != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid list value already set.",
		 function );

		return( -1 );
	}
	internal_list = memory_allocate_structure(
	                 libcdata_internal_list_t );

	if( internal_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create list.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     internal_list,
	     0,
	     sizeof( libcdata_internal_list_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear list.",
		 function );

		memory_free(
		 internal_list );

		return( -1 );
	}
#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	if( libcthreads_read_write_lock_initialize(
	     &( internal_list->read_write_lock ),
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
	*list = (libcdata_list_t *) internal_list;

	return( 1 );

on_error:
	if( internal_list != NULL )
	{
		memory_free(
		 internal_list );
	}
	return( -1 );
}

/* Frees a list including the elements
 * Uses the value_free_function to free the element value
 * Returns 1 if successful or -1 on error
 */
int libcdata_list_free(
     libcdata_list_t **list,
     int (*value_free_function)(
            intptr_t **value,
            libcerror_error_t **error ),
     libcerror_error_t **error )
{
	libcdata_internal_list_t *internal_list = NULL;
	static char *function                   = "libcdata_list_free";
	int result                              = 1;

	if( list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid list.",
		 function );

		return( -1 );
	}
	if( *list != NULL )
	{
		internal_list = (libcdata_internal_list_t *) *list;
		*list         = NULL;

		if( libcdata_list_empty(
		     (libcdata_list_t*) internal_list,
		     value_free_function,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to empty list.",
			 function );

			result = -1;
		}
#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
		if( libcthreads_read_write_lock_free(
		     &( internal_list->read_write_lock ),
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
		 internal_list );
	}
	return( result );
}

/* Empties a list and frees the elements
 * Uses the value_free_function to free the element value
 * Returns 1 if successful or -1 on error
 */
int libcdata_list_empty(
     libcdata_list_t *list,
     int (*value_free_function)(
            intptr_t **value,
            libcerror_error_t **error ),
     libcerror_error_t **error )
{
	libcdata_internal_list_t *internal_list = NULL;
	libcdata_list_element_t *list_element   = NULL;
	libcdata_list_element_t *next_element   = NULL;
	static char *function                   = "libcdata_list_empty";
	int element_index                       = 0;
	int number_of_elements                  = 0;
	int result                              = 1;

	if( list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid list.",
		 function );

		return( -1 );
	}
	internal_list = (libcdata_internal_list_t *) list;

#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_list->read_write_lock,
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
	if( internal_list->number_of_elements > 0 )
	{
		number_of_elements = internal_list->number_of_elements;

		list_element = internal_list->first_element;

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
			internal_list->first_element = next_element;

			if( internal_list->last_element == list_element )
			{
				internal_list->last_element = next_element;
			}
			internal_list->number_of_elements -= 1;

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
			if( libcdata_list_element_free(
			     &list_element,
			     value_free_function,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free list element: %d.",
				 function,
				 element_index );

				result = -1;
			}
			list_element = next_element;
		}
	}
#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_list->read_write_lock,
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

/* Clones the list and its elements
 *
 * The values are cloned using the value_clone_function
 * On error the values are freed using the value_free_function
 *
 * Returns 1 if successful or -1 on error
 */
int libcdata_list_clone(
     libcdata_list_t **destination_list,
     libcdata_list_t *source_list,
     int (*value_free_function)(
            intptr_t **value,
            libcerror_error_t **error ),
     int (*value_clone_function)(
            intptr_t **destination_value,
            intptr_t *source_value,
            libcerror_error_t **error ),
     libcerror_error_t **error )
{
	libcdata_internal_list_t *internal_destination_list            = NULL;
	libcdata_internal_list_t *internal_source_list                 = NULL;
	libcdata_internal_list_element_t *internal_source_list_element = NULL;
	intptr_t *destination_value                                    = NULL;
	static char *function                                          = "libcdata_list_clone";
	int element_index                                              = 0;

	if( destination_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid destination list.",
		 function );

		return( -1 );
	}
	if( *destination_list != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid destination list already set.",
		 function );

		return( -1 );
	}
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
	if( value_clone_function == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value clone function.",
		 function );

		return( -1 );
	}
	if( source_list == NULL )
	{
		*destination_list = NULL;

		return( 1 );
	}
	internal_source_list = (libcdata_internal_list_t *) source_list;

#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_source_list->read_write_lock,
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
	if( libcdata_list_initialize(
	     (libcdata_list_t **) &internal_destination_list,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create destination list.",
		 function );

		goto on_error;
	}
	if( internal_destination_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing destination list.",
		 function );

		goto on_error;
	}
	if( internal_source_list->first_element != NULL )
	{
		internal_source_list_element = (libcdata_internal_list_element_t *) internal_source_list->first_element;

		for( element_index = 0;
		     element_index < internal_source_list->number_of_elements;
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
			if( value_clone_function(
			     &destination_value,
			     internal_source_list_element->value,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create destination value: %d.",
				 function,
				 element_index );

				goto on_error;
			}
			if( libcdata_list_append_value(
			     (libcdata_list_t *) internal_destination_list,
			     destination_value,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
				 "%s: unable to append value: %d to destination list.",
				 function,
				 element_index );

				goto on_error;
			}
			destination_value = NULL;

			internal_source_list_element = (libcdata_internal_list_element_t *) internal_source_list_element->next_element;
		}
	}
#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_source_list->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		libcdata_list_free(
		 (libcdata_list_t **) &internal_destination_list,
		 value_free_function,
		 error );

		return( -1 );
	}
#endif
	*destination_list = (libcdata_list_t *) internal_destination_list;

	return( 1 );

on_error:
#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	libcthreads_read_write_lock_release_for_read(
	 internal_source_list->read_write_lock,
	 NULL );
#endif
	if( destination_value != NULL )
	{
		value_free_function(
		 &destination_value,
		 NULL );
	}
	if( internal_destination_list != NULL )
	{
		libcdata_list_free(
		 (libcdata_list_t **) &internal_destination_list,
		 value_free_function,
		 error );
	}
	return( -1 );
}

/* Retrieves the number of elements in the list
 * Returns 1 if successful or -1 on error
 */
int libcdata_list_get_number_of_elements(
     libcdata_list_t *list,
     int *number_of_elements,
     libcerror_error_t **error )
{
	libcdata_internal_list_t *internal_list = NULL;
	static char *function                   = "libcdata_list_get_number_of_elements";

	if( list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid list.",
		 function );

		return( -1 );
	}
	internal_list = (libcdata_internal_list_t *) list;

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
	     internal_list->read_write_lock,
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
	*number_of_elements = internal_list->number_of_elements;

#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_list->read_write_lock,
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

/* Retrieves the first elements in the list
 * Returns 1 if successful or -1 on error
 */
int libcdata_list_get_first_element(
     libcdata_list_t *list,
     libcdata_list_element_t **element,
     libcerror_error_t **error )
{
	libcdata_internal_list_t *internal_list = NULL;
	static char *function                   = "libcdata_list_get_first_element";

	if( list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid list.",
		 function );

		return( -1 );
	}
	internal_list = (libcdata_internal_list_t *) list;

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
	     internal_list->read_write_lock,
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
	*element = internal_list->first_element;

#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_list->read_write_lock,
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

/* Sets the first element in the list
 * Returns 1 if successful or -1 on error
 */
int libcdata_internal_list_set_first_element(
     libcdata_internal_list_t *internal_list,
     libcdata_list_element_t *element,
     libcerror_error_t **error )
{
	libcdata_list_element_t *backup_first_element = NULL;
	static char *function                         = "libcdata_internal_list_set_first_element";

	if( internal_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid list.",
		 function );

		return( -1 );
	}
	backup_first_element = internal_list->first_element;

	if( element != NULL )
	{
		if( libcdata_list_element_set_next_element(
		     element,
		     internal_list->first_element,
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
	if( internal_list->first_element != NULL )
	{
		if( libcdata_list_element_set_previous_element(
		     internal_list->first_element,
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
	internal_list->first_element = element;

	return( 1 );

on_error:
	if( element != NULL )
	{
		libcdata_list_element_set_next_element(
		 element,
		 NULL,
		 NULL );
	}
	if( backup_first_element != NULL )
	{
		libcdata_list_element_set_next_element(
		 backup_first_element,
		 NULL,
		 NULL );
	}
	internal_list->first_element = backup_first_element;

	return( -1 );
}

/* Retrieves the last elements in the list
 * Returns 1 if successful or -1 on error
 */
int libcdata_list_get_last_element(
     libcdata_list_t *list,
     libcdata_list_element_t **element,
     libcerror_error_t **error )
{
	libcdata_internal_list_t *internal_list = NULL;
	static char *function                   = "libcdata_list_get_last_element";

	if( list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid list.",
		 function );

		return( -1 );
	}
	internal_list = (libcdata_internal_list_t *) list;

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
	     internal_list->read_write_lock,
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
	*element = internal_list->last_element;

#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_list->read_write_lock,
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
int libcdata_internal_list_set_last_element(
     libcdata_internal_list_t *internal_list,
     libcdata_list_element_t *element,
     libcerror_error_t **error )
{
	libcdata_list_element_t *backup_last_element = NULL;
	static char *function                        = "libcdata_internal_list_set_last_element";

	if( internal_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid list.",
		 function );

		return( -1 );
	}
	backup_last_element = internal_list->last_element;

	if( element != NULL )
	{
		if( libcdata_list_element_set_previous_element(
		     element,
		     internal_list->last_element,
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
	if( internal_list->last_element != NULL )
	{
		if( libcdata_list_element_set_next_element(
		     internal_list->last_element,
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
	internal_list->last_element = element;

	return( 1 );

on_error:
	if( element != NULL )
	{
		libcdata_list_element_set_previous_element(
		 element,
		 NULL,
		 NULL );
	}
	if( backup_last_element != NULL )
	{
		libcdata_list_element_set_next_element(
		 backup_last_element,
		 NULL,
		 NULL );
	}
	internal_list->last_element = backup_last_element;

	return( -1 );
}

/* Retrieves a specific element from the list
 * Returns 1 if successful or -1 on error
 */
int libcdata_list_get_element_by_index(
     libcdata_list_t *list,
     int element_index,
     libcdata_list_element_t **element,
     libcerror_error_t **error )
{
	libcdata_internal_list_t *internal_list = NULL;
	libcdata_list_element_t *list_element   = NULL;
	static char *function                   = "libcdata_list_get_element_by_index";
	int element_iterator                    = 0;

	if( list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid list.",
		 function );

		return( -1 );
	}
	internal_list = (libcdata_internal_list_t *) list;

	if( ( element_index < 0 )
	 || ( element_index >= internal_list->number_of_elements ) )
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
		 "%s: invalid list element.",
		 function );

		return( -1 );
	}
#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_list->read_write_lock,
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
	if( element_index < ( internal_list->number_of_elements / 2 ) )
	{
		list_element = internal_list->first_element;

		for( element_iterator = 0;
		     element_iterator < element_index;
		     element_iterator++ )
		{
			if( list_element == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: corruption detected in element: %d.",
				 function,
				 element_iterator );

				goto on_error;
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
	}
	else
	{
		list_element = internal_list->last_element;

		for( element_iterator = ( internal_list->number_of_elements - 1 );
		     element_iterator > element_index;
		     element_iterator-- )
		{
			if( list_element == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: corruption detected in element: %d.",
				 function,
				 element_iterator );

				goto on_error;
			}
			if( libcdata_list_element_get_previous_element(
			     list_element,
			     &list_element,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve previous element from list element: %d.",
				 function,
				 element_index );

				goto on_error;
			}
		}
	}
	if( list_element == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: corruption detected - missing list element.",
		 function );

		goto on_error;
	}
	*element = list_element;

#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_list->read_write_lock,
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
	 internal_list->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

/* Retrieves a specific value from the list
 * Returns 1 if successful or -1 on error
 */
int libcdata_list_get_value_by_index(
     libcdata_list_t *list,
     int element_index,
     intptr_t **value,
     libcerror_error_t **error )
{
	libcdata_list_element_t *list_element = NULL;
	static char *function                 = "libcdata_list_get_value_by_index";

	if( libcdata_list_get_element_by_index(
	     list,
	     element_index,
	     &list_element,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve list element: %d from list.",
		 function,
		 element_index );

		return( -1 );
	}
	if( libcdata_list_element_get_value(
	     list_element,
	     value,
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
	return( 1 );
}

/* Prepends a list element to the list
 * Returns 1 if successful or -1 on error
 */
int libcdata_list_prepend_element(
     libcdata_list_t *list,
     libcdata_list_element_t *element,
     libcerror_error_t **error )
{
	libcdata_internal_list_t *internal_list       = NULL;
	libcdata_list_element_t *next_element         = NULL;
	libcdata_list_element_t *previous_element     = NULL;
	static char *function                         = "libcdata_list_prepend_element";
	int result                                    = 1;

#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	libcdata_list_element_t *backup_first_element = NULL;
	libcdata_list_element_t *backup_last_element  = NULL;
#endif

	if( list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid list.",
		 function );

		return( -1 );
	}
	internal_list = (libcdata_internal_list_t *) list;

	if( internal_list->number_of_elements == 0 )
	{
		if( internal_list->first_element != NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: corruption detected - first element already set.",
			 function );

			return( -1 );
		}
		if( internal_list->last_element != NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: corruption detected - last element already set.",
			 function );

			return( -1 );
		}
	}
	else
	{
		if( internal_list->first_element == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: corruption detected - missing first element.",
			 function );

			return( -1 );
		}
		if( internal_list->last_element == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: corruption detected - missing last element.",
			 function );

			return( -1 );
		}
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
#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_list->read_write_lock,
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
	backup_first_element = internal_list->first_element;
	backup_last_element  = internal_list->last_element;

#endif
	result = libcdata_list_element_set_next_element(
	          element,
	          internal_list->first_element,
	          error );

	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set next element of list element.",
		 function );

		result = -1;
	}
	if( result == 1 )
	{
		if( internal_list->first_element != NULL )
		{
			result = libcdata_list_element_set_previous_element(
			          internal_list->first_element,
			          element,
			          error );

			if( result != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set previous element of first element.",
				 function );

				libcdata_list_element_set_next_element(
				 element,
				 NULL,
				 NULL );

				result = -1;
			}
		}
	}
	if( result == 1 )
	{
		internal_list->first_element = element;

		if( internal_list->last_element == NULL )
		{
			internal_list->last_element = element;
		}
		internal_list->number_of_elements += 1;
	}
#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_list->read_write_lock,
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
	return( result );

#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
on_error:
	if( result == 1 )
	{
		libcdata_list_element_set_next_element(
		 element,
		 NULL,
		 NULL );

		libcdata_list_element_set_previous_element(
		 backup_first_element,
		 NULL,
		 NULL );

		internal_list->first_element = backup_first_element;
		internal_list->last_element  = backup_last_element;

		internal_list->number_of_elements -= 1;
	}
	return( -1 );
#endif
}

/* Prepends a value to the list
 * Creates a new list element
 * Returns 1 if successful or -1 on error
 */
int libcdata_list_prepend_value(
     libcdata_list_t *list,
     intptr_t *value,
     libcerror_error_t **error )
{
	libcdata_list_element_t *list_element = NULL;
	static char *function                 = "libcdata_list_prepend_value";

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
	if( libcdata_list_prepend_element(
	     list,
	     list_element,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to prepend element to list.",
		 function );

		goto on_error;
	}
	if( libcdata_list_element_set_value(
	     list_element,
	     value,
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

/* Appends a list element to the list
 * Returns 1 if successful or -1 on error
 */
int libcdata_list_append_element(
     libcdata_list_t *list,
     libcdata_list_element_t *element,
     libcerror_error_t **error )
{
	libcdata_internal_list_t *internal_list       = NULL;
	libcdata_list_element_t *next_element         = NULL;
	libcdata_list_element_t *previous_element     = NULL;
	static char *function                         = "libcdata_list_append_element";
	int result                                    = 1;

#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	libcdata_list_element_t *backup_first_element = NULL;
	libcdata_list_element_t *backup_last_element  = NULL;
#endif

	if( list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid list.",
		 function );

		return( -1 );
	}
	internal_list = (libcdata_internal_list_t *) list;

	if( internal_list->number_of_elements == 0 )
	{
		if( internal_list->first_element != NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: corruption detected - first element already set.",
			 function );

			return( -1 );
		}
		if( internal_list->last_element != NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: corruption detected - last element already set.",
			 function );

			return( -1 );
		}
	}
	else
	{
		if( internal_list->first_element == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: corruption detected - missing first element.",
			 function );

			return( -1 );
		}
		if( internal_list->last_element == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: corruption detected - missing last element.",
			 function );

			return( -1 );
		}
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
#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_list->read_write_lock,
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
	backup_first_element = internal_list->first_element;
	backup_last_element  = internal_list->last_element;

#endif
	result = libcdata_list_element_set_previous_element(
	          element,
	          internal_list->last_element,
	          error );

	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set previous element of list element.",
		 function );

		result = -1;
	}
	if( result == 1 )
	{
		if( internal_list->last_element != NULL )
		{
			result = libcdata_list_element_set_next_element(
			          internal_list->last_element,
			          element,
			          error );

			if( result != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set next element of last element.",
				 function );

				libcdata_list_element_set_previous_element(
				 element,
				 NULL,
				 NULL );

				result = -1;
			}
		}
	}
	if( result == 1 )
	{
		if( internal_list->first_element == NULL )
		{
			internal_list->first_element = element;
		}
		internal_list->last_element = element;

		internal_list->number_of_elements += 1;
	}
#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_list->read_write_lock,
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
	return( result );

#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
on_error:
	if( result == 1 )
	{
		libcdata_list_element_set_previous_element(
		 element,
		 NULL,
		 NULL );

		libcdata_list_element_set_next_element(
		 backup_last_element,
		 NULL,
		 NULL );

		internal_list->first_element = backup_first_element;
		internal_list->last_element  = backup_last_element;

		internal_list->number_of_elements -= 1;
	}
	return( -1 );
#endif
}

/* Appends a value to the list
 * Creates a new list element
 * Returns 1 if successful or -1 on error
 */
int libcdata_list_append_value(
     libcdata_list_t *list,
     intptr_t *value,
     libcerror_error_t **error )
{
	libcdata_list_element_t *element = NULL;
	static char *function            = "libcdata_list_append_value";

	if( libcdata_list_element_initialize(
	     &element,
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
	     element,
	     value,
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
	if( libcdata_list_append_element(
	     list,
	     element,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append element to list.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( element != NULL )
	{
		libcdata_list_element_free(
		 &element,
		 NULL,
		 NULL );
	}
	return( -1 );
}

/* Retrieves the element which the element should be inserted before
 *
 * Uses the value_compare_function to determine the order of the entries
 * The value_compare_function should return LIBCDATA_COMPARE_LESS,
 * LIBCDATA_COMPARE_EQUAL, LIBCDATA_COMPARE_GREATER if successful or -1 on error
 *
 * Duplicate entries are allowed by default and inserted after the last duplicate value.
 * Only allowing unique entries can be enforced by setting the flag LIBCDATA_INSERT_FLAG_UNIQUE_ENTRIES
 *
 * On return element will be set to NULL if the element should be inserted at the end of the list.
 *
 * Returns 1 if successful, 0 if a list element containing the value already exists or -1 on error
 */
int libcdata_internal_list_insert_element_find_element(
     libcdata_internal_list_t *internal_list,
     intptr_t *value_to_insert,
     int (*value_compare_function)(
            intptr_t *first_value,
            intptr_t *second_value,
            libcerror_error_t **error ),
     uint8_t insert_flags,
     int *element_index,
     libcdata_list_element_t **element,
     libcerror_error_t **error )
{
	libcdata_list_element_t *list_element = NULL;
	intptr_t *list_element_value          = NULL;
	static char *function                 = "libcdata_internal_list_insert_element_find_element";
	int compare_result                    = 0;
	int result                            = 1;
	int safe_element_index                = 0;

	if( internal_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid list.",
		 function );

		return( -1 );
	}
	if( value_compare_function == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value compare function.",
		 function );

		return( -1 );
	}
	if( ( insert_flags & ~( LIBCDATA_INSERT_FLAG_UNIQUE_ENTRIES ) ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported insert flags: 0x%02" PRIx8 ".",
		 function,
		 insert_flags );

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
	list_element   = internal_list->first_element;
	compare_result = LIBCDATA_COMPARE_GREATER;

	for( safe_element_index = 0;
	     safe_element_index < internal_list->number_of_elements;
	     safe_element_index++ )
	{
		if( libcdata_list_element_get_value(
		     list_element,
		     &list_element_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value from list element: %d.",
			 function,
			 safe_element_index );

			return( -1 );
		}
		compare_result = value_compare_function(
		                  value_to_insert,
		                  list_element_value,
		                  error );

		if( compare_result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to compare list element: %d.",
			 function,
			 safe_element_index );

			return( -1 );
		}
		else if( compare_result == LIBCDATA_COMPARE_EQUAL )
		{
			if( ( insert_flags & LIBCDATA_INSERT_FLAG_UNIQUE_ENTRIES ) != 0 )
			{
				result = 0;

				break;
			}
		}
		else if( compare_result == LIBCDATA_COMPARE_LESS )
		{
			break;
		}
		else if( compare_result != LIBCDATA_COMPARE_GREATER )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported value compare function return value: %d.",
			 function,
			 compare_result );

			return( -1 );
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
			 safe_element_index );

			return( -1 );
		}
	}
	if( ( compare_result == LIBCDATA_COMPARE_EQUAL )
	 || ( compare_result == LIBCDATA_COMPARE_LESS ) )
	{
		*element_index = safe_element_index;
		*element       = list_element;
	}
	else
	{
		*element_index = internal_list->number_of_elements;
		*element       = NULL;
	}
	return( result );
}

/* Inserts the element before the list element
 * If list_element is NULL the element is inserted before or as the first element in the list
 * Returns 1 if successful, or -1 on error
 */
int libcdata_internal_list_insert_element_before_element(
     libcdata_internal_list_t *internal_list,
     libcdata_list_element_t *list_element,
     libcdata_list_element_t *element_to_insert,
     libcerror_error_t **error )
{
	libcdata_list_element_t *backup_first_element = NULL;
	libcdata_list_element_t *backup_last_element  = NULL;
	libcdata_list_element_t *previous_element     = NULL;
	static char *function                         = "libcdata_internal_list_insert_element_before_element";

	if( internal_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid list.",
		 function );

		return( -1 );
	}
	if( element_to_insert == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid element to insert.",
		 function );

		return( -1 );
	}
	backup_first_element = internal_list->first_element;
	backup_last_element  = internal_list->last_element;

	if( list_element != NULL )
	{
		if( libcdata_list_element_get_previous_element(
		     list_element,
		     &previous_element,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve previous element from list element.",
			 function );

			return( -1 );
		}
	}
	if( internal_list->number_of_elements == 0 )
	{
		internal_list->first_element = element_to_insert;
		internal_list->last_element  = element_to_insert;
	}
	else if( list_element == NULL )
	{
		if( libcdata_internal_list_set_last_element(
		     internal_list,
		     element_to_insert,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set last list element.",
			 function );

			goto on_error;
		}
	}
	else
	{
		if( libcdata_list_element_set_elements(
		     element_to_insert,
		     previous_element,
		     list_element,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set previous and next element of element to insert.",
			 function );

			goto on_error;
		}
		if( internal_list->first_element == list_element )
		{
			internal_list->first_element = element_to_insert;
		}
		else
		{
			if( libcdata_list_element_set_next_element(
			     previous_element,
			     element_to_insert,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set next element of previous list element.",
				 function );

				goto on_error;
			}
		}
		if( libcdata_list_element_set_previous_element(
		     list_element,
		     element_to_insert,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set previous element of list element.",
			 function );

			goto on_error;
		}
	}
	internal_list->number_of_elements += 1;

	return( 1 );

on_error:
	if( element_to_insert != NULL )
	{
		libcdata_list_element_set_elements(
		 element_to_insert,
		 NULL,
		 NULL,
		 NULL );
	}
	if( previous_element != NULL )
	{
		libcdata_list_element_set_next_element(
		 previous_element,
		 list_element,
		 NULL );
	}
	if( list_element != NULL )
	{
		libcdata_list_element_set_previous_element(
		 list_element,
		 previous_element,
		 NULL );
	}
	internal_list->first_element = backup_first_element;
	internal_list->last_element  = backup_last_element;

	return( -1 );
}

/* Inserts a list element into the list
 *
 * Uses the value_compare_function to determine the order of the entries
 * The value_compare_function should return LIBCDATA_COMPARE_LESS,
 * LIBCDATA_COMPARE_EQUAL, LIBCDATA_COMPARE_GREATER if successful or -1 on error
 *
 * Duplicate entries are allowed by default and inserted after the last duplicate value.
 * Only allowing unique entries can be enforced by setting the flag LIBCDATA_INSERT_FLAG_UNIQUE_ENTRIES
 *
 * Returns 1 if successful, 0 if the list element already exists or -1 on error
 */
int libcdata_list_insert_element(
     libcdata_list_t *list,
     libcdata_list_element_t *element_to_insert,
     int (*value_compare_function)(
            intptr_t *first_value,
            intptr_t *second_value,
            libcerror_error_t **error ),
     uint8_t insert_flags,
     libcerror_error_t **error )
{
	libcdata_list_element_t *existing_element = NULL;
	static char *function                     = "libcdata_list_insert_element";
	int result                                = 0;

	result = libcdata_list_insert_element_with_existing(
	          list,
	          element_to_insert,
	          value_compare_function,
	          insert_flags,
	          &existing_element,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to insert element into list.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Inserts a list element into the list
 *
 * Uses the value_compare_function to determine the order of the entries
 * The value_compare_function should return LIBCDATA_COMPARE_LESS,
 * LIBCDATA_COMPARE_EQUAL, LIBCDATA_COMPARE_GREATER if successful or -1 on error
 *
 * Duplicate entries are allowed by default and inserted after the last duplicate value.
 * Only allowing unique entries can be enforced by setting the flag LIBCDATA_INSERT_FLAG_UNIQUE_ENTRIES
 *
 * Returns 1 if successful, 0 if the list element already exists or -1 on error
 */
int libcdata_list_insert_element_with_existing(
     libcdata_list_t *list,
     libcdata_list_element_t *element_to_insert,
     int (*value_compare_function)(
            intptr_t *first_value,
            intptr_t *second_value,
            libcerror_error_t **error ),
     uint8_t insert_flags,
     libcdata_list_element_t **existing_element,
     libcerror_error_t **error )
{
	libcdata_internal_list_t *internal_list        = NULL;
	libcdata_list_element_t *next_element          = NULL;
	libcdata_list_element_t *previous_element      = NULL;
	libcdata_list_element_t *safe_existing_element = NULL;
	intptr_t *value_to_insert                      = NULL;
	static char *function                          = "libcdata_list_insert_element_with_existing";
	int element_index                              = 0;
	int result                                     = 1;

#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	libcdata_list_element_t *backup_first_element  = NULL;
	libcdata_list_element_t *backup_last_element   = NULL;
#endif

	if( list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid list.",
		 function );

		return( -1 );
	}
	internal_list = (libcdata_internal_list_t *) list;

	if( internal_list->number_of_elements == 0 )
	{
		if( internal_list->first_element != NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: corruption detected - first element already set.",
			 function );

			return( -1 );
		}
		if( internal_list->last_element != NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: corruption detected - last element already set.",
			 function );

			return( -1 );
		}
	}
	else
	{
		if( internal_list->first_element == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: corruption detected - missing first element.",
			 function );

			return( -1 );
		}
		if( internal_list->last_element == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: corruption detected - missing last element.",
			 function );

			return( -1 );
		}
	}
	if( element_to_insert == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid element to insert.",
		 function );

		return( -1 );
	}
	if( value_compare_function == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value compare function.",
		 function );

		return( -1 );
	}
	if( ( insert_flags & ~( LIBCDATA_INSERT_FLAG_UNIQUE_ENTRIES ) ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported insert flags: 0x%02" PRIx8 ".",
		 function,
		 insert_flags );

		return( -1 );
	}
	if( existing_element == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid existing element.",
		 function );

		return( -1 );
	}
	*existing_element = NULL;

	if( libcdata_list_element_get_elements(
	     element_to_insert,
	     &previous_element,
	     &next_element,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve previous and next element from element to insert.",
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
		 "%s: invalid element to insert - already part of a list.",
		 function );

		return( -1 );
	}
	if( libcdata_list_element_get_value(
	     element_to_insert,
	     &value_to_insert,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value from element to insert.",
		 function );

		return( -1 );
	}
#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_list->read_write_lock,
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
	backup_first_element = internal_list->first_element;
	backup_last_element  = internal_list->last_element;

#endif /* defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA ) */

	result = libcdata_internal_list_insert_element_find_element(
	          internal_list,
	          value_to_insert,
	          value_compare_function,
	          insert_flags,
	          &element_index,
	          &safe_existing_element,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to find element in list.",
		 function );

		result = -1;
	}
	else if( result == 0 )
	{
		*existing_element = safe_existing_element;
	}
	else
	{
		if( safe_existing_element != NULL )
		{
			if( libcdata_list_element_get_elements(
			     safe_existing_element,
			     &previous_element,
			     &next_element,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve previous and next element from list element: %d.",
				 function,
				 element_index );

				result = -1;
			}
		}
		if( result == 1 )
		{
			if( libcdata_internal_list_insert_element_before_element(
			     internal_list,
			     safe_existing_element,
			     element_to_insert,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
				 "%s: unable to insert element before list element.",
				 function );

				result = -1;
			}
		}
	}
#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_list->read_write_lock,
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
	return( result );

#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )

on_error:
	if( result == 1 )
	{
		libcdata_list_element_set_elements(
		 element_to_insert,
		 NULL,
		 NULL,
		 NULL );

		if( safe_existing_element != NULL )
		{
			libcdata_list_element_set_elements(
			 safe_existing_element,
			 previous_element,
			 next_element,
			 NULL );
		}
		else if( backup_last_element != NULL )
		{
			libcdata_list_element_set_next_element(
			 backup_last_element,
			 NULL,
			 NULL );
		}
		internal_list->first_element = backup_first_element;
		internal_list->last_element  = backup_last_element;

		internal_list->number_of_elements -= 1;
	}
	return( -1 );

#endif /* defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA ) */
}

/* Inserts a value to the list
 *
 * Creates a new list element
 *
 * Uses the value_compare_function to determine the order of the entries
 * The value_compare_function should return LIBCDATA_COMPARE_LESS,
 * LIBCDATA_COMPARE_EQUAL, LIBCDATA_COMPARE_GREATER if successful or -1 on error
 *
 * Duplicate entries are allowed by default and inserted after the last duplicate value.
 * Only allowing unique entries can be enforced by setting the flag LIBCDATA_INSERT_FLAG_UNIQUE_ENTRIES
 *
 * Returns 1 if successful, 0 if the list element already exists or -1 on error
 */
int libcdata_list_insert_value(
     libcdata_list_t *list,
     intptr_t *value,
     int (*value_compare_function)(
            intptr_t *first_value,
            intptr_t *second_value,
            libcerror_error_t **error ),
     uint8_t insert_flags,
     libcerror_error_t **error )
{
	intptr_t *existing_value = NULL;
	static char *function    = "libcdata_list_insert_value";
	int result               = 0;

	result = libcdata_list_insert_value_with_existing(
	          list,
	          value,
	          value_compare_function,
	          insert_flags,
	          &existing_value,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to insert value into list.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Inserts a value to the list
 *
 * Creates a new list element
 *
 * Uses the value_compare_function to determine the order of the entries
 * The value_compare_function should return LIBCDATA_COMPARE_LESS,
 * LIBCDATA_COMPARE_EQUAL, LIBCDATA_COMPARE_GREATER if successful or -1 on error
 *
 * Duplicate entries are allowed by default and inserted after the last duplicate value.
 * Only allowing unique entries can be enforced by setting the flag LIBCDATA_INSERT_FLAG_UNIQUE_ENTRIES
 *
 * Returns 1 if successful, 0 if the list element already exists or -1 on error
 */
int libcdata_list_insert_value_with_existing(
     libcdata_list_t *list,
     intptr_t *value,
     int (*value_compare_function)(
            intptr_t *first_value,
            intptr_t *second_value,
            libcerror_error_t **error ),
     uint8_t insert_flags,
     intptr_t **existing_value,
     libcerror_error_t **error )
{
	libcdata_list_element_t *element          = NULL;
	libcdata_list_element_t *existing_element = NULL;
	static char *function                     = "libcdata_list_insert_value_with_existing";
	int result                                = 1;

	if( existing_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid existing value.",
		 function );

		return( -1 );
	}
	*existing_value = NULL;

	if( libcdata_list_element_initialize(
	     &element,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create list element.",
		 function );

		return( -1 );
	}
	if( libcdata_list_element_set_value(
	     element,
	     value,
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
	result = libcdata_list_insert_element_with_existing(
	          list,
	          element,
	          value_compare_function,
	          insert_flags,
	          &existing_element,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to insert element to list.",
		 function );

		goto on_error;
	}
	else if( result == 0 )
	{
		if( libcdata_list_element_free(
		     &element,
		     NULL,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free list element.",
			 function );

			goto on_error;
		}
		if( libcdata_list_element_get_value(
		     existing_element,
		     existing_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value from existing list element.",
			 function );

			goto on_error;
		}
	}
	return( result );

on_error:
	if( element != NULL )
	{
		libcdata_list_element_free(
		 &element,
		 NULL,
		 NULL );
	}
	return( -1 );
}

/* Removes a list element from the list
 * Returns 1 if successful or -1 on error
 */
int libcdata_list_remove_element(
     libcdata_list_t *list,
     libcdata_list_element_t *element_to_remove,
     libcerror_error_t **error )
{
	libcdata_internal_list_t *internal_list       = NULL;
	libcdata_list_element_t *next_element         = NULL;
	libcdata_list_element_t *previous_element     = NULL;
	static char *function                         = "libcdata_list_remove_element";
	int result                                    = 1;

#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	libcdata_list_element_t *backup_first_element = NULL;
	libcdata_list_element_t *backup_last_element  = NULL;
#endif

	if( list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid list.",
		 function );

		return( -1 );
	}
	internal_list = (libcdata_internal_list_t *) list;

	if( internal_list->number_of_elements == 0 )
	{
		if( internal_list->first_element != NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: corruption detected - first element already set.",
			 function );

			return( -1 );
		}
		if( internal_list->last_element != NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: corruption detected - last element already set.",
			 function );

			return( -1 );
		}
	}
	else
	{
		if( internal_list->first_element == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: corruption detected - missing first element.",
			 function );

			return( -1 );
		}
		if( internal_list->last_element == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: corruption detected - missing last element.",
			 function );

			return( -1 );
		}
	}
	if( element_to_remove == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid element to remove.",
		 function );

		return( -1 );
	}
	if( libcdata_list_element_get_elements(
	     element_to_remove,
	     &previous_element,
	     &next_element,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve previous and next element from element to remove.",
		 function );

		return( -1 );
	}
#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_list->read_write_lock,
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
	backup_first_element = internal_list->first_element;
	backup_last_element  = internal_list->last_element;

#endif
	result = libcdata_list_element_set_elements(
	          element_to_remove,
	          NULL,
	          NULL,
	          error );

	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set previous and next element of element to remove.",
		 function );

		result = -1;
	}
	if( result == 1 )
	{
		if( next_element != NULL )
		{
			result = libcdata_list_element_set_previous_element(
			          next_element,
			          previous_element,
			          error );

			if( result != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set previous element of next element.",
				 function );

				libcdata_list_element_set_elements(
				 element_to_remove,
				 previous_element,
				 next_element,
				 NULL );

				result = -1;
			}
		}
	}
	if( result == 1 )
	{
		if( previous_element != NULL )
		{
			result = libcdata_list_element_set_next_element(
			          previous_element,
			          next_element,
			          error );

			if( result != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set next element of previous element.",
				 function );

				if( next_element != NULL )
				{
					libcdata_list_element_set_previous_element(
					 next_element,
					 element_to_remove,
					 NULL );
				}
				libcdata_list_element_set_elements(
				 element_to_remove,
				 previous_element,
				 next_element,
				 NULL );

				result = -1;
			}
		}
	}
	if( result == 1 )
	{
		if( element_to_remove == internal_list->first_element )
		{
			internal_list->first_element = next_element;
		}
		if( element_to_remove == internal_list->last_element )
		{
			internal_list->last_element = previous_element;
		}
		internal_list->number_of_elements -= 1;
	}
#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_list->read_write_lock,
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
	return( result );

#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
on_error:
	if( result == 1 )
	{
		libcdata_list_element_set_elements(
		 element_to_remove,
		 previous_element,
		 next_element,
		 NULL );

		if( next_element != NULL )
		{
			libcdata_list_element_set_previous_element(
			 next_element,
			 element_to_remove,
			 NULL );
		}
		if( previous_element != NULL )
		{
			libcdata_list_element_set_next_element(
			 previous_element,
			 element_to_remove,
			 NULL );
		}
		internal_list->first_element = backup_first_element;
		internal_list->last_element  = backup_last_element;

		internal_list->number_of_elements += 1;
	}
	return( -1 );
#endif
}

