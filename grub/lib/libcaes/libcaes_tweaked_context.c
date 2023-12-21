/*
 * AES encryption functions
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

#include <libyal_wrap.h>

#if defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_EVP_H )
#include <openssl/err.h>
#include <openssl/evp.h>
#endif

#if defined( HAVE_EVP_CIPHERINIT_EX2 )
#include <openssl/core_names.h>
#endif

#include "libcaes_context.h"
#include "libcaes_definitions.h"
#include "libcaes_tweaked_context.h"
#include "libcaes_types.h"

/* Creates a tweaked context
 * Make sure the value context is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libcaes_tweaked_context_initialize(
     libcaes_tweaked_context_t **tweaked_context,
     libcerror_error_t **error )
{
	libcaes_internal_tweaked_context_t *internal_tweaked_context = NULL;
	static char *function                                        = "libcaes_tweaked_context_initialize";

#if defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_EVP_H ) && defined( HAVE_EVP_CRYPTO_AES_XTS )
	char error_string[ 256 ];

	unsigned long error_code                                     = 0;
#endif

	if( tweaked_context == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid tweaked context.",
		 function );

		return( -1 );
	}
	if( *tweaked_context != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid tweaked context value already set.",
		 function );

		return( -1 );
	}
	internal_tweaked_context = memory_allocate_structure(
	                            libcaes_internal_tweaked_context_t );

	if( internal_tweaked_context == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create tweaked context.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     internal_tweaked_context,
	     0,
	     sizeof( libcaes_internal_tweaked_context_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear tweaked context.",
		 function );

		memory_free(
		 internal_tweaked_context );

		return( -1 );
	}
#if defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_EVP_H ) && defined( HAVE_EVP_CRYPTO_AES_XTS )
#if defined( HAVE_EVP_CIPHER_CTX_INIT )
	EVP_CIPHER_CTX_init(
	 &( internal_tweaked_context->internal_evp_cipher_context ) );

	internal_tweaked_context->evp_cipher_context = &( internal_tweaked_context->internal_evp_cipher_context );
#else
	internal_tweaked_context->evp_cipher_context = EVP_CIPHER_CTX_new();

	if( internal_tweaked_context->evp_cipher_context == NULL )
	{
		error_code = ERR_get_error();

		ERR_error_string_n(
		 error_code,
		 error_string,
		 256 );

		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create EVP cipher context with error: %s.",
		 function,
		 error_string );

		goto on_error;
	}
#endif /* defined( HAVE_EVP_CIPHER_CTX_INIT ) */
#else
	if( libcaes_context_initialize(
	     &( internal_tweaked_context->main_context ),
	     error ) != 1)
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize main context.",
		 function );

		goto on_error;
	}
	if( libcaes_context_initialize(
	     &( internal_tweaked_context->tweak_context ),
	     error ) != 1)
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize tweak context.",
		 function );

		goto on_error;
	}
#endif /* defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_EVP_H ) && defined( HAVE_EVP_CRYPTO_AES_XTS ) */

	*tweaked_context = (libcaes_tweaked_context_t *) internal_tweaked_context;

	return( 1 );

on_error:
	if( internal_tweaked_context != NULL )
	{
#if defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_EVP_H ) && defined( HAVE_EVP_CRYPTO_AES_XTS )
		/* No additional clean up necessary */

#else
		if( internal_tweaked_context->main_context != NULL )
		{
			libcaes_context_free(
			 &( internal_tweaked_context->main_context ),
			 NULL );
		}
#endif /* defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_EVP_H ) && defined( HAVE_EVP_CRYPTO_AES_XTS ) */

		memory_free(
		 internal_tweaked_context );
	}
	return( -1 );
}

/* Frees a tweaked context
 * Returns 1 if successful or -1 on error
 */
