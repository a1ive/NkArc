/* $Id: VHDX.cpp $ */
/** @file
 * VHDX - VHDX Disk image, Core Code.
 */

 /*
  * Copyright (C) 2012-2023 Oracle and/or its affiliates.
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

  /*********************************************************************************************************************************
  *   On disk data structures                                                                                                      *
  *********************************************************************************************************************************/

  /**
   * VHDX file type identifier.
   */
#pragma pack(1)
typedef struct VhdxFileIdentifier
{
	/** Signature. */
	grub_uint64_t    u64Signature;
	/** Creator ID - UTF-16 string (not neccessarily null terminated). */
	grub_uint16_t    awszCreator[256];
} VhdxFileIdentifier;
#pragma pack()
/** Pointer to an on disk VHDX file type identifier. */
typedef VhdxFileIdentifier* PVhdxFileIdentifier;

/** VHDX file type identifier signature ("vhdxfile"). */
#define VHDX_FILE_IDENTIFIER_SIGNATURE 0x656c696678646876ULL
/** Start offset of the VHDX file type identifier. */
#define VHDX_FILE_IDENTIFIER_OFFSET    0

/**
 * VHDX header.
 */
#pragma pack(1)
typedef struct VhdxHeader
{
	/** Signature. */
	grub_uint32_t    u32Signature;
	/** Checksum. */
	grub_uint32_t    u32Checksum;
	/** Sequence number. */
	grub_uint64_t    u64SequenceNumber;
	/** File write UUID. */
	RTUUID      UuidFileWrite;
	/** Data write UUID. */
	RTUUID      UuidDataWrite;
	/** Log UUID. */
	RTUUID      UuidLog;
	/** Version of the log format. */
	grub_uint16_t    u16LogVersion;
	/** VHDX format version. */
	grub_uint16_t    u16Version;
	/** Length of the log region. */
	grub_uint32_t    u32LogLength;
	/** Start offset of the log offset in the file. */
	grub_uint64_t    u64LogOffset;
	/** Reserved bytes. */
	grub_uint8_t     u8Reserved[4016];
} VhdxHeader;
#pragma pack()
/** Pointer to an on disk VHDX header. */
typedef VhdxHeader* PVhdxHeader;

/** VHDX header signature ("head"). */
#define VHDX_HEADER_SIGNATURE    0x64616568U
/** Start offset of the first VHDX header. */
#define VHDX_HEADER1_OFFSET      _64K
/** Start offset of the second VHDX header. */
#define VHDX_HEADER2_OFFSET      _128K
/** Current Log format version. */
#define VHDX_HEADER_LOG_VERSION  0U
/** Current VHDX format version. */
#define VHDX_HEADER_VHDX_VERSION 1U

/**
 * VHDX region table header
 */
#pragma pack(1)
typedef struct VhdxRegionTblHdr
{
	/** Signature. */
	grub_uint32_t    u32Signature;
	/** Checksum. */
	grub_uint32_t    u32Checksum;
	/** Number of region table entries following this header. */
	grub_uint32_t    u32EntryCount;
	/** Reserved. */
	grub_uint32_t    u32Reserved;
} VhdxRegionTblHdr;
#pragma pack()
/** Pointer to an on disk VHDX region table header. */
typedef VhdxRegionTblHdr* PVhdxRegionTblHdr;

/** VHDX region table header signature. */
#define VHDX_REGION_TBL_HDR_SIGNATURE       0x69676572U
/** Maximum number of entries which can follow. */
#define VHDX_REGION_TBL_HDR_ENTRY_COUNT_MAX 2047U
/** Offset where the region table is stored (192 KB). */
#define VHDX_REGION_TBL_HDR_OFFSET          196608ULL
/** Maximum size of the region table. */
#define VHDX_REGION_TBL_SIZE_MAX            _64K

/**
 * VHDX region table entry.
 */
#pragma pack(1)
typedef struct VhdxRegionTblEntry
{
	/** Object UUID. */
	RTUUID      UuidObject;
	/** File offset of the region. */
	grub_uint64_t    u64FileOffset;
	/** Length of the region in bytes. */
	grub_uint32_t    u32Length;
	/** Flags for this object. */
	grub_uint32_t    u32Flags;
} VhdxRegionTblEntry;
#pragma pack()
/** Pointer to an on disk VHDX region table entry. */
typedef struct VhdxRegionTblEntry* PVhdxRegionTblEntry;

/** Flag whether this region is required. */
#define VHDX_REGION_TBL_ENTRY_FLAGS_IS_REQUIRED RT_BIT_32(0)
/** UUID for the BAT region. */
#define VHDX_REGION_TBL_ENTRY_UUID_BAT          "2dc27766-f623-4200-9d64-115e9bfd4a08"
/** UUID for the metadata region. */
#define VHDX_REGION_TBL_ENTRY_UUID_METADATA     "8b7ca206-4790-4b9a-b8fe-575f050f886e"

/**
 * VHDX Log entry header.
 */
#pragma pack(1)
typedef struct VhdxLogEntryHdr
{
	/** Signature. */
	grub_uint32_t    u32Signature;
	/** Checksum. */
	grub_uint32_t    u32Checksum;
	/** Total length of the entry in bytes. */
	grub_uint32_t    u32EntryLength;
	/** Tail of the log entries. */
	grub_uint32_t    u32Tail;
	/** Sequence number. */
	grub_uint64_t    u64SequenceNumber;
	/** Number of descriptors in this log entry. */
	grub_uint32_t    u32DescriptorCount;
	/** Reserved. */
	grub_uint32_t    u32Reserved;
	/** Log UUID. */
	RTUUID      UuidLog;
	/** VHDX file size in bytes while the log entry was written. */
	grub_uint64_t    u64FlushedFileOffset;
	/** File size in bytes all allocated file structures fit into when the
	 * log entry was written. */
	grub_uint64_t    u64LastFileOffset;
} VhdxLogEntryHdr;
#pragma pack()
/** Pointer to an on disk VHDX log entry header. */
typedef struct VhdxLogEntryHdr* PVhdxLogEntryHdr;

/** VHDX log entry signature ("loge"). */
#define VHDX_LOG_ENTRY_HEADER_SIGNATURE 0x65676f6cU

/**
 * VHDX log zero descriptor.
 */
#pragma pack(1)
typedef struct VhdxLogZeroDesc
{
	/** Signature of this descriptor. */
	grub_uint32_t    u32ZeroSignature;
	/** Reserved. */
	grub_uint32_t    u32Reserved;
	/** Length of the section to zero. */
	grub_uint64_t    u64ZeroLength;
	/** File offset to write zeros to. */
	grub_uint64_t    u64FileOffset;
	/** Sequence number (must macht the field in the log entry header). */
	grub_uint64_t    u64SequenceNumber;
} VhdxLogZeroDesc;
#pragma pack()
/** Pointer to an on disk VHDX log zero descriptor. */
typedef struct VhdxLogZeroDesc* PVhdxLogZeroDesc;

/** Signature of a VHDX log zero descriptor ("zero"). */
#define VHDX_LOG_ZERO_DESC_SIGNATURE 0x6f72657aU

/**
 * VHDX log data descriptor.
 */
#pragma pack(1)
typedef struct VhdxLogDataDesc
{
	/** Signature of this descriptor. */
	grub_uint32_t    u32DataSignature;
	/** Trailing 4 bytes removed from the update. */
	grub_uint32_t    u32TrailingBytes;
	/** Leading 8 bytes removed from the update. */
	grub_uint64_t    u64LeadingBytes;
	/** File offset to write zeros to. */
	grub_uint64_t    u64FileOffset;
	/** Sequence number (must macht the field in the log entry header). */
	grub_uint64_t    u64SequenceNumber;
} VhdxLogDataDesc;
#pragma pack()
/** Pointer to an on disk VHDX log data descriptor. */
typedef struct VhdxLogDataDesc* PVhdxLogDataDesc;

/** Signature of a VHDX log data descriptor ("desc"). */
#define VHDX_LOG_DATA_DESC_SIGNATURE 0x63736564U

/**
 * VHDX log data sector.
 */
