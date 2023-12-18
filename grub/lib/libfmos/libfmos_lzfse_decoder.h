/*
 * LZFSE decoder functions
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

#if !defined( _LIBFMOS_LZFSE_DECODER_H )
#define _LIBFMOS_LZFSE_DECODER_H

#include <libyal_wrap.h>

#include "libfmos_definitions.h"
#include "libfmos_extern.h"
#include "libfmos_lzfse_bit_stream.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfmos_lzfse_decoder_entry libfmos_lzfse_decoder_entry_t;

struct libfmos_lzfse_decoder_entry
{
	/* The number of bits
	 */
	int8_t number_of_bits;

	/* The symbol
	 */
	uint8_t symbol;

	/* The delta to compute the next state
	 */
	int16_t delta;
};

typedef struct libfmos_lzfse_value_decoder_entry libfmos_lzfse_value_decoder_entry_t;

struct libfmos_lzfse_value_decoder_entry
{
	/* The (total) number of bits
	 */
	uint8_t number_of_bits;

	/* The value bits
	 */
	uint8_t value_bits;

	/* The delta
	 */
	int16_t delta;

	/* The value base
	 */
	int32_t value_base;

	/* The value bitmask
	 */
	uint32_t value_bitmask;
};

typedef struct libfmos_lzfse_decoder libfmos_lzfse_decoder_t;

struct libfmos_lzfse_decoder
{
	/* The number of literals
	 */
	uint32_t number_of_literals;

	/* The number of L, M, D values
	 */
	uint32_t number_of_lmd_values;

	/* The literals data size
	 */
	uint32_t literals_data_size;

	/* The L, M, D values data size
	 */
	uint32_t lmd_values_data_size;

	/* The literal states
	 */
	uint16_t literal_states[ 4 ];

	/* The L value states
	 */
	uint16_t l_value_state;

	/* The M value states
	 */
	uint16_t m_value_state;

	/* The D value states
	 */
	uint16_t d_value_state;

	/* TODO
	 */
	int32_t literal_bits;

	/* TODO
	 */
	int32_t lmd_values_bits;

	/* The literal decoder table
	 */
	libfmos_lzfse_decoder_entry_t literal_decoder_table[ LIBFMOS_LZFSE_NUMBER_OF_LITERAL_STATES ];

	/* The L value decoder table
	 */
	libfmos_lzfse_value_decoder_entry_t l_value_decoder_table[ LIBFMOS_LZFSE_NUMBER_OF_L_VALUE_STATES ];

	/* The M value decoder table
	 */
	libfmos_lzfse_value_decoder_entry_t m_value_decoder_table[ LIBFMOS_LZFSE_NUMBER_OF_M_VALUE_STATES ];

	/* The D value decoder table
	 */
	libfmos_lzfse_value_decoder_entry_t d_value_decoder_table[ LIBFMOS_LZFSE_NUMBER_OF_D_VALUE_STATES ];
};

int libfmos_lzfse_decoder_initialize(
     libfmos_lzfse_decoder_t **decoder,
     libcerror_error_t **error );

int libfmos_lzfse_decoder_free(
     libfmos_lzfse_decoder_t **decoder,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFMOS_LZFSE_DECODER_H ) */

