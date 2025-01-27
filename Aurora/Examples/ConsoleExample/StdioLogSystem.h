//
// Copyright (c) 2010-2011 Matthew Jack and Doug Binks
//
// This software is provided 'as-is', without any express or implied
// warranty.  In no event will the authors be held liable for any damages
// arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would be
//    appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#pragma once

#ifndef STDIOLOGSYSTEM_INCLUDED
#define STDIOLOGSYSTEM_INCLUDED

#include "../../Systems/ILogSystem.h"

#include <string>
#include <stdio.h>

// This implementation logs to files only
// When created, it has no files to log to, it must be set

// Hmm. Would be useful to output the error level of the line, for later filtering.
// And the pretty printing and so on... was thinking of pasing in a callback wasn't I?

// Might make file flushing optional, and/or get this onto a different thread for performance

class StdioLogSystem : public ILogSystem
{
public:
	//// ILogSystem interface
	//SErrorDescriptor UnitTest(ILogSystem *) 
	//	{ return SErrorDescriptor(); }                         // Very awkward to unit test this implementation
	
	//// Unique to this implementation
	

	StdioLogSystem(void);
	~StdioLogSystem(void);
	
	ELogVerbosity GetVerbosity() const;
	void SetVerbosity(ELogVerbosity eVerbosity); 
	TVerbosityPeeker GetVerbosityPeeker() const;

	void Log(ELogVerbosity eVerbosity, const char * format, ...);
	void LogVa(va_list args, ELogVerbosity eVerbosity, const char * format);
                                                           // Returns false iff a test was attempted and failed.


protected:
	void LogInternal(ELogVerbosity eVerbosity, const char * format, va_list args);
	ELogVerbosity m_eVerbosity;
	char m_buff[LOGSYSTEM_MAX_BUFFER];
};


#endif //STDIOLOGSYSTEM_INCLUDED