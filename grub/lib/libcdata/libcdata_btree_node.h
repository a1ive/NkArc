/*
 * Balanced tree node functions
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

#if !defined( _LIBCDATA_BTREE_NODE_H )
#define _LIBCDATA_BTREE_NODE_H

#include <libyal_wrap.h>

#include "libcdata_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

int libcdata_btree_node_get_sub_node_by_value(
     libcdata_tree_node_t *node,
     intptr_t *value,
     int (*value_compare_function)(
            intptr_t *first_value,
            intptr_t *second_value,
            libcerror_error_t **error ),
     libcdata_tree_node_t **sub_node,
     libcdata_list_element_t **values_list_element,
     libcerror_error_t **error );

int libcdata_btree_node_get_upper_node_by_value(
     libcdata_tree_node_t *node,
     intptr_t *value,
     int (*value_compare_function)(
            intptr_t *first_value,
            intptr_t *second_value,
            libcerror_error_t **error ),
     libcdata_tree_node_t **upper_node,
     libcdata_list_element_t **values_list_element,
     libcerror_error_t **error );

int libcdata_btree_node_append_value(
     libcdata_tree_node_t *node,
     intptr_t *value,
     libcerror_error_t **error );

int libcdata_btree_node_insert_value(
     libcdata_tree_node_t *node,
     intptr_t *value,
     int (*value_compare_function)(
            intptr_t *first_value,
            intptr_t *second_value,
            libcerror_error_t **error ),
     libcerror_error_t **error );

int libcdata_btree_node_replace_value(
     libcdata_tree_node_t *node,
     intptr_t *value,
     intptr_t *replacement_value,
     libcerror_error_t **error );

int libcdata_btree_node_flatten_node(
     libcdata_tree_node_t **node,
     libcerror_error_t **error );

int libcdata_btree_node_remove_sub_node(
     libcdata_tree_node_t *node,
     libcdata_tree_node_t **sub_node,
     libcerror_error_t **error );

int libcdata_btree_node_remove_value(
     libcdata_tree_node_t *node,
     intptr_t *value,
     intptr_t *replacement_value,
     libcerror_error_t **error );

int libcdata_btree_node_split(
     libcdata_tree_node_t *node,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBCDATA_BTREE_NODE_H ) */

