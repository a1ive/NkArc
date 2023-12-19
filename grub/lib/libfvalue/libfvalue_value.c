/*
 * Value functions
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

#include "libfvalue_data_handle.h"
#include "libfvalue_definitions.h"
#include "libfvalue_libcdata.h"
#include "libfvalue_types.h"
#include "libfvalue_value.h"

/* Creates a value
 * Make sure the value value is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfvalue_value_initialize(
     libfvalue_value_t **value,
     const char *type_string,
     const char *type_description,
     libfvalue_data_handle_t *data_handle,
     int (*initialize_instance)(
           intptr_t **instance,
           libcerror_error_t **error ),
     int (*free_instance)(
           intptr_t **instance,
           libcerror_error_t **error ),
     int (*clone_instance)(
           intptr_t **destination_instance,
           intptr_t *source_instance,
           libcerror_error_t **error ),
     int (*copy_from_byte_stream)(
           intptr_t *instance,
           const uint8_t *byte_stream,
           size_t byte_stream_size,
           int encoding,
           libcerror_error_t **error ),
     int (*copy_to_byte_stream)(
           intptr_t *instance,
           uint8_t *byte_stream,
           size_t byte_stream_size,
           int encoding,
           libcerror_error_t **error ),
     int (*copy_from_integer)(
           intptr_t *instance,
           uint64_t integer_value,
           size_t integer_value_size,
           libcerror_error_t **error ),
     int (*copy_to_integer)(
           intptr_t *instance,
           uint64_t *integer_value,
           size_t *integer_value_size,
           libcerror_error_t **error ),
     int (*copy_from_floating_point)(
           intptr_t *instance,
           double floating_point_value,
           size_t floating_point_value_size,
           libcerror_error_t **error ),
     int (*copy_to_floating_point)(
           intptr_t *instance,
           double *floating_point_value,
           size_t *floating_point_value_size,
           libcerror_error_t **error ),
     int (*copy_from_utf8_string_with_index)(
           intptr_t *instance,
           const uint8_t *utf8_string,
           size_t utf8_string_length,
           size_t *utf8_string_index,
           uint32_t string_format_flags,
           libcerror_error_t **error ),
     int (*get_utf8_string_size)(
           intptr_t *instance,
           size_t *utf8_string_size,
           uint32_t string_format_flags,
           libcerror_error_t **error ),
     int (*copy_to_utf8_string_with_index)(
           intptr_t *instance,
           uint8_t *utf8_string,
           size_t utf8_string_size,
           size_t *utf8_string_index,
           uint32_t string_format_flags,
           libcerror_error_t **error ),
     int (*copy_from_utf16_string_with_index)(
           intptr_t *instance,
           const uint16_t *utf16_string,
           size_t utf16_string_length,
           size_t *utf16_string_index,
           uint32_t string_format_flags,
           libcerror_error_t **error ),
     int (*get_utf16_string_size)(
           intptr_t *instance,
           size_t *utf16_string_size,
           uint32_t string_format_flags,
           libcerror_error_t **error ),
     int (*copy_to_utf16_string_with_index)(
           intptr_t *instance,
           uint16_t *utf16_string,
           size_t utf16_string_size,
           size_t *utf16_string_index,
           uint32_t string_format_flags,
           libcerror_error_t **error ),
     int (*copy_from_utf32_string_with_index)(
           intptr_t *instance,
           const uint32_t *utf32_string,
           size_t utf32_string_length,
           size_t *utf32_string_index,
           uint32_t string_format_flags,
           libcerror_error_t **error ),
     int (*get_utf32_string_size)(
           intptr_t *instance,
           size_t *utf32_string_size,
           uint32_t string_format_flags,
           libcerror_error_t **error ),
     int (*copy_to_utf32_string_with_index)(
           intptr_t *instance,
           uint32_t *utf32_string,
           size_t utf32_string_size,
           size_t *utf32_string_index,
           uint32_t string_format_flags,
           libcerror_error_t **error ),
     uint8_t flags,
     libcerror_error_t **error )
{
	libfvalue_internal_value_t *internal_value = NULL;
	static char *function                      = "libfvalue_value_initialize";

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
	if( *value != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid value value already set.",
		 function );

		return( -1 );
	}
	if( type_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid type string",
		 function );

		return( -1 );
	}
	internal_value = memory_allocate_structure(
	                  libfvalue_internal_value_t );

	if( internal_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create value.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     internal_value,
	     0,
	     sizeof( libfvalue_internal_value_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear value.",
		 function );

		memory_free(
		 internal_value );

		return( -1 );
	}
	if( free_instance != NULL )
	{
		if( libcdata_array_initialize(
		     &( internal_value->value_instances ),
		     1,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create value instances array.",
			 function );

			goto on_error;
		}
	}
	if( data_handle == NULL )
	{
/* TODO add read function ? */
		if( libfvalue_data_handle_initialize(
		     &( internal_value->data_handle ),
		     NULL,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create data handle.",
			 function );

			goto on_error;
		}
		internal_value->flags |= LIBFVALUE_VALUE_FLAG_DATA_HANDLE_MANAGED;
	}
	else
	{
		internal_value->data_handle = data_handle;
	}
	internal_value->type_string                       = type_string;
	internal_value->type_description                  = type_description;

	internal_value->initialize_instance               = initialize_instance;
	internal_value->free_instance                     = free_instance;
	internal_value->clone_instance                    = clone_instance;

	internal_value->copy_from_byte_stream             = copy_from_byte_stream;
	internal_value->copy_to_byte_stream               = copy_to_byte_stream;

	internal_value->copy_from_integer                 = copy_from_integer;
	internal_value->copy_to_integer                   = copy_to_integer;

	internal_value->copy_from_floating_point          = copy_from_floating_point;
	internal_value->copy_to_floating_point            = copy_to_floating_point;

	internal_value->copy_from_utf8_string_with_index  = copy_from_utf8_string_with_index;
	internal_value->get_utf8_string_size              = get_utf8_string_size;
	internal_value->copy_to_utf8_string_with_index    = copy_to_utf8_string_with_index;

	internal_value->copy_from_utf16_string_with_index = copy_from_utf16_string_with_index;
	internal_value->get_utf16_string_size             = get_utf16_string_size;
	internal_value->copy_to_utf16_string_with_index   = copy_to_utf16_string_with_index;

	internal_value->copy_from_utf32_string_with_index = copy_from_utf32_string_with_index;
	internal_value->get_utf32_string_size             = get_utf32_string_size;
	internal_value->copy_to_utf32_string_with_index   = copy_to_utf32_string_with_index;

	internal_value->flags                            |= flags;

	*value = (libfvalue_value_t *) internal_value;

	return( 1 );

on_error:
	if( internal_value != NULL )
	{
		if( internal_value->value_instances != NULL )
		{
			libcdata_array_free(
			 &( internal_value->value_instances ),
			 NULL,
			 NULL );
		}
		memory_free(
		 internal_value );
	}
	return( -1 );
}

/* Frees a value
 * Returns 1 if successful or -1 on error
 */
int libfvalue_value_free(
     libfvalue_value_t **value,
     libcerror_error_t **error )
{
	libfvalue_internal_value_t *internal_value = NULL;
	static char *function                      = "libfvalue_value_free";
	int result                                 = 1;

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
	if( *value != NULL )
	{
		if( libfvalue_value_clear(
		     *value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free data handle.",
			 function );

			result = -1;
		}
		internal_value = (libfvalue_internal_value_t *) *value;
		*value         = NULL;

		if( internal_value->value_instances != NULL )
		{
			if( libcdata_array_free(
			     &( internal_value->value_instances ),
			     internal_value->free_instance,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free value instances array.",
				 function );

				result = -1;
			}
		}
		if( ( internal_value->flags & LIBFVALUE_VALUE_FLAG_DATA_HANDLE_MANAGED ) != 0 )
		{
			if( libfvalue_data_handle_free(
			     &( internal_value->data_handle ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free data handle.",
				 function );

				result = -1;
			}
			internal_value->flags &= ~( LIBFVALUE_VALUE_FLAG_DATA_HANDLE_MANAGED );
		}
		memory_free(
		 internal_value );
	}
	return( result );
}

/* Clones a value
 * Returns 1 if successful or -1 on error
 */
int libfvalue_value_clone(
     libfvalue_value_t **destination_value,
     libfvalue_value_t *source_value,
     libcerror_error_t **error )
{
	libfvalue_internal_value_t *internal_source_value = NULL;
	libfvalue_data_handle_t *destination_data_handle  = NULL;
	static char *function                             = "libfvalue_value_clone";

	if( destination_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid destination value.",
		 function );

		return( -1 );
	}
	if( *destination_value != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: destination value already set.",
		 function );

		return( -1 );
	}
	if( source_value == NULL )
	{
		*destination_value = NULL;

		return( 1 );
	}
	internal_source_value = (libfvalue_internal_value_t *) source_value;

	if( libfvalue_data_handle_clone(
	     &destination_data_handle,
	     internal_source_value->data_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create destination data handle.",
		 function );

		goto on_error;
	}
	if( libfvalue_value_initialize(
	     destination_value,
	     internal_source_value->type_string,
	     internal_source_value->type_description,
	     destination_data_handle,
	     internal_source_value->initialize_instance,
	     internal_source_value->free_instance,
	     internal_source_value->clone_instance,
	     internal_source_value->copy_from_byte_stream,
	     internal_source_value->copy_to_byte_stream,
	     internal_source_value->copy_from_integer,
	     internal_source_value->copy_to_integer,
	     internal_source_value->copy_from_floating_point,
	     internal_source_value->copy_to_floating_point,
	     internal_source_value->copy_from_utf8_string_with_index,
	     internal_source_value->get_utf8_string_size,
	     internal_source_value->copy_to_utf8_string_with_index,
	     internal_source_value->copy_from_utf16_string_with_index,
	     internal_source_value->get_utf16_string_size,
	     internal_source_value->copy_to_utf16_string_with_index,
	     internal_source_value->copy_from_utf32_string_with_index,
	     internal_source_value->get_utf32_string_size,
	     internal_source_value->copy_to_utf32_string_with_index,
	     internal_source_value->flags | LIBFVALUE_VALUE_FLAG_DATA_HANDLE_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create destination value.",
		 function );

		goto on_error;
	}
	if( *destination_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing destination value.",
		 function );

		goto on_error;
	}
	destination_data_handle = NULL;

	if( internal_source_value->identifier != NULL )
	{
		if( libfvalue_value_set_identifier(
		     *destination_value,
		     internal_source_value->identifier,
		     internal_source_value->identifier_size,
		     LIBFVALUE_VALUE_IDENTIFIER_FLAG_MANAGED,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set identifier in destination value.",
			 function );

			goto on_error;
		}
	}
	/* The value instances are not cloned and are re-created on demand
	 */
	return( 1 );

