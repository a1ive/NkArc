/*
 * MD5 functions
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

#include <libyal_wrap.h>

#if defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_MD5_H )
#include <openssl/md5.h>

#elif defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_EVP_H )
#include <openssl/err.h>
#include <openssl/evp.h>

#endif

#include "libhmac_byte_stream.h"
#include "libhmac_definitions.h"
#include "libhmac_md5_context.h"

#if !defined( LIBHMAC_HAVE_MD5_SUPPORT )

/* RFC 1321 based MD5 functions
 */

/* TODO decription what these values are based on
 */
uint32_t libhmac_md5_context_fixed_constants[ 4 ] = {
	0x67452301UL, 0xefcdab89UL, 0x98badcfeUL, 0x10325476UL
};

/* The first 32-bits of the sines (in radians) of the first 64 integers [ 0, 63 ]
 */
uint32_t libhmac_md5_context_sines[ 64 ] = {
	0xd76aa478UL, 0xe8c7b756UL, 0x242070dbUL, 0xc1bdceeeUL,
	0xf57c0fafUL, 0x4787c62aUL, 0xa8304613UL, 0xfd469501UL,
	0x698098d8UL, 0x8b44f7afUL, 0xffff5bb1UL, 0x895cd7beUL,
	0x6b901122UL, 0xfd987193UL, 0xa679438eUL, 0x49b40821UL,
	0xf61e2562UL, 0xc040b340UL, 0x265e5a51UL, 0xe9b6c7aaUL,
	0xd62f105dUL, 0x02441453UL, 0xd8a1e681UL, 0xe7d3fbc8UL,
	0x21e1cde6UL, 0xc33707d6UL, 0xf4d50d87UL, 0x455a14edUL,
	0xa9e3e905UL, 0xfcefa3f8UL, 0x676f02d9UL, 0x8d2a4c8aUL,
	0xfffa3942UL, 0x8771f681UL, 0x6d9d6122UL, 0xfde5380cUL,
	0xa4beea44UL, 0x4bdecfa9UL, 0xf6bb4b60UL, 0xbebfbc70UL,
	0x289b7ec6UL, 0xeaa127faUL, 0xd4ef3085UL, 0x04881d05UL,
	0xd9d4d039UL, 0xe6db99e5UL, 0x1fa27cf8UL, 0xc4ac5665UL,
	0xf4292244UL, 0x432aff97UL, 0xab9423a7UL, 0xfc93a039UL,
	0x655b59c3UL, 0x8f0ccc92UL, 0xffeff47dUL, 0x85845dd1UL,
	0x6fa87e4fUL, 0xfe2ce6e0UL, 0xa3014314UL, 0x4e0811a1UL,
	0xf7537e82UL, 0xbd3af235UL, 0x2ad7d2bbUL, 0xeb86d391UL
};

/* The bit shifts
 */
uint8_t libhmac_md5_context_bit_shifts[ 64 ] = {
	7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
	5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20,
	4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
	6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21
};

/* The 32-bit values indexes
 * [  0, 15 ] => index
 * [ 16, 31 ] => ( ( 5 x index ) + 1 ) mod 16
 * [ 32, 47 ] => ( ( 3 x index ) + 5 ) mod 16
 * [ 48, 63 ] => ( 7 x index ) mod 16
 */
uint8_t libhmac_md5_context_values_32bit_index[ 64 ] = {
	0, 1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15,
	1, 6, 11,  0,  5, 10, 15,  4,  9, 14,  3,  8, 13,  2,  7, 12,
	5, 8, 11, 14,  1,  4,  7, 10, 13,  0,  3,  6,  9, 12, 15,  2,
	0, 7, 14,  5, 12,  3, 10,  1,  8, 15,  6, 13,  4, 11,  2,  9
};

#define libhmac_md5_context_transform_unfolded_calculate_hash_value_round1( values_32bit, value_32bit_index, hash_values, hash_value_index0, hash_value_index1, hash_value_index2, hash_value_index3, block_index ) \
	hash_values[ hash_value_index0 ] += ( hash_values[ hash_value_index1 ] & hash_values[ hash_value_index2 ] ) \
	                                  | ( ~( hash_values[ hash_value_index1 ] ) & hash_values[ hash_value_index3 ] ); \
	hash_values[ hash_value_index0 ] += values_32bit[ value_32bit_index ]; \
	hash_values[ hash_value_index0 ] += libhmac_md5_context_sines[ block_index ]; \
\
	hash_values[ hash_value_index0 ] = byte_stream_bit_rotate_left_32bit( \
	                                    hash_values[ hash_value_index0 ],  \
	                                    libhmac_md5_context_bit_shifts[ block_index ] ); \
\
	hash_values[ hash_value_index0 ] += hash_values[ hash_value_index1 ];

