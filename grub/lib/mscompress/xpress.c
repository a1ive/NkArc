/*
 * Copyright (C) 2012 Michael Brown <mbrown@fensystems.co.uk>.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 */

#include "mscompress.h"

 /** Number of XCA codes */
#define XCA_CODES 512

/** XCA decompressor */
struct xca
{
	/** Huffman alphabet */
	struct huffman_alphabet alphabet;
	/** Raw symbols
	 *
	 * Must immediately follow the Huffman alphabet.
	 */
	huffman_raw_symbol_t raw[XCA_CODES];
	/** Code lengths */
	grub_uint8_t lengths[XCA_CODES];
};

/** XCA symbol Huffman lengths table */
GRUB_PACKED_START
struct xca_huf_len
{
	/** Lengths of each symbol */
	grub_uint8_t nibbles[XCA_CODES / 2];
};
GRUB_PACKED_END

/** Get word from source data stream */
static inline grub_uint16_t
XCA_GET16(const void* src)
{
	const grub_uint16_t* src16 = src;
	src = (const grub_uint8_t*)src + sizeof(*src16);
	return *src16;
}

/** Get byte from source data stream */
static inline grub_uint8_t
XCA_GET8(const void* src)
{
	const grub_uint8_t* src8 = src;
	src = (const grub_uint8_t*)src + sizeof(*src8);
	return *src8;
}

/** XCA source data stream end marker */
#define XCA_END_MARKER 256

/** XCA block size */
#define XCA_BLOCK_SIZE (64 * 1024)

 /**
  * Extract Huffman-coded length of a raw symbol
  *
  * @v lengths    Huffman lengths table
  * @v symbol    Raw symbol
  * @ret len    Huffman-coded length
  */
static inline unsigned int
xca_huf_len(const struct xca_huf_len* lengths, unsigned int symbol)
{
	return (((lengths->nibbles[symbol / 2]) >> (4 * (symbol % 2))) & 0x0f);
}

/**
 * Decompress XCA-compressed data
 *
 * @v data    Compressed data
 * @v len    Length of compressed data
 * @v buf    Decompression buffer, or NULL
 * @ret out_len    Length of decompressed data, or negative error
 */
grub_ssize_t
grub_xca_decompress(const void* data, grub_size_t len, void* buf)
{
	const void* src = data;
	const void* end = (grub_uint8_t*)src + len;
	grub_uint8_t* out = buf;
	grub_size_t out_len = 0;
	grub_size_t out_len_threshold = 0;
	const struct xca_huf_len* lengths;
	struct xca xca;
	grub_uint32_t accum = 0;
	int extra_bits = 0;
	unsigned int huf;
	struct huffman_symbols* sym;
	unsigned int raw;
	unsigned int match_len;
	unsigned int match_offset_bits;
	unsigned int match_offset;
	const grub_uint8_t* copy;
	int rc;

	/* Process data stream */
	while (src < end)
	{
		/* (Re)initialise decompressor if applicable */
		if (out_len >= out_len_threshold)
		{
			/* Construct symbol lengths */
			lengths = src;
			src = (grub_uint8_t*)src + sizeof(*lengths);
			if (src > end)
			{
				return -1;
			}
			for (raw = 0; raw < XCA_CODES; raw++)
				xca.lengths[raw] = xca_huf_len(lengths, raw);

			/* Construct Huffman alphabet */
			if ((rc = grub_huffman_alphabet(&xca.alphabet, xca.lengths, XCA_CODES)) != 0)
				return rc;

			/* Initialise state */
			accum = XCA_GET16(src);
			accum <<= 16;
			accum |= XCA_GET16(src);
			extra_bits = 16;

			/* Determine next threshold */
			out_len_threshold = (out_len + XCA_BLOCK_SIZE);
		}

		/* Determine symbol */
		huf = (accum >> (32 - HUFFMAN_BITS));
		sym = grub_huffman_sym(&xca.alphabet, huf);
		raw = grub_huffman_raw(sym, huf);
		accum <<= grub_huffman_len(sym);
		extra_bits -= grub_huffman_len(sym);
		if (extra_bits < 0)
		{
			accum |= (XCA_GET16(src) << (-extra_bits));
			extra_bits += 16;
		}

		/* Process symbol */
		if (raw < XCA_END_MARKER)
		{
			/* Literal symbol - add to output stream */
			if (buf)
				*(out++) = raw;
			out_len++;
		}
		else if ((raw == XCA_END_MARKER) &&
			((grub_uint8_t*)src >= ((grub_uint8_t*)end - 1)))
		{
			/* End marker symbol */
			return out_len;
		}
		else
		{
			/* LZ77 match symbol */
			raw -= XCA_END_MARKER;
			match_offset_bits = (raw >> 4);
			match_len = (raw & 0x0f);
			if (match_len == 0x0f)
			{
				match_len = XCA_GET8(src);
				if (match_len == 0xff)
				{
					match_len = XCA_GET16(src);
				}
				else
				{
					match_len += 0x0f;
				}
			}
			match_len += 3;
			if (match_offset_bits)
			{
				match_offset =
					((accum >> (32 - match_offset_bits))
						+ (1 << match_offset_bits));
			}
			else
			{
				match_offset = 1;
			}
			accum <<= match_offset_bits;
			extra_bits -= match_offset_bits;
			if (extra_bits < 0)
			{
				accum |= (XCA_GET16(src) << (-extra_bits));
				extra_bits += 16;
			}

			/* Copy data */
			out_len += match_len;
			if (buf)
			{
				copy = (out - match_offset);
				while (match_len--)
					*(out++) = *(copy++);
			}
		}
	}

	/* Allow for termination with no explicit end marker symbol */
	if (src == end)
		return out_len;

	return -1;
}
