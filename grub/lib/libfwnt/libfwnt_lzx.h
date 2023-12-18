/*
 * LZX (un)compression functions
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

#if !defined( _LIBFWNT_LZX_H )
#define _LIBFWNT_LZX_H

#include <libyal_wrap.h>

#include "libfwnt_bit_stream.h"
#include "libfwnt_extern.h"
#include "libfwnt_huffman_tree.h"

#if defined( __cplusplus )
extern "C" {
#endif

/* The block types
 */
enum LIBFWNT_LZX_BLOCK_TYPES
{
	LIBFWNT_LZX_BLOCK_TYPE_VERBATIM		= 0x01,
	LIBFWNT_LZX_BLOCK_TYPE_ALIGNED		= 0x02,
	LIBFWNT_LZX_BLOCK_TYPE_UNCOMPRESSED	= 0x03
};

int libfwnt_lzx_read_huffman_code_sizes(
     libfwnt_bit_stream_t *bit_stream,
     uint8_t *code_size_array,
     int number_of_code_sizes,
     libcerror_error_t **error );

int libfwnt_lzx_build_main_huffman_tree(
     libfwnt_bit_stream_t *bit_stream,
     uint8_t *code_size_array,
     libfwnt_huffman_tree_t *main_huffman_tre,
     libcerror_error_t **error );

int libfwnt_lzx_build_lengths_huffman_tree(
     libfwnt_bit_stream_t *bit_stream,
     uint8_t *code_size_array,
     libfwnt_huffman_tree_t *lengths_huffman_tre,
     libcerror_error_t **error );

int libfwnt_lzx_build_aligned_offsets_huffman_tree(
     libfwnt_bit_stream_t *bit_stream,
     uint8_t *code_size_array,
     libfwnt_huffman_tree_t *aligned_offsets_huffman_tre,
     libcerror_error_t **error );

int libfwnt_lzx_decode_huffman(
     libfwnt_bit_stream_t *bit_stream,
     uint32_t block_size,
     libfwnt_huffman_tree_t *main_huffman_tree,
     libfwnt_huffman_tree_t *lengths_huffman_tree,
     libfwnt_huffman_tree_t *aligned_offsets_huffman_tree,
     uint32_t *recent_compression_offsets,
     uint8_t *uncompressed_data,
     size_t uncompressed_data_size,
     size_t *uncompressed_data_offset,
     libcerror_error_t **error );

LIBFWNT_EXTERN \
int libfwnt_lzx_decompress(
     const uint8_t *compressed_data,
     size_t compressed_data_size,
     uint8_t *uncompressed_data,
     size_t *uncompressed_data_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFWNT_LZX_H ) */

