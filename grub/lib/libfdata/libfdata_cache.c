/*
 * Cache functions
 *
 * Copyright (C) 2010-2023, Joachim Metz <joachim.metz@gmail.com>
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

#include "libfdata_cache.h"
#include "libfdata_libfcache.h"

#if !defined( HAVE_LOCAL_LIBFDATA )

/* Creates a cache
 * Make sure the value cache is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfdata_cache_initialize(
     libfdata_cache_t **cache,
     int maximum_cache_entries,
     libcerror_error_t **error )
{
	return( libfcache_cache_initialize(
	         (libfcache_cache_t **) cache,
	         maximum_cache_entries,
	         error ) );
}

/* Frees a cache
 * Returns 1 if successful or -1 on error
 */
int libfdata_cache_free(
     libfdata_cache_t **cache,
     libcerror_error_t **error )
{
	return( libfcache_cache_free(
	         (libfcache_cache_t **) cache,
	         error ) );
}

#endif /* !defined( HAVE_LOCAL_LIBFDATA ) */

