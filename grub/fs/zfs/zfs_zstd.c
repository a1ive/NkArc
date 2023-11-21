// SPDX-License-Identifier: GPL-2.0+
/*
 *  GRUB  --  GRand Unified Bootloader
 *  Copyright (C) 1999,2000,2001,2002,2003,2004  Free Software Foundation, Inc.
 */

#define	ZSTD_STATIC_LINKING_ONLY
#include "../../lib/zstd/zstd.h"
#include <grub/err.h>
#include <grub/file.h>
#include <grub/mm.h>
#include <grub/misc.h>
#include <grub/disk.h>
#include <grub/types.h>

#include <grub/zfs/zfs.h>
#include <grub/zfs/zio.h>
#include <grub/zfs/dnode.h>
#include <grub/zfs/uberblock_impl.h>
#include <grub/zfs/vdev_impl.h>
#include <grub/zfs/zio_checksum.h>
#include <grub/zfs/zap_impl.h>
#include <grub/zfs/zap_leaf.h>
#include <grub/zfs/zfs_znode.h>
#include <grub/zfs/dmu.h>
#include <grub/zfs/dmu_objset.h>
#include <grub/zfs/sa_impl.h>
#include <grub/zfs/dsl_dir.h>
#include <grub/zfs/dsl_dataset.h>

enum zio_zstd_levels
{
	ZIO_ZSTD_LEVEL_INHERIT = 0,
	ZIO_ZSTD_LEVEL_1,
#define	ZIO_ZSTD_LEVEL_MIN	ZIO_ZSTD_LEVEL_1
	ZIO_ZSTD_LEVEL_2,
	ZIO_ZSTD_LEVEL_3,
#define	ZIO_ZSTD_LEVEL_DEFAULT	ZIO_ZSTD_LEVEL_3
	ZIO_ZSTD_LEVEL_4,
	ZIO_ZSTD_LEVEL_5,
	ZIO_ZSTD_LEVEL_6,
	ZIO_ZSTD_LEVEL_7,
	ZIO_ZSTD_LEVEL_8,
	ZIO_ZSTD_LEVEL_9,
	ZIO_ZSTD_LEVEL_10,
	ZIO_ZSTD_LEVEL_11,
	ZIO_ZSTD_LEVEL_12,
	ZIO_ZSTD_LEVEL_13,
	ZIO_ZSTD_LEVEL_14,
	ZIO_ZSTD_LEVEL_15,
	ZIO_ZSTD_LEVEL_16,
	ZIO_ZSTD_LEVEL_17,
	ZIO_ZSTD_LEVEL_18,
	ZIO_ZSTD_LEVEL_19,
#define	ZIO_ZSTD_LEVEL_MAX	ZIO_ZSTD_LEVEL_19
	ZIO_ZSTD_LEVEL_RESERVE = 101, /* Leave room for new positive levels */
	ZIO_ZSTD_LEVEL_FAST, /* Fast levels are negative */
	ZIO_ZSTD_LEVEL_FAST_1,
#define	ZIO_ZSTD_LEVEL_FAST_DEFAULT	ZIO_ZSTD_LEVEL_FAST_1
	ZIO_ZSTD_LEVEL_FAST_2,
	ZIO_ZSTD_LEVEL_FAST_3,
	ZIO_ZSTD_LEVEL_FAST_4,
	ZIO_ZSTD_LEVEL_FAST_5,
	ZIO_ZSTD_LEVEL_FAST_6,
	ZIO_ZSTD_LEVEL_FAST_7,
	ZIO_ZSTD_LEVEL_FAST_8,
	ZIO_ZSTD_LEVEL_FAST_9,
	ZIO_ZSTD_LEVEL_FAST_10,
	ZIO_ZSTD_LEVEL_FAST_20,
	ZIO_ZSTD_LEVEL_FAST_30,
	ZIO_ZSTD_LEVEL_FAST_40,
	ZIO_ZSTD_LEVEL_FAST_50,
	ZIO_ZSTD_LEVEL_FAST_60,
	ZIO_ZSTD_LEVEL_FAST_70,
	ZIO_ZSTD_LEVEL_FAST_80,
	ZIO_ZSTD_LEVEL_FAST_90,
	ZIO_ZSTD_LEVEL_FAST_100,
	ZIO_ZSTD_LEVEL_FAST_500,
	ZIO_ZSTD_LEVEL_FAST_1000,
#define	ZIO_ZSTD_LEVEL_FAST_MAX	ZIO_ZSTD_LEVEL_FAST_1000
	ZIO_ZSTD_LEVEL_AUTO = 251, /* Reserved for future use */
	ZIO_ZSTD_LEVEL_LEVELS
};

