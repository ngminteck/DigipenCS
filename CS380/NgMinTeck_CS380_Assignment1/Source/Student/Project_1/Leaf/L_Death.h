#pragma once
#include "BehaviorNode.h"

class L_Death : public BaseNode<L_Death>
{


protected:

	virtual void on_enter() override;
	virtual void on_update(float dt) override;
};