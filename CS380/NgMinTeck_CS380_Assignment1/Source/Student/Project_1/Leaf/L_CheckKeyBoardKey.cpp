#include <pch.h>
#include "L_CheckKeyBoardKey.h"
#include "../Agent/BehaviorAgent.h"

void L_CheckKeyBoardKey::on_update(float dt)
{
	const auto keyboardState1 = InputHandler::get_current_state(KBKeys::ONE);
	const auto keyboardState2 = InputHandler::get_current_state(KBKeys::TWO);
	const auto keyboardState3 = InputHandler::get_current_state(KBKeys::THREE);

	if (keyboardState1 == InputHandler::InputState::PRESSED)
	{
		agent->set_current_weapon(1);
		agent->set_range_limit(5.0f);
		if(agent->get_weapon())
			agent->get_weapon()->set_color(Vec3(0, 0, 0));
	}

	if (keyboardState2 == InputHandler::InputState::PRESSED)
	{
		agent->set_current_weapon(2);
		agent->set_range_limit(20.0f);
		if (agent->get_weapon())
			agent->get_weapon()->set_color(Vec3(1, 1, 1));
	}

	if (keyboardState3 == InputHandler::InputState::PRESSED)
	{
		agent->set_current_weapon(3);
		agent->set_range_limit(15.0f);
		if (agent->get_weapon())
			agent->get_weapon()->set_color(Vec3(1, 0, 0.5));
	}

	on_success();

	display_leaf_text();
}
