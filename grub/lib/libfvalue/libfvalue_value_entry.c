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

#include <libyal_wrap.h>

#include "libfvalue_value_entry.h"

/* Creates a value entry
 * Make sure the value value_entry is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfvalue_value_entry_initialize(
     libfvalue_value_entry_t **value_entry,
     libcerror_error_t **error )
{
	static char *function = "libfvalue_value_entry_initialize";

	if( value_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value entry.",
		 function );

		return( -1 );
	}
	if( *value_entry != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid value entry value already set.",
		 function );

		return( -1 );
	}
	*value_entry = memory_allocate_structure(
	                libfvalue_value_entry_t );

	if( *value_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create value entry.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *value_entry,
	     0,
	     sizeof( libfvalue_value_entry_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear value entry.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *value_entry != NULL )
	{
		memory_free(
		 *value_entry );

		*value_entry = NULL;
	}
	return( -1 );
}

/* Frees a value entry
 * Returns 1 if successful or -1 on error
 */
int libfvalue_value_entry_free(
     libfvalue_value_entry_t **value_entry,
     libcerror_error_t **error )
{
	static char *function = "libfvalue_value_entry_free";

	if( value_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value entry.",
		 function );

		return( -1 );
	}
	if( *value_entry != NULL )
	{
		memory_free(
		 *value_entry );

		*value_entry = NULL;
	}
	return( 1 );
}

/* Clones a value entry
 * Returns 1 if successful or -1 on error
 */
int libfvalue_value_entry_clone(
     libfvalue_value_entry_t **destination_value_entry,
     libfvalue_value_entry_t *source_value_entry,
     libcerror_error_t **error )
{
	static char *function = "libfvalue_value_entry_clone";

	if( destination_value_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid destination value entry.",
		 function );

		return( -1 );
	}
	if( *destination_value_entry != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: destination value entry already set.",
		 function );

		return( -1 );
	}
	if( source_value_entry == NULL )
	{
		*destination_value_entry = NULL;

		return( 1 );
	}
	*destination_value_entry = memory_allocate_structure(
	                            libfvalue_value_entry_t );

	if( *destination_value_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create destination value entry.",
		 function );

		goto on_error;
	}
	if( memory_copy(
	     *destination_value_entry,
	     source_value_entry,
	     sizeof( libfvalue_value_entry_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy value entry.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *destination_value_entry != NULL )
	{
		memory_free(
		 *destination_value_entry );

		*destination_value_entry = NULL;
	}
	return( -1 );
}

