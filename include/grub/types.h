/*
 *  GRUB  --  GRand Unified Bootloader
 *  Copyright (C) 2002,2005,2006,2007,2008,2009  Free Software Foundation, Inc.
 *
 *  GRUB is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  GRUB is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with GRUB.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef GRUB_TYPES_HEADER
#define GRUB_TYPES_HEADER	1

#include <config.h>
#include <stdbool.h>

#define GRUB_PACKED			#error "GRUB_PACKED is not defined"
#define GRUB_PACKED_START	__pragma(pack(push, 1))
#define GRUB_PACKED_END		__pragma(pack(pop))

#ifdef _WIN64
#define GRUB_CPU_SIZEOF_VOID_P	8
#define GRUB_CPU_SIZEOF_LONG	4
#define GRUB_TARGET_WORDSIZE	64
#else
#define GRUB_CPU_SIZEOF_VOID_P	4
#define GRUB_CPU_SIZEOF_LONG	4
#define GRUB_TARGET_WORDSIZE	32
#endif

#undef GRUB_CPU_WORDS_BIGENDIAN

#if GRUB_CPU_SIZEOF_VOID_P != 4 && GRUB_CPU_SIZEOF_VOID_P != 8
# error "This architecture is not supported because sizeof(void *) != 4 and sizeof(void *) != 8"
#endif

#if GRUB_CPU_SIZEOF_LONG != 4 && GRUB_CPU_SIZEOF_LONG != 8
# error "This architecture is not supported because sizeof(long) != 4 and sizeof(long) != 8"
#endif

#ifndef __CHAR_BIT__
# error __CHAR_BIT__ is not defined
#elif __CHAR_BIT__ != 8
# error __CHAR_BIT__ is not equal 8
#else
# define GRUB_CHAR_BIT	__CHAR_BIT__
#endif

#define GRUB_TYPE_BITS(type) (sizeof(type) * GRUB_CHAR_BIT)

 /* Define various wide integers.  */
typedef signed char		grub_int8_t;
typedef short			grub_int16_t;
typedef int				grub_int32_t;
# define PRIxGRUB_INT32_T	"x"
# define PRIdGRUB_INT32_T	"d"
#if GRUB_CPU_SIZEOF_LONG == 8
typedef long			grub_int64_t;
# define PRIxGRUB_INT64_T	"lx"
# define PRIdGRUB_INT64_T	"ld"
#else
typedef long long		grub_int64_t;
# define PRIxGRUB_INT64_T	"llx"
# define PRIdGRUB_INT64_T	"lld"
#endif

typedef unsigned char		grub_uint8_t;
typedef unsigned short		grub_uint16_t;
typedef unsigned int		grub_uint32_t;
# define PRIxGRUB_UINT32_T	"x"
# define PRIuGRUB_UINT32_T	"u"
#if GRUB_CPU_SIZEOF_LONG == 8
typedef unsigned long		grub_uint64_t;
# define PRIxGRUB_UINT64_T	"lx"
# define PRIuGRUB_UINT64_T	"lu"
#else
typedef unsigned long long	grub_uint64_t;
# define PRIxGRUB_UINT64_T	"llx"
# define PRIuGRUB_UINT64_T	"llu"
#endif

/* Misc types.  */

#if GRUB_CPU_SIZEOF_VOID_P == 8
typedef grub_uint64_t	grub_addr_t;
typedef grub_uint64_t	grub_size_t;
typedef grub_int64_t	grub_ssize_t;

# define GRUB_SIZE_MAX	18446744073709551615UL
# define GRUB_SSIZE_MAX	9223372036854775807L

# if GRUB_CPU_SIZEOF_LONG == 8
#  define PRIxGRUB_SIZE		"lx"
#  define PRIxGRUB_ADDR		"lx"
#  define PRIuGRUB_SIZE		"lu"
#  define PRIdGRUB_SSIZE	"ld"
# else
#  define PRIxGRUB_SIZE		"llx"
#  define PRIxGRUB_ADDR		"llx"
#  define PRIuGRUB_SIZE		"llu"
#  define PRIdGRUB_SSIZE	"lld"
# endif
#else
typedef grub_uint32_t	grub_addr_t;
typedef grub_uint32_t	grub_size_t;
typedef grub_int32_t	grub_ssize_t;