#define libhmac_md5_context_transform_unfolded_calculate_hash_value_round2( values_32bit, value_32bit_index, hash_values, hash_value_index0, hash_value_index1, hash_value_index2, hash_value_index3, block_index ) \
	hash_values[ hash_value_index0 ] += ( hash_values[ hash_value_index1 ] & hash_values[ hash_value_index3 ] ) \
	                                  | ( hash_values[ hash_value_index2 ] & ~( hash_values[ hash_value_index3 ] ) ); \
	hash_values[ hash_value_index0 ] += values_32bit[ value_32bit_index ]; \
	hash_values[ hash_value_index0 ] += libhmac_md5_context_sines[ block_index ]; \
\
	hash_values[ hash_value_index0 ] = byte_stream_bit_rotate_left_32bit( \
	                                    hash_values[ hash_value_index0 ],  \
	                                    libhmac_md5_context_bit_shifts[ block_index ] ); \
\
	hash_values[ hash_value_index0 ] += hash_values[ hash_value_index1 ];

#define libhmac_md5_context_transform_unfolded_calculate_hash_value_round3( values_32bit, value_32bit_index, hash_values, hash_value_index0, hash_value_index1, hash_value_index2, hash_value_index3, block_index ) \
	hash_values[ hash_value_index0 ] += hash_values[ hash_value_index1 ] \
	                                  ^ hash_values[ hash_value_index2 ] \
	                                  ^ hash_values[ hash_value_index3 ]; \
	hash_values[ hash_value_index0 ] += values_32bit[ value_32bit_index ]; \
	hash_values[ hash_value_index0 ] += libhmac_md5_context_sines[ block_index ]; \
\
	hash_values[ hash_value_index0 ] = byte_stream_bit_rotate_left_32bit( \
	                                    hash_values[ hash_value_index0 ],  \
	                                    libhmac_md5_context_bit_shifts[ block_index ] ); \
\
	hash_values[ hash_value_index0 ] += hash_values[ hash_value_index1 ];

#define libhmac_md5_context_transform_unfolded_calculate_hash_value_round4( values_32bit, value_32bit_index, hash_values, hash_value_index0, hash_value_index1, hash_value_index2, hash_value_index3, block_index ) \
	hash_values[ hash_value_index0 ] += hash_values[ hash_value_index2 ] \
	                                  ^ ( hash_values[ hash_value_index1 ] | ~( hash_values[ hash_value_index3 ] ) ); \
	hash_values[ hash_value_index0 ] += values_32bit[ value_32bit_index ]; \
	hash_values[ hash_value_index0 ] += libhmac_md5_context_sines[ block_index ]; \
\
	hash_values[ hash_value_index0 ] = byte_stream_bit_rotate_left_32bit( \
	                                    hash_values[ hash_value_index0 ],  \
	                                    libhmac_md5_context_bit_shifts[ block_index ] ); \
\
	hash_values[ hash_value_index0 ] += hash_values[ hash_value_index1 ];

#define libhmac_md5_context_transform_unfolded_calculate_hash_values( values_32bit, hash_values ) \
	libhmac_md5_context_transform_unfolded_calculate_hash_value_round1( values_32bit, 0, hash_values, 0, 1, 2, 3, 0 ); \
	libhmac_md5_context_transform_unfolded_calculate_hash_value_round1( values_32bit, 1, hash_values, 3, 0, 1, 2, 1 ); \
	libhmac_md5_context_transform_unfolded_calculate_hash_value_round1( values_32bit, 2, hash_values, 2, 3, 0, 1, 2 ); \
	libhmac_md5_context_transform_unfolded_calculate_hash_value_round1( values_32bit, 3, hash_values, 1, 2, 3, 0, 3 ); \
	libhmac_md5_context_transform_unfolded_calculate_hash_value_round1( values_32bit, 4, hash_values, 0, 1, 2, 3, 4 ); \
	libhmac_md5_context_transform_unfolded_calculate_hash_value_round1( values_32bit, 5, hash_values, 3, 0, 1, 2, 5 ); \
	libhmac_md5_context_transform_unfolded_calculate_hash_value_round1( values_32bit, 6, hash_values, 2, 3, 0, 1, 6 ); \
	libhmac_md5_context_transform_unfolded_calculate_hash_value_round1( values_32bit, 7, hash_values, 1, 2, 3, 0, 7 ); \
	libhmac_md5_context_transform_unfolded_calculate_hash_value_round1( values_32bit, 8, hash_values, 0, 1, 2, 3, 8 ); \
	libhmac_md5_context_transform_unfolded_calculate_hash_value_round1( values_32bit, 9, hash_values, 3, 0, 1, 2, 9 ); \
	libhmac_md5_context_transform_unfolded_calculate_hash_value_round1( values_32bit, 10, hash_values, 2, 3, 0, 1, 10 ); \
	libhmac_md5_context_transform_unfolded_calculate_hash_value_round1( values_32bit, 11, hash_values, 1, 2, 3, 0, 11 ); \
	libhmac_md5_context_transform_unfolded_calculate_hash_value_round1( values_32bit, 12, hash_values, 0, 1, 2, 3, 12 ); \
	libhmac_md5_context_transform_unfolded_calculate_hash_value_round1( values_32bit, 13, hash_values, 3, 0, 1, 2, 13 ); \
	libhmac_md5_context_transform_unfolded_calculate_hash_value_round1( values_32bit, 14, hash_values, 2, 3, 0, 1, 14 ); \
	libhmac_md5_context_transform_unfolded_calculate_hash_value_round1( values_32bit, 15, hash_values, 1, 2, 3, 0, 15 ); \
