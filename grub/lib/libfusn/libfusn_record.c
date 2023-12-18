/*
 * Record functions
 *
 * Copyright (C) 2011-2022, Joachim Metz <joachim.metz@gmail.com>
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

#include "libfusn_libfdatetime.h"
#include "libfusn_libuna.h"
#include "libfusn_record.h"

#include "fusn_record.h"

/* Creates a record
 * Make sure the value record is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfusn_record_initialize(
     libfusn_record_t **record,
     libcerror_error_t **error )
{
	libfusn_internal_record_t *internal_record = NULL;
	static char *function                      = "libfusn_record_initialize";

	if( record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	if( *record != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid record value already set.",
		 function );

		return( -1 );
	}
	internal_record = memory_allocate_structure(
			   libfusn_internal_record_t );

	if( internal_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create record.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     internal_record,
	     0,
	     sizeof( libfusn_internal_record_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear record.",
		 function );

		goto on_error;
	}
	*record = (libfusn_record_t *) internal_record;

	return( 1 );

on_error:
	if( internal_record != NULL )
	{
		memory_free(
		 internal_record );
	}
	return( -1 );
}

/* Frees a record
 * Returns 1 if successful or -1 on error
 */
int libfusn_record_free(
     libfusn_record_t **record,
     libcerror_error_t **error )
{
	libfusn_internal_record_t *internal_record = NULL;
	static char *function                      = "libfusn_record_free";

	if( record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	if( *record != NULL )
	{
		internal_record = (libfusn_internal_record_t *) *record;
		*record         = NULL;

		if( internal_record->name != NULL )
		{
			memory_free(
			 internal_record->name );
		}
		memory_free(
		 internal_record );
	}
	return( 1 );
}

/* Copies the record from the byte stream
 * Returns 1 if successful or -1 on error
 */
int libfusn_record_copy_from_byte_stream(
     libfusn_record_t *record,
     const uint8_t *byte_stream,
     size_t byte_stream_size,
     libcerror_error_t **error )
{
	libfusn_internal_record_t *internal_record = NULL;
	static char *function                      = "libfusn_record_copy_from_byte_stream";
	size_t byte_stream_offset                  = 0;
	uint16_t name_offset                       = 0;
	uint16_t name_size                         = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint32_t value_32bit                       = 0;
#endif

	if( record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	internal_record = (libfusn_internal_record_t *) record;

	if( internal_record->name != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid record - name value already set.",
		 function );

		return( -1 );
	}
	if( byte_stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid byte stream.",
		 function );

		return( -1 );
	}
	if( byte_stream_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid byte stream size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( byte_stream_size < sizeof( fusn_record_header_t ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: invalid byte stream value too small.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: record header data:\n",
		 function );
		libcnotify_print_data(
		 byte_stream,
		 sizeof( fusn_record_header_t ),
		 0 );
	}
#endif
	byte_stream_copy_to_uint32_little_endian(
	 ( (fusn_record_header_t *) byte_stream )->record_size,
	 internal_record->size );

	byte_stream_copy_to_uint16_little_endian(
	 ( (fusn_record_header_t *) byte_stream )->major_version,
	 internal_record->major_version );

	byte_stream_copy_to_uint16_little_endian(
	 ( (fusn_record_header_t *) byte_stream )->minor_version,
	 internal_record->minor_version );

	byte_stream_copy_to_uint64_little_endian(
	 ( (fusn_record_header_t *) byte_stream )->update_time,
	 internal_record->update_time );

	byte_stream_copy_to_uint64_little_endian(
	 ( (fusn_record_header_t *) byte_stream )->file_reference,
	 internal_record->file_reference );

	byte_stream_copy_to_uint32_little_endian(
	 ( (fusn_record_header_t *) byte_stream )->file_attribute_flags,
	 internal_record->file_attribute_flags );

	byte_stream_copy_to_uint64_little_endian(
	 ( (fusn_record_header_t *) byte_stream )->parent_file_reference,
	 internal_record->parent_file_reference );

	byte_stream_copy_to_uint64_little_endian(
	 ( (fusn_record_header_t *) byte_stream )->update_sequence_number,
	 internal_record->update_sequence_number );

	byte_stream_copy_to_uint32_little_endian(
	 ( (fusn_record_header_t *) byte_stream )->update_reason_flags,
	 internal_record->update_reason_flags );

	byte_stream_copy_to_uint32_little_endian(
	 ( (fusn_record_header_t *) byte_stream )->update_source_flags,
	 internal_record->update_source_flags );

	byte_stream_copy_to_uint16_little_endian(
	 ( (fusn_record_header_t *) byte_stream )->name_size,
	 name_size );

	byte_stream_copy_to_uint16_little_endian(
	 ( (fusn_record_header_t *) byte_stream )->name_offset,
	 name_offset );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: record size\t\t\t: %" PRIu32 "\n",
		 function,
		 internal_record->size );

		libcnotify_printf(
		 "%s: major version\t\t\t: %" PRIu16 "\n",
		 function,
		 internal_record->major_version );

		libcnotify_printf(
		 "%s: minor version\t\t\t: %" PRIu16 "\n",
		 function,
		 internal_record->minor_version );

		libcnotify_printf(
		 "%s: file reference\t\t\t: MFT entry: %" PRIu64 ", sequence: %" PRIu64 "\n",
		 function,
		 internal_record->file_reference & 0xffffffffffffUL,
		 internal_record->file_reference >> 48 );

		libcnotify_printf(
		 "%s: parent file reference\t\t: MFT entry: %" PRIu64 ", sequence: %" PRIu64 "\n",
		 function,
		 internal_record->parent_file_reference & 0xffffffffffffUL,
		 internal_record->parent_file_reference >> 48 );

		libcnotify_printf(
		 "%s: update sequence number\t\t: 0x%08" PRIx64 "\n",
		 function,
		 internal_record->update_sequence_number );

		if( libfusn_debug_print_filetime_value(
		     function,
		     "update time\t\t\t",
		     ( (fusn_record_header_t *) byte_stream )->update_time,
		     8,
		     LIBFDATETIME_ENDIAN_LITTLE,
		     LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME_NANO_SECONDS,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print FILETIME value.",
			 function );

			goto on_error;
		}
		libcnotify_printf(
		 "%s: update reason flags\t\t: 0x%08" PRIx32 "\n",
		 function,
		 internal_record->update_reason_flags );
		libfusn_debug_print_update_reason_flags(
		 internal_record->update_reason_flags );
		libcnotify_printf(
		 "\n" );

		libcnotify_printf(
		 "%s: update source flags\t\t: 0x%08" PRIx32 "\n",
		 function,
		 internal_record->update_source_flags );
		libfusn_debug_print_update_source_flags(
		 internal_record->update_source_flags );
		libcnotify_printf(
		 "\n" );

		byte_stream_copy_to_uint32_little_endian(
		 ( (fusn_record_header_t *) byte_stream )->security_identifier_index,
		 value_32bit );
		libcnotify_printf(
		 "%s: security identifier index\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "%s: file attribute flags\t\t: 0x%08" PRIx32 "\n",
		 function,
		 internal_record->file_attribute_flags );
		libfusn_debug_print_file_attribute_flags(
		 internal_record->file_attribute_flags );
		libcnotify_printf(
		 "\n" );

		libcnotify_printf(
		 "%s: name size\t\t\t\t: %" PRIu16 "\n",
		 function,
		 name_size );

		libcnotify_printf(
		 "%s: name offset\t\t\t: %" PRIu16 "\n",
		 function,
		 name_offset );
	}
