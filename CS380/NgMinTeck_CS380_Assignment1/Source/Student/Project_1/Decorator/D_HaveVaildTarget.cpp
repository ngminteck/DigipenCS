#include <pch.h>
#include "D_HaveVaildTarget.h"

void D_HaveVaildTarget::on_enter()
{
	if (agent->get_current_target())
	{
		BehaviorNode::on_enter();
	}
	else
	{
		on_failure();
	}
}

void D_HaveVaildTarget::on_update(float dt)
{
		if (agent->get_current_target()->get_status())
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
		else
		{
			agent->set_current_target(nullptr);
			agent->set_mode(false);
			on_failure();

		}
	

}