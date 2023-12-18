/*
 * LZXPRESS (de)compression functions
 *
 * Copyright (C) 2009-2023, Joachim Metz <joachim.metz@gmail.com>
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

#include "libfwnt_bit_stream.h"
#include "libfwnt_huffman_tree.h"
#include "libfwnt_lzxpress.h"

/* Compresses data using LZXPRESS (LZ77 + DIRECT2) compression
 * Returns 1 on success or -1 on error
 */
int libfwnt_lzxpress_compress(
     const uint8_t *uncompressed_data,
     size_t uncompressed_data_size,
     uint8_t *compressed_data,
     size_t *compressed_data_size,
     libcerror_error_t **error )
{
	static char *function = "libfwnt_lzxpress_compress";

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
	if( uncompressed_data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid uncompressed data size value exceeds maximum.",
		 function );

		return( -1 );
	}
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
	if( compressed_data_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid compressed data size.",
		 function );

		return( -1 );
	}
/* TODO implement */
	return( -1 );
}

/* Decompresses data using LZXPRESS (LZ77 + DIRECT2) compression
 * Return 1 on success or -1 on error
 */
int libfwnt_lzxpress_decompress(
     const uint8_t *compressed_data,
     size_t compressed_data_size,
     uint8_t *uncompressed_data,
     size_t *uncompressed_data_size,
     libcerror_error_t **error )
{
	static char *function                  = "libfwnt_lzxpress_decompress";
	size_t compressed_data_offset          = 0;
	size_t compression_index               = 0;
	size_t compression_shared_byte_index   = 0;
	size_t safe_uncompressed_data_size     = 0;
	size_t uncompressed_data_offset        = 0;
	uint32_t compression_indicator         = 0;
	uint32_t compression_indicator_bitmask = 0;
	uint16_t compression_tuple             = 0;
	uint16_t compression_tuple_size        = 0;
	int16_t compression_tuple_offset       = 0;

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
	if( ( compressed_data_size < 2 )
	 || ( compressed_data_size > (size_t) SSIZE_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid compressed data size value out of bounds.",
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
	if( *uncompressed_data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid uncompressed data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	safe_uncompressed_data_size = *uncompressed_data_size;

	while( compressed_data_offset < compressed_data_size )
	{
		if( uncompressed_data_offset >= safe_uncompressed_data_size )
		{
			break;
		}
		if( ( compressed_data_size < 4 )
		 || ( compressed_data_offset > ( compressed_data_size - 4 ) ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
			 "%s: compressed data too small.",
			 function );

			return( -1 );
		}
		byte_stream_copy_to_uint32_little_endian(
		 &( compressed_data[ compressed_data_offset ] ),
		 compression_indicator );

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: compressed data offset\t\t\t: %" PRIzd " (0x%08" PRIzx ")\n",
			 function,
			 compressed_data_offset,
			 compressed_data_offset );

			libcnotify_printf(
			 "%s: compression indicator\t\t\t: 0x%08" PRIx32 "\n",
			 function,
			 compression_indicator );

			libcnotify_printf(
			 "\n" );
		}
#endif
		compressed_data_offset += 4;

		for( compression_indicator_bitmask = 0x80000000UL;
		     compression_indicator_bitmask > 0;
		     compression_indicator_bitmask >>= 1 )
		{
			if( uncompressed_data_offset >= safe_uncompressed_data_size )
			{
				break;
			}
			if( compressed_data_offset >= compressed_data_size )
			{
				break;
			}
			/* If the indicator bit is 0 the data is uncompressed
			 * or 1 if the data is compressed
			 */
			if( ( compression_indicator & compression_indicator_bitmask ) != 0 )
			{
				if( compressed_data_offset > ( compressed_data_size - 2 ) )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
					 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
					 "%s: compressed data too small.",
					 function );

					return( -1 );
				}
		                byte_stream_copy_to_uint16_little_endian(
		                 &( compressed_data[ compressed_data_offset ] ),
		                 compression_tuple );

				compressed_data_offset += 2;

				/* The compression tuple contains:
				 * 0 - 2	the size
				 * 3 - 15	the offset - 1
				 */
				compression_tuple_size   = ( compression_tuple & 0x0007 );
				compression_tuple_offset = ( compression_tuple >> 3 ) + 1;

				/* Check for a first level extended size
				 * stored in the 4-bits of a shared extended compression tuple size byte
				 * the size is added to the previous size
				 */
				if( compression_tuple_size == 0x07 )
				{
					if( compression_shared_byte_index == 0 )
					{
						if( compressed_data_offset >= compressed_data_size )
						{
							libcerror_error_set(
							 error,
							 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
							 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
							 "%s: compressed data too small.",
							 function );

							return( -1 );
						}
						compression_tuple_size += compressed_data[ compressed_data_offset ] & 0x0f;

						compression_shared_byte_index = compressed_data_offset++;
					}
					else
					{
						compression_tuple_size += compressed_data[ compression_shared_byte_index ] >> 4;

						compression_shared_byte_index = 0;
					}
				}
				/* Check for a second level extended size
				 * stored in the 8-bits of the next byte
				 * the size is added to the previous size
				 */
				if( compression_tuple_size == ( 0x07 + 0x0f ) )
				{
					if( compressed_data_offset >= compressed_data_size )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
						 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
						 "%s: compressed data too small.",
						 function );

						return( -1 );
					}
					compression_tuple_size += compressed_data[ compressed_data_offset++ ];
				}
				/* Check for a third level extended size
				 * stored in the 16-bits of the next two bytes
				 * the previous size is ignored
				 */
				if( compression_tuple_size == ( 0x07 + 0x0f + 0xff ) )
				{
					if( compressed_data_offset > ( compressed_data_size - 2 ) )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
						 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
						 "%s: compressed data too small.",
						 function );

						return( -1 );
					}
			                byte_stream_copy_to_uint16_little_endian(
			                 &( compressed_data[ compressed_data_offset ] ),
			                 compression_tuple_size );
		
					compressed_data_offset += 2;
				}
				/* The size value is stored as
				 * size - 3
				 */
				compression_tuple_size += 3;

