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

/** @file common/util.h
 *  Common utility functions and macros.
 */

#ifndef COMMON_UTIL_H
#define COMMON_UTIL_H

#include "common/types.h"

#include <string>
#include <istream>

#ifdef MAX
#undef MAX
#endif

#ifdef MIN
#undef MIN
#endif

template <typename T> T ABS (T x) { return (x >= 0) ? x : -x; }
template <typename T> T MAX (T a, T b) { return (a > b) ? a : b; }
template <typename T> T MIN (T a, T b) { return (a < b) ? a : b; }
template <typename T> void SWAP(T &a, T &b) { T tmp = a; a = b; b = tmp; }
#define ARRAYSIZE(x) ((int)(sizeof(x) / sizeof(x[0])))

namespace Common {

uint8  readUint8   (std::istream &stream);
uint16 readUint16BE(std::istream &stream);
uint16 readUint16LE(std::istream &stream);
uint32 readUint32BE(std::istream &stream);
uint32 readUint32LE(std::istream &stream);

uint16 readUint16BE(const byte *data);
uint16 readUint16LE(const byte *data);
uint32 readUint32BE(const byte *data);
uint32 readUint32LE(const byte *data);

void readFixedString(std::istream &stream, char *str, int n);

bool dumpToFile(std::istream &input, uint32 offset, uint32 size, const std::string &output);

} // End of namespace Common

#endif // COMMON_UTIL_H
