/*
 * LZFSE decoder functions
 *
 * Copyright (C) 2019-2023, Joachim Metz <joachim.metz@gmail.com>
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

#include "libfmos_definitions.h"
#include "libfmos_lzfse_decoder.h"

/* Creates a decoder
 * Make sure the value decoder is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfmos_lzfse_decoder_initialize(
     libfmos_lzfse_decoder_t **decoder,
     libcerror_error_t **error )
{
	static char *function = "libfmos_lzfse_decoder_initialize";

	if( decoder == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid decoder.",
		 function );

		return( -1 );
	}
	if( *decoder != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid decoder value already set.",
		 function );

		return( -1 );
	}
	*decoder = memory_allocate_structure(
	            libfmos_lzfse_decoder_t );

	if( *decoder == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create decoder.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *decoder,
	     0,
	     sizeof( libfmos_lzfse_decoder_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear decoder.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *decoder != NULL )
	{
		memory_free(
		 *decoder );

		*decoder = NULL;
	}
	return( -1 );
}

/* Frees a decoder
 * Returns 1 if successful or -1 on error
 */
int libfmos_lzfse_decoder_free(
     libfmos_lzfse_decoder_t **decoder,
     libcerror_error_t **error )
{
	static char *function = "libfmos_lzfse_decoder_free";

	if( decoder == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid decoder.",
		 function );

		return( -1 );
	}
	if( *decoder != NULL )
	{
		memory_free(
		 *decoder );

		*decoder = NULL;
	}
	return( 1 );
}

