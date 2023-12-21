/*
 * SHA-512 functions
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

#include "libhmac_definitions.h"
#include "libhmac_sha512.h"
#include "libhmac_sha512_context.h"

/* Creates a SHA512 context
 * Make sure the value context is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libhmac_sha512_initialize(
     libhmac_sha512_context_t **context,
     libcerror_error_t **error )
{
	static char *function = "libhmac_sha512_initialize";

	if( libhmac_sha512_context_initialize(
	     context,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize context.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Frees a SHA512 context
 * Returns 1 if successful or -1 on error
 */
int libhmac_sha512_free(
     libhmac_sha512_context_t **context,
     libcerror_error_t **error )
{
	static char *function = "libhmac_sha512_free";

	if( libhmac_sha512_context_free(
	     context,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free context.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Updates the SHA512 context
 * Returns 1 if successful or -1 on error
 */
int libhmac_sha512_update(
     libhmac_sha512_context_t *context,
     const uint8_t *buffer,
     size_t size,
     libcerror_error_t **error )
{
	static char *function = "libhmac_sha512_update";

	if( libhmac_sha512_context_update(
	     context,
	     buffer,
	     size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to update context.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Finalizes the SHA512 context
 * Returns 1 if successful or -1 on error
 */
int libhmac_sha512_finalize(
     libhmac_sha512_context_t *context,
     uint8_t *hash,
     size_t hash_size,
     libcerror_error_t **error )
{
	static char *function = "libhmac_sha512_finalize";

	if( libhmac_sha512_context_finalize(
	     context,
	     hash,
	     hash_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to finalize context.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Calculates the SHA-512 of the buffer
 * Returns 1 if successful or -1 on error
 */
int libhmac_sha512_calculate(
     const uint8_t *buffer,
     size_t size,
     uint8_t *hash,
     size_t hash_size,
     libcerror_error_t **error )
{
	libhmac_sha512_context_t *context = NULL;
	static char *function             = "libhmac_sha512_calculate";

	if( libhmac_sha512_context_initialize(
	     &context,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize context.",
		 function );

		goto on_error;
	}
	if( libhmac_sha512_context_update(
	     context,
	     buffer,
	     size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to update context.",
		 function );

		goto on_error;
	}
	if( libhmac_sha512_context_finalize(
	     context,
	     hash,
	     hash_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to finalize context.",
		 function );

		goto on_error;
	}
	if( libhmac_sha512_context_free(
	     &context,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free context.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( context != NULL )
	{
		libhmac_sha512_context_free(
		 &context,
		 NULL );
	}
	return( -1 );
}

/* Calculates the SHA-512 HMAC of the buffer
 * HMAC is defined in RFC 2104
 * Returns 1 if successful or -1 on error
 */
int libhmac_sha512_calculate_hmac(
     const uint8_t *key,
     size_t key_size,
     const uint8_t *buffer,
     size_t size,
     uint8_t *hmac,
     size_t hmac_size,
     libcerror_error_t **error )
{
	uint8_t key_hash[ LIBHMAC_SHA512_HASH_SIZE ];

	libhmac_sha512_context_t *context = NULL;
	uint8_t *key_data                 = NULL;
	uint8_t *inner_padding            = NULL;
	uint8_t *outer_padding            = NULL;
	static char *function             = "libhmac_sha512_calculate_hmac";
	size_t block_index                = 0;
	size_t block_size                 = 128;

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
	if( key_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid key size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( hmac_size < (size_t) LIBHMAC_SHA512_HASH_SIZE )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: invalid HMAC size value too small.",
		 function );

		return( -1 );
	}
	key_data = (uint8_t *) memory_allocate(
	                        sizeof( uint8_t ) * block_size );

	if( key_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create key data.",
		 function );

		goto on_error;
	}
	if( key_size <= block_size )
	{
		if( memory_copy(
		     key_data,
		     key,
		     key_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy key data.",
			 function );

			goto on_error;
		}
		if( memory_set(
		     &( key_data[ key_size ] ),
		     0,
		     block_size - key_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear key remaining data.",
			 function );

			goto on_error;
		}
	}
	else
	{
		if( libhmac_sha512_context_initialize(
		     &context,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to initialize context.",
			 function );

			goto on_error;
		}
		if( libhmac_sha512_context_update(
		     context,
		     key,
		     key_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to update context.",
			 function );

			goto on_error;
		}
		if( libhmac_sha512_context_finalize(
		     context,
		     key_hash,
		     LIBHMAC_SHA512_HASH_SIZE,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to finalize context.",
			 function );

			goto on_error;
		}
		if( libhmac_sha512_context_free(
		     &context,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free context.",
			 function );

			goto on_error;
		}
		if( block_size > LIBHMAC_SHA512_HASH_SIZE )
		{
			if( memory_set(
			     &( key_data[ LIBHMAC_SHA512_HASH_SIZE ] ),
			     0,
			     block_size - LIBHMAC_SHA512_HASH_SIZE ) == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_SET_FAILED,
				 "%s: unable to clear remaining key data.",
				 function );

				goto on_error;
			}
		}
		if( memory_copy(
		     key_data,
		     key_hash,
		     LIBHMAC_SHA512_HASH_SIZE ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy key hash data.",
			 function );

			goto on_error;
		}
	}
	inner_padding = (uint8_t *) memory_allocate(
	                             sizeof( uint8_t ) * block_size );

	if( inner_padding == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create inner padding.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     inner_padding,
	     0x36,
	     block_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to set inner padding.",
		 function );

		goto on_error;
	}
	outer_padding = (uint8_t *) memory_allocate(
	                             sizeof( uint8_t ) * block_size );

	if( outer_padding == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create outer padding.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     outer_padding,
	     0x5c,
	     block_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to set outer padding.",
		 function );

		goto on_error;
	}
	for( block_index = 0;
	     block_index < block_size;
	     block_index++ )
	{
		inner_padding[ block_index ] ^= key_data[ block_index ];
		outer_padding[ block_index ] ^= key_data[ block_index ];
	}
	if( libhmac_sha512_context_initialize(
	     &context,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize context.",
		 function );

		goto on_error;
	}
	if( libhmac_sha512_context_update(
	     context,
	     inner_padding,
	     block_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to update context.",
		 function );

		goto on_error;
	}
	if( libhmac_sha512_context_update(
	     context,
	     buffer,
	     size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to update context.",
		 function );

		goto on_error;
	}
	if( libhmac_sha512_context_finalize(
	     context,
	     hmac,
	     hmac_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to finalize context.",
		 function );

		goto on_error;
	}
	if( libhmac_sha512_context_free(
	     &context,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free context.",
		 function );

		goto on_error;
	}
	if( libhmac_sha512_context_initialize(
	     &context,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize context.",
		 function );

		goto on_error;
	}
	if( libhmac_sha512_context_update(
	     context,
	     outer_padding,
	     block_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to update context.",
		 function );

		goto on_error;
	}
	if( libhmac_sha512_context_update(
	     context,
	     hmac,
	     LIBHMAC_SHA512_HASH_SIZE,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to update context.",
		 function );

		goto on_error;
	}
	if( libhmac_sha512_context_finalize(
	     context,
	     hmac,
	     hmac_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to finalize context.",
		 function );

		goto on_error;
	}
	if( libhmac_sha512_context_free(
	     &context,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free context.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     outer_padding,
	     0,
	     block_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear outer padding.",
		 function );

		goto on_error;
	}
	memory_free(
	 outer_padding );

	outer_padding = NULL;

	if( memory_set(
	     inner_padding,
	     0,
	     block_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear inner padding.",
		 function );

		goto on_error;
	}
	memory_free(
	 inner_padding );

	inner_padding = NULL;

	if( memory_set(
	     key_data,
	     0,
	     block_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear key data.",
		 function );

		goto on_error;
	}
	memory_free(
	 key_data );

	key_data = NULL;

	return( 1 );

on_error:
	if( context != NULL )
	{
		libhmac_sha512_context_free(
		 &context,
		 NULL );
	}
	if( outer_padding != NULL )
	{
		memory_set(
        	 outer_padding,
	         0,
        	 block_size );

		memory_free(
		 outer_padding );
	}
	if( inner_padding != NULL )
	{
		memory_set(
        	 inner_padding,
	         0,
        	 block_size );

		memory_free(
		 inner_padding );
	}
	if( key_data != NULL )
	{
		memory_set(
        	 key_data,
	         0,
        	 block_size );

		memory_free(
		 key_data );
	}
	return( -1 );
}

