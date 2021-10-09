#include <pch.h>
#include "L_PickupOwnArrow.h"
#include "Agent/BehaviorAgent.h"

void L_PickupOwnArrow::on_enter()
{
	arrow = agent->get_arrow_on_floor();

	if (arrow)
	{
		BehaviorNode::on_leaf_enter();
	}
	else
	{
		on_failure();
	}

}

void L_PickupOwnArrow::on_update(float dt)
{
	const auto result = agent->move_toward_point(arrow->get_position(), dt);

	if (result == true)
	{
		agent->set_current_ammos(agent->get_current_ammos() + 1);
		arrow->set_inventory(true);
		auto pos = arrow->get_position();
		pos.y = -1000;
		arrow->set_position(pos);
		on_success();
	}

	display_leaf_text();
}
