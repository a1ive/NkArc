/* mm.c - functions for memory manager */
/*
	*  GRUB  --  GRand Unified Bootloader
	*  Copyright (C) 2002,2005,2007,2008,2009  Free Software Foundation, Inc.
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
		The design of this memory manager.

		This is a simple implementation of malloc with a few extensions. These are
		the extensions:

		- memalign is implemented efficiently.

		- multiple regions may be used as free space. They may not be
		contiguous.

		- if existing regions are insufficient to satisfy an allocation, a new
		region can be requested from firmware.

		Regions are managed by a singly linked list, and the meta information is
		stored in the beginning of each region. Space after the meta information
		is used to allocate memory.

		The memory space is used as cells instead of bytes for simplicity. This
		is important for some CPUs which may not access multiple bytes at a time
		when the first byte is not aligned at a certain boundary (typically,
		4-byte or 8-byte). The size of each cell is equal to the size of struct
		grub_mm_header, so the header of each allocated/free block fits into one
		cell precisely. One cell is 16 bytes on 32-bit platforms and 32 bytes
		on 64-bit platforms.

		There are two types of blocks: allocated blocks and free blocks.

		In allocated blocks, the header of each block has only its size. Note that
		this size is based on cells but not on bytes. The header is located right
		before the returned pointer, that is, the header resides at the previous
		cell.

		Free blocks constitutes a ring, using a singly linked list. The first free
		block is pointed to by the meta information of a region. The allocator
		attempts to pick up the second block instead of the first one. This is
		a typical optimization against defragmentation, and makes the
		implementation a bit easier.

		For safety, both allocated blocks and free ones are marked by magic
		numbers. Whenever anything unexpected is detected, GRUB aborts the
		operation.
	*/

#include <config.h>
#include <stdlib.h>
#include <grub/mm.h>
#include <grub/misc.h>
#include <grub/err.h>
#include <grub/types.h>

#include <stdlib.h>

void* grub_calloc (grub_size_t nmemb, grub_size_t size)
{
	void* ptr = calloc(nmemb, size);
	if (!ptr)
		grub_error(GRUB_ERR_OUT_OF_MEMORY, "out of memory");
	return ptr;
}

void* grub_malloc (grub_size_t size)
{
	void* ptr = malloc(size);
	if (!ptr)
		grub_error(GRUB_ERR_OUT_OF_MEMORY, "out of memory");
	return ptr;
}

void* grub_zalloc (grub_size_t size)
{
	return grub_calloc(1, size);
}

void grub_free (void* ptr)
{
	free(ptr);
}

void* grub_realloc (void* ptr, grub_size_t size)
{
	void* new_ptr = realloc(ptr, size);
	if (!new_ptr)
		grub_error(GRUB_ERR_OUT_OF_MEMORY, "out of memory");
	return new_ptr;
}
