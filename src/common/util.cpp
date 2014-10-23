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

/** @file common/util.cpp
 *  Common utility functions and macros.
 */

#include <fstream>

#include "common/util.h"

namespace Common {

uint8 readUint8(std::istream &stream) {
	const uint8 x = (uint8) stream.get();

	return x;
}

uint16 readUint16BE(std::istream &stream) {
	const uint16 u1 = readUint8(stream);
	const uint16 u2 = readUint8(stream);

	return (uint16) (u1 << 8) | u2;
}

uint16 readUint16LE(std::istream &stream) {
	const uint16 u1 = readUint8(stream);
	const uint16 u2 = readUint8(stream);

	return (uint16) (u2 << 8) | u1;
}

uint32 readUint32BE(std::istream &stream) {
	const uint32 u1 = readUint8(stream);
	const uint32 u2 = readUint8(stream);
	const uint32 u3 = readUint8(stream);
	const uint32 u4 = readUint8(stream);

	return (uint32) (u1 << 24) | (u2 << 16) | (u3 << 8) | u4;
}

uint32 readUint32LE(std::istream &stream) {
	const uint32 u1 = readUint8(stream);
	const uint32 u2 = readUint8(stream);
	const uint32 u3 = readUint8(stream);
	const uint32 u4 = readUint8(stream);

	return (uint32) (u4 << 24) | (u3 << 16) | (u2 << 8) | u1;
}

uint16 readUint16BE(const byte *data) {
	const uint16 u1 = data[0];
	const uint16 u2 = data[1];

	return (uint16) (u1 << 8) | u2;
}

uint16 readUint16LE(const byte *data) {
	const uint16 u1 = data[0];
	const uint16 u2 = data[1];

	return (uint16) (u2 << 8) | u1;
}

uint32 readUint32BE(const byte *data) {
	const uint16 u1 = data[0];
	const uint16 u2 = data[1];
	const uint16 u3 = data[2];
	const uint16 u4 = data[3];

	return (uint32) (u1 << 24) | (u2 << 16) | (u3 << 8) | u4;
}

uint32 readUint32LE(const byte *data) {
	const uint16 u1 = data[0];
	const uint16 u2 = data[1];
	const uint16 u3 = data[2];
	const uint16 u4 = data[3];

	return (uint32) (u4 << 24) | (u3 << 16) | (u2 << 8) | u1;
}

void readFixedString(std::istream &stream, char *str, int n) {
	stream.read(str, n);
	str[n] = '\0';
}

bool dumpToFile(std::istream &input, uint32 offset, uint32 size, const std::string &output) {
	input.seekg(offset, std::ios_base::beg);

	if (input.tellg() != offset)
		return false;

	std::ofstream outFile;

	outFile.open(output.c_str());
	if (!outFile.is_open())
		return false;

	char buffer[4096];
	while (size > 0) {
		uint32 toRead = MIN<uint32>(size, 4096);

		input.read(buffer, toRead);
		outFile.write(buffer, toRead);

		size -= toRead;
	}

	outFile.flush();

	return input.good() && outFile.good();
}

uint32 getSize(std::istream &stream) {
	uint32 pos = stream.tellg();

	stream.seekg(0, std::ios_base::end);

	uint32 size = stream.tellg();

	stream.seekg(pos, std::ios_base::beg);

	return stream.good() ? size : 0xFFFFFFFF;
}

} // End of namespace Common
