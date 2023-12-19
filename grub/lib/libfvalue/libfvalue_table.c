/*
 * Values table functions
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

#include "libfvalue_codepage.h"
#include "libfvalue_definitions.h"
#include "libfvalue_libcdata.h"
#include "libfvalue_table.h"
#include "libfvalue_types.h"
#include "libfvalue_value.h"
#include "libfvalue_value_type.h"

/* Creates a values table
 * Make sure the value table is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfvalue_table_initialize(
     libfvalue_table_t **table,
     int number_of_values,
     libcerror_error_t **error )
{
	libfvalue_internal_table_t *internal_table = NULL;
	static char *function                      = "libfvalue_table_initialize";

	if( table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid values table.",
		 function );

		return( -1 );
	}
	if( *table != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid table value already set.",
		 function );

		return( -1 );
	}
	internal_table = memory_allocate_structure(
	                  libfvalue_internal_table_t );

	if( internal_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create values table.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     internal_table,
	     0,
	     sizeof( libfvalue_internal_table_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear values table.",
		 function );

		memory_free(
		 internal_table );

		return( -1 );
	}
	if( libcdata_array_initialize(
	     &( internal_table->values ),
	     number_of_values,
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
	*table = (libfvalue_table_t *) internal_table;

	return( 1 );

on_error:
	if( internal_table != NULL )
	{
		memory_free(
		 internal_table );
	}
	return( -1 );
}

/* Frees a table
 * Returns 1 if successful or -1 on error
 */
int libfvalue_table_free(
     libfvalue_table_t **table,
     libcerror_error_t **error )
{
	libfvalue_internal_table_t *internal_table = NULL;
	static char *function                      = "libfvalue_table_free";
	int result                                 = 1;

	if( table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table.",
		 function );

		return( -1 );
	}
	if( *table != NULL )
	{
		internal_table = (libfvalue_internal_table_t *) *table;
		*table         = NULL;

		if( libcdata_array_free(
		     &( internal_table->values ),
		     (int (*)(intptr_t **, libcerror_error_t **)) &libfvalue_value_free,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free the values array.",
			 function );

			result = -1;
		}
		memory_free(
		 internal_table );
	}
	return( result );
}

/* Empties the values table
 * Returns 1 if successful or -1 on error
 */
