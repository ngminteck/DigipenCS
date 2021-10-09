#pragma once
#include "BehaviorNode.h"

class L_FindNearbyTarget : public BaseNode<L_FindNearbyTarget>
{
protected:
	virtual void on_enter() override;
	virtual void on_update(float dt) override;

private:

};