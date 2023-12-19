/*
 * Filetime functions
 *
 * Copyright (C) 2010-2022, Joachim Metz <joachim.metz@gmail.com>
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

#include "libfvalue_definitions.h"
#include "libfvalue_filetime.h"
#include "libfvalue_libfdatetime.h"

#if defined( HAVE_LIBFDATETIME ) || defined( HAVE_LOCAL_LIBFDATETIME )

/* Copies the filetime from an integer value
 * Returns 1 if successful or -1 on error
 */
int libfvalue_filetime_copy_from_integer(
     libfdatetime_filetime_t *filetime,
     uint64_t integer_value,
     size_t integer_value_size,
     libcerror_error_t **error )
{
	static char *function = "libfvalue_filetime_copy_from_integer";

	if( integer_value_size != 64 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported integer value size.",
		 function );

		return( -1 );
	}
	if( libfdatetime_filetime_copy_from_64bit(
	     filetime,
	     integer_value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy filetime from 64-bit value.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Copies the filetime to an integer value
 * Returns 1 if successful or -1 on error
 */
int libfvalue_filetime_copy_to_integer(
     libfdatetime_filetime_t *filetime,
     uint64_t *integer_value,
     size_t *integer_value_size,
     libcerror_error_t **error )
{
	static char *function = "libfvalue_filetime_copy_to_integer";

	if( integer_value_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid integer value size.",
		 function );

		return( -1 );
	}
	if( libfdatetime_filetime_copy_to_64bit(
	     filetime,
	     integer_value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy filetime to 64-bit value.",
		 function );

		return( -1 );
	}
	*integer_value_size = 64;

	return( 1 );
}

#endif /* defined( HAVE_LIBFDATETIME ) || defined( HAVE_LOCAL_LIBFDATETIME ) */

