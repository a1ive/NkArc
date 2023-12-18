/*
 * Tree functions
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
#include "libcdata_tree_node.h"
#include "libcdata_types.h"

/* Creates a tree node
 * Make sure the value node is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libcdata_tree_node_initialize(
     libcdata_tree_node_t **node,
     libcerror_error_t **error )
{
	libcdata_internal_tree_node_t *internal_node = NULL;
	static char *function                        = "libcdata_tree_node_initialize";

	if( node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid node.",
		 function );

		return( -1 );
	}
	if( *node != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid node value already set.",
		 function );

		return( -1 );
	}
	internal_node = memory_allocate_structure(
	                 libcdata_internal_tree_node_t );

	if( internal_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create node.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     internal_node,
	     0,
	     sizeof( libcdata_internal_tree_node_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear node.",
		 function );

		memory_free(
		 internal_node );

		return( -1 );
	}
#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	if( libcthreads_read_write_lock_initialize(
	     &( internal_node->read_write_lock ),
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
	*node = (libcdata_tree_node_t *) internal_node;

	return( 1 );

on_error:
	if( internal_node != NULL )
	{
		memory_free(
		 internal_node );
	}
	return( -1 );
}

/* Frees a tree node, its sub nodes
 * Uses the value_free_function to free the value
 * Returns 1 if successful or -1 on error
 */
int libcdata_tree_node_free(
     libcdata_tree_node_t **node,
     int (*value_free_function)(
            intptr_t **value,
            libcerror_error_t **error ),
     libcerror_error_t **error )
{
	libcdata_internal_tree_node_t *internal_node = NULL;
	static char *function                        = "libcdata_tree_node_free";
	int result                                   = 1;

	if( node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid node.",
		 function );

		return( -1 );
	}
	if( *node != NULL )
	{
		internal_node = (libcdata_internal_tree_node_t *) *node;

		if( ( internal_node->parent_node != NULL )
		 || ( internal_node->previous_node != NULL )
		 || ( internal_node->next_node != NULL ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
			 "%s: invalid node - connected to other nodes.",
			 function );

			return( -1 );
		}
		*node = NULL;

		if( libcdata_tree_node_empty(
		     (libcdata_tree_node_t *) internal_node,
		     value_free_function,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to empty node.",
			 function );

			result = -1;
		}
#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
		if( libcthreads_read_write_lock_free(
		     &( internal_node->read_write_lock ),
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
		if( internal_node->value != NULL )
		{
			if( value_free_function != NULL )
			{
				if( value_free_function(
				     &( internal_node->value ),
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
				internal_node->value = NULL;
			}
		}
		memory_free(
		 internal_node );
	}
	return( result );
}

/* Empties a tree node and frees its sub nodes
 * Uses the value_free_function to free the value of the sub nodes
 * Returns 1 if successful or -1 on error
 */
int libcdata_tree_node_empty(
     libcdata_tree_node_t *tree_node,
     int (*value_free_function)(
            intptr_t **value,
            libcerror_error_t **error ),
     libcerror_error_t **error )
{
	libcdata_internal_tree_node_t *internal_node = NULL;
	libcdata_tree_node_t *next_node              = NULL;
	libcdata_tree_node_t *parent_node            = NULL;
	libcdata_tree_node_t *previous_node          = NULL;
	libcdata_tree_node_t *sub_node               = NULL;
	static char *function                        = "libcdata_tree_node_empty";
	int number_of_sub_nodes                      = 0;
	int result                                   = 1;
	int sub_node_index                           = 0;

	if( tree_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid tree node.",
		 function );

		return( -1 );
	}
	internal_node = (libcdata_internal_tree_node_t *) tree_node;

#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_node->read_write_lock,
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
	number_of_sub_nodes = internal_node->number_of_sub_nodes;

	sub_node = internal_node->first_sub_node;

	for( sub_node_index = 0;
	     sub_node_index < number_of_sub_nodes;
	     sub_node_index++ )
	{
		if( libcdata_tree_node_get_nodes(
		     sub_node,
		     &parent_node,
		     &previous_node,
		     &next_node,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve nodes of sub node: %d.",
			 function,
			 sub_node_index );

			goto on_error;
		}
		if( previous_node != NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
			 "%s: invalid sub node: %d - previous node is set.",
			 function,
			 sub_node_index );

			goto on_error;
		}
		internal_node->first_sub_node = next_node;

		if( internal_node->last_sub_node == sub_node )
		{
			internal_node->last_sub_node = next_node;
		}
		internal_node->number_of_sub_nodes -= 1;

		if( next_node != NULL )
		{
			if( libcdata_tree_node_set_previous_node(
			     next_node,
			     NULL,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set previous node of sub node: %d.",
				 function,
				 sub_node_index + 1 );

				goto on_error;
			}
		}
		if( libcdata_tree_node_set_nodes(
		     sub_node,
		     NULL,
		     NULL,
		     NULL,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set nodes of sub node: %d.",
			 function,
			 sub_node_index );

			goto on_error;
		}
		if( libcdata_tree_node_free(
		     &sub_node,
		     value_free_function,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free sub node: %d.",
			 function,
			 sub_node_index );

			result = -1;
		}
		sub_node = next_node;
	}
#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_node->read_write_lock,
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
	 internal_node->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

/* Clones the tree node and its sub nodes
 *
 * The values are cloned using the value_clone_function
 * On error the values are freed using the value_free_function
 *
 * Returns 1 if successful or -1 on error
 */
int libcdata_tree_node_clone(
     libcdata_tree_node_t **destination_node,
     libcdata_tree_node_t *source_node,
     int (*value_free_function)(
            intptr_t **value,
            libcerror_error_t **error ),
     int (*value_clone_function)(
            intptr_t **destination_value,
            intptr_t *source_value,
            libcerror_error_t **error ),
     libcerror_error_t **error )
{
	libcdata_internal_tree_node_t *internal_destination_node = NULL;
	libcdata_internal_tree_node_t *internal_source_node      = NULL;
	libcdata_tree_node_t *destination_sub_node               = NULL;
	libcdata_tree_node_t *sub_node                           = NULL;
	static char *function                                    = "libcdata_tree_node_clone";
	int sub_node_index                                       = 0;

	if( destination_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid destination tree node.",
		 function );

		return( -1 );
	}
	if( *destination_node != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid destination tree node already set.",
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
	if( source_node == NULL )
	{
		*destination_node = NULL;

		return( 1 );
	}
	internal_source_node = (libcdata_internal_tree_node_t *) source_node;

#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_source_node->read_write_lock,
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
	if( libcdata_tree_node_initialize(
	     (libcdata_tree_node_t **) &internal_destination_node,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create destination tree node.",
		 function );

		goto on_error;
	}
	if( internal_destination_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing destination tree node.",
		 function );

		goto on_error;
	}
	if( value_clone_function(
	     &( internal_destination_node->value ),
	     internal_source_node->value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create destination tree node value.",
		 function );

		goto on_error;
	}
	/* Clone the sub nodes
	 */
	sub_node = internal_source_node->first_sub_node;

	for( sub_node_index = 0;
	     sub_node_index < internal_source_node->number_of_sub_nodes;
	     sub_node_index++ )
	{
		if( sub_node == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: corruption detected in source sub node: %d.",
			 function,
			 sub_node_index );

			goto on_error;
		}
		if( libcdata_tree_node_clone(
		     &destination_sub_node,
		     sub_node,
		     value_free_function,
		     value_clone_function,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to create destination sub node: %d.",
			 function,
			 sub_node_index );

			goto on_error;
		}
		if( libcdata_internal_tree_node_append_node(
		     internal_destination_node,
		     destination_sub_node,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append sub node: %d to destination tree node.",
			 function,
			 sub_node_index );

			goto on_error;
		}
		destination_sub_node = NULL;

		if( libcdata_tree_node_get_next_node(
		     sub_node,
		     &sub_node,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve next node of sub node: %d.",
			 function,
			 sub_node_index );

			goto on_error;
		}
	}
#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_source_node->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		libcdata_tree_node_free(
		 &destination_sub_node,
		 value_free_function,
		 NULL );

		return( -1 );
	}