# define GRUB_SIZE_MAX	4294967295UL
# define GRUB_SSIZE_MAX	2147483647L

# define PRIxGRUB_SIZE	"x"
# define PRIxGRUB_ADDR	"x"
# define PRIuGRUB_SIZE	"u"
# define PRIdGRUB_SSIZE	"d"
#endif

#define GRUB_SCHAR_MAX	127
#define GRUB_SCHAR_MIN	(-GRUB_SCHAR_MAX - 1)
#define GRUB_UCHAR_MAX	0xFF
#define GRUB_USHRT_MAX	65535
#define GRUB_SHRT_MAX	0x7fff
#define GRUB_SHRT_MIN	(-GRUB_SHRT_MAX - 1)
#define GRUB_UINT_MAX	4294967295U
#define GRUB_INT_MAX	0x7fffffff
#define GRUB_INT_MIN	(-GRUB_INT_MAX - 1)
#define GRUB_INT32_MAX	2147483647
#define GRUB_INT32_MIN	(-GRUB_INT32_MAX - 1)

#if GRUB_CPU_SIZEOF_LONG == 8
# define GRUB_ULONG_MAX	18446744073709551615UL
# define GRUB_LONG_MAX	9223372036854775807L
#else
# define GRUB_ULONG_MAX	4294967295UL
# define GRUB_LONG_MAX	2147483647L
#endif
# define GRUB_LONG_MIN	(-GRUB_LONG_MAX - 1)

/*
 * Cast to unsigned long long so that the "return value" is always a consistent
 * type and to ensure an unsigned value regardless of type parameter.
 */
 //#define GRUB_TYPE_U_MAX(type)	((unsigned long long)((typeof (type))(~0)))
 //#define GRUB_TYPE_U_MIN(type)	0ULL

typedef grub_uint64_t grub_properly_aligned_t;

#define GRUB_PROPERLY_ALIGNED_ARRAY(name, size) grub_properly_aligned_t name[((size) + sizeof (grub_properly_aligned_t) - 1) / sizeof (grub_properly_aligned_t)]

/* The type for representing a file offset.  */
typedef grub_uint64_t		grub_off_t;
#define PRIxGRUB_OFFSET		PRIxGRUB_UINT64_T
#define PRIuGRUB_OFFSET		PRIuGRUB_UINT64_T

/* The type for representing a disk block address.  */
typedef grub_uint64_t		grub_disk_addr_t;
#define PRIxGRUB_DISK_ADDR	PRIxGRUB_UINT64_T

/* Byte-orders.  */
static inline grub_uint16_t grub_swap_bytes16(grub_uint16_t _x)
{
	return (grub_uint16_t)((_x << 8) | (_x >> 8));
}

#define grub_swap_bytes16_compile_time(x)	((((x) & 0xff) << 8) | (((x) & 0xff00) >> 8))
#define grub_swap_bytes32_compile_time(x)	((((x) & 0xff) << 24) | (((x) & 0xff00) << 8) | (((x) & 0xff0000) >> 8) | (((x) & 0xff000000UL) >> 24))
#define grub_swap_bytes64_compile_time(x)	\
((grub_uint64_t) ((((grub_uint64_t)x) << 56) \
				| ((((grub_uint64_t)x) & (grub_uint64_t) 0xFF00ULL) << 40) \
				| ((((grub_uint64_t)x) & (grub_uint64_t) 0xFF0000ULL) << 24) \
				| ((((grub_uint64_t)x) & (grub_uint64_t) 0xFF000000ULL) << 8) \
				| ((((grub_uint64_t)x) & (grub_uint64_t) 0xFF00000000ULL) >> 8) \
				| ((((grub_uint64_t)x) & (grub_uint64_t) 0xFF0000000000ULL) >> 24) \
				| ((((grub_uint64_t)x) & (grub_uint64_t) 0xFF000000000000ULL) >> 40) \
				| (((grub_uint64_t)x) >> 56)) \
)

