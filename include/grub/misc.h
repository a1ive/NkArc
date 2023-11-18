/* misc.h - prototypes for misc functions */
/*
 *  GRUB  --  GRand Unified Bootloader
 *  Copyright (C) 2002,2003,2005,2006,2007,2008,2009,2010  Free Software Foundation, Inc.
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

#ifndef GRUB_MISC_HEADER
#define GRUB_MISC_HEADER	1

#include <stdarg.h>
#include <grub/types.h>
#include <grub/symbol.h>
#include <grub/err.h>

#include <intrin.h>

#define ALIGN_UP(addr, align) \
	(((addr) + (align) - 1) & ~((align) - 1))
#define ALIGN_UP_OVERHEAD(addr, align) ((-(addr)) & ((align)-1))
#define ALIGN_DOWN(addr, align) \
	((addr) & ~((align) - 1))
#define ARRAY_SIZE(array) (sizeof (array) / sizeof (array[0]))
#define COMPILE_TIME_ASSERT(cond) switch (0) { case 1: case !(cond): ; }

#define grub_dprintf(condition, ...) grub_real_dprintf(__FILE__, __LINE__, condition, __VA_ARGS__)

void* EXPORT_FUNC(grub_memmove) (void* dest, const void* src, grub_size_t n);
char* EXPORT_FUNC(grub_strcpy) (char* dest, const char* src);

static inline char*
grub_strncpy(char* dest, const char* src, int c)
{
	char* p = dest;

	while ((*p++ = *src++) != '\0' && --c)
		;

	return dest;
}

static inline char*
grub_stpcpy(char* dest, const char* src)
{
	char* d = dest;
	const char* s = src;

	do
		*d++ = *s;
	while (*s++ != '\0');

	return d - 1;
}

/* XXX: If grub_memmove is too slow, we must implement grub_memcpy.  */
static inline void*
grub_memcpy(void* dest, const void* src, grub_size_t n)
{
	return grub_memmove(dest, src, n);
}

int EXPORT_FUNC(grub_memcmp) (const void* s1, const void* s2, grub_size_t n);
int EXPORT_FUNC(grub_strcmp) (const char* s1, const char* s2);
int EXPORT_FUNC(grub_strncmp) (const char* s1, const char* s2, grub_size_t n);

char* EXPORT_FUNC(grub_strchr) (const char* s, int c);
char* EXPORT_FUNC(grub_strrchr) (const char* s, int c);
int EXPORT_FUNC(grub_strword) (const char* haystack, const char* needle);

/* Copied from gnulib.
	 Written by Bruno Haible <bruno@clisp.org>, 2005. */
static inline char*
grub_strstr(const char* haystack, const char* needle)
{
	/* Be careful not to look at the entire extent of haystack or needle
		 until needed.  This is useful because of these two cases:
			 - haystack may be very long, and a match of needle found early,
			 - needle may be very long, and not even a short initial segment of
			 needle may be found in haystack.  */
	if (*needle != '\0')
	{
		/* Speed up the following searches of needle by caching its first
 character.  */
		char b = *needle++;

		for (;; haystack++)
		{
			if (*haystack == '\0')
				/* No match.  */
				return 0;
			if (*haystack == b)
				/* The first character matches.  */
			{
				const char* rhaystack = haystack + 1;
				const char* rneedle = needle;

				for (;; rhaystack++, rneedle++)
				{
					if (*rneedle == '\0')
						/* Found a match.  */
						return (char*)haystack;
					if (*rhaystack == '\0')
						/* No match.  */
						return 0;
					if (*rhaystack != *rneedle)
						/* Nothing in this round.  */
						break;
				}
			}
		}
	}
	else
		return (char*)haystack;
}

int EXPORT_FUNC(grub_isspace) (int c);

static inline int
grub_isprint(int c)
{
	return (c >= ' ' && c <= '~');
}

static inline int
grub_iscntrl(int c)
{
	return (c >= 0x00 && c <= 0x1F) || c == 0x7F;
}