#endif
	*destination_node = (libcdata_tree_node_t *) internal_destination_node;

	return( 1 );

on_error:
#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	libcthreads_read_write_lock_release_for_read(
	 internal_source_node->read_write_lock,
	 NULL );
#endif
	if( destination_sub_node != NULL )
	{
		libcdata_tree_node_free(
		 &destination_sub_node,
		 value_free_function,
		 NULL );
	}
	if( internal_destination_node != NULL )
	{
		libcdata_tree_node_free(
		 (libcdata_tree_node_t **) &internal_destination_node,
		 value_free_function,
		 NULL );
	}
	return( -1 );
}

/* Retrieves the value from the tree node
 * Returns 1 if successful or -1 on error
 */
int libcdata_tree_node_get_value(
     libcdata_tree_node_t *node,
     intptr_t **value,
     libcerror_error_t **error )
{
	libcdata_internal_tree_node_t *internal_node = NULL;
	static char *function                        = "libcdata_tree_node_get_value";

	if( node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid node.",
		 function );

		return( -1 );
	}
	internal_node = (libcdata_internal_tree_node_t *) node;

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
	     internal_node->read_write_lock,
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
	*value = internal_node->value;

#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_node->read_write_lock,
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

/* Sets the value in the tree node
 * Returns 1 if successful or -1 on error
 */
int libcdata_tree_node_set_value(
     libcdata_tree_node_t *node,
     intptr_t *value,
     libcerror_error_t **error )
{
	libcdata_internal_tree_node_t *internal_node = NULL;
	static char *function                        = "libcdata_tree_node_set_value";

#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	intptr_t *backup_value                       = NULL;
#endif

	if( node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid node.",
		 function );

		return( -1 );
	}
	internal_node = (libcdata_internal_tree_node_t *) node;

#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_node->read_write_lock,
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
	backup_value = internal_node->value;
#endif

	internal_node->value = value;

#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_node->read_write_lock,
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
	internal_node->value = backup_value;

	return( -1 );
#endif
}

/* Retrieves the parent node from the tree node
 * Returns 1 if successful or -1 on error
 */
int libcdata_tree_node_get_parent_node(
     libcdata_tree_node_t *node,
     libcdata_tree_node_t **parent_node,
     libcerror_error_t **error )
{
	libcdata_internal_tree_node_t *internal_node = NULL;
	static char *function                        = "libcdata_tree_node_get_parent_node";

	if( node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid node.",
		 function );

		return( -1 );
	}
	internal_node = (libcdata_internal_tree_node_t *) node;

	if( parent_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid parent node.",
		 function );

		return( -1 );
	}
#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_node->read_write_lock,
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
	*parent_node = internal_node->parent_node;

#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_node->read_write_lock,
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

/* Sets the parent node in the tree node
 * Returns 1 if successful or -1 on error
 */
int libcdata_tree_node_set_parent_node(
     libcdata_tree_node_t *node,
     libcdata_tree_node_t *parent_node,
     libcerror_error_t **error )
{
	libcdata_internal_tree_node_t *internal_node = NULL;
	static char *function                        = "libcdata_tree_node_set_parent_node";

#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	libcdata_tree_node_t *backup_parent_node     = NULL;
#endif

	if( node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid node.",
		 function );

		return( -1 );
	}
	internal_node = (libcdata_internal_tree_node_t *) node;

#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_node->read_write_lock,
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
	backup_parent_node = internal_node->parent_node;
#endif

	internal_node->parent_node = parent_node;

#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_node->read_write_lock,
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
	internal_node->parent_node = backup_parent_node;

	return( -1 );
#endif
}

/* Retrieves the previous node from the tree node
 * Returns 1 if successful or -1 on error
 */
int libcdata_tree_node_get_previous_node(
     libcdata_tree_node_t *node,
     libcdata_tree_node_t **previous_node,
     libcerror_error_t **error )
{
	libcdata_internal_tree_node_t *internal_node = NULL;
	static char *function                        = "libcdata_tree_node_get_previous_node";

	if( node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid node.",
		 function );

		return( -1 );
	}
	internal_node = (libcdata_internal_tree_node_t *) node;

	if( previous_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid previous node.",
		 function );

		return( -1 );
	}
#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_node->read_write_lock,
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
	*previous_node = internal_node->previous_node;

#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_node->read_write_lock,
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

/* Sets the previous node in the tree node
 * Returns 1 if successful or -1 on error
 */
int libcdata_tree_node_set_previous_node(
     libcdata_tree_node_t *node,
     libcdata_tree_node_t *previous_node,
     libcerror_error_t **error )
{
	libcdata_internal_tree_node_t *internal_node = NULL;
	static char *function                        = "libcdata_tree_node_set_previous_node";

#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	libcdata_tree_node_t *backup_previous_node   = NULL;
#endif

	if( node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid node.",
		 function );

		return( -1 );
	}
	internal_node = (libcdata_internal_tree_node_t *) node;

#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_node->read_write_lock,
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
	backup_previous_node = internal_node->previous_node;
#endif

	internal_node->previous_node = previous_node;

#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_node->read_write_lock,
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
	internal_node->previous_node = backup_previous_node;

	return( -1 );
#endif
}

/* Retrieves the next node from the tree node
 * Returns 1 if successful or -1 on error
 */
int libcdata_tree_node_get_next_node(
     libcdata_tree_node_t *node,
     libcdata_tree_node_t **next_node,
     libcerror_error_t **error )
{
	libcdata_internal_tree_node_t *internal_node = NULL;
	static char *function                        = "libcdata_tree_node_get_next_node";

	if( node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid node.",
		 function );

		return( -1 );
	}
	internal_node = (libcdata_internal_tree_node_t *) node;

	if( next_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid next node.",
		 function );

		return( -1 );
	}
#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_node->read_write_lock,
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
	*next_node = internal_node->next_node;

#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_node->read_write_lock,
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

/* Sets the next node in the tree node
 * Returns 1 if successful or -1 on error
 */
int libcdata_tree_node_set_next_node(
     libcdata_tree_node_t *node,
     libcdata_tree_node_t *next_node,
     libcerror_error_t **error )
{
	libcdata_internal_tree_node_t *internal_node = NULL;
	static char *function                        = "libcdata_tree_node_set_next_node";

#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	libcdata_tree_node_t *backup_next_node       = NULL;
#endif

	if( node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid node.",
		 function );

		return( -1 );
	}
	internal_node = (libcdata_internal_tree_node_t *) node;

#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_node->read_write_lock,
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
	backup_next_node = internal_node->next_node ;
#endif

	internal_node->next_node = next_node;

#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_node->read_write_lock,
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
	internal_node->next_node = backup_next_node;

	return( -1 );
#endif
}

/* Retrieves the nodes from the tree node
 * Returns 1 if successful or -1 on error
 */