#endif
	byte_stream_offset = sizeof( fusn_record_header_t );

	if( internal_record->major_version != 2 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported major version.",
		 function );

		goto on_error;
	}
	if( ( (size_t) internal_record->size < sizeof( fusn_record_header_t ) )
	 || ( (size_t) internal_record->size > byte_stream_size ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: record size value out of bounds.",
		 function );

		goto on_error;
	}
	if( name_offset > 0 )
	{
		if( ( (size_t) name_offset < sizeof( fusn_record_header_t ) )
		 || ( (size_t) name_offset > internal_record->size ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: name offset value out of bounds.",
			 function );

			goto on_error;
		}
		if( (size_t) name_size > ( internal_record->size - name_offset ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: name size value out of bounds.",
			 function );

			goto on_error;
		}
		if( (size_t) name_offset > byte_stream_offset )
		{
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: record header trailing data:\n",
				 function );
				libcnotify_print_data(
				 &( byte_stream[ byte_stream_offset ] ),
				 (size_t) name_offset - byte_stream_offset,
				 0 );
			}
#endif
			byte_stream_offset = (size_t) name_offset;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: name data:\n",
			 function );
			libcnotify_print_data(
			 &( byte_stream[ name_offset ] ),
			 (size_t) name_size,
			 0 );
		}
