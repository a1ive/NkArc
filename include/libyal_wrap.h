#pragma once

#include <grub/types.h>
#include <grub/misc.h>
#include <grub/mm.h>
#include <grub/err.h>

typedef grub_uint8_t uint8_t;
typedef grub_uint16_t uint16_t;
typedef grub_uint32_t uint32_t;
typedef grub_uint64_t uint64_t;

typedef grub_uint64_t size64_t;

#define _BYTE_STREAM_HOST_IS_ENDIAN_BIG		( *((uint32_t *) "\x01\x02\x03\x04" ) == 0x01020304 )
#define _BYTE_STREAM_HOST_IS_ENDIAN_LITTLE	( *((uint32_t *) "\x01\x02\x03\x04" ) == 0x04030201 )
#define _BYTE_STREAM_HOST_IS_ENDIAN_MIDDLE	( *((uint32_t *) "\x01\x02\x03\x04" ) == 0x02010403 )

#define _BYTE_STREAM_ENDIAN_BIG			(uint8_t) 'b'
#define _BYTE_STREAM_ENDIAN_LITTLE		(uint8_t) 'l'
#define _BYTE_STREAM_ENDIAN_MIDDLE		(uint8_t) 'm'

#define byte_stream_copy_to_uint16_big_endian( byte_stream, value ) \
	( value )   = ( byte_stream )[ 0 ]; \
	( value ) <<= 8; \
	( value )  |= ( byte_stream )[ 1 ];

#define byte_stream_copy_to_uint16_little_endian( byte_stream, value ) \
	( value )   = ( byte_stream )[ 1 ]; \
	( value ) <<= 8; \
	( value )  |= ( byte_stream )[ 0 ];

#define byte_stream_copy_to_uint24_big_endian( byte_stream, value ) \
	( value )   = ( byte_stream )[ 0 ]; \
	( value ) <<= 8; \
	( value )  |= ( byte_stream )[ 1 ]; \
	( value ) <<= 8; \
	( value )  |= ( byte_stream )[ 2 ];

#define byte_stream_copy_to_uint24_little_endian( byte_stream, value ) \
	( value )   = ( byte_stream )[ 2 ]; \
	( value ) <<= 8; \
	( value )  |= ( byte_stream )[ 1 ]; \
	( value ) <<= 8; \
	( value )  |= ( byte_stream )[ 0 ];

#define byte_stream_copy_to_uint32_big_endian( byte_stream, value ) \
	( value )   = ( byte_stream )[ 0 ]; \
	( value ) <<= 8; \
	( value )  |= ( byte_stream )[ 1 ]; \
	( value ) <<= 8; \
	( value )  |= ( byte_stream )[ 2 ]; \
	( value ) <<= 8; \
	( value )  |= ( byte_stream )[ 3 ];

#define byte_stream_copy_to_uint32_little_endian( byte_stream, value ) \
	( value )   = ( byte_stream )[ 3 ]; \
	( value ) <<= 8; \
	( value )  |= ( byte_stream )[ 2 ]; \
	( value ) <<= 8; \
	( value )  |= ( byte_stream )[ 1 ]; \
	( value ) <<= 8; \
	( value )  |= ( byte_stream )[ 0 ];

#define byte_stream_copy_to_uint48_big_endian( byte_stream, value ) \
	( value )   = ( byte_stream )[ 0 ]; \
	( value ) <<= 8; \
	( value )  |= ( byte_stream )[ 1 ]; \
	( value ) <<= 8; \
	( value )  |= ( byte_stream )[ 2 ]; \
	( value ) <<= 8; \
	( value )  |= ( byte_stream )[ 3 ]; \
	( value ) <<= 8; \
	( value )  |= ( byte_stream )[ 4 ]; \
	( value ) <<= 8; \
	( value )  |= ( byte_stream )[ 5 ];