int libcaes_tweaked_context_free(
     libcaes_tweaked_context_t **tweaked_context,
     libcerror_error_t **error )
{
	libcaes_internal_tweaked_context_t *internal_tweaked_context = NULL;
	static char *function                                        = "libcaes_tweaked_context_free";
	int result                                                   = 1;

#if defined( HAVE_LIBCRYPTO ) && defined( HAVE_EVP_CIPHER_CTX_CLEANUP )
	char error_string[ 256 ];

	unsigned long error_code                                     = 0;
#endif

	if( tweaked_context == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid tweaked context.",
		 function );

		return( -1 );
	}
	if( *tweaked_context != NULL )
	{
		internal_tweaked_context = (libcaes_internal_tweaked_context_t *) *tweaked_context;
		*tweaked_context         = NULL;

#if defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_EVP_H ) && defined( HAVE_EVP_CRYPTO_AES_XTS )
#if defined( HAVE_EVP_CIPHER_CTX_CLEANUP )
		if( EVP_CIPHER_CTX_cleanup(
		     &( internal_tweaked_context->internal_evp_cipher_context ) ) != 1 )
		{
			error_code = ERR_get_error();

			ERR_error_string_n(
			 error_code,
			 error_string,
			 256 );

			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to clean up EVP cipher context with error: %s.",
			 function,
			 error_string );

			result = -1;
		}
		/* Make sure the error state is removed otherwise OpenSSL will leak memory
		 */
		ERR_remove_thread_state(
		 NULL );
#else
		EVP_CIPHER_CTX_free(
		 internal_tweaked_context->evp_cipher_context );

#endif /* defined( HAVE_EVP_CIPHER_CTX_CLEANUP ) */

		internal_tweaked_context->evp_cipher_context = NULL;
#else
		if( libcaes_context_free(
		     &( internal_tweaked_context->tweak_context ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free tweak context.",
			 function );

			result = -1;
		}
		if( libcaes_context_free(
		     &( internal_tweaked_context->main_context ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free main context.",
			 function );

			result = -1;
		}
#endif /* defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_EVP_H ) && defined( HAVE_EVP_CRYPTO_AES_XTS ) */

		memory_free(
		 internal_tweaked_context );
	}
	return( result );
}

/* Sets the keys
 * Returns 1 if successful or -1 on error
 */
int libcaes_tweaked_context_set_keys(
     libcaes_tweaked_context_t *tweaked_context,
     int mode,
     const uint8_t *key,
     size_t key_bit_size,
     const uint8_t *tweak_key,
     size_t tweak_key_bit_size,
     libcerror_error_t **error )
{
	libcaes_internal_tweaked_context_t *internal_tweaked_context = NULL;
	static char *function                                        = "libcaes_tweaked_context_set_key";

#if defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_EVP_H ) && defined( HAVE_EVP_CRYPTO_AES_XTS )
	size_t key_byte_size                                         = 0;
#endif

	if( tweaked_context == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid tweaked context.",
		 function );

		return( -1 );
	}
	internal_tweaked_context = (libcaes_internal_tweaked_context_t *) tweaked_context;

	if( ( mode != LIBCAES_CRYPT_MODE_DECRYPT )
	 && ( mode != LIBCAES_CRYPT_MODE_ENCRYPT ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported mode.",
		 function );

		return( -1 );
	}
	if( ( key_bit_size != 128 )
	 && ( key_bit_size != 256 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported key bit size.",
		 function );

		return( -1 );
	}
	if( tweak_key_bit_size != key_bit_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported tweak key bit size.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_EVP_H ) && defined( HAVE_EVP_CRYPTO_AES_XTS )
	if( key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key.",
		 function );

		return( -1 );
	}
	if( tweak_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid tweak key.",
		 function );

		return( -1 );
	}
	key_byte_size = key_bit_size / 8;

	if( memory_copy(
	     internal_tweaked_context->key,
	     key,
	     key_byte_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy key.",
		 function );

		return( -1 );
	}
	if( memory_copy(
	     &( internal_tweaked_context->key[ key_byte_size ] ),
	     tweak_key,
	     key_byte_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy tweak key.",
		 function );

		return( -1 );
	}
	internal_tweaked_context->key_bit_size = key_bit_size;

