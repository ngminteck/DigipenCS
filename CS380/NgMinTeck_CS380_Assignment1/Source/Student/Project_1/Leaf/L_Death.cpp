#include <pch.h>
#include "L_Death.h"

void L_Death::on_enter()
{
	BehaviorNode::on_leaf_enter();
	agent->set_pitch(agent->get_pitch() - (90 * PI) / 180);
	agent->set_status(false);
	agent->set_mode(false);
	agent->set_current_target(nullptr);
}

void L_Death::on_update(float dt)
{
	
	on_success();
	display_leaf_text();
}
