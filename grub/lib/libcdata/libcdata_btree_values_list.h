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

#if !defined( _LIBCDATA_BTREE_VALUES_LIST_H )
#define _LIBCDATA_BTREE_VALUES_LIST_H

#include <libyal_wrap.h>

#include "libcdata_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

int libcdata_btree_values_list_free(
     libcdata_list_t **values_list,
     libcerror_error_t **error );

int libcdata_btree_values_list_remove_element(
     libcdata_list_t *values_list,
     libcdata_list_element_t **values_list_element,
     libcerror_error_t **error );

int libcdata_btree_values_list_replace_element_with_previous(
     libcdata_list_t *values_list,
     libcdata_list_element_t **values_list_element,
     intptr_t **replacement_value,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBCDATA_BTREE_VALUES_LIST_H ) */

