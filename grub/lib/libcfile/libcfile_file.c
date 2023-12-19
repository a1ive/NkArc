/*
 * File functions
 *
 * Copyright (C) 2008-2021, Joachim Metz <joachim.metz@gmail.com>
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

#include "libcfile_definitions.h"
#include "libcfile_file.h"
#include "libcfile_system_string.h"
#include "libcfile_types.h"

/* Creates a file
 * Make sure the value file is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libcfile_file_initialize(
     libcfile_file_t **file,
     libcerror_error_t **error )
{
	libcfile_internal_file_t *internal_file = NULL;
	static char *function                   = "libcfile_file_initialize";

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	if( *file != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file value already set.",
		 function );

		return( -1 );
	}
	internal_file = memory_allocate_structure(
		         libcfile_internal_file_t );

	if( internal_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create file.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     internal_file,
	     0,
	     sizeof( libcfile_internal_file_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear file.",
		 function );

		goto on_error;
	}

	internal_file->descriptor = NULL;

	*file = (libcfile_file_t *) internal_file;

	return( 1 );

on_error:
	if( internal_file != NULL )
	{
		memory_free(
		 internal_file );
	}
	return( -1 );
}

/* Frees a file
 * Returns 1 if successful or -1 on error
 */
int libcfile_file_free(
     libcfile_file_t **file,
     libcerror_error_t **error )
{
	libcfile_internal_file_t *internal_file = NULL;
	static char *function                   = "libcfile_file_free";
	int result                              = 1;

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	if( *file != NULL )
	{
		internal_file = (libcfile_internal_file_t *) *file;

		if( internal_file->descriptor != NULL )
		{
			if( libcfile_file_close(
			     *file,
			     error ) != 0 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_CLOSE_FAILED,
				 "%s: unable to close file.",
				 function );

				result = -1;
			}
		}
		*file = NULL;

		memory_free(
		 internal_file );
	}
	return( result );
}

/* Opens a file
 * Returns 1 if successful or -1 on error
 */
int libcfile_file_open(
     libcfile_file_t *file,
     const char *filename,
     int access_flags,
     libcerror_error_t **error )
{
	static char *function = "libcfile_file_open";
	uint32_t error_code   = 0;

	if( libcfile_file_open_with_error_code(
	     file,
	     filename,
	     access_flags,
	     &error_code,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open file.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Opens a file
 * This function uses the POSIX open function or equivalent
 * Returns 1 if successful or -1 on error
 */
int libcfile_file_open_with_error_code(
     libcfile_file_t *file,
     const char *filename,
     int access_flags,
     uint32_t *error_code,
     libcerror_error_t **error )
{
	libcfile_internal_file_t *internal_file = NULL;
	static char *function                   = "libcfile_file_open_with_error_code";

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	internal_file = (libcfile_internal_file_t *) file;

	if( internal_file->descriptor != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file - descriptor value already set.",
		 function );

		return( -1 );
	}
	if( filename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filename.",
		 function );

		return( -1 );
	}

	if( error_code == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid error code.",
		 function );

		return( -1 );
	}

	grub_errno = GRUB_ERR_NONE;
	internal_file->descriptor = grub_file_open(
	                             filename,
	                             GRUB_FILE_TYPE_NO_DECOMPRESS | GRUB_FILE_TYPE_LIBYAL | GRUB_FILE_TYPE_SKIP_SIGNATURE);

	if( internal_file->descriptor == NULL )
	{
		*error_code = (uint32_t) grub_errno;
		grub_errno = GRUB_ERR_NONE;
		return( -1 );
	}
	if( libcfile_internal_file_get_size(
	     internal_file,
	     &( internal_file->size ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve size.",
		 function );

		return( -1 );
	}
	internal_file->access_flags   = access_flags;
	internal_file->current_offset = 0;

	return( 1 );
}

/* Closes the file
 * This function uses the POSIX close function or equivalent
 * Returns 0 if successful or -1 on error
 */
int libcfile_file_close(
     libcfile_file_t *file,
     libcerror_error_t **error )
{
	libcfile_internal_file_t *internal_file = NULL;
	static char *function                   = "libcfile_file_close";

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	internal_file = (libcfile_internal_file_t *) file;

	if( internal_file->descriptor != NULL )
	{
		grub_errno = GRUB_ERR_NONE;
		if( grub_file_close(
		     internal_file->descriptor ) != GRUB_ERR_NONE )
		{
			return( -1 );
		}
		internal_file->descriptor     = NULL;
		internal_file->access_flags   = 0;
		internal_file->size           = 0;
		internal_file->current_offset = 0;
	}
	if( internal_file->block_data != NULL )
	{
		if( memory_set(
		     internal_file->block_data,
		     0,
		     internal_file->block_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear block data.",
			 function );

			return( -1 );
		}
	}
	return( 0 );
}

/* Reads a buffer from the file
 * Returns the number of bytes read if successful, or -1 on error
 */
ssize_t libcfile_file_read_buffer(
         libcfile_file_t *file,
         uint8_t *buffer,
         size_t size,
         libcerror_error_t **error )
{
	static char *function = "libcfile_file_read_buffer";
	ssize_t read_count    = 0;
	uint32_t error_code   = 0;

	read_count = libcfile_file_read_buffer_with_error_code(
	              file,
	              buffer,
	              size,
	              &error_code,
	              error );

	if( read_count == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read from file.",
		 function );

		return( -1 );
	}
	return( read_count );
}