#pragma pack(1)
typedef struct VhdxLogDataSector
{
	/** Signature of the data sector. */
	grub_uint32_t    u32DataSignature;
	/** 4 most significant bytes of the sequence number. */
	grub_uint32_t    u32SequenceHigh;
	/** Raw data associated with the update. */
	grub_uint8_t     u8Data[4084];
	/** 4 least significant bytes of the sequence number. */
	grub_uint32_t    u32SequenceLow;
} VhdxLogDataSector;
#pragma pack()
/** Pointer to an on disk VHDX log data sector. */
typedef VhdxLogDataSector* PVhdxLogDataSector;

/** Signature of a VHDX log data sector ("data"). */
#define VHDX_LOG_DATA_SECTOR_SIGNATURE 0x61746164U

/**
 * VHDX BAT entry.
 */
#pragma pack(1)
typedef struct VhdxBatEntry
{
	/** The BAT entry, contains state and offset. */
	grub_uint64_t    u64BatEntry;
} VhdxBatEntry;
#pragma pack()
typedef VhdxBatEntry* PVhdxBatEntry;

/** Return the BAT state from a given entry. */
#define VHDX_BAT_ENTRY_GET_STATE(bat) ((bat) & 0x7ULL)
/** Get the FileOffsetMB field from a given BAT entry. */
#define VHDX_BAT_ENTRY_GET_FILE_OFFSET_MB(bat) (((bat) & 0xfffffffffff00000ULL) >> 20)
/** Get a byte offset from the BAT entry. */
#define VHDX_BAT_ENTRY_GET_FILE_OFFSET(bat) (VHDX_BAT_ENTRY_GET_FILE_OFFSET_MB(bat) * (grub_uint64_t)_1M)

/** Block not present and the data is undefined. */
#define VHDX_BAT_ENTRY_PAYLOAD_BLOCK_NOT_PRESENT       (0)
/** Data in this block is undefined. */
#define VHDX_BAT_ENTRY_PAYLOAD_BLOCK_UNDEFINED         (1)
/** Data in this block contains zeros. */
#define VHDX_BAT_ENTRY_PAYLOAD_BLOCK_ZERO              (2)
/** Block was unmapped by the application or system and data is either zero or
 * the data before the block was unmapped. */
#define VHDX_BAT_ENTRY_PAYLOAD_BLOCK_UNMAPPED          (3)
 /** Block data is in the file pointed to by the FileOffsetMB field. */
#define VHDX_BAT_ENTRY_PAYLOAD_BLOCK_FULLY_PRESENT     (6)
/** Block is partially present, use sector bitmap to get present sectors. */
#define VHDX_BAT_ENTRY_PAYLOAD_BLOCK_PARTIALLY_PRESENT (7)

/** The sector bitmap block is undefined and not allocated in the file. */
#define VHDX_BAT_ENTRY_SB_BLOCK_NOT_PRESENT            (0)
/** The sector bitmap block is defined at the file location. */
#define VHDX_BAT_ENTRY_SB_BLOCK_PRESENT                (6)

/**
 * VHDX Metadata tabl header.
 */
#pragma pack(1)
typedef struct VhdxMetadataTblHdr
{
	/** Signature. */
	grub_uint64_t    u64Signature;
	/** Reserved. */
	grub_uint16_t    u16Reserved;
	/** Number of entries in the table. */
	grub_uint16_t    u16EntryCount;
	/** Reserved */
	grub_uint32_t    u32Reserved2[5];
} VhdxMetadataTblHdr;
#pragma pack()
/** Pointer to an on disk metadata table header. */
typedef VhdxMetadataTblHdr* PVhdxMetadataTblHdr;

/** Signature of a VHDX metadata table header ("metadata"). */
#define VHDX_METADATA_TBL_HDR_SIGNATURE       0x617461646174656dULL
/** Maximum number of entries the metadata table can have. */
#define VHDX_METADATA_TBL_HDR_ENTRY_COUNT_MAX 2047U

/**
 * VHDX Metadata table entry.
 */
#pragma pack(1)
typedef struct VhdxMetadataTblEntry
{
	/** Item UUID. */
	RTUUID      UuidItem;
	/** Offset of the metadata item. */
	grub_uint32_t    u32Offset;
	/** Length of the metadata item. */
	grub_uint32_t    u32Length;
	/** Flags for the metadata item. */
	grub_uint32_t    u32Flags;
	/** Reserved. */
	grub_uint32_t    u32Reserved;
} VhdxMetadataTblEntry;
#pragma pack()
/** Pointer to an on disk metadata table entry. */
typedef VhdxMetadataTblEntry* PVhdxMetadataTblEntry;

/** FLag whether the metadata item is system or user metadata. */
#define VHDX_METADATA_TBL_ENTRY_FLAGS_IS_USER     RT_BIT_32(0)
/** FLag whether the metadata item is file or virtual disk metadata. */
#define VHDX_METADATA_TBL_ENTRY_FLAGS_IS_VDISK    RT_BIT_32(1)
/** FLag whether the backend must understand the metadata item to load the image. */
#define VHDX_METADATA_TBL_ENTRY_FLAGS_IS_REQUIRED RT_BIT_32(2)

/** File parameters item UUID. */
#define VHDX_METADATA_TBL_ENTRY_ITEM_FILE_PARAMS    "caa16737-fa36-4d43-b3b6-33f0aa44e76b"
/** Virtual disk size item UUID. */
#define VHDX_METADATA_TBL_ENTRY_ITEM_VDISK_SIZE     "2fa54224-cd1b-4876-b211-5dbed83bf4b8"
/** Page 83 UUID. */
#define VHDX_METADATA_TBL_ENTRY_ITEM_PAGE83_DATA    "beca12ab-b2e6-4523-93ef-c309e000c746"
/** Logical sector size UUID. */
#define VHDX_METADATA_TBL_ENTRY_ITEM_LOG_SECT_SIZE  "8141bf1d-a96f-4709-ba47-f233a8faab5f"
/** Physical sector size UUID. */
#define VHDX_METADATA_TBL_ENTRY_ITEM_PHYS_SECT_SIZE "cda348c7-445d-4471-9cc9-e9885251c556"
/** Parent locator UUID. */
#define VHDX_METADATA_TBL_ENTRY_ITEM_PARENT_LOCATOR "a8d35f2d-b30b-454d-abf7-d3d84834ab0c"

/**
 * VHDX File parameters metadata item.
 */
#pragma pack(1)
typedef struct VhdxFileParameters
{
	/** Block size. */
	grub_uint32_t    u32BlockSize;
	/** Flags. */
	grub_uint32_t    u32Flags;
} VhdxFileParameters;
#pragma pack()
/** Pointer to an on disk VHDX file parameters metadata item. */
typedef struct VhdxFileParameters* PVhdxFileParameters;

/** Flag whether to leave blocks allocated in the file or if it is possible to unmap them. */
#define VHDX_FILE_PARAMETERS_FLAGS_LEAVE_BLOCKS_ALLOCATED RT_BIT_32(0)
/** Flag whether this file has a parent VHDX file. */
#define VHDX_FILE_PARAMETERS_FLAGS_HAS_PARENT             RT_BIT_32(1)

/**
 * VHDX virtual disk size metadata item.
 */
#pragma pack(1)
typedef struct VhdxVDiskSize
{
	/** Virtual disk size. */
	grub_uint64_t    u64VDiskSize;
} VhdxVDiskSize;
#pragma pack()
/** Pointer to an on disk VHDX virtual disk size metadata item. */
typedef struct VhdxVDiskSize* PVhdxVDiskSize;

/**
 * VHDX page 83 data metadata item.
 */
#pragma pack(1)
typedef struct VhdxPage83Data
{
	/** UUID for the SCSI device. */
	RTUUID      UuidPage83Data;
} VhdxPage83Data;
#pragma pack()
/** Pointer to an on disk VHDX vpage 83 data metadata item. */
typedef struct VhdxPage83Data* PVhdxPage83Data;

/**
 * VHDX virtual disk logical sector size.
 */
#pragma pack(1)
typedef struct VhdxVDiskLogicalSectorSize
{
	/** Logical sector size. */
	grub_uint32_t    u32LogicalSectorSize;
} VhdxVDiskLogicalSectorSize;
#pragma pack()
/** Pointer to an on disk VHDX virtual disk logical sector size metadata item. */
typedef struct VhdxVDiskLogicalSectorSize* PVhdxVDiskLogicalSectorSize;

