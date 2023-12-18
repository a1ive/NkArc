/*
 * Balanced tree values list functions
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

#include "libcdata_btree_values_list.h"
#include "libcdata_list.h"
#include "libcdata_list_element.h"
#include "libcdata_types.h"

/* Frees a B-tree values list
 * Returns 1 if successful or -1 on error
 */
int libcdata_btree_values_list_free(
     libcdata_list_t **values_list,
     libcerror_error_t **error )
{
	static char *function = "libcdata_btree_values_list_free";
	int result            = 1;

	if( values_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid values list.",
		 function );

		return( -1 );
	}
	if( libcdata_list_free(
	     values_list,
	     NULL,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free values list.",
		 function );

		result = -1;
	}
	return( result );
}

/* Removes an element from the values list
 * Returns 1 if successful or -1 on error
 */
int libcdata_btree_values_list_remove_element(
     libcdata_list_t *values_list,
     libcdata_list_element_t **values_list_element,
     libcerror_error_t **error )
{
	static char *function = "libcdata_btree_values_list_remove_element";

	if( values_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid values list.",
		 function );

		return( -1 );
	}
	if( values_list_element == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid values list element.",
		 function );

		return( -1 );
	}
	if( libcdata_list_remove_element(
	     values_list,
	     *values_list_element,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_REMOVE_FAILED,
		 "%s: unable to remove element from values list.",
		 function );

		return( -1 );
	}
	if( libcdata_list_element_free(
	     values_list_element,
	     NULL,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free element.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Replaces the values list element with the previous element
 * If available replacement_value is set to the value of the previous element
 * Returns 1 if successful or -1 on error
 */
int libcdata_btree_values_list_replace_element_with_previous(
     libcdata_list_t *values_list,
     libcdata_list_element_t **values_list_element,
     intptr_t **replacement_value,
     libcerror_error_t **error )
{
	libcdata_list_element_t *previous_element = NULL;
	static char *function                     = "libcdata_btree_values_list_replace_element_with_previous";

	if( values_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid values list.",
		 function );

		return( -1 );
	}
	if( values_list_element == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid values list element.",
		 function );

		return( -1 );
	}
	if( libcdata_list_element_get_previous_element(
	     *values_list_element,
	     &previous_element,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve previous element from values list element.",
		 function );

		return( -1 );
	}
	if( previous_element != NULL )
	{
		if( libcdata_list_element_get_value(
		     previous_element,
		     replacement_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value from previous element.",
			 function );

			return( -1 );
		}
	}
	if( libcdata_btree_values_list_remove_element(
	     values_list,
	     values_list_element,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_REMOVE_FAILED,
		 "%s: unable to remove element from values list.",
		 function );

		return( -1 );
	}
	*values_list_element = previous_element;

	return( 1 );
}

