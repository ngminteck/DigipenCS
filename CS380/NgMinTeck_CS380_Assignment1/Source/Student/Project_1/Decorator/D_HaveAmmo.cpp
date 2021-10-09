#include <pch.h>
#include "D_HaveAmmo.h"

void D_HaveAmmo::on_enter()
{
	if (agent->get_role() == 0)
	{
		BehaviorNode::on_enter();
	}
	else
	{
		int ammo = agent->get_current_ammos();

		std::cout << "arrow:" << ammo << std::endl;

		if (ammo <= 0)
		{
			on_failure();
		}
		else
		{
			std::cout << "run1" << std::endl;
			BehaviorNode::on_enter();
		}
	}
}


void D_HaveAmmo::on_update(float dt)
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