/**
 * VHDX virtual disk physical sector size.
 */
#pragma pack(1)
typedef struct VhdxVDiskPhysicalSectorSize
{
	/** Physical sector size. */
	grub_uint64_t    u64PhysicalSectorSize;
} VhdxVDiskPhysicalSectorSize;
#pragma pack()
/** Pointer to an on disk VHDX virtual disk physical sector size metadata item. */
typedef struct VhdxVDiskPhysicalSectorSize* PVhdxVDiskPhysicalSectorSize;

/**
 * VHDX parent locator header.
 */
#pragma pack(1)
typedef struct VhdxParentLocatorHeader
{
	/** Locator type UUID. */
	RTUUID      UuidLocatorType;
	/** Reserved. */
	grub_uint16_t    u16Reserved;
	/** Number of key value pairs. */
	grub_uint16_t    u16KeyValueCount;
} VhdxParentLocatorHeader;
#pragma pack()
/** Pointer to an on disk VHDX parent locator header metadata item. */
typedef struct VhdxParentLocatorHeader* PVhdxParentLocatorHeader;

/** VHDX parent locator type. */
#define VHDX_PARENT_LOCATOR_TYPE_VHDX "b04aefb7-d19e-4a81-b789-25b8e9445913"

/**
 * VHDX parent locator entry.
 */
#pragma pack(1)
typedef struct VhdxParentLocatorEntry
{
	/** Offset of the key. */
	grub_uint32_t    u32KeyOffset;
	/** Offset of the value. */
	grub_uint32_t    u32ValueOffset;
	/** Length of the key. */
	grub_uint16_t    u16KeyLength;
	/** Length of the value. */
	grub_uint16_t    u16ValueLength;
} VhdxParentLocatorEntry;
#pragma pack()
/** Pointer to an on disk VHDX parent locator entry. */
typedef struct VhdxParentLocatorEntry* PVhdxParentLocatorEntry;


/*********************************************************************************************************************************
*   Constants And Macros, Structures and Typedefs                                                                                *
*********************************************************************************************************************************/

typedef enum VHDXMETADATAITEM
{
	VHDXMETADATAITEM_UNKNOWN = 0,
	VHDXMETADATAITEM_FILE_PARAMS,
	VHDXMETADATAITEM_VDISK_SIZE,
	VHDXMETADATAITEM_PAGE83_DATA,
	VHDXMETADATAITEM_LOGICAL_SECTOR_SIZE,
	VHDXMETADATAITEM_PHYSICAL_SECTOR_SIZE,
	VHDXMETADATAITEM_PARENT_LOCATOR,
	VHDXMETADATAITEM_32BIT_HACK = 0x7fffffff
} VHDXMETADATAITEM;

/**
 * Table to validate the metadata item UUIDs and the flags.
 */
typedef struct VHDXMETADATAITEMPROPS
{
	/** Item UUID. */
	const char* pszItemUuid;
	/** Flag whether this is a user or system metadata item. */
	int                 fIsUser;
	/** Flag whether this is a virtual disk or file metadata item. */
	int                 fIsVDisk;
	/** Flag whether this metadata item is required to load the file. */
	int                 fIsRequired;
	/** Metadata item enum associated with this UUID. */
	VHDXMETADATAITEM     enmMetadataItem;
} VHDXMETADATAITEMPROPS;

/**
 * VHDX image data structure.
 */
typedef struct VHDXIMAGE
{
	/** Descriptor file if applicable. */
	grub_file_t File;
	/** File size on the host disk (including all headers). */
	grub_uint64_t   FileSize;

	/** Open flags passed by VBoxHD layer. */
	unsigned            uOpenFlags;
	/** Image flags defined during creation or determined during open. */
	unsigned            uImageFlags;
	/** Version of the VHDX image format. */
	unsigned            uVersion;
	/** Total size of the image. */
	grub_uint64_t            cbSize;
	/** Logical sector size of the image. */
	grub_uint32_t            cbLogicalSector;
	/** Block size of the image. */
	grub_size_t              cbBlock;

	/** The BAT. */
	PVhdxBatEntry       paBat;
	/** Chunk ratio. */
	grub_uint32_t            uChunkRatio;
} VHDXIMAGE, * PVHDXIMAGE;

/**
 * Endianess conversion direction.
 */
typedef enum VHDXECONV
{
	/** Host to file endianess. */
	VHDXECONV_H2F = 0,
	/** File to host endianess. */
	VHDXECONV_F2H
} VHDXECONV;

/** Macros for endianess conversion. */
#define SET_ENDIAN_U16(u16) (enmConv == VHDXECONV_H2F ? RT_H2LE_U16(u16) : RT_LE2H_U16(u16))
#define SET_ENDIAN_U32(u32) (enmConv == VHDXECONV_H2F ? RT_H2LE_U32(u32) : RT_LE2H_U32(u32))
#define SET_ENDIAN_U64(u64) (enmConv == VHDXECONV_H2F ? RT_H2LE_U64(u64) : RT_LE2H_U64(u64))


/*********************************************************************************************************************************
*   Static Variables                                                                                                             *
*********************************************************************************************************************************/

/**
 * Static table to verify the metadata item properties and the flags.
 */
static const VHDXMETADATAITEMPROPS s_aVhdxMetadataItemProps[] =
{
	/* pcszItemUuid                               fIsUser, fIsVDisk, fIsRequired, enmMetadataItem */
	{VHDX_METADATA_TBL_ENTRY_ITEM_FILE_PARAMS,    0,   0,     1,        VHDXMETADATAITEM_FILE_PARAMS},
	{VHDX_METADATA_TBL_ENTRY_ITEM_VDISK_SIZE,     0,   1,     1,        VHDXMETADATAITEM_VDISK_SIZE},
	{VHDX_METADATA_TBL_ENTRY_ITEM_PAGE83_DATA,    0,   1,     1,        VHDXMETADATAITEM_PAGE83_DATA},
	{VHDX_METADATA_TBL_ENTRY_ITEM_LOG_SECT_SIZE,  0,   1,     1,        VHDXMETADATAITEM_LOGICAL_SECTOR_SIZE},
	{VHDX_METADATA_TBL_ENTRY_ITEM_PHYS_SECT_SIZE, 0,   1,     1,        VHDXMETADATAITEM_PHYSICAL_SECTOR_SIZE},
	{VHDX_METADATA_TBL_ENTRY_ITEM_PARENT_LOCATOR, 0,   0,     1,        VHDXMETADATAITEM_PARENT_LOCATOR}
};

static int
vhdxFileReadSync(PVHDXIMAGE pImage, grub_uint64_t off, void* pvBuf, grub_size_t cbRead, grub_ssize_t* pcbRead)
{
	int rc = GRUB_ERR_NONE;
	grub_file_seek(pImage->File, off);
	grub_ssize_t bytesRead = grub_file_read(pImage->File, pvBuf, cbRead);
	if (pcbRead)
		*pcbRead = bytesRead;
	return rc;
}

/*********************************************************************************************************************************
*   Internal Functions                                                                                                           *
*********************************************************************************************************************************/

/**
 * Converts the file identifier between file and host endianness.
 *
 * @param   enmConv             Direction of the conversion.
 * @param   pFileIdentifierConv Where to store the converted file identifier.
 * @param   pFileIdentifier     The file identifier to convert.
 *
 * @note It is safe to use the same pointer for pFileIdentifierConv and pFileIdentifier.
 */
static void
vhdxConvFileIdentifierEndianess(VHDXECONV enmConv, PVhdxFileIdentifier pFileIdentifierConv,
	PVhdxFileIdentifier pFileIdentifier)
{
	pFileIdentifierConv->u64Signature = SET_ENDIAN_U64(pFileIdentifier->u64Signature);
	for (unsigned i = 0; i < RT_ELEMENTS(pFileIdentifierConv->awszCreator); i++)
		pFileIdentifierConv->awszCreator[i] = SET_ENDIAN_U16(pFileIdentifier->awszCreator[i]);
}