int libcdata_tree_node_get_nodes(
     libcdata_tree_node_t *node,
     libcdata_tree_node_t **parent_node,
     libcdata_tree_node_t **previous_node,
     libcdata_tree_node_t **next_node,
     libcerror_error_t **error )
{
	libcdata_internal_tree_node_t *internal_node = NULL;
	static char *function                        = "libcdata_tree_node_get_nodes";

	if( node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid node.",
		 function );

		return( -1 );
	}
	internal_node = (libcdata_internal_tree_node_t *) node;

	if( parent_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid parent node.",
		 function );

		return( -1 );
	}
	if( previous_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid previous node.",
		 function );

		return( -1 );
	}
	if( next_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid next node.",
		 function );

		return( -1 );
	}
#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_node->read_write_lock,
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
	*parent_node   = internal_node->parent_node;
	*previous_node = internal_node->previous_node;
	*next_node     = internal_node->next_node;

#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_node->read_write_lock,
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

/* Sets the nodes in the tree node
 * Returns 1 if successful or -1 on error
 */
int libcdata_tree_node_set_nodes(
     libcdata_tree_node_t *node,
     libcdata_tree_node_t *parent_node,
     libcdata_tree_node_t *previous_node,
     libcdata_tree_node_t *next_node,
     libcerror_error_t **error )
{
	libcdata_internal_tree_node_t *internal_node = NULL;
	static char *function                        = "libcdata_tree_node_set_nodes";

#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	libcdata_tree_node_t *backup_next_node       = NULL;
	libcdata_tree_node_t *backup_parent_node     = NULL;
	libcdata_tree_node_t *backup_previous_node   = NULL;
#endif

	if( node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid node.",
		 function );

		return( -1 );
	}
	internal_node = (libcdata_internal_tree_node_t *) node;

#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_node->read_write_lock,
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
	backup_parent_node   = internal_node->parent_node;
	backup_previous_node = internal_node->previous_node;
	backup_next_node     = internal_node->next_node;
#endif

	internal_node->parent_node   = parent_node;
	internal_node->previous_node = previous_node;
	internal_node->next_node     = next_node;

#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_node->read_write_lock,
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
	internal_node->parent_node   = backup_parent_node;
	internal_node->previous_node = backup_previous_node;
	internal_node->next_node     = backup_next_node;

	return( -1 );
#endif
}

/* Retrieves the first sub node from the tree node
 * Returns 1 if successful or -1 on error
 */
int libcdata_tree_node_get_first_sub_node(
     libcdata_tree_node_t *node,
     libcdata_tree_node_t **first_sub_node,
     libcerror_error_t **error )
{
	libcdata_internal_tree_node_t *internal_node = NULL;
	static char *function                        = "libcdata_tree_node_get_first_sub_node";

	if( node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid node.",
		 function );

		return( -1 );
	}
	internal_node = (libcdata_internal_tree_node_t *) node;

	if( first_sub_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid first sub node.",
		 function );

		return( -1 );
	}
#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_node->read_write_lock,
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
	*first_sub_node = internal_node->first_sub_node;

#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_node->read_write_lock,
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

/* Sets the first sub node in the tree node
 * Returns 1 if successful or -1 on error
 */
int libcdata_internal_tree_node_set_first_sub_node(
     libcdata_internal_tree_node_t *internal_node,
     libcdata_tree_node_t *first_sub_node,
     libcerror_error_t **error )
{
	libcdata_tree_node_t *backup_first_sub_node = NULL;
	libcdata_tree_node_t *backup_previous_node  = NULL;
	static char *function                       = "libcdata_internal_tree_node_set_first_sub_node";

	if( internal_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid node.",
		 function );

		return( -1 );
	}
	if( first_sub_node != NULL )
	{
		if( libcdata_tree_node_get_previous_node(
		     first_sub_node,
		     &backup_previous_node,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve previous node of sub node.",
			 function );

			return( -1 );
		}
	}
	backup_first_sub_node = internal_node->first_sub_node;

	if( first_sub_node != NULL )
	{
		if( libcdata_tree_node_set_previous_node(
		     first_sub_node,
		     internal_node->first_sub_node,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set previous node of sub node.",
			 function );

			goto on_error;
		}
	}
	if( internal_node->first_sub_node != NULL )
	{
		if( libcdata_tree_node_set_next_node(
		     internal_node->first_sub_node,
		     first_sub_node,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set next node of first sub node.",
			 function );

			goto on_error;
		}
	}
	internal_node->first_sub_node = first_sub_node;

	return( 1 );

on_error:
	if( first_sub_node != NULL )
	{
		libcdata_tree_node_set_previous_node(
		 first_sub_node,
		 backup_previous_node,
		 NULL );
	}
	if( backup_first_sub_node != NULL )
	{
		libcdata_tree_node_set_next_node(
		 backup_first_sub_node,
		 NULL,
		 NULL );
	}
	internal_node->first_sub_node = backup_first_sub_node;

	return( -1 );
}

/* Retrieves the last sub node from the tree node
 * Returns 1 if successful or -1 on error
 */
int libcdata_tree_node_get_last_sub_node(
     libcdata_tree_node_t *node,
     libcdata_tree_node_t **last_sub_node,
     libcerror_error_t **error )
{
	libcdata_internal_tree_node_t *internal_node = NULL;
	static char *function                        = "libcdata_tree_node_get_last_sub_node";

	if( node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid node.",
		 function );

		return( -1 );
	}
	internal_node = (libcdata_internal_tree_node_t *) node;

	if( last_sub_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid last sub node.",
		 function );

		return( -1 );
	}
#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_node->read_write_lock,
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
	*last_sub_node = internal_node->last_sub_node;

#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_node->read_write_lock,
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

/* Sets the last sub node in the tree node
 * Returns 1 if successful or -1 on error
 */
int libcdata_internal_tree_node_set_last_sub_node(
     libcdata_internal_tree_node_t *internal_node,
     libcdata_tree_node_t *last_sub_node,
     libcerror_error_t **error )
{
	libcdata_tree_node_t *backup_last_sub_node = NULL;
	libcdata_tree_node_t *backup_previous_node = NULL;
	static char *function                      = "libcdata_internal_tree_node_set_last_sub_node";

	if( internal_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid node.",
		 function );

		return( -1 );
	}
	if( last_sub_node != NULL )
	{
		if( libcdata_tree_node_get_previous_node(
		     last_sub_node,
		     &backup_previous_node,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve previous node of sub node.",
			 function );

			return( -1 );
		}
	}
	backup_last_sub_node = internal_node->last_sub_node;

	if( last_sub_node != NULL )
	{
		if( libcdata_tree_node_set_previous_node(
		     last_sub_node,
		     internal_node->last_sub_node,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set previous node of sub node.",
			 function );

			goto on_error;
		}
	}
	if( internal_node->last_sub_node != NULL )
	{
		if( libcdata_tree_node_set_next_node(
		     internal_node->last_sub_node,
		     last_sub_node,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set next node of last sub node.",
			 function );

			goto on_error;
		}
	}
	internal_node->last_sub_node = last_sub_node;

	return( 1 );

on_error:
	if( last_sub_node != NULL )
	{
		libcdata_tree_node_set_previous_node(
		 last_sub_node,
		 backup_previous_node,
		 NULL );
	}
	if( backup_last_sub_node != NULL )
	{
		libcdata_tree_node_set_next_node(
		 backup_last_sub_node,
		 NULL,
		 NULL );
	}
	internal_node->last_sub_node = backup_last_sub_node;

	return( -1 );
}

/* Retrieves the sub nodes from the tree node
 * Returns 1 if successful or -1 on error
 */
