/*
 * The Update Sequence Number (USN) Journal record definition
 *
 * Copyright (C) 2011-2022, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _FUSN_RECORD_H )
#define _FUSN_RECORD_H

#include <libyal_wrap.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct fusn_record_header fusn_record_header_t;

struct fusn_record_header
{
	/* The record size
	 * Consists of 4 bytes
	 */
	uint8_t record_size[ 4 ];

	/* The major version
	 * Consists of 2 bytes
	 */
	uint8_t major_version[ 2 ];

	/* The minor version
	 * Consists of 2 bytes
	 */
	uint8_t minor_version[ 2 ];

	/* The file reference
	 * Consists of 8 bytes
	 */
	uint8_t file_reference[ 8 ];

	/* The parent file reference
	 * Consists of 8 bytes
	 */
	uint8_t parent_file_reference[ 8 ];

	/* The update sequence number (USN)
	 * Consists of 8 bytes
	 */
	uint8_t update_sequence_number[ 8 ];

	/* The update date and time
	 * Consists of 8 bytes
	 * Contains a filetime
	 */
	uint8_t update_time[ 8 ];

	/* The update reason flags
	 * Consists of 4 bytes
	 */
	uint8_t update_reason_flags[ 4 ];

	/* The update source flags
	 * Consists of 4 bytes
	 */
	uint8_t update_source_flags[ 4 ];

	/* The security identifier index
	 * Consists of 4 bytes
	 */
	uint8_t security_identifier_index[ 4 ];

	/* The file attribute flags
	 * Consists of 4 bytes
	 */
	uint8_t file_attribute_flags[ 4 ];

	/* The name size
	 * Consists of 2 bytes
	 */
	uint8_t name_size[ 2 ];

	/* The name offset
	 * Consists of 2 bytes
	 */
	uint8_t name_offset[ 2 ];
};

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _FUSN_RECORD_H ) */

