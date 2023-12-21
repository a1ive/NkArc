/*
 * MD5 functions
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

#if !defined( _LIBHMAC_MD5_H )
#define _LIBHMAC_MD5_H

#include <libyal_wrap.h>

#include "libhmac_extern.h"
#include "libhmac_md5_context.h"
#include "libhmac_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

LIBHMAC_EXTERN \
int libhmac_md5_initialize(
     libhmac_md5_context_t **context,
     libcerror_error_t **error );

LIBHMAC_EXTERN \
int libhmac_md5_free(
     libhmac_md5_context_t **context,
     libcerror_error_t **error );

LIBHMAC_EXTERN \
int libhmac_md5_update(
     libhmac_md5_context_t *context,
     const uint8_t *buffer,
     size_t size,
     libcerror_error_t **error );

LIBHMAC_EXTERN \
int libhmac_md5_finalize(
     libhmac_md5_context_t *context,
     uint8_t *hash,
     size_t hash_size,
     libcerror_error_t **error );

LIBHMAC_EXTERN \
int libhmac_md5_calculate(
     const uint8_t *buffer,
     size_t size,
     uint8_t *hash,
     size_t hash_size,
     libcerror_error_t **error );

LIBHMAC_EXTERN \
int libhmac_md5_calculate_hmac(
     const uint8_t *key,
     size_t key_size,
     const uint8_t *buffer,
     size_t size,
     uint8_t *hmac,
     size_t hmac_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBHMAC_MD5_H ) */