int libcdata_tree_node_get_sub_nodes(
     libcdata_tree_node_t *node,
     libcdata_tree_node_t **first_sub_node,
     libcdata_tree_node_t **last_sub_node,
     libcerror_error_t **error )
{
	libcdata_internal_tree_node_t *internal_node = NULL;
	static char *function                        = "libcdata_tree_node_sub_get_nodes";

	if( node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid node.",
		 function );

		return( -1 );
	}
	internal_node = (libcdata_internal_tree_node_t *) node;

	if( first_sub_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid first sub node.",
		 function );

		return( -1 );
	}
	if( last_sub_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid last sub node.",
		 function );

		return( -1 );
	}
#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_node->read_write_lock,
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
	*first_sub_node = internal_node->first_sub_node;
	*last_sub_node  = internal_node->last_sub_node;

#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_node->read_write_lock,
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

/* Sets the sub nodes in the tree node
 * Returns 1 if successful or -1 on error
 */
int libcdata_tree_node_set_sub_nodes(
     libcdata_tree_node_t *node,
     libcdata_tree_node_t *first_sub_node,
     libcdata_tree_node_t *last_sub_node,
     libcerror_error_t **error )
{
	libcdata_internal_tree_node_t *internal_node = NULL;
	static char *function                        = "libcdata_tree_node_set_sub_nodes";

#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	libcdata_tree_node_t *backup_first_sub_node  = NULL;
	libcdata_tree_node_t *backup_last_sub_node   = NULL;
#endif

	if( node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid node.",
		 function );

		return( -1 );
	}
	internal_node = (libcdata_internal_tree_node_t *) node;

#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_node->read_write_lock,
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
	backup_first_sub_node = internal_node->first_sub_node;
	backup_last_sub_node  = internal_node->last_sub_node;
#endif

	internal_node->first_sub_node = first_sub_node;
	internal_node->last_sub_node  = last_sub_node;

#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_node->read_write_lock,
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
	internal_node->first_sub_node = backup_first_sub_node;
	internal_node->last_sub_node  = backup_last_sub_node;

	return( -1 );
#endif
}

/* Appends a sub tree node to the node
 * Returns 1 if successful or -1 on error
 */
int libcdata_internal_tree_node_append_node(
     libcdata_internal_tree_node_t *internal_node,
     libcdata_tree_node_t *node_to_append,
     libcerror_error_t **error )
{
	libcdata_tree_node_t *to_append_next_node     = NULL;
	libcdata_tree_node_t *to_append_parent_node   = NULL;
	libcdata_tree_node_t *to_append_previous_node = NULL;
	static char *function                         = "libcdata_internal_tree_node_append_node";

	if( internal_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid node.",
		 function );

		return( -1 );
	}
	if( node_to_append == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid node to append.",
		 function );

		return( -1 );
	}
	if( libcdata_tree_node_get_nodes(
	     node_to_append,
	     &to_append_parent_node,
	     &to_append_previous_node,
	     &to_append_next_node,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve nodes of node to append.",
		 function );

		return( -1 );
	}
	if( ( to_append_parent_node != NULL )
	 || ( to_append_previous_node != NULL )
	 || ( to_append_next_node != NULL ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid node to append - node is already part of a tree.",
		 function );

		return( -1 );
	}
	if( internal_node->number_of_sub_nodes == 0 )
	{
		if( internal_node->first_sub_node != NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
			 "%s: corruption detected - first sub node already set.",
			 function );

			return( -1 );
		}
		if( internal_node->last_sub_node != NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
			 "%s: corruption detected - last sub node already set.",
			 function );

			return( -1 );
		}
		internal_node->first_sub_node = node_to_append;
		internal_node->last_sub_node  = node_to_append;
	}
	else
	{
		if( internal_node->first_sub_node == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: corruption detected - missing first sub node.",
			 function );

			return( -1 );
		}
		if( internal_node->last_sub_node == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: corruption detected - missing last sub node.",
			 function );

			return( -1 );
		}
		if( libcdata_tree_node_set_next_node(
		     internal_node->last_sub_node,
		     node_to_append,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set next node of last sub node.",
			 function );

			return( -1 );
		}
		if( libcdata_tree_node_set_previous_node(
		     node_to_append,
		     internal_node->last_sub_node,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set previous node of node to append.",
			 function );

			return( -1 );
		}
		internal_node->last_sub_node = node_to_append;
	}
	if( libcdata_tree_node_set_parent_node(
	     node_to_append,
	     (libcdata_tree_node_t *) internal_node,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set parent node of node to append.",
		 function );

		return( -1 );
	}
	internal_node->number_of_sub_nodes += 1;

	return( 1 );
}

/* Appends a tree node to the node
 * Returns 1 if successful or -1 on error
 */
int libcdata_tree_node_append_node(
     libcdata_tree_node_t *node,
     libcdata_tree_node_t *node_to_append,
     libcerror_error_t **error )
{
	libcdata_internal_tree_node_t *internal_node  = NULL;
	libcdata_tree_node_t *to_append_next_node     = NULL;
	libcdata_tree_node_t *to_append_parent_node   = NULL;
	libcdata_tree_node_t *to_append_previous_node = NULL;
	static char *function                         = "libcdata_tree_node_append_node";
	int result                                    = 1;

#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	libcdata_tree_node_t *backup_first_sub_node   = NULL;
	libcdata_tree_node_t *backup_last_sub_node    = NULL;
#endif

	if( node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid node.",
		 function );

		return( -1 );
	}
	internal_node = (libcdata_internal_tree_node_t *) node;

	if( internal_node->number_of_sub_nodes == 0 )
	{
		if( internal_node->first_sub_node != NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: corruption detected - first sub node already set.",
			 function );

			return( -1 );
		}
		if( internal_node->last_sub_node != NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: corruption detected - last sub node already set.",
			 function );

			return( -1 );
		}
	}
	else
	{
		if( internal_node->first_sub_node == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: corruption detected - missing first sub node.",
			 function );

			return( -1 );
		}
		if( internal_node->last_sub_node == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: corruption detected - missing last sub node.",
			 function );

			return( -1 );
		}
	}
	if( libcdata_tree_node_get_nodes(
	     node_to_append,
	     &to_append_parent_node,
	     &to_append_previous_node,
	     &to_append_next_node,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve nodes of node to append.",
		 function );

		return( -1 );
	}
	if( ( to_append_parent_node != NULL )
	 || ( to_append_previous_node != NULL )
	 || ( to_append_next_node != NULL ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid node to append - node is already part of a tree.",
		 function );

		return( -1 );
	}
#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_node->read_write_lock,
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
	backup_first_sub_node = internal_node->first_sub_node;
	backup_last_sub_node  = internal_node->last_sub_node;
#endif

	result = libcdata_tree_node_set_parent_node(
	          node_to_append,
	          node,
	          error );

	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set parent node of node to append.",
		 function );

		result = -1;
	}
	if( result == 1 )
	{
		result = libcdata_tree_node_set_previous_node(
		          node_to_append,
		          internal_node->last_sub_node,
		          error );

		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set previous node of node to append.",
			 function );

			libcdata_tree_node_set_parent_node(
			 node_to_append,
			 NULL,
			 NULL );

			result = -1;
		}
	}
	if( result == 1 )
	{
		if( internal_node->last_sub_node != NULL )
		{
			result = libcdata_tree_node_set_next_node(
			          internal_node->last_sub_node,
			          node_to_append,
			          error );

			if( result != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set next node of last sub node.",
				 function );

				libcdata_tree_node_set_parent_node(
				 node_to_append,
				 NULL,
				 NULL );

				libcdata_tree_node_set_previous_node(
				 node_to_append,
				 NULL,
				 NULL );

				result = -1;
			}
		}
	}
	if( result == 1 )
	{
		if( internal_node->first_sub_node == NULL )
		{
			internal_node->first_sub_node = node_to_append;
		}
		internal_node->last_sub_node = node_to_append;

		internal_node->number_of_sub_nodes += 1;
	}
