/*
 * SHA1 context functions
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

#if defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_SHA_H )
#include <openssl/sha.h>

#elif defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_EVP_H )
#include <openssl/err.h>
#include <openssl/evp.h>
#endif

#include "libhmac_byte_stream.h"
#include "libhmac_definitions.h"
#include "libhmac_sha1_context.h"

#if !defined( LIBHMAC_HAVE_SHA1_SUPPORT )

/* RFC 3174/FIPS 180-1 based SHA-1 functions
 */

/* TODO decription what these values are based on
 */
uint32_t libhmac_sha1_context_fixed_constants[ 5 ] = {
	0x67452301UL, 0xefcdab89UL, 0x98badcfeUL, 0x10325476UL, 0xc3d2e1f0UL
};

#define libhmac_sha1_context_transform_extend_32bit_value( values_32bit, value_32bit_index ) \
	values_32bit[ value_32bit_index ] = values_32bit[ value_32bit_index - 3 ] \
	                                  ^ values_32bit[ value_32bit_index - 8 ] \
	                                  ^ values_32bit[ value_32bit_index - 14 ] \
	                                  ^ values_32bit[ value_32bit_index - 16 ]; \
	values_32bit[ value_32bit_index ] = byte_stream_bit_rotate_left_32bit( \
	                                     values_32bit[ value_32bit_index ], \
	                                     1 );

#define libhmac_sha1_context_transform_unfolded_extend_32bit_values( values_32bit ) \
	libhmac_sha1_context_transform_extend_32bit_value( values_32bit, 16 ); \
	libhmac_sha1_context_transform_extend_32bit_value( values_32bit, 17 ); \
	libhmac_sha1_context_transform_extend_32bit_value( values_32bit, 18 ); \
	libhmac_sha1_context_transform_extend_32bit_value( values_32bit, 19 ); \
	libhmac_sha1_context_transform_extend_32bit_value( values_32bit, 20 ); \
	libhmac_sha1_context_transform_extend_32bit_value( values_32bit, 21 ); \
	libhmac_sha1_context_transform_extend_32bit_value( values_32bit, 22 ); \
	libhmac_sha1_context_transform_extend_32bit_value( values_32bit, 23 ); \
	libhmac_sha1_context_transform_extend_32bit_value( values_32bit, 24 ); \
	libhmac_sha1_context_transform_extend_32bit_value( values_32bit, 25 ); \
	libhmac_sha1_context_transform_extend_32bit_value( values_32bit, 26 ); \
	libhmac_sha1_context_transform_extend_32bit_value( values_32bit, 27 ); \
	libhmac_sha1_context_transform_extend_32bit_value( values_32bit, 28 ); \
	libhmac_sha1_context_transform_extend_32bit_value( values_32bit, 29 ); \
	libhmac_sha1_context_transform_extend_32bit_value( values_32bit, 30 ); \
	libhmac_sha1_context_transform_extend_32bit_value( values_32bit, 31 ); \
	libhmac_sha1_context_transform_extend_32bit_value( values_32bit, 32 ); \
	libhmac_sha1_context_transform_extend_32bit_value( values_32bit, 33 ); \
	libhmac_sha1_context_transform_extend_32bit_value( values_32bit, 34 ); \
	libhmac_sha1_context_transform_extend_32bit_value( values_32bit, 35 ); \
	libhmac_sha1_context_transform_extend_32bit_value( values_32bit, 36 ); \
	libhmac_sha1_context_transform_extend_32bit_value( values_32bit, 37 ); \
	libhmac_sha1_context_transform_extend_32bit_value( values_32bit, 38 ); \
	libhmac_sha1_context_transform_extend_32bit_value( values_32bit, 39 ); \
	libhmac_sha1_context_transform_extend_32bit_value( values_32bit, 40 ); \
	libhmac_sha1_context_transform_extend_32bit_value( values_32bit, 41 ); \
	libhmac_sha1_context_transform_extend_32bit_value( values_32bit, 42 ); \
	libhmac_sha1_context_transform_extend_32bit_value( values_32bit, 43 ); \
	libhmac_sha1_context_transform_extend_32bit_value( values_32bit, 44 ); \
	libhmac_sha1_context_transform_extend_32bit_value( values_32bit, 45 ); \
	libhmac_sha1_context_transform_extend_32bit_value( values_32bit, 46 ); \
	libhmac_sha1_context_transform_extend_32bit_value( values_32bit, 47 ); \
	libhmac_sha1_context_transform_extend_32bit_value( values_32bit, 48 ); \
	libhmac_sha1_context_transform_extend_32bit_value( values_32bit, 49 ); \
	libhmac_sha1_context_transform_extend_32bit_value( values_32bit, 50 ); \
	libhmac_sha1_context_transform_extend_32bit_value( values_32bit, 51 ); \
	libhmac_sha1_context_transform_extend_32bit_value( values_32bit, 52 ); \
	libhmac_sha1_context_transform_extend_32bit_value( values_32bit, 53 ); \
	libhmac_sha1_context_transform_extend_32bit_value( values_32bit, 54 ); \
	libhmac_sha1_context_transform_extend_32bit_value( values_32bit, 55 ); \
	libhmac_sha1_context_transform_extend_32bit_value( values_32bit, 56 ); \
	libhmac_sha1_context_transform_extend_32bit_value( values_32bit, 57 ); \
	libhmac_sha1_context_transform_extend_32bit_value( values_32bit, 58 ); \
	libhmac_sha1_context_transform_extend_32bit_value( values_32bit, 59 ); \
	libhmac_sha1_context_transform_extend_32bit_value( values_32bit, 60 ); \
	libhmac_sha1_context_transform_extend_32bit_value( values_32bit, 61 ); \
	libhmac_sha1_context_transform_extend_32bit_value( values_32bit, 62 ); \
	libhmac_sha1_context_transform_extend_32bit_value( values_32bit, 63 ); \
	libhmac_sha1_context_transform_extend_32bit_value( values_32bit, 64 ); \
	libhmac_sha1_context_transform_extend_32bit_value( values_32bit, 65 ); \
	libhmac_sha1_context_transform_extend_32bit_value( values_32bit, 66 ); \
	libhmac_sha1_context_transform_extend_32bit_value( values_32bit, 67 ); \
	libhmac_sha1_context_transform_extend_32bit_value( values_32bit, 68 ); \
	libhmac_sha1_context_transform_extend_32bit_value( values_32bit, 69 ); \
	libhmac_sha1_context_transform_extend_32bit_value( values_32bit, 70 ); \
	libhmac_sha1_context_transform_extend_32bit_value( values_32bit, 71 ); \
	libhmac_sha1_context_transform_extend_32bit_value( values_32bit, 72 ); \
	libhmac_sha1_context_transform_extend_32bit_value( values_32bit, 73 ); \
	libhmac_sha1_context_transform_extend_32bit_value( values_32bit, 74 ); \
	libhmac_sha1_context_transform_extend_32bit_value( values_32bit, 75 ); \
	libhmac_sha1_context_transform_extend_32bit_value( values_32bit, 76 ); \
	libhmac_sha1_context_transform_extend_32bit_value( values_32bit, 77 ); \
	libhmac_sha1_context_transform_extend_32bit_value( values_32bit, 78 ); \
	libhmac_sha1_context_transform_extend_32bit_value( values_32bit, 79 );