static inline grub_uint32_t grub_swap_bytes32(grub_uint32_t _x)
{
	return ((_x << 24)
		| ((_x & (grub_uint32_t)0xFF00UL) << 8)
		| ((_x & (grub_uint32_t)0xFF0000UL) >> 8)
		| (_x >> 24));
}

static inline grub_uint64_t grub_swap_bytes64(grub_uint64_t _x)
{
	return ((_x << 56)
		| ((_x & (grub_uint64_t)0xFF00ULL) << 40)
		| ((_x & (grub_uint64_t)0xFF0000ULL) << 24)
		| ((_x & (grub_uint64_t)0xFF000000ULL) << 8)
		| ((_x & (grub_uint64_t)0xFF00000000ULL) >> 8)
		| ((_x & (grub_uint64_t)0xFF0000000000ULL) >> 24)
		| ((_x & (grub_uint64_t)0xFF000000000000ULL) >> 40)
		| (_x >> 56));
}

#ifdef GRUB_CPU_WORDS_BIGENDIAN
# define grub_cpu_to_le16(x)	grub_swap_bytes16(x)
# define grub_cpu_to_le32(x)	grub_swap_bytes32(x)
# define grub_cpu_to_le64(x)	grub_swap_bytes64(x)
# define grub_le_to_cpu16(x)	grub_swap_bytes16(x)
# define grub_le_to_cpu32(x)	grub_swap_bytes32(x)
# define grub_le_to_cpu64(x)	grub_swap_bytes64(x)
# define grub_cpu_to_be16(x)	((grub_uint16_t) (x))
# define grub_cpu_to_be32(x)	((grub_uint32_t) (x))
# define grub_cpu_to_be64(x)	((grub_uint64_t) (x))
# define grub_be_to_cpu16(x)	((grub_uint16_t) (x))
# define grub_be_to_cpu32(x)	((grub_uint32_t) (x))
# define grub_be_to_cpu64(x)	((grub_uint64_t) (x))
# define grub_cpu_to_be16_compile_time(x)	((grub_uint16_t) (x))
# define grub_cpu_to_be32_compile_time(x)	((grub_uint32_t) (x))
# define grub_cpu_to_be64_compile_time(x)	((grub_uint64_t) (x))
# define grub_be_to_cpu64_compile_time(x)	((grub_uint64_t) (x))
# define grub_cpu_to_le32_compile_time(x)	grub_swap_bytes32_compile_time(x)
# define grub_cpu_to_le64_compile_time(x)	grub_swap_bytes64_compile_time(x)
# define grub_cpu_to_le16_compile_time(x)	grub_swap_bytes16_compile_time(x)
#else /* ! WORDS_BIGENDIAN */
# define grub_cpu_to_le16(x)	((grub_uint16_t) (x))
# define grub_cpu_to_le32(x)	((grub_uint32_t) (x))
# define grub_cpu_to_le64(x)	((grub_uint64_t) (x))
# define grub_le_to_cpu16(x)	((grub_uint16_t) (x))
# define grub_le_to_cpu32(x)	((grub_uint32_t) (x))
# define grub_le_to_cpu64(x)	((grub_uint64_t) (x))
# define grub_cpu_to_be16(x)	grub_swap_bytes16(x)
# define grub_cpu_to_be32(x)	grub_swap_bytes32(x)
# define grub_cpu_to_be64(x)	grub_swap_bytes64(x)
# define grub_be_to_cpu16(x)	grub_swap_bytes16(x)
# define grub_be_to_cpu32(x)	grub_swap_bytes32(x)
# define grub_be_to_cpu64(x)	grub_swap_bytes64(x)
# define grub_cpu_to_be16_compile_time(x)	grub_swap_bytes16_compile_time(x)
# define grub_cpu_to_be32_compile_time(x)	grub_swap_bytes32_compile_time(x)
# define grub_cpu_to_be64_compile_time(x)	grub_swap_bytes64_compile_time(x)
# define grub_be_to_cpu64_compile_time(x)	grub_swap_bytes64_compile_time(x)
# define grub_cpu_to_le16_compile_time(x)	((grub_uint16_t) (x))
# define grub_cpu_to_le32_compile_time(x)	((grub_uint32_t) (x))
# define grub_cpu_to_le64_compile_time(x)	((grub_uint64_t) (x))