#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: compressed data offset\t\t\t: %" PRIzd " (0x%08" PRIzx ")\n",
					 function,
					 compressed_data_offset,
					 compressed_data_offset );

					libcnotify_printf(
					 "%s: compression tuple offset\t\t\t: %" PRIi16 "\n",
					 function,
					 compression_tuple_offset );

					libcnotify_printf(
					 "%s: compression tuple size\t\t\t: %" PRIu16 "\n",
					 function,
					 compression_tuple_size );

					libcnotify_printf(
					 "%s: uncompressed data offset\t\t\t: %" PRIzd " (0x%08" PRIzx ")\n",
					 function,
					 uncompressed_data_offset,
					 uncompressed_data_offset );

					libcnotify_printf(
					 "\n" );
				}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

				if( compression_tuple_size > 32771 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
					 "%s: compression tuple size value out of bounds.",
					 function );

					return( -1 );
				}
				compression_index = uncompressed_data_offset - compression_tuple_offset;

				while( compression_tuple_size > 0 )
				{
					if( compression_index >= uncompressed_data_offset )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
						 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
						 "%s: invalid compressed data at offset: %" PRIzd " - compression index: %" PRIzd " out of range: %" PRIzd ".",
						 function,
						 compressed_data_offset,
						 compression_index,
						 uncompressed_data_offset );

						return( -1 );
					}
					if( uncompressed_data_offset >= safe_uncompressed_data_size )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
						 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
						 "%s: uncompressed data too small.",
						 function );

						return( -1 );
					}
					uncompressed_data[ uncompressed_data_offset++ ] = uncompressed_data[ compression_index++ ];

					compression_tuple_size--;
				}
			}
			else
			{
				if( compressed_data_offset >= compressed_data_size )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
					 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
					 "%s: compressed data too small.",
					 function );

					return( -1 );
				}
				if( uncompressed_data_offset > safe_uncompressed_data_size )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
					 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
					 "%s: uncompressed data too small.",
					 function );

					return( -1 );
				}
				uncompressed_data[ uncompressed_data_offset++ ] = compressed_data[ compressed_data_offset++ ];
			}
		}
	}
	*uncompressed_data_size = uncompressed_data_offset;

	return( 1 );
}

