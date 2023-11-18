#pragma once

#pragma warning(disable: 4146)	// "unary minus operator applied to unsigned type, result still unsigned"
#pragma warning(disable: 4244)	// "Conversion from X to Y, possible loss of data"
#pragma warning(disable: 4267)	// "Conversion from X to Y, possible loss of data"
#pragma warning(disable: 4334)	// "Result of 32-bit shift implicitly converted to 64 bits"

#if defined(_M_X64) || defined(__x86_64__)
#define GRUB_TARGET_CPU "x86_64"
#elif defined(_M_IX86) || defined(__i386__)
#define GRUB_TARGET_CPU "i386"
#elif defined (_M_ARM) || defined(__arm__)
#define GRUB_TARGET_CPU "arm"
#elif defined (_M_ARM64) || defined(__aarch64__)
#define GRUB_TARGET_CPU "arm64"
#elif defined (_M_RISCV64) || (defined (__riscv) && (__riscv_xlen == 64))
#define GRUB_TARGET_CPU "riscv64"
#else
#error Usupported architecture
#endif

#if !defined(_LARGEFILE_SOURCE)
#define _LARGEFILE_SOURCE
#endif
#define _FILE_OFFSET_BITS 64
#define PACKAGE_STRING "GRUB 2.0"

#define GRUB_PLATFORM "emu"
#define GRUB_MACHINE_EMU
#define GRUB_KERNEL

//#define GRUB_UTIL

#define N_(x) x
#define _(x) x

#define __attribute__(x)
#define __attribute(x)

#define GRUB_MOD_LICENSE(x)

#define GRUB_MOD_INIT(x) \
	void grub_module_init_##x(void)

#define GRUB_MOD_FINI(x) \
	void grub_module_fini_##x(void)

#if !defined(__CHAR_BIT__)
#define __CHAR_BIT__ 8
#endif
