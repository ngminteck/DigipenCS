#include <pch.h>
#include "D_ZeroHp.h"

void D_ZeroHp::on_enter()
{
	if (agent->get_health() <= 0)
	{
		BehaviorNode::on_enter();
	}
	else
	{
		on_failure();
	}
}

void D_ZeroHp::on_update(float dt)
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