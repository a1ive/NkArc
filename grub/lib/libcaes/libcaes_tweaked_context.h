/*
 * AES tweaked de/encryption context functions
 *
 * Copyright (C) 2011-2023, Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#if !defined( _LIBCAES_TWEAKED_CONTEXT_H )
#define _LIBCAES_TWEAKED_CONTEXT_H

#include <libyal_wrap.h>

#if defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_EVP_H )
#include <openssl/evp.h>
#endif

#include "libcaes_extern.h"
#include "libcaes_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libcaes_internal_tweaked_context libcaes_internal_tweaked_context_t;

struct libcaes_internal_tweaked_context
{
#if defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_EVP_H ) && defined( HAVE_EVP_CRYPTO_AES_XTS )
	/* The EVP cipher context
	 */
#if defined( HAVE_EVP_CIPHER_CTX_INIT )
	EVP_CIPHER_CTX internal_evp_cipher_context;
#endif

	EVP_CIPHER_CTX *evp_cipher_context;

	/* The key
	 */
        uint8_t key[ 64 ];

	/* The key bit size
	 */
	size_t key_bit_size;

#else
	/* The main de/encryption context
	 */
	libcaes_context_t *main_context;

	/* The tweak encryption context
	 */
	libcaes_context_t *tweak_context;

#endif /* defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_EVP_H ) && defined( HAVE_EVP_CRYPTO_AES_XTS ) */
};

LIBCAES_EXTERN \
int libcaes_tweaked_context_initialize(
     libcaes_tweaked_context_t **tweaked_context,
     libcerror_error_t **error );

LIBCAES_EXTERN \
int libcaes_tweaked_context_free(
     libcaes_tweaked_context_t **tweaked_context,
     libcerror_error_t **error );

LIBCAES_EXTERN \
int libcaes_tweaked_context_set_keys(
     libcaes_tweaked_context_t *tweaked_context,
     int mode,
     const uint8_t *key,
     size_t key_bit_size,
     const uint8_t *tweak_key,
     size_t tweak_key_bit_size,
     libcerror_error_t **error );

LIBCAES_EXTERN \
int libcaes_crypt_xts(
     libcaes_tweaked_context_t *tweaked_context,
     int mode,
     const uint8_t *tweak_value,
     size_t tweak_value_size,
     const uint8_t *input_data,
     size_t input_data_size,
     uint8_t *output_data,
     size_t output_data_size,
     libcerror_error_t **error );

#ifdef __cplusplus
}
#endif

#endif /* !defined( _LIBCAES_TWEAKED_CONTEXT_H ) */

