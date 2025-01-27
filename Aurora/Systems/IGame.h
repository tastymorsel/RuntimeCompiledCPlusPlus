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

#ifndef IGAME_INCLUDED
#define IGAME_INCLUDED

#include "ISystem.h"

// IGame Interface
//  Contains game functions games should override
struct IGame : public ISystem
{
	virtual void CompileAll( bool bForceRecompile ) = 0;
	virtual void Reset() = 0;
	virtual void Restart() = 0;
	virtual void ToggleConsoleGUI() = 0;
	virtual void Exit() = 0;
	virtual void GetWindowSize( float& width, float& height ) const = 0;
	virtual void AddToRuntimeFileList( const char* filename ) = 0;
	virtual void RemoveFromRuntimeFileList( const char* filename ) = 0;

	//temporary stuff which should move
	virtual void SetVolume( float volume ) = 0;
	virtual void SetSpeed( float speed ) = 0; 
	virtual void SetAutoCompile( bool autoCompile ) = 0;
};

#endif //IGAME_INCLUDED