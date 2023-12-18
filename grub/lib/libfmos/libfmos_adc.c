/*
 * ADC (un)compression functions
 *
 * Copyright (C) 2019-2023, Joachim Metz <joachim.metz@gmail.com>
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

#include "libfmos_adc.h"

/* Decompresses ADC compressed data
 * Returns 1 on success or -1 on error
 */
int libfmos_adc_decompress(
     const uint8_t *compressed_data,
     size_t compressed_data_size,
     uint8_t *uncompressed_data,
     size_t *uncompressed_data_size,
     libcerror_error_t **error )
{
	static char *function              = "libfmos_adc_decompress";
	size_t compressed_data_offset      = 0;
	size_t match_offset                = 0;
	size_t safe_uncompressed_data_size = 0;
	size_t uncompressed_data_offset    = 0;
	uint16_t distance                  = 0;
	uint8_t oppcode                    = 0;
	uint8_t size                       = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	size_t debug_match_offset          = 0;
	uint16_t debug_match_size          = 0;
#endif

	if( compressed_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid compressed data.",
		 function );

		return( -1 );
	}
	if( compressed_data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid compressed data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( uncompressed_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid uncompressed data.",
		 function );

		return( -1 );
	}
	if( uncompressed_data_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid uncompressed data size.",
		 function );

		return( -1 );
	}
	safe_uncompressed_data_size = *uncompressed_data_size;

	if( safe_uncompressed_data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid uncompressed data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	while( compressed_data_offset < compressed_data_size )
	{
		if( uncompressed_data_offset >= safe_uncompressed_data_size )
		{
			break;
		}
		if( compressed_data_offset >= compressed_data_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
			 "%s: compressed data size value too small.",
			 function );

			return( -1 );
		}
		oppcode = compressed_data[ compressed_data_offset++ ];

		if( ( oppcode & 0x80 ) != 0 )
		{
			size = ( oppcode & 0x7f ) + 1;

			if( ( (size_t) size > compressed_data_size )
			 || ( compressed_data_offset > ( compressed_data_size - size ) ) )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: literal size value exceeds compressed data size.",
				 function );

				return( -1 );
			}
			if( ( (size_t) size > safe_uncompressed_data_size )
			 || ( uncompressed_data_offset > ( safe_uncompressed_data_size - size ) ) )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: literal size value exceeds uncompressed data size.",
				 function );

				return( -1 );
			}
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: literal:\n",
				 function );
				libcnotify_print_data(
				 &( compressed_data[ compressed_data_offset ] ),
				 size,
				 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
			}
#endif
			if( memory_copy(
			     &( uncompressed_data[ uncompressed_data_offset ] ),
			     &( compressed_data[ compressed_data_offset ] ),
			     (size_t) size ) == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
				 "%s: unable to copy literal to uncompressed data.",
				 function );

				return( -1 );
			}
			compressed_data_offset   += (size_t) size;
			uncompressed_data_offset += (size_t) size;
		}
		else
		{
			if( ( oppcode & 0x40 ) != 0 )
			{
				if( ( compressed_data_size < 2 )
				 || ( compressed_data_offset > ( compressed_data_size - 2 ) ) )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
					 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
					 "%s: compressed data size value too small.",
					 function );

					return( -1 );
				}
				size       = ( oppcode & 0x3f ) + 4;
				distance   = compressed_data[ compressed_data_offset++ ];
				distance <<= 8;
				distance  |= compressed_data[ compressed_data_offset++ ];
			}
			else
			{
				if( compressed_data_offset >= compressed_data_size )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
					 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
					 "%s: compressed data size value too small.",
					 function );

					return( -1 );
				}
				size       = ( ( oppcode & 0x3f ) >> 2 ) + 3;
				distance   = oppcode & 0x03;
				distance <<= 8;
				distance  |= compressed_data[ compressed_data_offset++ ];
			}
			if( (size_t) distance >= uncompressed_data_offset )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: invalid distance value out of bounds.",
				 function );

				return( -1 );
			}
			if( uncompressed_data_offset < 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: invalid uncompressed data offset value out of bounds.",
				 function );

				return( -1 );
			}
			match_offset = uncompressed_data_offset - distance - 1;

			if( ( (size_t) size > safe_uncompressed_data_size )
			 || ( uncompressed_data_offset > ( safe_uncompressed_data_size - size ) ) )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: invalid match size value out of bounds.",
				 function );

				return( -1 );
			}
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				debug_match_offset = match_offset;
				debug_match_size   = size;

				libcnotify_printf(
				 "%s: match offset\t\t\t\t\t\t: 0x%" PRIzx "\n",
				 function,
				 debug_match_offset );
			}
#endif
			while( size > 0 )
			{
				uncompressed_data[ uncompressed_data_offset++ ] = uncompressed_data[ match_offset++ ];

				size--;
			}
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: match:\n",
				 function );
				libcnotify_print_data(
				 &( uncompressed_data[ debug_match_offset ] ),
				 debug_match_size,
				 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
			}
#endif
		}
	}
	*uncompressed_data_size = uncompressed_data_offset;

	return( 1 );
}

