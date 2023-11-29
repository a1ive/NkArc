/* $Id: VHD.cpp $ */
/** @file
 * VHD Disk image, Core Code.
 */

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
  * SPDX-License-Identifier: GPL-3.0-only
  */

#include <grub/types.h>
#include <grub/misc.h>
#include <grub/file.h>
#include <grub/mm.h>

#include "../lib/vbox/vbox.h"

GRUB_MOD_LICENSE("GPLv3+");

/** No flags. */
#define VD_IMAGE_FLAGS_NONE                     (0)
/** Fixed image. */
#define VD_IMAGE_FLAGS_FIXED                    (0x10000)
/** Diff image. Mutually exclusive with fixed image. */
#define VD_IMAGE_FLAGS_DIFF                     (0x20000)

#define VHD_RELATIVE_MAX_PATH 512
#define VHD_ABSOLUTE_MAX_PATH 512

#define VHD_SECTOR_SIZE 512
#define VHD_BLOCK_SIZE  (2 * _1M)

/** The maximum VHD size is 2TB due to the 32bit sector numbers in the BAT.
 * Note that this is the maximum file size including all footers and headers
 * and not the maximum virtual disk size presented to the guest.
 */
#define VHD_MAX_SIZE    (2 * _1T)
 /** Maximum number of 512 byte sectors for a VHD image. */
#define VHD_MAX_SECTORS (VHD_MAX_SIZE / VHD_SECTOR_SIZE)

/* This is common to all VHD disk types and is located at the end of the image */
#pragma pack(1)
typedef struct VHDFooter
{
	char          Cookie[8];
	grub_uint32_t Features;
	grub_uint32_t Version;
	grub_uint64_t DataOffset;
	grub_uint32_t Timestamp;
	grub_uint8_t  CreatorApp[4];
	grub_uint32_t CreatorVer;
	grub_uint32_t CreatorOS;
	grub_uint64_t OrigSize;
	grub_uint64_t CurSize;
	grub_uint16_t DiskGeometryCylinder;
	grub_uint8_t  DiskGeometryHeads;
	grub_uint8_t  DiskGeometrySectors;
	grub_uint32_t DiskType;
	grub_uint32_t Checksum;
	char          UniqueID[16];
	grub_uint8_t  SavedState;
	grub_uint8_t  Reserved[427];
} VHDFooter;
#pragma pack()

/* this really is spelled with only one n */
#define VHD_FOOTER_COOKIE "conectix"
#define VHD_FOOTER_COOKIE_SIZE 8

#define VHD_FOOTER_DISK_TYPE_FIXED        2
#define VHD_FOOTER_DISK_TYPE_DYNAMIC      3
#define VHD_FOOTER_DISK_TYPE_DIFFERENCING 4

#define VHD_MAX_LOCATOR_ENTRIES           8

/* Header for expanding disk images. */
#pragma pack(1)
typedef struct VHDParentLocatorEntry
{
	grub_uint32_t u32Code;
	grub_uint32_t u32DataSpace;
	grub_uint32_t u32DataLength;
	grub_uint32_t u32Reserved;
	grub_uint64_t u64DataOffset;
} VHDPLE, * PVHDPLE;

typedef struct VHDDynamicDiskHeader
{
	char          Cookie[8];
	grub_uint64_t DataOffset;
	grub_uint64_t TableOffset;
	grub_uint32_t HeaderVersion;
	grub_uint32_t MaxTableEntries;
	grub_uint32_t BlockSize;
	grub_uint32_t Checksum;
	grub_uint8_t  ParentUuid[16];
	grub_uint32_t ParentTimestamp;
	grub_uint32_t Reserved0;
	grub_uint16_t ParentUnicodeName[256];
	VHDPLE        ParentLocatorEntry[VHD_MAX_LOCATOR_ENTRIES];
	grub_uint8_t  Reserved1[256];
} VHDDynamicDiskHeader;
#pragma pack()

