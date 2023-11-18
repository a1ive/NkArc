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

#ifndef _CRYPTO_HEADER
#define _CRYPTO_HEADER 1

#include <grub/types.h>
#include <grub/mm.h>

typedef enum
{
	GPG_ERR_NO_ERROR,
	GPG_ERR_BAD_MPI,
	GPG_ERR_BAD_SECKEY,
	GPG_ERR_BAD_SIGNATURE,
	GPG_ERR_CIPHER_ALGO,
	GPG_ERR_CONFLICT,
	GPG_ERR_DECRYPT_FAILED,
	GPG_ERR_DIGEST_ALGO,
	GPG_ERR_GENERAL,
	GPG_ERR_INTERNAL,
	GPG_ERR_INV_ARG,
	GPG_ERR_INV_CIPHER_MODE,
	GPG_ERR_INV_FLAG,
	GPG_ERR_INV_KEYLEN,
	GPG_ERR_INV_OBJ,
	GPG_ERR_INV_OP,
	GPG_ERR_INV_SEXP,
	GPG_ERR_INV_VALUE,
	GPG_ERR_MISSING_VALUE,
	GPG_ERR_NO_ENCRYPTION_SCHEME,
	GPG_ERR_NO_OBJ,
	GPG_ERR_NO_PRIME,
	GPG_ERR_NO_SIGNATURE_SCHEME,
	GPG_ERR_NOT_FOUND,
	GPG_ERR_NOT_IMPLEMENTED,
	GPG_ERR_NOT_SUPPORTED,
	GPG_ERR_PUBKEY_ALGO,
	GPG_ERR_SELFTEST_FAILED,
	GPG_ERR_TOO_SHORT,
	GPG_ERR_UNSUPPORTED,
	GPG_ERR_WEAK_KEY,
	GPG_ERR_WRONG_KEY_USAGE,
	GPG_ERR_WRONG_PUBKEY_ALGO,
	GPG_ERR_OUT_OF_MEMORY,
	GPG_ERR_TOO_LARGE,
	GPG_ERR_ENOMEM
} gpg_err_code_t;
typedef gpg_err_code_t gpg_error_t;
typedef gpg_error_t gcry_error_t;
typedef gpg_err_code_t gcry_err_code_t;
#define gcry_error_t gcry_err_code_t
#if 0
enum gcry_cipher_modes
{
	GCRY_CIPHER_MODE_NONE = 0,  /* Not yet specified. */
	GCRY_CIPHER_MODE_ECB = 1,  /* Electronic codebook. */
	GCRY_CIPHER_MODE_CFB = 2,  /* Cipher feedback. */
	GCRY_CIPHER_MODE_CBC = 3,  /* Cipher block chaining. */
	GCRY_CIPHER_MODE_STREAM = 4,  /* Used with stream ciphers. */
	GCRY_CIPHER_MODE_OFB = 5,  /* Outer feedback. */
	GCRY_CIPHER_MODE_CTR = 6   /* Counter. */
};
#endif

/* Don't rely on this. Check!  */
#define GRUB_CRYPTO_MAX_MDLEN 64
#define GRUB_CRYPTO_MAX_CIPHER_BLOCKSIZE 16
#define GRUB_CRYPTO_MAX_MD_CONTEXT_SIZE 256

/* Type for the cipher_setkey function.  */
typedef gcry_err_code_t(*gcry_cipher_setkey_t) (void* c,
	const unsigned char* key,
	unsigned keylen);

/* Type for the cipher_encrypt function.  */
typedef void (*gcry_cipher_encrypt_t) (void* c,
	unsigned char* outbuf,
	const unsigned char* inbuf);

/* Type for the cipher_decrypt function.  */
typedef void (*gcry_cipher_decrypt_t) (void* c,
	unsigned char* outbuf,
	const unsigned char* inbuf);

/* Type for the cipher_stencrypt function.  */
typedef void (*gcry_cipher_stencrypt_t) (void* c,
	unsigned char* outbuf,
	const unsigned char* inbuf,
	unsigned int n);

/* Type for the cipher_stdecrypt function.  */
typedef void (*gcry_cipher_stdecrypt_t) (void* c,
	unsigned char* outbuf,
	const unsigned char* inbuf,
	unsigned int n);