#endif
		internal_record->name = (uint8_t *) memory_allocate(
		                                     sizeof( uint8_t ) * name_size );

		if( internal_record->name == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create name.",
			 function );

			goto on_error;
		}
		internal_record->name_size = name_size;

		if( memory_copy(
		     internal_record->name,
		     &( byte_stream[ name_offset ] ),
		     name_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy name.",
			 function );

			goto on_error;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			if( libfusn_debug_print_utf16_string_value(
			     function,
			     "name\t\t\t\t\t",
			     internal_record->name,
			     internal_record->name_size,
			     LIBUNA_ENDIAN_LITTLE,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
				 "%s: unable to print UTF-16 string value.",
				 function );

				return( -1 );
			}
		}
#endif
		byte_stream_offset += (size_t) name_size;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( byte_stream_offset < (size_t) internal_record->size )
		{
			libcnotify_printf(
			 "%s: trailing data:\n",
			 function );
			libcnotify_print_data(
			 &( byte_stream[ byte_stream_offset ] ),
			 (size_t) internal_record->size - byte_stream_offset,
			 0 );
		}
		else
		{
			libcnotify_printf(
			 "\n" );
		}
	}
#endif
	return( 1 );

on_error:
	if( internal_record->name != NULL )
	{
		memory_free(
		 internal_record->name );

		internal_record->name = NULL;
	}
	internal_record->name_size = 0;

	return( -1 );
}

/* Retrieves the size
 * Returns 1 if successful or -1 on error
 */
int libfusn_record_get_size(
     libfusn_record_t *record,
     uint32_t *size,
     libcerror_error_t **error )
{
	libfusn_internal_record_t *internal_record = NULL;
	static char *function                      = "libfusn_record_get_size";

	if( record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	internal_record = (libfusn_internal_record_t *) record;

	if( size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid size.",
		 function );

		return( -1 );
	}
	*size = internal_record->size;

	return( 1 );
}

/* Retrieves the update time
 * Returns 1 if successful or -1 on error
 */
int libfusn_record_get_update_time(
     libfusn_record_t *record,
     uint64_t *update_time,
     libcerror_error_t **error )
{
	libfusn_internal_record_t *internal_record = NULL;
	static char *function                      = "libfusn_record_get_update_time";

	if( record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	internal_record = (libfusn_internal_record_t *) record;

	if( update_time == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid update time.",
		 function );

		return( -1 );
	}
	*update_time = internal_record->update_time;

	return( 1 );
}

/* Retrieves the file reference
 * Returns 1 if successful or -1 on error
 */
int libfusn_record_get_file_reference(
     libfusn_record_t *record,
     uint64_t *file_reference,
     libcerror_error_t **error )
{
	libfusn_internal_record_t *internal_record = NULL;
	static char *function                      = "libfusn_record_get_file_reference";

	if( record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	internal_record = (libfusn_internal_record_t *) record;

	if( file_reference == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file reference.",
		 function );

		return( -1 );
	}
	*file_reference = internal_record->file_reference;

	return( 1 );
}

/* Retrieves the parent file reference
 * Returns 1 if successful or -1 on error
 */
int libfusn_record_get_parent_file_reference(
     libfusn_record_t *record,
     uint64_t *parent_file_reference,
     libcerror_error_t **error )
{
	libfusn_internal_record_t *internal_record = NULL;
	static char *function                      = "libfusn_record_get_parent_file_reference";

	if( record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	internal_record = (libfusn_internal_record_t *) record;

	if( parent_file_reference == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid parent file reference.",
		 function );

		return( -1 );
	}
	*parent_file_reference = internal_record->parent_file_reference;

	return( 1 );
}

/* Retrieves the update sequence number
 * Returns 1 if successful or -1 on error
 */
int libfusn_record_get_update_sequence_number(
     libfusn_record_t *record,
     uint64_t *update_sequence_number,
     libcerror_error_t **error )
{
	libfusn_internal_record_t *internal_record = NULL;
	static char *function                      = "libfusn_record_get_update_sequence_number";

	if( record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	internal_record = (libfusn_internal_record_t *) record;

	if( update_sequence_number == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid update sequence number.",
		 function );

		return( -1 );
	}
	*update_sequence_number = internal_record->update_sequence_number;

	return( 1 );
}

/* Retrieves the update reason flags
 * Returns 1 if successful or -1 on error
 */
