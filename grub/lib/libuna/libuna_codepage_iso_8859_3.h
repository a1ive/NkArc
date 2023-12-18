/*
 * ISO 8859-3 codepage (Latin 3) functions
 *
 * Copyright (C) 2008-2023, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _LIBUNA_CODEPAGE_ISO_8859_3_H )
#define _LIBUNA_CODEPAGE_ISO_8859_3_H

#include <libyal_wrap.h>

#include "libuna_extern.h"

#if defined( __cplusplus )
extern "C" {
#endif

LIBUNA_EXTERN_VARIABLE \
const uint16_t libuna_codepage_iso_8859_3_byte_stream_to_unicode_base_0xa0[ 96 ];

LIBUNA_EXTERN_VARIABLE \
const uint8_t libuna_codepage_iso_8859_3_unicode_to_byte_stream_base_0x00a0[ 96 ];

LIBUNA_EXTERN_VARIABLE \
const uint8_t libuna_codepage_iso_8859_3_unicode_to_byte_stream_base_0x0108[ 8 ];

LIBUNA_EXTERN_VARIABLE \
const uint8_t libuna_codepage_iso_8859_3_unicode_to_byte_stream_base_0x0118[ 16 ];

LIBUNA_EXTERN_VARIABLE \
const uint8_t libuna_codepage_iso_8859_3_unicode_to_byte_stream_base_0x0130[ 8 ];

LIBUNA_EXTERN_VARIABLE \
const uint8_t libuna_codepage_iso_8859_3_unicode_to_byte_stream_base_0x0158[ 8 ];

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBUNA_CODEPAGE_ISO_8859_3_H ) */

