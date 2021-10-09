#include <pch.h>
#include "L_RandomRespawn.h"
#include "Agent/BehaviorAgent.h"

void L_RandomRespawn::on_enter()
{

	agent->set_pitch(agent->get_pitch() +(90 * PI) / 180);
	pos = RNG::world_position();
	pos.y = agent->get_position().y;
	agent->set_position(pos);
	agent->respawn();

	BehaviorNode::on_leaf_enter();
}

void L_RandomRespawn::on_update(float dt)
{

	on_success();

	display_leaf_text();
}