#define libhmac_sha1_context_transform_unfolded_calculate_hash_value_round1( values_32bit, value_32bit_index, hash_values, hash_value_index0, hash_value_index1, hash_value_index2, hash_value_index3, hash_value_index4 ) \
	hash_values[ hash_value_index4 ] += ( hash_values[ hash_value_index1 ] & hash_values[ hash_value_index2 ] ) \
	                                  | ( ~( hash_values[ hash_value_index1 ] ) & hash_values[ hash_value_index3 ] ); \
	hash_values[ hash_value_index4 ] += 0x5a827999UL; \
	hash_values[ hash_value_index4 ] += byte_stream_bit_rotate_left_32bit( \
	                                     hash_values[ hash_value_index0 ], \
	                                     5 ); \
	hash_values[ hash_value_index4 ] += values_32bit[ value_32bit_index ]; \
\
	hash_values[ hash_value_index1 ] = byte_stream_bit_rotate_left_32bit( \
	                                    hash_values[ hash_value_index1 ], \
			                    30 );

#define libhmac_sha1_context_transform_unfolded_calculate_hash_value_round2( values_32bit, value_32bit_index, hash_values, hash_value_index0, hash_value_index1, hash_value_index2, hash_value_index3, hash_value_index4 ) \
	hash_values[ hash_value_index4 ] += hash_values[ hash_value_index1 ] \
	                                  ^ hash_values[ hash_value_index2 ] \
	                                  ^ hash_values[ hash_value_index3 ]; \
	hash_values[ hash_value_index4 ] += 0x6ed9eba1UL; \
	hash_values[ hash_value_index4 ] += byte_stream_bit_rotate_left_32bit( \
	                                     hash_values[ hash_value_index0 ], \
	                                     5 ); \
	hash_values[ hash_value_index4 ] += values_32bit[ value_32bit_index ]; \
\
	hash_values[ hash_value_index1 ] = byte_stream_bit_rotate_left_32bit( \
	                                    hash_values[ hash_value_index1 ], \
			                    30 );