/**
 * Converts a UUID between file and host endianness.
 *
 * @param   enmConv             Direction of the conversion.
 * @param   pUuidConv           Where to store the converted UUID.
 * @param   pUuid               The UUID to convert.
 *
 * @note It is safe to use the same pointer for pUuidConv and pUuid.
 */
static void
vhdxConvUuidEndianess(VHDXECONV enmConv, PRTUUID pUuidConv, PRTUUID pUuid)
{
	(void)enmConv;
	/** @todo r=andy Code looks temporary disabled to me, fixes strict release builds:
	 *        "accessing 16 bytes at offsets 0 and 0 overlaps 16 bytes at offset 0 [-Werror=restrict]" */
	RTUUID uuidTmp;
	grub_memcpy(&uuidTmp, pUuid, sizeof(RTUUID));
	grub_memcpy(pUuidConv, &uuidTmp, sizeof(RTUUID));
}

/**
 * Converts a VHDX header between file and host endianness.
 *
 * @param   enmConv             Direction of the conversion.
 * @param   pHdrConv            Where to store the converted header.
 * @param   pHdr                The VHDX header to convert.
 *
 * @note It is safe to use the same pointer for pHdrConv and pHdr.
 */
static void
vhdxConvHeaderEndianess(VHDXECONV enmConv, PVhdxHeader pHdrConv, PVhdxHeader pHdr)
{
	pHdrConv->u32Signature = SET_ENDIAN_U32(pHdr->u32Signature);
	pHdrConv->u32Checksum = SET_ENDIAN_U32(pHdr->u32Checksum);
	pHdrConv->u64SequenceNumber = SET_ENDIAN_U64(pHdr->u64SequenceNumber);
	vhdxConvUuidEndianess(enmConv, &pHdrConv->UuidFileWrite, &pHdrConv->UuidFileWrite);
	vhdxConvUuidEndianess(enmConv, &pHdrConv->UuidDataWrite, &pHdrConv->UuidDataWrite);
	vhdxConvUuidEndianess(enmConv, &pHdrConv->UuidLog, &pHdrConv->UuidLog);
	pHdrConv->u16LogVersion = SET_ENDIAN_U16(pHdr->u16LogVersion);
	pHdrConv->u16Version = SET_ENDIAN_U16(pHdr->u16Version);
	pHdrConv->u32LogLength = SET_ENDIAN_U32(pHdr->u32LogLength);
	pHdrConv->u64LogOffset = SET_ENDIAN_U64(pHdr->u64LogOffset);
}

/**
 * Converts a VHDX region table header between file and host endianness.
 *
 * @param   enmConv             Direction of the conversion.
 * @param   pRegTblHdrConv      Where to store the converted header.
 * @param   pRegTblHdr          The VHDX region table header to convert.
 *
 * @note It is safe to use the same pointer for pRegTblHdrConv and pRegTblHdr.
 */
static void
vhdxConvRegionTblHdrEndianess(VHDXECONV enmConv, PVhdxRegionTblHdr pRegTblHdrConv,
	PVhdxRegionTblHdr pRegTblHdr)
{
	pRegTblHdrConv->u32Signature = SET_ENDIAN_U32(pRegTblHdr->u32Signature);
	pRegTblHdrConv->u32Checksum = SET_ENDIAN_U32(pRegTblHdr->u32Checksum);
	pRegTblHdrConv->u32EntryCount = SET_ENDIAN_U32(pRegTblHdr->u32EntryCount);
	pRegTblHdrConv->u32Reserved = SET_ENDIAN_U32(pRegTblHdr->u32Reserved);
}

/**
 * Converts a VHDX region table entry between file and host endianness.
 *
 * @param   enmConv             Direction of the conversion.
 * @param   pRegTblEntConv      Where to store the converted region table entry.
 * @param   pRegTblEnt          The VHDX region table entry to convert.
 *
 * @note It is safe to use the same pointer for pRegTblEntConv and pRegTblEnt.
 */
static void
vhdxConvRegionTblEntryEndianess(VHDXECONV enmConv, PVhdxRegionTblEntry pRegTblEntConv,
	PVhdxRegionTblEntry pRegTblEnt)
{
	vhdxConvUuidEndianess(enmConv, &pRegTblEntConv->UuidObject, &pRegTblEnt->UuidObject);
	pRegTblEntConv->u64FileOffset = SET_ENDIAN_U64(pRegTblEnt->u64FileOffset);
	pRegTblEntConv->u32Length = SET_ENDIAN_U32(pRegTblEnt->u32Length);
	pRegTblEntConv->u32Flags = SET_ENDIAN_U32(pRegTblEnt->u32Flags);
}

/**
 * Converts a BAT between file and host endianess.
 *
 * @param   enmConv             Direction of the conversion.
 * @param   paBatEntriesConv    Where to store the converted BAT.
 * @param   paBatEntries        The VHDX BAT to convert.
 * @param   cBatEntries         Number of entries in the BAT.
 *
 * @note It is safe to use the same pointer for paBatEntriesConv and paBatEntries.
 */
static void
vhdxConvBatTableEndianess(VHDXECONV enmConv, PVhdxBatEntry paBatEntriesConv,
	PVhdxBatEntry paBatEntries, grub_uint32_t cBatEntries)
{
	for (grub_uint32_t i = 0; i < cBatEntries; i++)
		paBatEntriesConv[i].u64BatEntry = SET_ENDIAN_U64(paBatEntries[i].u64BatEntry);
}

/**
 * Converts a VHDX metadata table header between file and host endianness.
 *
 * @param   enmConv             Direction of the conversion.
 * @param   pMetadataTblHdrConv Where to store the converted metadata table header.
 * @param   pMetadataTblHdr     The VHDX metadata table header to convert.
 *
 * @note It is safe to use the same pointer for pMetadataTblHdrConv and pMetadataTblHdr.
 */
static void
vhdxConvMetadataTblHdrEndianess(VHDXECONV enmConv, PVhdxMetadataTblHdr pMetadataTblHdrConv,
	PVhdxMetadataTblHdr pMetadataTblHdr)
{
	pMetadataTblHdrConv->u64Signature = SET_ENDIAN_U64(pMetadataTblHdr->u64Signature);
	pMetadataTblHdrConv->u16Reserved = SET_ENDIAN_U16(pMetadataTblHdr->u16Reserved);
	pMetadataTblHdrConv->u16EntryCount = SET_ENDIAN_U16(pMetadataTblHdr->u16EntryCount);
	for (unsigned i = 0; i < RT_ELEMENTS(pMetadataTblHdr->u32Reserved2); i++)
		pMetadataTblHdrConv->u32Reserved2[i] = SET_ENDIAN_U32(pMetadataTblHdr->u32Reserved2[i]);
}

/**
 * Converts a VHDX metadata table entry between file and host endianness.
 *
 * @param   enmConv               Direction of the conversion.
 * @param   pMetadataTblEntryConv Where to store the converted metadata table entry.
 * @param   pMetadataTblEntry     The VHDX metadata table entry to convert.
 *
 * @note It is safe to use the same pointer for pMetadataTblEntryConv and pMetadataTblEntry.
 */
static void
vhdxConvMetadataTblEntryEndianess(VHDXECONV enmConv, PVhdxMetadataTblEntry pMetadataTblEntryConv,
	PVhdxMetadataTblEntry pMetadataTblEntry)
{
	vhdxConvUuidEndianess(enmConv, &pMetadataTblEntryConv->UuidItem, &pMetadataTblEntry->UuidItem);
	pMetadataTblEntryConv->u32Offset = SET_ENDIAN_U32(pMetadataTblEntry->u32Offset);
	pMetadataTblEntryConv->u32Length = SET_ENDIAN_U32(pMetadataTblEntry->u32Length);
	pMetadataTblEntryConv->u32Flags = SET_ENDIAN_U32(pMetadataTblEntry->u32Flags);
	pMetadataTblEntryConv->u32Reserved = SET_ENDIAN_U32(pMetadataTblEntry->u32Reserved);
}

/**
 * Converts a VHDX file parameters item between file and host endianness.
 *
 * @param   enmConv               Direction of the conversion.
 * @param   pFileParamsConv       Where to store the converted file parameters item entry.
 * @param   pFileParams           The VHDX file parameters item to convert.
 *
 * @note It is safe to use the same pointer for pFileParamsConv and pFileParams.
 */