#define byte_stream_copy_to_uint48_little_endian( byte_stream, value ) \
	( value )  |= ( byte_stream )[ 5 ]; \
	( value ) <<= 8; \
	( value )  |= ( byte_stream )[ 4 ]; \
	( value ) <<= 8; \
	( value )  |= ( byte_stream )[ 3 ]; \
	( value ) <<= 8; \
	( value )  |= ( byte_stream )[ 2 ]; \
	( value ) <<= 8; \
	( value )  |= ( byte_stream )[ 1 ]; \
	( value ) <<= 8; \
	( value )  |= ( byte_stream )[ 0 ];

#define byte_stream_copy_to_uint64_big_endian( byte_stream, value ) \
	( value )   = ( byte_stream )[ 0 ]; \
	( value ) <<= 8; \
	( value )  |= ( byte_stream )[ 1 ]; \
	( value ) <<= 8; \
	( value )  |= ( byte_stream )[ 2 ]; \
	( value ) <<= 8; \
	( value )  |= ( byte_stream )[ 3 ]; \
	( value ) <<= 8; \
	( value )  |= ( byte_stream )[ 4 ]; \
	( value ) <<= 8; \
	( value )  |= ( byte_stream )[ 5 ]; \
	( value ) <<= 8; \
	( value )  |= ( byte_stream )[ 6 ]; \
	( value ) <<= 8; \
	( value )  |= ( byte_stream )[ 7 ];

#define byte_stream_copy_to_uint64_little_endian( byte_stream, value ) \
	( value )   = ( byte_stream )[ 7 ]; \
	( value ) <<= 8; \
	( value )  |= ( byte_stream )[ 6 ]; \
	( value ) <<= 8; \
	( value )  |= ( byte_stream )[ 5 ]; \
	( value ) <<= 8; \
	( value )  |= ( byte_stream )[ 4 ]; \
	( value ) <<= 8; \
	( value )  |= ( byte_stream )[ 3 ]; \
	( value ) <<= 8; \
	( value )  |= ( byte_stream )[ 2 ]; \
	( value ) <<= 8; \
	( value )  |= ( byte_stream )[ 1 ]; \
	( value ) <<= 8; \
	( value )  |= ( byte_stream )[ 0 ];

#define byte_stream_copy_from_uint16_big_endian( byte_stream, value ) \
	( byte_stream )[ 0 ] = (uint8_t) ( ( ( value ) >> 8 ) & 0x0ff ); \
	( byte_stream )[ 1 ] = (uint8_t) ( ( value ) & 0x0ff )

#define byte_stream_copy_from_uint16_little_endian( byte_stream, value ) \
	( byte_stream )[ 1 ] = (uint8_t) ( ( ( value ) >> 8 ) & 0x0ff ); \
	( byte_stream )[ 0 ] = (uint8_t) ( ( value ) & 0x0ff )

#define byte_stream_copy_from_uint24_big_endian( byte_stream, value ) \
	( byte_stream )[ 0 ] = (uint8_t) ( ( ( value ) >> 16 ) & 0x0ff ); \
	( byte_stream )[ 1 ] = (uint8_t) ( ( ( value ) >> 8 ) & 0x0ff ); \
	( byte_stream )[ 2 ] = (uint8_t) ( ( value ) & 0x0ff )

#define byte_stream_copy_from_uint24_little_endian( byte_stream, value ) \
	( byte_stream )[ 2 ] = (uint8_t) ( ( ( value ) >> 16 ) & 0x0ff ); \
	( byte_stream )[ 1 ] = (uint8_t) ( ( ( value ) >> 8 ) & 0x0ff ); \
	( byte_stream )[ 0 ] = (uint8_t) ( ( value ) & 0x0ff )