int libfvalue_table_empty(
     libfvalue_table_t *table,
     libcerror_error_t **error )
{
	libfvalue_internal_table_t *internal_table = NULL;
	static char *function                      = "libfvalue_table_empty";

	if( table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid values table.",
		 function );

		return( -1 );
	}
	internal_table = (libfvalue_internal_table_t *) table;

	if( libcdata_array_empty(
	     internal_table->values,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libfvalue_value_free,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to empty values array.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Resizes the values table
 * Returns 1 if successful or -1 on error
 */
int libfvalue_table_resize(
     libfvalue_table_t *table,
     int number_of_values,
     libcerror_error_t **error )
{
	libfvalue_internal_table_t *internal_table = NULL;
	static char *function                      = "libfvalue_table_resize";

	if( table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid values table.",
		 function );

		return( -1 );
	}
	internal_table = (libfvalue_internal_table_t *) table;

	if( libcdata_array_resize(
	     internal_table->values,
	     number_of_values,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libfvalue_value_free,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_RESIZE_FAILED,
		 "%s: unable to resize values array.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Clones a table
 * Returns 1 if successful or -1 on error
 */
int libfvalue_table_clone(
     libfvalue_table_t **destination_table,
     libfvalue_table_t *source_table,
     libcerror_error_t **error )
{
	libfvalue_internal_table_t *internal_destination_table = NULL;
	libfvalue_internal_table_t *internal_source_table      = NULL;
	static char *function                                  = "libfvalue_table_clone";

	if( destination_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid destination table.",
		 function );

		return( -1 );
	}
	if( *destination_table != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: destination table already set.",
		 function );

		return( -1 );
	}
	if( source_table == NULL )
	{
		*destination_table = NULL;

		return( 1 );
	}
	internal_source_table = (libfvalue_internal_table_t *) source_table;

	internal_destination_table = memory_allocate_structure(
			              libfvalue_internal_table_t );

	if( internal_destination_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create destination values table.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     internal_destination_table,
	     0,
	     sizeof( libfvalue_internal_table_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear destination values table.",
		 function );

		goto on_error;
	}
	if( libcdata_array_clone(
	     &( internal_destination_table->values ),
	     internal_source_table->values,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libfvalue_value_free,
	     (int (*)(intptr_t **, intptr_t *, libcerror_error_t **) ) &libfvalue_value_clone,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create destination values array.",
		 function );

		goto on_error;
	}
	*destination_table = (libfvalue_table_t *) internal_destination_table;

	return( 1 );

on_error:
	if( internal_destination_table != NULL )
	{
		memory_free(
		 internal_destination_table );
	}
	return( -1 );
}

/* Retrieves the number of values
 * Returns 1 if successful or -1 on error
 */
int libfvalue_table_get_number_of_values(
     libfvalue_table_t *table,
     int *number_of_values,
     libcerror_error_t **error )
{
	libfvalue_internal_table_t *internal_table = NULL;
	static char *function                      = "libfvalue_table_get_number_of_values";

	if( table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid values table.",
		 function );

		return( -1 );
	}
	internal_table = (libfvalue_internal_table_t *) table;

	if( libcdata_array_get_number_of_entries(
	     internal_table->values,
	     number_of_values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of entries in the values array.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the index for an identifier
 *
 * When LIBFVALUE_TABLE_FLAG_ALLOW_PARTIAL_MATCH is set a match
 * of the first part of the identifier is allowed.
 *
 * Returns 1 if successful, 0 if no index was found or -1 on error
 */
int libfvalue_table_get_index_by_identifier(
     libfvalue_table_t *table,
     const uint8_t *identifier,
     size_t identifier_size,
     int *value_index,
     uint8_t flags,
     libcerror_error_t **error )
{
	libfvalue_internal_table_t *internal_table = NULL;
	libfvalue_internal_value_t *internal_value = NULL;
	static char *function                      = "libfvalue_table_get_index_by_identifier";
	size_t compare_size                        = 0;
	int number_of_values                       = 0;
	int result                                 = 0;

	if( table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid values table.",
		 function );

		return( -1 );
	}
	internal_table = (libfvalue_internal_table_t *) table;

	if( identifier == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid identifier.",
		 function );

		return( -1 );
	}
	if( ( identifier_size == 0 )
	 || ( identifier_size > (size_t) SSIZE_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid identifier size value out of bounds.",
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
	if( ( flags & ~( LIBFVALUE_TABLE_FLAG_ALLOW_PARTIAL_MATCH ) ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported flags: 0x%02" PRIx8 ".",
		 function,
		 flags );

		return( -1 );
	}
	if( libcdata_array_get_number_of_entries(
	     internal_table->values,
	     &number_of_values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of entries in the values array.",
		 function );

		return( -1 );
	}
	for( *value_index = 0;
	     *value_index < number_of_values;
	     *value_index += 1 )
	{
		if( libcdata_array_get_entry_by_index(
		     internal_table->values,
		     *value_index,
		     (intptr_t **) &internal_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve entry: %d from values array.",
			 function,
			 *value_index );

			return( -1 );
		}
		if( internal_value == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing internal value.",
			 function );

			return( -1 );
		}
		if( ( flags & LIBFVALUE_TABLE_FLAG_ALLOW_PARTIAL_MATCH ) == 0 )
		{
			if( internal_value->identifier_size != identifier_size )
			{
				continue;
			}
		}
		if( internal_value->identifier_size <= identifier_size )
		{
			compare_size = internal_value->identifier_size;
		}
		else
		{
			compare_size = identifier_size;
		}
		result = memory_compare(
		          internal_value->identifier,
		          identifier,
		          compare_size );

		if( result == 0 )
		{
			return( 1 );
		}
	}
	return( 0 );
}

/* Retrieves a specific value
 * Returns 1 if successful or -1 on error
 */
int libfvalue_table_get_value_by_index(
     libfvalue_table_t *table,
     int value_index,
     libfvalue_value_t **value,
     libcerror_error_t **error )
{
	libfvalue_internal_table_t *internal_table = NULL;
	static char *function                      = "libfvalue_table_get_value_by_index";

	if( table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid values table.",
		 function );

		return( -1 );
	}
	internal_table = (libfvalue_internal_table_t *) table;

	if( libcdata_array_get_entry_by_index(
	     internal_table->values,
	     value_index,
	     (intptr_t **) value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve entry: %d from values array.",
		 function,
		 value_index );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves a value for the specific identifier
 *
 * When LIBFVALUE_TABLE_FLAG_ALLOW_PARTIAL_MATCH is set a match
 * of the first part of the identifier is allowed.
 *
 * Returns 1 if successful, 0 if value not available or -1 on error
 */
int libfvalue_table_get_value_by_identifier(
     libfvalue_table_t *table,
     const uint8_t *identifier,
     size_t identifier_size,
     libfvalue_value_t **value,
     uint8_t flags,
     libcerror_error_t **error )
{
	libfvalue_internal_table_t *internal_table = NULL;
	static char *function                      = "libfvalue_table_get_value_by_identifier";
	int value_index                            = 0;
	int result                                 = 0;

	if( table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid values table.",
		 function );

		return( -1 );
	}
	internal_table = (libfvalue_internal_table_t *) table;

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
	result = libfvalue_table_get_index_by_identifier(
	          table,
	          identifier,
	          identifier_size,
	          &value_index,
	          flags,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to find index for: %s.",
		 function,
		 identifier );

		return( -1 );
	}
	else if( result != 0 )
	{
		if( libcdata_array_get_entry_by_index(
		     internal_table->values,
		     value_index,
		     (intptr_t **) value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve entry: %d from values array.",
			 function,
			 value_index );

			return( -1 );
		}
	}
	return( result );
}

/* Sets a specific value
 * Frees the stored value if necessary
 * Returns 1 if successful or -1 on error
 */
int libfvalue_table_set_value_by_index(
     libfvalue_table_t *table,
     int value_index,
     libfvalue_value_t *value,
     libcerror_error_t **error )
{
	libfvalue_internal_table_t *internal_table = NULL;
	libfvalue_value_t *stored_value            = NULL;
	static char *function                      = "libfvalue_table_set_value_by_index";

	if( table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid values table.",
		 function );

		return( -1 );
	}
	internal_table = (libfvalue_internal_table_t *) table;

	if( libcdata_array_get_entry_by_index(
	     internal_table->values,
	     value_index,
	     (intptr_t **) &stored_value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve entry: %d from values array.",
		 function,
		 value_index );

		return( -1 );
	}
	if( ( stored_value != NULL )
	 && ( stored_value != value ) )
	{
		if( libfvalue_value_free(
		     &stored_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free value: %d.",
			 function,
			 value_index );

			return( -1 );
		}
	}
	if( libcdata_array_set_entry_by_index(
	     internal_table->values,
	     value_index,
	     (intptr_t *) value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set entry: %d in values array.",
		 function,
		 value_index );

		return( -1 );
	}
	return( 1 );
}

/* Sets a value in the values table
 * This function appends a new value or replaces an existing value
 * Returns 1 if successful or -1 on error
 */
int libfvalue_table_set_value(
     libfvalue_table_t *table,
     libfvalue_value_t *value,
     libcerror_error_t **error )
{
	libfvalue_internal_table_t *internal_table = NULL;
	libfvalue_internal_value_t *internal_value = NULL;
	libfvalue_value_t *stored_value            = NULL;
	static char *function                      = "libfvalue_table_set_value";
	int value_index                            = 0;
	int result                                 = 0;

	if( table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid values table.",
		 function );

		return( -1 );
	}
	internal_table = (libfvalue_internal_table_t *) table;

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
	internal_value = (libfvalue_internal_value_t *) value;

	result = libfvalue_table_get_index_by_identifier(
	          table,
	          internal_value->identifier,
	          internal_value->identifier_size,
	          &value_index,
	          0,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to find index for: %s.",
		 function,
		 (char *) internal_value->identifier );

		return( -1 );
	}
	else if( result == 0 )
	{
		if( libcdata_array_insert_entry(
		     internal_table->values,
		     &value_index,
		     (intptr_t *) value,
		     &libfvalue_compare_identifier,
		     LIBCDATA_INSERT_FLAG_UNIQUE_ENTRIES,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to insert value: %s in values array.",
			 function,
			 (char *) internal_value->identifier );

			return( -1 );
		}
	}
	else
	{
		if( libcdata_array_get_entry_by_index(
		     internal_table->values,
		     value_index,
		     (intptr_t **) &stored_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve entry: %d from values array.",
			 function,
			 value_index );

			return( -1 );
		}
		if( ( stored_value != NULL )
		 && ( stored_value != value ) )
		{
			if( libcdata_array_set_entry_by_index(
			     internal_table->values,
			     value_index,
			     (intptr_t *) value,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set entry: %d in values array.",
				 function,
				 value_index );

				return( -1 );
			}
			if( libfvalue_value_free(
			     &stored_value,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free value: %d.",
				 function,
				 value_index );

				return( -1 );
			}
		}
	}
	return( 1 );
}

