#include <pch.h>
#include "D_HaveRunes.h"

void D_HaveRunes::on_enter()
{
	BehaviorNode::on_enter();
}



void D_HaveRunes::on_update(float dt)
{

	if (agent->get_role() == 0)
	{
		on_success();
	}
	else
	{
		int runes = agent->get_current_runes();

		if (runes <= 0)
		{
			on_failure();
		}
		else
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
	}
}