#define libhmac_sha1_context_transform_unfolded_calculate_hash_value_round3( values_32bit, value_32bit_index, hash_values, hash_value_index0, hash_value_index1, hash_value_index2, hash_value_index3, hash_value_index4 ) \
	hash_values[ hash_value_index4 ] += ( hash_values[ hash_value_index1 ] & hash_values[ hash_value_index2 ] ) \
	                                  | ( hash_values[ hash_value_index1 ] & hash_values[ hash_value_index3 ] ) \
	                                  | ( hash_values[ hash_value_index2 ] & hash_values[ hash_value_index3 ] ); \
	hash_values[ hash_value_index4 ] += 0x8f1bbcdcUL; \
	hash_values[ hash_value_index4 ] += byte_stream_bit_rotate_left_32bit( \
	                                     hash_values[ hash_value_index0 ], \
	                                     5 ); \
	hash_values[ hash_value_index4 ] += values_32bit[ value_32bit_index ]; \
\
	hash_values[ hash_value_index1 ] = byte_stream_bit_rotate_left_32bit( \
	                                    hash_values[ hash_value_index1 ], \
			                    30 );

#define libhmac_sha1_context_transform_unfolded_calculate_hash_value_round4( values_32bit, value_32bit_index, hash_values, hash_value_index0, hash_value_index1, hash_value_index2, hash_value_index3, hash_value_index4 ) \
	hash_values[ hash_value_index4 ] += hash_values[ hash_value_index1 ] \
	                                  ^ hash_values[ hash_value_index2 ] \
	                                  ^ hash_values[ hash_value_index3 ]; \
	hash_values[ hash_value_index4 ] += 0xca62c1d6UL; \
	hash_values[ hash_value_index4 ] += byte_stream_bit_rotate_left_32bit( \
	                                     hash_values[ hash_value_index0 ], \
	                                     5 ); \
	hash_values[ hash_value_index4 ] += values_32bit[ value_32bit_index ]; \
\
	hash_values[ hash_value_index1 ] = byte_stream_bit_rotate_left_32bit( \
	                                    hash_values[ hash_value_index1 ], \
			                    30 );

#define libhmac_sha1_context_transform_unfolded_calculate_hash_values( values_32bit, hash_values ) \
        libhmac_sha1_context_transform_unfolded_calculate_hash_value_round1( values_32bit, 0, hash_values, 0, 1, 2, 3, 4 ); \
        libhmac_sha1_context_transform_unfolded_calculate_hash_value_round1( values_32bit, 1, hash_values, 4, 0, 1, 2, 3 ); \
        libhmac_sha1_context_transform_unfolded_calculate_hash_value_round1( values_32bit, 2, hash_values, 3 ,4, 0, 1, 2 ); \
        libhmac_sha1_context_transform_unfolded_calculate_hash_value_round1( values_32bit, 3, hash_values, 2, 3, 4, 0, 1 ); \
        libhmac_sha1_context_transform_unfolded_calculate_hash_value_round1( values_32bit, 4, hash_values, 1, 2, 3, 4, 0 ); \
        libhmac_sha1_context_transform_unfolded_calculate_hash_value_round1( values_32bit, 5, hash_values, 0, 1, 2, 3, 4 ); \
        libhmac_sha1_context_transform_unfolded_calculate_hash_value_round1( values_32bit, 6, hash_values, 4, 0, 1, 2, 3 ); \
        libhmac_sha1_context_transform_unfolded_calculate_hash_value_round1( values_32bit, 7, hash_values, 3 ,4, 0, 1, 2 ); \
        libhmac_sha1_context_transform_unfolded_calculate_hash_value_round1( values_32bit, 8, hash_values, 2, 3, 4, 0, 1 ); \
        libhmac_sha1_context_transform_unfolded_calculate_hash_value_round1( values_32bit, 9, hash_values, 1, 2, 3, 4, 0 ); \
        libhmac_sha1_context_transform_unfolded_calculate_hash_value_round1( values_32bit, 10, hash_values, 0, 1, 2, 3, 4 ); \
        libhmac_sha1_context_transform_unfolded_calculate_hash_value_round1( values_32bit, 11, hash_values, 4, 0, 1, 2, 3 ); \
        libhmac_sha1_context_transform_unfolded_calculate_hash_value_round1( values_32bit, 12, hash_values, 3 ,4, 0, 1, 2 ); \
        libhmac_sha1_context_transform_unfolded_calculate_hash_value_round1( values_32bit, 13, hash_values, 2, 3, 4, 0, 1 ); \
        libhmac_sha1_context_transform_unfolded_calculate_hash_value_round1( values_32bit, 14, hash_values, 1, 2, 3, 4, 0 ); \
        libhmac_sha1_context_transform_unfolded_calculate_hash_value_round1( values_32bit, 15, hash_values, 0, 1, 2, 3, 4 ); \
        libhmac_sha1_context_transform_unfolded_calculate_hash_value_round1( values_32bit, 16, hash_values, 4, 0, 1, 2, 3 ); \
        libhmac_sha1_context_transform_unfolded_calculate_hash_value_round1( values_32bit, 17, hash_values, 3 ,4, 0, 1, 2 ); \
        libhmac_sha1_context_transform_unfolded_calculate_hash_value_round1( values_32bit, 18, hash_values, 2, 3, 4, 0, 1 ); \
        libhmac_sha1_context_transform_unfolded_calculate_hash_value_round1( values_32bit, 19, hash_values, 1, 2, 3, 4, 0 ); \
