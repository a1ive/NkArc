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

#include <nkctx.h>
#include <lang.h>

#include <grub/types.h>
#include <grub/err.h>
#include <grub/misc.h>
#include <grub/file.h>
#include <grub/crypto.h>

#define BUF_SIZE 4096

static grub_err_t
hash_file(grub_file_t file, const gcry_md_spec_t* hash, void* result)
{
	void* context;
	grub_uint8_t* readbuf;
	readbuf = grub_malloc(BUF_SIZE);
	if (!readbuf)
		return grub_errno;
	context = grub_zalloc(hash->contextsize);
	if (!context)
		goto fail;
	nkctx_show_progress();
	hash->init(context);
	while (1)
	{
		grub_ssize_t r = grub_file_read(file, readbuf, BUF_SIZE);
		if (r < 0)
			goto fail;
		if (r == 0)
			break;
		hash->write(context, readbuf, r);
	}
	hash->final(context);
	grub_memcpy(result, hash->read(context), hash->mdlen);
	nkctx_hide_progress();
	grub_free(readbuf);
	grub_free(context);
	return GRUB_ERR_NONE;

fail:
	nkctx_hide_progress();
	grub_free(readbuf);
	grub_free(context);
	return grub_errno;
}

static char*
get_checksum(const char* opt, const char* path)
{
	char* ret = NULL;
	grub_file_t file = NULL;
	grub_size_t len;
	const gcry_md_spec_t* hash = NULL;
	if (opt)
		hash = grub_crypto_lookup_md_by_name(opt);
	if (!hash)
		goto fail;
	if (hash->mdlen > GRUB_CRYPTO_MAX_MDLEN)
		goto fail;
	file = grub_file_open(path, GRUB_FILE_TYPE_HASHLIST | GRUB_FILE_TYPE_NO_DECOMPRESS);
	if (!file)
		goto fail;
	GRUB_PROPERLY_ALIGNED_ARRAY(result, GRUB_CRYPTO_MAX_MDLEN);
	grub_errno = GRUB_ERR_NONE;
	if (hash_file(file, hash, result) != GRUB_ERR_NONE)
		goto fail;
	len = 2 * hash->mdlen + 1;
	ret = grub_malloc(len);
	if (!ret)
		goto fail;
	for (grub_size_t i = 0; i < hash->mdlen; i++)
		grub_snprintf(&ret[2 * i], len, "%02X", ((grub_uint8_t*)result)[i]);
fail:
	if (file)
		grub_file_close(file);
	grub_errno = GRUB_ERR_NONE;
	return ret;
}

enum
{
	M_CTX_PATH = 0,
	M_CTX_MD5 = 1,
	M_CTX_SHA1 = 2,
	M_CTX_SHA256 = 3,
	M_CTX_CRC32 = 4,
	M_CTX_CRC64 = 5,
	M_CTX_MAX,
};

static char* m_ctx[M_CTX_MAX];

void
nkctx_hash_init(const char* path)
{
	memset(m_ctx, 0, sizeof(m_ctx));
	m_ctx[M_CTX_PATH] = grub_strdup(path);
}

void
nkctx_hash_fini(void)
{
	for (grub_size_t i = 0; i < ARRAY_SIZE(m_ctx); i++)
	{
		grub_free(m_ctx[i]);
		m_ctx[i] = NULL;
	}
}

static void
draw_hash(struct nk_context* ctx, const char* desc, const char* name, grub_size_t id)
{
	nk_layout_row_dynamic(ctx, 0, 1);
	nk_label(ctx, desc, NK_TEXT_LEFT);
	if (!m_ctx[id])
	{
		nk_layout_row(ctx, NK_DYNAMIC, 0, 2, (float[2]) { 0.3f, 0.4f });
		nk_spacer(ctx);
		if (nk_button_label(ctx, GET_STR(LANG_STR_CALC)))
			m_ctx[id] = get_checksum(name, m_ctx[M_CTX_PATH]);
	}
	else
		nk_label_wrap(ctx, m_ctx[id]);
}

void
nkctx_hash_window(struct nk_context* ctx, float width, float height)
{
	if (!m_ctx[M_CTX_PATH])
		return;
	if (!nk_begin(ctx, "Checksum",
		nk_rect(4.0f, height / 4.0f, width * 0.9f, height / 2.0f),
		NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE | NK_WINDOW_CLOSABLE))
	{
		nkctx_hash_fini();
		goto out;
	}

	draw_hash(ctx, "MD5", "md5", M_CTX_MD5);
	draw_hash(ctx, "SHA1", "sha1", M_CTX_SHA1);
	draw_hash(ctx, "SHA256", "sha256", M_CTX_SHA256);
	draw_hash(ctx, "CRC32", "crc32", M_CTX_CRC32);
	draw_hash(ctx, "CRC64", "crc64", M_CTX_CRC64);

out:
	nk_end(ctx);
}