struct levelmap
{
	grub_int16_t cookie;
	enum zio_zstd_levels level;
};

/* Level map for converting ZFS internal levels to ZSTD levels and vice versa */
static struct levelmap zstd_levels[] =
{
	{ZIO_ZSTD_LEVEL_1, ZIO_ZSTD_LEVEL_1},
	{ZIO_ZSTD_LEVEL_2, ZIO_ZSTD_LEVEL_2},
	{ZIO_ZSTD_LEVEL_3, ZIO_ZSTD_LEVEL_3},
	{ZIO_ZSTD_LEVEL_4, ZIO_ZSTD_LEVEL_4},
	{ZIO_ZSTD_LEVEL_5, ZIO_ZSTD_LEVEL_5},
	{ZIO_ZSTD_LEVEL_6, ZIO_ZSTD_LEVEL_6},
	{ZIO_ZSTD_LEVEL_7, ZIO_ZSTD_LEVEL_7},
	{ZIO_ZSTD_LEVEL_8, ZIO_ZSTD_LEVEL_8},
	{ZIO_ZSTD_LEVEL_9, ZIO_ZSTD_LEVEL_9},
	{ZIO_ZSTD_LEVEL_10, ZIO_ZSTD_LEVEL_10},
	{ZIO_ZSTD_LEVEL_11, ZIO_ZSTD_LEVEL_11},
	{ZIO_ZSTD_LEVEL_12, ZIO_ZSTD_LEVEL_12},
	{ZIO_ZSTD_LEVEL_13, ZIO_ZSTD_LEVEL_13},
	{ZIO_ZSTD_LEVEL_14, ZIO_ZSTD_LEVEL_14},
	{ZIO_ZSTD_LEVEL_15, ZIO_ZSTD_LEVEL_15},
	{ZIO_ZSTD_LEVEL_16, ZIO_ZSTD_LEVEL_16},
	{ZIO_ZSTD_LEVEL_17, ZIO_ZSTD_LEVEL_17},
	{ZIO_ZSTD_LEVEL_18, ZIO_ZSTD_LEVEL_18},
	{ZIO_ZSTD_LEVEL_19, ZIO_ZSTD_LEVEL_19},
	{-1, ZIO_ZSTD_LEVEL_FAST_1},
	{-2, ZIO_ZSTD_LEVEL_FAST_2},
	{-3, ZIO_ZSTD_LEVEL_FAST_3},
	{-4, ZIO_ZSTD_LEVEL_FAST_4},
	{-5, ZIO_ZSTD_LEVEL_FAST_5},
	{-6, ZIO_ZSTD_LEVEL_FAST_6},
	{-7, ZIO_ZSTD_LEVEL_FAST_7},
	{-8, ZIO_ZSTD_LEVEL_FAST_8},
	{-9, ZIO_ZSTD_LEVEL_FAST_9},
	{-10, ZIO_ZSTD_LEVEL_FAST_10},
	{-20, ZIO_ZSTD_LEVEL_FAST_20},
	{-30, ZIO_ZSTD_LEVEL_FAST_30},
	{-40, ZIO_ZSTD_LEVEL_FAST_40},
	{-50, ZIO_ZSTD_LEVEL_FAST_50},
	{-60, ZIO_ZSTD_LEVEL_FAST_60},
	{-70, ZIO_ZSTD_LEVEL_FAST_70},
	{-80, ZIO_ZSTD_LEVEL_FAST_80},
	{-90, ZIO_ZSTD_LEVEL_FAST_90},
	{-100, ZIO_ZSTD_LEVEL_FAST_100},
	{-500, ZIO_ZSTD_LEVEL_FAST_500},
	{-1000, ZIO_ZSTD_LEVEL_FAST_1000},
};