#else
	if( libcaes_context_set_key(
	     internal_tweaked_context->main_context,
	     mode,
	     key,
	     key_bit_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set key in main context.",
		 function );

		return( -1 );
	}
	if( libcaes_context_set_key(
	     internal_tweaked_context->tweak_context,
	     LIBCAES_CRYPT_MODE_ENCRYPT,
	     tweak_key,
	     tweak_key_bit_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set tweak key in tweak context.",
		 function );

		return( -1 );
	}
#endif /* defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_EVP_H ) && defined( HAVE_EVP_CRYPTO_AES_XTS ) */

	return( 1 );
}

#if defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_EVP_H ) && defined( HAVE_EVP_CRYPTO_AES_XTS )

/* De- or encrypts a block of data using AES-XTS (XEX-based tweaked-codebook mode with ciphertext stealing) using OpenSSL EVP
 * The size must be a multitude of the AES block size (16 byte)
 * Returns 1 if successful or -1 on error
 */
int libcaes_crypt_xts(
     libcaes_tweaked_context_t *tweaked_context,
     int mode,
     const uint8_t *tweak_value,
     size_t tweak_value_size,
     const uint8_t *input_data,
     size_t input_data_size,
     uint8_t *output_data,
     size_t output_data_size,
     libcerror_error_t **error )
{
	uint8_t block_data[ EVP_MAX_BLOCK_LENGTH ];
	char error_string[ 256 ];

#if defined( HAVE_EVP_CIPHERINIT_EX2 )
	OSSL_PARAM parameters[2];

	EVP_CIPHER *cipher                                           = NULL;
	const char *cipher_string                                    = NULL;
	unsigned int padding                                         = 0;
#else
	const EVP_CIPHER *cipher                                     = NULL;
#endif

	libcaes_internal_tweaked_context_t *internal_tweaked_context = NULL;
	static char *function                                        = "libcaes_crypt_xts";
	unsigned long error_code                                     = 0;
	int safe_output_data_size                                    = 0;

	if( tweaked_context == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid tweaked context.",
		 function );

		return( -1 );
	}
	internal_tweaked_context = (libcaes_internal_tweaked_context_t *) tweaked_context;

	if( ( mode != LIBCAES_CRYPT_MODE_DECRYPT )
	 && ( mode != LIBCAES_CRYPT_MODE_ENCRYPT ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported mode.",
		 function );

		return( -1 );
	}
	if( tweak_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid tweak value.",
		 function );

		return( -1 );
	}
	if( tweak_value_size != 16 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid tweak value size value out of bounds.",
		 function );

		return( -1 );
	}
	if( input_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid input data.",
		 function );

		return( -1 );
	}
	if( input_data_size > (size_t) INT_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid input data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( input_data_size < 16 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: invalid input data size value too small.",
		 function );

		return( -1 );
	}
	if( output_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid output data.",
		 function );

		return( -1 );
	}
	if( output_data_size > (size_t) INT_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid output data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( output_data_size < input_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid ouput data size smaller than input data size.",
		 function );

		return( -1 );
	}
	if( memory_set(
	     block_data,
	     0,
	     EVP_MAX_BLOCK_LENGTH ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear input block data.",
		 function );

		goto on_error;
	}
#if defined( HAVE_EVP_CIPHERINIT_EX2 )
	if( EVP_CIPHER_CTX_reset(
	     internal_tweaked_context->evp_cipher_context ) != 1 )
	{
		error_code = ERR_get_error();

		ERR_error_string_n(
		 error_code,
		 error_string,
		 256 );

		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to reset cipher context with error: %s.",
		 function,
		 error_string );

		goto on_error;
	}
	if( internal_tweaked_context->key_bit_size == 128 )
	{
		cipher_string = "AES-128-XTS";
	}
	else if( internal_tweaked_context->key_bit_size == 256 )
	{
		cipher_string = "AES-256-XTS";
	}
	cipher = EVP_CIPHER_fetch(
	          NULL,
	          cipher_string,
	          NULL );

	if( cipher == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing cipher.",
		 function );

		goto on_error;
	}
	parameters[0] = OSSL_PARAM_construct_uint(
	                 OSSL_CIPHER_PARAM_PADDING,
	                 &padding );

	parameters[1] = OSSL_PARAM_construct_end();

	if( EVP_CipherInit_ex2(
	     internal_tweaked_context->evp_cipher_context,
	     cipher,
	     (unsigned char *) internal_tweaked_context->key,
	     (unsigned char *) tweak_value,
	     mode,
	     parameters ) != 1 )
	{
		error_code = ERR_get_error();

		ERR_error_string_n(
		 error_code,
		 error_string,
		 256 );

		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize cipher with error: %s.",
		 function,
		 error_string );

		goto on_error;
	}
