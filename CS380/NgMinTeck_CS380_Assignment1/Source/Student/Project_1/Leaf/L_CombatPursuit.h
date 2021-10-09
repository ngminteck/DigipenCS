#pragma once
#include "BehaviorNode.h"
#include "Misc/NiceTypes.h"

class L_CombatPursuit : public BaseNode<L_CombatPursuit>
{
protected:
	virtual void on_enter() override;
	virtual void on_update(float dt) override;

private:

};