/*
 * Bit-stream functions
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

#if !defined( _LIBFWNT_BIT_STREAM_H )
#define _LIBFWNT_BIT_STREAM_H

#include <libyal_wrap.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfwnt_bit_stream libfwnt_bit_stream_t;

struct libfwnt_bit_stream
{
	/* The byte stream
	 */
	const uint8_t *byte_stream;

	/* The byte stream size
	 */
	size_t byte_stream_size;

	/* The byte stream offset
	 */
	size_t byte_stream_offset;

	/* The bit buffer
	 */
	uint32_t bit_buffer;

	/* The number of bits in the bit buffer
	 */
	uint8_t bit_buffer_size;
};

int libfwnt_bit_stream_initialize(
     libfwnt_bit_stream_t **bit_stream,
     const uint8_t *byte_stream,
     size_t byte_stream_size,
     libcerror_error_t **error );

int libfwnt_bit_stream_free(
     libfwnt_bit_stream_t **bit_stream,
     libcerror_error_t **error );

int libfwnt_bit_stream_read(
     libfwnt_bit_stream_t *bit_stream,
     uint8_t number_of_bits,
     libcerror_error_t **error );

int libfwnt_bit_stream_get_value(
     libfwnt_bit_stream_t *bit_stream,
     uint8_t number_of_bits,
     uint32_t *value_32bit,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFWNT_BIT_STREAM_H ) */

