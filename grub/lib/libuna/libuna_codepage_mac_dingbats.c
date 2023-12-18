/*
 * MacDingbats codepage functions
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

#include <libyal_wrap.h>

#include "libuna_codepage_mac_dingbats.h"
#include "libuna_types.h"

/* Extended ASCII to Unicode character lookup table for the MacDingbats codepage
 * Unknown are filled with the Unicode replacement character 0xfffd
 */
const uint16_t libuna_codepage_mac_dingbats_byte_stream_to_unicode_base_0x20[ 224 ] = {
	0x0020, 0x2701, 0x2702, 0x2703, 0x2704, 0x260e, 0x2706, 0x2707,
	0x2708, 0x2709, 0x261b, 0x261e, 0x270c, 0x270d, 0x270e, 0x270f,
	0x2710, 0x2711, 0x2712, 0x2713, 0x2714, 0x2715, 0x2716, 0x2717,
	0x2718, 0x2719, 0x271a, 0x271b, 0x271c, 0x271d, 0x271e, 0x271f,
	0x2720, 0x2721, 0x2722, 0x2723, 0x2724, 0x2725, 0x2726, 0x2727,
	0x2605, 0x2729, 0x272a, 0x272b, 0x272c, 0x272d, 0x272e, 0x272f,
	0x2730, 0x2731, 0x2732, 0x2733, 0x2734, 0x2735, 0x2736, 0x2737,
	0x2738, 0x2739, 0x273a, 0x273b, 0x273c, 0x273d, 0x273e, 0x273f,
	0x2740, 0x2741, 0x2742, 0x2743, 0x2744, 0x2745, 0x2746, 0x2747,
	0x2748, 0x2749, 0x274a, 0x274b, 0x25cf, 0x274d, 0x25a0, 0x274f,
	0x2750, 0x2751, 0x2752, 0x25b2, 0x25bc, 0x25c6, 0x2756, 0x25d7,
	0x2758, 0x2759, 0x275a, 0x275b, 0x275c, 0x275d, 0x275e, 0x007f,
	0x2768, 0x2769, 0x276a, 0x276b, 0x276c, 0x276d, 0x276e, 0x276f,
	0x2770, 0x2771, 0x2772, 0x2773, 0x2774, 0x2775, 0xfffd, 0xfffd,
	0xfffd, 0xfffd, 0xfffd, 0xfffd, 0xfffd, 0xfffd, 0xfffd, 0xfffd,
	0xfffd, 0xfffd, 0xfffd, 0xfffd, 0xfffd, 0xfffd, 0xfffd, 0xfffd,
	0xfffd, 0x2761, 0x2762, 0x2763, 0x2764, 0x2765, 0x2766, 0x2767,
	0x2663, 0x2666, 0x2665, 0x2660, 0x2460, 0x2461, 0x2462, 0x2463,
	0x2464, 0x2465, 0x2466, 0x2467, 0x2468, 0x2469, 0x2776, 0x2777,
	0x2778, 0x2779, 0x277a, 0x277b, 0x277c, 0x277d, 0x277e, 0x277f,
	0x2780, 0x2781, 0x2782, 0x2783, 0x2784, 0x2785, 0x2786, 0x2787,
	0x2788, 0x2789, 0x278a, 0x278b, 0x278c, 0x278d, 0x278e, 0x278f,
	0x2790, 0x2791, 0x2792, 0x2793, 0x2794, 0x2192, 0x2194, 0x2195,
	0x2798, 0x2799, 0x279a, 0x279b, 0x279c, 0x279d, 0x279e, 0x279f,
	0x27a0, 0x27a1, 0x27a2, 0x27a3, 0x27a4, 0x27a5, 0x27a6, 0x27a7,
	0x27a8, 0x27a9, 0x27aa, 0x27ab, 0x27ac, 0x27ad, 0x27ae, 0x27af,
	0xfffd, 0x27b1, 0x27b2, 0x27b3, 0x27b4, 0x27b5, 0x27b6, 0x27b7,
	0x27b8, 0x27b9, 0x27ba, 0x27bb, 0x27bc, 0x27bd, 0x27be, 0xfffd
};

/* Unicode to ASCII character lookup tables for the MacDingbats codepage
 * Unknown are filled with the ASCII replacement character 0x1a
 */
const uint8_t libuna_codepage_mac_dingbats_unicode_to_byte_stream_base_0x2460[ 16 ] = {
	0xac, 0xad, 0xae, 0xaf, 0xb0, 0xb1, 0xb2, 0xb3,
	0xb4, 0xb5, 0x1a, 0x1a, 0x1a, 0x1a, 0x1a, 0x1a
};

const uint8_t libuna_codepage_mac_dingbats_unicode_to_byte_stream_base_0x2700[ 192 ] = {
	0x1a, 0x21, 0x22, 0x23, 0x24, 0x1a, 0x26, 0x27,
	0x28, 0x29, 0x1a, 0x1a, 0x2c, 0x2d, 0x2e, 0x2f,
	0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
	0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f,
	0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47,
	0x1a, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f,
	0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57,
	0x58, 0x59, 0x5a, 0x5b, 0x5c, 0x5d, 0x5e, 0x5f,
	0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67,
	0x68, 0x69, 0x6a, 0x6b, 0x1a, 0x6d, 0x1a, 0x6f,
	0x70, 0x71, 0x72, 0x1a, 0x1a, 0x1a, 0x76, 0x1a,
	0x78, 0x79, 0x7a, 0x7b, 0x7c, 0x7d, 0x7e, 0x1a,
	0x1a, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7,
	0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
	0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0xb6, 0xb7,
	0xb8, 0xb9, 0xba, 0xbb, 0xbc, 0xbd, 0xbe, 0xbf,
	0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7,
	0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf,
	0xd0, 0xd1, 0xd2, 0xd3, 0xd4, 0x1a, 0x1a, 0x1a,
	0xd8, 0xd9, 0xda, 0xdb, 0xdc, 0xdd, 0xde, 0xdf,
	0xe0, 0xe1, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7,
	0xe8, 0xe9, 0xea, 0xeb, 0xec, 0xed, 0xee, 0xef,
	0x1a, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7,
	0xf8, 0xf9, 0xfa, 0xfb, 0xfc, 0xfd, 0xfe, 0x1a
};