\
        libhmac_sha1_context_transform_unfolded_calculate_hash_value_round2( values_32bit, 20, hash_values, 0, 1, 2, 3, 4 ); \
        libhmac_sha1_context_transform_unfolded_calculate_hash_value_round2( values_32bit, 21, hash_values, 4, 0, 1, 2, 3 ); \
        libhmac_sha1_context_transform_unfolded_calculate_hash_value_round2( values_32bit, 22, hash_values, 3 ,4, 0, 1, 2 ); \
        libhmac_sha1_context_transform_unfolded_calculate_hash_value_round2( values_32bit, 23, hash_values, 2, 3, 4, 0, 1 ); \
        libhmac_sha1_context_transform_unfolded_calculate_hash_value_round2( values_32bit, 24, hash_values, 1, 2, 3, 4, 0 ); \
        libhmac_sha1_context_transform_unfolded_calculate_hash_value_round2( values_32bit, 25, hash_values, 0, 1, 2, 3, 4 ); \
        libhmac_sha1_context_transform_unfolded_calculate_hash_value_round2( values_32bit, 26, hash_values, 4, 0, 1, 2, 3 ); \
        libhmac_sha1_context_transform_unfolded_calculate_hash_value_round2( values_32bit, 27, hash_values, 3 ,4, 0, 1, 2 ); \
        libhmac_sha1_context_transform_unfolded_calculate_hash_value_round2( values_32bit, 28, hash_values, 2, 3, 4, 0, 1 ); \
        libhmac_sha1_context_transform_unfolded_calculate_hash_value_round2( values_32bit, 29, hash_values, 1, 2, 3, 4, 0 ); \
        libhmac_sha1_context_transform_unfolded_calculate_hash_value_round2( values_32bit, 30, hash_values, 0, 1, 2, 3, 4 ); \
        libhmac_sha1_context_transform_unfolded_calculate_hash_value_round2( values_32bit, 31, hash_values, 4, 0, 1, 2, 3 ); \
        libhmac_sha1_context_transform_unfolded_calculate_hash_value_round2( values_32bit, 32, hash_values, 3 ,4, 0, 1, 2 ); \
        libhmac_sha1_context_transform_unfolded_calculate_hash_value_round2( values_32bit, 33, hash_values, 2, 3, 4, 0, 1 ); \
        libhmac_sha1_context_transform_unfolded_calculate_hash_value_round2( values_32bit, 34, hash_values, 1, 2, 3, 4, 0 ); \
        libhmac_sha1_context_transform_unfolded_calculate_hash_value_round2( values_32bit, 35, hash_values, 0, 1, 2, 3, 4 ); \
        libhmac_sha1_context_transform_unfolded_calculate_hash_value_round2( values_32bit, 36, hash_values, 4, 0, 1, 2, 3 ); \
        libhmac_sha1_context_transform_unfolded_calculate_hash_value_round2( values_32bit, 37, hash_values, 3 ,4, 0, 1, 2 ); \
        libhmac_sha1_context_transform_unfolded_calculate_hash_value_round2( values_32bit, 38, hash_values, 2, 3, 4, 0, 1 ); \
        libhmac_sha1_context_transform_unfolded_calculate_hash_value_round2( values_32bit, 39, hash_values, 1, 2, 3, 4, 0 ); \
