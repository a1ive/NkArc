/*
 * Copyright (C) 2006-2023 Oracle and/or its affiliates.
 *
 * This file is part of VirtualBox base platform packages, as
 * available from https://www.virtualbox.org.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation, in version 3 of the
 * License.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <https://www.gnu.org/licenses>.
 *
 * The contents of this file may alternatively be used under the terms
 * of the Common Development and Distribution License Version 1.0
 * (CDDL), a copy of it is provided in the "COPYING.CDDL" file included
 * in the VirtualBox distribution, in which case the provisions of the
 * CDDL are applicable instead of those of the GPL.
 *
 * You may elect to license modified versions of this file under the
 * terms and conditions of either the GPL or the CDDL or both.
 *
 * SPDX-License-Identifier: GPL-3.0-only OR CDDL-1.0
 */

#pragma once

#include <grub/types.h>

#define _64K                   0x00010000
#define _128K                  0x00020000
#define _1M                    0x00100000
#define _1T                    0x0000010000000000LL

#define RT_SUCCESS(rc) (rc == GRUB_ERR_NONE)
#define RT_FAILURE(rc) (rc != GRUB_ERR_NONE)

#define RT_MIN(Value1, Value2) ((Value1) <= (Value2) ? (Value1) : (Value2))

#define RT_BIT(bit)            (1U << (bit))
#define RT_BIT_32(bit)         (1U << (bit))
#define RT_BIT_64(bit)         (1ULL << (bit))

/** No flags. */
#define VD_IMAGE_FLAGS_NONE                     (0)
/** Fixed image. */
#define VD_IMAGE_FLAGS_FIXED                    (0x10000)
/** Diff image. Mutually exclusive with fixed image. */
#define VD_IMAGE_FLAGS_DIFF                     (0x20000)
/** VMDK: Split image into 2GB extents. */
#define VD_VMDK_IMAGE_FLAGS_SPLIT_2G            (0x0001)
/** VMDK: Raw disk image (giving access to a number of host partitions). */
#define VD_VMDK_IMAGE_FLAGS_RAWDISK             (0x0002)
/** VMDK: stream optimized image, read only. */
#define VD_VMDK_IMAGE_FLAGS_STREAM_OPTIMIZED    (0x0004)
/** VMDK: ESX variant, use in addition to other flags. */
#define VD_VMDK_IMAGE_FLAGS_ESX                 (0x0008)
/** VDI: Fill new blocks with zeroes while expanding image file. Only valid
 * for newly created images, never set for opened existing images. */
#define VD_VDI_IMAGE_FLAGS_ZERO_EXPAND          (0x0100)

typedef union RTUUID
{
	/** 8-bit view. */
	grub_uint8_t     au8[16];
	/** 16-bit view. */
	grub_uint16_t    au16[8];
	/** 32-bit view. */
	grub_uint32_t    au32[4];
	/** 64-bit view. */
	grub_uint64_t    au64[2];
	/** The way the UUID is declared by the DCE specification. */
	struct
	{
		grub_uint32_t    u32TimeLow;
		grub_uint16_t    u16TimeMid;
		grub_uint16_t    u16TimeHiAndVersion;
		grub_uint8_t     u8ClockSeqHiAndReserved;
		grub_uint8_t     u8ClockSeqLow;
		grub_uint8_t     au8Node[6];
	} Gen;
} RTUUID;
/** Pointer to UUID data. */
typedef RTUUID* PRTUUID;
/** Pointer to readonly UUID data. */
typedef const RTUUID* PCRTUUID;

#ifdef GRUB_CPU_WORDS_BIGENDIAN

#define RT_BE2H_U16(x) ((grub_uint16_t) (x)
#define RT_BE2H_U32(x) ((grub_uint32_t) (x)
#define RT_BE2H_U64(x) ((grub_uint64_t) (x))

#define RT_H2LE_U64(x)   grub_swap_bytes64(x)
#define RT_H2LE_U32(x)   grub_swap_bytes32(x)
#define RT_H2LE_U16(x)   grub_swap_bytes16(x)

#define RT_LE2H_U64(x)   grub_swap_bytes64(x)
#define RT_LE2H_U32(x)   grub_swap_bytes32(x)
#define RT_LE2H_U16(x)   grub_swap_bytes16(x)

#else

#define RT_BE2H_U16(x) (grub_swap_bytes16 (x))
#define RT_BE2H_U32(x) (grub_swap_bytes32 (x))
#define RT_BE2H_U64(x) (grub_swap_bytes64 (x))

#define RT_H2LE_U64(x)  ((grub_uint64_t) (x))
#define RT_H2LE_U32(x)  ((grub_uint32_t) (x))
#define RT_H2LE_U16(x)  ((grub_uint16_t) (x))

#define RT_LE2H_U64(x)  ((grub_uint64_t) (x))
#define RT_LE2H_U32(x)  ((grub_uint32_t) (x))
#define RT_LE2H_U16(x)  ((grub_uint16_t) (x))

#endif

/** @def RT_ELEMENTS
 * Calculates the number of elements in a statically sized array.
 * @returns Element count.
 * @param   aArray      Array in question.
 */
#define RT_ELEMENTS(aArray) (sizeof(aArray) / sizeof((aArray)[0]))

int
RTUuidIsNull(PCRTUUID pUuid);

int
RTUuidCompare(PCRTUUID pUuid1, PCRTUUID pUuid2);

int
RTUuidFromStr(PRTUUID pUuid, const char* pszString);

int
RTUuidCompareStr(PCRTUUID pUuid1, const char* pszString2);

grub_uint32_t
RTCrc32C(const void* pv, grub_size_t cb);
