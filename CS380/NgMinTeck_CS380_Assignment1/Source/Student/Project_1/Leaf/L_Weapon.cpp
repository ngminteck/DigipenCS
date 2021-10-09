#include <pch.h>
#include "L_Weapon.h"
#include "Agent/BehaviorAgent.h"



void L_Weapon::on_update(float dt)
{
	if (agent->get_master())
	{
		agent->set_yaw(agent->get_master()->get_yaw());
		agent->set_pitch(agent->get_master()->get_pitch() - (135 * PI) / 180);
		agent->set_roll(agent->get_master()->get_roll() + (180 * PI) / 180);

		agent->set_position(agent->get_master()->get_position());
		auto getpos = agent->get_position();
		getpos.y += 3.5f;
		getpos.x -= 2.0f;
		getpos.z += 0.7f;
		
		agent->set_position(getpos);
	   
	}

	on_success();

}