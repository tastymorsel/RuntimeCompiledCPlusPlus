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

#include "Environment.h"

#include "../../Systems/Systems.h"

#include "../../Systems/LogSystem/FileLogSystem/FileLogSystem.h"
#include "../../Systems/LogSystem/MultiLogSystem/MultiLogSystem.h"
#include "../../Systems/LogSystem/RocketLogSystem/RocketLogSystem.h"
#include "../../Systems/LogSystem/ThreadsafeLogSystem/ThreadsafeLogSystem.h"
#include "../../Systems/TimeSystem/TimeSystem.h"
#include "../../Systems/EntitySystem/EntitySystem.h"
#include "../../Systems/AssetSystem/AssetSystem.h"
#include "../../Systems/ObjectFactorySystem//ObjectFactorySystem.h"
#include "../../Systems/GUISystem/GUISystem.h"
#include "../../RuntimeCompiler/FileChangeNotifier.h"


Environment::Environment( IGame* pGame )
{
	sys = new SystemTable();

	// Set the systems library to refer to this system table
	gSys = sys;

	sys->pGame = pGame;

	FileLogSystem *pFileLog = new FileLogSystem();
	pFileLog->SetLogPath("Log.txt");
	pFileLog->SetVerbosity(eLV_COMMENTS);
	pFileLog->Log(eLV_EVENTS, "Started file logger\n");

	RocketLogSystem *pRocketLog = new RocketLogSystem();
	pRocketLog->SetVerbosity(eLV_COMMENTS);
	pRocketLog->Log(eLV_EVENTS,"Started RocketLib logger\n");
	sys->pRocketLogSystem = pRocketLog;

	MultiLogSystem *pMultiLog = new MultiLogSystem();
	pMultiLog->AddLogSystem(pFileLog);
	pMultiLog->AddLogSystem(pRocketLog);

	ThreadsafeLogSystem *pThreadsafeLog = new ThreadsafeLogSystem();
	pThreadsafeLog->SetProtectedLogSystem(pMultiLog);

	sys->pLogSystem = pThreadsafeLog;

	sys->pLogSystem->Log(eLV_EVENTS, "All logs initialised\n");

	sys->pTimeSystem = new TimeSystem();
	sys->pTimeSystem->StartSession();

	sys->pEntitySystem = new EntitySystem();

	sys->pAssetSystem = new AssetSystem();

	sys->pObjectFactorySystem = new ObjectFactorySystem();

	sys->pGUISystem = new GUISystem();

	sys->pFileChangeNotifier = new FileChangeNotifier();
}

Environment::~Environment()
{
	// Reverse order as a rule

	delete sys->pFileChangeNotifier;
	delete sys->pGUISystem;
	delete sys->pObjectFactorySystem;
	delete sys->pAssetSystem;
	delete sys->pEntitySystem;
	delete sys->pTimeSystem;
	delete sys->pLogSystem;
}