on_error:
	if( destination_data_handle != NULL )
	{
		libfvalue_data_handle_free(
		 &destination_data_handle,
		 NULL );
	}
	if( *destination_value != NULL )
	{
		libfvalue_value_free(
		 destination_value,
		 NULL );
	}
	return( -1 );
}

/* Clears a value
 * Returns 1 if successful or -1 on error
 */
int libfvalue_value_clear(
     libfvalue_value_t *value,
     libcerror_error_t **error )
{
	libfvalue_internal_value_t *internal_value = NULL;
	static char *function                      = "libfvalue_value_clear";

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

	if( ( internal_value->flags & LIBFVALUE_VALUE_FLAG_IDENTIFIER_MANAGED ) != 0 )
	{
		if( internal_value->identifier != NULL )
		{
			memory_free(
			 internal_value->identifier );
		}
		internal_value->flags &= ~( LIBFVALUE_VALUE_FLAG_IDENTIFIER_MANAGED );
	}
	if( libfvalue_data_handle_clear(
	     internal_value->data_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to clear data handle.",
		 function );

		return( -1 );
	}
	if( internal_value->value_instances != NULL )
	{
		if( internal_value->free_instance == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid value - missing free instance function.",
			 function );

			return( -1 );
		}
		if( libcdata_array_empty(
		     internal_value->value_instances,
		     internal_value->free_instance,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to empty value instances array.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Retrieves the value type
 * Returns 1 if successful or -1 on error
 */
int libfvalue_value_get_type(
     libfvalue_value_t *value,
     int *value_type,
     libcerror_error_t **error )
{
	libfvalue_internal_value_t *internal_value = NULL;
	static char *function                      = "libfvalue_value_get_type";

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

	if( value_type == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value type.",
		 function );

		return( -1 );
	}
	*value_type = internal_value->type;

	return( 1 );
}

/* Compares two index values
 * Returns LIBCDATA_COMPARE_LESS, LIBCDATA_COMPARE_EQUAL, LIBCDATA_COMPARE_GREATER if successful or -1 on error
 */
int libfvalue_compare_identifier(
     intptr_t *first_value,
     intptr_t *second_value,
     libcerror_error_t **error )
{
	libfvalue_internal_value_t *internal_first_value  = NULL;
	libfvalue_internal_value_t *internal_second_value = NULL;
	static char *function                             = "libfvalue_compare_identifier";
	size_t compare_size                               = 0;
	int result                                        = 0;

	if( first_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid first value.",
		 function );

		return( -1 );
	}
	internal_first_value = (libfvalue_internal_value_t *) first_value;

	if( second_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid second value.",
		 function );

		return( -1 );
	}
	internal_second_value = (libfvalue_internal_value_t *) second_value;

	if( internal_first_value->identifier == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid first value - missing identifier.",
		 function );

		return( -1 );
	}
	if( internal_second_value->identifier == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid second value - missing identifier.",
		 function );

		return( -1 );
	}
	if( internal_first_value->identifier_size <= internal_second_value->identifier_size )
	{
		compare_size = internal_first_value->identifier_size;
	}
	else
	{
		compare_size = internal_second_value->identifier_size;
	}
	result = memory_compare(
	          internal_first_value->identifier,
	          internal_second_value->identifier,
	          compare_size );

	if( result < 0 )
	{
		return( LIBCDATA_COMPARE_LESS );
	}
	else if( result > 0 )
	{
		return( LIBCDATA_COMPARE_GREATER );
	}
	if( internal_first_value->identifier_size < internal_second_value->identifier_size )
	{
		return( LIBCDATA_COMPARE_LESS );
	}
	else if( internal_first_value->identifier_size > internal_second_value->identifier_size )
	{
		return( LIBCDATA_COMPARE_GREATER );
	}
	return( LIBCDATA_COMPARE_EQUAL );
}

/* Retrieves the identifier
 * Returns 1 if successful or -1 on error
 */
int libfvalue_value_get_identifier(
     libfvalue_value_t *value,
     uint8_t **identifier,
     size_t *identifier_size,
     libcerror_error_t **error )
{
	libfvalue_internal_value_t *internal_value = NULL;
	static char *function                      = "libfvalue_value_get_identifier";

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
	if( identifier_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid identifier size.",
		 function );

		return( -1 );
	}
	*identifier      = internal_value->identifier;
	*identifier_size = internal_value->identifier_size;

	return( 1 );
}

/* Sets the identifier
 * Returns 1 if successful or -1 on error
 */
int libfvalue_value_set_identifier(
     libfvalue_value_t *value,
     const uint8_t *identifier,
     size_t identifier_size,
     uint8_t flags,
     libcerror_error_t **error )
{
	libfvalue_internal_value_t *internal_value = NULL;
	static char *function                      = "libfvalue_value_set_identifier";
	uint8_t supported_flags                    = 0;

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

	if( internal_value->identifier != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid value - identifier already set.",
		 function );

		return( -1 );
	}
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
		 "%s: invalid identifier length value out of bounds.",
		 function );

		return( -1 );
	}
	supported_flags = LIBFVALUE_VALUE_IDENTIFIER_FLAG_MANAGED
	                | LIBFVALUE_VALUE_IDENTIFIER_FLAG_CLONE_BY_REFERENCE;

	if( ( flags & ~( supported_flags ) ) != 0 )
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
	if( ( internal_value->flags & LIBFVALUE_VALUE_FLAG_IDENTIFIER_MANAGED ) != 0 )
	{
		if( internal_value->identifier != NULL )
		{
			memory_free(
			 internal_value->identifier );

			internal_value->identifier      = NULL;
			internal_value->identifier_size = 0;
		}
		internal_value->flags &= ~( LIBFVALUE_VALUE_FLAG_IDENTIFIER_MANAGED );
	}
	if( ( flags & LIBFVALUE_VALUE_IDENTIFIER_FLAG_CLONE_BY_REFERENCE ) != 0 )
	{
		internal_value->identifier = (uint8_t *) identifier;

		if( ( flags & LIBFVALUE_VALUE_IDENTIFIER_FLAG_MANAGED ) != 0 )
		{
			internal_value->flags |= LIBFVALUE_VALUE_FLAG_IDENTIFIER_MANAGED;
		}
	}
	else
	{
		internal_value->identifier = (uint8_t *) memory_allocate(
		                                          sizeof( uint8_t ) * identifier_size );

		if( internal_value->identifier == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create identifier.",
			 function );

			goto on_error;
		}
		if( memory_copy(
		     internal_value->identifier,
		     identifier,
		     identifier_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy identifier.",
			 function );

			goto on_error;
		}
		internal_value->flags |= LIBFVALUE_VALUE_FLAG_IDENTIFIER_MANAGED;
	}
	internal_value->identifier_size = identifier_size;

	return( 1 );

on_error:
	if( internal_value->identifier != NULL )
	{
		memory_free(
		 internal_value->identifier );

		internal_value->identifier = NULL;
	}
	return( -1 );
}

/* Retrieves the value data flags
 * Returns 1 if successful or -1 on error
 */