#else
	if( internal_tweaked_context->key_bit_size == 128 )
	{
		cipher = EVP_aes_128_xts();
	}
	else if( internal_tweaked_context->key_bit_size == 256 )
	{
		cipher = EVP_aes_256_xts();
	}
	if( EVP_CipherInit_ex(
	     internal_tweaked_context->evp_cipher_context,
	     cipher,
	     NULL,
	     (unsigned char *) internal_tweaked_context->key,
	     (unsigned char *) tweak_value,
	     mode ) != 1 )
	{
		error_code = ERR_get_error();

		ERR_error_string_n(
		 error_code,
		 error_string,
		 256 );

		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize cipher with error: %s.",
		 function,
		 error_string );

		goto on_error;
	}
	if( EVP_CIPHER_CTX_set_padding(
	     internal_tweaked_context->evp_cipher_context,
	     1 ) != 1 )
	{
		error_code = ERR_get_error();

		ERR_error_string_n(
		 error_code,
		 error_string,
		 256 );

		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set padding in context with error: %s.",
		 function,
		 error_string );

		goto on_error;
	}
#endif /* defined( HAVE_EVP_CIPHERINIT_EX2 ) */

	if( EVP_CipherUpdate(
	     internal_tweaked_context->evp_cipher_context,
	     (unsigned char *) output_data,
	     &safe_output_data_size,
	     (unsigned char *) input_data,
	     input_data_size ) != 1 )
	{
		error_code = ERR_get_error();

		ERR_error_string_n(
		 error_code,
		 error_string,
		 256 );

		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to update cipher with error: %s.",
		 function,
		 error_string );

		goto on_error;
	}
	/* Just ignore the output of this function
	 */
	EVP_CipherFinal_ex(
	 internal_tweaked_context->evp_cipher_context,
	 (unsigned char *) block_data,
	 &safe_output_data_size );

#if defined( HAVE_EVP_CIPHERINIT_EX2 )
	EVP_CIPHER_free(
	 cipher );
#endif
	return( 1 );

on_error:
#if defined( HAVE_EVP_CIPHERINIT_EX2 )
	if( cipher != NULL )
	{
		EVP_CIPHER_free(
		 cipher );
	}
#endif
	return( -1 );
}

#else

/* De- or encrypts a block of data using AES-XTS (XEX-based tweaked-codebook mode with ciphertext stealing) using fallback implementation
 * The size must be a multitude of the AES block size (16 byte)
 * Returns 1 if successful or -1 on error
 */