/* Decompresses a LZXPRESS Huffman compressed chunk
 * Return 1 on success or -1 on error
 */
int libfwnt_lzxpress_huffman_decompress_chunk(
     libfwnt_bit_stream_t *bit_stream,
     uint8_t *uncompressed_data,
     size_t uncompressed_data_size,
     size_t *uncompressed_data_offset,
     libcerror_error_t **error )
{
	uint8_t code_size_array[ 512 ];

	libfwnt_huffman_tree_t *huffman_tree       = NULL;
	static char *function                      = "libfwnt_lzxpress_huffman_decompress_chunk";
	size_t next_chunk_uncompressed_data_offset = 0;
	size_t safe_uncompressed_data_offset       = 0;
	uint32_t compression_offset                = 0;
	uint32_t compression_size                  = 0;
	uint32_t symbol                            = 0;
	uint8_t byte_value                         = 0;

	if( bit_stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid bit stream.",
		 function );

		return( -1 );
	}
	if( ( bit_stream->byte_stream_size - bit_stream->byte_stream_offset ) < 260 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: invalid bit stream - byte stream value too small.",
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
	if( uncompressed_data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid uncompressed data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( uncompressed_data_offset == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid uncompressed data offset.",
		 function );

		return( -1 );
	}
	safe_uncompressed_data_offset = *uncompressed_data_offset;

	if( safe_uncompressed_data_offset >= uncompressed_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: uncompressed data offset value out of bounds.",
		 function );

		return( -1 );
	}
	/* The table contains 4-bits code size per symbol
	 */
	while( symbol < 512 )
	{
		byte_value = bit_stream->byte_stream[ bit_stream->byte_stream_offset ];

		code_size_array[ symbol++ ] = byte_value & 0x0f;

		byte_value >>= 4;

		code_size_array[ symbol++ ] = byte_value & 0x0f;

		bit_stream->byte_stream_offset += 1;
	}
	if( libfwnt_huffman_tree_initialize(
	     &huffman_tree,
	     512,
	     15,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create Huffman tree.",
		 function );

		goto on_error;
	}
	if( libfwnt_huffman_tree_build(
	     huffman_tree,
	     code_size_array,
	     512,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to build Huffman tree.",
		 function );

		goto on_error;
	}
	if( libfwnt_bit_stream_read(
	     bit_stream,
	     32,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read 32-bit from bit stream.",
		 function );

		goto on_error;
	}
	next_chunk_uncompressed_data_offset = safe_uncompressed_data_offset + 65536;

	if( next_chunk_uncompressed_data_offset > uncompressed_data_size )
	{
		next_chunk_uncompressed_data_offset = uncompressed_data_size;
	}
        while( ( bit_stream->byte_stream_offset < bit_stream->byte_stream_size )
            || ( bit_stream->bit_buffer_size > 0 ) )
	{
		if( safe_uncompressed_data_offset >= next_chunk_uncompressed_data_offset )
		{
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: end of chunk at compressed data offset: %" PRIzd " (0x%08" PRIzx "), uncompressed data offset: %" PRIzd " (0x%08" PRIzx ")\n",
				 function,
				 bit_stream->byte_stream_offset,
				 bit_stream->byte_stream_offset,
				 safe_uncompressed_data_offset,
				 safe_uncompressed_data_offset );

				libcnotify_printf(
				 "\n" );
			}
#endif
			bit_stream->bit_buffer_size = 0;

			break;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: compressed data offset\t: %" PRIzd " (0x%08" PRIzx ")\n",
			 function,
			 bit_stream->byte_stream_offset,
			 bit_stream->byte_stream_offset );
		}
#endif
		if( libfwnt_huffman_tree_get_symbol_from_bit_stream(
		     huffman_tree,
		     bit_stream,
		     &symbol,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to read symbol.",
			 function );

			goto on_error;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: huffman symbol\t\t: 0x%04" PRIx16 "\n",
			 function,
			 symbol );
		}