#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_node->read_write_lock,
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
	libcdata_tree_node_set_parent_node(
	 node_to_append,
	 NULL,
	 NULL );

	libcdata_tree_node_set_previous_node(
	 node_to_append,
	 NULL,
	 NULL );

	internal_node->first_sub_node = backup_first_sub_node;
	internal_node->last_sub_node  = backup_last_sub_node;

	internal_node->number_of_sub_nodes -= 1;

	return( -1 );
#endif
}

/* Appends a value to the node
 * Creates a new sub tree node
 * Returns 1 if successful or -1 on error
 */
int libcdata_tree_node_append_value(
     libcdata_tree_node_t *node,
     intptr_t *value,
     libcerror_error_t **error )
{
	libcdata_tree_node_t *sub_node = NULL;
	static char *function          = "libcdata_tree_node_append_value";

	if( node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid node.",
		 function );

		return( -1 );
	}
	if( libcdata_tree_node_initialize(
	     &sub_node,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create sub node.",
		 function );

		goto on_error;
	}
	if( libcdata_tree_node_set_value(
	     sub_node,
	     value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to set value in sub node.",
		 function );

		goto on_error;
	}
	if( libcdata_tree_node_append_node(
	     node,
	     sub_node,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append sub node to node.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( sub_node != NULL )
	{
		libcdata_tree_node_free(
		 &sub_node,
		 NULL,
		 NULL );
	}
	return( -1 );
}


/* Retrieves the node which the sub node should be inserted before
 *
 * Uses the value_compare_function to determine the order of the entries
 * The value_compare_function should return LIBCDATA_COMPARE_LESS,
 * LIBCDATA_COMPARE_EQUAL, LIBCDATA_COMPARE_GREATER if successful or -1 on error
 *
 * Duplicate entries are allowed by default and inserted after the last duplicate value.
 * Only allowing unique entries can be enforced by setting the flag LIBCDATA_INSERT_FLAG_UNIQUE_ENTRIES
 *
 * On return sub_node will be set to NULL if the sub node should be inserted at the end of the list.
 *
 * Returns 1 if successful, 0 if a sub node containing the value already exists or -1 on error
 */
int libcdata_internal_tree_node_insert_node_find_sub_node(
     libcdata_internal_tree_node_t *internal_node,
     intptr_t *value_to_insert,
     int (*value_compare_function)(
            intptr_t *first_value,
            intptr_t *second_value,
            libcerror_error_t **error ),
     uint8_t insert_flags,
     int *sub_node_index,
     libcdata_tree_node_t **sub_node,
     libcerror_error_t **error )
{
	libcdata_tree_node_t *sub_tree_node = NULL;
	intptr_t *sub_node_value            = NULL;
	static char *function               = "libcdata_internal_tree_node_insert_node_find_sub_node";
	int compare_result                  = 0;
	int result                          = 1;
	int safe_sub_node_index             = 0;

	if( internal_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid node.",
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
	if( sub_node_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sub node index.",
		 function );

		return( -1 );
	}
	if( sub_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sub node.",
		 function );

		return( -1 );
	}
	sub_tree_node = internal_node->first_sub_node;

	for( safe_sub_node_index = 0;
	     safe_sub_node_index < internal_node->number_of_sub_nodes;
	     safe_sub_node_index++ )
	{
		if( libcdata_tree_node_get_value(
		     sub_tree_node,
		     &sub_node_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value of sub node: %d.",
			 function,
			 safe_sub_node_index );

			return( -1 );
		}
		compare_result = value_compare_function(
		                  value_to_insert,
		                  sub_node_value,
		                  error );

		if( compare_result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to compare sub node: %d.",
			 function,
			 safe_sub_node_index );

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
			 result );

			return( -1 );
		}
		if( libcdata_tree_node_get_next_node(
		     sub_tree_node,
		     &sub_tree_node,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve next node of sub node: %d.",
			 function,
			 safe_sub_node_index );

			return( -1 );
		}
	}
	if( ( compare_result == LIBCDATA_COMPARE_EQUAL )
	 || ( compare_result == LIBCDATA_COMPARE_LESS ) )
	{
		*sub_node_index = safe_sub_node_index;
		*sub_node       = sub_tree_node;
	}
	else
	{
		*sub_node_index = internal_node->number_of_sub_nodes;
		*sub_node       = NULL;
	}
	return( result );
}

/* Inserts the node before the sub node
 * If sub_node is NULL the node is inserted before or as the first sub node in the list
 * Returns 1 if successful, or -1 on error
 */
int libcdata_internal_tree_node_insert_node_before_sub_node(
     libcdata_internal_tree_node_t *internal_node,
     libcdata_tree_node_t *sub_node,
     libcdata_tree_node_t *node_to_insert,
     libcerror_error_t **error )
{
	libcdata_tree_node_t *backup_first_sub_node = NULL;
	libcdata_tree_node_t *backup_last_sub_node  = NULL;
	libcdata_tree_node_t *previous_node         = NULL;
	static char *function                       = "libcdata_internal_tree_node_insert_node_before_sub_node";

	if( internal_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid node.",
		 function );

		return( -1 );
	}
	if( node_to_insert == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid node to insert.",
		 function );

		return( -1 );
	}
	backup_first_sub_node = internal_node->first_sub_node;
	backup_last_sub_node  = internal_node->last_sub_node;

	if( sub_node != NULL )
	{
		if( libcdata_tree_node_get_previous_node(
		     sub_node,
		     &previous_node,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve previous node from sub node.",
			 function );

			return( -1 );
		}
	}
	if( internal_node->number_of_sub_nodes == 0 )
	{
		internal_node->first_sub_node = node_to_insert;
		internal_node->last_sub_node  = node_to_insert;
	}
	else if( sub_node == NULL )
	{
		if( libcdata_internal_tree_node_set_last_sub_node(
		     internal_node,
		     node_to_insert,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set last sub node.",
			 function );

			goto on_error;
		}
	}
	else
	{
		if( libcdata_tree_node_set_nodes(
		     node_to_insert,
		     (libcdata_tree_node_t *) internal_node,
		     previous_node,
		     sub_node,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set parent, previous and next node of node to insert.",
			 function );

			goto on_error;
		}
		if( internal_node->first_sub_node == sub_node )
		{
			internal_node->first_sub_node = node_to_insert;
		}
		else
		{
			if( libcdata_tree_node_set_next_node(
			     previous_node,
			     node_to_insert,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set next node of previous node.",
				 function );

				goto on_error;
			}
		}
		if( libcdata_tree_node_set_previous_node(
		     sub_node,
		     node_to_insert,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set previous node of sub node.",
			 function );

			goto on_error;
		}
	}
	if( libcdata_tree_node_set_parent_node(
	     node_to_insert,
	     (libcdata_tree_node_t *) internal_node,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set parent node of node to insert.",
		 function );

		goto on_error;
	}
	internal_node->number_of_sub_nodes += 1;

	return( 1 );

on_error:
	if( node_to_insert != NULL )
	{
		libcdata_tree_node_set_nodes(
		 node_to_insert,
		 NULL,
		 NULL,
		 NULL,
		 NULL );
	}
	if( previous_node != NULL )
	{
		libcdata_tree_node_set_next_node(
		 previous_node,
		 sub_node,
		 NULL );
	}
	if( sub_node != NULL )
	{
		libcdata_tree_node_set_previous_node(
		 sub_node,
		 previous_node,
		 NULL );
	}
	internal_node->first_sub_node = backup_first_sub_node;
	internal_node->last_sub_node  = backup_last_sub_node;

	return( -1 );
}

