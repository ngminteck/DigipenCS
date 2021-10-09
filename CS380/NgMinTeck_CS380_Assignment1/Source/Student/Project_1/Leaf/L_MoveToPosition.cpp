#include <pch.h>
#include "L_MoveToPosition.h"
#include "Agent/BehaviorAgent.h"

void L_MoveToPosition::on_enter()
{
	BehaviorNode::on_leaf_enter();
}

void L_MoveToPosition::on_update(float dt)
{
	if (agent->get_current_target())
	{
		const auto result = agent->move_toward_point(agent->get_current_target()->get_position(), dt);

		if (result == true)
		{
			on_success();
		}
	}
	else
	{
		on_failure();
	}
	

	display_leaf_text();
}