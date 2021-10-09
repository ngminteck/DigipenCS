#include <pch.h>
#include "D_IsNotAlive.h"

void D_IsNotAlive::on_enter()
{
	
	if (agent->get_status())
	{
		on_failure();
	}
	else
	{
		BehaviorNode::on_enter();
	}
}


void D_IsNotAlive::on_update(float dt)
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