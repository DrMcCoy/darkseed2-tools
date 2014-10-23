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

/** @file unglue.cpp
 *  Tool to extract Glue archives.
 */

#include <cassert>
#include <cctype>
#include <cstdio>
#include <cstring>

#include <list>
#include <string>
#include <fstream>

#include "common/util.h"
#include "common/version.h"

struct FileInfo {
	char name[13];
	uint32 offset;
	uint32 size;

	FileInfo(const char *n = "", uint32 o = 0, uint32 s = 0) : offset(o), size(s) {
		strncpy(name, n, 12);
		name[12] = '\0';
	}
};

enum Command {
	kCommandNone    = -1,
	kCommandList        ,
	kCommandExtract     ,
	kCommandMAX
};

/** A class wrapping a memory block into an std::istream. */
class MemoryReadStream : public std::istream {
private:
	class StreamBuf : public std::streambuf {
	public:
		StreamBuf(byte *data, uint32 size) {
			setg((char *) data, (char *) data, (char *) data + size);
		}

		~StreamBuf() {
		}

		std::streampos seekoff(std::streamoff off, std::ios_base::seekdir way, std::ios_base::openmode which) {
			if (!(which & std::ios_base::in))
				return -1;

			int32 offset = 0;
			if      (way == std::ios_base::beg)
				offset = off;
			else if (way == std::ios_base::cur)
				offset = (gptr() - eback()) + off;
			else if (way == std::ios_base::end)
				offset = (egptr() - eback()) + off;

			if ((offset < 0) || (offset >= (egptr() - eback())))
				return -1;

			setg(eback(), eback() + offset, egptr());
			return offset;
		}

		std::streampos seekpos(std::streampos sp, std::ios_base::openmode which) {
			return seekoff(sp, std::ios_base::beg, which);
		}
	};

	bool _dispose;
	byte *_data;
	StreamBuf *_streamBuf;

public:
	MemoryReadStream(byte *data, uint32 size, bool dispose = false) :
		std::istream(_streamBuf = new StreamBuf(data, size)), _dispose(dispose), _data(data) {
	}

	~MemoryReadStream() {
		delete _streamBuf;

		if (_dispose)
			delete[] _data;
	}
};

const char *kCommandChar[kCommandMAX] = { "l", "x" };

void printUsage(FILE *stream, const char *name);
bool parseCommandLine(int argc, char **argv, int &returnValue, Command &command, std::string &file);

bool isCompressed(std::istream &stream);
MemoryReadStream *uncompressGlue(std::istream &stream);
uint32 uncompressGlueChunk(byte *outBuf, const byte *inBuf, int n);

void readFileList(std::istream &stream, std::list<FileInfo> &files, uint32 count);

void listFiles(std::istream *glue);
void extractFiles(std::istream *glue);

int main(int argc, char **argv) {
	int returnValue;
	Command command;
	std::string file;
	if (!parseCommandLine(argc, argv, returnValue, command, file))
		return returnValue;

	std::ifstream glue;

	glue.open(file.c_str());

	if (!glue.is_open()) {
		std::printf("Error opening file \"%s\"\n", file.c_str());
		return 2;
	}

	if      (command == kCommandList)
		listFiles(&glue);
	else if (command == kCommandExtract)
		extractFiles(&glue);

	glue.close();

	return 0;
}

bool parseCommandLine(int argc, char **argv, int &returnValue, Command &command, std::string &file) {
	file.clear();

	// No command, just display the help
	if (argc == 1) {
		printUsage(stdout, argv[0]);
		returnValue = 0;

		return false;
	}

	// Wrong number of arguments, display the help
	if (argc != 3) {
		printUsage(stderr, argv[0]);
		returnValue = 1;

		return false;
	}

	// Find out what we should do
	command = kCommandNone;
	for (int i = 0; i < kCommandMAX; i++)
		if (!strcmp(argv[1], kCommandChar[i]))
			command = (Command) i;

	// Unknown command
	if (command == kCommandNone) {
		printUsage(stderr, argv[0]);
		returnValue = 1;

		return false;
	}

	// This is the file to use
	file = argv[2];

	return true;
}

void printUsage(FILE *stream, const char *name) {
	std::fprintf(stream, "Dark Seed II Glue archive extractor\n");
	std::fprintf(stream, "\n");
	std::fprintf(stream, "%s\n", DS2TOOLS_NAMEVERSION);
	std::fprintf(stream, "Copyright (c) %s, %s\n", DS2TOOLS_COPYRIGHTYEAR, DS2TOOLS_COPYRIGHTAUTHOR);
	std::fprintf(stream, "%s\n", DS2TOOLS_URL);
	std::fprintf(stream, "\n");
	std::fprintf(stream, "Usage: %s <command> <file>\n\n", name);
	std::fprintf(stream, "Commands:\n");
	std::fprintf(stream, "  l          List archive contents\n");
	std::fprintf(stream, "  x          Extract files to current directory\n");
}