int libcaes_crypt_xts(
     libcaes_tweaked_context_t *tweaked_context,
     int mode,
     const uint8_t *tweak_value,
     size_t tweak_value_size,
     const uint8_t *input_data,
     size_t input_data_size,
     uint8_t *output_data,
     size_t output_data_size,
     libcerror_error_t **error )
{
	uint8_t encrypted_tweak_value[ 16 ];
	uint8_t encrypted_tweak_value_copy[ 16 ];

	libcaes_internal_tweaked_context_t *internal_tweaked_context = NULL;
	static char *function                                        = "libcaes_crypt_xts";
	size_t data_offset                                           = 0;
	size_t remaining_data_size                                   = 0;
	uint8_t block_index                                          = 0;
	uint8_t byte_value                                           = 0;
	uint8_t carry_bit                                            = 0;

	if( tweaked_context == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid tweaked context.",
		 function );

		return( -1 );
	}
	internal_tweaked_context = (libcaes_internal_tweaked_context_t *) tweaked_context;

	if( ( mode != LIBCAES_CRYPT_MODE_DECRYPT )
	 && ( mode != LIBCAES_CRYPT_MODE_ENCRYPT ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported mode.",
		 function );

		return( -1 );
	}
	if( tweak_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid tweak value.",
		 function );

		return( -1 );
	}
	if( tweak_value_size != 16 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid tweak value size value out of bounds.",
		 function );

		return( -1 );
	}
	if( input_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid input data.",
		 function );

		return( -1 );
	}
	if( input_data_size < 16 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: invalid input data size value too small.",
		 function );

		return( -1 );
	}
	if( input_data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid input data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( output_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid output data.",
		 function );

		return( -1 );
	}
	if( output_data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid output data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( output_data_size < input_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid ouput data size smaller than input data size.",
		 function );

		return( -1 );
	}
	if( libcaes_crypt_ecb(
	     internal_tweaked_context->tweak_context,
	     LIBCAES_CRYPT_MODE_ENCRYPT,
	     tweak_value,
	     16,
	     encrypted_tweak_value,
	     16,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ENCRYPTION,
		 LIBCERROR_ENCRYPTION_ERROR_GENERIC,
		 "%s: unable to encrypt tweak value.",
		 function );

		goto on_error;
	}
	if( memory_copy(
	     output_data,
	     input_data,
	     input_data_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy input data to output data.",
		 function );

		return( -1 );
	}
	remaining_data_size = input_data_size;

	while( data_offset <= ( input_data_size - 16 ) )
	{
		if( ( remaining_data_size < 32 )
		 && ( remaining_data_size != 16 ) )
		{
			/* If the input data size is not a multitude of 16 the remaining data needs to be handled differently
			 */
			if( mode == LIBCAES_CRYPT_MODE_DECRYPT )
			{
				if( memory_copy(
				     encrypted_tweak_value_copy,
				     encrypted_tweak_value,
				     16 ) == NULL )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_MEMORY,
					 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
					 "%s: unable to copy encrypted tweak value.",
					 function );
				
					goto on_error;
				}
				/* Update the encrypted tweak value for the next 16-byte block
				 */
				carry_bit = 0;

				for( block_index = 0;
				     block_index < 16;
				     block_index++ )
				{
					byte_value = ( encrypted_tweak_value[ block_index ] << 1 ) | carry_bit;
					carry_bit  = encrypted_tweak_value[ block_index ] >> 7;

					encrypted_tweak_value[ block_index ] = byte_value;
				}
				if( carry_bit > 0 )
				{
					encrypted_tweak_value[ 0 ] ^= 0x87;
				}
			}
		}
#if defined( LIBCAES_UNFOLLED_LOOPS )
		output_data[ data_offset++ ] ^= encrypted_tweak_value[ 0 ];
		output_data[ data_offset++ ] ^= encrypted_tweak_value[ 1 ];
		output_data[ data_offset++ ] ^= encrypted_tweak_value[ 2 ];
		output_data[ data_offset++ ] ^= encrypted_tweak_value[ 3 ];
		output_data[ data_offset++ ] ^= encrypted_tweak_value[ 4 ];
		output_data[ data_offset++ ] ^= encrypted_tweak_value[ 5 ];
		output_data[ data_offset++ ] ^= encrypted_tweak_value[ 6 ];
		output_data[ data_offset++ ] ^= encrypted_tweak_value[ 7 ];
		output_data[ data_offset++ ] ^= encrypted_tweak_value[ 8 ];
		output_data[ data_offset++ ] ^= encrypted_tweak_value[ 9 ];
		output_data[ data_offset++ ] ^= encrypted_tweak_value[ 10 ];
		output_data[ data_offset++ ] ^= encrypted_tweak_value[ 11 ];
		output_data[ data_offset++ ] ^= encrypted_tweak_value[ 12 ];
		output_data[ data_offset++ ] ^= encrypted_tweak_value[ 13 ];
		output_data[ data_offset++ ] ^= encrypted_tweak_value[ 14 ];
		output_data[ data_offset++ ] ^= encrypted_tweak_value[ 15 ];