/* Inserts a sub node in the node
 *
 * Uses the value_compare_function to determine the order of the entries
 * The value_compare_function should return LIBCDATA_COMPARE_LESS,
 * LIBCDATA_COMPARE_EQUAL, LIBCDATA_COMPARE_GREATER if successful or -1 on error
 *
 * Duplicate entries are allowed by default and inserted after the last duplicate value.
 * Only allowing unique entries can be enforced by setting the flag LIBCDATA_INSERT_FLAG_UNIQUE_ENTRIES
 *
 * Returns 1 if successful, 0 if the node already exists or -1 on error
 */
int libcdata_tree_node_insert_node(
     libcdata_tree_node_t *node,
     libcdata_tree_node_t *node_to_insert,
     int (*value_compare_function)(
            intptr_t *first_value,
            intptr_t *second_value,
            libcerror_error_t **error ),
     uint8_t insert_flags,
     libcerror_error_t **error )
{
	libcdata_internal_tree_node_t *internal_node = NULL;
	libcdata_tree_node_t *next_node              = NULL;
	libcdata_tree_node_t *parent_node            = NULL;
	libcdata_tree_node_t *previous_node          = NULL;
	libcdata_tree_node_t *sub_node               = NULL;
	intptr_t *value_to_insert                    = NULL;
	static char *function                        = "libcdata_tree_node_insert_node";
	int result                                   = 1;
	int sub_node_index                           = 0;

#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	libcdata_tree_node_t *backup_first_sub_node  = NULL;
	libcdata_tree_node_t *backup_last_sub_node   = NULL;
#endif

	if( node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid node.",
		 function );

		return( -1 );
	}
	internal_node = (libcdata_internal_tree_node_t *) node;

	if( internal_node->number_of_sub_nodes == 0 )
	{
		if( internal_node->first_sub_node != NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: corruption detected - first sub node already set.",
			 function );

			return( -1 );
		}
		if( internal_node->last_sub_node != NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: corruption detected - last sub node already set.",
			 function );

			return( -1 );
		}
	}
	else
	{
		if( internal_node->first_sub_node == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: corruption detected - missing first sub node.",
			 function );

			return( -1 );
		}
		if( internal_node->last_sub_node == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: corruption detected - missing last sub node.",
			 function );

			return( -1 );
		}
	}
	if( node_to_insert == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid node to insert.",
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
	if( libcdata_tree_node_get_nodes(
	     node_to_insert,
	     &parent_node,
	     &previous_node,
	     &next_node,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve nodes of node to insert.",
		 function );

		return( -1 );
	}
	if( ( parent_node != NULL )
	 || ( previous_node != NULL )
	 || ( next_node != NULL ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid node to insert - node is already part of a tree.",
		 function );

		return( -1 );
	}
	if( libcdata_tree_node_get_value(
	     node_to_insert,
	     &value_to_insert,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value from node to insert.",
		 function );

		return( -1 );
	}
#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_node->read_write_lock,
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
	backup_first_sub_node = internal_node->first_sub_node;
	backup_last_sub_node  = internal_node->last_sub_node;
#endif

	result = libcdata_internal_tree_node_insert_node_find_sub_node(
	          internal_node,
	          value_to_insert,
	          value_compare_function,
	          insert_flags,
	          &sub_node_index,
	          &sub_node,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to find sub node in tree node.",
		 function );

		result = -1;
	}
	else if( result == 1 )
	{
		if( sub_node != NULL )
		{
			if( libcdata_tree_node_get_nodes(
			     sub_node,
			     &parent_node,
			     &previous_node,
			     &next_node,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve nodes of sub node: %d.",
				 function,
				 sub_node_index );

				result = -1;
			}
		}
		if( result == 1 )
		{
			if( libcdata_internal_tree_node_insert_node_before_sub_node(
			     internal_node,
			     sub_node,
			     node_to_insert,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
				 "%s: unable to insert node before tree sub node.",
				 function );

				result = -1;
			}
		}
	}
#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_node->read_write_lock,
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
		if( node_to_insert != NULL )
		{
			libcdata_tree_node_set_nodes(
			 node_to_insert,
			 NULL,
			 NULL,
			 NULL,
			 NULL );
		}
		if( previous_node != NULL )
		{
			libcdata_tree_node_set_next_node(
			 previous_node,
			 sub_node,
			 NULL );
		}
		if( sub_node != NULL )
		{
			libcdata_tree_node_set_previous_node(
			 sub_node,
			 previous_node,
			 NULL );
		}
		internal_node->first_sub_node = backup_first_sub_node;
		internal_node->last_sub_node  = backup_last_sub_node;

		internal_node->number_of_sub_nodes -= 1;
	}
	return( -1 );
#endif
}

/* Inserts a value in the node
 *
 * Creates a new sub tree node
 *
 * Uses the value_compare_function to determine the order of the entries
 * The value_compare_function should return LIBCDATA_COMPARE_LESS,
 * LIBCDATA_COMPARE_EQUAL, LIBCDATA_COMPARE_GREATER if successful or -1 on error
 *
 * Duplicate entries are allowed by default and inserted after the last duplicate value.
 * Only allowing unique entries can be enforced by setting the flag LIBCDATA_INSERT_FLAG_UNIQUE_ENTRIES
 *
 * Returns 1 if successful, 0 if the node already exists or -1 on error
 */
int libcdata_tree_node_insert_value(
     libcdata_tree_node_t *node,
     intptr_t *value,
     int (*value_compare_function)(
            intptr_t *first_value,
            intptr_t *second_value,
            libcerror_error_t **error ),
     uint8_t insert_flags,
     libcerror_error_t **error )
{
	libcdata_tree_node_t *sub_node = NULL;
	static char *function          = "libcdata_tree_node_insert_value";
	int result                     = 0;

	if( node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid node.",
		 function );

		return( -1 );
	}
	if( libcdata_tree_node_initialize(
	     &sub_node,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create sub node.",
		 function );

		goto on_error;
	}
	if( libcdata_tree_node_set_value(
	     sub_node,
	     value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to set value in sub node.",
		 function );

		goto on_error;
	}
	result = libcdata_tree_node_insert_node(
	          node,
	          sub_node,
	          value_compare_function,
	          insert_flags,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to insert node.",
		 function );

		goto on_error;
	}
	else if( result == 0 )
	{
		if( libcdata_tree_node_free(
		     &sub_node,
		     NULL,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free sub node.",
			 function );

			goto on_error;
		}
	}
	return( result );

on_error:
	if( sub_node != NULL )
	{
		libcdata_tree_node_free(
		 &sub_node,
		 NULL,
		 NULL );
	}
	return( -1 );
}

/* Replaces a tree node with another tree node
 * Returns 1 if successful or -1 on error
 */
