/*
 *  GRUB  --  GRand Unified Bootloader
 *  Copyright (C) 2022 Free Software Foundation, Inc.
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

#ifndef GRUB_MSCOMPRESS_HEADER
#define GRUB_MSCOMPRESS_HEADER 1

#include <grub/symbol.h>
#include <grub/types.h>

 /** Maximum length of a Huffman symbol (in bits) */
#define HUFFMAN_BITS 16

/** Raw huffman symbol */
typedef grub_uint16_t huffman_raw_symbol_t;

/** Quick lookup length for a Huffman symbol (in bits)
 *
 * This is a policy decision.
 */
#define HUFFMAN_QL_BITS 7

 /** Quick lookup shift */
#define HUFFMAN_QL_SHIFT (HUFFMAN_BITS - HUFFMAN_QL_BITS)

/** A Huffman-coded set of symbols of a given length */
struct huffman_symbols
{
	/** Length of Huffman-coded symbols (in bits) */
	grub_uint8_t bits;
	/** Shift to normalise symbols of this length to HUFFMAN_BITS bits */
	grub_uint8_t shift;
	/** Number of Huffman-coded symbols having this length */
	grub_uint16_t freq;
	/** First symbol of this length (normalised to HUFFMAN_BITS bits)
	 *
	 * Stored as a 32-bit value to allow the value
	 * (1<<HUFFMAN_BITS ) to be used for empty sets of symbols
	 * longer than the maximum utilised length.
	 */
	grub_uint32_t start;
	/** Raw symbols having this length */
	huffman_raw_symbol_t* raw;
};

/** A Huffman-coded alphabet */
struct huffman_alphabet
{
	/** Huffman-coded symbol set for each length */
	struct huffman_symbols huf[HUFFMAN_BITS];
	/** Quick lookup table */
	grub_uint8_t lookup[1 << HUFFMAN_QL_BITS];
	/** Raw symbols
	 *
	 * Ordered by Huffman-coded symbol length, then by symbol
	 * value.  This field has a variable length.
	 */
	huffman_raw_symbol_t raw[1];
};

/**
 * Get Huffman symbol length
 *
 * @v sym     Huffman symbol set
 * @ret len   Length (in bits)
 */
static inline __attribute__((always_inline)) unsigned int
grub_huffman_len(struct huffman_symbols* sym)
{
	return sym->bits;
}

/**
 * Get Huffman symbol value
 *
 * @v sym     Huffman symbol set
 * @v huf     Raw input value (normalised to HUFFMAN_BITS bits)
 * @ret raw   Raw symbol value
 */
static inline __attribute__((always_inline)) huffman_raw_symbol_t
grub_huffman_raw(struct huffman_symbols* sym, unsigned int huf)
{
	return sym->raw[huf >> sym->shift];
}

int
grub_huffman_alphabet(struct huffman_alphabet* alphabet,
	grub_uint8_t* lengths, unsigned int count);

struct huffman_symbols*
grub_huffman_sym(struct huffman_alphabet* alphabet, unsigned int huf);

grub_ssize_t
grub_lzx_decompress(const void* data, grub_size_t len, void* buf);

grub_ssize_t
grub_xca_decompress(const void* data, grub_size_t len, void* buf);

#endif /* ! GRUB_MSCOMPRESS_HEADER */