#endif
		if( symbol < 256 )
		{
			uncompressed_data[ safe_uncompressed_data_offset++ ] = (uint8_t) symbol;
		}
		/* Make sure the bit buffer contains at least 16-bit to ensure end-of-block marker is read correctly
		 */
		if( bit_stream->bit_buffer_size < 16 )
		{
			if( libfwnt_bit_stream_read(
			     bit_stream,
			     16,
			     error ) == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read 16-bit from bit stream.",
				 function );

				goto on_error;
			}
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: number of bits\t\t: %" PRId8 "\n",
			 function,
			 bit_stream->bit_buffer_size );
		}
#endif
		/* Check if we have an end-of-block marker (symbol 256) and the number of remaining bits are 0
		 */
/* TODO add ( symbol == 256 ) */
		if( ( bit_stream->bit_buffer == 0 )
		 && ( safe_uncompressed_data_offset >= uncompressed_data_size ) )
		{
			break;
		}
		if( symbol >= 256 )
		{
			symbol            -= 256;
			compression_offset = 0;
			compression_size   = symbol & 0x000f;
			symbol           >>= 4;

			if( symbol != 0 )
			{
				if( libfwnt_bit_stream_get_value(
				     bit_stream,
				     (uint8_t) symbol,
				     &compression_offset,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve compression offset from bit stream.",
					 function );

					goto on_error;
				}
#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: compression offset\t\t: %" PRIu32 "\n",
					 function,
					 compression_offset );
				}
#endif
			}
			compression_offset = (uint32_t) ( ( 1 << symbol ) | compression_offset );

#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: compression size\t\t: %" PRIu32 "\n",
				 function,
				 compression_size );
			}
#endif
			/* Ignore any data beyond the uncompressed block size
			 */
			if( compression_size == 15 )
			{
				if( bit_stream->byte_stream_offset > ( bit_stream->byte_stream_size - 1 ) )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
					 "%s: compressed data size value too small.",
					 function );

					goto on_error;
				}
				compression_size = bit_stream->byte_stream[ bit_stream->byte_stream_offset ] + 15;

#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: extended compression offset\t: %" PRIzd " (0x%08" PRIzx ")\n",
					 function,
					 bit_stream->byte_stream_offset,
					 bit_stream->byte_stream_offset );

					libcnotify_printf(
					 "%s: extended compression size\t: %" PRIu32 "\n",
					 function,
					 compression_size );
				}
#endif
				bit_stream->byte_stream_offset += 1;

				if( compression_size == 270 )
				{
					if( bit_stream->byte_stream_offset > ( bit_stream->byte_stream_size - 2 ) )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
						 "%s: compressed data size value too small.",
						 function );

						goto on_error;
					}
					byte_stream_copy_to_uint16_little_endian(
					 &( bit_stream->byte_stream[ bit_stream->byte_stream_offset ] ),
					 compression_size );

#if defined( HAVE_DEBUG_OUTPUT )
					if( libcnotify_verbose != 0 )
					{
						libcnotify_printf(
						 "%s: extended compression offset\t: %" PRIzd " (0x%08" PRIzx ")\n",
						 function,
						 bit_stream->byte_stream_offset,
						 bit_stream->byte_stream_offset );

						libcnotify_printf(
						 "%s: extended compression size\t: %" PRIu32 "\n",
						 function,
						 compression_size );
					}
#endif
					bit_stream->byte_stream_offset += 2;

					if( compression_size == 0 )
					{
						if( bit_stream->byte_stream_offset > ( bit_stream->byte_stream_size - 4 ) )
						{
							libcerror_error_set(
							 error,
							 LIBCERROR_ERROR_DOMAIN_RUNTIME,
							 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
							 "%s: compressed data size value too small.",
							 function );

							goto on_error;
						}
						byte_stream_copy_to_uint32_little_endian(
						 &( bit_stream->byte_stream[ bit_stream->byte_stream_offset ] ),
						 compression_size );

#if defined( HAVE_DEBUG_OUTPUT )
						if( libcnotify_verbose != 0 )
						{
							libcnotify_printf(
							 "%s: extended compression offset\t: %" PRIzd " (0x%08" PRIzx ")\n",
							 function,
							 bit_stream->byte_stream_offset,
							 bit_stream->byte_stream_offset );

							libcnotify_printf(
							 "%s: extended compression size\t: %" PRIu32 "\n",
							 function,
							 compression_size );
						}
#endif
						bit_stream->byte_stream_offset += 4;

					}
				}
			}
			compression_size += 3;