typedef struct gcry_cipher_oid_spec
{
	const char* oid;
	int mode;
} gcry_cipher_oid_spec_t;

/* Module specification structure for ciphers.  */
typedef struct gcry_cipher_spec
{
	const char* name;
	const char** aliases;
	gcry_cipher_oid_spec_t* oids;
	grub_size_t blocksize;
	grub_size_t keylen;
	grub_size_t contextsize;
	gcry_cipher_setkey_t setkey;
	gcry_cipher_encrypt_t encrypt;
	gcry_cipher_decrypt_t decrypt;
	gcry_cipher_stencrypt_t stencrypt;
	gcry_cipher_stdecrypt_t stdecrypt;
#ifdef GRUB_UTIL
	const char* modname;
#endif
	struct gcry_cipher_spec* next;
} gcry_cipher_spec_t;

/* Type for the md_init function.  */
typedef void (*gcry_md_init_t) (void* c);

/* Type for the md_write function.  */
typedef void (*gcry_md_write_t) (void* c, const void* buf, grub_size_t nbytes);

/* Type for the md_final function.  */
typedef void (*gcry_md_final_t) (void* c);

/* Type for the md_read function.  */
typedef unsigned char* (*gcry_md_read_t) (void* c);

typedef struct gcry_md_oid_spec
{
	const char* oidstring;
} gcry_md_oid_spec_t;

typedef struct gcry_md_spec
{
	const char* name;
	unsigned char* asnoid;
	int asnlen;
	gcry_md_oid_spec_t* oids;
	grub_size_t mdlen;
	gcry_md_init_t init;
	gcry_md_write_t write;
	gcry_md_final_t final;
	gcry_md_read_t read;
	grub_size_t contextsize; /* allocate this amount of context */
	/* Block size, needed for HMAC.  */
	grub_size_t blocksize;
#ifdef GRUB_UTIL
	const char* modname;
#endif
	struct gcry_md_spec* next;
} gcry_md_spec_t;

struct gcry_mpi;
typedef struct gcry_mpi* gcry_mpi_t;

/* Type for the pk_generate function.  */
typedef gcry_err_code_t(*gcry_pk_generate_t) (int algo,
	unsigned int nbits,
	unsigned long use_e,
	gcry_mpi_t* skey,
	gcry_mpi_t** retfactors);

/* Type for the pk_check_secret_key function.  */
typedef gcry_err_code_t(*gcry_pk_check_secret_key_t) (int algo,
	gcry_mpi_t* skey);

/* Type for the pk_encrypt function.  */
typedef gcry_err_code_t(*gcry_pk_encrypt_t) (int algo,
	gcry_mpi_t* resarr,
	gcry_mpi_t data,
	gcry_mpi_t* pkey,
	int flags);

/* Type for the pk_decrypt function.  */
typedef gcry_err_code_t(*gcry_pk_decrypt_t) (int algo,
	gcry_mpi_t* result,
	gcry_mpi_t* data,
	gcry_mpi_t* skey,
	int flags);

/* Type for the pk_sign function.  */
typedef gcry_err_code_t(*gcry_pk_sign_t) (int algo,
	gcry_mpi_t* resarr,
	gcry_mpi_t data,
	gcry_mpi_t* skey);

/* Type for the pk_verify function.  */
typedef gcry_err_code_t(*gcry_pk_verify_t) (int algo,
	gcry_mpi_t hash,
	gcry_mpi_t* data,
	gcry_mpi_t* pkey,
	int (*cmp) (void*, gcry_mpi_t),
	void* opaquev);

/* Type for the pk_get_nbits function.  */
typedef unsigned (*gcry_pk_get_nbits_t) (int algo, gcry_mpi_t* pkey);

/* Module specification structure for message digests.  */
typedef struct gcry_pk_spec
{
	const char* name;
	const char** aliases;
	const char* elements_pkey;
	const char* elements_skey;
	const char* elements_enc;
	const char* elements_sig;
	const char* elements_grip;
	int use;
	gcry_pk_generate_t generate;
	gcry_pk_check_secret_key_t check_secret_key;
	gcry_pk_encrypt_t encrypt;
	gcry_pk_decrypt_t decrypt;
	gcry_pk_sign_t sign;
	gcry_pk_verify_t verify;
	gcry_pk_get_nbits_t get_nbits;
#ifdef GRUB_UTIL
	const char* modname;
#endif
} gcry_pk_spec_t;

