#include <pch.h>
#include "D_Delay.h"

D_Delay::D_Delay() : delay(0.0f)
{}

void D_Delay::on_enter()
{
	if (agent->get_role() == 0)
	{
		delay = 3.0f;
	}
	else
	{
		delay = 3.0f;
	}

    BehaviorNode::on_enter();
}

void D_Delay::on_update(float dt)
{
    delay -= dt;

    if (delay < 0.0f)
    {
        BehaviorNode *child = children.front();

        child->tick(dt);

        // assume same status as child
        set_status(child->get_status());
        set_result(child->get_result());
    }
}