\
        libhmac_sha1_context_transform_unfolded_calculate_hash_value_round3( values_32bit, 40, hash_values, 0, 1, 2, 3, 4 ); \
        libhmac_sha1_context_transform_unfolded_calculate_hash_value_round3( values_32bit, 41, hash_values, 4, 0, 1, 2, 3 ); \
        libhmac_sha1_context_transform_unfolded_calculate_hash_value_round3( values_32bit, 42, hash_values, 3 ,4, 0, 1, 2 ); \
        libhmac_sha1_context_transform_unfolded_calculate_hash_value_round3( values_32bit, 43, hash_values, 2, 3, 4, 0, 1 ); \
        libhmac_sha1_context_transform_unfolded_calculate_hash_value_round3( values_32bit, 44, hash_values, 1, 2, 3, 4, 0 ); \
        libhmac_sha1_context_transform_unfolded_calculate_hash_value_round3( values_32bit, 45, hash_values, 0, 1, 2, 3, 4 ); \
        libhmac_sha1_context_transform_unfolded_calculate_hash_value_round3( values_32bit, 46, hash_values, 4, 0, 1, 2, 3 ); \
        libhmac_sha1_context_transform_unfolded_calculate_hash_value_round3( values_32bit, 47, hash_values, 3 ,4, 0, 1, 2 ); \
        libhmac_sha1_context_transform_unfolded_calculate_hash_value_round3( values_32bit, 48, hash_values, 2, 3, 4, 0, 1 ); \
        libhmac_sha1_context_transform_unfolded_calculate_hash_value_round3( values_32bit, 49, hash_values, 1, 2, 3, 4, 0 ); \
        libhmac_sha1_context_transform_unfolded_calculate_hash_value_round3( values_32bit, 50, hash_values, 0, 1, 2, 3, 4 ); \
        libhmac_sha1_context_transform_unfolded_calculate_hash_value_round3( values_32bit, 51, hash_values, 4, 0, 1, 2, 3 ); \
        libhmac_sha1_context_transform_unfolded_calculate_hash_value_round3( values_32bit, 52, hash_values, 3 ,4, 0, 1, 2 ); \
        libhmac_sha1_context_transform_unfolded_calculate_hash_value_round3( values_32bit, 53, hash_values, 2, 3, 4, 0, 1 ); \
        libhmac_sha1_context_transform_unfolded_calculate_hash_value_round3( values_32bit, 54, hash_values, 1, 2, 3, 4, 0 ); \
        libhmac_sha1_context_transform_unfolded_calculate_hash_value_round3( values_32bit, 55, hash_values, 0, 1, 2, 3, 4 ); \
        libhmac_sha1_context_transform_unfolded_calculate_hash_value_round3( values_32bit, 56, hash_values, 4, 0, 1, 2, 3 ); \
        libhmac_sha1_context_transform_unfolded_calculate_hash_value_round3( values_32bit, 57, hash_values, 3 ,4, 0, 1, 2 ); \
        libhmac_sha1_context_transform_unfolded_calculate_hash_value_round3( values_32bit, 58, hash_values, 2, 3, 4, 0, 1 ); \
        libhmac_sha1_context_transform_unfolded_calculate_hash_value_round3( values_32bit, 59, hash_values, 1, 2, 3, 4, 0 ); \
\
        libhmac_sha1_context_transform_unfolded_calculate_hash_value_round4( values_32bit, 60, hash_values, 0, 1, 2, 3, 4 ); \
        libhmac_sha1_context_transform_unfolded_calculate_hash_value_round4( values_32bit, 61, hash_values, 4, 0, 1, 2, 3 ); \
        libhmac_sha1_context_transform_unfolded_calculate_hash_value_round4( values_32bit, 62, hash_values, 3 ,4, 0, 1, 2 ); \
        libhmac_sha1_context_transform_unfolded_calculate_hash_value_round4( values_32bit, 63, hash_values, 2, 3, 4, 0, 1 ); \
        libhmac_sha1_context_transform_unfolded_calculate_hash_value_round4( values_32bit, 64, hash_values, 1, 2, 3, 4, 0 ); \
        libhmac_sha1_context_transform_unfolded_calculate_hash_value_round4( values_32bit, 65, hash_values, 0, 1, 2, 3, 4 ); \
        libhmac_sha1_context_transform_unfolded_calculate_hash_value_round4( values_32bit, 66, hash_values, 4, 0, 1, 2, 3 ); \
        libhmac_sha1_context_transform_unfolded_calculate_hash_value_round4( values_32bit, 67, hash_values, 3 ,4, 0, 1, 2 ); \
        libhmac_sha1_context_transform_unfolded_calculate_hash_value_round4( values_32bit, 68, hash_values, 2, 3, 4, 0, 1 ); \
        libhmac_sha1_context_transform_unfolded_calculate_hash_value_round4( values_32bit, 69, hash_values, 1, 2, 3, 4, 0 ); \
        libhmac_sha1_context_transform_unfolded_calculate_hash_value_round4( values_32bit, 70, hash_values, 0, 1, 2, 3, 4 ); \
        libhmac_sha1_context_transform_unfolded_calculate_hash_value_round4( values_32bit, 71, hash_values, 4, 0, 1, 2, 3 ); \
        libhmac_sha1_context_transform_unfolded_calculate_hash_value_round4( values_32bit, 72, hash_values, 3 ,4, 0, 1, 2 ); \
        libhmac_sha1_context_transform_unfolded_calculate_hash_value_round4( values_32bit, 73, hash_values, 2, 3, 4, 0, 1 ); \
        libhmac_sha1_context_transform_unfolded_calculate_hash_value_round4( values_32bit, 74, hash_values, 1, 2, 3, 4, 0 ); \
        libhmac_sha1_context_transform_unfolded_calculate_hash_value_round4( values_32bit, 75, hash_values, 0, 1, 2, 3, 4 ); \
        libhmac_sha1_context_transform_unfolded_calculate_hash_value_round4( values_32bit, 76, hash_values, 4, 0, 1, 2, 3 ); \
        libhmac_sha1_context_transform_unfolded_calculate_hash_value_round4( values_32bit, 77, hash_values, 3 ,4, 0, 1, 2 ); \
        libhmac_sha1_context_transform_unfolded_calculate_hash_value_round4( values_32bit, 78, hash_values, 2, 3, 4, 0, 1 ); \
        libhmac_sha1_context_transform_unfolded_calculate_hash_value_round4( values_32bit, 79, hash_values, 1, 2, 3, 4, 0 );