\
	libhmac_md5_context_transform_unfolded_calculate_hash_value_round2( values_32bit, 1, hash_values, 0, 1, 2, 3, 16 ); \
	libhmac_md5_context_transform_unfolded_calculate_hash_value_round2( values_32bit, 6, hash_values, 3, 0, 1, 2, 17 ); \
	libhmac_md5_context_transform_unfolded_calculate_hash_value_round2( values_32bit, 11, hash_values, 2, 3, 0, 1, 18 ); \
	libhmac_md5_context_transform_unfolded_calculate_hash_value_round2( values_32bit, 0, hash_values, 1, 2, 3, 0, 19 ); \
	libhmac_md5_context_transform_unfolded_calculate_hash_value_round2( values_32bit, 5, hash_values, 0, 1, 2, 3, 20 ); \
	libhmac_md5_context_transform_unfolded_calculate_hash_value_round2( values_32bit, 10, hash_values, 3, 0, 1, 2, 21 ); \
	libhmac_md5_context_transform_unfolded_calculate_hash_value_round2( values_32bit, 15, hash_values, 2, 3, 0, 1, 22 ); \
	libhmac_md5_context_transform_unfolded_calculate_hash_value_round2( values_32bit, 4, hash_values, 1, 2, 3, 0, 23 ); \
	libhmac_md5_context_transform_unfolded_calculate_hash_value_round2( values_32bit, 9, hash_values, 0, 1, 2, 3, 24 ); \
	libhmac_md5_context_transform_unfolded_calculate_hash_value_round2( values_32bit, 14, hash_values, 3, 0, 1, 2, 25 ); \
	libhmac_md5_context_transform_unfolded_calculate_hash_value_round2( values_32bit, 3, hash_values, 2, 3, 0, 1, 26 ); \
	libhmac_md5_context_transform_unfolded_calculate_hash_value_round2( values_32bit, 8, hash_values, 1, 2, 3, 0, 27 ); \
	libhmac_md5_context_transform_unfolded_calculate_hash_value_round2( values_32bit, 13, hash_values, 0, 1, 2, 3, 28 ); \
	libhmac_md5_context_transform_unfolded_calculate_hash_value_round2( values_32bit, 2, hash_values, 3, 0, 1, 2, 29 ); \
	libhmac_md5_context_transform_unfolded_calculate_hash_value_round2( values_32bit, 7, hash_values, 2, 3, 0, 1, 30 ); \
	libhmac_md5_context_transform_unfolded_calculate_hash_value_round2( values_32bit, 12, hash_values, 1, 2, 3, 0, 31 ); \
\
	libhmac_md5_context_transform_unfolded_calculate_hash_value_round3( values_32bit, 5, hash_values, 0, 1, 2, 3, 32 ); \
	libhmac_md5_context_transform_unfolded_calculate_hash_value_round3( values_32bit, 8, hash_values, 3, 0, 1, 2, 33 ); \
	libhmac_md5_context_transform_unfolded_calculate_hash_value_round3( values_32bit, 11, hash_values, 2, 3, 0, 1, 34 ); \
	libhmac_md5_context_transform_unfolded_calculate_hash_value_round3( values_32bit, 14, hash_values, 1, 2, 3, 0, 35 ); \
	libhmac_md5_context_transform_unfolded_calculate_hash_value_round3( values_32bit, 1, hash_values, 0, 1, 2, 3, 36 ); \
	libhmac_md5_context_transform_unfolded_calculate_hash_value_round3( values_32bit, 4, hash_values, 3, 0, 1, 2, 37 ); \
	libhmac_md5_context_transform_unfolded_calculate_hash_value_round3( values_32bit, 7, hash_values, 2, 3, 0, 1, 38 ); \
	libhmac_md5_context_transform_unfolded_calculate_hash_value_round3( values_32bit, 10, hash_values, 1, 2, 3, 0, 39 ); \
	libhmac_md5_context_transform_unfolded_calculate_hash_value_round3( values_32bit, 13, hash_values, 0, 1, 2, 3, 40 ); \
	libhmac_md5_context_transform_unfolded_calculate_hash_value_round3( values_32bit, 0, hash_values, 3, 0, 1, 2, 41 ); \
	libhmac_md5_context_transform_unfolded_calculate_hash_value_round3( values_32bit, 3, hash_values, 2, 3, 0, 1, 42 ); \
	libhmac_md5_context_transform_unfolded_calculate_hash_value_round3( values_32bit, 6, hash_values, 1, 2, 3, 0, 43 ); \
	libhmac_md5_context_transform_unfolded_calculate_hash_value_round3( values_32bit, 9, hash_values, 0, 1, 2, 3, 44 ); \
	libhmac_md5_context_transform_unfolded_calculate_hash_value_round3( values_32bit, 12, hash_values, 3, 0, 1, 2, 45 ); \
	libhmac_md5_context_transform_unfolded_calculate_hash_value_round3( values_32bit, 15, hash_values, 2, 3, 0, 1, 46 ); \
	libhmac_md5_context_transform_unfolded_calculate_hash_value_round3( values_32bit, 2, hash_values, 1, 2, 3, 0, 47 ); \
