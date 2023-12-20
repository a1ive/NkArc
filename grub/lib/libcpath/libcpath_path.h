/*
 * Path functions
 *
 * Copyright (C) 2008-2022, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _LIBCPATH_PATH_H )
#define _LIBCPATH_PATH_H

#include <libyal_wrap.h>

#include "libcpath_extern.h"

#if defined( __cplusplus )
extern "C" {
#endif

int libcpath_path_get_sanitized_character_size(
     char character,
     size_t *sanitized_character_size,
     libcerror_error_t **error );

int libcpath_path_get_sanitized_character(
     char character,
     size_t sanitized_character_size,
     char *sanitized_path,
     size_t sanitized_path_size,
     size_t *sanitized_path_index,
     libcerror_error_t **error );

LIBCPATH_EXTERN \
int libcpath_path_get_sanitized_filename(
     const char *filename,
     size_t filename_length,
     char **sanitized_filename,
     size_t *sanitized_filename_size,
     libcerror_error_t **error );

LIBCPATH_EXTERN \
int libcpath_path_get_sanitized_path(
     const char *path,
     size_t path_length,
     char **sanitized_path,
     size_t *sanitized_path_size,
     libcerror_error_t **error );

LIBCPATH_EXTERN \
int libcpath_path_join(
     char **path,
     size_t *path_size,
     const char *directory_name,
     size_t directory_name_length,
     const char *filename,
     size_t filename_length,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBCPATH_PATH_H ) */