enum LIBFVALUE_XML_TAG_TYPES
{
	LIBFVALUE_XML_TAG_TYPE_CLOSE	= (uint8_t) 'c',
	LIBFVALUE_XML_TAG_TYPE_OPEN	= (uint8_t) 'o'
};

/* Copies the values table from an UTF-8 encoded XML string
 * Returns 1 if successful, 0 if the requested section could not be found or -1 on error
 */
int libfvalue_table_copy_from_utf8_xml_string(
     libfvalue_table_t *table,
     const uint8_t *utf8_string,
     size_t utf8_string_size,
     const uint8_t *table_name,
     size_t table_name_length,
     libcerror_error_t **error )
{
	libfvalue_internal_table_t *internal_table = NULL;
	libfvalue_value_t *value                   = NULL;
	uint8_t *value_data                        = NULL;
	uint8_t *value_identifier                  = NULL;
	const uint8_t *xml_table_name              = NULL;
	const uint8_t *xml_tag_data                = NULL;
	const uint8_t *xml_tag_name                = NULL;
	static char *function                      = "libfvalue_table_copy_from_utf8_xml_string";
	size_t string_index                        = 0;
	size_t value_identifier_length             = 0;
	size_t value_data_length                   = 0;
	size_t xml_table_name_length               = 0;
	size_t xml_tag_name_length                 = 0;
	uint8_t xml_tag_type                       = 0;
	int result                                 = 0;
	int value_index                            = 0;

	if( table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid values table.",
		 function );

		return( -1 );
	}
	internal_table = (libfvalue_internal_table_t *) table;

	if( utf8_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-8 string.",
		 function );

		return( -1 );
	}
	if( ( utf8_string_size == 0 )
	 || ( utf8_string_size > (size_t) SSIZE_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid UTF-8 string size value out of bounds.",
		 function );

		return( -1 );
	}
	if( table_name == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table name.",
		 function );

		return( -1 );
	}
	if( table_name_length > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid table name size value out of bounds.",
		 function );

		return( -1 );
	}
