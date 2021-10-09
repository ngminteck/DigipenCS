#include <pch.h>
#include "L_FindNearbyTarget.h"
#include "Agent/BehaviorAgent.h"

void L_FindNearbyTarget::on_enter()
{


	BehaviorNode::on_leaf_enter();

}

void L_FindNearbyTarget::on_update(float dt)
{
	Agent * my_target = agent->get_nearby_target();


	if (my_target)
	{
		agent->set_current_target(my_target);
		agent->set_mode(true);
		on_success();
	}
	else
	{
		on_failure();
	}

	display_leaf_text();
}

