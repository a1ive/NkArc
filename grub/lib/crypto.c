/*
 *  NkArc
 *  Copyright (C) 2023 A1ive
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <grub/types.h>
#include <grub/crypto.h>
#include <grub/misc.h>
#include "libgcrypt/gcry_wrap.h"

void
_gcry_burn_stack(int bytes)
{
	char buf[64];

	wipememory(buf, sizeof buf);
	bytes -= sizeof buf;
	if (bytes > 0)
		_gcry_burn_stack(bytes);
}

void
grub_crypto_hash(const gcry_md_spec_t* hash, void* out, const void* in, grub_size_t inlen)
{
	GRUB_PROPERLY_ALIGNED_ARRAY(ctx, GRUB_CRYPTO_MAX_MD_CONTEXT_SIZE);

	if (hash->contextsize > sizeof(ctx))
	{
		grub_fatal("Too large md context");
	}
	hash->init(&ctx);
	hash->write(&ctx, in, inlen);
	hash->final(&ctx);
	grub_memcpy(out, hash->read(&ctx), hash->mdlen);
}

const gcry_md_spec_t*
grub_crypto_lookup_md_by_name(const char* name)
{
	const gcry_md_spec_t* md = NULL;
	if (grub_strcasecmp(name, "ADLER32") == 0)
		md = GRUB_MD_ADLER32;
	else if (grub_strcasecmp(name, "CRC32") == 0)
		md = GRUB_MD_CRC32;
	else if (grub_strcasecmp(name, "CRC64") == 0)
		md = GRUB_MD_CRC64;
	else if (grub_strcasecmp(name, "SHA1") == 0)
		md = GRUB_MD_SHA1;
	else if (grub_strcasecmp(name, "SHA256") == 0)
		md = GRUB_MD_SHA256;
	else if (grub_strcasecmp(name, "MD5") == 0)
		md = GRUB_MD_MD5;
	return md;
}