#ifdef TODO
	if( libcdata_array_empty(
	     internal_table->values,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libfvalue_value_free,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to empty values array.",
		 function );

		return( -1 );
	}
#endif
	/* TODO check for "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"; */

	/* Note that below numeric values for the characters are used to prevent
	 * platform specific values being assigned
	 */
	while( string_index < utf8_string_size )
	{
		/* Look for the start of the XML tag: '<'
		 */
		if( utf8_string[ string_index++ ] != 0x3c )
		{
			continue;
		}
		if( string_index >= utf8_string_size )
		{
			break;
		}
		/* Determine if the XML tag is an open or close tag: '/'
		 */
		if( utf8_string[ string_index ] != 0x2f )
		{
			xml_tag_type = LIBFVALUE_XML_TAG_TYPE_OPEN;
		}
		else
		{
			xml_tag_type = LIBFVALUE_XML_TAG_TYPE_CLOSE;

			string_index++;

			if( string_index >= utf8_string_size )
			{
				break;
			}
		}
		xml_tag_name        = &( utf8_string[ string_index ] );
		xml_tag_name_length = 0;

		while( string_index < utf8_string_size )
		{
			/* The XML tag name should only consists of: [A-Za-z0-9_]
			 */
			if( ( ( utf8_string[ string_index ] >= 0x41 )
			  &&  ( utf8_string[ string_index ] <= 0x5a ) )
			 || ( ( utf8_string[ string_index ] >= 0x61 )
			  &&  ( utf8_string[ string_index ] <= 0x7a ) )
			 || ( ( utf8_string[ string_index ] >= 0x30 )
			  &&  ( utf8_string[ string_index ] <= 0x39 ) )
			 ||   ( utf8_string[ string_index ] == 0x5f ) )
			{
				xml_tag_name_length++;
			}
			else
			{
				break;
			}
			string_index++;
		}
		if( string_index >= utf8_string_size )
		{
			break;
		}
		/* TODO add support for XML tag attributes but ignore them for now
		 */
		while( string_index < utf8_string_size )
		{
			/* Look for the end of the XML tag: '>'
			 */
			if( utf8_string[ string_index ] == 0x3e )
			{
				break;
			}
			string_index++;
		}
		if( string_index >= utf8_string_size )
		{
			break;
		}
		/* Look for the end of the XML tag: '>'
		 */
		if( utf8_string[ string_index++ ] != 0x3e )
		{
			continue;
		}
		if( string_index >= utf8_string_size )
		{
			break;
		}
		if( ( xml_tag_name == NULL )
		 || ( xml_tag_name_length == 0 ) )
		{
			continue;
		}
		if( xml_table_name == NULL )
		{
			if( ( xml_tag_type == LIBFVALUE_XML_TAG_TYPE_OPEN )
			 && ( xml_tag_name_length == table_name_length )
			 && ( narrow_string_compare(
			       table_name,
			       xml_tag_name,
			       xml_tag_name_length ) == 0 ) )
			{
				xml_table_name        = xml_tag_name;
				xml_table_name_length = xml_tag_name_length;
			}
		}
		else if( value_identifier == NULL )
		{
			if( ( xml_tag_type == LIBFVALUE_XML_TAG_TYPE_CLOSE )
			 && ( xml_tag_name_length == xml_table_name_length )
			 && ( narrow_string_compare(
			       xml_table_name,
			       xml_tag_name,
			       xml_tag_name_length ) == 0 ) )
			{
				xml_table_name = NULL;
				result         = 1;

				break;
			}
			else if( xml_tag_type == LIBFVALUE_XML_TAG_TYPE_OPEN )
			{
				value_identifier_length = xml_tag_name_length;

				value_identifier = (uint8_t *) memory_allocate(
				                                sizeof( uint8_t ) * ( value_identifier_length + 1 ) );

				if( value_identifier == NULL )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_MEMORY,
					 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
					 "%s: unable to create value identifier.",
					 function );

					goto on_error;
				}
				if( memory_copy(
				     value_identifier,
				     xml_tag_name,
				     value_identifier_length ) == NULL )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_MEMORY,
					 LIBCERROR_MEMORY_ERROR_SET_FAILED,
					 "%s: unable to clear values table.",
					 function );

					goto on_error;
				}
				value_identifier[ value_identifier_length ] = 0;

				xml_tag_data = &( utf8_string[ string_index ] );
			}
		}
		else
		{
			if( ( xml_tag_type == LIBFVALUE_XML_TAG_TYPE_CLOSE )
			 && ( xml_tag_name_length == value_identifier_length )
			 && ( narrow_string_compare(
			       value_identifier,
			       xml_tag_name,
			       xml_tag_name_length ) == 0 ) )
			{
				result = libfvalue_table_get_index_by_identifier(
				          table,
				          value_identifier,
				          value_identifier_length + 1,
				          &value_index,
				          0,
				          error );

				if( result == -1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to find index for: %s.",
					 function,
					 (char *) value_identifier );

					goto on_error;
				}
				else if( result == 0 )
				{
					if( libfvalue_value_type_initialize(
					     &value,
					     LIBFVALUE_VALUE_TYPE_STRING_UTF8,
					     error ) != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
						 "%s: unable to create value: %s.",
						 function,
						 (char *) value_identifier );

						goto on_error;
					}
					if( libfvalue_value_set_identifier(
					     value,
					     value_identifier,
					     value_identifier_length + 1,
					     LIBFVALUE_VALUE_IDENTIFIER_FLAG_MANAGED,
					     error ) != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
						 "%s: unable to set identifier in value: %s.",
						 function,
						 (char *) value_identifier );

						libfvalue_value_free(
						 &value,
						 NULL );

						goto on_error;
					}
					if( libcdata_array_insert_entry(
					     internal_table->values,
					     &value_index,
					     (intptr_t *) value,
					     &libfvalue_compare_identifier,
					     LIBCDATA_INSERT_FLAG_UNIQUE_ENTRIES,
					     error ) != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
						 "%s: unable to insert value: %s in values array.",
						 function,
						 (char *) value_identifier );

						libfvalue_value_free(
						 &value,
						 NULL );

						goto on_error;
					}
				}
				else
				{
					if( libcdata_array_get_entry_by_index(
					     internal_table->values,
					     value_index,
					     (intptr_t **) &value,
					     error ) != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
						 "%s: unable to retrieve entry: %d from values array.",
						 function,
						 value_index );

						goto on_error;
					}
				}
				/* Note that the XML tag name of the closing tag is preceded by
				 * 2 characters '</' which are not part of the data
				 */
				value_data_length = (size_t) ( xml_tag_name - ( xml_tag_data + 2 ) );

				value_data = (uint8_t *) memory_allocate(
				                          sizeof( uint8_t ) * ( value_data_length + 1 ) );

				if( value_data == NULL )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_MEMORY,
					 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
					 "%s: unable to create value data.",
					 function );

					goto on_error;
				}
				if( memory_copy(
				     value_data,
				     xml_tag_data,
				     value_data_length ) == NULL )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_MEMORY,
					 LIBCERROR_MEMORY_ERROR_SET_FAILED,
					 "%s: unable to clear values table.",
					 function );

					goto on_error;
				}
				value_data[ value_data_length ] = 0;

				if( libfvalue_value_set_data(
				     value,
				     (uint8_t *) value_data,
				     value_data_length + 1,
				     LIBFVALUE_CODEPAGE_UTF8,
				     LIBFVALUE_VALUE_DATA_FLAG_MANAGED | LIBFVALUE_VALUE_DATA_FLAG_CLONE_BY_REFERENCE,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
					 "%s: unable to set value: %s data.",
					 function,
					 (char *) value_identifier );

					goto on_error;
				}
				/* value_data is now managed by the value
				 */
				memory_free(
				 value_identifier );

				value_identifier = NULL;
				value_data       = NULL;
				value            = NULL;
			}
		}
	}
	if( value_identifier != NULL )
	{
		memory_free(
		 value_identifier );
	}
	return( result );

on_error:
	if( value_data != NULL )
	{
		memory_free(
		 value_data );
	}
	if( value_identifier != NULL )
	{
		memory_free(
		 value_identifier );
	}
	return( -1 );
}