#define VHD_DYNAMIC_DISK_HEADER_COOKIE "cxsparse"
#define VHD_DYNAMIC_DISK_HEADER_COOKIE_SIZE 8

/**
 * Complete VHD image data structure.
 */
typedef struct VHDIMAGE
{
	/** Descriptor file if applicable. */
	grub_file_t File;
	/** File size on the host disk (including all headers). */
	grub_uint64_t   FileSize;

	/** Open flags passed by VBoxHDD layer. */
	unsigned        uOpenFlags;
	/** Image flags defined during creation or determined during open. */
	unsigned        uImageFlags;
	/** Total size of the image. */
	grub_uint64_t        cbSize;

	/** Image UUID. */
	RTUUID          ImageUuid;
	/** Parent image UUID. */
	RTUUID          ParentUuid;

	/** Parent's time stamp at the time of image creation. */
	grub_uint32_t        u32ParentTimestamp;
	/** Relative path to the parent image. */
	char* pszParentFilename;

	/** The Block Allocation Table. */
	grub_uint32_t* pBlockAllocationTable;
	/** Number of entries in the table. */
	grub_uint32_t        cBlockAllocationTableEntries;

	/** Size of one data block. */
	grub_uint32_t        cbDataBlock;
	/** Sectors per data block. */
	grub_uint32_t        cSectorsPerDataBlock;
	/** Length of the sector bitmap in bytes. */
	grub_uint32_t        cbDataBlockBitmap;
	/** A copy of the disk footer. */
	VHDFooter       vhdFooterCopy;
	/** Current end offset of the file (without the disk footer). */
	grub_uint64_t        uCurrentEndOfFile;
	/** Size of the data block bitmap in sectors. */
	grub_uint32_t        cDataBlockBitmapSectors;
	/** Start of the block allocation table. */
	grub_uint64_t        uBlockAllocationTableOffset;
	/** Buffer to hold block's bitmap for bit search operations. */
	grub_uint8_t* pu8Bitmap;
	/** Offset to the next data structure (dynamic disk header). */
	grub_uint64_t        u64DataOffset;
} VHDIMAGE, * PVHDIMAGE;

static int vhdFileReadSync(PVHDIMAGE pImage, grub_uint64_t off, void* pvBuf, grub_size_t cbRead, grub_ssize_t* pcbRead)
{
	int rc = GRUB_ERR_NONE;
	grub_file_seek(pImage->File, off);
	grub_ssize_t bytesRead = grub_file_read(pImage->File, pvBuf, cbRead);
	if (pcbRead)
		*pcbRead = bytesRead;
	return rc;
}

static int
vhdFreeImage(PVHDIMAGE pImage)
{
	int rc = GRUB_ERR_NONE;

	/* Freeing a never allocated image (e.g. because the open failed) is
	 * not signalled as an error. After all nothing bad happens. */
	if (pImage)
	{
		if (pImage->pszParentFilename)
		{
			grub_free(pImage->pszParentFilename);
			pImage->pszParentFilename = NULL;
		}
		if (pImage->pBlockAllocationTable)
		{
			grub_free(pImage->pBlockAllocationTable);
			pImage->pBlockAllocationTable = NULL;
		}
		if (pImage->pu8Bitmap)
		{
			grub_free(pImage->pu8Bitmap);
			pImage->pu8Bitmap = NULL;
		}
	}

	return rc;
}

/**
 * Internal: Allocates the block bitmap rounding up to the next 32bit or 64bit boundary.
 *           Can be freed with grub_free. The memory is zeroed.
 */
static grub_uint8_t *
vhdBlockBitmapAllocate(PVHDIMAGE pImage)
{
	return (grub_uint8_t*)grub_zalloc(pImage->cbDataBlockBitmap + sizeof(void*));
}