static void
vhdxConvFileParamsEndianess(VHDXECONV enmConv, PVhdxFileParameters pFileParamsConv,
	PVhdxFileParameters pFileParams)
{
	pFileParamsConv->u32BlockSize = SET_ENDIAN_U32(pFileParams->u32BlockSize);
	pFileParamsConv->u32Flags = SET_ENDIAN_U32(pFileParams->u32Flags);
}

/**
 * Converts a VHDX virtual disk size item between file and host endianness.
 *
 * @param   enmConv               Direction of the conversion.
 * @param   pVDiskSizeConv        Where to store the converted virtual disk size item entry.
 * @param   pVDiskSize            The VHDX virtual disk size item to convert.
 *
 * @note It is safe to use the same pointer for pVDiskSizeConv and pVDiskSize.
 */
static void
vhdxConvVDiskSizeEndianess(VHDXECONV enmConv, PVhdxVDiskSize pVDiskSizeConv,
	PVhdxVDiskSize pVDiskSize)
{
	pVDiskSizeConv->u64VDiskSize = SET_ENDIAN_U64(pVDiskSize->u64VDiskSize);
}

/**
 * Converts a VHDX logical sector size item between file and host endianness.
 *
 * @param   enmConv               Direction of the conversion.
 * @param   pVDiskLogSectSizeConv Where to store the converted logical sector size item entry.
 * @param   pVDiskLogSectSize     The VHDX logical sector size item to convert.
 *
 * @note It is safe to use the same pointer for pVDiskLogSectSizeConv and pVDiskLogSectSize.
 */
static void
vhdxConvVDiskLogSectSizeEndianess(VHDXECONV enmConv, PVhdxVDiskLogicalSectorSize pVDiskLogSectSizeConv,
	PVhdxVDiskLogicalSectorSize pVDiskLogSectSize)
{
	pVDiskLogSectSizeConv->u32LogicalSectorSize = SET_ENDIAN_U32(pVDiskLogSectSize->u32LogicalSectorSize);
}

/**
 * Internal. Free all allocated space for representing an image except pImage,
 * and optionally delete the image from disk.
 */
static int vhdxFreeImage(PVHDXIMAGE pImage)
{
	int rc = GRUB_ERR_NONE;

	/* Freeing a never allocated image (e.g. because the open failed) is
	 * not signalled as an error. After all nothing bad happens. */
	if (pImage)
	{
		if (pImage->paBat)
		{
			grub_free(pImage->paBat);
			pImage->paBat = NULL;
		}
	}
	
	return rc;
}

/**
 * Loads all required fields from the given VHDX header.
 * The header must be converted to the host endianess and validated already.
 *
 * @returns VBox status code.
 * @param   pImage    Image instance data.
 * @param   pHdr      The header to load.
 */
static int vhdxLoadHeader(PVHDXIMAGE pImage, PVhdxHeader pHdr)
{
	int rc = GRUB_ERR_NONE;
	/*
	 * Most fields in the header are not required because the backend implements
	 * readonly access only so far.
	 * We just have to check that the log is empty, we have to refuse to load the
	 * image otherwsie because replaying the log is not implemented.
	 */
	if (pHdr->u16Version == VHDX_HEADER_VHDX_VERSION)
	{
		/* Check that the log UUID is zero. */
		pImage->uVersion = pHdr->u16Version;
		if (!RTUuidIsNull(&pHdr->UuidLog))
			rc = GRUB_ERR_NOT_IMPLEMENTED_YET;
	}
	else
		rc = GRUB_ERR_NOT_IMPLEMENTED_YET;

	return rc;
}

/**
 * Determines the current header and loads it.
 *
 * @returns VBox status code.
 * @param   pImage    Image instance data.
 */
static int vhdxFindAndLoadCurrentHeader(PVHDXIMAGE pImage)
{
	PVhdxHeader pHdr1, pHdr2;
	grub_uint32_t u32ChkSum = 0;
	grub_uint32_t u32ChkSumSaved = 0;
	int fHdr1Valid = 0;
	int fHdr2Valid = 0;
	int rc = GRUB_ERR_NONE;

	/*
	 * The VHDX format defines two headers at different offsets to provide failure
	 * consistency. Only one header is current. This can be determined using the
	 * sequence number and checksum fields in the header.
	 */
	pHdr1 = (PVhdxHeader)grub_zalloc(sizeof(VhdxHeader));
	pHdr2 = (PVhdxHeader)grub_zalloc(sizeof(VhdxHeader));

	if (pHdr1 && pHdr2)
	{
		/* Read the first header. */
		rc = vhdxFileReadSync(pImage, VHDX_HEADER1_OFFSET,
			pHdr1, sizeof(*pHdr1), NULL);
		if (RT_SUCCESS(rc))
		{
			vhdxConvHeaderEndianess(VHDXECONV_F2H, pHdr1, pHdr1);

			/* Validate checksum. */
			u32ChkSumSaved = pHdr1->u32Checksum;
			pHdr1->u32Checksum = 0;
			u32ChkSum = RTCrc32C(pHdr1, sizeof(VhdxHeader));

			if (pHdr1->u32Signature == VHDX_HEADER_SIGNATURE
				&& u32ChkSum == u32ChkSumSaved)
				fHdr1Valid = 1;
		}

		/* Try to read the second header in any case (even if reading the first failed). */
		rc = vhdxFileReadSync(pImage, VHDX_HEADER2_OFFSET,
			pHdr2, sizeof(*pHdr2), NULL);
		if (RT_SUCCESS(rc))
		{
			vhdxConvHeaderEndianess(VHDXECONV_F2H, pHdr2, pHdr2);

			/* Validate checksum. */
			u32ChkSumSaved = pHdr2->u32Checksum;
			pHdr2->u32Checksum = 0;
			u32ChkSum = RTCrc32C(pHdr2, sizeof(VhdxHeader));

			if (pHdr2->u32Signature == VHDX_HEADER_SIGNATURE
				&& u32ChkSum == u32ChkSumSaved)
				fHdr2Valid = 1;
		}

		/* Determine the current header. */
		if (fHdr1Valid != fHdr2Valid)
		{
			/* Only one header is valid - use it. */
			rc = vhdxLoadHeader(pImage, fHdr1Valid ? pHdr1 : pHdr2);
		}
		else if (!fHdr1Valid && !fHdr2Valid)
		{
			/* Crap, both headers are corrupt, refuse to load the image. */
			rc = GRUB_ERR_BAD_DEVICE;
		}
		else
		{
			/* Both headers are valid. Use the sequence number to find the current one. */
			if (pHdr1->u64SequenceNumber > pHdr2->u64SequenceNumber)
				rc = vhdxLoadHeader(pImage, pHdr1);
			else
				rc = vhdxLoadHeader(pImage, pHdr2);
		}
	}
	else
		rc = GRUB_ERR_OUT_OF_MEMORY;

	if (pHdr1)
		grub_free(pHdr1);
	if (pHdr2)
		grub_free(pHdr2);

	return rc;
}

/**
 * Loads the BAT region.
 *
 * @returns VBox status code.
 * @param   pImage    Image instance data.
 * @param   offRegion Start offset of the region.
 * @param   cbRegion  Size of the region.
 */
