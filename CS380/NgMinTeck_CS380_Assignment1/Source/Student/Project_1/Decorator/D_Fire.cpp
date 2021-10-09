#include <pch.h>
#include "D_Fire.h"

void D_Fire::on_enter()
{

	if (agent->get_fire())
	{
		if (agent->get_current_target())
		{
			BehaviorNode::on_enter();
		}
		else
		{
			agent->set_fire(false);
			agent->set_inventory(true);
			agent->set_position(Vec3(0, -1000, 0));
			on_failure();
		}
	}
	else
	{
		on_failure();
	}
}


void D_Fire::on_update(float dt)
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