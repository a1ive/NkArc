/*
 * Balanced tree type functions
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

#include "libcdata_array.h"
#include "libcdata_btree.h"
#include "libcdata_btree_node.h"
#include "libcdata_btree_values_list.h"
#include "libcdata_definitions.h"
#include "libcdata_list.h"
#include "libcdata_list_element.h"
#include "libcdata_tree_node.h"
#include "libcdata_types.h"

/* Creates a tree
 * Make sure the value tree is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libcdata_btree_initialize(
     libcdata_btree_t **tree,
     int maximum_number_of_values,
     libcerror_error_t **error )
{
	libcdata_internal_btree_t *internal_tree = NULL;
	static char *function                    = "libcdata_btree_initialize";

	if( tree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid tree.",
		 function );

		return( -1 );
	}
	if( *tree != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid tree value already set.",
		 function );

		return( -1 );
	}
	if( maximum_number_of_values <= 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid maximum number of values value out of bounds.",
		 function );

		return( -1 );
	}
	internal_tree = memory_allocate_structure(
	                 libcdata_internal_btree_t );

	if( internal_tree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create tree.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     internal_tree,
	     0,
	     sizeof( libcdata_internal_btree_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear tree.",
		 function );

		memory_free(
		 internal_tree );

		return( -1 );
	}
	if( libcdata_array_initialize(
	     &( internal_tree->values_array ),
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create values array.",
		 function );

		goto on_error;
	}
	if( libcdata_tree_node_initialize(
	     &( internal_tree->root_node ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create root node.",
		 function );

		goto on_error;
	}
	internal_tree->maximum_number_of_values = maximum_number_of_values;

#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	if( libcthreads_read_write_lock_initialize(
	     &( internal_tree->read_write_lock ),
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
	*tree = (libcdata_btree_t *) internal_tree;

	return( 1 );

on_error:
	if( internal_tree != NULL )
	{
		if( internal_tree->values_array != NULL )
		{
			libcdata_array_free(
			 &( internal_tree->values_array ),
			 NULL,
			 NULL );
		}
		memory_free(
		 internal_tree );
	}
	return( -1 );
}

/* Frees a tree, its sub nodes
 * Uses the value_free_function to free the value
 * Returns 1 if successful or -1 on error
 */