static inline int
grub_isalpha(int c)
{
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

static inline int
grub_islower(int c)
{
	return (c >= 'a' && c <= 'z');
}

static inline int
grub_isupper(int c)
{
	return (c >= 'A' && c <= 'Z');
}

static inline int
grub_isgraph(int c)
{
	return (c >= '!' && c <= '~');
}

static inline int
grub_isdigit(int c)
{
	return (c >= '0' && c <= '9');
}

static inline int
grub_isxdigit(int c)
{
	return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
}

static inline int
grub_isalnum(int c)
{
	return grub_isalpha(c) || grub_isdigit(c);
}

static inline int
grub_tolower(int c)
{
	if (c >= 'A' && c <= 'Z')
		return c - 'A' + 'a';

	return c;
}

static inline int
grub_toupper(int c)
{
	if (c >= 'a' && c <= 'z')
		return c - 'a' + 'A';

	return c;
}

static inline int
grub_strcasecmp(const char* s1, const char* s2)
{
	while (*s1 && *s2)
	{
		if (grub_tolower((grub_uint8_t)*s1)
			!= grub_tolower((grub_uint8_t)*s2))
			break;

		s1++;
		s2++;
	}

	return (int)grub_tolower((grub_uint8_t)*s1)
		- (int)grub_tolower((grub_uint8_t)*s2);
}

static inline int
grub_strncasecmp(const char* s1, const char* s2, grub_size_t n)
{
	if (n == 0)
		return 0;

	while (*s1 && *s2 && --n)
	{
		if (grub_tolower((grub_uint8_t)*s1)
			!= grub_tolower((grub_uint8_t)*s2))
			break;

		s1++;
		s2++;
	}

	return (int)grub_tolower((grub_uint8_t)*s1)
		- (int)grub_tolower((grub_uint8_t)*s2);
}

/*
 * Do a case insensitive compare of two UUID strings by ignoring all dashes.
 * Note that the parameter n, is the number of significant characters to
 * compare, where significant characters are any except the dash.
 */
static inline int
grub_uuidcasecmp(const char* uuid1, const char* uuid2, grub_size_t n)
{
	if (n == 0)
		return 0;

	while (*uuid1 && *uuid2 && --n)
	{
		/* Skip forward to non-dash on both UUIDs. */
		while ('-' == *uuid1)
			++uuid1;

		while ('-' == *uuid2)
			++uuid2;

		if (grub_tolower((grub_uint8_t)*uuid1) != grub_tolower((grub_uint8_t)*uuid2))
			break;

		uuid1++;
		uuid2++;
	}

	return (int)grub_tolower((grub_uint8_t)*uuid1) - (int)grub_tolower((grub_uint8_t)*uuid2);
}

/*
 * Note that these differ from the C standard's definitions of strtol,
 * strtoul(), and strtoull() by the addition of two const qualifiers on the end
 * pointer, which make the declaration match the *semantic* requirements of
 * their behavior.  This means that instead of:
 *
 *  char *s = "1234 abcd";
 *  char *end;
 *  unsigned long l;
 *
 *  l = grub_strtoul(s, &end, 10);
 *
 * We must one of:
 *
 *  const char *end;
 *  ... or ...
 *  l = grub_strtoul(s, (const char ** const)&end, 10);
 */
unsigned long EXPORT_FUNC(grub_strtoul) (const char* str, const char** const end, int base);
unsigned long long EXPORT_FUNC(grub_strtoull) (const char* str, const char** const end, int base);

static inline long
grub_strtol(const char* str, const char** const end, int base)
{
	int negative = 0;
	unsigned long long magnitude;

	while (*str && grub_isspace(*str))
		str++;

	if (*str == '-')
	{
		negative = 1;
		str++;
	}

	magnitude = grub_strtoull(str, end, base);
	if (negative)
	{
		if (magnitude > (unsigned long)GRUB_LONG_MAX + 1)
		{
			grub_error(GRUB_ERR_OUT_OF_RANGE, "overflow is detected");
			return GRUB_LONG_MIN;
		}
		return -((long)magnitude);
	}
	else
	{
		if (magnitude > GRUB_LONG_MAX)
		{
			grub_error(GRUB_ERR_OUT_OF_RANGE, "overflow is detected");
			return GRUB_LONG_MAX;
		}
		return (long)magnitude;
	}
}

_Check_return_
char* EXPORT_FUNC(grub_strdup) (const char* s);
_Check_return_
char* EXPORT_FUNC(grub_strndup) (const char* s, grub_size_t n);
void* EXPORT_FUNC(grub_memset) (void* s, int c, grub_size_t n);
_Check_return_
grub_size_t EXPORT_FUNC(grub_strlen) (const char* s);
grub_size_t EXPORT_FUNC(grub_strnlen) (const char* s, grub_size_t n);

/* Replace all `ch' characters of `input' with `with' and copy the
	 result into `output'; return EOS address of `output'. */
static inline char*
grub_strchrsub(char* output, const char* input, char ch, const char* with)
{
	while (*input)
	{
		if (*input == ch)
		{
			grub_strcpy(output, with);
			output += grub_strlen(with);
			input++;
			continue;
		}
		*output++ = *input++;
	}
	*output = '\0';
	return output;
}

int EXPORT_FUNC(grub_debug_enabled) (const char* condition);
void EXPORT_FUNC(grub_real_dprintf) (const char* file,
	const int line,
	const char* condition,
	_Printf_format_string_ const char* fmt, ...);
int EXPORT_FUNC(grub_printf) (_Printf_format_string_ const char* fmt, ...);
int EXPORT_FUNC(grub_vprintf) (const char* fmt, va_list ap);
int EXPORT_FUNC(grub_snprintf) (char* str, grub_size_t n, _Printf_format_string_ const char* fmt, ...);
int EXPORT_FUNC(grub_vsnprintf) (char* str, grub_size_t n, const char* fmt,
	va_list ap);
_Check_return_
char* EXPORT_FUNC(grub_xasprintf) (_Printf_format_string_ const char* fmt, ...);
_Check_return_
char* EXPORT_FUNC(grub_xvasprintf) (const char* fmt, va_list ap);

#define GRUB_DIVISION_IN_SOFTWARE 0

grub_uint64_t
grub_divmod64(grub_uint64_t n, grub_uint64_t d, grub_uint64_t* r);

/* Inline functions.  */

static inline char*
grub_memchr(const void* p, int c, grub_size_t len)
{
	const char* s = (const char*)p;
	const char* e = s + len;

	for (; s < e; s++)
		if (*s == c)
			return (char*)s;

	return 0;
}

static inline unsigned int
grub_abs(int x)
{
	if (x < 0)
		return (unsigned int)(-x);
	else
		return (unsigned int)x;
}

#ifdef GRUB_MACHINE_EMU
/* Flag to check if module loading is available.  */
extern const int EXPORT_VAR(grub_no_modules);
#else
#define grub_no_modules 0
#endif

static inline void
grub_error_save(struct grub_error_saved* save)
{
	grub_memcpy(save->errmsg, grub_errmsg, sizeof(save->errmsg));
	save->grub_errno = grub_errno;
	grub_errno = GRUB_ERR_NONE;
}

static inline void
grub_error_load(const struct grub_error_saved* save)
{
	grub_memcpy(grub_errmsg, save->errmsg, sizeof(grub_errmsg));
	grub_errno = save->grub_errno;
}

/*
 * grub_printf_fmt_checks() a fmt string for printf() against an expected
 * format. It is intended for cases where the fmt string could come from
 * an outside source and cannot be trusted.
 *
 * While expected fmt accepts a printf() format string it should be kept
 * as simple as possible. The printf() format strings with positional
 * parameters are NOT accepted, neither for fmt nor for fmt_expected.
 *
 * The fmt is accepted if it has equal or less arguments than fmt_expected
 * and if the type of all arguments match.
 *
 * Returns GRUB_ERR_NONE if fmt is acceptable.
 */
grub_err_t EXPORT_FUNC(grub_printf_fmt_check) (const char* fmt, const char* fmt_expected);

#define grub_boot_time(...)

#define grub_max(a, b) (((a) > (b)) ? (a) : (b))
#define grub_min(a, b) (((a) < (b)) ? (a) : (b))

grub_ssize_t
EXPORT_FUNC(grub_utf8_to_utf16_alloc) (const char* str8, grub_uint16_t** utf16_msg, grub_uint16_t** last_position);

enum grub_human_size_type
{
	GRUB_HUMAN_SIZE_NORMAL,
	GRUB_HUMAN_SIZE_SHORT,
	GRUB_HUMAN_SIZE_SPEED,
};

const char*
grub_get_human_size(grub_uint64_t size, enum grub_human_size_type type);

#endif /* ! GRUB_MISC_HEADER */