/* Calculates the SHA1 of 64 byte sized blocks of data in a buffer
 * Returns the number of bytes used if successful or -1 on error
 */
ssize_t libhmac_sha1_context_transform(
         libhmac_internal_sha1_context_t *internal_context,
         const uint8_t *buffer,
         size_t size,
         libcerror_error_t **error )
{
	uint32_t hash_values[ 5 ];
	uint32_t values_32bit[ 80 ];

	static char *function     = "libhmac_sha1_context_transform";
	size_t buffer_offset      = 0;

#if !defined( LIBHMAC_UNFOLLED_LOOPS )
	uint32_t hash_value       = 0;
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
	while( size >= LIBHMAC_SHA1_BLOCK_SIZE )
	{
		if( memory_copy(
		     hash_values,
		     internal_context->hash_values,
		     sizeof( uint32_t ) * 5 ) == NULL )
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
		libhmac_byte_stream_copy_to_16x_uint32_big_endian(
		 &( buffer[ buffer_offset ] ),
		 values_32bit );

		buffer_offset += LIBHMAC_SHA1_BLOCK_SIZE;

		/* Extend to 80 x 32-bit values
		 */
		libhmac_sha1_context_transform_unfolded_extend_32bit_values(
		 values_32bit );

		/* Calculate the hash values for the 32-bit values
		 */
		libhmac_sha1_context_transform_unfolded_calculate_hash_values(
		 values_32bit,
		 hash_values );

		/* Update the hash values in the context
		 */
		internal_context->hash_values[ 0 ] += hash_values[ 0 ];
		internal_context->hash_values[ 1 ] += hash_values[ 1 ];
		internal_context->hash_values[ 2 ] += hash_values[ 2 ];
		internal_context->hash_values[ 3 ] += hash_values[ 3 ];
		internal_context->hash_values[ 4 ] += hash_values[ 4 ];
#else
		/* Break the block into 16 x 32-bit values
		 */
		for( value_32bit_index = 0;
		     value_32bit_index < 16;
		     value_32bit_index++ )
		{
			byte_stream_copy_to_uint32_big_endian(
			 &( buffer[ buffer_offset ] ),
			 values_32bit[ value_32bit_index ] );

			buffer_offset += sizeof( uint32_t );
		}
		/* Extend to 80 x 32-bit values
		 */
		for( value_32bit_index = 16;
		     value_32bit_index < 80;
		     value_32bit_index++ )
		{
			libhmac_sha1_context_transform_extend_32bit_value(
			 values_32bit,
			 value_32bit_index );
		}
		/* Calculate the hash values for the 32-bit values
		 */
		for( value_32bit_index = 0;
		     value_32bit_index < 80;
		     value_32bit_index++ )
		{
			if( value_32bit_index < 20 )
			{
				hash_values[ 4 ] += ( hash_values[ 1 ] & hash_values[ 2 ] )
				                  | ( ~( hash_values[ 1 ] ) & hash_values[ 3 ] );
				hash_values[ 4 ] += 0x5a827999UL;
			}
			else if( value_32bit_index < 40 )
			{
				hash_values[ 4 ] += hash_values[ 1 ]
				                  ^ hash_values[ 2 ]
				                  ^ hash_values[ 3 ];
				hash_values[ 4 ] += 0x6ed9eba1UL;
			}
			else if( value_32bit_index < 60 )
			{
				hash_values[ 4 ] += ( hash_values[ 1 ] & hash_values[ 2 ] )
				                  | ( hash_values[ 1 ] & hash_values[ 3 ] )
				                  | ( hash_values[ 2 ] & hash_values[ 3 ] );
				hash_values[ 4 ] += 0x8f1bbcdcUL;
			}
			else
			{
				hash_values[ 4 ] += hash_values[ 1 ]
				                  ^ hash_values[ 2 ]
				                  ^ hash_values[ 3 ];
				hash_values[ 4 ] += 0xca62c1d6UL;
			}
			hash_values[ 4 ] += byte_stream_bit_rotate_left_32bit(
			                     hash_values[ 0 ],
			                     5 );
			hash_values[ 4 ] += values_32bit[ value_32bit_index ];

			hash_value       = hash_values[ 4 ];
			hash_values[ 4 ] = hash_values[ 3 ];
			hash_values[ 3 ] = hash_values[ 2 ];
			hash_values[ 2 ] = byte_stream_bit_rotate_left_32bit(
			                    hash_values[ 1 ],
			                    30 );
			hash_values[ 1 ] = hash_values[ 0 ];
			hash_values[ 0 ] = hash_value;
		}
		/* Update the hash values in the context
		 */
		for( hash_values_index = 0;
		     hash_values_index < 5;
		     hash_values_index++ )
		{
			internal_context->hash_values[ hash_values_index ] += hash_values[ hash_values_index ];
		}
#endif /* defined( LIBHMAC_UNFOLLED_LOOPS ) */

		size -= LIBHMAC_SHA1_BLOCK_SIZE;
	}
	/* Prevent sensitive data from leaking
	 */
	if( memory_set(
	     hash_values,
	     0,
	     sizeof( uint32_t ) * 5 ) == NULL )
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
	 sizeof( uint32_t ) * 64 );

	memory_set(
	 hash_values,
	 0,
	 sizeof( uint32_t ) * 5 );

	return( -1 );
}

