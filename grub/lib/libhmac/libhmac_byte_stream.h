/*
 * Byte stream functions
 *
 * Copyright (C) 2011-2023, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _LIBHMAC_BYTE_STREAM_H )
#define _LIBHMAC_BYTE_STREAM_H

#include <libyal_wrap.h>

#if defined( __cplusplus )
extern "C" {
#endif

#define libhmac_byte_stream_copy_to_1x_uint32_big_endian( byte_stream, values, values_index ) \
	values[ values_index ]   = ( byte_stream )[ ( values_index << 2 ) ]; \
	values[ values_index ] <<= 8; \
	values[ values_index ]  |= ( byte_stream )[ ( values_index << 2 ) + 1 ]; \
	values[ values_index ] <<= 8; \
	values[ values_index ]  |= ( byte_stream )[ ( values_index << 2 ) + 2 ]; \
	values[ values_index ] <<= 8; \
	values[ values_index ]  |= ( byte_stream )[ ( values_index << 2 ) + 3 ];

#define libhmac_byte_stream_copy_to_1x_uint32_little_endian( byte_stream, values, values_index ) \
	values[ values_index ]   = ( byte_stream )[ ( values_index << 2 ) + 3 ]; \
	values[ values_index ] <<= 8; \
	values[ values_index ]  |= ( byte_stream )[ ( values_index << 2 ) + 2 ]; \
	values[ values_index ] <<= 8; \
	values[ values_index ]  |= ( byte_stream )[ ( values_index << 2 ) + 1 ]; \
	values[ values_index ] <<= 8; \
	values[ values_index ]  |= ( byte_stream )[ ( values_index << 2 ) ];

#define libhmac_byte_stream_copy_to_16x_uint32_big_endian( byte_stream, values ) \
	libhmac_byte_stream_copy_to_1x_uint32_big_endian( byte_stream, values, 0 ) \
	libhmac_byte_stream_copy_to_1x_uint32_big_endian( byte_stream, values, 1 ) \
	libhmac_byte_stream_copy_to_1x_uint32_big_endian( byte_stream, values, 2 ) \
	libhmac_byte_stream_copy_to_1x_uint32_big_endian( byte_stream, values, 3 ) \
	libhmac_byte_stream_copy_to_1x_uint32_big_endian( byte_stream, values, 4 ) \
	libhmac_byte_stream_copy_to_1x_uint32_big_endian( byte_stream, values, 5 ) \
	libhmac_byte_stream_copy_to_1x_uint32_big_endian( byte_stream, values, 6 ) \
	libhmac_byte_stream_copy_to_1x_uint32_big_endian( byte_stream, values, 7 ) \
	libhmac_byte_stream_copy_to_1x_uint32_big_endian( byte_stream, values, 8 ) \
	libhmac_byte_stream_copy_to_1x_uint32_big_endian( byte_stream, values, 9 ) \
	libhmac_byte_stream_copy_to_1x_uint32_big_endian( byte_stream, values, 10 ) \
	libhmac_byte_stream_copy_to_1x_uint32_big_endian( byte_stream, values, 11 ) \
	libhmac_byte_stream_copy_to_1x_uint32_big_endian( byte_stream, values, 12 ) \
	libhmac_byte_stream_copy_to_1x_uint32_big_endian( byte_stream, values, 13 ) \
	libhmac_byte_stream_copy_to_1x_uint32_big_endian( byte_stream, values, 14 ) \
	libhmac_byte_stream_copy_to_1x_uint32_big_endian( byte_stream, values, 15 )

#define libhmac_byte_stream_copy_to_16x_uint32_little_endian( byte_stream, values ) \
	libhmac_byte_stream_copy_to_1x_uint32_little_endian( byte_stream, values, 0 ) \
	libhmac_byte_stream_copy_to_1x_uint32_little_endian( byte_stream, values, 1 ) \
	libhmac_byte_stream_copy_to_1x_uint32_little_endian( byte_stream, values, 2 ) \
	libhmac_byte_stream_copy_to_1x_uint32_little_endian( byte_stream, values, 3 ) \
	libhmac_byte_stream_copy_to_1x_uint32_little_endian( byte_stream, values, 4 ) \
	libhmac_byte_stream_copy_to_1x_uint32_little_endian( byte_stream, values, 5 ) \
	libhmac_byte_stream_copy_to_1x_uint32_little_endian( byte_stream, values, 6 ) \
	libhmac_byte_stream_copy_to_1x_uint32_little_endian( byte_stream, values, 7 ) \
	libhmac_byte_stream_copy_to_1x_uint32_little_endian( byte_stream, values, 8 ) \
	libhmac_byte_stream_copy_to_1x_uint32_little_endian( byte_stream, values, 9 ) \
	libhmac_byte_stream_copy_to_1x_uint32_little_endian( byte_stream, values, 10 ) \
	libhmac_byte_stream_copy_to_1x_uint32_little_endian( byte_stream, values, 11 ) \
	libhmac_byte_stream_copy_to_1x_uint32_little_endian( byte_stream, values, 12 ) \
	libhmac_byte_stream_copy_to_1x_uint32_little_endian( byte_stream, values, 13 ) \
	libhmac_byte_stream_copy_to_1x_uint32_little_endian( byte_stream, values, 14 ) \
	libhmac_byte_stream_copy_to_1x_uint32_little_endian( byte_stream, values, 15 )

#define libhmac_byte_stream_copy_to_1x_uint64_big_endian( byte_stream, values, values_index ) \
	values[ values_index ]   = ( byte_stream )[ ( values_index << 3 ) ]; \
	values[ values_index ] <<= 8; \
	values[ values_index ]  |= ( byte_stream )[ ( values_index << 3 ) + 1 ]; \
	values[ values_index ] <<= 8; \
	values[ values_index ]  |= ( byte_stream )[ ( values_index << 3 ) + 2 ]; \
	values[ values_index ] <<= 8; \
	values[ values_index ]  |= ( byte_stream )[ ( values_index << 3 ) + 3 ]; \
	values[ values_index ] <<= 8; \
	values[ values_index ]  |= ( byte_stream )[ ( values_index << 3 ) + 4 ]; \
	values[ values_index ] <<= 8; \
	values[ values_index ]  |= ( byte_stream )[ ( values_index << 3 ) + 5 ]; \
	values[ values_index ] <<= 8; \
	values[ values_index ]  |= ( byte_stream )[ ( values_index << 3 ) + 6 ]; \
	values[ values_index ] <<= 8; \
	values[ values_index ]  |= ( byte_stream )[ ( values_index << 3 ) + 7 ];

#define libhmac_byte_stream_copy_to_1x_uint64_little_endian( byte_stream, values, values_index ) \
	values[ values_index ]   = ( byte_stream )[ ( values_index << 3 ) + 7 ]; \
	values[ values_index ] <<= 8; \
	values[ values_index ]  |= ( byte_stream )[ ( values_index << 3 ) + 6 ]; \
	values[ values_index ] <<= 8; \
	values[ values_index ]  |= ( byte_stream )[ ( values_index << 3 ) + 5 ]; \
	values[ values_index ] <<= 8; \
	values[ values_index ]  |= ( byte_stream )[ ( values_index << 3 ) + 4 ]; \
	values[ values_index ] <<= 8; \
	values[ values_index ]   = ( byte_stream )[ ( values_index << 3 ) + 3 ]; \
	values[ values_index ] <<= 8; \
	values[ values_index ]  |= ( byte_stream )[ ( values_index << 3 ) + 2 ]; \
	values[ values_index ] <<= 8; \
	values[ values_index ]  |= ( byte_stream )[ ( values_index << 3 ) + 1 ]; \
	values[ values_index ] <<= 8; \
	values[ values_index ]  |= ( byte_stream )[ ( values_index << 3 ) ];

#define libhmac_byte_stream_copy_to_16x_uint64_big_endian( byte_stream, values ) \
	libhmac_byte_stream_copy_to_1x_uint64_big_endian( byte_stream, values, 0 ) \
	libhmac_byte_stream_copy_to_1x_uint64_big_endian( byte_stream, values, 1 ) \
	libhmac_byte_stream_copy_to_1x_uint64_big_endian( byte_stream, values, 2 ) \
	libhmac_byte_stream_copy_to_1x_uint64_big_endian( byte_stream, values, 3 ) \
	libhmac_byte_stream_copy_to_1x_uint64_big_endian( byte_stream, values, 4 ) \
	libhmac_byte_stream_copy_to_1x_uint64_big_endian( byte_stream, values, 5 ) \
	libhmac_byte_stream_copy_to_1x_uint64_big_endian( byte_stream, values, 6 ) \
	libhmac_byte_stream_copy_to_1x_uint64_big_endian( byte_stream, values, 7 ) \
	libhmac_byte_stream_copy_to_1x_uint64_big_endian( byte_stream, values, 8 ) \
	libhmac_byte_stream_copy_to_1x_uint64_big_endian( byte_stream, values, 9 ) \
	libhmac_byte_stream_copy_to_1x_uint64_big_endian( byte_stream, values, 10 ) \
	libhmac_byte_stream_copy_to_1x_uint64_big_endian( byte_stream, values, 11 ) \
	libhmac_byte_stream_copy_to_1x_uint64_big_endian( byte_stream, values, 12 ) \
	libhmac_byte_stream_copy_to_1x_uint64_big_endian( byte_stream, values, 13 ) \
	libhmac_byte_stream_copy_to_1x_uint64_big_endian( byte_stream, values, 14 ) \
	libhmac_byte_stream_copy_to_1x_uint64_big_endian( byte_stream, values, 15 )

#define libhmac_byte_stream_copy_to_16x_uint64_little_endian( byte_stream, values ) \
	libhmac_byte_stream_copy_to_1x_uint64_little_endian( byte_stream, values, 0 ) \
	libhmac_byte_stream_copy_to_1x_uint64_little_endian( byte_stream, values, 1 ) \
	libhmac_byte_stream_copy_to_1x_uint64_little_endian( byte_stream, values, 2 ) \
	libhmac_byte_stream_copy_to_1x_uint64_little_endian( byte_stream, values, 3 ) \
	libhmac_byte_stream_copy_to_1x_uint64_little_endian( byte_stream, values, 4 ) \
	libhmac_byte_stream_copy_to_1x_uint64_little_endian( byte_stream, values, 5 ) \
	libhmac_byte_stream_copy_to_1x_uint64_little_endian( byte_stream, values, 6 ) \
	libhmac_byte_stream_copy_to_1x_uint64_little_endian( byte_stream, values, 7 ) \
	libhmac_byte_stream_copy_to_1x_uint64_little_endian( byte_stream, values, 8 ) \
	libhmac_byte_stream_copy_to_1x_uint64_little_endian( byte_stream, values, 9 ) \
	libhmac_byte_stream_copy_to_1x_uint64_little_endian( byte_stream, values, 10 ) \
	libhmac_byte_stream_copy_to_1x_uint64_little_endian( byte_stream, values, 11 ) \
	libhmac_byte_stream_copy_to_1x_uint64_little_endian( byte_stream, values, 12 ) \
	libhmac_byte_stream_copy_to_1x_uint64_little_endian( byte_stream, values, 13 ) \
	libhmac_byte_stream_copy_to_1x_uint64_little_endian( byte_stream, values, 14 ) \
	libhmac_byte_stream_copy_to_1x_uint64_little_endian( byte_stream, values, 15 )

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBHMAC_BYTE_STREAM_H ) */