int libfvalue_value_get_data_flags(
     libfvalue_value_t *value,
     uint32_t *data_flags,
     libcerror_error_t **error )
{
	libfvalue_internal_value_t *internal_value = NULL;
	static char *function                      = "libfvalue_value_get_data_flags";

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

	if( libfvalue_data_handle_get_data_flags(
	     internal_value->data_handle,
	     data_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve data flags from data handle.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Sets the value data flags
 * Returns 1 if successful or -1 on error
 */
int libfvalue_value_set_data_flags(
     libfvalue_value_t *value,
     uint32_t data_flags,
     libcerror_error_t **error )
{
	libfvalue_internal_value_t *internal_value = NULL;
	static char *function                      = "libfvalue_value_set_data_flags";

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

	if( libfvalue_data_handle_set_data_flags(
	     internal_value->data_handle,
	     data_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set data flags in data handle.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Determines if the value has data
 * Returns 1 if the value has data, 0 if not or -1 on error
 */
int libfvalue_value_has_data(
     libfvalue_value_t *value,
     libcerror_error_t **error )
{
	libfvalue_internal_value_t *internal_value = NULL;
	uint8_t *data                              = NULL;
	static char *function                      = "libfvalue_value_has_data";
	size_t data_size                           = 0;
	int encoding                               = 0;

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

	if( libfvalue_data_handle_get_data(
	     internal_value->data_handle,
	     &data,
	     &data_size,
	     &encoding,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve data from data handle.",
		 function );

		return( -1 );
	}
	if( data == NULL )
	{
		return( 0 );
	}
	return( 1 );
}

/* Initializes the data
 * Returns 1 if successful or -1 on error
 */
int libfvalue_value_initialize_data(
     libfvalue_value_t *value,
     size_t data_size,
     libcerror_error_t **error )
{
	libfvalue_internal_value_t *internal_value = NULL;
	uint8_t *data                              = NULL;
	static char *function                      = "libfvalue_value_initialize_data";
	int result                                 = 0;

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

	if( ( data_size == 0 )
	 || ( data_size > (size_t) SSIZE_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid data size value out of bounds.",
		 function );

		return( -1 );
	}
	result = libfvalue_value_has_data(
		  value,
		  error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if value has data.",
		 function );

		return( -1 );
	}
	else if( result != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid value data already set.",
		 function );

		return( -1 );
	}
	data = (uint8_t *) memory_allocate(
	                    sizeof( uint8_t ) * data_size );

	if( data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create data.",
		 function );

		goto on_error;
	}
	if( libfvalue_data_handle_set_data(
	     internal_value->data_handle,
	     data,
	     data_size,
	     LIBFVALUE_ENDIAN_NATIVE,
	     LIBFVALUE_VALUE_DATA_FLAG_MANAGED | LIBFVALUE_VALUE_DATA_FLAG_CLONE_BY_REFERENCE,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set data in data handle.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( data != NULL )
	{
		memory_free(
		 data );
	}
	return( -1 );
}

/* Retrieves the data size
 * Returns 1 if successful or -1 on error
 */
int libfvalue_value_get_data_size(
     libfvalue_value_t *value,
     size_t *data_size,
     libcerror_error_t **error )
{
	libfvalue_internal_value_t *internal_value = NULL;
	uint8_t *data                              = NULL;
	static char *function                      = "libfvalue_value_get_data_size";
	int encoding                               = 0;

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

	if( libfvalue_data_handle_get_data(
	     internal_value->data_handle,
	     &data,
	     data_size,
	     &encoding,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve data from data handle.",
		 function );

		return( -1 );
	}
	if( data == NULL )
	{
		if( data_size == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid data size.",
			 function );

			return( -1 );
		}
		*data_size = 0;
	}
	return( 1 );
}

/* Retrieves the data
 * Returns 1 if successful or -1 on error
 */
int libfvalue_value_get_data(
     libfvalue_value_t *value,
     uint8_t **data,
     size_t *data_size,
     int *encoding,
     libcerror_error_t **error )
{
	libfvalue_internal_value_t *internal_value = NULL;
	static char *function                      = "libfvalue_value_get_data";

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

	if( libfvalue_data_handle_get_data(
	     internal_value->data_handle,
	     data,
	     data_size,
	     encoding,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve data from data handle.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Sets the data
 * Returns 1 if successful or -1 on error
 */
int libfvalue_value_set_data(
     libfvalue_value_t *value,
     const uint8_t *data,
     size_t data_size,
     int encoding,
     uint8_t flags,
     libcerror_error_t **error )
{
	libfvalue_internal_value_t *internal_value = NULL;
	static char *function                      = "libfvalue_value_set_data";

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

	if( libfvalue_data_handle_set_data(
	     internal_value->data_handle,
	     data,
	     data_size,
	     encoding,
	     flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set data in data handle.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Copies the data
 * Returns 1 if successful or -1 on error
 */
int libfvalue_value_copy_data(
     libfvalue_value_t *value,
     uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	libfvalue_internal_value_t *internal_value = NULL;
	uint8_t *data_handle_data                  = NULL;
	static char *function                      = "libfvalue_value_copy_data";
	size_t data_handle_data_size               = 0;
	int encoding                               = 0;

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

	if( data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data.",
		 function );

		return( -1 );
	}
	if( data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( libfvalue_data_handle_get_data(
	     internal_value->data_handle,
	     &data_handle_data,
	     &data_handle_data_size,
	     &encoding,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve data from data handle.",
		 function );

		return( -1 );
	}
	if( ( data_handle_data == NULL )
	 || ( data_handle_data_size == 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing data in data handle.",
		 function );

		return( -1 );
	}
	if( data_handle_data_size > data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: invalid data size value too small.",
		 function );

		return( -1 );
	}
	if( memory_copy(
	     data,
	     data_handle_data,
	     data_handle_data_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy data.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the format flags
 * Returns 1 if successful or -1 on error
 */
int libfvalue_value_get_format_flags(
     libfvalue_value_t *value,
     uint32_t *format_flags,
     libcerror_error_t **error )
{
	libfvalue_internal_value_t *internal_value = NULL;
	static char *function                      = "libfvalue_value_get_format_flags";

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

	if( format_flags == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid format flags.",
		 function );

		return( -1 );
	}
	*format_flags = internal_value->format_flags;

	return( 1 );
}

/* Sets the format flags
 * Returns 1 if successful or -1 on error
 */
int libfvalue_value_set_format_flags(
     libfvalue_value_t *value,
     uint32_t format_flags,
     libcerror_error_t **error )
{
	libfvalue_internal_value_t *internal_value = NULL;
	static char *function                      = "libfvalue_value_set_format_flags";

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

	internal_value->format_flags = format_flags;

	return( 1 );
}

/* Value instance functions
 */

/* Retrieves the value instance
 * The value instance is created if it does not exist
 * Returns 1 if successful or -1 on error
 */
int libfvalue_value_get_value_instance_by_index(
     libfvalue_value_t *value,
     int value_entry_index,
     intptr_t **value_instance,
     libcerror_error_t **error )
{
	libfvalue_internal_value_t *internal_value = NULL;
	uint8_t *value_entry_data                  = NULL;
	static char *function                      = "libfvalue_value_get_value_instance_by_index";
	size_t value_entry_data_size               = 0;
	int encoding                               = 0;
	int result                                 = 0;

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

	if( internal_value->initialize_instance == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid value - missing initialize instance function.",
		 function );

		return( -1 );
	}
	if( internal_value->value_instances != NULL )
	{
		if( internal_value->free_instance == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid value - missing free instance function.",
			 function );

			return( -1 );
		}
		if( internal_value->copy_from_byte_stream == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid value - missing free copy from byte stream function.",
			 function );

			return( -1 );
		}
	}
	if( value_instance == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value instance.",
		 function );

		return( -1 );
	}
	if( *value_instance != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid value instance value already set.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_entry_by_index(
	     internal_value->value_instances,
	     value_entry_index,
	     value_instance,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve entry: %d from values instances array.",
		 function,
		 value_entry_index );

		goto on_error;
	}
	if( *value_instance == NULL )
	{
		result = libfvalue_value_get_entry_data(
		          value,
		          value_entry_index,
		          &value_entry_data,
		          &value_entry_data_size,
		          &encoding,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve entry data: %d.",
			 function,
			 value_entry_index );

			goto on_error;
		}
		else if( result != 0 )
		{
			if( internal_value->initialize_instance(
			     value_instance,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create value instance.",
				 function );

				goto on_error;
			}
			if( internal_value->copy_from_byte_stream(
			     *value_instance,
			     value_entry_data,
			     value_entry_data_size,
			     encoding,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to copy value instance from byte stream.",
				 function );

				goto on_error;
			}
		}
		else
		{
			*value_instance = NULL;
		}
		if( libcdata_array_set_entry_by_index(
		     internal_value->value_instances,
		     value_entry_index,
		     *value_instance,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set entry: %d in values instances array.",
			 function,
			 value_entry_index );

			goto on_error;
		}
	}
	return( 1 );

on_error:
	if( *value_instance != NULL )
	{
		internal_value->free_instance(
		 value_instance,
		 NULL );
	}
	return( -1 );
}

/* Value entry functions
 */

/* Retrieves the number of values entries
 * Returns 1 if successful or -1 on error
 */
int libfvalue_value_get_number_of_value_entries(
     libfvalue_value_t *value,
     int *number_of_value_entries,
     libcerror_error_t **error )
{
	libfvalue_internal_value_t *internal_value = NULL;
	static char *function                      = "libfvalue_value_get_number_of_value_entries";
	int safe_number_of_value_entries           = 0;
	int result                                 = 0;

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

	if( number_of_value_entries == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid number of value entries.",
		 function );

		return( -1 );
	}
	result = libfvalue_value_has_data(
		  value,
		  error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if value has data.",
		 function );

		return( -1 );
	}
	else if( result != 0 )
	{
		if( libfvalue_data_handle_get_number_of_value_entries(
		     internal_value->data_handle,
		     &safe_number_of_value_entries,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of value entries from data handle.",
			 function );

			return( -1 );
		}
	}
	else if( internal_value->value_instances != NULL )
	{
		if( libcdata_array_get_number_of_entries(
		     internal_value->value_instances,
		     &safe_number_of_value_entries,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of entries from values instances array.",
			 function );

			return( -1 );
		}
	}
	if( safe_number_of_value_entries < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid number of value entries value out of bounds.",
		 function );

		return( -1 );
	}
	*number_of_value_entries = safe_number_of_value_entries;

	return( 1 );
}

/* Retrieves the entry
 * Returns 1 if successful, 0 if the value has no data or -1 on error
 */
int libfvalue_value_get_entry(
     libfvalue_value_t *value,
     int value_entry_index,
     size_t *entry_data_offset,
     size_t *entry_data_size,
     libcerror_error_t **error )
{
	libfvalue_internal_value_t *internal_value = NULL;
	static char *function                      = "libfvalue_value_get_entry";
	int result                                 = 0;

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

	if( entry_data_offset == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid entry data offset.",
		 function );

		return( -1 );
	}
	if( entry_data_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid entry data size.",
		 function );

		return( -1 );
	}
	result = libfvalue_value_has_data(
	          value,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if value has data.",
		 function );

		return( -1 );
	}
	else if( result != 0 )
	{
		if( libfvalue_data_handle_get_value_entry(
		     internal_value->data_handle,
		     value_entry_index,
		     entry_data_offset,
		     entry_data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve entry: %d from data handle.",
			 function,
			 value_entry_index );

			return( -1 );
		}
		if( *entry_data_size == 0 )
		{
			result = 0;
		}
	}
	if( result == 0 )
	{
		*entry_data_offset = 0;
		*entry_data_size   = 0;
	}
	return( result );
}