\
	libhmac_md5_context_transform_unfolded_calculate_hash_value_round4( values_32bit, 0, hash_values, 0, 1, 2, 3, 48 ); \
	libhmac_md5_context_transform_unfolded_calculate_hash_value_round4( values_32bit, 7, hash_values, 3, 0, 1, 2, 49 ); \
	libhmac_md5_context_transform_unfolded_calculate_hash_value_round4( values_32bit, 14, hash_values, 2, 3, 0, 1, 50 ); \
	libhmac_md5_context_transform_unfolded_calculate_hash_value_round4( values_32bit, 5, hash_values, 1, 2, 3, 0, 51 ); \
	libhmac_md5_context_transform_unfolded_calculate_hash_value_round4( values_32bit, 12, hash_values, 0, 1, 2, 3, 52 ); \
	libhmac_md5_context_transform_unfolded_calculate_hash_value_round4( values_32bit, 3, hash_values, 3, 0, 1, 2, 53 ); \
	libhmac_md5_context_transform_unfolded_calculate_hash_value_round4( values_32bit, 10, hash_values, 2, 3, 0, 1, 54 ); \
	libhmac_md5_context_transform_unfolded_calculate_hash_value_round4( values_32bit, 1, hash_values, 1, 2, 3, 0, 55 ); \
	libhmac_md5_context_transform_unfolded_calculate_hash_value_round4( values_32bit, 8, hash_values, 0, 1, 2, 3, 56 ); \
	libhmac_md5_context_transform_unfolded_calculate_hash_value_round4( values_32bit, 15, hash_values, 3, 0, 1, 2, 57 ); \
	libhmac_md5_context_transform_unfolded_calculate_hash_value_round4( values_32bit, 6, hash_values, 2, 3, 0, 1, 58 ); \
	libhmac_md5_context_transform_unfolded_calculate_hash_value_round4( values_32bit, 13, hash_values, 1, 2, 3, 0, 59 ); \
	libhmac_md5_context_transform_unfolded_calculate_hash_value_round4( values_32bit, 4, hash_values, 0, 1, 2, 3, 60 ); \
	libhmac_md5_context_transform_unfolded_calculate_hash_value_round4( values_32bit, 11, hash_values, 3, 0, 1, 2, 61 ); \
	libhmac_md5_context_transform_unfolded_calculate_hash_value_round4( values_32bit, 2, hash_values, 2, 3, 0, 1, 62 ); \
	libhmac_md5_context_transform_unfolded_calculate_hash_value_round4( values_32bit, 9, hash_values, 1, 2, 3, 0, 63 );

/* Calculates the MD5 of 64 byte sized blocks of data in a buffer
 * Returns the number of bytes used if successful or -1 on error
 */