static int
vhdLoadDynamicDisk(PVHDIMAGE pImage, grub_uint64_t uDynamicDiskHeaderOffset)
{
	VHDDynamicDiskHeader vhdDynamicDiskHeader;
	int rc = GRUB_ERR_NONE;
	grub_uint32_t* pBlockAllocationTable;
	grub_uint64_t uBlockAllocationTableOffset;
	unsigned i = 0;

	/*
	 * Read the dynamic disk header.
	 */
	rc = vhdFileReadSync(pImage, uDynamicDiskHeaderOffset,
		&vhdDynamicDiskHeader, sizeof(VHDDynamicDiskHeader), NULL);
	if (grub_memcmp(vhdDynamicDiskHeader.Cookie, VHD_DYNAMIC_DISK_HEADER_COOKIE, VHD_DYNAMIC_DISK_HEADER_COOKIE_SIZE))
		return GRUB_ERR_BAD_ARGUMENT;

	pImage->cbDataBlock = RT_BE2H_U32(vhdDynamicDiskHeader.BlockSize);
	
	pImage->cBlockAllocationTableEntries = RT_BE2H_U32(vhdDynamicDiskHeader.MaxTableEntries);

	/*
	 * Bail out if the number of BAT entries exceeds the number of sectors for a maximum image.
	 * Lower the number of sectors in the BAT as a few sectors are already occupied by the footers
	 * and headers.
	 */
	if (pImage->cBlockAllocationTableEntries > (VHD_MAX_SECTORS - 2))
		return GRUB_ERR_BAD_DEVICE;

	pImage->cSectorsPerDataBlock = pImage->cbDataBlock / VHD_SECTOR_SIZE;
	

	/*
	 * Every block starts with a bitmap indicating which sectors are valid and which are not.
	 * We store the size of it to be able to calculate the real offset.
	 */
	pImage->cbDataBlockBitmap = pImage->cSectorsPerDataBlock / 8;
	pImage->cDataBlockBitmapSectors = pImage->cbDataBlockBitmap / VHD_SECTOR_SIZE;
	/* Round up to full sector size */
	if (pImage->cbDataBlockBitmap % VHD_SECTOR_SIZE > 0)
		pImage->cDataBlockBitmapSectors++;
	
	

	pImage->pu8Bitmap = vhdBlockBitmapAllocate(pImage);
	if (!pImage->pu8Bitmap)
		return GRUB_ERR_OUT_OF_MEMORY;

	pBlockAllocationTable = (grub_uint32_t*)grub_zalloc(pImage->cBlockAllocationTableEntries * sizeof(grub_uint32_t));
	if (!pBlockAllocationTable)
		return GRUB_ERR_OUT_OF_MEMORY;

	/*
	 * Read the table.
	 */
	uBlockAllocationTableOffset = RT_BE2H_U64(vhdDynamicDiskHeader.TableOffset);
	
	pImage->uBlockAllocationTableOffset = uBlockAllocationTableOffset;
	rc = vhdFileReadSync(pImage,
		uBlockAllocationTableOffset, pBlockAllocationTable,
		pImage->cBlockAllocationTableEntries * sizeof(grub_uint32_t), NULL);
	if (RT_FAILURE(rc))
	{
		grub_free(pBlockAllocationTable);
		return rc;
	}

	/*
	 * Because the offset entries inside the allocation table are stored big endian
	 * we need to convert them into host endian.
	 */
	pImage->pBlockAllocationTable = (grub_uint32_t*)grub_zalloc(pImage->cBlockAllocationTableEntries * sizeof(grub_uint32_t));
	if (!pImage->pBlockAllocationTable)
	{
		grub_free(pBlockAllocationTable);
		return GRUB_ERR_OUT_OF_MEMORY;
	}

	for (i = 0; i < pImage->cBlockAllocationTableEntries; i++)
		pImage->pBlockAllocationTable[i] = RT_BE2H_U32(pBlockAllocationTable[i]);

	grub_free(pBlockAllocationTable);

	if (pImage->uImageFlags & VD_IMAGE_FLAGS_DIFF)
		grub_memcpy(pImage->ParentUuid.au8, vhdDynamicDiskHeader.ParentUuid, sizeof(pImage->ParentUuid));

	return rc;
}