struct grub_crypto_cipher_handle
{
	const struct gcry_cipher_spec* cipher;
	char ctx[0];
};

typedef struct grub_crypto_cipher_handle* grub_crypto_cipher_handle_t;

static inline void
grub_crypto_cipher_close(grub_crypto_cipher_handle_t cipher)
{
	grub_free(cipher);
}

static inline void
grub_crypto_xor(void* out, const void* in1, const void* in2, grub_size_t size)
{
	const grub_uint8_t* in1ptr = in1, * in2ptr = in2;
	grub_uint8_t* outptr = out;
	while (size && (((grub_addr_t)in1ptr & (sizeof(grub_uint64_t) - 1))
		|| ((grub_addr_t)in2ptr & (sizeof(grub_uint64_t) - 1))
		|| ((grub_addr_t)outptr & (sizeof(grub_uint64_t) - 1))))
	{
		*outptr = *in1ptr ^ *in2ptr;
		in1ptr++;
		in2ptr++;
		outptr++;
		size--;
	}
	while (size >= sizeof(grub_uint64_t))
	{
		/* We've already checked that all pointers are aligned.  */
		*(grub_uint64_t*)(void*)outptr
			= (*(const grub_uint64_t*)(const void*)in1ptr
				^ *(const grub_uint64_t*)(const void*)in2ptr);
		in1ptr += sizeof(grub_uint64_t);
		in2ptr += sizeof(grub_uint64_t);
		outptr += sizeof(grub_uint64_t);
		size -= sizeof(grub_uint64_t);
	}
	while (size)
	{
		*outptr = *in1ptr ^ *in2ptr;
		in1ptr++;
		in2ptr++;
		outptr++;
		size--;
	}
}

void
grub_crypto_hash(const gcry_md_spec_t* hash, void* out, const void* in, grub_size_t inlen);

const gcry_md_spec_t*
grub_crypto_lookup_md_by_name(const char* name);

extern gcry_md_spec_t _gcry_digest_spec_adler32;
extern gcry_md_spec_t _gcry_digest_spec_crc32;
extern gcry_md_spec_t _gcry_digest_spec_crc64;
extern gcry_md_spec_t _gcry_digest_spec_sha1;
extern gcry_md_spec_t _gcry_digest_spec_sha256;
extern gcry_md_spec_t _gcry_digest_spec_md5;

#define GRUB_MD_ADLER32 ((const gcry_md_spec_t *) &_gcry_digest_spec_adler32)
#define GRUB_MD_CRC32 ((const gcry_md_spec_t *) &_gcry_digest_spec_crc32)
#define GRUB_MD_CRC64 ((const gcry_md_spec_t *) &_gcry_digest_spec_crc64)
#define GRUB_MD_SHA1 ((const gcry_md_spec_t *) &_gcry_digest_spec_sha1)
#define GRUB_MD_SHA256 ((const gcry_md_spec_t *) &_gcry_digest_spec_sha256)
#define GRUB_MD_MD5 ((const gcry_md_spec_t *) &_gcry_digest_spec_md5)

#ifndef STR
#define STR(v) #v
#endif
#define STR2(v) STR(v)
#define DIM(v) (sizeof(v)/sizeof((v)[0]))
#define DIMof(type,member) DIM(((type *)0)->member)

/* Stack burning.  */

void _gcry_burn_stack(int bytes);


/* To avoid that a compiler optimizes certain memset calls away, these
   macros may be used instead. */
#define wipememory2(_ptr,_set,_len) \
	do { \
		volatile char *_vptr=(volatile char *)(_ptr); \
		size_t _vlen=(_len); \
		while(_vlen) { *_vptr=(_set); _vptr++; _vlen--; } \
	} while(0)
#define wipememory(_ptr,_len) wipememory2(_ptr,0,_len)

#define rol(x,n) ( ((x) << (n)) | ((x) >> (32-(n))) )
#define ror(x,n) ( ((x) >> (n)) | ((x) << (32-(n))) )

#endif