static int
vhdxLoadBatRegion(PVHDXIMAGE pImage, grub_uint64_t offRegion,
	grub_size_t cbRegion)
{
	int rc = GRUB_ERR_NONE;
	grub_uint32_t cDataBlocks;
	grub_uint32_t uChunkRatio;
	grub_uint32_t cSectorBitmapBlocks;
	grub_uint32_t cBatEntries;
	grub_uint32_t cbBatEntries;
	PVhdxBatEntry paBatEntries = NULL;

	/* Calculate required values first. */
	grub_uint64_t uChunkRatio64 = (RT_BIT_64(23) * pImage->cbLogicalSector) / pImage->cbBlock;
	uChunkRatio = (grub_uint32_t)uChunkRatio64;
	grub_uint64_t cDataBlocks64 = pImage->cbSize / pImage->cbBlock;
	cDataBlocks = (grub_uint32_t)cDataBlocks64;

	if (pImage->cbSize % pImage->cbBlock)
		cDataBlocks++;

	cSectorBitmapBlocks = cDataBlocks / uChunkRatio;
	if (cDataBlocks % uChunkRatio)
		cSectorBitmapBlocks++;

	cBatEntries = cDataBlocks + (cDataBlocks - 1) / uChunkRatio;
	cbBatEntries = cBatEntries * sizeof(VhdxBatEntry);

	if (cbBatEntries <= cbRegion)
	{
		/*
		 * Load the complete BAT region first, convert to host endianess and process
		 * it afterwards. The SB entries can be removed because they are not needed yet.
		 */
		paBatEntries = (PVhdxBatEntry)grub_malloc(cbBatEntries);
		if (paBatEntries)
		{
			rc = vhdxFileReadSync(pImage, offRegion,
				paBatEntries, cbBatEntries, NULL);
			if (RT_SUCCESS(rc))
			{
				vhdxConvBatTableEndianess(VHDXECONV_F2H, paBatEntries, paBatEntries,
					cBatEntries);

				/* Go through the table and validate it. */
				for (unsigned i = 0; i < cBatEntries; i++)
				{
					if (i != 0
						&& (i % uChunkRatio) == 0)
					{
						/**
						 * Disabled the verification because there are images out there with the sector bitmap
						 * marked as present. The entry is never accessed and the image is readonly anyway,
						 * so no harm done.
						 */
#if 0
						 /* Sector bitmap block. */
						if (VHDX_BAT_ENTRY_GET_STATE(paBatEntries[i].u64BatEntry)
							!= VHDX_BAT_ENTRY_SB_BLOCK_NOT_PRESENT)
						{
							rc = GRUB_ERR_BAD_DEVICE;
							break;
						}
#endif
					}
					else
					{
						/* Payload block. */
						if (VHDX_BAT_ENTRY_GET_STATE(paBatEntries[i].u64BatEntry)
							== VHDX_BAT_ENTRY_PAYLOAD_BLOCK_PARTIALLY_PRESENT)
						{
							rc = GRUB_ERR_BAD_DEVICE;
							break;
						}
					}
				}

				if (RT_SUCCESS(rc))
				{
					pImage->paBat = paBatEntries;
					pImage->uChunkRatio = uChunkRatio;
				}
			}
			else
				rc = GRUB_ERR_BAD_DEVICE;
		}
		else
			rc = GRUB_ERR_OUT_OF_MEMORY;
	}
	else
		rc = GRUB_ERR_BAD_DEVICE;

	if (RT_FAILURE(rc)
		&& paBatEntries)
		grub_free(paBatEntries);

	return rc;
}

/**
 * Load the file parameters metadata item from the file.
 *
 * @returns VBox status code.
 * @param   pImage    Image instance data.
 * @param   offItem   File offset where the data is stored.
 * @param   cbItem    Size of the item in the file.
 */
static int
vhdxLoadFileParametersMetadata(PVHDXIMAGE pImage, grub_uint64_t offItem, grub_size_t cbItem)
{
	int rc = GRUB_ERR_NONE;

	if (cbItem != sizeof(VhdxFileParameters))
		rc = GRUB_ERR_BAD_DEVICE;
	else
	{
		VhdxFileParameters FileParameters;

		rc = vhdxFileReadSync(pImage, offItem,
			&FileParameters, sizeof(FileParameters), NULL);
		if (RT_SUCCESS(rc))
		{
			vhdxConvFileParamsEndianess(VHDXECONV_F2H, &FileParameters, &FileParameters);
			pImage->cbBlock = FileParameters.u32BlockSize;

			/** @todo No support for differencing images yet. */
			if (FileParameters.u32Flags & VHDX_FILE_PARAMETERS_FLAGS_HAS_PARENT)
				rc = GRUB_ERR_NOT_IMPLEMENTED_YET;
		}
		else
			rc = GRUB_ERR_IO;
	}

	return rc;
}

/**
 * Load the virtual disk size metadata item from the file.
 *
 * @returns VBox status code.
 * @param   pImage    Image instance data.
 * @param   offItem   File offset where the data is stored.
 * @param   cbItem    Size of the item in the file.
 */
static int
vhdxLoadVDiskSizeMetadata(PVHDXIMAGE pImage, grub_uint64_t offItem, grub_size_t cbItem)
{
	int rc = GRUB_ERR_NONE;

	if (cbItem != sizeof(VhdxVDiskSize))
		rc = GRUB_ERR_BAD_DEVICE;
	else
	{
		VhdxVDiskSize VDiskSize;

		rc = vhdxFileReadSync(pImage, offItem,
			&VDiskSize, sizeof(VDiskSize), NULL);
		if (RT_SUCCESS(rc))
		{
			vhdxConvVDiskSizeEndianess(VHDXECONV_F2H, &VDiskSize, &VDiskSize);
			pImage->cbSize = VDiskSize.u64VDiskSize;
		}
		else
			rc = GRUB_ERR_BAD_DEVICE;
	}

	return rc;
}

/**
 * Load the logical sector size metadata item from the file.
 *
 * @returns VBox status code.
 * @param   pImage    Image instance data.
 * @param   offItem   File offset where the data is stored.
 * @param   cbItem    Size of the item in the file.
 */
static int
vhdxLoadVDiskLogSectorSizeMetadata(PVHDXIMAGE pImage, grub_uint64_t offItem, grub_size_t cbItem)
{
	int rc = GRUB_ERR_NONE;

	if (cbItem != sizeof(VhdxVDiskLogicalSectorSize))
		rc = GRUB_ERR_BAD_DEVICE;
	else
	{
		VhdxVDiskLogicalSectorSize VDiskLogSectSize;

		rc = vhdxFileReadSync(pImage, offItem,
			&VDiskLogSectSize, sizeof(VDiskLogSectSize), NULL);
		if (RT_SUCCESS(rc))
		{
			vhdxConvVDiskLogSectSizeEndianess(VHDXECONV_F2H, &VDiskLogSectSize,
				&VDiskLogSectSize);
			pImage->cbLogicalSector = VDiskLogSectSize.u32LogicalSectorSize;
		}
		else
			rc = GRUB_ERR_BAD_DEVICE;
	}

	return rc;
}

/**
 * Loads the metadata region.
 *
 * @returns VBox status code.
 * @param   pImage    Image instance data.
 * @param   offRegion Start offset of the region.
 * @param   cbRegion  Size of the region.
 */