/* Sets the entry
 * Returns 1 if successful or -1 on error
 */
int libfvalue_value_set_entry(
     libfvalue_value_t *value,
     int value_entry_index,
     size_t entry_data_offset,
     size_t entry_data_size,
     libcerror_error_t **error )
{
	libfvalue_internal_value_t *internal_value = NULL;
	intptr_t *value_instance                   = NULL;
	static char *function                      = "libfvalue_value_set_entry";

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

	if( internal_value->free_instance == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid value - missing free instance function.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_entry_by_index(
	     internal_value->value_instances,
	     value_entry_index,
	     &value_instance,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve entry: %d from values instances array.",
		 function,
		 value_entry_index );

		return( -1 );
	}
	if( value_instance != NULL )
	{
		if( libcdata_array_set_entry_by_index(
		     internal_value->value_instances,
		     value_entry_index,
		     NULL,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set entry: %d in values instances array.",
			 function,
			 value_entry_index );

			return( -1 );
		}
		if( internal_value->free_instance(
		     &value_instance,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free value instance: %d.",
			 function,
			 value_entry_index );

			return( -1 );
		}
	}
	if( libfvalue_data_handle_set_value_entry(
	     internal_value->data_handle,
	     value_entry_index,
	     entry_data_offset,
	     entry_data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set entry: %d in data handle.",
		 function,
		 value_entry_index );

		return( -1 );
	}
	return( 1 );
}

/* Appends the entry
 * Returns 1 if successful or -1 on error
 */
int libfvalue_value_append_entry(
     libfvalue_value_t *value,
     int *value_entry_index,
     size_t entry_data_offset,
     size_t entry_data_size,
     libcerror_error_t **error )
{
	libfvalue_internal_value_t *internal_value = NULL;
	intptr_t *value_instance                   = NULL;
	static char *function                      = "libfvalue_value_append_entry";
	int entry_index                            = 0;
	int number_of_value_entries                = 0;

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

	if( internal_value->free_instance == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid value - missing free instance function.",
		 function );

		return( -1 );
	}
	if( value_entry_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value entry index.",
		 function );

		return( -1 );
	}
	if( libfvalue_data_handle_get_number_of_value_entries(
	     internal_value->data_handle,
	     &number_of_value_entries,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of value entries from data handle.",
		 function );

		return( -1 );
	}
	/* Since the data handle can reallocate the buffer used to store the data
	 * and the value instances reference this data make sure that no stale
	 * value instances are kept around.
	 */
	for( entry_index = 0;
	     entry_index < number_of_value_entries;
	     entry_index += 1 )
	{
		if( libcdata_array_get_entry_by_index(
		     internal_value->value_instances,
		     entry_index,
		     &value_instance,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve entry: %d from values instances array.",
			 function,
			 entry_index );

			return( -1 );
		}
		if( value_instance != NULL )
		{
			if( libcdata_array_set_entry_by_index(
			     internal_value->value_instances,
			     entry_index,
			     NULL,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set entry: %d in values instances array.",
				 function,
				 entry_index );

				return( -1 );
			}
			if( internal_value->free_instance(
			     &value_instance,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free value instance: %d.",
				 function,
				 entry_index );

				return( -1 );
			}
		}
	}
	if( libcdata_array_resize(
	     internal_value->value_instances,
	     number_of_value_entries + 1,
	     internal_value->free_instance,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to resize value instances array.",
		 function );

		return( -1 );
	}
	if( libfvalue_data_handle_append_value_entry(
	     internal_value->data_handle,
	     value_entry_index,
	     entry_data_offset,
	     entry_data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append entry to data handle.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the entry data
 * Returns 1 if successful, 0 if the value has no data or -1 on error
 */
int libfvalue_value_get_entry_data(
     libfvalue_value_t *value,
     int value_entry_index,
     uint8_t **entry_data,
     size_t *entry_data_size,
     int *encoding,
     libcerror_error_t **error )
{
	libfvalue_internal_value_t *internal_value = NULL;
	static char *function                      = "libfvalue_value_get_entry_data";
	int result                                 = 0;

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

	if( entry_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid entry data.",
		 function );

		return( -1 );
	}
	if( entry_data_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid entry data size.",
		 function );

		return( -1 );
	}
	if( encoding == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid encoding.",
		 function );

		return( -1 );
	}
	result = libfvalue_value_has_data(
	          value,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if value has data.",
		 function );

		return( -1 );
	}
	else if( result != 0 )
	{
		if( libfvalue_data_handle_get_value_entry_data(
		     internal_value->data_handle,
		     value_entry_index,
		     entry_data,
		     entry_data_size,
		     encoding,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve entry: %d data from data handle.",
			 function,
			 value_entry_index );

			return( -1 );
		}
		if( ( *entry_data == NULL )
		 || ( *entry_data_size == 0 ) )
		{
			result = 0;
		}
	}
	if( result == 0 )
	{
		*entry_data      = NULL;
		*entry_data_size = 0;
	}
	return( result );
}

/* Sets the entry data
 * Returns 1 if successful or -1 on error
 */
int libfvalue_value_set_entry_data(
     libfvalue_value_t *value,
     int value_entry_index,
     const uint8_t *entry_data,
     size_t entry_data_size,
     int encoding,
     libcerror_error_t **error )
{
	libfvalue_internal_value_t *internal_value = NULL;
	intptr_t *value_instance                   = NULL;
	static char *function                      = "libfvalue_value_set_entry_data";

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

	if( internal_value->free_instance == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid value - missing free instance function.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_entry_by_index(
	     internal_value->value_instances,
	     value_entry_index,
	     &value_instance,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve entry: %d from values instances array.",
		 function,
		 value_entry_index );

		return( -1 );
	}
	if( value_instance != NULL )
	{
		if( libcdata_array_set_entry_by_index(
		     internal_value->value_instances,
		     value_entry_index,
		     NULL,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set entry: %d data in values instances array.",
			 function,
			 value_entry_index );

			return( -1 );
		}
		if( internal_value->free_instance(
		     &value_instance,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free value instance: %d.",
			 function,
			 value_entry_index );

			return( -1 );
		}
	}
	if( libfvalue_data_handle_set_value_entry_data(
	     internal_value->data_handle,
	     value_entry_index,
	     entry_data,
	     entry_data_size,
	     encoding,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set entry: %d in data handle.",
		 function,
		 value_entry_index );

		return( -1 );
	}
	return( 1 );
}

/* Appends the entry data
 * Returns 1 if successful or -1 on error
 */
int libfvalue_value_append_entry_data(
     libfvalue_value_t *value,
     int *value_entry_index,
     const uint8_t *entry_data,
     size_t entry_data_size,
     int encoding,
     libcerror_error_t **error )
{
	libfvalue_internal_value_t *internal_value = NULL;
	intptr_t *value_instance                   = NULL;
	static char *function                      = "libfvalue_value_append_entry_data";
	int number_of_value_entries                = 0;

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

	if( internal_value->free_instance == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid value - missing free instance function.",
		 function );

		return( -1 );
	}
	if( value_entry_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value entry index.",
		 function );

		return( -1 );
	}
	if( libfvalue_data_handle_get_number_of_value_entries(
	     internal_value->data_handle,
	     &number_of_value_entries,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of value entries from data handle.",
		 function );

		return( -1 );
	}
	/* Since the data handle can reallocate the buffer used to store the data
	 * and the value instances reference this data make sure that no stale
	 * value instances are kept around.
	 */
	for( *value_entry_index = 0;
	     *value_entry_index < number_of_value_entries;
	     *value_entry_index += 1 )
	{
		if( libcdata_array_get_entry_by_index(
		     internal_value->value_instances,
		     *value_entry_index,
		     &value_instance,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve entry: %d from values instances array.",
			 function,
			 *value_entry_index );

			return( -1 );
		}
		if( value_instance != NULL )
		{
			if( libcdata_array_set_entry_by_index(
			     internal_value->value_instances,
			     *value_entry_index,
			     NULL,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set entry: %d in values instances array.",
				 function,
				 *value_entry_index );

				return( -1 );
			}
			if( internal_value->free_instance(
			     &value_instance,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free value instance: %d.",
				 function,
				 *value_entry_index );

				return( -1 );
			}
		}
	}
	if( libcdata_array_resize(
	     internal_value->value_instances,
	     number_of_value_entries + 1,
	     internal_value->free_instance,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to resize value instances array.",
		 function );

		return( -1 );
	}
	if( libfvalue_data_handle_append_value_entry_data(
	     internal_value->data_handle,
	     value_entry_index,
	     entry_data,
	     entry_data_size,
	     encoding,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append entry to data handle.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Copies the entry data
 * Returns 1 if successful, 0 if the value has no data or -1 on error
 */
int libfvalue_value_copy_entry_data(
     libfvalue_value_t *value,
     int value_entry_index,
     uint8_t *entry_data,
     size_t entry_data_size,
     int *encoding,
     libcerror_error_t **error )
{
	libfvalue_internal_value_t *internal_value = NULL;
	uint8_t *value_entry_data                  = NULL;
	static char *function                      = "libfvalue_value_copy_entry_data";
	size_t value_entry_data_size               = 0;
	int result                                 = 0;

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

	if( entry_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid entry data.",
		 function );

		return( -1 );
	}
	if( entry_data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid entry data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	result = libfvalue_value_has_data(
	          value,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if value has data.",
		 function );

		return( -1 );
	}
	else if( result != 0 )
	{
		if( libfvalue_data_handle_get_value_entry_data(
		     internal_value->data_handle,
		     value_entry_index,
		     &value_entry_data,
		     &value_entry_data_size,
		     encoding,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve entry: %d data from data handle.",
			 function,
			 value_entry_index );

			return( -1 );
		}
		if( ( value_entry_data == NULL )
		 || ( value_entry_data_size == 0 ) )
		{
			result = 0;
		}
	}
	if( result != 0 )
	{
		if( entry_data_size < value_entry_data_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid entry: %d data size value out of bounds.",
			 function,
			 value_entry_index );

			return( -1 );
		}
		if( memory_copy(
		     entry_data,
		     value_entry_data,
		     value_entry_data_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy entry: %d data.",
			 function,
			 value_entry_index );

			return( -1 );
		}
	}
	return( result );
}

