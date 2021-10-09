#include <pch.h>
#include "L_CombatPursuit.h"
#include "Agent/BehaviorAgent.h"

void L_CombatPursuit::on_enter()
{


	if(agent->get_current_target())
	{
		BehaviorNode::on_leaf_enter();
	}
	else
	{
		on_failure();
	}



}

void L_CombatPursuit::on_update(float dt)
{
	const auto result = agent->move_toward_point_with_radius(agent->get_current_target()->get_position(), dt,agent->get_range_limit());

	if (result == true)
	{
		on_success();
	}

	display_leaf_text();
}
