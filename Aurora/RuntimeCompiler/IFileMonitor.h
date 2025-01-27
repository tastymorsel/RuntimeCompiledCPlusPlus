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

#ifndef IFILEMONITOR_INCLUDED
#define IFILEMONITOR_INCLUDED

#define BOOST_FILESYSTEM_VERSION 3
#include "boost/filesystem.hpp"   // includes all needed Boost.Filesystem declarations


struct IFileMonitorListener
{
	virtual void OnFileChange( const boost::filesystem::path& filename ) = 0;
};


struct IFileMonitor
{
	virtual void Update( float fTimeDelta ) = 0;
	virtual void Watch( const boost::filesystem::path& filename, IFileMonitorListener *pListener=0 ) = 0; // can be file or directory
	virtual void Watch( const char *filename, IFileMonitorListener *pListener=0 ) = 0; // can be file or directory
};

#endif // IFILEMONITOR_INCLUDED