static int
vhdOpenImage(PVHDIMAGE pImage)
{
	VHDFooter vhdFooter;
	int rc;

	pImage->FileSize = grub_file_size(pImage->File);
	pImage->uCurrentEndOfFile = pImage->FileSize - sizeof(VHDFooter);

	rc = vhdFileReadSync(pImage, pImage->uCurrentEndOfFile, &vhdFooter, sizeof(VHDFooter), NULL);
	if (RT_SUCCESS(rc))
	{
		if (grub_memcmp(vhdFooter.Cookie, VHD_FOOTER_COOKIE, VHD_FOOTER_COOKIE_SIZE) != 0)
		{
			/*
			* There is also a backup header at the beginning in case the image got corrupted.
			* Such corrupted images are detected here to let the open handler repair it later.
			*/
			rc = vhdFileReadSync(pImage, 0, &vhdFooter, sizeof(VHDFooter), NULL);
			if (RT_SUCCESS(rc))
			{
				if (grub_memcmp(vhdFooter.Cookie, VHD_FOOTER_COOKIE, VHD_FOOTER_COOKIE_SIZE) != 0)
					rc = GRUB_ERR_BAD_DEVICE;
			}
		}
	}

	if (RT_FAILURE(rc))
	{
		vhdFreeImage(pImage);
		return rc;
	}

	switch (RT_BE2H_U32(vhdFooter.DiskType))
	{
	case VHD_FOOTER_DISK_TYPE_FIXED:
		pImage->uImageFlags |= VD_IMAGE_FLAGS_FIXED;
		break;
	case VHD_FOOTER_DISK_TYPE_DYNAMIC:
		pImage->uImageFlags &= ~VD_IMAGE_FLAGS_FIXED;
		break;
	case VHD_FOOTER_DISK_TYPE_DIFFERENCING:
		pImage->uImageFlags |= VD_IMAGE_FLAGS_DIFF;
		pImage->uImageFlags &= ~VD_IMAGE_FLAGS_FIXED;
		break;
	default:
		vhdFreeImage(pImage);
		return GRUB_ERR_NOT_IMPLEMENTED_YET;
	}

	pImage->cbSize = RT_BE2H_U64(vhdFooter.CurSize);

	/*
	 * Copy of the disk footer.
	 * If we allocate new blocks in differencing disks on write access
	 * the footer is overwritten. We need to write it at the end of the file.
	 */
	grub_memcpy(&pImage->vhdFooterCopy, &vhdFooter, sizeof(VHDFooter));

	/*
	 * Is there a better way?
	 */
	grub_memcpy(&pImage->ImageUuid, &vhdFooter.UniqueID, 16);

	pImage->u64DataOffset = RT_BE2H_U64(vhdFooter.DataOffset);
	

	if (!(pImage->uImageFlags & VD_IMAGE_FLAGS_FIXED))
		rc = vhdLoadDynamicDisk(pImage, pImage->u64DataOffset);

	if (RT_FAILURE(rc))
		vhdFreeImage(pImage);
	return rc;
}

/**
 * Internal: Checks if a sector in the block bitmap is set
 */
static int
vhdBlockBitmapSectorContainsData(PVHDIMAGE pImage, grub_uint32_t cBlockBitmapEntry)
{
	grub_uint32_t iBitmap = (cBlockBitmapEntry / 8); /* Byte in the block bitmap. */

	/*
	 * The index of the bit in the byte of the data block bitmap.
	 * The most significant bit stands for a lower sector number.
	 */
	grub_uint8_t  iBitInByte = (8 - 1) - (cBlockBitmapEntry % 8);
	grub_uint8_t* puBitmap = pImage->pu8Bitmap + iBitmap;

	return ((*puBitmap) & RT_BIT(iBitInByte)) != 0;
}

