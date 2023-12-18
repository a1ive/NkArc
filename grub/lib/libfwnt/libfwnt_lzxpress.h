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

#if !defined( _LIBFWNT_LZXPRESS_H )
#define _LIBFWNT_LZXPRESS_H

#include <libyal_wrap.h>

#include "libfwnt_bit_stream.h"
#include "libfwnt_extern.h"
#include "libfwnt_huffman_tree.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfwnt_lzxpress_huffman_code_symbol libfwnt_lzxpress_huffman_code_symbol_t;

struct libfwnt_lzxpress_huffman_code_symbol
{
	/* The symbol
	 */
	uint16_t symbol;

	/* The code size 
	 */
	uint16_t code_size;
};

LIBFWNT_EXTERN \
int libfwnt_lzxpress_compress(
     const uint8_t *uncompressed_data,
     size_t uncompressed_data_size,
     uint8_t *compressed_data,
     size_t *compressed_data_size,
     libcerror_error_t **error );

LIBFWNT_EXTERN \
int libfwnt_lzxpress_decompress(
     const uint8_t *compressed_data,
     size_t compressed_data_size,
     uint8_t *uncompressed_data,
     size_t *uncompressed_data_size,
     libcerror_error_t **error );

int libfwnt_lzxpress_huffman_decompress_chunk(
     libfwnt_bit_stream_t *bit_stream,
     uint8_t *uncompressed_data,
     size_t uncompressed_data_size,
     size_t *uncompressed_data_offset,
     libcerror_error_t **error );

LIBFWNT_EXTERN \
int libfwnt_lzxpress_huffman_decompress(
     const uint8_t *compressed_data,
     size_t compressed_data_size,
     uint8_t *uncompressed_data,
     size_t *uncompressed_data_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFWNT_LZXPRESS_H ) */