/* Copies an Unicode character from a MacDingbats encoded byte stream
 * Returns 1 if successful or -1 on error
 */
int libuna_codepage_mac_dingbats_copy_from_byte_stream(
     libuna_unicode_character_t *unicode_character,
     const uint8_t *byte_stream,
     size_t byte_stream_size,
     size_t *byte_stream_index,
     libcerror_error_t **error )
{
	static char *function                             = "libuna_codepage_mac_dingbats_copy_from_byte_stream";
	libuna_unicode_character_t safe_unicode_character = 0xfffd;
	size_t safe_byte_stream_index                     = 0;
	uint8_t byte_stream_character                     = 0;

	if( unicode_character == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid Unicode character.",
		 function );

		return( -1 );
	}
	if( byte_stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid byte stream.",
		 function );

		return( -1 );
	}
	if( byte_stream_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid byte stream size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( byte_stream_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid byte stream index.",
		 function );

		return( -1 );
	}
	safe_byte_stream_index = *byte_stream_index;

	if( safe_byte_stream_index >= byte_stream_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: byte stream too small.",
		 function );

		return( -1 );
	}
	byte_stream_character = byte_stream[ safe_byte_stream_index++ ];

	if( byte_stream_character < 0x20 )
	{
		safe_unicode_character = byte_stream_character;
	}
	else
	{
		byte_stream_character -= 0x20;

		safe_unicode_character = libuna_codepage_mac_dingbats_byte_stream_to_unicode_base_0x20[ byte_stream_character ];
	}
	*unicode_character = safe_unicode_character;
	*byte_stream_index = safe_byte_stream_index;

	return( 1 );
}

/* Copies an Unicode character to a MacDingbats encoded byte stream
 * Returns 1 if successful or -1 on error
 */
int libuna_codepage_mac_dingbats_copy_to_byte_stream(
     libuna_unicode_character_t unicode_character,
     uint8_t *byte_stream,
     size_t byte_stream_size,
     size_t *byte_stream_index,
     libcerror_error_t **error )
{
	static char *function         = "libuna_codepage_mac_dingbats_copy_to_byte_stream";
	size_t safe_byte_stream_index = 0;
	uint16_t byte_stream_value    = 0x001a;

	if( byte_stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid byte stream.",
		 function );

		return( -1 );
	}
	if( byte_stream_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid byte stream size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( byte_stream_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid byte stream index.",
		 function );

		return( -1 );
	}
	safe_byte_stream_index = *byte_stream_index;

	if( safe_byte_stream_index >= byte_stream_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: byte stream too small.",
		 function );

		return( -1 );
	}
	if( ( unicode_character <= 0x0020 )
	 || ( unicode_character == 0x007f ) )
	{
		byte_stream_value = (uint16_t) unicode_character;
	}
	else if( ( unicode_character >= 0x2460 )
	      && ( unicode_character < 0x2470 ) )
	{
		unicode_character -= 0x2460;

		byte_stream_value = libuna_codepage_mac_dingbats_unicode_to_byte_stream_base_0x2460[ unicode_character ];
	}
	else if( ( unicode_character >= 0x2700 )
	      && ( unicode_character < 0x27c0 ) )
	{
		unicode_character -= 0x2700;

		byte_stream_value = libuna_codepage_mac_dingbats_unicode_to_byte_stream_base_0x2700[ unicode_character ];
	}
	else switch( unicode_character )
	{
		case 0x2192:
			byte_stream_value = 0xd5;
			break;

		case 0x2194:
			byte_stream_value = 0xd6;
			break;

		case 0x2195:
			byte_stream_value = 0xd7;
			break;

		case 0x25a0:
			byte_stream_value = 0x6e;
			break;

		case 0x25b2:
			byte_stream_value = 0x73;
			break;

		case 0x25bc:
			byte_stream_value = 0x74;
			break;

		case 0x25c6:
			byte_stream_value = 0x75;
			break;

		case 0x25cf:
			byte_stream_value = 0x6c;
			break;

		case 0x25d7:
			byte_stream_value = 0x77;
			break;

		case 0x2605:
			byte_stream_value = 0x48;
			break;

		case 0x260e:
			byte_stream_value = 0x25;
			break;

		case 0x261b:
			byte_stream_value = 0x2a;
			break;

		case 0x261e:
			byte_stream_value = 0x2b;
			break;

		case 0x2660:
			byte_stream_value = 0xab;
			break;

		case 0x2663:
			byte_stream_value = 0xa8;
			break;

		case 0x2665:
			byte_stream_value = 0xaa;
			break;

		case 0x2666:
			byte_stream_value = 0xa9;
			break;

		default:
			byte_stream_value = 0x1a;
			break;
	}
	byte_stream[ safe_byte_stream_index++ ] = (uint8_t) ( byte_stream_value & 0x00ff );

	*byte_stream_index = safe_byte_stream_index;

	return( 1 );
}