static int
vhdOpen(grub_file_t File, void** ppBackendData)
{
	int rc = GRUB_ERR_NONE;

	PVHDIMAGE pImage = (PVHDIMAGE)grub_zalloc(sizeof(VHDIMAGE));
	if (!pImage)
	{
		rc = GRUB_ERR_OUT_OF_MEMORY;
		return rc;
	}

	pImage->File = File;
	rc = vhdOpenImage(pImage);

	if (RT_SUCCESS(rc))
		*ppBackendData = pImage;
	else
		grub_free(pImage);

	return rc;
}

static int
vhdClose(void* pBackendData)
{
	PVHDIMAGE pImage = (PVHDIMAGE)pBackendData;

	int rc = vhdFreeImage(pImage);
	grub_free(pImage);

	return rc;
}

static int
vhdRead(void* pBackendData, grub_uint64_t uOffset, void* pvBuf, grub_size_t cbToRead,
	grub_size_t* pcbActuallyRead)
{
	PVHDIMAGE pImage = (PVHDIMAGE)pBackendData;
	int rc = GRUB_ERR_NONE;

	if (uOffset + cbToRead > pImage->cbSize)
        return GRUB_ERR_BAD_ARGUMENT;

	/*
	 * If we have a dynamic disk image, we need to find the data block and sector to read.
	 */
	if (pImage->pBlockAllocationTable)
	{
		/*
		 * Get the data block first.
		 */
		grub_uint32_t cBlockAllocationTableEntry = (uOffset / VHD_SECTOR_SIZE) / pImage->cSectorsPerDataBlock;
		grub_uint32_t cBATEntryIndex = (uOffset / VHD_SECTOR_SIZE) % pImage->cSectorsPerDataBlock;
		grub_uint64_t uVhdOffset;

		/*
		 * Clip read range to remain in this data block.
		 */
		cbToRead = RT_MIN(cbToRead, (pImage->cbDataBlock - (cBATEntryIndex * VHD_SECTOR_SIZE)));

		/*
		 * If the block is not allocated the content of the entry is ~0
		 */
		if (pImage->pBlockAllocationTable[cBlockAllocationTableEntry] == ~0U)
			rc = GRUB_ERR_NONE;
		else
		{
			uVhdOffset = ((grub_uint64_t)pImage->pBlockAllocationTable[cBlockAllocationTableEntry] + pImage->cDataBlockBitmapSectors + cBATEntryIndex) * VHD_SECTOR_SIZE;

			/* Clip read range to remain in this data block. */
			cbToRead = RT_MIN(cbToRead, (pImage->cbDataBlock - (cBATEntryIndex * VHD_SECTOR_SIZE)));

			/* Read in the block's bitmap. */
			rc = vhdFileReadSync(pImage,
				((grub_uint64_t)pImage->pBlockAllocationTable[cBlockAllocationTableEntry]) * VHD_SECTOR_SIZE,
				pImage->pu8Bitmap, pImage->cbDataBlockBitmap, NULL);

			if (RT_SUCCESS(rc))
			{
				grub_uint32_t cSectors = 0;

				if (vhdBlockBitmapSectorContainsData(pImage, cBATEntryIndex))
				{
					cBATEntryIndex++;
					cSectors = 1;

					/*
					 * The first sector being read is marked dirty, read as much as we
					 * can from child. Note that only sectors that are marked dirty
					 * must be read from child.
					 */
					while ((cSectors < (cbToRead / VHD_SECTOR_SIZE))
						&& vhdBlockBitmapSectorContainsData(pImage, cBATEntryIndex))
					{
						cBATEntryIndex++;
						cSectors++;
					}

					cbToRead = (grub_size_t) cSectors * VHD_SECTOR_SIZE;

					rc = vhdFileReadSync(pImage, uVhdOffset, pvBuf, cbToRead, NULL);
				}
				else
				{
					/*
					 * The first sector being read is marked clean, so we should read from
					 * our parent instead, but only as much as there are the following
					 * clean sectors, because the block may still contain dirty sectors
					 * further on. We just need to compute the number of clean sectors
					 * and pass it to our caller along with the notification that they
					 * should be read from the parent.
					 */
					cBATEntryIndex++;
					cSectors = 1;

					while ((cSectors < (cbToRead / VHD_SECTOR_SIZE))
						&& !vhdBlockBitmapSectorContainsData(pImage, cBATEntryIndex))
					{
						cBATEntryIndex++;
						cSectors++;
					}

					cbToRead = (grub_size_t) cSectors * VHD_SECTOR_SIZE;
					
					rc = GRUB_ERR_NONE;
				}
			}
		}
	}
	else
		rc = vhdFileReadSync(pImage, uOffset, pvBuf, cbToRead, NULL);

	if (pcbActuallyRead)
		*pcbActuallyRead = cbToRead;
	
	return rc;
}

