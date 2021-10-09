#include <pch.h>
#include "L_Fire.h"
#include "Agent/BehaviorAgent.h"

void L_Fire::on_enter()
{
	if (agent->get_current_target())
	{
		pos = agent->get_current_target()->get_position();
		pos.y += 3.0f;
		agent->set_pitch(-(90 * PI) / 180);
		agent->set_roll((180 * PI) / 180);
		auto spos = agent->get_position();
		spos.y += 3.0f;
		agent->set_position(spos);
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

void L_Fire::on_update(float dt)
{
	
		auto result = agent->move_toward_point(pos , dt);

		if (result == true)
		{
			agent->set_fire(false);
			agent->set_current_target(nullptr);

			Agent * owner = agent->get_master();

			if (owner->get_role() == 2)
			{
				auto pos = agent->get_position();
				pos.y = 0;
				agent->set_position(pos);
			}
			else
			{
				agent->set_position(Vec3(0, -1000, 0));
			}
			on_success();
		}
	

	display_leaf_text();
}