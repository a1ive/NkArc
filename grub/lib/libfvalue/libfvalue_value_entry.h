/*
 * Value entry functions
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

#if !defined( _LIBFVALUE_VALUE_ENTRY_H )
#define _LIBFVALUE_VALUE_ENTRY_H

#include <libyal_wrap.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfvalue_value_entry libfvalue_value_entry_t;

struct libfvalue_value_entry
{
	/* The offset
	 */
	size_t offset;

	/* The size
	 */
	size_t size;
};

int libfvalue_value_entry_initialize(
     libfvalue_value_entry_t **value_entry,
     libcerror_error_t **error );

int libfvalue_value_entry_free(
     libfvalue_value_entry_t **value_entry,
     libcerror_error_t **error );

int libfvalue_value_entry_clone(
     libfvalue_value_entry_t **destination_value_entry,
     libfvalue_value_entry_t *source_value_entry,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFVALUE_VALUE_ENTRY_H ) */

