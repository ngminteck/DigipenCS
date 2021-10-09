#pragma once

// Include all node headers in this file

// Example Control Flow Nodes
#include "ControlFlow/C_ParallelSequencer.h"
#include "ControlFlow/C_RandomSelector.h"
#include "ControlFlow/C_Selector.h"
#include "ControlFlow/C_Sequencer.h"

// Student Control Flow Nodes
// weight selector array

// Example Decorator Nodes
#include "Decorator/D_Delay.h"
#include "Decorator/D_InvertedRepeater.h"
#include "Decorator/D_RepeatFourTimes.h"

// Student Decorator Nodes
#include "Decorator/D_IsNotAlive.h"
#include "Decorator/D_ZeroHp.h"
#include "Decorator/D_CombatMode.h"
#include "Decorator/D_HaveVaildTarget.h"
#include "Decorator/D_HaveRunes.h"
#include "Decorator/D_HaveAmmo.h"
#include "Decorator/D_Fire.h"


// check target vaild
// check range
// monster radius else retreat

// Example Leaf Nodes
#include "Leaf/L_CheckMouseClick.h"
#include "Leaf/L_Idle.h"
#include "Leaf/L_MoveToFurthestAgent.h"
#include "Leaf/L_MoveToMouseClick.h"
#include "Leaf/L_MoveToRandomPosition.h"

// Student Leaf Nodes
#include "Leaf/L_RandomRespawn.h"
#include "Leaf/L_Death.h"
#include "Leaf/L_CheckKeyBoardKey.h"
#include "Leaf/L_FindNearbyTarget.h"
#include "Leaf/L_Attack.h"
#include "Leaf/L_CombatPursuit.h"
#include "Leaf/L_MoveToPosition.h"
#include "Leaf/L_WanderRandom.h"
#include "Leaf/L_Weapon.h"
#include "Leaf/L_Fire.h"
#include "Leaf/L_PickupOwnArrow.h"

// move to target
// find target
// pick up arrow
// eat food
// teleport
// retreat