static int
vhdxLoadMetadataRegion(PVHDXIMAGE pImage, grub_uint64_t offRegion,
	grub_size_t cbRegion)
{
	VhdxMetadataTblHdr MetadataTblHdr;
	int rc = GRUB_ERR_NONE;

	/* Load the header first. */
	rc = vhdxFileReadSync(pImage, offRegion,
		&MetadataTblHdr, sizeof(MetadataTblHdr), NULL);
	if (RT_SUCCESS(rc))
	{
		vhdxConvMetadataTblHdrEndianess(VHDXECONV_F2H, &MetadataTblHdr, &MetadataTblHdr);

		/* Validate structure. */
		if (MetadataTblHdr.u64Signature != VHDX_METADATA_TBL_HDR_SIGNATURE)
			rc = GRUB_ERR_BAD_DEVICE;
		else if (MetadataTblHdr.u16EntryCount > VHDX_METADATA_TBL_HDR_ENTRY_COUNT_MAX)
			rc = GRUB_ERR_BAD_DEVICE;
		else if (cbRegion < (MetadataTblHdr.u16EntryCount * sizeof(VhdxMetadataTblEntry) + sizeof(VhdxMetadataTblHdr)))
			rc = GRUB_ERR_BAD_DEVICE;

		if (RT_SUCCESS(rc))
		{
			grub_uint64_t offMetadataTblEntry = offRegion + sizeof(VhdxMetadataTblHdr);

			for (unsigned i = 0; i < MetadataTblHdr.u16EntryCount; i++)
			{
				grub_uint64_t offMetadataItem = 0;
				VHDXMETADATAITEM enmMetadataItem = VHDXMETADATAITEM_UNKNOWN;
				VhdxMetadataTblEntry MetadataTblEntry;

				rc = vhdxFileReadSync(pImage, offMetadataTblEntry,
					&MetadataTblEntry, sizeof(MetadataTblEntry), NULL);
				if (RT_FAILURE(rc))
				{
					rc = GRUB_ERR_IO;
					break;
				}

				vhdxConvMetadataTblEntryEndianess(VHDXECONV_F2H, &MetadataTblEntry, &MetadataTblEntry);

				/* Check whether the flags match the expectations. */
				for (unsigned idxProp = 0; idxProp < RT_ELEMENTS(s_aVhdxMetadataItemProps); idxProp++)
				{
					if (!RTUuidCompareStr(&MetadataTblEntry.UuidItem,
						s_aVhdxMetadataItemProps[idxProp].pszItemUuid))
					{
						/*
						 * Check for specification violations and bail out, except
						 * for the required flag of the physical sector size metadata item.
						 * Early images had the required flag not set opposed to the specification.
						 * We don't want to brerak those images.
						 */
						if (!!(MetadataTblEntry.u32Flags & VHDX_METADATA_TBL_ENTRY_FLAGS_IS_USER)
							!= s_aVhdxMetadataItemProps[idxProp].fIsUser)
							rc = GRUB_ERR_BAD_DEVICE;
						else if (!!(MetadataTblEntry.u32Flags & VHDX_METADATA_TBL_ENTRY_FLAGS_IS_VDISK)
							!= s_aVhdxMetadataItemProps[idxProp].fIsVDisk)
							rc = GRUB_ERR_BAD_DEVICE;
						else if (!!(MetadataTblEntry.u32Flags & VHDX_METADATA_TBL_ENTRY_FLAGS_IS_REQUIRED)
							!= s_aVhdxMetadataItemProps[idxProp].fIsRequired
							&& (s_aVhdxMetadataItemProps[idxProp].enmMetadataItem != VHDXMETADATAITEM_PHYSICAL_SECTOR_SIZE))
							rc = GRUB_ERR_BAD_DEVICE;
						else
							enmMetadataItem = s_aVhdxMetadataItemProps[idxProp].enmMetadataItem;

						break;
					}
				}

				if (RT_FAILURE(rc))
					break;

				offMetadataItem = offRegion + MetadataTblEntry.u32Offset;

				switch (enmMetadataItem)
				{
				case VHDXMETADATAITEM_FILE_PARAMS:
				{
					rc = vhdxLoadFileParametersMetadata(pImage, offMetadataItem,
						MetadataTblEntry.u32Length);
					break;
				}
				case VHDXMETADATAITEM_VDISK_SIZE:
				{
					rc = vhdxLoadVDiskSizeMetadata(pImage, offMetadataItem,
						MetadataTblEntry.u32Length);
					break;
				}
				case VHDXMETADATAITEM_PAGE83_DATA:
				{
					/*
					 * Nothing to do here for now (marked as required but
					 * there is no API to pass this information to the caller)
					 * so far.
					 */
					break;
				}
				case VHDXMETADATAITEM_LOGICAL_SECTOR_SIZE:
				{
					rc = vhdxLoadVDiskLogSectorSizeMetadata(pImage, offMetadataItem,
						MetadataTblEntry.u32Length);
					break;
				}
				case VHDXMETADATAITEM_PHYSICAL_SECTOR_SIZE:
				{
					/*
					 * Nothing to do here for now (marked as required but
					 * there is no API to pass this information to the caller)
					 * so far.
					 */
					break;
				}
				case VHDXMETADATAITEM_PARENT_LOCATOR:
				{
					rc = GRUB_ERR_NOT_IMPLEMENTED_YET;
					break;
				}
				case VHDXMETADATAITEM_UNKNOWN:
				default:
					if (MetadataTblEntry.u32Flags & VHDX_METADATA_TBL_ENTRY_FLAGS_IS_REQUIRED)
						rc = GRUB_ERR_NOT_IMPLEMENTED_YET;
				}

				if (RT_FAILURE(rc))
					break;

				offMetadataTblEntry += sizeof(MetadataTblEntry);
			}
		}
	}
	else
		rc = GRUB_ERR_IO;

	return rc;
}

/**
 * Loads the region table and the associated regions.
 *
 * @returns VBox status code.
 * @param   pImage    Image instance data.
 */
static int
vhdxLoadRegionTable(PVHDXIMAGE pImage)
{
	grub_uint8_t* pbRegionTbl = NULL;
	int rc = GRUB_ERR_NONE;

	/* Load the complete region table into memory. */
	pbRegionTbl = (grub_uint8_t*)grub_malloc(VHDX_REGION_TBL_SIZE_MAX);
	if (pbRegionTbl)
	{
		rc = vhdxFileReadSync(pImage, VHDX_REGION_TBL_HDR_OFFSET,
			pbRegionTbl, VHDX_REGION_TBL_SIZE_MAX, NULL);
		if (RT_SUCCESS(rc))
		{
			PVhdxRegionTblHdr pRegionTblHdr;
			VhdxRegionTblHdr RegionTblHdr;
			grub_uint32_t u32ChkSum = 0;

			/*
			 * Copy the region table header to a dedicated structure where we can
			 * convert it to host endianess.
			 */
			grub_memcpy(&RegionTblHdr, pbRegionTbl, sizeof(RegionTblHdr));
			vhdxConvRegionTblHdrEndianess(VHDXECONV_F2H, &RegionTblHdr, &RegionTblHdr);

			/* Set checksum field to 0 during crc computation. */
			pRegionTblHdr = (PVhdxRegionTblHdr)pbRegionTbl;
			pRegionTblHdr->u32Checksum = 0;

			/* Verify the region table integrity. */
			u32ChkSum = RTCrc32C(pbRegionTbl, VHDX_REGION_TBL_SIZE_MAX);

			if (RegionTblHdr.u32Signature != VHDX_REGION_TBL_HDR_SIGNATURE)
				rc = GRUB_ERR_BAD_DEVICE;
			else if (u32ChkSum != RegionTblHdr.u32Checksum)
				rc = GRUB_ERR_BAD_DEVICE;
			else if (RegionTblHdr.u32EntryCount > VHDX_REGION_TBL_HDR_ENTRY_COUNT_MAX)
				rc = GRUB_ERR_BAD_DEVICE;

			if (RT_SUCCESS(rc))
			{
				/* Parse the region table entries. */
				PVhdxRegionTblEntry pRegTblEntry = (PVhdxRegionTblEntry)(pbRegionTbl + sizeof(VhdxRegionTblHdr));
				VhdxRegionTblEntry RegTblEntryBat; /* BAT region table entry. */
				int fBatRegPresent = 0;
				grub_memset(&RegTblEntryBat, 0, sizeof(VhdxRegionTblEntry)); /* Maybe uninitialized, gcc. */

				for (unsigned i = 0; i < RegionTblHdr.u32EntryCount; i++)
				{
					vhdxConvRegionTblEntryEndianess(VHDXECONV_F2H, pRegTblEntry, pRegTblEntry);

					/* Check the uuid for known regions. */
					if (!RTUuidCompareStr(&pRegTblEntry->UuidObject, VHDX_REGION_TBL_ENTRY_UUID_BAT))
					{
						/*
						 * Save the BAT region and process it later.
						 * It may come before the metadata region but needs the block size.
						 */
						if (pRegTblEntry->u32Flags & VHDX_REGION_TBL_ENTRY_FLAGS_IS_REQUIRED)
						{
							fBatRegPresent = 1;
							RegTblEntryBat.u32Length = pRegTblEntry->u32Length;
							RegTblEntryBat.u64FileOffset = pRegTblEntry->u64FileOffset;
						}
						else
							rc = GRUB_ERR_BAD_DEVICE;
					}
					else if (!RTUuidCompareStr(&pRegTblEntry->UuidObject, VHDX_REGION_TBL_ENTRY_UUID_METADATA))
					{
						if (pRegTblEntry->u32Flags & VHDX_REGION_TBL_ENTRY_FLAGS_IS_REQUIRED)
							rc = vhdxLoadMetadataRegion(pImage, pRegTblEntry->u64FileOffset, pRegTblEntry->u32Length);
						else
							rc = GRUB_ERR_BAD_DEVICE;
					}
					else if (pRegTblEntry->u32Flags & VHDX_REGION_TBL_ENTRY_FLAGS_IS_REQUIRED)
					{
						/* The region is not known but marked as required, fail to load the image. */
						rc = GRUB_ERR_NOT_IMPLEMENTED_YET;
					}

					if (RT_FAILURE(rc))
						break;

					pRegTblEntry++;
				}

				if (fBatRegPresent)
					rc = vhdxLoadBatRegion(pImage, RegTblEntryBat.u64FileOffset, RegTblEntryBat.u32Length);
				else
					rc = GRUB_ERR_BAD_DEVICE;
			}
		}
		else
			rc = GRUB_ERR_IO;
	}
	else
		rc = GRUB_ERR_OUT_OF_MEMORY;

	if (pbRegionTbl)
		grub_free(pbRegionTbl);

	return rc;
}