int libcdata_btree_free(
     libcdata_btree_t **tree,
     int (*value_free_function)(
            intptr_t **value,
            libcerror_error_t **error ),
     libcerror_error_t **error )
{
	libcdata_internal_btree_t *internal_tree = NULL;
	static char *function                    = "libcdata_btree_free";
	int result                               = 1;

	if( tree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid tree.",
		 function );

		return( -1 );
	}
	if( *tree != NULL )
	{
		internal_tree = (libcdata_internal_btree_t *) *tree;
		*tree         = NULL;

#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
		if( libcthreads_read_write_lock_free(
		     &( internal_tree->read_write_lock ),
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
		if( libcdata_tree_node_free(
		     &( internal_tree->root_node ),
		     (int (*)(intptr_t **, libcerror_error_t **)) &libcdata_btree_values_list_free,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free root node.",
			 function );

			result = -1;
		}
		if( libcdata_array_free(
		     &( internal_tree->values_array ),
		     value_free_function,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free values array.",
			 function );

			result = -1;
		}
		memory_free(
		 internal_tree );
	}
	return( result );
}

/* TODO add clone function ? */

/* Retrieves the number of values in the tree
 * Returns 1 if successful or -1 on error
 */
int libcdata_btree_get_number_of_values(
     libcdata_btree_t *tree,
     int *number_of_values,
     libcerror_error_t **error )
{
	libcdata_internal_btree_t *internal_tree = NULL;
	static char *function                    = "libcdata_btree_get_number_of_values";

	if( tree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid tree.",
		 function );

		return( -1 );
	}
	internal_tree = (libcdata_internal_btree_t *) tree;

	if( libcdata_array_get_number_of_entries(
	     internal_tree->values_array,
	     number_of_values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of values array entries.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves a specific value
 * Returns 1 if successful or -1 on error
 */
int libcdata_btree_get_value_by_index(
     libcdata_btree_t *tree,
     int value_index,
     intptr_t **value,
     libcerror_error_t **error )
{
	libcdata_internal_btree_t *internal_tree = NULL;
	static char *function                    = "libcdata_btree_get_value_by_index";

	if( tree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid tree.",
		 function );

		return( -1 );
	}
	internal_tree = (libcdata_internal_btree_t *) tree;

	if( libcdata_array_get_entry_by_index(
	     internal_tree->values_array,
	     value_index,
	     value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value: %d from array.",
		 function,
		 value_index );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves a value from the tree
 *
 * Uses the value_compare_function to determine the similarity of the entries
 * The value_compare_function should return LIBCDATA_COMPARE_LESS,
 * LIBCDATA_COMPARE_EQUAL, LIBCDATA_COMPARE_GREATER if successful or -1 on error
 *
 * Returns 1 if successful, 0 if no such value or -1 on error
 */
int libcdata_btree_get_value_by_value(
     libcdata_btree_t *tree,
     intptr_t *value,
     int (*value_compare_function)(
            intptr_t *first_value,
            intptr_t *second_value,
            libcerror_error_t **error ),
     libcdata_tree_node_t **upper_node,
     intptr_t **existing_value,
     libcerror_error_t **error )
{
	libcdata_internal_btree_t *internal_tree       = NULL;
	libcdata_list_element_t *existing_list_element = NULL;
	static char *function                          = "libcdata_btree_get_value_by_value";
	int result                                     = 0;

	if( tree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid tree.",
		 function );

		return( -1 );
	}
	internal_tree = (libcdata_internal_btree_t *) tree;

	result = libcdata_btree_node_get_upper_node_by_value(
	          internal_tree->root_node,
	          value,
	          value_compare_function,
	          upper_node,
	          &existing_list_element,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve upper node by value.",
		 function );

		return( -1 );
	}
	else if( result != 0 )
	{
		if( libcdata_list_element_get_value(
		     existing_list_element,
		     existing_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value from values list element.",
			 function );

			return( -1 );
		}
	}
	else
	{
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
	}
	return( result );
}

/* Inserts a value into a tree
 *
 * Uses the value_compare_function to determine the order of the entries
 * The value_compare_function should return LIBCDATA_COMPARE_LESS,
 * LIBCDATA_COMPARE_EQUAL, LIBCDATA_COMPARE_GREATER if successful or -1 on error
 *
 * Returns 1 if successful, 0 if the value already exists or -1 on error
 */
int libcdata_btree_insert_value(
     libcdata_btree_t *tree,
     int *value_index,
     intptr_t *value,
     int (*value_compare_function)(
            intptr_t *first_value,
            intptr_t *second_value,
            libcerror_error_t **error ),
     libcdata_tree_node_t **upper_node,
     intptr_t **existing_value,
     libcerror_error_t **error )
{
	libcdata_internal_btree_t *internal_tree       = NULL;
	libcdata_list_t *values_list                   = NULL;
	libcdata_list_element_t *existing_list_element = NULL;
	static char *function                          = "libcdata_btree_insert_value";
	int number_of_values_list_elements             = 0;
	int result                                     = 0;

	if( tree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid tree.",
		 function );

		return( -1 );
	}
	internal_tree = (libcdata_internal_btree_t *) tree;

	if( upper_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid upper node.",
		 function );

		return( -1 );
	}
	if( value_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value index.",
		 function );

		return( -1 );
	}
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
	result = libcdata_btree_node_get_upper_node_by_value(
	          internal_tree->root_node,
	          value,
	          value_compare_function,
	          upper_node,
	          &existing_list_element,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve upper node in root node.",
		 function );

		return( -1 );
	}
	else if( result != 0 )
	{
		if( libcdata_list_element_get_value(
		     existing_list_element,
		     existing_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value from values list element.",
			 function );

			return( -1 );
		}
		return( 0 );
	}
	*existing_value = NULL;

	if( libcdata_btree_node_insert_value(
	     *upper_node,
	     value,
	     value_compare_function,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to insert value in upper node.",
		 function );

		return( -1 );
	}
	if( libcdata_tree_node_get_value(
	     *upper_node,
	     (intptr_t **) &values_list,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve values list.",
		 function );

		return( -1 );
	}
	if( libcdata_list_get_number_of_elements(
	     values_list,
	     &number_of_values_list_elements,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of values list elements.",
		 function );

		return( -1 );
	}
	if( number_of_values_list_elements >= internal_tree->maximum_number_of_values )
	{
		if( libcdata_btree_node_split(
		     *upper_node,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to split upper node.",
			 function );

			return( -1 );
		}
/* TODO do merge of upper node with its parent node */
/* TODO loop until number_of_values_list_elements < internal_tree->maximum_number_of_values */

		/* Make sure existing list element updated after the split
		 */
		result = libcdata_btree_node_get_sub_node_by_value(
		          *upper_node,
		          value,
		          value_compare_function,
		          upper_node,
		          &existing_list_element,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve split sub node by value.",
			 function );

			return( -1 );
		}
		result = libcdata_btree_node_get_sub_node_by_value(
		          *upper_node,
		          value,
		          value_compare_function,
		          upper_node,
		          &existing_list_element,
		          error );

		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve split sub node by value.",
			 function );

			return( -1 );
		}
	}
	if( libcdata_array_append_entry(
	     internal_tree->values_array,
	     value_index,
	     value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append value to values array.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Replaces a value in the tree
 * Returns 1 if successful or -1 on error
 */
int libcdata_btree_replace_value(
     libcdata_btree_t *tree,
     libcdata_tree_node_t *upper_node,
     int *value_index,
     intptr_t *value,
     int *replacement_value_index,
     intptr_t *replacement_value,
     libcerror_error_t **error )
{
	libcdata_internal_btree_t *internal_tree = NULL;
	intptr_t *check_value                    = NULL;
	static char *function                    = "libcdata_btree_replace_value";
	int number_of_sub_nodes                  = 0;

	if( tree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid tree.",
		 function );

		return( -1 );
	}
	internal_tree = (libcdata_internal_btree_t *) tree;

	if( upper_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid upper node.",
		 function );

		return( -1 );
	}
	if( value_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value index.",
		 function );

		return( -1 );
	}
	if( replacement_value_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value index.",
		 function );

		return( -1 );
	}
	if( libcdata_tree_node_get_number_of_sub_nodes(
	     upper_node,
	     &number_of_sub_nodes,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of sub nodes.",
		 function );

		return( -1 );
	}
	if( number_of_sub_nodes != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: cannot replace upper node with sub nodes.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_entry_by_index(
	     internal_tree->values_array,
	     *value_index,
	     &check_value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value: %d from array.",
		 function,
		 *value_index );

		return( -1 );
	}
	if( value != check_value )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid value: %d value out of bounds.",
		 function,
		 *value_index );

		return( -1 );
	}
	if( libcdata_btree_node_replace_value(
	     upper_node,
	     value,
	     replacement_value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_REMOVE_FAILED,
		 "%s: unable to replace value: %d.",
		 function,
		 *value_index );

		return( -1 );
	}
	if( libcdata_array_set_entry_by_index(
	     internal_tree->values_array,
	     *value_index,
	     replacement_value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to set value: %d in values array.",
		 function,
		 *value_index );

		return( -1 );
	}
	*replacement_value_index = *value_index;
	*value_index             = -1;

	return( 1 );
}