#else
		for( block_index = 0;
		     block_index < 16;
		     block_index++ )
		{
			output_data[ data_offset++ ] ^= encrypted_tweak_value[ block_index ];
		}
#endif
		data_offset -= 16;

		if( libcaes_crypt_ecb(
		     internal_tweaked_context->main_context,
		     mode,
		     &( output_data[ data_offset ] ),
		     16,
		     &( output_data[ data_offset ] ),
		     16,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ENCRYPTION,
			 LIBCERROR_ENCRYPTION_ERROR_GENERIC,
			 "%s: unable to de/encrypt data.",
			 function );

			goto on_error;
		}
#if defined( LIBCAES_UNFOLLED_LOOPS )
		output_data[ data_offset++ ] ^= encrypted_tweak_value[ 0 ];
		output_data[ data_offset++ ] ^= encrypted_tweak_value[ 1 ];
		output_data[ data_offset++ ] ^= encrypted_tweak_value[ 2 ];
		output_data[ data_offset++ ] ^= encrypted_tweak_value[ 3 ];
		output_data[ data_offset++ ] ^= encrypted_tweak_value[ 4 ];
		output_data[ data_offset++ ] ^= encrypted_tweak_value[ 5 ];
		output_data[ data_offset++ ] ^= encrypted_tweak_value[ 6 ];
		output_data[ data_offset++ ] ^= encrypted_tweak_value[ 7 ];
		output_data[ data_offset++ ] ^= encrypted_tweak_value[ 8 ];
		output_data[ data_offset++ ] ^= encrypted_tweak_value[ 9 ];
		output_data[ data_offset++ ] ^= encrypted_tweak_value[ 10 ];
		output_data[ data_offset++ ] ^= encrypted_tweak_value[ 11 ];
		output_data[ data_offset++ ] ^= encrypted_tweak_value[ 12 ];
		output_data[ data_offset++ ] ^= encrypted_tweak_value[ 13 ];
		output_data[ data_offset++ ] ^= encrypted_tweak_value[ 14 ];
		output_data[ data_offset++ ] ^= encrypted_tweak_value[ 15 ];
#else
		for( block_index = 0;
		     block_index < 16;
		     block_index++ )
		{
			output_data[ data_offset++ ] ^= encrypted_tweak_value[ block_index ];
		}
#endif
		remaining_data_size -= 16;

		/* Update the encrypted tweak value for the next 16-byte block
		 */
		carry_bit = 0;

		for( block_index = 0;
		     block_index < 16;
		     block_index++ )
		{
			byte_value = ( encrypted_tweak_value[ block_index ] << 1 ) | carry_bit;
			carry_bit  = encrypted_tweak_value[ block_index ] >> 7;

			encrypted_tweak_value[ block_index ] = byte_value;
		}
		if( carry_bit > 0 )
		{
			encrypted_tweak_value[ 0 ] ^= 0x87;
		}
	}
	/* Any remaining data needs to be handled differently
	 */
	if( remaining_data_size > 0 )
	{
		if( mode == LIBCAES_CRYPT_MODE_DECRYPT )
		{
			if( memory_copy(
			     encrypted_tweak_value,
			     encrypted_tweak_value_copy,
			     16 ) == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
				 "%s: unable to copy encrypted tweak value.",
				 function );
			
				goto on_error;
			}
			if( memory_set(
			     encrypted_tweak_value_copy,
			     0,
			     16 ) == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_SET_FAILED,
				 "%s: unable to clear encrypted tweak value copy.",
				 function );

				goto on_error;
			}
		}
		/* Swap the data of the last 16-byte block with the remaining data
		 */
		data_offset -= 16;

		if( memory_copy(
		     &( output_data[ data_offset + 16 ] ),
		     &( output_data[ data_offset ] ),
		     remaining_data_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy remaining output data.",
			 function );

			goto on_error;
		}
		if( memory_copy(
		     &( output_data[ data_offset ] ),
		     &( input_data[ data_offset + 16 ] ),
		     remaining_data_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy input data to block data.",
			 function );

			goto on_error;
		}
