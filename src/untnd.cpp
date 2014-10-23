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

/** @file untnd.cpp
 *  Tool to extract TND archives.
 */

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

const char *kCommandChar[kCommandMAX] = { "l", "x" };

void printUsage(FILE *stream, const char *name);
bool parseCommandLine(int argc, char **argv, int &returnValue, Command &command, std::string &file);

void readFileList(std::istream &stream, std::list<FileInfo> &files, uint32 count);

void listFiles(std::istream &tnd);
void extractFiles(std::istream &tnd);

int main(int argc, char **argv) {
	int returnValue;
	Command command;
	std::string file;
	if (!parseCommandLine(argc, argv, returnValue, command, file))
		return returnValue;

	std::ifstream tnd;

	tnd.open(file.c_str());

	if (!tnd.is_open()) {
		std::printf("Error opening file \"%s\"\n", file.c_str());
		return 2;
	}

	uint32 tndSize    = Common::readUint32BE(tnd);
	uint32 streamSize = Common::getSize(tnd);

	if ((streamSize == 0xFFFFFFFF) || (tndSize != streamSize)) {
		std::printf("Not a valid TND file\n");
		return 3;
	}

	if      (command == kCommandList)
		listFiles(tnd);
	else if (command == kCommandExtract)
		extractFiles(tnd);

	tnd.close();

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
	std::fprintf(stream, "Dark Seed II TND archive extractor\n");
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

void readFileList(std::istream &stream, std::list<FileInfo> &files, uint32 count) {
	// Offset to the start of the data area, directly after the file list
	//                        (name + size + offset) + TND size + number of files
	int startOffset = count * (  8  +   4  +    4  ) +     4    +      4;

	while (count-- > 0) {
		FileInfo file;

		Common::readFixedString(stream, file.name, 8);
		strcat(file.name, ".TXT");

		file.size   = Common::readUint32BE(stream);
		file.offset = Common::readUint32BE(stream) + startOffset;

		files.push_back(file);
	}
}

void listFiles(std::istream &tnd) {
	uint32 fileCount = Common::readUint32BE(tnd);

	std::list<FileInfo> files;
	readFileList(tnd, files, fileCount);

	std::printf("Number of files: %u\n\n", fileCount);

	std::printf(" Filename    | Size\n");
	std::printf("=============|===========\n");

	for (std::list<FileInfo>::const_iterator f = files.begin(); f != files.end(); ++f)
		std::printf("%12s | %10d\n", f->name, f->size);
}

void extractFiles(std::istream &tnd) {
	uint32 fileCount = Common::readUint32BE(tnd);

	std::list<FileInfo> files;
	readFileList(tnd, files, fileCount);

	std::printf("Number of files: %u\n\n", fileCount);

	uint i = 1;
	for (std::list<FileInfo>::const_iterator f = files.begin(); f != files.end(); ++f, ++i) {
		std::printf("Extracting %u/%u: \"%s\"... ", i, fileCount, f->name);
		std::fflush(stdout);

		if (Common::dumpToFile(tnd, f->offset, f->size, f->name))
			std::printf("done\n");
		else
			std::printf("FAILED\n");
	}
}