/**
 * Internal: Open an image, constructing all necessary data structures.
 */
static int
vhdxOpenImage(PVHDXIMAGE pImage)
{
	VhdxFileIdentifier FileIdentifier;
	grub_uint64_t cbFile = grub_file_size(pImage->File);
	int rc = GRUB_ERR_NONE;

	pImage->FileSize = cbFile;
	if (cbFile > sizeof(FileIdentifier))
	{
		rc = vhdxFileReadSync(pImage, VHDX_FILE_IDENTIFIER_OFFSET,
			&FileIdentifier, sizeof(FileIdentifier), NULL);
		if (RT_SUCCESS(rc))
		{
			vhdxConvFileIdentifierEndianess(VHDXECONV_F2H, &FileIdentifier,
				&FileIdentifier);
			if (FileIdentifier.u64Signature != VHDX_FILE_IDENTIFIER_SIGNATURE)
				rc = GRUB_ERR_BAD_DEVICE;
			else
				rc = vhdxFindAndLoadCurrentHeader(pImage);

			/* Load the region table. */
			if (RT_SUCCESS(rc))
				rc = vhdxLoadRegionTable(pImage);
		}
		else
			rc = GRUB_ERR_BAD_DEVICE;
	}

	if (RT_FAILURE(rc))
		vhdxFreeImage(pImage);

	return rc;
}

static int
vhdxOpen(grub_file_t File, void** ppBackendData)
{
	int rc;
	PVHDXIMAGE pImage = (PVHDXIMAGE)grub_zalloc(sizeof(VHDXIMAGE));
	if (!pImage)
	{
		rc = GRUB_ERR_OUT_OF_MEMORY;
		return rc;
	}

	pImage->File = File;
	rc = vhdxOpenImage(pImage);

	if (RT_SUCCESS(rc))
		*ppBackendData = pImage;
	else
		grub_free(pImage);

	return rc;
}

static int
vhdxClose(void* pBackendData)
{
	PVHDXIMAGE pImage = (PVHDXIMAGE)pBackendData;

	int rc = vhdxFreeImage(pImage);
	grub_free(pImage);

	return rc;
}

static int
vhdxRead(void* pBackendData, grub_uint64_t uOffset, void* pvBuf, grub_size_t cbToRead,
	grub_size_t* pcbActuallyRead)
{
	PVHDXIMAGE pImage = (PVHDXIMAGE)pBackendData;
	int rc = GRUB_ERR_NONE;

	if (uOffset + cbToRead > pImage->cbSize
		|| cbToRead == 0)
		rc = GRUB_ERR_BAD_ARGUMENT;
	else
	{
		grub_uint32_t idxBat = (grub_uint32_t)(uOffset / pImage->cbBlock);
		grub_uint32_t offRead = uOffset % pImage->cbBlock;
		grub_uint64_t uBatEntry;

		idxBat += idxBat / pImage->uChunkRatio; /* Add interleaving sector bitmap entries. */
		uBatEntry = pImage->paBat[idxBat].u64BatEntry;

		cbToRead = RT_MIN(cbToRead, pImage->cbBlock - offRead);

		switch (VHDX_BAT_ENTRY_GET_STATE(uBatEntry))
		{
		case VHDX_BAT_ENTRY_PAYLOAD_BLOCK_NOT_PRESENT:
		case VHDX_BAT_ENTRY_PAYLOAD_BLOCK_UNDEFINED:
		case VHDX_BAT_ENTRY_PAYLOAD_BLOCK_ZERO:
		case VHDX_BAT_ENTRY_PAYLOAD_BLOCK_UNMAPPED:
		{
			grub_memset(pvBuf, 0, cbToRead);
			break;
		}
		case VHDX_BAT_ENTRY_PAYLOAD_BLOCK_FULLY_PRESENT:
		{
			grub_uint64_t offFile = VHDX_BAT_ENTRY_GET_FILE_OFFSET(uBatEntry) + offRead;
			rc = vhdxFileReadSync(pImage, offFile,
				pvBuf, cbToRead, NULL);
			break;
		}
		case VHDX_BAT_ENTRY_PAYLOAD_BLOCK_PARTIALLY_PRESENT:
		default:
			rc = GRUB_ERR_BAD_ARGUMENT;
			break;
		}

		if (pcbActuallyRead)
			*pcbActuallyRead = cbToRead;
	}

	return rc;
}

static grub_uint64_t
vhdxGetSize(void* pBackendData)
{
	PVHDXIMAGE pImage = (PVHDXIMAGE)pBackendData;

	if (pImage)
		return pImage->cbSize;
	return 0;
}

struct grub_vhdx
{
	grub_file_t file;
	void* vhdx;
};
typedef struct grub_vhdx* grub_vhdx_t;

static struct grub_fs grub_vhdx_fs;

static grub_err_t
grub_vhdx_close(grub_file_t file)
{
	grub_vhdx_t vhdxio = file->data;

	vhdxClose(vhdxio->vhdx);
	grub_file_close(vhdxio->file);
	grub_free(vhdxio);
	file->disk = 0;
	file->name = 0;
	return grub_errno;
}

static grub_file_t
grub_vhdx_open(grub_file_t io, enum grub_file_type type)
{
	grub_file_t file;
	grub_vhdx_t vhdxio;
	void* vhdx = NULL;

	if (!(type & GRUB_FILE_TYPE_FILTER_VDISK))
		return io;
	if (io->size < 0x10000)
		return io;

	if (vhdxOpen(io, &vhdx) != GRUB_ERR_NONE)
	{
		grub_file_seek(io, 0);
		grub_errno = GRUB_ERR_NONE;
		return io;
	}

	file = (grub_file_t)grub_zalloc(sizeof(*file));
	if (!file)
	{
		vhdxClose(vhdx);
		return 0;
	}

	vhdxio = grub_zalloc(sizeof(*vhdxio));
	if (!vhdxio)
	{
		vhdxClose(vhdx);
		grub_free(file);
		return 0;
	}
	vhdxio->file = io;
	vhdxio->vhdx = vhdx;

	file->disk = io->disk;
	file->data = vhdxio;
	file->fs = &grub_vhdx_fs;
	file->size = GRUB_FILE_SIZE_UNKNOWN;
	file->not_easily_seekable = io->not_easily_seekable;

	file->size = vhdxGetSize(vhdx);

	return file;
}

static grub_ssize_t
grub_vhdx_read(grub_file_t file, char* buf, grub_size_t len)
{
	int rc = GRUB_ERR_NONE;
	grub_size_t real_size;
	grub_ssize_t size = 0;
	grub_vhdx_t vhdxio = file->data;

	while (rc == GRUB_ERR_NONE)
	{
		rc = vhdxRead(vhdxio->vhdx, file->offset, buf, len, &real_size);
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

static struct grub_fs grub_vhdx_fs =
{
	.name = "vhdx",
	.fs_dir = 0,
	.fs_open = 0,
	.fs_read = grub_vhdx_read,
	.fs_close = grub_vhdx_close,
	.fs_label = 0,
	.next = 0
};

GRUB_MOD_INIT(vhdx)
{
	grub_file_filter_register(GRUB_FILE_FILTER_VHDX, grub_vhdx_open);
}

GRUB_MOD_FINI(vhdx)
{
	grub_file_filter_unregister(GRUB_FILE_FILTER_VHDX);
}