/* Reads a buffer from the file
 * This function uses the POSIX read function or equivalent
 * Returns the number of bytes read if successful, or -1 on error
 */
ssize_t libcfile_file_read_buffer_with_error_code(
         libcfile_file_t *file,
         uint8_t *buffer,
         size_t size,
         uint32_t *error_code,
         libcerror_error_t **error )
{
	libcfile_internal_file_t *internal_file = NULL;
	static char *function                   = "libcfile_file_read_buffer_with_error_code";
	size_t buffer_offset                    = 0;
	size_t read_size                        = 0;
	size_t read_size_remainder              = 0;
	ssize_t read_count                      = 0;
	int result                              = 0;

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	internal_file = (libcfile_internal_file_t *) file;

	if( internal_file->descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file - missing descriptor.",
		 function );

		return( -1 );
	}
	if( buffer == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid buffer.",
		 function );

		return( -1 );
	}
	if( size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( error_code == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid error code.",
		 function );

		return( -1 );
	}
	if( internal_file->block_size != 0 )
	{
		if( internal_file->block_data == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid file - missing block data.",
			 function );

			return( -1 );
		}
	}
	if( internal_file->current_offset < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid file - current offset value out of bounds.",
		 function );

		return( -1 );
	}
	if( ( size == 0 )
	 || ( (size64_t) internal_file->current_offset > internal_file->size ) )
	{
		return( 0 );
	}
	if( ( (size64_t) internal_file->current_offset + size ) > internal_file->size )
	{
		size = (size_t) ( internal_file->size - internal_file->current_offset );
	}
	if( internal_file->block_size != 0 )
	{
		/* Read a block of data to align with the next block
		 */
		if( ( internal_file->block_data_offset > 0 )
		 && ( internal_file->block_data_size == 0 ) )
		{
			if( memory_set(
			     internal_file->block_data,
			     0,
			     internal_file->block_size ) == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_SET_FAILED,
				 "%s: unable to clear block data.",
				 function );

				return( -1 );
			}
			grub_errno = GRUB_ERR_NONE;
			read_count = grub_file_read(
			              internal_file->descriptor,
			              internal_file->block_data,
			              internal_file->block_size );

			if( read_count != (ssize_t) internal_file->block_size )
			{
				*error_code = (uint32_t) grub_errno;
				grub_errno = GRUB_ERR_NONE;
				return( -1 );
			}
			internal_file->block_data_size = (size_t) read_count;
		}
		if( ( internal_file->block_data_offset > 0 )
		 && ( internal_file->block_data_offset < internal_file->block_data_size ) )
		{
			read_size = internal_file->block_data_size - internal_file->block_data_offset;

			if( read_size > size )
			{
				read_size = size;
			}
			if( memory_copy(
			     buffer,
			     &( internal_file->block_data[ internal_file->block_data_offset ] ),
			     read_size ) == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
				 "%s: unable to copy block data.",
				 function );

				return( -1 );
			}
			buffer_offset                    += read_size;
			size                             -= read_size;
			internal_file->block_data_offset += read_size;
			internal_file->current_offset    += read_size;
		}
		if( size == 0 )
		{
			return( (ssize_t) buffer_offset );
		}
	}
	read_size = size;

	if( internal_file->block_size != 0 )
	{
		/* Read block aligned
		 */
		read_size_remainder = read_size % internal_file->block_size;
		read_size          -= read_size_remainder;
	}
	if( read_size > 0 )
	{
		grub_errno = GRUB_ERR_NONE;
		read_count = grub_file_read(
		              internal_file->descriptor,
		              (void *) &( buffer[ buffer_offset ] ),
		              read_size );

		if( ( internal_file->block_size == 0 )
		 && ( read_count < 0 ) )
		{
			result = 0;
		}
		else if( ( internal_file->block_size != 0 )
		      && ( read_count != (ssize_t) read_size ) )
		{
			result = 0;
		}
		else
		{
			result = 1;
		}
		if( result == 0 )
		{
			*error_code = (uint32_t) grub_errno;
			grub_errno = GRUB_ERR_NONE;
			return( -1 );
		}
		buffer_offset                 += (size_t) read_count;
		internal_file->current_offset += read_count;
	}
	/* Read the non-aligned remainder
	 */
	if( read_size_remainder > 0 )
	{
		if( memory_set(
		     internal_file->block_data,
		     0,
		     internal_file->block_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear block data.",
			 function );

			return( -1 );
		}
		grub_errno = GRUB_ERR_NONE;
		read_count = grub_file_read(
		              internal_file->descriptor,
		              internal_file->block_data,
		              internal_file->block_size );

		if( read_count != (ssize_t) internal_file->block_size )
		{
			*error_code = (uint32_t) grub_errno;
			grub_errno = GRUB_ERR_NONE;
			return( -1 );
		}
		internal_file->block_data_offset = 0;
		internal_file->block_data_size   = (size_t) read_count;

		if( memory_copy(
		     &( buffer[ buffer_offset ] ),
		     internal_file->block_data,
		     read_size_remainder ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy block data.",
			 function );

			return( -1 );
		}
		buffer_offset                    += read_size_remainder;
		internal_file->block_data_offset += read_size_remainder;
		internal_file->current_offset    += read_size_remainder;
	}
	return( (ssize_t) buffer_offset );
}