// Check whether a glue is compressed by size range and other sanity checks
bool isCompressed(std::istream &stream) {
	stream.seekg(0, std::ios_base::beg);

	uint32 fSize = Common::getSize(stream);

	uint32 numRes = Common::readUint16LE(stream);

	// The resource list has to fit
	if (fSize <= (numRes * 22)) {
		stream.seekg(0, std::ios_base::beg);
		return true;
	}

	char buffer[12];
	while (numRes-- > 0) {
		stream.read(buffer, 12);

		// Only these character are allowed in a resource file name
		for (int i = 0; (i < 12) && (buffer[i] != 0); i++)
			if (!isalnum(buffer[i]) && (buffer[i] != '.') && (buffer[i] != '_')) {
				stream.seekg(0, std::ios_base::beg);
				return true;
			}

		uint32 size   = Common::readUint32LE(stream);
		uint32 offset = Common::readUint32LE(stream);

		// The resources have to fit
		if ((size + offset) > fSize) {
			stream.seekg(0, std::ios_base::beg);
			return true;
		}
	}

	stream.seekg(0, std::ios_base::beg);
	return false;
}

// Some LZ-variant
uint32 uncompressGlueChunk(byte *outBuf, const byte *inBuf, int n) {
	int countRead    = 0;
	int countWritten = 0;

	uint16 mask;
	int32 offset;
	uint32 count;

	mask = 0xFF00 | *inBuf++;

	while (1) {
		if (mask & 1) {
			// Direct copy

			mask >>= 1;

			*outBuf++ = *inBuf++;
			*outBuf++ = *inBuf++;

			countWritten += 2;

		} else {
			// Copy from previous output

			mask >>= 1;

			count = Common::readUint16LE(inBuf);
			inBuf += 2;

			offset = (count >> 4)  + 1;
			count  = (count & 0xF) + 3;

			for (int i = 0; i < 8; i++)
				outBuf[i] = outBuf[-offset + i];

			if (count > 8)
				for (int i = 0; i < 10; i++)
					outBuf[i + 8] = outBuf[-offset + 8 + i];

			outBuf += count;
			countWritten += count;
		}

		if ((mask & 0xFF00) == 0) {
			countRead += 17;
			if (countRead >= n)
				break;

			mask = 0xFF00 | *inBuf++;
		}
	}

	return countWritten;
}

// Uncompress a glue from 2048 byte LZ chunks
MemoryReadStream *uncompressGlue(std::istream &stream) {
	stream.seekg(0, std::ios_base::beg);

	byte inBuf[2048];

	memset(inBuf, 0, 2048);

	stream.read((char *) inBuf, 2048);
	int nRead = stream.gcount();

	if (nRead != 2048)
		return 0;

	uint32 size = Common::readUint32LE(inBuf + 2044) + 128;

	// Sanity check
	assert(size < (10*1024*1024));

	byte *outBuf = new byte[size];

	memset(outBuf, 0, size);

	byte *oBuf = outBuf;
	while (nRead != 0) {
		uint32 toRead = 2040;
		uint32 written;

		if (nRead != 2048)
			// Round up to the next 17 byte block
			toRead = ((nRead + 16) / 17) * 17;

		// Decompress that chunk
		written = uncompressGlueChunk(oBuf, inBuf, toRead);

		oBuf += written;

		memset(inBuf, 0, 2048);
		stream.read((char *) inBuf, 2048);
		nRead = stream.gcount();
	}

	return new MemoryReadStream(outBuf, size, true);
}

void readFileList(std::istream &stream, std::list<FileInfo> &files, uint32 count) {
	while (count-- > 0) {
		FileInfo file;

		Common::readFixedString(stream, file.name, 12);

		file.size   = Common::readUint32LE(stream);
		file.offset = Common::readUint32LE(stream);

		files.push_back(file);
	}
}

void listFiles(std::istream *glue) {
	MemoryReadStream *uncompressed = 0;

	// If the file is compressed, uncompress it and operate on that
	if (isCompressed(*glue))
		glue = uncompressed = uncompressGlue(*glue);

	uint32 fileCount = Common::readUint16LE(*glue);

	std::list<FileInfo> files;
	readFileList(*glue, files, fileCount);

	std::printf("Number of files: %u\n\n", fileCount);

	std::printf(" Filename    | Size\n");
	std::printf("=============|===========\n");

	for (std::list<FileInfo>::const_iterator f = files.begin(); f != files.end(); ++f)
		std::printf("%12s | %10d\n", f->name, f->size);

	delete uncompressed;
}

void extractFiles(std::istream *glue) {
	MemoryReadStream *uncompressed = 0;

	// If the file is compressed, uncompress it and operate on that
	if (isCompressed(*glue))
		glue = uncompressed = uncompressGlue(*glue);

	uint32 fileCount = Common::readUint16LE(*glue);

	std::list<FileInfo> files;
	readFileList(*glue, files, fileCount);

	std::printf("Number of files: %u\n\n", fileCount);

	uint i = 1;
	for (std::list<FileInfo>::const_iterator f = files.begin(); f != files.end(); ++f, ++i) {
		std::printf("Extracting %u/%u: \"%s\"... ", i, fileCount, f->name);
		std::fflush(stdout);

		if (Common::dumpToFile(*glue, f->offset, f->size, f->name))
			std::printf("done\n");
		else
			std::printf("FAILED\n");
	}

	delete uncompressed;
}
