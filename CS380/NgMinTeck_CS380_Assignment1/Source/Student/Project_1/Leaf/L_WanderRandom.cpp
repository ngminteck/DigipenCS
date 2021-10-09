#include <pch.h>
#include "L_WanderRandom.h"
#include "Agent/BehaviorAgent.h"

void L_WanderRandom::on_enter()
{
	// set animation, speed, etc
	targetPoint = RNG::world_position();
	//agent->look_at_point(targetPoint);

	BehaviorNode::on_leaf_enter();
}

void L_WanderRandom::on_update(float dt)
{
	 auto result = agent->move_toward_point(targetPoint, dt);

	if (agent->get_current_target())
	{
		
		result = true;
	}


	if (result == true)
	{
		on_success();
	}

	display_leaf_text();
}