/* Seeks a certain offset within the file
 * This function uses the POSIX lseek function or equivalent
 * Returns the offset if the seek is successful or -1 on error
 */
off64_t libcfile_file_seek_offset(
         libcfile_file_t *file,
         off64_t offset,
         int whence,
         libcerror_error_t **error )
{
	libcfile_internal_file_t *internal_file = NULL;
	static char *function                   = "libcfile_file_seek_offset";
	off64_t offset_remainder                = 0;

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	internal_file = (libcfile_internal_file_t *) file;

	if( internal_file->descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file - missing descriptor.",
		 function );

		return( -1 );
	}
	if( offset > (off64_t) INT64_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid offset value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( ( whence != SEEK_CUR )
	 && ( whence != SEEK_END )
	 && ( whence != SEEK_SET ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported whence.",
		 function );

		return( -1 );
	}
	if( internal_file->block_size != 0 )
	{
		if( whence == SEEK_CUR )
		{
			offset += internal_file->current_offset;
		}
		else if( whence == SEEK_END )
		{
			offset += internal_file->size;
		}
		whence           = SEEK_SET;
		offset_remainder = offset % internal_file->block_size;
		offset          -= offset_remainder;
	}

	switch (whence)
	{
		case SEEK_CUR:
			offset = grub_file_seek(internal_file->descriptor,
				grub_file_tell(internal_file->descriptor) + offset);
			break;
		case SEEK_SET:
			offset = grub_file_seek(internal_file->descriptor, offset);
			break;
		default:
			offset = -1;
	}

	if( offset == (grub_off_t) -1)
	{
		grub_errno = GRUB_ERR_NONE;
		return( -1 );
	}
	internal_file->current_offset = offset;

	if( internal_file->block_size != 0 )
	{
		internal_file->current_offset   += offset_remainder;
		internal_file->block_data_offset = (size_t) offset_remainder;
		internal_file->block_data_size   = 0;
	}
	return( internal_file->current_offset );
}

/* Checks if the file is open
 * Returns 1 if open, 0 if not or -1 on error
 */