ssize_t libhmac_md5_context_transform(
         libhmac_internal_md5_context_t *internal_context,
         const uint8_t *buffer,
         size_t size,
         libcerror_error_t **error )
{
	uint32_t hash_values[ 4 ];
	uint32_t values_32bit[ 16 ];

	static char *function     = "libhmac_md5_context_transform";
	size_t buffer_offset      = 0;

#if !defined( LIBHMAC_UNFOLLED_LOOPS )
	uint32_t hash_value       = 0;
	uint8_t block_index       = 0;
	uint8_t hash_values_index = 0;
	uint8_t value_32bit_index = 0;
#endif

	if( internal_context == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid internal context.",
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
	while( size >= LIBHMAC_MD5_BLOCK_SIZE )
	{
		if( memory_copy(
		     hash_values,
		     internal_context->hash_values,
		     sizeof( uint32_t ) * 4 ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy hash values.",
			 function );

			goto on_error;
		}
#if defined( LIBHMAC_UNFOLLED_LOOPS )
		/* Break the block into 16 x 32-bit values
		 */
		libhmac_byte_stream_copy_to_16x_uint32_little_endian(
		 &( buffer[ buffer_offset ] ),
		 values_32bit );

		buffer_offset += LIBHMAC_MD5_BLOCK_SIZE;

		/* Calculate the hash values for the 32-bit values
		 */
		libhmac_md5_context_transform_unfolded_calculate_hash_values(
		 values_32bit,
		 hash_values );

		/* Update the hash values in the context
		 */
		internal_context->hash_values[ 0 ] += hash_values[ 0 ];
		internal_context->hash_values[ 1 ] += hash_values[ 1 ];
		internal_context->hash_values[ 2 ] += hash_values[ 2 ];
		internal_context->hash_values[ 3 ] += hash_values[ 3 ];

#else
		/* Break the block into 16 x 32-bit values
		 */
		for( value_32bit_index = 0;
		     value_32bit_index < 16;
		     value_32bit_index++ )
		{
			byte_stream_copy_to_uint32_little_endian(
			 &( buffer[ buffer_offset ] ),
			 values_32bit[ value_32bit_index ] );

			buffer_offset += sizeof( uint32_t );
		}
		/* Calculate the hash values for the 32-bit values
		 */
		for( block_index = 0;
		     block_index < 64;
		     block_index++ )
		{
			if( block_index < 16 )
			{
				hash_values[ 0 ] += ( hash_values[ 1 ] & hash_values[ 2 ] )
				                  | ( ~( hash_values[ 1 ] ) & hash_values[ 3 ] );
			}
			else if( block_index < 32 )
			{
				hash_values[ 0 ] += ( hash_values[ 1 ] & hash_values[ 3 ] )
				                  | ( hash_values[ 2 ] & ~( hash_values[ 3 ] ) );
			}
			else if( block_index < 48 )
			{
				hash_values[ 0 ] += hash_values[ 1 ]
				                  ^ hash_values[ 2 ]
				                  ^ hash_values[ 3 ];
			}
			else
			{
				hash_values[ 0 ] += hash_values[ 2 ]
				                  ^ ( hash_values[ 1 ] | ~( hash_values[ 3 ] ) );
			}
			value_32bit_index = libhmac_md5_context_values_32bit_index[ block_index ];

			hash_values[ 0 ] += values_32bit[ value_32bit_index ];
			hash_values[ 0 ] += libhmac_md5_context_sines[ block_index ];
			hash_values[ 0 ]  = byte_stream_bit_rotate_left_32bit(
			                     hash_values[ 0 ],
			                     libhmac_md5_context_bit_shifts[ block_index ] );

			hash_value        = hash_values[ 3 ];
			hash_values[ 3 ]  = hash_values[ 2 ];
			hash_values[ 2 ]  = hash_values[ 1 ];
			hash_values[ 1 ] += hash_values[ 0 ];
			hash_values[ 0 ]  = hash_value;
		}
		/* Update the hash values in the context
		 */
		for( hash_values_index = 0;
		     hash_values_index < 4;
		     hash_values_index++ )
		{
			internal_context->hash_values[ hash_values_index ] += hash_values[ hash_values_index ];
		}
#endif /* defined( LIBHMAC_UNFOLLED_LOOPS ) */

		size -= LIBHMAC_MD5_BLOCK_SIZE;
	}
	/* Prevent sensitive data from leaking
	 */
	if( memory_set(
	     hash_values,
	     0,
	     sizeof( uint32_t ) * 4 ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear hash values.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     values_32bit,
	     0,
	     sizeof( uint32_t ) * 16 ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear 32-bit values.",
		 function );

		goto on_error;
	}
	return( (ssize_t) buffer_offset );

on_error:
	memory_set(
	 values_32bit,
	 0,
	 sizeof( uint32_t ) * 16 );

	memory_set(
	 hash_values,
	 0,
	 sizeof( uint32_t ) * 4 );

	return( -1 );
}

#endif /* !defined( LIBHMAC_HAVE_MD5_SUPPORT ) */

/* Creates a MD5 context
 * Make sure the value context is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libhmac_md5_context_initialize(
     libhmac_md5_context_t **context,
     libcerror_error_t **error )
{
	libhmac_internal_md5_context_t *internal_context = NULL;
	static char *function                            = "libhmac_md5_context_initialize";

#if defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_EVP_H ) && defined( HAVE_EVP_MD5 )
	const EVP_MD *evp_md_type                        = NULL;
#endif

	if( context == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid context.",
		 function );

		return( -1 );
	}
	if( *context != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid context value already set.",
		 function );

		return( -1 );
	}
	internal_context = memory_allocate_structure(
	                    libhmac_internal_md5_context_t );

	if( internal_context == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create context.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     internal_context,
	     0,
	     sizeof( libhmac_internal_md5_context_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear context.",
		 function );

		memory_free(
		 internal_context );

		return( -1 );
	}
#if defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_MD5_H ) && defined( MD5_DIGEST_LENGTH )
	if( MD5_Init(
	     &( internal_context->md5_context ) ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize context.",
		 function );

		goto on_error;
	}

#elif defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_EVP_H ) && defined( HAVE_EVP_MD5 )
#if defined( HAVE_EVP_MD_CTX_INIT )
	EVP_MD_CTX_init(
	 &( internal_context->internal_evp_md_context ) );

	internal_context->evp_md_context = &( internal_context->internal_evp_md_context );
#else
	internal_context->evp_md_context = EVP_MD_CTX_new();

	if( internal_context->evp_md_context == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create EVP message digest context.",
		 function );

		goto on_error;
	}
#endif /* defined( HAVE_EVP_MD_CTX_INIT ) */

/* TODO use EVP_MD_fetch for EVP_DigestInit_ex2 */
	evp_md_type = EVP_md5();

#if defined( HAVE_EVP_DIGESTINIT_EX2 )
	if( EVP_DigestInit_ex2(
	     internal_context->evp_md_context,
	     evp_md_type,
	     NULL ) != 1 )
#else
	if( EVP_DigestInit_ex(
	     internal_context->evp_md_context,
	     evp_md_type,
	     NULL ) != 1 )
#endif
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize EVP message digest context.",
		 function );