#endif /* !defined( LIBHMAC_HAVE_SHA1_SUPPORT ) */

/* Creates a SHA1 context
 * Make sure the value context is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libhmac_sha1_context_initialize(
     libhmac_sha1_context_t **context,
     libcerror_error_t **error )
{
	libhmac_internal_sha1_context_t *internal_context = NULL;
	static char *function                             = "libhmac_sha1_context_initialize";

#if defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_EVP_H ) && defined( HAVE_EVP_SHA1 )
	const EVP_MD *evp_md_type                         = NULL;
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
	                    libhmac_internal_sha1_context_t );

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
	     sizeof( libhmac_internal_sha1_context_t ) ) == NULL )
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
#if defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_SHA_H ) && defined( SHA_DIGEST_LENGTH )
	if( SHA1_Init(
	     &( internal_context->sha1_context ) ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize context.",
		 function );

		goto on_error;
	}

#elif defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_EVP_H ) && defined( HAVE_EVP_SHA1 )
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
	evp_md_type = EVP_sha1();

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
		 "%s: unable to initialize context.",
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
	     libhmac_sha1_context_fixed_constants,
	     sizeof( uint32_t ) * 5 ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy fixed constants to hash values.",
		 function );

		goto on_error;
	}
#endif /* defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_SHA_H ) && defined( SHA_DIGEST_LENGTH ) */

	*context = (libhmac_sha1_context_t *) internal_context;

	return( 1 );

on_error:
	if( internal_context != NULL )
	{
		memory_free(
		 internal_context );
	}
	return( -1 );
}

/* Frees a SHA1 context
 * Returns 1 if successful or -1 on error
 */
int libhmac_sha1_context_free(
     libhmac_sha1_context_t **context,
     libcerror_error_t **error )
{
	libhmac_internal_sha1_context_t *internal_context = NULL;
	static char *function                             = "libhmac_sha1_context_free";

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
		internal_context = (libhmac_internal_sha1_context_t *) *context;
		*context         = NULL;

#if defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_SHA_H ) && defined( SHA_DIGEST_LENGTH )
		/* No additional clean up necessary
		 */

#elif defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_EVP_H ) && defined( HAVE_EVP_SHA1 )
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

#endif /* defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_SHA_H ) && defined( SHA_DIGEST_LENGTH ) */
		memory_free(
		 internal_context );
	}
	return( 1 );
}

#if defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_SHA_H ) && defined( SHA_DIGEST_LENGTH )

/* Updates the SHA1 context using OpenSSL
 * Returns 1 if successful or -1 on error
 */