static grub_uint64_t
vhdGetSize(void* pBackendData)
{
	PVHDIMAGE pImage = (PVHDIMAGE)pBackendData;

	if (pImage)
		return pImage->cbSize;
	return 0;
}

struct grub_vhd
{
	grub_file_t file;
	void* vhd;
};
typedef struct grub_vhd* grub_vhd_t;

static struct grub_fs grub_vhd_fs;

static grub_err_t
grub_vhd_close(grub_file_t file)
{
	grub_vhd_t vhdio = file->data;

	vhdClose(vhdio->vhd);
	grub_file_close(vhdio->file);
	grub_free(vhdio);
	file->disk = 0;
	file->name = 0;
	return grub_errno;
}

static grub_file_t
grub_vhd_open(grub_file_t io, enum grub_file_type type)
{
	grub_file_t file;
	grub_vhd_t vhdio;
	void* vhd = NULL;

	if (!(type & GRUB_FILE_TYPE_FILTER_VDISK))
		return io;
	if (io->size < 0x10000)
		return io;

	if (vhdOpen(io, &vhd) != GRUB_ERR_NONE)
	{
		grub_file_seek(io, 0);
		grub_errno = GRUB_ERR_NONE;
		return io;
	}

	file = (grub_file_t)grub_zalloc(sizeof(*file));
	if (!file)
	{
		vhdClose(vhd);
		return 0;
	}

	vhdio = grub_zalloc(sizeof(*vhdio));
	if (!vhdio)
	{
		vhdClose(vhd);
		grub_free(file);
		return 0;
	}
	vhdio->file = io;
	vhdio->vhd = vhd;

	file->disk = io->disk;
	file->data = vhdio;
	file->fs = &grub_vhd_fs;
	file->size = GRUB_FILE_SIZE_UNKNOWN;
	file->not_easily_seekable = io->not_easily_seekable;

	file->size = vhdGetSize(vhd);

	return file;
}

static grub_ssize_t
grub_vhd_read(grub_file_t file, char* buf, grub_size_t len)
{
	int rc = GRUB_ERR_NONE;
	grub_size_t real_size;
	grub_ssize_t size = 0;
	grub_vhd_t vhdio = file->data;

	while (rc == GRUB_ERR_NONE)
	{
		rc = vhdRead(vhdio->vhd, file->offset, buf, len, &real_size);
		file->offset += real_size;
		buf += real_size;
		size += real_size;
		if (real_size >= len)
			break;
		len -= real_size;
	}

	if (rc != GRUB_ERR_NONE)
		return -1;
	return size;
}

static struct grub_fs grub_vhd_fs =
{
	.name = "vhd",
	.fs_dir = 0,
	.fs_open = 0,
	.fs_read = grub_vhd_read,
	.fs_close = grub_vhd_close,
	.fs_label = 0,
	.next = 0
};

GRUB_MOD_INIT(vhd)
{
	grub_file_filter_register(GRUB_FILE_FILTER_VHD, grub_vhd_open);
}

GRUB_MOD_FINI(vhd)
{
	grub_file_filter_unregister(GRUB_FILE_FILTER_VHD);
}