#if defined( HAVE_EVP_MD_CTX_CLEANUP )
		EVP_MD_CTX_cleanup(
		 &( internal_context->internal_evp_md_context ) );
		ERR_remove_thread_state(
		 NULL );
#else
		EVP_MD_CTX_free(
		 internal_context->evp_md_context );
#endif
		internal_context->evp_md_context = NULL;

		goto on_error;
	}
#else
	if( memory_copy(
	     internal_context->hash_values,
	     libhmac_md5_context_fixed_constants,
	     sizeof( uint32_t ) * 4 ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy fixed constants to hash values.",
		 function );

		goto on_error;
	}
#endif /* defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_MD5_H ) && defined( MD5_DIGEST_LENGTH ) */

	*context = (libhmac_md5_context_t *) internal_context;

	return( 1 );

on_error:
	if( internal_context != NULL )
	{
		memory_free(
		 internal_context );
	}
	return( -1 );
}

/* Frees a MD5 context
 * Returns 1 if successful or -1 on error
 */
int libhmac_md5_context_free(
     libhmac_md5_context_t **context,
     libcerror_error_t **error )
{
	libhmac_internal_md5_context_t *internal_context = NULL;
	static char *function                            = "libhmac_md5_context_free";

	if( context == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid context.",
		 function );

		return( -1 );
	}
	if( *context != NULL )
	{
		internal_context = (libhmac_internal_md5_context_t *) *context;
		*context         = NULL;

#if defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_MD5_H ) && defined( MD5_DIGEST_LENGTH )
		/* No additional clean up necessary
		 */

#elif defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_EVP_H ) && defined( HAVE_EVP_MD5 )
#if defined( HAVE_EVP_MD_CTX_CLEANUP )
		if( EVP_MD_CTX_cleanup(
		     &( internal_context->internal_evp_md_context ) ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to clean up EVP message digest context.",
			 function );
		}
		/* Make sure the error state is removed otherwise OpenSSL will leak memory
		 */
		ERR_remove_thread_state(
		 NULL );
#else
		EVP_MD_CTX_free(
		 internal_context->evp_md_context );

#endif /* defined( HAVE_EVP_MD_CTX_CLEANUP ) */

		internal_context->evp_md_context = NULL;
#else
		/* No additional clean up necessary
		 */
#endif /* defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_MD5_H ) && defined( MD5_DIGEST_LENGTH ) */

		memory_free(
		 internal_context );
	}
	return( 1 );
}

#if defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_MD5_H ) && defined( MD5_DIGEST_LENGTH )

/* Updates the MD5 context using OpenSSL
 * Returns 1 if successful or -1 on error
 */
int libhmac_md5_context_update(
     libhmac_md5_context_t *context,
     const uint8_t *buffer,
     size_t size,
     libcerror_error_t **error )
{
	libhmac_internal_md5_context_t *internal_context = NULL;
	static char *function                            = "libhmac_md5_context_update";
	unsigned long safe_hash_size                     = 0;

	if( context == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid context.",
		 function );

		return( -1 );
	}
	internal_context = (libhmac_internal_md5_context_t *) context;

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
#if ( SIZEOF_LONG < SIZEOF_SIZE_T )
	if( size > (size_t) ULONG_MAX )
#else
	if( size > (size_t) SSIZE_MAX )
#endif
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( size == 0 )
	{
		return( 1 );
	}
	safe_hash_size = (unsigned long) size;

	if( MD5_Update(
	     &( internal_context->md5_context ),
	     (const void *) buffer,
	     size ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to update context.",
		 function );

		return( -1 );
	}
	return( 1 );
}

