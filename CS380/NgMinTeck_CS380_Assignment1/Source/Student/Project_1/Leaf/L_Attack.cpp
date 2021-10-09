#include <pch.h>
#include "L_Attack.h"
#include "Agent/BehaviorAgent.h"

void L_Attack::on_enter()
{
	if (agent->get_current_target())
	{
		BehaviorNode::on_leaf_enter();
	}
	else
	{
		on_failure();
	}
	
}

void L_Attack::on_update(float dt)
{
	
		int damage = 0;

		if (agent->get_role() == 0)
		{
			if (agent->get_current_weapon() ==1 )
			{
				damage = 20;
			}
			else if (agent->get_current_weapon() == 2)
			{
				damage = 20;
				Agent * arrow = agent->get_arrow();

				if (arrow)
				{
					arrow->set_position(agent->get_position());
					arrow->set_current_target(agent->get_current_target());
					arrow->set_fire(true);
				}

			}
			else if (agent->get_current_weapon() == 3)
			{
				damage = 20;

				Agent * spell = agent->get_spell();

				if (spell)
				{
					spell->set_position(agent->get_position());
					spell->set_current_target(agent->get_current_target());
					spell->set_fire(true);
				}
			}
			else
			{
				damage = 20;
			}
		}
		else if (agent->get_role() == 1)
		{
			damage = 12;
		}
		else if (agent->get_role() == 2)
		{
			damage = 8;
			agent->set_current_ammos(agent->get_current_ammos() - 1);
			Agent * arrow = agent->get_arrow();

			if (arrow)
			{
				arrow->set_yaw(agent->get_yaw());
				arrow->set_pitch(agent->get_pitch());
				arrow->set_roll(agent->get_roll());
				arrow->set_position(agent->get_position());
				arrow->set_current_target(agent->get_current_target());
				arrow->set_inventory(false);
				arrow->set_fire(true);
			}

		}
		else if (agent->get_role() == 3)
		{
			damage = 9;
			agent->set_current_runes(agent->get_current_runes() - 1);

			Agent * spell = agent->get_spell();

			if (spell)
			{
				spell->set_position(agent->get_position());
				spell->set_current_target(agent->get_current_target());
				spell->set_fire(true);
			}
		}


		
		agent->get_current_target()->set_health(agent->get_current_target()->get_health() - damage);
		agent->get_current_target()->set_mode(true);
		agent->get_current_target()->set_current_target(agent);
		on_success();
	


	display_leaf_text();
}