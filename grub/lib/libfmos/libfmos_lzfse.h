/*
 * LZFSE (un)compression functions
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

#if !defined( _LIBFMOS_LZFSE_H )
#define _LIBFMOS_LZFSE_H

#include <libyal_wrap.h>

#include "libfmos_extern.h"
#include "libfmos_lzfse_bit_stream.h"
#include "libfmos_lzfse_decoder.h"

#if defined( __cplusplus )
extern "C" {
#endif

int libfmos_lzfse_build_decoder_table(
     int number_of_states,
     uint16_t number_of_symbols,
     const uint16_t *frequency_table,
     libfmos_lzfse_decoder_entry_t *decoder_table,
     libcerror_error_t **error );

int libfmos_lzfse_build_value_decoder_table(
     int number_of_states,
     uint16_t number_of_symbols,
     const uint16_t *frequency_table,
     const uint8_t *value_bits_table,
     const int32_t *value_base_table,
     libfmos_lzfse_value_decoder_entry_t *value_decoder_table,
     libcerror_error_t **error );

int libfmos_lzfse_read_block_v1_header(
     libfmos_lzfse_decoder_t *decoder,
     const uint8_t *compressed_data,
     size_t compressed_data_size,
     size_t *compressed_data_offset,
     uint16_t *frequency_table,
     libcerror_error_t **error );

int libfmos_lzfse_read_block_v2_header(
     libfmos_lzfse_decoder_t *decoder,
     const uint8_t *compressed_data,
     size_t compressed_data_size,
     size_t *compressed_data_offset,
     uint16_t *frequency_table,
     libcerror_error_t **error );

int libfmos_lzfse_read_compressed_frequency_table(
     const uint8_t *compressed_data,
     size_t compressed_data_size,
     uint16_t *frequency_table,
     libcerror_error_t **error );

int libfmos_lzfse_read_block(
     libfmos_lzfse_decoder_t *decoder,
     const uint8_t *compressed_data,
     size_t compressed_data_size,
     size_t *compressed_data_offset,
     uint8_t *uncompressed_data,
     size_t uncompressed_data_size,
     size_t *uncompressed_data_offset,
     libcerror_error_t **error );

int libfmos_lzfse_read_literal_values(
     libfmos_lzfse_decoder_t *decoder,
     libfmos_lzfse_bit_stream_t *bit_stream,
     uint8_t *literal_values,
     libcerror_error_t **error );

int libfmos_lzfse_read_lmd_values(
     libfmos_lzfse_decoder_t *decoder,
     libfmos_lzfse_bit_stream_t *bit_stream,
     uint8_t *literal_values,
     uint8_t *uncompressed_data,
     size_t uncompressed_data_size,
     size_t *uncompressed_data_offset,
     libcerror_error_t **error );

LIBFMOS_EXTERN \
int libfmos_lzfse_decompress(
     const uint8_t *compressed_data,
     size_t compressed_data_size,
     uint8_t *uncompressed_data,
     size_t *uncompressed_data_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFMOS_LZFSE_H ) */