#elif defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_EVP_H ) && defined( HAVE_EVP_MD5 )

/* Updates the MD5 context using OpenSSL EVP
 * Returns 1 if successful or -1 on error
 */
int libhmac_md5_context_update(
     libhmac_md5_context_t *context,
     const uint8_t *buffer,
     size_t size,
     libcerror_error_t **error )
{
	libhmac_internal_md5_context_t *internal_context = NULL;
	static char *function                            = "libhmac_md5_context_update";

	if( context == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid context.",
		 function );

		return( -1 );
	}
	internal_context = (libhmac_internal_md5_context_t *) context;

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
	if( size == 0 )
	{
		return( 1 );
	}
	if( EVP_DigestUpdate(
	     internal_context->evp_md_context,
	     (const void *) buffer,
	     size ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to update context.",
		 function );

		return( -1 );
	}
	return( 1 );
}

#else

/* Updates the MD5 context using fallback implementation
 * Returns 1 if successful or -1 on error
 */
int libhmac_md5_context_update(
     libhmac_md5_context_t *context,
     const uint8_t *buffer,
     size_t size,
     libcerror_error_t **error )
{
	libhmac_internal_md5_context_t *internal_context = NULL;
	static char *function                            = "libhmac_md5_context_update";
	size_t buffer_offset                             = 0;
	size_t remaining_block_size                      = 0;
	ssize_t process_count                            = 0;

	if( context == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid context.",
		 function );

		return( -1 );
	}
	internal_context = (libhmac_internal_md5_context_t *) context;

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
	if( size == 0 )
	{
		return( 1 );
	}
	if( internal_context->block_offset > 0 )
	{
		if( internal_context->block_offset >= LIBHMAC_MD5_BLOCK_SIZE )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid context - block offset value out of bounds.",
			 function );

			return( -1 );
		}
		remaining_block_size = LIBHMAC_MD5_BLOCK_SIZE - internal_context->block_offset;

		if( remaining_block_size > size )
		{
			remaining_block_size = size;
		}
		if( memory_copy(
		     &( internal_context->block[ internal_context->block_offset ] ),
		     buffer,
		     remaining_block_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy data to context block.",
			 function );

			return( -1 );
		}
		internal_context->block_offset += remaining_block_size;

		if( internal_context->block_offset < LIBHMAC_MD5_BLOCK_SIZE )
		{
			return( 1 );
		}
		buffer_offset += remaining_block_size;
		size          -= remaining_block_size;

		process_count = libhmac_md5_context_transform(
		                 internal_context,
		                 internal_context->block,
		                 LIBHMAC_MD5_BLOCK_SIZE,
		                 error );

		if( process_count == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to transform context block.",
			 function );

			return( -1 );
		}
		internal_context->hash_count  += process_count;
		internal_context->block_offset = 0;
	}
	if( size > 0 )
	{
		process_count = libhmac_md5_context_transform(
		                 internal_context,
		                 &( buffer[ buffer_offset ] ),
		                 size,
		                 error );

		if( process_count == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to transform buffer.",
			 function );

			return( -1 );
		}
		internal_context->hash_count += process_count;

		buffer_offset += process_count;
		size          -= process_count;
	}
	if( size > 0 )
	{
		if( size >= LIBHMAC_MD5_BLOCK_SIZE )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid size value out of bounds.",
			 function );

			return( -1 );
		}
		if( memory_copy(
		     internal_context->block,
		     &( buffer[ buffer_offset ] ),
		     size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy remaining data to context block.",
			 function );

			return( -1 );
		}
		internal_context->block_offset = size;
	}
	return( 1 );
}

#endif /* defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_MD5_H ) && defined( MD5_DIGEST_LENGTH ) */

#if defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_MD5_H ) && defined( MD5_DIGEST_LENGTH )

/* Finalizes the MD5 context using OpenSSL
 * Returns 1 if successful or -1 on error
 */
int libhmac_md5_context_finalize(
     libhmac_md5_context_t *context,
     uint8_t *hash,
     size_t hash_size,
     libcerror_error_t **error )
{
	libhmac_internal_md5_context_t *internal_context = NULL;
	static char *function                            = "libhmac_md5_context_finalize";

	if( context == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid context.",
		 function );

		return( -1 );
	}
	internal_context = (libhmac_internal_md5_context_t *) context;

	if( hash == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid hash.",
		 function );

		return( -1 );
	}
	if( hash_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid hash size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( hash_size < (size_t) LIBHMAC_MD5_HASH_SIZE )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: invalid hash size value too small.",
		 function );

		return( -1 );
	}
	if( MD5_Final(
	     hash,
	     &( internal_context->md5_context ) ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to finalize context.",
		 function );

		return( -1 );
	}
	return( 1 );
}

