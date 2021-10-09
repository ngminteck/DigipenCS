#pragma once
#include "BehaviorNode.h"

class D_CombatMode : public BaseNode<D_CombatMode>
{
public:

protected:
	virtual void on_enter() override;
	virtual void on_update(float dt) override;
};