/*
 *  GRUB  --  GRand Unified Bootloader
 *  Copyright (C) 2020  Free Software Foundation, Inc.
 *
 *  GRUB is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  GRUB is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with GRUB.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  Arithmetic operations that protect against overflow.
 */

#ifndef GRUB_SAFEMATH_H
#define GRUB_SAFEMATH_H 1

#include <intsafe.h>

#define grub_add(a, b, res)	(UIntPtrAdd(a, b, (UINT_PTR *)res) != S_OK)
#define grub_sub(a, b, res)	(UIntPtrSub(a, b, (UINT_PTR *)res) != S_OK)
#define grub_mul(a, b, res)	(UIntPtrMult(a, b, (UINT_PTR *)res) != S_OK)

#endif /* GRUB_SAFEMATH_H */
