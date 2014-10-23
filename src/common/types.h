/* darkseed2-tools - Tools to inspect Dark Seed II resources
 *
 * Copyright (c) 2014, Sven Hesse (DrMcCoy) <drmccoy@drmccoy.de>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Dark Seed is a registered trademark of Cyberdreams, Inc. All rights reserved.
 */

/** @file common/types.h
 *  Low-level type definitions to handle fixed width types portably.
 */

#ifndef COMMON_TYPES_H
#define COMMON_TYPES_H

#ifdef HAVE_CONFIG_H
	#include "config.h"
#endif

#ifdef HAVE_STDINT_H
	#include <stdint.h>
#endif // HAVE_STDINT_H
#ifdef HAVE_INTTYPES_H
	#include <inttypes.h>
#endif // HAVE_INTTYPES_H
#ifdef HAVE_SYS_TYPES_H
	#include <sys/types.h>
#endif // HAVE_SYS_TYPES_H

#if defined (HAVE_STDINT_H)
	typedef int8_t int8;
	typedef uint8_t uint8;
	typedef int16_t int16;
	typedef uint16_t uint16;
	typedef int32_t int32;
	typedef uint32_t uint32;
	typedef int64_t int64;
	typedef uint64_t uint64;
#elif defined (HAVE_INTTYPES_H)
	typedef int8_t int8;
	typedef uint8_t uint8;
	typedef int_16_t int16;
	typedef uint16_t uint16;
	typedef int32_t int32;
	typedef uint32_t uint32;
	typedef int64_t int64;
	typedef uint64_t uint64;
#elif defined (HAVE_SYS_TYPES_H)
	typedef int8_t int8;
	typedef u_int8_t uint8;
	typedef int16_t int16;
	typedef u_int16_t uint16;
	typedef int32_t int32;
	typedef u_int32_t uint32;
	typedef int64_t int64;
	typedef u_int64_t uint64;
#else
	// TODO: Add a fall-back type detection to the configure script
	#error No way to derive fixed-width variable types
#endif

typedef uint8 byte;
typedef unsigned int uint;

#endif // COMMON_TYPES_H