int libfusn_record_get_update_reason_flags(
     libfusn_record_t *record,
     uint32_t *update_reason_flags,
     libcerror_error_t **error )
{
	libfusn_internal_record_t *internal_record = NULL;
	static char *function                      = "libfusn_record_get_update_reason_flags";

	if( record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	internal_record = (libfusn_internal_record_t *) record;

	if( update_reason_flags == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid update reason flags.",
		 function );

		return( -1 );
	}
	*update_reason_flags = internal_record->update_reason_flags;

	return( 1 );
}

/* Retrieves the update source flags
 * Returns 1 if successful or -1 on error
 */
int libfusn_record_get_update_source_flags(
     libfusn_record_t *record,
     uint32_t *update_source_flags,
     libcerror_error_t **error )
{
	libfusn_internal_record_t *internal_record = NULL;
	static char *function                      = "libfusn_record_get_update_source_flags";

	if( record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	internal_record = (libfusn_internal_record_t *) record;

	if( update_source_flags == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid update source flags.",
		 function );

		return( -1 );
	}
	*update_source_flags = internal_record->update_source_flags;

	return( 1 );
}

/* Retrieves the file attribute flags
 * Returns 1 if successful or -1 on error
 */
int libfusn_record_get_file_attribute_flags(
     libfusn_record_t *record,
     uint32_t *file_attribute_flags,
     libcerror_error_t **error )
{
	libfusn_internal_record_t *internal_record = NULL;
	static char *function                      = "libfusn_record_get_file_attribute_flags";

	if( record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	internal_record = (libfusn_internal_record_t *) record;

	if( file_attribute_flags == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file attribute flags.",
		 function );

		return( -1 );
	}
	*file_attribute_flags = internal_record->file_attribute_flags;

	return( 1 );
}

/* Retrieves the size of the UTF-8 encoded name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libfusn_record_get_utf8_name_size(
     libfusn_record_t *record,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	libfusn_internal_record_t *internal_record = NULL;
	static char *function                      = "libfusn_record_get_utf8_name_size";

	if( record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	internal_record = (libfusn_internal_record_t *) record;

	if( ( internal_record->name == NULL )
	 || ( internal_record->name_size == 0 ) )
	{
		if( utf8_string_size == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid UTF-8 string size.",
			 function );

			return( -1 );
		}
		*utf8_string_size = 0;
	}
	else
	{
		if( libuna_utf8_string_size_from_utf16_stream(
		     internal_record->name,
		     (size_t) internal_record->name_size,
		     LIBUNA_ENDIAN_LITTLE,
		     utf8_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve UTF-8 string size.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Retrieves the UTF-8 encoded name
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libfusn_record_get_utf8_name(
     libfusn_record_t *record,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	libfusn_internal_record_t *internal_record = NULL;
	static char *function                      = "libfusn_record_get_utf8_name";

	if( record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	internal_record = (libfusn_internal_record_t *) record;

	if( internal_record->name == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid record - missing name.",
		 function );

		return( -1 );
	}
	if( libuna_utf8_string_copy_from_utf16_stream(
	     utf8_string,
	     utf8_string_size,
	     internal_record->name,
	     (size_t) internal_record->name_size,
	     LIBUNA_ENDIAN_LITTLE,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-8 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size of the UTF-16 encoded name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libfusn_record_get_utf16_name_size(
     libfusn_record_t *record,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	libfusn_internal_record_t *internal_record = NULL;
	static char *function                      = "libfusn_record_get_utf16_name_size";

	if( record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	internal_record = (libfusn_internal_record_t *) record;

	if( ( internal_record->name == NULL )
	 || ( internal_record->name_size == 0 ) )
	{
		if( utf16_string_size == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid UTF-16 string size.",
			 function );

			return( -1 );
		}
		*utf16_string_size = 0;
	}
	else
	{
		if( libuna_utf16_string_size_from_utf16_stream(
		     internal_record->name,
		     (size_t) internal_record->name_size,
		     LIBUNA_ENDIAN_LITTLE,
		     utf16_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve UTF-16 string size.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Retrieves the UTF-16 encoded name
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libfusn_record_get_utf16_name(
     libfusn_record_t *record,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	libfusn_internal_record_t *internal_record = NULL;
	static char *function                      = "libfusn_record_get_utf16_name";

	if( record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	internal_record = (libfusn_internal_record_t *) record;

	if( internal_record->name == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid record - missing name.",
		 function );

		return( -1 );
	}
	if( libuna_utf16_string_copy_from_utf16_stream(
	     utf16_string,
	     utf16_string_size,
	     internal_record->name,
	     (size_t) internal_record->name_size,
	     LIBUNA_ENDIAN_LITTLE,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-16 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

