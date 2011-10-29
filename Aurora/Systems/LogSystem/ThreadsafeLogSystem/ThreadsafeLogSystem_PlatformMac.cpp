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

#include "ThreadsafeLogSystem.h"

#include <stdarg.h>
#include <assert.h>

#pragma warning( disable : 4996 4800 )

typedef int CRITICAL_SECTION;

void InitializeCriticalSection(int*) {}
void EnterCriticalSection(int*) {}
void LeaveCriticalSection(int*) {}
void DeleteCriticalSection(int*) {}


struct ThreadsafeLogSystem::TLSPlatformImpl
{
	CRITICAL_SECTION critSec;
};


ThreadsafeLogSystem::ThreadsafeLogSystem(void)
{
	m_pImpl = new TLSPlatformImpl();
	InitializeCriticalSection(&(m_pImpl->critSec));

	m_eVerbosity = eLV_COMMENTS;   // By default, defer any filtering
	m_protectedLogger = NULL;			
}

ThreadsafeLogSystem::~ThreadsafeLogSystem(void)
{
	delete m_protectedLogger;
	DeleteCriticalSection(&(m_pImpl->critSec));
	delete m_pImpl;
}

void ThreadsafeLogSystem::SetProtectedLogSystem(ILogSystem *pLogSystem)
{
	EnterCriticalSection(&(m_pImpl->critSec));

	if (m_protectedLogger)
		delete m_protectedLogger;
	m_protectedLogger = pLogSystem;

	LeaveCriticalSection(&(m_pImpl->critSec));
}

ELogVerbosity ThreadsafeLogSystem::GetVerbosity() const
{
	return m_eVerbosity;
}

void ThreadsafeLogSystem::SetVerbosity(ELogVerbosity eVerbosity)
{
	EnterCriticalSection(&(m_pImpl->critSec));

	m_eVerbosity = eVerbosity;

	LeaveCriticalSection(&(m_pImpl->critSec));
}

ThreadsafeLogSystem::TVerbosityPeeker ThreadsafeLogSystem::GetVerbosityPeeker() const
{
	return (&m_eVerbosity);
}

void ThreadsafeLogSystem::Log(ELogVerbosity eVerbosity, const char * format, ...)
{
	va_list args;
	va_start(args, format);
	LogInternal(eVerbosity, format, args);
}

void ThreadsafeLogSystem::LogVa(va_list args, ELogVerbosity eVerbosity, const char * format)
{
	LogInternal(eVerbosity, format, args);
}

void ThreadsafeLogSystem::LogInternal(ELogVerbosity eVerbosity, const char * format, va_list args)
{
	if (eVerbosity > m_eVerbosity || eVerbosity == eLV_NEVER) return;

	// Possible bug here in all loggers - should also check against "compile out" value
	EnterCriticalSection(&(m_pImpl->critSec));

	m_protectedLogger->LogVa(args, eVerbosity, format);

	LeaveCriticalSection(&(m_pImpl->critSec));
}