int libcdata_tree_node_replace_node(
     libcdata_tree_node_t *node,
     libcdata_tree_node_t *replacement_node,
     libcerror_error_t **error )
{
	libcdata_internal_tree_node_t *internal_node       = NULL;
	libcdata_tree_node_t *backup_parent_first_sub_node = NULL;
	libcdata_tree_node_t *backup_parent_last_sub_node  = NULL;
	libcdata_tree_node_t *next_node                    = NULL;
	libcdata_tree_node_t *parent_first_sub_node        = NULL;
	libcdata_tree_node_t *parent_last_sub_node         = NULL;
	libcdata_tree_node_t *parent_node                  = NULL;
	libcdata_tree_node_t *previous_node                = NULL;
	libcdata_tree_node_t *replacement_next_node        = NULL;
	libcdata_tree_node_t *replacement_parent_node      = NULL;
	libcdata_tree_node_t *replacement_previous_node    = NULL;
	static char *function                              = "libcdata_tree_node_replace_node";
	int result                                         = 1;

	if( node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid node.",
		 function );

		return( -1 );
	}
	internal_node = (libcdata_internal_tree_node_t *) node;

	if( replacement_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid replacement node.",
		 function );

		return( -1 );
	}
	if( replacement_node == node )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: cannot replace node with itself.",
		 function );

		return( -1 );
	}
	if( libcdata_tree_node_get_nodes(
	     replacement_node,
	     &replacement_parent_node,
	     &replacement_previous_node,
	     &replacement_next_node,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve nodes of replacement node.",
		 function );

		return( -1 );
	}
	if( ( replacement_parent_node != NULL )
	 || ( replacement_previous_node != NULL )
	 || ( replacement_next_node != NULL ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid replacement node - already part of a tree.",
		 function );

		return( -1 );
	}
#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_node->read_write_lock,
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
	parent_node   = internal_node->parent_node;
	previous_node = internal_node->previous_node;
	next_node     = internal_node->next_node;

	if( parent_node != NULL )
	{
		if( libcdata_tree_node_get_sub_nodes(
		     parent_node,
		     &parent_first_sub_node,
		     &parent_last_sub_node,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve sub nodes of parent node.",
			 function );

			result = -1;
		}
	}
	if( result == 1 )
	{
		if( libcdata_tree_node_set_nodes(
		     replacement_node,
		     parent_node,
		     previous_node,
		     next_node,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set nodes of replacement node.",
			 function );

			result = -1;
		}
	}
	if( result == 1 )
	{
		if( parent_node != NULL )
		{
			backup_parent_first_sub_node = parent_first_sub_node;
			backup_parent_last_sub_node  = parent_last_sub_node;

			if( parent_first_sub_node == node )
			{
				parent_first_sub_node = replacement_node;
			}
			if( parent_last_sub_node == node )
			{
				parent_last_sub_node = replacement_node;
			}
			if( libcdata_tree_node_set_sub_nodes(
			     parent_node,
			     parent_first_sub_node,
			     parent_last_sub_node,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set sub nodes of parent node.",
				 function );

				libcdata_tree_node_set_nodes(
				 replacement_node,
				 NULL,
				 NULL,
				 NULL,
				 NULL );

				result = -1;
			}
		}
	}
	if( result == 1 )
	{
		if( previous_node != NULL )
		{
			if( libcdata_tree_node_set_next_node(
			     previous_node,
			     replacement_node,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set next node of previous node.",
				 function );

				libcdata_tree_node_set_nodes(
				 replacement_node,
				 NULL,
				 NULL,
				 NULL,
				 NULL );

				if( parent_node != NULL )
				{
					libcdata_tree_node_set_sub_nodes(
					 parent_node,
					 backup_parent_first_sub_node,
					 backup_parent_last_sub_node,
					 NULL );
				}
				result = -1;
			}
		}
	}
	if( result == 1 )
	{
		if( next_node != NULL )
		{
			if( libcdata_tree_node_set_previous_node(
			     next_node,
			     replacement_node,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set previous node of next node.",
				 function );

				libcdata_tree_node_set_nodes(
				 replacement_node,
				 NULL,
				 NULL,
				 NULL,
				 NULL );

				if( parent_node != NULL )
				{
					libcdata_tree_node_set_sub_nodes(
					 parent_node,
					 backup_parent_first_sub_node,
					 backup_parent_last_sub_node,
					 NULL );
				}
				if( previous_node != NULL )
				{
					libcdata_tree_node_set_next_node(
					 previous_node,
					 node,
					 NULL );
				}
				result = -1;
			}
		}
	}
	if( result == 1 )
	{
		internal_node->parent_node   = NULL;
		internal_node->previous_node = NULL;
		internal_node->next_node     = NULL;
	}
#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_node->read_write_lock,
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
		libcdata_tree_node_set_nodes(
		 replacement_node,
		 NULL,
		 NULL,
		 NULL,
		 NULL );

		if( parent_node != NULL )
		{
			libcdata_tree_node_set_sub_nodes(
			 parent_node,
			 backup_parent_first_sub_node,
			 backup_parent_last_sub_node,
			 NULL );
		}
		if( previous_node != NULL )
		{
			libcdata_tree_node_set_next_node(
			 previous_node,
			 node,
			 NULL );
		}
		if( next_node != NULL )
		{
			libcdata_tree_node_set_previous_node(
			 next_node,
			 node,
			 NULL );
		}
		internal_node->parent_node   = parent_node;
		internal_node->previous_node = previous_node;
		internal_node->next_node     = next_node;
	}
	return( -1 );
#endif
}

/* Removes a sub tree node from the tree node
 * Returns 1 if successful or -1 on error
 */
int libcdata_tree_node_remove_node(
     libcdata_tree_node_t *node,
     libcdata_tree_node_t *sub_node_to_remove,
     libcerror_error_t **error )
{
	libcdata_internal_tree_node_t *internal_node = NULL;
	libcdata_tree_node_t *next_node              = NULL;
	libcdata_tree_node_t *parent_node            = NULL;
	libcdata_tree_node_t *previous_node          = NULL;
	static char *function                        = "libcdata_tree_node_remove_node";
	int result                                   = 1;

#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	libcdata_tree_node_t *backup_first_sub_node  = NULL;
	libcdata_tree_node_t *backup_last_sub_node   = NULL;
#endif

	if( node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid node.",
		 function );

		return( -1 );
	}
	internal_node = (libcdata_internal_tree_node_t *) node;

	if( internal_node->number_of_sub_nodes == 0 )
	{
		if( internal_node->first_sub_node != NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: corruption detected - first sub node already set.",
			 function );

			return( -1 );
		}
		if( internal_node->last_sub_node != NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: corruption detected - last sub node already set.",
			 function );

			return( -1 );
		}
	}
	else
	{
		if( internal_node->first_sub_node == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: corruption detected - missing first sub node.",
			 function );

			return( -1 );
		}
		if( internal_node->last_sub_node == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: corruption detected - missing last sub node.",
			 function );

			return( -1 );
		}
	}
	if( sub_node_to_remove == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sub node to remove.",
		 function );

		return( -1 );
	}
	if( internal_node->number_of_sub_nodes == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid node - missing number of sub nodes.",
		 function );

		return( -1 );
	}
	if( internal_node->first_sub_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid node - missing first sub node.",
		 function );

		return( -1 );
	}
	if( internal_node->last_sub_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid node - missing last sub node.",
		 function );

		return( -1 );
	}
	if( libcdata_tree_node_get_nodes(
	     sub_node_to_remove,
	     &parent_node,
	     &previous_node,
	     &next_node,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve nodes of node to remove.",
		 function );

		return( -1 );
	}
	if( parent_node != node )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid node to remove - parent node mismatch.",
		 function );

		return( -1 );
	}
#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_node->read_write_lock,
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
	backup_first_sub_node = internal_node->first_sub_node;
	backup_last_sub_node  = internal_node->last_sub_node;