#endif /* ! WORDS_BIGENDIAN */

GRUB_PACKED_START
struct grub_unaligned_uint16
{
	grub_uint16_t val;
};
GRUB_PACKED_END

GRUB_PACKED_START
struct grub_unaligned_uint32
{
	grub_uint32_t val;
};
GRUB_PACKED_END

GRUB_PACKED_START
struct grub_unaligned_uint64
{
	grub_uint64_t val;
};
GRUB_PACKED_END

typedef struct grub_unaligned_uint16 grub_unaligned_uint16_t;
typedef struct grub_unaligned_uint32 grub_unaligned_uint32_t;
typedef struct grub_unaligned_uint64 grub_unaligned_uint64_t;

static inline grub_uint16_t grub_get_unaligned16(const void* ptr)
{
	const struct grub_unaligned_uint16* dd
		= (const struct grub_unaligned_uint16*)ptr;
	return dd->val;
}

static inline void grub_set_unaligned16(void* ptr, grub_uint16_t val)
{
	struct grub_unaligned_uint16* dd = (struct grub_unaligned_uint16*)ptr;
	dd->val = val;
}

static inline grub_uint32_t grub_get_unaligned32(const void* ptr)
{
	const struct grub_unaligned_uint32* dd
		= (const struct grub_unaligned_uint32*)ptr;
	return dd->val;
}

static inline void grub_set_unaligned32(void* ptr, grub_uint32_t val)
{
	struct grub_unaligned_uint32* dd = (struct grub_unaligned_uint32*)ptr;
	dd->val = val;
}

static inline grub_uint64_t grub_get_unaligned64(const void* ptr)
{
	const struct grub_unaligned_uint64* dd
		= (const struct grub_unaligned_uint64*)ptr;
	return dd->val;
}

static inline void grub_set_unaligned64(void* ptr, grub_uint64_t val)
{
	struct grub_unaligned_uint64* dd = (struct grub_unaligned_uint64*)ptr;
	dd->val = val;
}

/*
 * The grub_absolute_pointer() macro borrows the idea from Linux kernel of using
 * RELOC_HIDE() macro to stop GCC from checking the result of pointer arithmetic
 * and also its conversion to be inside the symbol's boundary [1]. The check
 * is sometimes false positive, especially it is controversial to emit the array
 * bounds [-Warray-bounds] warning on all hardwired literal pointers since GCC
 * 11/12 [2]. Unless a good solution can be settled, for the time being we
 * would be in favor of the macro instead of GCC pragmas which cannot match the
 * places the warning needs to be ignored in an exact way.
 *
 * [1] https://lists.linuxcoding.com/kernel/2006-q3/msg17979.html
 * [2] https://gcc.gnu.org/bugzilla/show_bug.cgi?id=99578
 */
#if defined(__GNUC__)
# define grub_absolute_pointer(val)					\
({									\
	grub_addr_t __ptr;						\
	asm ("" : "=r" (__ptr) : "0" ((void *) (val)));			\
	(void *) (__ptr);						\
})
#else
# define grub_absolute_pointer(val) ((void *) (val))
#endif

__pragma(pack(push, 4))
struct grub_guid
{
	grub_uint32_t data1;
	grub_uint16_t data2;
	grub_uint16_t data3;
	grub_uint8_t data4[8];
};
__pragma(pack(pop))
typedef struct grub_guid grub_guid_t;

GRUB_PACKED_START
struct grub_packed_guid
{
	grub_uint32_t data1;
	grub_uint16_t data2;
	grub_uint16_t data3;
	grub_uint8_t data4[8];
};
GRUB_PACKED_END
typedef struct grub_packed_guid grub_packed_guid_t;

#endif /* ! GRUB_TYPES_HEADER */
