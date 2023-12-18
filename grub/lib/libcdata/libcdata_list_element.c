/*
 * List element functions
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

#include "libcdata_list_element.h"
#include "libcdata_types.h"

/* Creates a list element
 * Make sure the value element is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libcdata_list_element_initialize(
     libcdata_list_element_t **element,
     libcerror_error_t **error )
{
	libcdata_internal_list_element_t *internal_element = NULL;
	static char *function                              = "libcdata_list_element_initialize";

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
	if( *element != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid element value already set.",
		 function );

		return( -1 );
	}
	internal_element = memory_allocate_structure(
	                    libcdata_internal_list_element_t );

	if( internal_element == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create list element.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     internal_element,
	     0,
	     sizeof( libcdata_internal_list_element_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear list element.",
		 function );

		memory_free(
		 internal_element );

		return( -1 );
	}
#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	if( libcthreads_read_write_lock_initialize(
	     &( internal_element->read_write_lock ),
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
	*element = (libcdata_list_element_t *) internal_element;

	return( 1 );

on_error:
	if( internal_element != NULL )
	{
		memory_free(
		 internal_element );
	}
	return( -1 );
}

/* Frees a list element
 * Uses the value_free_function to free the element value
 * Returns 1 if successful or -1 on error
 */
int libcdata_list_element_free(
     libcdata_list_element_t **element,
     int (*value_free_function)(
            intptr_t **value,
            libcerror_error_t **error ),
     libcerror_error_t **error )
{
	libcdata_internal_list_element_t *internal_element = NULL;
	static char *function                              = "libcdata_list_element_free";
	int result                                         = 1;

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
	if( *element != NULL )
	{
		internal_element = (libcdata_internal_list_element_t *) *element;

		if( ( internal_element->previous_element != NULL )
		 || ( internal_element->next_element != NULL ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
			 "%s: list element part of a list.",
			 function );

			return( -1 );
		}
		*element = NULL;

#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
		if( libcthreads_read_write_lock_free(
		     &( internal_element->read_write_lock ),
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
		if( value_free_function != NULL )
		{
			if( value_free_function(
			     &( internal_element->value ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free value.",
				 function );

				result = -1;
			}
		}
		memory_free(
		 internal_element );
	}
	return( result );
}

/* Retrieves the parent list from the list element
 * Returns 1 if successful or -1 on error
 */
int libcdata_list_element_get_parent_list(
     libcdata_list_element_t *element,
     intptr_t **parent_list,
     libcerror_error_t **error )
{
	libcdata_internal_list_element_t *internal_element = NULL;
	static char *function                              = "libcdata_list_element_get_parent_list";

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
	internal_element = (libcdata_internal_list_element_t *) element;

	if( parent_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid parent list.",
		 function );

		return( -1 );
	}
#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_element->read_write_lock,
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
	*parent_list = internal_element->parent_list;

#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_element->read_write_lock,
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

/* Sets the parent list in the list element
 * Returns 1 if successful or -1 on error
 */
int libcdata_list_element_set_parent_list(
     libcdata_list_element_t *element,
     intptr_t *parent_list,
     libcerror_error_t **error )
{
	libcdata_internal_list_element_t *internal_element = NULL;
	static char *function                              = "libcdata_list_element_set_parent_list";

#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	intptr_t *backup_parent_list                       = NULL;
#endif

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
	internal_element = (libcdata_internal_list_element_t *) element;

#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_element->read_write_lock,
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
	backup_parent_list = internal_element->parent_list;
#endif
	internal_element->parent_list = parent_list;

#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_element->read_write_lock,
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
	return( 1 );

#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
on_error:
	internal_element->parent_list = backup_parent_list;

	return( -1 );
#endif
}

/* Retrieves the previous element from the list element
 * Returns 1 if successful or -1 on error
 */
int libcdata_list_element_get_previous_element(
     libcdata_list_element_t *element,
     libcdata_list_element_t **previous_element,
     libcerror_error_t **error )
{
	libcdata_internal_list_element_t *internal_element = NULL;
	static char *function                              = "libcdata_list_element_get_previous_element";

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
	internal_element = (libcdata_internal_list_element_t *) element;

	if( previous_element == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid previous element.",
		 function );

		return( -1 );
	}
#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_element->read_write_lock,
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
	*previous_element = internal_element->previous_element;

#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_element->read_write_lock,
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

/* Sets the previous element in the list element
 * Returns 1 if successful or -1 on error
 */
int libcdata_list_element_set_previous_element(
     libcdata_list_element_t *element,
     libcdata_list_element_t *previous_element,
     libcerror_error_t **error )
{
	libcdata_internal_list_element_t *internal_element = NULL;
	static char *function                              = "libcdata_list_element_set_previous_element";

#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	libcdata_list_element_t *backup_previous_element   = NULL;
#endif

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
	internal_element = (libcdata_internal_list_element_t *) element;

#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_element->read_write_lock,
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
	backup_previous_element = internal_element->previous_element;
#endif
	internal_element->previous_element = previous_element;

#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_element->read_write_lock,
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
	return( 1 );

#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
on_error:
	internal_element->previous_element = backup_previous_element;

	return( -1 );
#endif
}

/* Retrieves the next element from the list element
 * Returns 1 if successful or -1 on error
 */
int libcdata_list_element_get_next_element(
     libcdata_list_element_t *element,
     libcdata_list_element_t **next_element,
     libcerror_error_t **error )
{
	libcdata_internal_list_element_t *internal_element = NULL;
	static char *function                              = "libcdata_list_element_get_next_element";

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
	internal_element = (libcdata_internal_list_element_t *) element;

	if( next_element == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid next element.",
		 function );

		return( -1 );
	}
#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_element->read_write_lock,
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
	*next_element = internal_element->next_element;

#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_element->read_write_lock,
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

/* Sets the next element in the list element
 * Returns 1 if successful or -1 on error
 */
int libcdata_list_element_set_next_element(
     libcdata_list_element_t *element,
     libcdata_list_element_t *next_element,
     libcerror_error_t **error )
{
	libcdata_internal_list_element_t *internal_element = NULL;
	static char *function                              = "libcdata_list_element_set_next_element";

#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	libcdata_list_element_t *backup_next_element       = NULL;
#endif

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
	internal_element = (libcdata_internal_list_element_t *) element;

#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_element->read_write_lock,
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
	backup_next_element = internal_element->next_element;
#endif
	internal_element->next_element = next_element;

#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_element->read_write_lock,
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
	return( 1 );

#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
on_error:
	internal_element->next_element = backup_next_element;

	return( -1 );
#endif
}

/* Retrieves the previous and next element from the list element
 * Returns 1 if successful or -1 on error
 */
int libcdata_list_element_get_elements(
     libcdata_list_element_t *element,
     libcdata_list_element_t **previous_element,
     libcdata_list_element_t **next_element,
     libcerror_error_t **error )
{
	libcdata_internal_list_element_t *internal_element = NULL;
	static char *function                              = "libcdata_list_element_get_elements";

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
	internal_element = (libcdata_internal_list_element_t *) element;

	if( previous_element == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid previous element.",
		 function );

		return( -1 );
	}
	if( next_element == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid next element.",
		 function );

		return( -1 );
	}
#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_element->read_write_lock,
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
	*previous_element = internal_element->previous_element;
	*next_element     = internal_element->next_element;

#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_element->read_write_lock,
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

/* Sets the previous and next element in the list element
 * Returns 1 if successful or -1 on error
 */
int libcdata_list_element_set_elements(
     libcdata_list_element_t *element,
     libcdata_list_element_t *previous_element,
     libcdata_list_element_t *next_element,
     libcerror_error_t **error )
{
	libcdata_internal_list_element_t *internal_element = NULL;
	static char *function                              = "libcdata_list_element_set_elements";

#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	libcdata_list_element_t *backup_next_element       = NULL;
	libcdata_list_element_t *backup_previous_element   = NULL;
#endif

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
	internal_element = (libcdata_internal_list_element_t *) element;

#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_element->read_write_lock,
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
	backup_previous_element = internal_element->previous_element;
	backup_next_element     = internal_element->next_element;
#endif
	internal_element->previous_element = previous_element;
	internal_element->next_element     = next_element;

#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_element->read_write_lock,
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
	return( 1 );

#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
on_error:
	internal_element->previous_element = backup_previous_element;
	internal_element->next_element     = backup_next_element;

	return( -1 );
#endif
}

/* Retrieves the value from the list element
 * Returns 1 if successful or -1 on error
 */
int libcdata_list_element_get_value(
     libcdata_list_element_t *element,
     intptr_t **value,
     libcerror_error_t **error )
{
	libcdata_internal_list_element_t *internal_element = NULL;
	static char *function                              = "libcdata_list_element_get_value";

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
	internal_element = (libcdata_internal_list_element_t *) element;

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
	     internal_element->read_write_lock,
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
	*value = internal_element->value;

#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_element->read_write_lock,
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

/* Sets the value in the list element
 * Returns 1 if successful or -1 on error
 */
int libcdata_list_element_set_value(
     libcdata_list_element_t *element,
     intptr_t *value,
     libcerror_error_t **error )
{
	libcdata_internal_list_element_t *internal_element = NULL;
	static char *function                              = "libcdata_list_element_set_value";

#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	intptr_t *backup_value                             = NULL;
#endif

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
	internal_element = (libcdata_internal_list_element_t *) element;

#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_element->read_write_lock,
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
	backup_value = internal_element->value;
#endif
	internal_element->value = value;

#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_element->read_write_lock,
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
	return( 1 );

#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
on_error:
	internal_element->value = backup_value;

	return( -1 );
#endif
}

