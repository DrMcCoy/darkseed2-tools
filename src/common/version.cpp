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

/** @file common/version.cpp
 *  Basic version information
 */

#include "common/version.h"

#if defined(HAVE_CONFIG_H)
	#include "config.h"
#endif

// Define default values if the real ones can't be determined

#ifndef PACKAGE_NAME
	#define PACKAGE_NAME "darkseed2-tools"
#endif

#ifndef PACKAGE_VERSION
	#define PACKAGE_VERSION "0.0.0"
#endif

#ifndef PACKAGE_URL
	#define PACKAGE_URL "https://github.com/DrMcCoy/darkseed2-tools/"
#endif

const char *DS2TOOLS_NAME            = PACKAGE_NAME;
const char *DS2TOOLS_VERSION         = PACKAGE_VERSION;
const char *DS2TOOLS_NAMEVERSION     = PACKAGE_NAME " " PACKAGE_VERSION;
const char *DS2TOOLS_COPYRIGHTYEAR   = "2014";
const char *DS2TOOLS_COPYRIGHTAUTHOR = "Sven Hesse (DrMcCoy) <drmccoy@drmccoy.de>";
const char *DS2TOOLS_URL             = PACKAGE_URL;