/* Boolean value functions
 */

/* Copies the value data from a boolean value
 * Returns 1 if successful, 0 if value could not be set or -1 on error
 */
int libfvalue_value_copy_from_boolean(
     libfvalue_value_t *value,
     int value_entry_index,
     uint8_t value_boolean,
     libcerror_error_t **error )
{
	libfvalue_internal_value_t *internal_value = NULL;
	intptr_t *value_instance                   = NULL;
	static char *function                      = "libfvalue_value_copy_from_boolean";
	int result                                 = 0;

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

	if( internal_value->copy_from_integer != NULL )
	{
		if( libfvalue_value_get_value_instance_by_index(
		     value,
		     value_entry_index,
		     &value_instance,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value instance: %d.",
			 function,
			 value_entry_index );

			return( -1 );
		}
		if( value_instance != NULL )
		{
			if( internal_value->copy_from_integer(
			     value_instance,
			     (uint64_t) value_boolean,
			     1,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy instance from boolean.",
				 function );

				return( -1 );
			}
			result = 1;
		}
	}
	return( result );
}

/* Copies the value data to a boolean value
 * Returns 1 if successful, 0 if value could not be set or -1 on error
 */
int libfvalue_value_copy_to_boolean(
     libfvalue_value_t *value,
     int value_entry_index,
     uint8_t *value_boolean,
     libcerror_error_t **error )
{
	libfvalue_internal_value_t *internal_value = NULL;
	intptr_t *value_instance                   = NULL;
	static char *function                      = "libfvalue_value_copy_to_boolean";
	size_t integer_value_size                  = 0;
	uint64_t integer_value                     = 0;
	int result                                 = 0;

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

	if( value_boolean == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing value boolean.",
		 function );

		return( -1 );
	}
	if( internal_value->copy_to_integer != NULL )
	{
		if( libfvalue_value_get_value_instance_by_index(
		     value,
		     value_entry_index,
		     &value_instance,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value instance: %d.",
			 function,
			 value_entry_index );

			return( -1 );
		}
		if( value_instance != NULL )
		{
			if( internal_value->copy_to_integer(
			     value_instance,
			     &integer_value,
			     &integer_value_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy instance to boolean.",
				 function );

				return( -1 );
			}
			if( integer_value != 0 )
			{
				*value_boolean = 1;
			}
			else
			{
				*value_boolean = 0;
			}
			result = 1;
		}
	}
	return( result );
}

/* Integer value functions
 */

/* Copies the value data from an 8-bit value
 * Returns 1 if successful, 0 if value could not be set or -1 on error
 */
int libfvalue_value_copy_from_8bit(
     libfvalue_value_t *value,
     int value_entry_index,
     uint8_t value_8bit,
     libcerror_error_t **error )
{
	libfvalue_internal_value_t *internal_value = NULL;
	intptr_t *value_instance                   = NULL;
	static char *function                      = "libfvalue_value_copy_from_8bit";
	int result                                 = 0;

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

	if( internal_value->copy_from_integer != NULL )
	{
		if( libfvalue_value_get_value_instance_by_index(
		     value,
		     value_entry_index,
		     &value_instance,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value instance: %d.",
			 function,
			 value_entry_index );

			return( -1 );
		}
		if( value_instance != NULL )
		{
			if( internal_value->copy_from_integer(
			     value_instance,
			     (uint64_t) value_8bit,
			     8,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy instance from 8-bit integer.",
				 function );

				return( -1 );
			}
			result = 1;
		}
	}
	return( result );
}

/* Copies the value data to an 8-bit value
 * Returns 1 if successful, 0 if value could not be set or -1 on error
 */
int libfvalue_value_copy_to_8bit(
     libfvalue_value_t *value,
     int value_entry_index,
     uint8_t *value_8bit,
     libcerror_error_t **error )
{
	libfvalue_internal_value_t *internal_value = NULL;
	intptr_t *value_instance                   = NULL;
	static char *function                      = "libfvalue_value_copy_to_8bit";
	size_t integer_value_size                  = 0;
	uint64_t integer_value                     = 0;
	int result                                 = 0;

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

	if( value_8bit == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing value 8-bit.",
		 function );

		return( -1 );
	}
	if( internal_value->copy_to_integer != NULL )
	{
		if( libfvalue_value_get_value_instance_by_index(
		     value,
		     value_entry_index,
		     &value_instance,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value instance: %d.",
			 function,
			 value_entry_index );

			return( -1 );
		}
		if( value_instance != NULL )
		{
			if( internal_value->copy_to_integer(
			     value_instance,
			     &integer_value,
			     &integer_value_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy instance to integer value.",
				 function );

				return( -1 );
			}
			if( integer_value > (uint64_t) UINT8_MAX )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: integer value out of bounds.",
				 function );

				return( -1 );
			}
			*value_8bit = (uint8_t) integer_value;

			result = 1;
		}
	}
	return( result );
}

/* Copies the value data from a 16-bit value
 * Returns 1 if successful, 0 if value could not be set or -1 on error
 */
int libfvalue_value_copy_from_16bit(
     libfvalue_value_t *value,
     int value_entry_index,
     uint16_t value_16bit,
     libcerror_error_t **error )
{
	libfvalue_internal_value_t *internal_value = NULL;
	intptr_t *value_instance                   = NULL;
	static char *function                      = "libfvalue_value_copy_from_16bit";
	int result                                 = 0;

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

	if( internal_value->copy_from_integer != NULL )
	{
		if( libfvalue_value_get_value_instance_by_index(
		     value,
		     value_entry_index,
		     &value_instance,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value instance: %d.",
			 function,
			 value_entry_index );

			return( -1 );
		}
		if( value_instance != NULL )
		{
			if( internal_value->copy_from_integer(
			     value_instance,
			     (uint64_t) value_16bit,
			     16,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy instance from 16-bit integer.",
				 function );

				return( -1 );
			}
			result = 1;
		}
	}
	return( result );
}

/* Copies the value data to a 16-bit value
 * Returns 1 if successful, 0 if value could not be set or -1 on error
 */
int libfvalue_value_copy_to_16bit(
     libfvalue_value_t *value,
     int value_entry_index,
     uint16_t *value_16bit,
     libcerror_error_t **error )
{
	libfvalue_internal_value_t *internal_value = NULL;
	intptr_t *value_instance                   = NULL;
	static char *function                      = "libfvalue_value_copy_to_16bit";
	size_t integer_value_size                  = 0;
	uint64_t integer_value                     = 0;
	int result                                 = 0;

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

	if( value_16bit == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing value 16-bit.",
		 function );

		return( -1 );
	}
	if( internal_value->copy_to_integer != NULL )
	{
		if( libfvalue_value_get_value_instance_by_index(
		     value,
		     value_entry_index,
		     &value_instance,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value instance: %d.",
			 function,
			 value_entry_index );

			return( -1 );
		}
		if( value_instance != NULL )
		{
			if( internal_value->copy_to_integer(
			     value_instance,
			     &integer_value,
			     &integer_value_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy instance to integer value.",
				 function );

				return( -1 );
			}
			if( integer_value > (uint64_t) UINT16_MAX )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: integer value out of bounds.",
				 function );

				return( -1 );
			}
			*value_16bit = (uint16_t) integer_value;

			result = 1;
		}
	}
	return( result );
}

/* Copies the value data from a 32-bit value
 * Returns 1 if successful, 0 if value could not be set or -1 on error
 */
int libfvalue_value_copy_from_32bit(
     libfvalue_value_t *value,
     int value_entry_index,
     uint32_t value_32bit,
     libcerror_error_t **error )
{
	libfvalue_internal_value_t *internal_value = NULL;
	intptr_t *value_instance                   = NULL;
	static char *function                      = "libfvalue_value_copy_from_32bit";
	int result                                 = 0;

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

	if( internal_value->copy_from_integer != NULL )
	{
		if( libfvalue_value_get_value_instance_by_index(
		     value,
		     value_entry_index,
		     &value_instance,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value instance: %d.",
			 function,
			 value_entry_index );

			return( -1 );
		}
		if( value_instance != NULL )
		{
			if( internal_value->copy_from_integer(
			     value_instance,
			     (uint64_t) value_32bit,
			     32,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy instance from 32-bit integer.",
				 function );

				return( -1 );
			}
			result = 1;
		}
	}
	return( result );
}

/* Copies the value data to a 32-bit value
 * Returns 1 if successful, 0 if value could not be set or -1 on error
 */
int libfvalue_value_copy_to_32bit(
     libfvalue_value_t *value,
     int value_entry_index,
     uint32_t *value_32bit,
     libcerror_error_t **error )
{
	libfvalue_internal_value_t *internal_value = NULL;
	intptr_t *value_instance                   = NULL;
	static char *function                      = "libfvalue_value_copy_to_32bit";
	size_t integer_value_size                  = 0;
	uint64_t integer_value                     = 0;
	int result                                 = 0;

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

	if( value_32bit == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing value 32-bit.",
		 function );

		return( -1 );
	}
	if( internal_value->copy_to_integer != NULL )
	{
		if( libfvalue_value_get_value_instance_by_index(
		     value,
		     value_entry_index,
		     &value_instance,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value instance: %d.",
			 function,
			 value_entry_index );

			return( -1 );
		}
		if( value_instance != NULL )
		{
			if( internal_value->copy_to_integer(
			     value_instance,
			     &integer_value,
			     &integer_value_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy instance to integer value.",
				 function );

				return( -1 );
			}
			if( integer_value > (uint64_t) UINT32_MAX )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: integer value out of bounds.",
				 function );

				return( -1 );
			}
			*value_32bit = (uint32_t) integer_value;

			result = 1;
		}
	}
	return( result );
}