static int
zstd_enum_to_cookie(enum zio_zstd_levels level, grub_int16_t* cookie)
{
	for (unsigned int i = 0; i < ARRAY_SIZE(zstd_levels); i++)
	{
		if (zstd_levels[i].level == level)
		{
			*cookie = zstd_levels[i].cookie;
			return (0);
		}
	}

	/* Invalid/unknown ZSTD level - this should never happen. */
	return (1);
}

static void*
zstd_dctx_alloc(void* opaque __attribute__((unused)), size_t size)
{
	return grub_malloc(size);
}

static void
zstd_free(void* opaque __attribute__((unused)), void* ptr)
{
	grub_free(ptr);
}

/* Decompression memory handler */
static const ZSTD_customMem zstd_dctx_malloc =
{
	zstd_dctx_alloc,
	zstd_free,
	NULL,
};

typedef struct
{
	grub_uint32_t version : 24;
	grub_uint8_t level;
} version_level_t;

/* NOTE: all fields in this header are in big endian order */
GRUB_PACKED_START
struct zstd_header
{
	/* contains compressed size */
	grub_uint32_t size;
	/*
	 * contains the version and level
	 * we have to choose a union here to handle
	 * endian conversation since the version and level
	 * is bitmask encoded.
	 */
	union
	{
		grub_uint32_t version_data;
		version_level_t version_level;
	};
	char data[];
};
GRUB_PACKED_END

grub_err_t
zstd_decompress(void* s_start, void* d_start, grub_size_t s_len,
	grub_size_t d_len);

grub_err_t
zstd_decompress(void* s_start, void* d_start, grub_size_t s_len,
	grub_size_t d_len)
{
	ZSTD_DCtx* dctx;
	size_t result;
	grub_uint32_t bufsize;
	grub_uint8_t zstdlevel;
	grub_uint32_t version;
	grub_int16_t levelcookie;
	const struct zstd_header* hdr = (const struct zstd_header*)s_start;
	struct zstd_header hdr_copy = { 0 };
	hdr_copy.version_data = grub_be_to_cpu32(hdr->version_data);
	/* Read buffer size */
	bufsize = grub_be_to_cpu32(hdr->size);

	/* Read the level */
	zstdlevel = hdr_copy.version_level.level;

	/*
	 * We ignore the ZSTD version for now. As soon as incompatibilities
	 * occurr, it has to be read and handled accordingly.
	 */
	version = hdr_copy.version_level.version;

	/*
	 * Convert and check the level
	 * An invalid level is a strong indicator for data corruption! In such
	 * case return an error so the upper layers can try to fix it.
	 */
	if (zstd_enum_to_cookie(zstdlevel, &levelcookie))
	{
		return grub_error(GRUB_ERR_BAD_FS, "zstd decompression failed");
	}

	if (bufsize + sizeof(*hdr) > s_len)
	{
		return grub_error(GRUB_ERR_BAD_FS, "zstd decompression failed");
	}

	dctx = ZSTD_createDCtx_advanced(zstd_dctx_malloc);
	if (!dctx)
	{
		return grub_error(GRUB_ERR_BAD_FS, "zstd decompression failed");
	}

	/*
	 * special case for supporting older development versions
	 * which did contain the magic
	 */

	if (version >= 10405)
	{
		/* Set header type to "magicless" */
		ZSTD_DCtx_setParameter(dctx, ZSTD_d_format,
			ZSTD_f_zstd1_magicless);
	}

	result = ZSTD_decompressDCtx(dctx, d_start, d_len,
		hdr->data, bufsize);

	ZSTD_freeDCtx(dctx);

	/*
	 * Returns 0 on success (decompression function returned non-negative)
	 * and non-zero on failure (decompression function returned negative.
	 */
	if (ZSTD_isError(result))
	{
		return grub_error(GRUB_ERR_BAD_FS, "zstd decompression failed");
	}

	return GRUB_ERR_NONE;
}
