#pragma once
#include "BehaviorNode.h"

class L_PickupOwnArrow : public BaseNode<L_PickupOwnArrow>
{
protected:
	virtual void on_enter() override;
	virtual void on_update(float dt) override;

private:
	Agent * arrow = nullptr;
};