#if defined( LIBCAES_UNFOLLED_LOOPS )
		output_data[ data_offset++ ] ^= encrypted_tweak_value[ 0 ];
		output_data[ data_offset++ ] ^= encrypted_tweak_value[ 1 ];
		output_data[ data_offset++ ] ^= encrypted_tweak_value[ 2 ];
		output_data[ data_offset++ ] ^= encrypted_tweak_value[ 3 ];
		output_data[ data_offset++ ] ^= encrypted_tweak_value[ 4 ];
		output_data[ data_offset++ ] ^= encrypted_tweak_value[ 5 ];
		output_data[ data_offset++ ] ^= encrypted_tweak_value[ 6 ];
		output_data[ data_offset++ ] ^= encrypted_tweak_value[ 7 ];
		output_data[ data_offset++ ] ^= encrypted_tweak_value[ 8 ];
		output_data[ data_offset++ ] ^= encrypted_tweak_value[ 9 ];
		output_data[ data_offset++ ] ^= encrypted_tweak_value[ 10 ];
		output_data[ data_offset++ ] ^= encrypted_tweak_value[ 11 ];
		output_data[ data_offset++ ] ^= encrypted_tweak_value[ 12 ];
		output_data[ data_offset++ ] ^= encrypted_tweak_value[ 13 ];
		output_data[ data_offset++ ] ^= encrypted_tweak_value[ 14 ];
		output_data[ data_offset++ ] ^= encrypted_tweak_value[ 15 ];
#else
		for( block_index = 0;
		     block_index < 16;
		     block_index++ )
		{
			output_data[ data_offset++ ] ^= encrypted_tweak_value[ block_index ];
		}
#endif
		data_offset -= 16;

		if( libcaes_crypt_ecb(
		     internal_tweaked_context->main_context,
		     mode,
		     &( output_data[ data_offset ] ),
		     16,
		     &( output_data[ data_offset ] ),
		     16,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ENCRYPTION,
			 LIBCERROR_ENCRYPTION_ERROR_GENERIC,
			 "%s: unable to de/encrypt data.",
			 function );

			goto on_error;
		}
#if defined( LIBCAES_UNFOLLED_LOOPS )
		output_data[ data_offset++ ] ^= encrypted_tweak_value[ 0 ];
		output_data[ data_offset++ ] ^= encrypted_tweak_value[ 1 ];
		output_data[ data_offset++ ] ^= encrypted_tweak_value[ 2 ];
		output_data[ data_offset++ ] ^= encrypted_tweak_value[ 3 ];
		output_data[ data_offset++ ] ^= encrypted_tweak_value[ 4 ];
		output_data[ data_offset++ ] ^= encrypted_tweak_value[ 5 ];
		output_data[ data_offset++ ] ^= encrypted_tweak_value[ 6 ];
		output_data[ data_offset++ ] ^= encrypted_tweak_value[ 7 ];
		output_data[ data_offset++ ] ^= encrypted_tweak_value[ 8 ];
		output_data[ data_offset++ ] ^= encrypted_tweak_value[ 9 ];
		output_data[ data_offset++ ] ^= encrypted_tweak_value[ 10 ];
		output_data[ data_offset++ ] ^= encrypted_tweak_value[ 11 ];
		output_data[ data_offset++ ] ^= encrypted_tweak_value[ 12 ];
		output_data[ data_offset++ ] ^= encrypted_tweak_value[ 13 ];
		output_data[ data_offset++ ] ^= encrypted_tweak_value[ 14 ];
		output_data[ data_offset++ ] ^= encrypted_tweak_value[ 15 ];
#else
		for( block_index = 0;
		     block_index < 16;
		     block_index++ )
		{
			output_data[ data_offset++ ] ^= encrypted_tweak_value[ block_index ];
		}
#endif
	}
	if( memory_set(
	     encrypted_tweak_value,
	     0,
	     16 ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear encrypted tweak value.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	memory_set(
	 encrypted_tweak_value_copy,
	 0,
	 16 );

	memory_set(
	 encrypted_tweak_value,
	 0,
	 16 );

	return( -1 );
}

#endif /* defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_EVP_H ) && defined( HAVE_EVP_CRYPTO_AES_XTS ) */