/* Copies the value data from a 64-bit value
 * Returns 1 if successful, 0 if value could not be set or -1 on error
 */
int libfvalue_value_copy_from_64bit(
     libfvalue_value_t *value,
     int value_entry_index,
     uint64_t value_64bit,
     libcerror_error_t **error )
{
	libfvalue_internal_value_t *internal_value = NULL;
	intptr_t *value_instance                   = NULL;
	static char *function                      = "libfvalue_value_copy_from_64bit";
	int result                                 = 0;

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

	if( internal_value->copy_from_integer != NULL )
	{
		if( libfvalue_value_get_value_instance_by_index(
		     value,
		     value_entry_index,
		     &value_instance,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value instance: %d.",
			 function,
			 value_entry_index );

			return( -1 );
		}
		if( value_instance != NULL )
		{
			if( internal_value->copy_from_integer(
			     value_instance,
			     (uint64_t) value_64bit,
			     64,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy instance from 64-bit integer.",
				 function );

				return( -1 );
			}
			result = 1;
		}
	}
	return( result );
}

/* Copies the value data to a 64-bit value
 * Returns 1 if successful, 0 if value could not be set or -1 on error
 */
int libfvalue_value_copy_to_64bit(
     libfvalue_value_t *value,
     int value_entry_index,
     uint64_t *value_64bit,
     libcerror_error_t **error )
{
	libfvalue_internal_value_t *internal_value = NULL;
	intptr_t *value_instance                   = NULL;
	static char *function                      = "libfvalue_value_copy_to_64bit";
	size_t integer_value_size                  = 0;
	uint64_t integer_value                     = 0;
	int result                                 = 0;

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

	if( value_64bit == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing value 64-bit.",
		 function );

		return( -1 );
	}
	if( internal_value->copy_to_integer != NULL )
	{
		if( libfvalue_value_get_value_instance_by_index(
		     value,
		     value_entry_index,
		     &value_instance,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value instance: %d.",
			 function,
			 value_entry_index );

			return( -1 );
		}
		if( value_instance != NULL )
		{
			if( internal_value->copy_to_integer(
			     value_instance,
			     &integer_value,
			     &integer_value_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy instance to integer value.",
				 function );

				return( -1 );
			}
			*value_64bit = (uint64_t) integer_value;

			result = 1;
		}
	}
	return( result );
}

/* Floating point value functions
 */

/* Copies the value data from a float value
 * Returns 1 if successful, 0 if value could not be set or -1 on error
 */
int libfvalue_value_copy_from_float(
     libfvalue_value_t *value,
     int value_entry_index,
     float value_float,
     libcerror_error_t **error )
{
	libfvalue_internal_value_t *internal_value = NULL;
	intptr_t *value_instance                   = NULL;
	static char *function                      = "libfvalue_value_copy_from_float";
	int result                                 = 0;

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

	if( internal_value->copy_from_floating_point != NULL )
	{
		if( libfvalue_value_get_value_instance_by_index(
		     value,
		     value_entry_index,
		     &value_instance,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value instance: %d.",
			 function,
			 value_entry_index );

			return( -1 );
		}
		if( value_instance != NULL )
		{
			if( internal_value->copy_from_floating_point(
			     value_instance,
			     (double) value_float,
			     32,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy instance from 32-bit floating point.",
				 function );

				return( -1 );
			}
			result = 1;
		}
	}
	return( result );
}

/* Copies the value data to a float value
 * Returns 1 if successful, 0 if value could not be set or -1 on error
 */
int libfvalue_value_copy_to_float(
     libfvalue_value_t *value,
     int value_entry_index,
     float *value_float,
     libcerror_error_t **error )
{
	libfvalue_internal_value_t *internal_value = NULL;
	intptr_t *value_instance                   = NULL;
	static char *function                      = "libfvalue_value_copy_to_float";
	size_t floating_point_value_size           = 0;
	double floating_point_value                = 0.0;
	int result                                 = 0;

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

	if( value_float == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value float.",
		 function );

		return( -1 );
	}
	if( internal_value->copy_to_floating_point != NULL )
	{
		if( libfvalue_value_get_value_instance_by_index(
		     value,
		     value_entry_index,
		     &value_instance,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value instance: %d.",
			 function,
			 value_entry_index );

			return( -1 );
		}
		if( value_instance != NULL )
		{
			if( internal_value->copy_to_floating_point(
			     value_instance,
			     &floating_point_value,
			     &floating_point_value_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy instance to floating point value.",
				 function );

				return( -1 );
			}
			*value_float = (float) floating_point_value;

			result = 1;
		}
	}
	return( result );
}

/* Copies the value data from a double value
 * Returns 1 if successful, 0 if value could not be set or -1 on error
 */
int libfvalue_value_copy_from_double(
     libfvalue_value_t *value,
     int value_entry_index,
     double value_double,
     libcerror_error_t **error )
{
	libfvalue_internal_value_t *internal_value = NULL;
	intptr_t *value_instance                   = NULL;
	static char *function                      = "libfvalue_value_copy_from_double";
	int result                                 = 0;

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

	if( internal_value->copy_from_floating_point != NULL )
	{
		if( libfvalue_value_get_value_instance_by_index(
		     value,
		     value_entry_index,
		     &value_instance,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value instance: %d.",
			 function,
			 value_entry_index );

			return( -1 );
		}
		if( value_instance != NULL )
		{
			if( internal_value->copy_from_floating_point(
			     value_instance,
			     (double) value_double,
			     64,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy instance from 32-bit floating point.",
				 function );

				return( -1 );
			}
			result = 1;
		}
	}
	return( result );
}

/* Copies the value data to a double value
 * Returns 1 if successful, 0 if value could not be set or -1 on error
 */
int libfvalue_value_copy_to_double(
     libfvalue_value_t *value,
     int value_entry_index,
     double *value_double,
     libcerror_error_t **error )
{
	libfvalue_internal_value_t *internal_value = NULL;
	intptr_t *value_instance                   = NULL;
	static char *function                      = "libfvalue_value_copy_to_double";
	size_t floating_point_value_size           = 0;
	double floating_point_value                = 0.0;
	int result                                 = 0;

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

	if( value_double == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value double.",
		 function );

		return( -1 );
	}
	if( internal_value->copy_to_floating_point != NULL )
	{
		if( libfvalue_value_get_value_instance_by_index(
		     value,
		     value_entry_index,
		     &value_instance,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value instance: %d.",
			 function,
			 value_entry_index );

			return( -1 );
		}
		if( value_instance != NULL )
		{
			if( internal_value->copy_to_floating_point(
			     value_instance,
			     &floating_point_value,
			     &floating_point_value_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy instance to floating point value.",
				 function );

				return( -1 );
			}
			*value_double = (double) floating_point_value;

			result = 1;
		}
	}
	return( result );
}

/* String value functions
 */

/* Copies the value data from an UTF-8 encoded string
 * Returns 1 if successful, 0 if value could not be set or -1 on error
 */
int libfvalue_value_copy_from_utf8_string(
     libfvalue_value_t *value,
     int value_entry_index,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libcerror_error_t **error )
{
	libfvalue_internal_value_t *internal_value = NULL;
	intptr_t *value_instance                   = NULL;
	static char *function                      = "libfvalue_value_copy_from_utf8_string";
	size_t utf8_string_index                   = 0;
	int result                                 = 0;

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

	if( internal_value->initialize_instance == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid value - missing initialize instance function.",
		 function );

		return( -1 );
	}
	if( internal_value->free_instance == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid value - missing free instance function.",
		 function );

		return( -1 );
	}
	if( internal_value->copy_from_utf8_string_with_index != NULL )
	{
		if( libfvalue_value_get_value_instance_by_index(
		     value,
		     value_entry_index,
		     &value_instance,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value instance: %d.",
			 function,
			 value_entry_index );

			return( -1 );
		}
		if( value_instance == NULL )
		{
			if( internal_value->initialize_instance(
			     &value_instance,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create value instance.",
				 function );

				return( -1 );
			}
			if( libcdata_array_set_entry_by_index(
			     internal_value->value_instances,
			     value_entry_index,
			     value_instance,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set entry: %d in values instances array.",
				 function,
				 value_entry_index );

				internal_value->free_instance(
				 &value_instance,
				 NULL );

				return( -1 );
			}
		}
		if( internal_value->copy_from_utf8_string_with_index(
		     value_instance,
		     utf8_string,
		     utf8_string_length,
		     &utf8_string_index,
		     internal_value->format_flags,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy instance from UTF-8 string.",
			 function );

			return( -1 );
		}
		result = 1;
	}
	return( result );
}

/* Retrieves the size of an UTF-8 encoded string of the value data
 * Returns 1 if successful, 0 if value could not be set or -1 on error
 */
int libfvalue_value_get_utf8_string_size(
     libfvalue_value_t *value,
     int value_entry_index,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	libfvalue_internal_value_t *internal_value = NULL;
	intptr_t *value_instance                   = NULL;
	static char *function                      = "libfvalue_value_get_utf8_string_size";
	int result                                 = 0;

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

	if( internal_value->get_utf8_string_size != NULL )
	{
		if( libfvalue_value_get_value_instance_by_index(
		     value,
		     value_entry_index,
		     &value_instance,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value instance: %d.",
			 function,
			 value_entry_index );

			return( -1 );
		}
		if( value_instance != NULL )
		{
			if( internal_value->get_utf8_string_size(
			     value_instance,
			     utf8_string_size,
			     internal_value->format_flags,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to determine size UTF-8 string of instance.",
				 function );

				return( -1 );
			}
			result = 1;
		}
	}
	return( result );
}

