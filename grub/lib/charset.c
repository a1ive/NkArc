/*
 *  GRUB  --  GRand Unified Bootloader
 *  Copyright (C) 1999,2000,2001,2002,2003,2004,2005,2006,2007,2008,2009  Free Software Foundation, Inc.
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

 /*
   Current problems with Unicode rendering:
   - B and BN bidi type characters (ignored)
   - Mc type characters with combining class 0 (poorly combined)
   - Mn type characters with combining class 0 (poorly combined)
   - Me type characters with combining class 0 (poorly combined)
   - Cf type characters (ignored)
   - Cc type characters (ignored)
   - Line-breaking rules (e.g. Zs type characters)
   - Indic languages
   - non-Semitic shaping (rarely used)
   - Zl and Zp characters
   - Combining characters of types 7, 8, 9, 21, 35, 36, 84, 91, 103, 107,
   118, 122, 129, 130, 132, 218, 224, 226, 233, 234
   - Private use characters (not really a problem)
   - Variations (no font support)
   - Vertical text
   - Ligatures
   Font information ignored:
   - Kerning
   - Justification data
   - Glyph posititioning
   - Baseline data
   Most underline diacritics aren't displayed in gfxterm
  */

#include <grub/charset.h>
#include <grub/mm.h>
#include <grub/misc.h>
#include <grub/safemath.h>

  /* Returns -2 if not enough space, -1 on invalid character.  */
grub_ssize_t
grub_encode_utf8_character(grub_uint8_t* dest, grub_uint8_t* destend,
	grub_uint32_t code)
{
	if (dest >= destend)
		return -2;
	if (code <= 0x007F)
	{
		*dest++ = code;
		return 1;
	}
	if (code <= 0x07FF)
	{
		if (dest + 1 >= destend)
			return -2;
		*dest++ = (code >> 6) | 0xC0;
		*dest++ = (code & 0x3F) | 0x80;
		return 2;
	}
	if ((code >= 0xDC00 && code <= 0xDFFF)
		|| (code >= 0xD800 && code <= 0xDBFF))
	{
		/* No surrogates in UCS-4... */
		return -1;
	}
	if (code < 0x10000)
	{
		if (dest + 2 >= destend)
			return -2;
		*dest++ = (code >> 12) | 0xE0;
		*dest++ = ((code >> 6) & 0x3F) | 0x80;
		*dest++ = (code & 0x3F) | 0x80;
		return 3;
	}
	{
		if (dest + 3 >= destend)
			return -2;
		*dest++ = (code >> 18) | 0xF0;
		*dest++ = ((code >> 12) & 0x3F) | 0x80;
		*dest++ = ((code >> 6) & 0x3F) | 0x80;
		*dest++ = (code & 0x3F) | 0x80;
		return 4;
	}

}

/* Convert UCS-4 to UTF-8.  */
grub_size_t
grub_ucs4_to_utf8(const grub_uint32_t* src, grub_size_t size,
	grub_uint8_t* dest, grub_size_t destsize)
{
	/* Keep last char for \0.  */
	grub_uint8_t* destend = dest + destsize - 1;
	grub_uint8_t* dest0 = dest;

	while (size-- && dest < destend)
	{
		grub_uint32_t code = *src++;
		grub_ssize_t s;
		s = grub_encode_utf8_character(dest, destend, code);
		if (s == -2)
			break;
		if (s == -1)
		{
			*dest++ = '?';
			continue;
		}
		dest += s;
	}
	*dest = 0;
	return dest - dest0;
}

/* Returns the number of bytes the string src would occupy is converted
   to UTF-8, excluding trailing \0.  */
grub_size_t
grub_get_num_of_utf8_bytes(const grub_uint32_t* src, grub_size_t size)
{
	grub_size_t remaining;
	const grub_uint32_t* ptr;
	grub_size_t cnt = 0;

	remaining = size;
	ptr = src;
	while (remaining--)
	{
		grub_uint32_t code = *ptr++;

		if (code <= 0x007F)
			cnt++;
		else if (code <= 0x07FF)
			cnt += 2;
		else if ((code >= 0xDC00 && code <= 0xDFFF)
			|| (code >= 0xD800 && code <= 0xDBFF))
			/* No surrogates in UCS-4... */
			cnt++;
		else if (code < 0x10000)
			cnt += 3;
		else
			cnt += 4;
	}
	return cnt;
}

/* Convert UCS-4 to UTF-8.  */
char*
grub_ucs4_to_utf8_alloc(const grub_uint32_t* src, grub_size_t size)
{
	grub_uint8_t* ret;
	grub_size_t cnt = grub_get_num_of_utf8_bytes(src, size) + 1;

	ret = grub_malloc(cnt);
	if (!ret)
		return 0;

	grub_ucs4_to_utf8(src, size, ret, cnt);

	return (char*)ret;
}

int
grub_is_valid_utf8(const grub_uint8_t* src, grub_size_t srcsize)
{
	int count = 0;
	grub_uint32_t code = 0;

	while (srcsize)
	{
		if (srcsize != (grub_size_t)-1)
			srcsize--;
		if (!grub_utf8_process(*src++, &code, &count))
			return 0;
		if (count != 0)
			continue;
		if (code == 0)
			return 1;
		if (code > 0x10ffff)
			return 0;
	}

	return 1;
}

grub_ssize_t
grub_utf8_to_ucs4_alloc(const char* msg, grub_uint32_t** unicode_msg,
	grub_uint32_t** last_position)
{
	grub_size_t msg_len = grub_strlen(msg);

	*unicode_msg = grub_calloc(msg_len, sizeof(grub_uint32_t));

	if (!*unicode_msg)
		return -1;

	msg_len = grub_utf8_to_ucs4(*unicode_msg, msg_len,
		(grub_uint8_t*)msg, -1, 0);

	if (last_position)
		*last_position = *unicode_msg + msg_len;

	return msg_len;
}

/* Convert a (possibly null-terminated) UTF-8 string of at most SRCSIZE
   bytes (if SRCSIZE is -1, it is ignored) in length to a UCS-4 string.
   Return the number of characters converted. DEST must be able to hold
   at least DESTSIZE characters.
   If SRCEND is not NULL, then *SRCEND is set to the next byte after the
   last byte used in SRC.  */
grub_size_t
grub_utf8_to_ucs4(grub_uint32_t* dest, grub_size_t destsize,
	const grub_uint8_t* src, grub_size_t srcsize,
	const grub_uint8_t** srcend)
{
	grub_uint32_t* p = dest;
	int count = 0;
	grub_uint32_t code = 0;

	if (srcend)
		*srcend = src;

	while (srcsize && destsize)
	{
		int was_count = count;
		if (srcsize != (grub_size_t)-1)
			srcsize--;
		if (!grub_utf8_process(*src++, &code, &count))
		{
			code = '?';
			count = 0;
			/* Character c may be valid, don't eat it.  */
			if (was_count)
				src--;
		}
		if (count != 0)
			continue;
		if (code == 0)
			break;
		*p++ = code;
		destsize--;
	}

	if (srcend)
		*srcend = src;
	return p - dest;
}
