#pragma once
#include "BehaviorNode.h"

class L_CheckKeyBoardKey : public BaseNode<L_CheckKeyBoardKey>
{
protected:
	virtual void on_update(float dt);
};