#elif defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_EVP_H ) && defined( HAVE_EVP_MD5 )

/* Finalizes the MD5 context using OpenSSL EVP
 * Returns 1 if successful or -1 on error
 */
int libhmac_md5_context_finalize(
     libhmac_md5_context_t *context,
     uint8_t *hash,
     size_t hash_size,
     libcerror_error_t **error )
{
	libhmac_internal_md5_context_t *internal_context = NULL;
	static char *function                            = "libhmac_md5_context_finalize";
	unsigned int safe_hash_size                      = 0;

	if( context == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid context.",
		 function );

		return( -1 );
	}
	internal_context = (libhmac_internal_md5_context_t *) context;

	if( hash == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid hash.",
		 function );

		return( -1 );
	}
	if( ( hash_size < (size_t) LIBHMAC_MD5_HASH_SIZE )
	 || ( hash_size > (size_t) UINT_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid hash size value out of bounds.",
		 function );

		return( -1 );
	}
	safe_hash_size = (unsigned int) hash_size;

	if( EVP_DigestFinal_ex(
	     internal_context->evp_md_context,
	     (unsigned char *) hash,
	     &safe_hash_size ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to finalize context.",
		 function );

		return( -1 );
	}
	return( 1 );
}

#else

/* Finalizes the MD5 context using fallback implementation
 * Returns 1 if successful or -1 on error
 */
int libhmac_md5_context_finalize(
     libhmac_md5_context_t *context,
     uint8_t *hash,
     size_t hash_size,
     libcerror_error_t **error )
{
	libhmac_internal_md5_context_t *internal_context = NULL;
	static char *function                            = "libhmac_md5_context_finalize";
	size_t block_size                                = 0;
	size_t number_of_blocks                          = 0;
	ssize_t process_count                            = 0;
	uint64_t bit_size                                = 0;

#if !defined( LIBHMAC_UNFOLLED_LOOPS )
	size_t hash_index                                = 0;
	int hash_values_index                            = 0;
#endif

	if( context == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid context.",
		 function );

		return( -1 );
	}
	internal_context = (libhmac_internal_md5_context_t *) context;

	if( hash == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid hash.",
		 function );

		return( -1 );
	}
	if( hash_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid hash size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( hash_size < (size_t) LIBHMAC_MD5_HASH_SIZE )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: invalid hash size value too small.",
		 function );

		return( -1 );
	}
	/* Add padding with a size of 56 mod 64
	 */
	number_of_blocks = 1;

	if( internal_context->block_offset > 55 )
	{
		number_of_blocks += 1;
	}
	block_size = number_of_blocks * LIBHMAC_MD5_BLOCK_SIZE;

	if( memory_set(
	     &( internal_context->block[ internal_context->block_offset ] ),
	     0,
	     block_size - internal_context->block_offset ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear context block.",
		 function );

		return( -1 );
	}
	/* The first byte of the padding contains 0x80
	 */
	internal_context->block[ internal_context->block_offset ] = 0x80;

	bit_size = ( internal_context->hash_count + internal_context->block_offset ) * 8;

	byte_stream_copy_from_uint64_little_endian(
	 &( internal_context->block[ block_size - 8 ] ),
	 bit_size );

	process_count = libhmac_md5_context_transform(
	                 internal_context,
	                 internal_context->block,
	                 block_size,
	                 error );

	if( process_count == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to transform context block.",
		 function );

		return( -1 );
	}
#if !defined( LIBHMAC_UNFOLLED_LOOPS )
	for( hash_values_index = 0;
	     hash_values_index < 4;
	     hash_values_index++ )
	{
		byte_stream_copy_from_uint32_little_endian(
		 &( hash[ hash_index ] ),
		 internal_context->hash_values[ hash_values_index ] );

		hash_index += sizeof( uint32_t );
	}
#else
	byte_stream_copy_from_uint32_little_endian(
	 &( hash[ 0 ] ),
	 internal_context->hash_values[ 0 ] );

	byte_stream_copy_from_uint32_little_endian(
	 &( hash[ 4 ] ),
	 internal_context->hash_values[ 1 ] );

	byte_stream_copy_from_uint32_little_endian(
	 &( hash[ 8 ] ),
	 internal_context->hash_values[ 2 ] );

	byte_stream_copy_from_uint32_little_endian(
	 &( hash[ 12 ] ),
	 internal_context->hash_values[ 3 ] );

#endif /* !defined( LIBHMAC_UNFOLLED_LOOPS ) */

	/* Prevent sensitive data from leaking
	 */
	if( memory_set(
	     internal_context,
	     0,
	     sizeof( libhmac_internal_md5_context_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear context.",
		 function );

		return( -1 );
	}
	return( 1 );
}

#endif /* defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_MD5_H ) && defined( MD5_DIGEST_LENGTH ) */