int libcfile_file_is_open(
     libcfile_file_t *file,
     libcerror_error_t **error )
{
	libcfile_internal_file_t *internal_file = NULL;
	static char *function                   = "libcfile_file_is_open";

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	internal_file = (libcfile_internal_file_t *) file;

	if( internal_file->descriptor == NULL )
	{
		return( 0 );
	}
	return( 1 );
}

/* Retrieves the current offset in the file
 * Returns 1 if successful or -1 on error
 */
int libcfile_file_get_offset(
     libcfile_file_t *file,
     off64_t *offset,
     libcerror_error_t **error )
{
	libcfile_internal_file_t *internal_file = NULL;
	static char *function                   = "libcfile_file_get_offset";

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	internal_file = (libcfile_internal_file_t *) file;

	if( internal_file->descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file - missing descriptor.",
		 function );

		return( -1 );
	}

	if( offset == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid offset.",
		 function );

		return( -1 );
	}
	*offset = internal_file->current_offset;

	return( 1 );
}

/* Retrieves the size of the file
 * This function uses the POSIX fstat function or equivalent
 * Returns 1 if successful or -1 on error
 */
int libcfile_internal_file_get_size(
     libcfile_internal_file_t *internal_file,
     size64_t *size,
     libcerror_error_t **error )
{
	static char *function     = "libcfile_internal_file_get_size";
	size64_t safe_size        = 0;

	if( internal_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	if( internal_file->descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file - missing descriptor.",
		 function );

		return( -1 );
	}
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

	safe_size = grub_file_size(internal_file->descriptor);
	if( safe_size == GRUB_FILE_SIZE_UNKNOWN )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve file statistics.",
		 function );

		return( -1 );
	}

	*size = safe_size;

	return( 1 );
}

/* Retrieves the size of the file
 * Returns 1 if successful or -1 on error
 */
int libcfile_file_get_size(
     libcfile_file_t *file,
     size64_t *size,
     libcerror_error_t **error )
{
	libcfile_internal_file_t *internal_file = NULL;
	static char *function                   = "libcfile_file_get_size";

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	internal_file = (libcfile_internal_file_t *) file;

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
	*size = internal_file->size;

	return( 1 );
}

/* Sets the block size for the read and seek operations
 * A block size of 0 represents no block-based operations
 * The total size must be a multitude of block size
 * Returns 1 if successful or -1 on error
 */
int libcfile_internal_file_set_block_size(
     libcfile_internal_file_t *internal_file,
     size_t block_size,
     libcerror_error_t **error )
{
	static char *function = "libcfile_internal_file_set_block_size";

	if( internal_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}

	if( block_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid block size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( internal_file->block_data != NULL )
	{
		if( block_size != internal_file->block_size )
		{
			memory_free(
			 internal_file->block_data );

			internal_file->block_data      = NULL;
			internal_file->block_data_size = 0;
		}
	}
	if( internal_file->block_data == NULL )
	{
		if( block_size > 0 )
		{
			internal_file->block_data = (uint8_t *) memory_allocate(
			                                         sizeof( uint8_t ) * block_size );

			if( internal_file->block_data == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
				 "%s: unable to create block data.",
				 function );

				return( -1 );
			}
			if( memory_set(
			     internal_file->block_data,
			     0,
			     block_size ) == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_SET_FAILED,
				 "%s: unable to clear block data.",
				 function );

				return( -1 );
			}
		}
		internal_file->block_size = block_size;
	}
	return( 1 );
}

/* Sets the block size for the read and seek operations
 * A block size of 0 represents no block-based operations
 * The total size must be a multitude of block size
 * Returns 1 if successful or -1 on error
 */
int libcfile_file_set_block_size(
     libcfile_file_t *file,
     size_t block_size,
     libcerror_error_t **error )
{
	libcfile_internal_file_t *internal_file = NULL;
	static char *function                   = "libcfile_file_set_block_size";

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	internal_file = (libcfile_internal_file_t *) file;

	if( internal_file->descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file - missing descriptor.",
		 function );

		return( -1 );
	}

	if( block_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid block size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( ( block_size != 0 )
	 && ( ( internal_file->size % block_size ) != 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid block size value out of bounds.",
		 function );

		return( -1 );
	}
	if( libcfile_internal_file_set_block_size(
	     internal_file,
	     block_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set block size.",
		 function );

		return( -1 );
	}
	return( 1 );
}