/* Removes a value from the tree
 * Returns 1 if successful or -1 on error
 */
int libcdata_btree_remove_value(
     libcdata_btree_t *tree,
     libcdata_tree_node_t *upper_node,
     int *value_index,
     intptr_t *value,
     libcerror_error_t **error )
{
	libcdata_internal_btree_t *internal_tree = NULL;
	intptr_t *check_value                    = NULL;
	static char *function                    = "libcdata_btree_remove_value";
	int number_of_sub_nodes                  = 0;

	if( tree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid tree.",
		 function );

		return( -1 );
	}
	internal_tree = (libcdata_internal_btree_t *) tree;

	if( upper_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid upper node.",
		 function );

		return( -1 );
	}
	if( value_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value index.",
		 function );

		return( -1 );
	}
	if( libcdata_tree_node_get_number_of_sub_nodes(
	     upper_node,
	     &number_of_sub_nodes,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of sub nodes.",
		 function );

		return( -1 );
	}
	if( number_of_sub_nodes != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: cannot replace upper node with sub nodes.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_entry_by_index(
	     internal_tree->values_array,
	     *value_index,
	     &check_value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value: %d from array.",
		 function,
		 *value_index );

		return( -1 );
	}
	if( value != check_value )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid value: %d value out of bounds.",
		 function,
		 *value_index );

		return( -1 );
	}
	if( libcdata_btree_node_remove_value(
	     upper_node,
	     value,
	     NULL,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_REMOVE_FAILED,
		 "%s: unable to remove value: %d from upper node.",
		 function,
		 *value_index );

		return( -1 );
	}
/* TODO reshuffle values array ?
 * Better to mark and ignore deleted items, otherwise index values need to be updated as well
 */
	if( libcdata_array_set_entry_by_index(
	     internal_tree->values_array,
	     *value_index,
	     NULL,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to set value: %d in values array.",
		 function,
		 *value_index );

		return( -1 );
	}
	*value_index = -1;

	return( 1 );
}

