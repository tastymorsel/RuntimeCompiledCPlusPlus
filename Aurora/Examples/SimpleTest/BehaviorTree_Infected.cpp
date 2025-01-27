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

#include "IBehaviorTree.h"
#include "IGameObject.h"
#include "IBlackboard.h"
#include "IBlackboardManager.h"
#include "IObjectUtils.h"

#include "BB_Global.h"
#include "BB_Team_Infection.h"
#include "BB_Group_Infected.h"
#include "BB_Individual_Infected.h"
#include "BB_Individual_Common.h"

#include "../../RunTimeCompiler/ObjectInterfacePerModule.h"
#include "../../Systems/SystemTable.h"
#include "../../Systems/IEntitySystem.h"
#include "../../Systems/IAssetSystem.h"
#include "../../Systems/ILogSystem.h"
#include "../../Systems/ISimpleSerializer.h"

#include <assert.h>


class BehaviorTree_Infected : public IBehaviorTree
{
public:

	virtual void Execute( IGameObject* pGameObject )
	{
		BB_Global* pBBGlobal = (BB_Global*)m_pBBManager->GetBlackboardGlobal();
		BB_Team_Infection* pBBTeam = (BB_Team_Infection*)m_pBBManager->GetBlackboardTeam( pGameObject->GetGameTeam() );
		BB_Group_Infected* pBBGroup = (BB_Group_Infected*)m_pBBManager->GetBlackboardGroup( pGameObject->GetGameObjectType() );
		BB_Individual_Infected* pBBIndividual = (BB_Individual_Infected*)m_pBBManager->GetBlackboardIndividual( pGameObject );
		BB_Individual_Common* pBBCommon = (BB_Individual_Common*)m_pBBManager->GetBlackboardIndividualCommon( pGameObject );

		if ( pBBCommon->enemy_collision_objectid.IsValid() )
		{
			pGameObject->SetBehavior( "Behavior_Infected_Combat" );
		}
		else if ( !pBBCommon->target_position.IsInfinite() )
		{
			pGameObject->SetBehavior( "Behavior_Infected_Approach" );
		}
		else if ( pBBGlobal->infection_team_strength > pBBGlobal->immune_team_strength * 1.2f && pBBGroup->group_size > 1 )
		{
			pGameObject->SetBehavior( "Behavior_Infected_HuntWBC" );
		}
		else if ( pBBGlobal->immune_count > 0 )
		{
			pGameObject->SetBehavior( "Behavior_Infected_HuntRBC" );
		}
		else
		{
			pGameObject->SetBehavior( "Behavior_Infected_Idle" );
		}	
	}

	virtual void Init( bool isFirstInit )
	{
		m_pBBManager = (IBlackboardManager*)IObjectUtils::GetUniqueInterface( "BlackboardManager", IID_IBLACKBOARDMANAGER );
	}

private:

	IBlackboardManager* m_pBBManager;
};

REGISTERCLASS(BehaviorTree_Infected);