/* Copies the value data to an UTF-8 encoded string
 * Returns 1 if successful, 0 if value could not be set or -1 on error
 */
int libfvalue_value_copy_to_utf8_string(
     libfvalue_value_t *value,
     int value_entry_index,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	static char *function    = "libfvalue_value_copy_to_utf8_string";
	size_t utf8_string_index = 0;
	int result               = 0;

	result = libfvalue_value_copy_to_utf8_string_with_index(
	          value,
	          value_entry_index,
	          utf8_string,
	          utf8_string_size,
	          &utf8_string_index,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy value: %d to UTF-8 string.",
		 function,
		 value_entry_index );

		return( -1 );
	}
	return( result );
}

/* Copies the value data to an UTF-8 encoded string
 * Returns 1 if successful, 0 if value could not be set or -1 on error
 */
int libfvalue_value_copy_to_utf8_string_with_index(
     libfvalue_value_t *value,
     int value_entry_index,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     size_t *utf8_string_index,
     libcerror_error_t **error )
{
	libfvalue_internal_value_t *internal_value = NULL;
	intptr_t *value_instance                   = NULL;
	static char *function                      = "libfvalue_value_copy_to_utf8_string_with_index";
	int result                                 = 0;

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

	if( internal_value->copy_to_utf8_string_with_index != NULL )
	{
		if( libfvalue_value_get_value_instance_by_index(
		     value,
		     value_entry_index,
		     &value_instance,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value instance: %d.",
			 function,
			 value_entry_index );

			return( -1 );
		}
		if( value_instance != NULL )
		{
			if( internal_value->copy_to_utf8_string_with_index(
			     value_instance,
			     utf8_string,
			     utf8_string_size,
			     utf8_string_index,
			     internal_value->format_flags,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy instance to UTF-8 string.",
				 function );

				return( -1 );
			}
			result = 1;
		}
	}
	return( result );
}

/* Copies the value data from an UTF-16 encoded string
 * Returns 1 if successful, 0 if value could not be set or -1 on error
 */
int libfvalue_value_copy_from_utf16_string(
     libfvalue_value_t *value,
     int value_entry_index,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libcerror_error_t **error )
{
	libfvalue_internal_value_t *internal_value = NULL;
	intptr_t *value_instance                   = NULL;
	static char *function                      = "libfvalue_value_copy_from_utf16_string";
	size_t utf16_string_index                  = 0;
	int result                                 = 0;

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

	if( internal_value->initialize_instance == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid value - missing initialize instance function.",
		 function );

		return( -1 );
	}
	if( internal_value->free_instance == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid value - missing free instance function.",
		 function );

		return( -1 );
	}
	if( internal_value->copy_from_utf16_string_with_index != NULL )
	{
		if( libfvalue_value_get_value_instance_by_index(
		     value,
		     value_entry_index,
		     &value_instance,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value instance: %d.",
			 function,
			 value_entry_index );

			return( -1 );
		}
		if( value_instance == NULL )
		{
			if( internal_value->initialize_instance(
			     &value_instance,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create value instance.",
				 function );

				return( -1 );
			}
			if( libcdata_array_set_entry_by_index(
			     internal_value->value_instances,
			     value_entry_index,
			     value_instance,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set entry: %d in values instances array.",
				 function,
				 value_entry_index );

				internal_value->free_instance(
				 &value_instance,
				 NULL );

				return( -1 );
			}
		}
		if( internal_value->copy_from_utf16_string_with_index(
		     value_instance,
		     utf16_string,
		     utf16_string_length,
		     &utf16_string_index,
		     internal_value->format_flags,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy instance from UTF-16 string.",
			 function );

			return( -1 );
		}
		result = 1;
	}
	return( result );
}

/* Retrieves the size of an UTF-16 encoded string of the value data
 * Returns 1 if successful, 0 if value could not be set or -1 on error
 */
int libfvalue_value_get_utf16_string_size(
     libfvalue_value_t *value,
     int value_entry_index,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	libfvalue_internal_value_t *internal_value = NULL;
	intptr_t *value_instance                   = NULL;
	static char *function                      = "libfvalue_value_get_utf16_string_size";
	int result                                 = 0;

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

	if( internal_value->get_utf16_string_size != NULL )
	{
		if( libfvalue_value_get_value_instance_by_index(
		     value,
		     value_entry_index,
		     &value_instance,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value instance: %d.",
			 function,
			 value_entry_index );

			return( -1 );
		}
		if( value_instance != NULL )
		{
			if( internal_value->get_utf16_string_size(
			     value_instance,
			     utf16_string_size,
			     internal_value->format_flags,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to determine size UTF-16 string of instance.",
				 function );

				return( -1 );
			}
			result = 1;
		}
	}
	return( result );
}

/* Copies the value data to an UTF-16 encoded string
 * Returns 1 if successful, 0 if value could not be set or -1 on error
 */
int libfvalue_value_copy_to_utf16_string(
     libfvalue_value_t *value,
     int value_entry_index,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	static char *function     = "libfvalue_value_copy_to_utf16_string";
	size_t utf16_string_index = 0;
	int result                = 0;

	result = libfvalue_value_copy_to_utf16_string_with_index(
	          value,
	          value_entry_index,
	          utf16_string,
	          utf16_string_size,
	          &utf16_string_index,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy value: %d to UTF-16 string.",
		 function,
		 value_entry_index );

		return( -1 );
	}
	return( result );
}

/* Copies the value data to an UTF-16 encoded string
 * Returns 1 if successful, 0 if value could not be set or -1 on error
 */
int libfvalue_value_copy_to_utf16_string_with_index(
     libfvalue_value_t *value,
     int value_entry_index,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     size_t *utf16_string_index,
     libcerror_error_t **error )
{
	libfvalue_internal_value_t *internal_value = NULL;
	intptr_t *value_instance                   = NULL;
	static char *function                      = "libfvalue_value_copy_to_utf16_string_with_index";
	int result                                 = 0;

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

	if( internal_value->copy_to_utf16_string_with_index != NULL )
	{
		if( libfvalue_value_get_value_instance_by_index(
		     value,
		     value_entry_index,
		     &value_instance,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value instance: %d.",
			 function,
			 value_entry_index );

			return( -1 );
		}
		if( value_instance != NULL )
		{
			if( internal_value->copy_to_utf16_string_with_index(
			     value_instance,
			     utf16_string,
			     utf16_string_size,
			     utf16_string_index,
			     internal_value->format_flags,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy instance to UTF-16 string.",
				 function );

				return( -1 );
			}
			result = 1;
		}
	}
	return( result );
}

/* Copies the value data from an UTF-32 encoded string
 * Returns 1 if successful, 0 if value could not be set or -1 on error
 */
int libfvalue_value_copy_from_utf32_string(
     libfvalue_value_t *value,
     int value_entry_index,
     const uint32_t *utf32_string,
     size_t utf32_string_length,
     libcerror_error_t **error )
{
	libfvalue_internal_value_t *internal_value = NULL;
	intptr_t *value_instance                   = NULL;
	static char *function                      = "libfvalue_value_copy_from_utf32_string";
	size_t utf32_string_index                  = 0;
	int result                                 = 0;

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

	if( internal_value->initialize_instance == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid value - missing initialize instance function.",
		 function );

		return( -1 );
	}
	if( internal_value->free_instance == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid value - missing free instance function.",
		 function );

		return( -1 );
	}
	internal_value = (libfvalue_internal_value_t *) value;

	if( internal_value->copy_from_utf32_string_with_index != NULL )
	{
		if( libfvalue_value_get_value_instance_by_index(
		     value,
		     value_entry_index,
		     &value_instance,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value instance: %d.",
			 function,
			 value_entry_index );

			return( -1 );
		}
		if( value_instance == NULL )
		{
			if( internal_value->initialize_instance(
			     &value_instance,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create value instance.",
				 function );

				return( -1 );
			}
			if( libcdata_array_set_entry_by_index(
			     internal_value->value_instances,
			     value_entry_index,
			     value_instance,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set entry: %d in values instances array.",
				 function,
				 value_entry_index );

				internal_value->free_instance(
				 &value_instance,
				 NULL );

				return( -1 );
			}
		}
		if( internal_value->copy_from_utf32_string_with_index(
		     value_instance,
		     utf32_string,
		     utf32_string_length,
		     &utf32_string_index,
		     internal_value->format_flags,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy instance from UTF-32 string.",
			 function );

			return( -1 );
		}
		result = 1;
	}
	return( result );
}

/* Retrieves the size of an UTF-32 encoded string of the value data
 * Returns 1 if successful, 0 if value could not be set or -1 on error
 */
int libfvalue_value_get_utf32_string_size(
     libfvalue_value_t *value,
     int value_entry_index,
     size_t *utf32_string_size,
     libcerror_error_t **error )
{
	libfvalue_internal_value_t *internal_value = NULL;
	intptr_t *value_instance                   = NULL;
	static char *function                      = "libfvalue_value_get_utf32_string_size";
	int result                                 = 0;

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

	if( internal_value->get_utf32_string_size != NULL )
	{
		if( libfvalue_value_get_value_instance_by_index(
		     value,
		     value_entry_index,
		     &value_instance,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value instance: %d.",
			 function,
			 value_entry_index );

			return( -1 );
		}
		if( value_instance != NULL )
		{
			if( internal_value->get_utf32_string_size(
			     value_instance,
			     utf32_string_size,
			     internal_value->format_flags,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to determine size UTF-32 string of instance.",
				 function );

				return( -1 );
			}
			result = 1;
		}
	}
	return( result );
}