#define byte_stream_copy_from_uint32_big_endian( byte_stream, value ) \
	( byte_stream )[ 0 ] = (uint8_t) ( ( ( value ) >> 24 ) & 0x0ff ); \
	( byte_stream )[ 1 ] = (uint8_t) ( ( ( value ) >> 16 ) & 0x0ff ); \
	( byte_stream )[ 2 ] = (uint8_t) ( ( ( value ) >> 8 ) & 0x0ff ); \
	( byte_stream )[ 3 ] = (uint8_t) ( ( value ) & 0x0ff )

#define byte_stream_copy_from_uint32_little_endian( byte_stream, value ) \
	( byte_stream )[ 3 ] = (uint8_t) ( ( ( value ) >> 24 ) & 0x0ff ); \
	( byte_stream )[ 2 ] = (uint8_t) ( ( ( value ) >> 16 ) & 0x0ff ); \
	( byte_stream )[ 1 ] = (uint8_t) ( ( ( value ) >> 8 ) & 0x0ff ); \
	( byte_stream )[ 0 ] = (uint8_t) ( ( value ) & 0x0ff )

#define byte_stream_copy_from_uint48_big_endian( byte_stream, value ) \
	( byte_stream )[ 0 ] = (uint8_t) ( ( ( value ) >> 40 ) & 0x0ff ); \
	( byte_stream )[ 1 ] = (uint8_t) ( ( ( value ) >> 32 ) & 0x0ff ); \
	( byte_stream )[ 2 ] = (uint8_t) ( ( ( value ) >> 24 ) & 0x0ff ); \
	( byte_stream )[ 3 ] = (uint8_t) ( ( ( value ) >> 16 ) & 0x0ff ); \
	( byte_stream )[ 4 ] = (uint8_t) ( ( ( value ) >> 8 ) & 0x0ff ); \
	( byte_stream )[ 5 ] = (uint8_t) ( ( value ) & 0x0ff )

#define byte_stream_copy_from_uint48_little_endian( byte_stream, value ) \
	( byte_stream )[ 5 ] = (uint8_t) ( ( ( value ) >> 40 ) & 0x0ff ); \
	( byte_stream )[ 4 ] = (uint8_t) ( ( ( value ) >> 32 ) & 0x0ff ); \
	( byte_stream )[ 3 ] = (uint8_t) ( ( ( value ) >> 24 ) & 0x0ff ); \
	( byte_stream )[ 2 ] = (uint8_t) ( ( ( value ) >> 16 ) & 0x0ff ); \
	( byte_stream )[ 1 ] = (uint8_t) ( ( ( value ) >> 8 ) & 0x0ff ); \
	( byte_stream )[ 0 ] = (uint8_t) ( ( value ) & 0x0ff )

#define byte_stream_copy_from_uint64_big_endian( byte_stream, value ) \
	( byte_stream )[ 0 ] = (uint8_t) ( ( ( value ) >> 56 ) & 0x0ff ); \
	( byte_stream )[ 1 ] = (uint8_t) ( ( ( value ) >> 48 ) & 0x0ff ); \
	( byte_stream )[ 2 ] = (uint8_t) ( ( ( value ) >> 40 ) & 0x0ff ); \
	( byte_stream )[ 3 ] = (uint8_t) ( ( ( value ) >> 32 ) & 0x0ff ); \
	( byte_stream )[ 4 ] = (uint8_t) ( ( ( value ) >> 24 ) & 0x0ff ); \
	( byte_stream )[ 5 ] = (uint8_t) ( ( ( value ) >> 16 ) & 0x0ff ); \
	( byte_stream )[ 6 ] = (uint8_t) ( ( ( value ) >> 8 ) & 0x0ff ); \
	( byte_stream )[ 7 ] = (uint8_t) ( ( value ) & 0x0ff )

