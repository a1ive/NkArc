#pragma once

#include <grub/types.h>
#include <grub/misc.h>
#include <grub/mm.h>
#include <grub/err.h>

typedef grub_uint8_t uint8_t;
typedef grub_uint16_t uint16_t;
typedef grub_uint32_t uint32_t;
typedef grub_uint64_t uint64_t;

typedef grub_int8_t int8_t;
typedef grub_int16_t int16_t;
typedef grub_int32_t int32_t;
typedef grub_int64_t int64_t;

typedef grub_ssize_t ssize_t;
typedef grub_size_t size_t;
typedef grub_uint64_t size64_t;

typedef grub_int64_t off64_t;

typedef char system_character_t;

#ifdef GRUB_CPU_WORDS_BIGENDIAN
#define _BYTE_STREAM_HOST_IS_ENDIAN_BIG		1
#define _BYTE_STREAM_HOST_IS_ENDIAN_LITTLE	0
#define _BYTE_STREAM_HOST_IS_ENDIAN_MIDDLE	0
#else
#define _BYTE_STREAM_HOST_IS_ENDIAN_BIG		0
#define _BYTE_STREAM_HOST_IS_ENDIAN_LITTLE	1
#define _BYTE_STREAM_HOST_IS_ENDIAN_MIDDLE	0
#endif

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
#define INT16_MAX 32767
#define INT32_MAX GRUB_INT32_MAX
#define INT64_MAX 9223372036854775807LL
#define UINT8_MAX 255
#define UINT16_MAX 65535
#define UINT32_MAX 4294967295U
#define ULONG_MAX GRUB_ULONG_MAX

#define libcerror_error_set(...)
#define libcerror_error_free(x)
#define libcerror_error_t int

#define MEMORY_MAXIMUM_ALLOCATION_SIZE \
	( 128 * 1024 * 1024 )

#define memory_allocate(x) grub_malloc(x)

#define memory_allocate_structure(x) grub_malloc(sizeof(x))

#define memory_reallocate(x, y) grub_realloc(x, y)

#define memory_copy(x, y, z) grub_memcpy(x, y, z)

#define memory_compare(x, y, z) grub_memcmp(x, y, z)

#define memory_set(x, y, z) grub_memset(x, y, z)

#define memory_free(x) grub_free(x)

#ifndef SEEK_CUR
#define SEEK_CUR    1
#endif
#ifndef SEEK_END
#define SEEK_END    2
#endif
#ifndef SEEK_SET
#define SEEK_SET    0
#endif

#define narrow_string_allocate( size ) \
	(char *) memory_allocate( sizeof( char ) * ( size ) )

#define narrow_string_reallocate( string, size ) \
	(char *) memory_reallocate( string, ( sizeof( char ) * ( size ) ) )

#define narrow_string_length( string ) \
	grub_strlen( string )

#define narrow_string_compare( string1, string2, size ) \
	grub_strncmp( (void *) string1, (void *) string2, size )

#define narrow_string_compare_no_case( string1, string2, size ) \
	grub_strncasecmp( string1, string2, size )

#define narrow_string_copy( destination, source, size ) \
	grub_strncpy( destination, source, size )

#define narrow_string_search_character( string, character, size ) \
	grub_strchr( string, (int) character )

#define narrow_string_search_character_reverse( string, character, size ) \
	grub_strrchr( string, (int) character )

#define narrow_string_search_string( string, substring, size ) \
	grub_strstr( string, substring )

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )

#if SIZEOF_WCHAR_T != 2
#error Unsupported wide system character size
#endif

/* Intermediate version of the macro required
 * for correct evaluation predefined string
 */
#define _SYSTEM_STRING_INTERMEDIATE( string ) \
	L ## string

#define _SYSTEM_STRING( string ) \
	_SYSTEM_STRING_INTERMEDIATE( string )

#define system_string_allocate( size ) \
	wide_string_allocate( size )

#define system_string_reallocate( string, size ) \
	wide_string_reallocate( string, size )

#define system_string_compare( destination, source, size ) \
	wide_string_compare( destination, source, size )

#define system_string_compare_no_case( destination, source, size ) \
	wide_string_compare_no_case( destination, source, size )

#define system_string_copy( destination, source, size ) \
	wide_string_copy( destination, source, size )

#define system_string_length( string ) \
	wide_string_length( string )

#define system_string_search_character( string, character, size ) \
	wide_string_search_character( string, character, size )

#define system_string_search_character_reverse( string, character, size ) \
	wide_string_search_character_reverse( string, character, size )

#define system_string_search_string( string, substring, size ) \
	wide_string_search_string( string, substring, size )

#else

#define _SYSTEM_STRING( string ) \
	string

#define system_string_allocate( size ) \
	narrow_string_allocate( size )

#define system_string_reallocate( string, size ) \
	narrow_string_reallocate( string, size )

#define system_string_compare( destination, source, size ) \
	narrow_string_compare( destination, source, size )

#define system_string_compare_no_case( destination, source, size ) \
	narrow_string_compare_no_case( destination, source, size )

#define system_string_copy( destination, source, size ) \
	narrow_string_copy( destination, source, size )

#define system_string_length( string ) \
	narrow_string_length( string )

#define system_string_search_character( string, character, size ) \
	narrow_string_search_character( string, character, size )

#define system_string_search_character_reverse( string, character, size ) \
	narrow_string_search_character_reverse( string, character, size )

#define system_string_search_string( string, substring, size ) \
	narrow_string_search_string( string, substring, size )

#endif /* defined( HAVE_WIDE_SYSTEM_CHARACTER ) */