#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: compression offset\t\t: %" PRIu32 "\n",
				 function,
				 compression_offset );

				libcnotify_printf(
				 "%s: compression size\t\t: %" PRIu32 "\n",
				 function,
				 compression_size );

				libcnotify_printf(
				 "%s: uncompressed data offset\t: %" PRIzd " (0x%08" PRIzx ")\n",
				 function,
				 safe_uncompressed_data_offset,
				 safe_uncompressed_data_offset );
			}
#endif
			if( compression_offset > safe_uncompressed_data_offset )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: compression offset value out of bounds.",
				 function );

				goto on_error;
			}
			if( compression_size > ( uncompressed_data_size - safe_uncompressed_data_offset ) )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: compression size value out of bounds.",
				 function );

				goto on_error;
			}
			compression_offset = safe_uncompressed_data_offset - compression_offset;

			while( compression_size > 0 )
			{
				uncompressed_data[ safe_uncompressed_data_offset++ ] = uncompressed_data[ compression_offset++ ];

				compression_size--;
			}
			/* Make sure the bit buffer contains at least 16-bit to ensure successive chunks in a stream are read correctly
			 */
			if( bit_stream->bit_buffer_size < 16 )
			{
				if( libfwnt_bit_stream_read(
				     bit_stream,
				     16,
				     error ) == -1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_READ_FAILED,
					 "%s: unable to read 16-bit from bit stream.",
					 function );

					goto on_error;
				}
			}
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "\n" );
		}
#endif
	}
	if( libfwnt_huffman_tree_free(
	     &huffman_tree,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free Huffman tree.",
		 function );

		goto on_error;
	}
	*uncompressed_data_offset = safe_uncompressed_data_offset;

	return( 1 );

on_error:
	if( huffman_tree != NULL )
	{
		libfwnt_huffman_tree_free(
		 &huffman_tree,
		 NULL );
	}
	return( -1 );
}

/* Decompresses data using LZXPRESS Huffman compression
 * Return 1 on success or -1 on error
 */
int libfwnt_lzxpress_huffman_decompress(
     const uint8_t *compressed_data,
     size_t compressed_data_size,
     uint8_t *uncompressed_data,
     size_t *uncompressed_data_size,
     libcerror_error_t **error )
{
	libfwnt_bit_stream_t *bit_stream   = NULL;
	static char *function              = "libfwnt_lzxpress_huffman_decompress";
	size_t safe_uncompressed_data_size = 0;
	size_t uncompressed_data_offset    = 0;

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
	if( libfwnt_bit_stream_initialize(
	     &bit_stream,
	     compressed_data,
	     compressed_data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create bit stream.",
		 function );

		goto on_error;
	}
	safe_uncompressed_data_size = *uncompressed_data_size;

	while( bit_stream->byte_stream_offset < bit_stream->byte_stream_size )
	{
		if( uncompressed_data_offset >= safe_uncompressed_data_size )
		{
			break;
		}
		if( libfwnt_lzxpress_huffman_decompress_chunk(
		     bit_stream,
		     uncompressed_data,
		     safe_uncompressed_data_size,
		     &uncompressed_data_offset,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_COMPRESSION,
			 LIBCERROR_COMPRESSION_ERROR_DECOMPRESS_FAILED,
			 "%s: unable to decompress chunk.",
			 function );

			goto on_error;
		}
	}
	if( libfwnt_bit_stream_free(
	     &bit_stream,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free bit stream.",
		 function );

		goto on_error;
	}
	*uncompressed_data_size = uncompressed_data_offset;

	return( 1 );

on_error:
	if( bit_stream != NULL )
	{
		libfwnt_bit_stream_free(
		 &bit_stream,
		 NULL );
	}
	return( -1 );
}

