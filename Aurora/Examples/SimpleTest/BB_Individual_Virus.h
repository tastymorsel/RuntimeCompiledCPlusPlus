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

#ifndef BB_INDIVIDUAL_VIRUS_INCLUDED
#define BB_INDIVIDUAL_VIRUS_INCLUDED

#include "IBlackboard.h"
#include "../../Systems/ISimpleSerializer.h"
#include <assert.h>


struct BB_Individual_Virus : public IBlackboard
{
	BB_Individual_Virus()
	{
		patrol_change_frequency = 10.0f;
	}

	virtual void Serialize(ISimpleSerializer *pSerializer)
	{
		AU_ASSERT(pSerializer);
		
		SERIALIZE(visible_wbc);
		SERIALIZE(visible_rbc);
		SERIALIZE(patrol_change_frequency);
	}

	// Members

	AUDynArray<ObjectId> visible_wbc;
	AUDynArray<ObjectId> visible_rbc;
	float patrol_change_frequency;
};

// Registered inside BlackboardManager.cpp
// REGISTERCLASS(BB_Individual_Virus);


#endif // BB_INDIVIDUAL_VIRUS_INCLUDED