int libhmac_sha1_context_update(
     libhmac_sha1_context_t *context,
     const uint8_t *buffer,
     size_t size,
     libcerror_error_t **error )
{
	libhmac_internal_sha1_context_t *internal_context = NULL;
	static char *function                             = "libhmac_sha1_context_update";
	unsigned long safe_hash_size                      = 0;

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
	internal_context = (libhmac_internal_sha1_context_t *) context;

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

	if( SHA1_Update(
	     &( internal_context->sha1_context ),
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

#elif defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_EVP_H ) && defined( HAVE_EVP_SHA1 )

/* Updates the SHA1 context using OpenSSL EVP
 * Returns 1 if successful or -1 on error
 */
int libhmac_sha1_context_update(
     libhmac_sha1_context_t *context,
     const uint8_t *buffer,
     size_t size,
     libcerror_error_t **error )
{
	libhmac_internal_sha1_context_t *internal_context = NULL;
	static char *function                             = "libhmac_sha1_context_update";

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
	internal_context = (libhmac_internal_sha1_context_t *) context;

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

/* Updates the SHA1 context using fallback implementation
 * Returns 1 if successful or -1 on error
 */
int libhmac_sha1_context_update(
     libhmac_sha1_context_t *context,
     const uint8_t *buffer,
     size_t size,
     libcerror_error_t **error )
{
	libhmac_internal_sha1_context_t *internal_context = NULL;
	static char *function                             = "libhmac_sha1_context_update";
	size_t buffer_offset                              = 0;
	size_t remaining_block_size                       = 0;
	ssize_t process_count                             = 0;

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
	internal_context = (libhmac_internal_sha1_context_t *) context;

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
		if( internal_context->block_offset >= LIBHMAC_SHA1_BLOCK_SIZE )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid context - block offset value out of bounds.",
			 function );

			return( -1 );
		}
		remaining_block_size = LIBHMAC_SHA1_BLOCK_SIZE - internal_context->block_offset;

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

		if( internal_context->block_offset < LIBHMAC_SHA1_BLOCK_SIZE )
		{
			return( 1 );
		}
		buffer_offset += remaining_block_size;
		size          -= remaining_block_size;

		process_count = libhmac_sha1_context_transform(
		                 internal_context,
		                 internal_context->block,
		                 LIBHMAC_SHA1_BLOCK_SIZE,
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
		process_count = libhmac_sha1_context_transform(
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
		if( size >= LIBHMAC_SHA1_BLOCK_SIZE )
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

#endif /* defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_SHA_H ) && defined( SHA_DIGEST_LENGTH ) */

#if defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_SHA_H ) && defined( SHA_DIGEST_LENGTH )

/* Finalizes the SHA1 context using OpenSSL
 * Returns 1 if successful or -1 on error
 */
int libhmac_sha1_context_finalize(
     libhmac_sha1_context_t *context,
     uint8_t *hash,
     size_t hash_size,
     libcerror_error_t **error )
{
	libhmac_internal_sha1_context_t *internal_context = NULL;
	static char *function                             = "libhmac_sha1_context_finalize";

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
	internal_context = (libhmac_internal_sha1_context_t *) context;

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
	if( hash_size < (size_t) LIBHMAC_SHA1_HASH_SIZE )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: invalid hash size value too small.",
		 function );

		return( -1 );
	}
	if( SHA1_Final(
	     hash,
	     &( internal_context->sha1_context ) ) != 1 )
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

#elif defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_EVP_H ) && defined( HAVE_EVP_SHA1 )

/* Finalizes the SHA1 context using OpenSSL EVP
 * Returns 1 if successful or -1 on error
 */
int libhmac_sha1_context_finalize(
     libhmac_sha1_context_t *context,
     uint8_t *hash,
     size_t hash_size,
     libcerror_error_t **error )
{
	libhmac_internal_sha1_context_t *internal_context = NULL;
	static char *function                             = "libhmac_sha1_context_finalize";
	unsigned int safe_hash_size                       = 0;

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
	internal_context = (libhmac_internal_sha1_context_t *) context;

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
	if( ( hash_size < (size_t) LIBHMAC_SHA1_HASH_SIZE )
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

/* Finalizes the SHA1 context using fallback implementation
 * Returns 1 if successful or -1 on error
 */
int libhmac_sha1_context_finalize(
     libhmac_sha1_context_t *context,
     uint8_t *hash,
     size_t hash_size,
     libcerror_error_t **error )
{
	libhmac_internal_sha1_context_t *internal_context = NULL;
	static char *function                             = "libhmac_sha1_context_finalize";
	size_t block_size                                 = 0;
	size_t number_of_blocks                           = 0;
	ssize_t process_count                             = 0;
	uint64_t bit_size                                 = 0;

#if !defined( LIBHMAC_UNFOLLED_LOOPS )
	size_t hash_index                                 = 0;
	int hash_values_index                             = 0;
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
	internal_context = (libhmac_internal_sha1_context_t *) context;

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
	if( hash_size < (size_t) LIBHMAC_SHA1_HASH_SIZE )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: invalid hash value too small.",
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
	if( hash_size < (size_t) LIBHMAC_SHA1_HASH_SIZE )
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
	block_size = number_of_blocks * LIBHMAC_SHA1_BLOCK_SIZE;

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

	byte_stream_copy_from_uint64_big_endian(
	 &( internal_context->block[ block_size - 8 ] ),
	 bit_size );

	process_count = libhmac_sha1_context_transform(
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
	     hash_values_index < 5;
	     hash_values_index++ )
	{
		byte_stream_copy_from_uint32_big_endian(
		 &( hash[ hash_index ] ),
		 internal_context->hash_values[ hash_values_index ] );

		hash_index += sizeof( uint32_t );
	}
#else
	byte_stream_copy_from_uint32_big_endian(
	 &( hash[ 0 ] ),
	 internal_context->hash_values[ 0 ] );

	byte_stream_copy_from_uint32_big_endian(
	 &( hash[ 4 ] ),
	 internal_context->hash_values[ 1 ] );

	byte_stream_copy_from_uint32_big_endian(
	 &( hash[ 8 ] ),
	 internal_context->hash_values[ 2 ] );

	byte_stream_copy_from_uint32_big_endian(
	 &( hash[ 12 ] ),
	 internal_context->hash_values[ 3 ] );

	byte_stream_copy_from_uint32_big_endian(
	 &( hash[ 16 ] ),
	 internal_context->hash_values[ 4 ] );

#endif /* !defined( LIBHMAC_UNFOLLED_LOOPS ) */

	/* Prevent sensitive data from leaking
	 */
	if( memory_set(
	     internal_context,
	     0,
	     sizeof( libhmac_internal_sha1_context_t ) ) == NULL )
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

#endif /* defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_SHA_H ) && defined( SHA_DIGEST_LENGTH ) */