#define byte_stream_copy_from_uint64_little_endian( byte_stream, value ) \
	( byte_stream )[ 7 ] = (uint8_t) ( ( ( value ) >> 56 ) & 0x0ff ); \
	( byte_stream )[ 6 ] = (uint8_t) ( ( ( value ) >> 48 ) & 0x0ff ); \
	( byte_stream )[ 5 ] = (uint8_t) ( ( ( value ) >> 40 ) & 0x0ff ); \
	( byte_stream )[ 4 ] = (uint8_t) ( ( ( value ) >> 32 ) & 0x0ff ); \
	( byte_stream )[ 3 ] = (uint8_t) ( ( ( value ) >> 24 ) & 0x0ff ); \
	( byte_stream )[ 2 ] = (uint8_t) ( ( ( value ) >> 16 ) & 0x0ff ); \
	( byte_stream )[ 1 ] = (uint8_t) ( ( ( value ) >> 8 ) & 0x0ff ); \
	( byte_stream )[ 0 ] = (uint8_t) ( ( value ) & 0x0ff )

#define byte_stream_bit_rotate_left_8bit( byte_stream, number_of_bits ) \
	( ( ( byte_stream ) << ( number_of_bits ) ) | ( ( byte_stream ) >> ( 8 - ( number_of_bits ) ) ) )

#define byte_stream_bit_rotate_right_8bit( byte_stream, number_of_bits ) \
	( ( ( byte_stream ) >> ( number_of_bits ) ) | ( ( byte_stream ) << ( 8 - ( number_of_bits ) ) ) )

#define byte_stream_bit_rotate_left_16bit( byte_stream, number_of_bits ) \
	( ( ( byte_stream ) << ( number_of_bits ) ) | ( ( byte_stream ) >> ( 16 - ( number_of_bits ) ) ) )

#define byte_stream_bit_rotate_right_16bit( byte_stream, number_of_bits ) \
	( ( ( byte_stream ) >> ( number_of_bits ) ) | ( ( byte_stream ) << ( 16 - ( number_of_bits ) ) ) )

#define byte_stream_bit_rotate_left_32bit( byte_stream, number_of_bits ) \
	( ( ( byte_stream ) << ( number_of_bits ) ) | ( ( byte_stream ) >> ( 32 - ( number_of_bits ) ) ) )

#define byte_stream_bit_rotate_right_32bit( byte_stream, number_of_bits ) \
	( ( ( byte_stream ) >> ( number_of_bits ) ) | ( ( byte_stream ) << ( 32 - ( number_of_bits ) ) ) )

#define byte_stream_bit_rotate_left_64bit( byte_stream, number_of_bits ) \
	( ( ( byte_stream ) << ( number_of_bits ) ) | ( ( byte_stream ) >> ( 64 - ( number_of_bits ) ) ) )

#define byte_stream_bit_rotate_right_64bit( byte_stream, number_of_bits ) \
	( ( ( byte_stream ) >> ( number_of_bits ) ) | ( ( byte_stream ) << ( 64 - ( number_of_bits ) ) ) )

#define byte_stream_bit_rotate_left( value, number_of_bits ) \
	( ( ( value ) << ( number_of_bits ) ) | ( ( value ) >> ( ( sizeof( value ) << 3 ) - ( number_of_bits ) ) ) )

#define byte_stream_bit_rotate_right( value, number_of_bits ) \
	( ( ( value ) >> ( number_of_bits ) ) | ( ( value ) << ( ( sizeof( value ) << 3 ) - ( number_of_bits ) ) ) )

#define SSIZE_MAX GRUB_SSIZE_MAX
#define INT_MAX GRUB_INT_MAX
#define INT64_MAX 9223372036854775807LL

#define libcerror_error_set(...)
#define libcerror_error_t int

#define memory_allocate(x) grub_malloc(x)

#define memory_allocate_structure(x) grub_malloc(sizeof(x))

#define memory_reallocate(x, y) grub_realloc(x, y)

#define memory_copy(x, y, z) grub_memcpy(x, y, z)

#define memory_set(x, y, z) grub_memset(x, y, z)

#define memory_free(x) grub_free(x)