#endif
	result = libcdata_tree_node_set_nodes(
	          sub_node_to_remove,
	          NULL,
	          NULL,
	          NULL,
	          error );

	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set nodes of node to remove.",
		 function );

		result = -1;
	}
	if( result == 1 )
	{
		if( next_node != NULL )
		{
			if( libcdata_tree_node_set_previous_node(
			     next_node,
			     previous_node,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set previous node of next node.",
				 function );

				libcdata_tree_node_set_nodes(
				 sub_node_to_remove,
				 parent_node,
				 previous_node,
				 next_node,
				 NULL );

				result = -1;
			}
		}
	}
	if( result == 1 )
	{
		if( previous_node != NULL )
		{
			if( libcdata_tree_node_set_next_node(
			     previous_node,
			     next_node,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set next node of previous node.",
				 function );

				libcdata_tree_node_set_nodes(
				 sub_node_to_remove,
				 parent_node,
				 previous_node,
				 next_node,
				 NULL );

				if( next_node != NULL )
				{
					libcdata_tree_node_set_previous_node(
					 next_node,
					 sub_node_to_remove,
					 NULL );
				}
				result = -1;
			}
		}
	}
	if( result == 1 )
	{
		if( internal_node->first_sub_node == sub_node_to_remove )
		{
			internal_node->first_sub_node = next_node;
		}
		if( internal_node->last_sub_node == sub_node_to_remove )
		{
			internal_node->last_sub_node = previous_node;
		}
		internal_node->number_of_sub_nodes -= 1;
	}
#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_node->read_write_lock,
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
	if( result == 1 )
	{
		libcdata_tree_node_set_nodes(
		 sub_node_to_remove,
		 parent_node,
		 previous_node,
		 next_node,
		 NULL );

		if( next_node != NULL )
		{
			libcdata_tree_node_set_previous_node(
			 next_node,
			 sub_node_to_remove,
			 NULL );
		}
		if( previous_node != NULL )
		{
			libcdata_tree_node_set_next_node(
			 previous_node,
			 sub_node_to_remove,
			 NULL );
		}
		internal_node->first_sub_node = backup_first_sub_node;
		internal_node->last_sub_node  = backup_last_sub_node;

		internal_node->number_of_sub_nodes += 1;
	}
	return( -1 );
#endif
}

/* Retrieves the number of sub nodes in the tree node
 * Returns 1 if successful or -1 on error
 */
int libcdata_tree_node_get_number_of_sub_nodes(
     libcdata_tree_node_t *node,
     int *number_of_sub_nodes,
     libcerror_error_t **error )
{
	libcdata_internal_tree_node_t *internal_node = NULL;
	static char *function                        = "libcdata_tree_node_get_number_of_sub_nodes";

	if( node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid node.",
		 function );

		return( -1 );
	}
	internal_node = (libcdata_internal_tree_node_t *) node;

	if( number_of_sub_nodes == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid number of sub nodes.",
		 function );

		return( -1 );
	}
#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_node->read_write_lock,
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
	*number_of_sub_nodes = internal_node->number_of_sub_nodes;

#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_node->read_write_lock,
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

/* Retrieves a specific sub node from the tree node
 * Returns 1 if successful or -1 on error
 */
int libcdata_tree_node_get_sub_node_by_index(
     libcdata_tree_node_t *node,
     int sub_node_index,
     libcdata_tree_node_t **sub_node,
     libcerror_error_t **error )
{
	libcdata_internal_tree_node_t *internal_node = NULL;
	libcdata_tree_node_t *safe_sub_node          = NULL;
	static char *function                        = "libcdata_tree_node_get_sub_node_by_index";
	int result                                   = -1;
	int sub_node_iterator                        = 0;

	if( node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid node.",
		 function );

		return( -1 );
	}
	internal_node = (libcdata_internal_tree_node_t *) node;

	if( ( sub_node_index < 0 )
	 || ( sub_node_index >= internal_node->number_of_sub_nodes ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid sub node index value out of bounds.",
		 function );

		return( -1 );
	}
	if( sub_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sub node.",
		 function );

		return( -1 );
	}
#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_node->read_write_lock,
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
	/* Check if the sub nodes should be searched front to back
	 * or back to front
	 */
	if( sub_node_index < ( internal_node->number_of_sub_nodes / 2 ) )
	{
		safe_sub_node = internal_node->first_sub_node;

		for( sub_node_iterator = 0;
		     sub_node_iterator < internal_node->number_of_sub_nodes;
		     sub_node_iterator++ )
		{
			if( sub_node_iterator == sub_node_index )
			{
				result = 1;

				break;
			}
			if( libcdata_tree_node_get_next_node(
			     safe_sub_node,
			     &safe_sub_node,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve next node of sub node: %d.",
				 function,
				 sub_node_iterator );

				goto on_error;
			}
		}
	}
	else
	{
		safe_sub_node = internal_node->last_sub_node;

		for( sub_node_iterator = ( internal_node->number_of_sub_nodes - 1 );
		     sub_node_iterator >= 0;
		     sub_node_iterator-- )
		{
			if( sub_node_iterator == sub_node_index )
			{
				result = 1;

				break;
			}
			if( libcdata_tree_node_get_previous_node(
			     safe_sub_node,
			     &safe_sub_node,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve previous node of sub node: %d.",
				 function,
				 sub_node_iterator );

				goto on_error;
			}
		}
	}
	*sub_node = safe_sub_node;

#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_node->read_write_lock,
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
	 internal_node->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

/* Retrieves a list of all the leaf nodes
 * Returns 1 if successful or -1 on error
 */
int libcdata_tree_node_get_leaf_node_list(
     libcdata_tree_node_t *node,
     libcdata_list_t **leaf_node_list,
     libcerror_error_t **error )
{
	libcdata_internal_tree_node_t *internal_node = NULL;
	libcdata_tree_node_t *sub_node               = NULL;
	static char *function                        = "libcdata_tree_node_get_leaf_node_list";
	int leaf_node_list_created_in_node           = 0;
	int sub_node_index                           = 0;

	if( node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid node.",
		 function );

		return( -1 );
	}
	internal_node = (libcdata_internal_tree_node_t *) node;

	if( leaf_node_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid leaf node list.",
		 function );

		return( -1 );
	}
#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_node->read_write_lock,
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
	if( *leaf_node_list == NULL )
	{
		if( libcdata_list_initialize(
		     leaf_node_list,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create leaf node list.",
			 function );

			goto on_error;
		}
		leaf_node_list_created_in_node = 1;
	}
	/* Traverse the sub nodes
	 */
	if( internal_node->number_of_sub_nodes == 0 )
	{
		if( internal_node->value == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid node - missing value.",
			 function );

			goto on_error;
		}
		if( libcdata_list_append_value(
		     *leaf_node_list,
		     internal_node->value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append tree node to leaf node list.",
			 function );

			goto on_error;
		}
	}
	else
	{
		sub_node = internal_node->first_sub_node;

		for( sub_node_index = 0;
		     sub_node_index < internal_node->number_of_sub_nodes;
		     sub_node_index++ )
		{
			if( sub_node == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: corruption detected for sub node: %d.",
				 function,
				 sub_node_index );

				goto on_error;
			}
			if( libcdata_tree_node_get_leaf_node_list(
			     sub_node,
			     leaf_node_list,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: unable to traverse sub node: %d.",
				 function,
				 sub_node_index );

				goto on_error;
			}
			if( libcdata_tree_node_get_next_node(
			     sub_node,
			     &sub_node,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve next node of sub node: %d.",
				 function,
				 sub_node_index );

				goto on_error;
			}
		}
	}
#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_node->read_write_lock,
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
	if( leaf_node_list_created_in_node != 0 )
	{
		if( *leaf_node_list == NULL )
		{
			libcdata_list_initialize(
			 leaf_node_list,
			 NULL );
		}
	}
#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBCDATA )
	libcthreads_read_write_lock_release_for_read(
	 internal_node->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

