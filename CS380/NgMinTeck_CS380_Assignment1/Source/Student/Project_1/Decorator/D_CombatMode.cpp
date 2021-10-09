#include <pch.h>
#include "D_CombatMode.h"

void D_CombatMode::on_enter()
{
	BehaviorNode::on_enter();
}


void D_CombatMode::on_update(float dt)
{
	if (agent->get_mode())
	{
		BehaviorNode *child = children.front();
		child->tick(dt);

		if (child->succeeded() == true)
		{
			on_success();
		}
		else if (child->failed() == true)
		{
			on_failure();
		}

	}
	else
	{
		on_failure();
	}
}