/* Copies the value data to an UTF-32 encoded string
 * Returns 1 if successful, 0 if value could not be set or -1 on error
 */
int libfvalue_value_copy_to_utf32_string(
     libfvalue_value_t *value,
     int value_entry_index,
     uint32_t *utf32_string,
     size_t utf32_string_size,
     libcerror_error_t **error )
{
	static char *function     = "libfvalue_value_copy_to_utf32_string";
	size_t utf32_string_index = 0;
	int result                = 0;

	result = libfvalue_value_copy_to_utf32_string_with_index(
	          value,
	          value_entry_index,
	          utf32_string,
	          utf32_string_size,
	          &utf32_string_index,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy value: %d to UTF-32 string.",
		 function,
		 value_entry_index );

		return( -1 );
	}
	return( result );
}

/* Copies the value data to an UTF-32 encoded string
 * Returns 1 if successful, 0 if value could not be set or -1 on error
 */
int libfvalue_value_copy_to_utf32_string_with_index(
     libfvalue_value_t *value,
     int value_entry_index,
     uint32_t *utf32_string,
     size_t utf32_string_size,
     size_t *utf32_string_index,
     libcerror_error_t **error )
{
	libfvalue_internal_value_t *internal_value = NULL;
	intptr_t *value_instance                   = NULL;
	static char *function                      = "libfvalue_value_copy_to_utf32_string_with_index";
	int result                                 = 0;

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

	if( internal_value->copy_to_utf32_string_with_index != NULL )
	{
		if( libfvalue_value_get_value_instance_by_index(
		     value,
		     value_entry_index,
		     &value_instance,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value instance: %d.",
			 function,
			 value_entry_index );

			return( -1 );
		}
		if( value_instance != NULL )
		{
			if( internal_value->copy_to_utf32_string_with_index(
			     value_instance,
			     utf32_string,
			     utf32_string_size,
			     utf32_string_index,
			     internal_value->format_flags,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy instance to UTF-32 string.",
				 function );

				return( -1 );
			}
			result = 1;
		}
	}
	return( result );
}

/* Marshalling functions
 */

#ifdef TODO

/* Reads the value data from a file stream
 * Returns the number of bytes read or -1 on error
 */
ssize_t libfvalue_value_read_from_file_stream(
         libfvalue_value_t *value,
         FILE *file_stream,
         libcerror_error_t **error )
{
	libfvalue_internal_value_t *internal_value = NULL;
	static char *function                      = "libfvalue_value_read_from_file_stream";

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

	if( internal_value->copy_from_utf8_string_with_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid value - missing copy from UTF-8 string function.",
		 function );

		return( -1 );
	}
	if( file_stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file stream.",
		 function );

		return( -1 );
	}
/* TODO implement function */
	return( -1 );
}

#endif /* TODO */

/* Writes the value data to a file stream
 * Returns the number of bytes written or -1 on error
 */
ssize_t libfvalue_value_write_to_file_stream(
         libfvalue_value_t *value,
         FILE *file_stream,
         libcerror_error_t **error )
{
	libfvalue_internal_value_t *internal_value = NULL;
	intptr_t *value_instance                   = NULL;
	uint8_t *value_string                      = NULL;
	static char *function                      = "libfvalue_value_write_to_file_stream";
	size_t value_string_index                  = 0;
	size_t value_string_size                   = 0;
	ssize_t write_count                        = 0;
	int print_count                            = 0;
	int number_of_value_entries                = 0;
	int value_entry_index                      = 0;

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

	if( internal_value->type_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid value - missing type string.",
		 function );

		return( -1 );
	}
	if( ( internal_value->identifier == NULL )
	 || ( internal_value->identifier[ 0 ] == 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid value - missing identifier.",
		 function );

		return( -1 );
	}
	if( internal_value->get_utf8_string_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid value - missing get UTF-8 string size function.",
		 function );

		return( -1 );
	}
	if( internal_value->copy_to_utf8_string_with_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid value - missing copy to UTF-8 string function.",
		 function );

		return( -1 );
	}
	if( file_stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file stream.",
		 function );

		return( -1 );
	}
	if( libfvalue_value_get_number_of_value_entries(
	     value,
	     &number_of_value_entries,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of value entries.",
		 function );

		goto on_error;
	}
	for( value_entry_index = 0;
	     value_entry_index < number_of_value_entries;
	     value_entry_index++ )
	{
		if( libfvalue_value_get_value_instance_by_index(
		     value,
		     value_entry_index,
		     &value_instance,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value instance: %d.",
			 function,
			 value_entry_index );

			goto on_error;
		}
		print_count = fprintf(
			       file_stream,
			       "<%s type=\"%s\">",
			       (char *) internal_value->identifier,
			       internal_value->type_string );

		if( print_count < 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write to file stream.",
			 function );

			goto on_error;
		}
		write_count += print_count;

		if( number_of_value_entries > 1 )
		{
			print_count = fprintf(
				       file_stream,
				       "<value_entry>" );

			if( print_count < 0 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_WRITE_FAILED,
				 "%s: unable to write to file stream.",
				 function );

				goto on_error;
			}
			write_count += print_count;
		}
		if( value_instance != NULL )
		{
			if( internal_value->get_utf8_string_size(
			     value_instance,
			     &value_string_size,
			     internal_value->format_flags,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to determine size UTF-8 string of instance.",
				 function );

				goto on_error;
			}
			if( value_string_size > 0 )
			{
				value_string = (uint8_t *) memory_allocate(
							    sizeof( uint8_t ) * value_string_size );

				if( value_string == NULL )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_MEMORY,
					 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
					 "%s: unable to value string.",
					 function );

					goto on_error;
				}
				if( internal_value->copy_to_utf8_string_with_index(
				     value_instance,
				     value_string,
				     value_string_size,
				     &value_string_index,
				     internal_value->format_flags,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
					 "%s: unable to copy instance to UTF-8 string.",
					 function );

					goto on_error;
				}
				print_count = fprintf(
					       file_stream,
					       "%s",
					       value_string );

				if( print_count < 0 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_WRITE_FAILED,
					 "%s: unable to write to file stream.",
					 function );

					goto on_error;
				}
				write_count += print_count;

				memory_free(
				 value_string );

				value_string = NULL;
			}
		}
		if( number_of_value_entries > 1 )
		{
			print_count = fprintf(
				       file_stream,
				       "</value_entry>" );

			if( print_count < 0 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_WRITE_FAILED,
				 "%s: unable to write to file stream.",
				 function );

				goto on_error;
			}
			write_count += print_count;
		}
		print_count = fprintf(
			       file_stream,
			       "</%s>",
			       (char *) internal_value->identifier );

		if( print_count < 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write to file stream.",
			 function );

			goto on_error;
		}
		write_count += print_count;
	}
	return( write_count );

on_error:
	if( value_string != NULL )
	{
		memory_free(
		 value_string );
	}
	return( -1 );
}

/* Prints the value
 * Returns 1 if successful or -1 on error
 */
int libfvalue_value_print(
     libfvalue_value_t *value,
     int value_entry_index,
     uint8_t flags,
     libcerror_error_t **error )
{
	libfvalue_internal_value_t *internal_value = NULL;
	system_character_t *value_string           = NULL;
	intptr_t *value_instance                   = NULL;
	static char *function                      = "libfvalue_value_print";
	size_t value_string_index                  = 0;
	size_t value_string_size                   = 0;
	int result                                 = 0;

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

	if( internal_value->type_description == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid value - missing type description.",
		 function );

		return( -1 );
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	if( internal_value->get_utf16_string_size == NULL )
#else
	if( internal_value->get_utf8_string_size == NULL )
#endif
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid value - missing get string size function.",
		 function );

		return( -1 );
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	if( internal_value->copy_to_utf16_string_with_index == NULL )
#else
	if( internal_value->copy_to_utf8_string_with_index == NULL )
#endif
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid value - missing copy to string function.",
		 function );

		return( -1 );
	}
	if( ( flags & ~( LIBFVALUE_PRINT_FLAG_WITH_TYPE_DESCRIPTION ) ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported flags: 0x%02" PRIx8 ".",
		 function,
		 flags );

		return( -1 );
	}

	result = libfvalue_value_has_data(
	          value,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if value has data.",
		 function );

		goto on_error;
	}
	else if( result != 0 )
	{
		if( libfvalue_value_get_value_instance_by_index(
		     value,
		     value_entry_index,
		     &value_instance,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value instance: %d.",
			 function,
			 value_entry_index );

			goto on_error;
		}
		if( value_instance != NULL )
		{
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
			result = internal_value->get_utf16_string_size(
				  value_instance,
				  &value_string_size,
				  internal_value->format_flags,
				  error );
#else
			result = internal_value->get_utf8_string_size(
				  value_instance,
				  &value_string_size,
				  internal_value->format_flags,
				  error );
#endif
			if( result != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to determine size value string of instance.",
				 function );

				goto on_error;
			}
			if( value_string_size > 0 )
			{
				value_string = system_string_allocate(
				                value_string_size );

				if( value_string == NULL )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_MEMORY,
					 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
					 "%s: unable to value string.",
					 function );

					goto on_error;
				}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
				result = internal_value->copy_to_utf16_string_with_index(
					  value_instance,
					  (uint16_t *) value_string,
					  value_string_size,
					  &value_string_index,
					  internal_value->format_flags,
					  error );
#else
				result = internal_value->copy_to_utf8_string_with_index(
					  value_instance,
					  (uint8_t *) value_string,
					  value_string_size,
					  &value_string_index,
					  internal_value->format_flags,
					  error );
#endif
				if( result != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
					 "%s: unable to copy instance to value string.",
					 function );

					goto on_error;
				}

				memory_free(
				 value_string );

				value_string = NULL;
			}
		}
	}

	return( 1 );

on_error:
	if( value_string != NULL )
	{
		memory_free(
		 value_string );
	}
	return( -1 );
}

