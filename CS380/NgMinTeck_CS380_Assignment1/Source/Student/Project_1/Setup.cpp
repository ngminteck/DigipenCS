#include <pch.h>
#include "Projects/ProjectOne.h"
#include "Agent/CameraAgent.h"

void ProjectOne::setup()
{
    // Create your inital agents
    auto player = agents->create_behavior_agent("player", BehaviorTreeTypes::Player, 0);
	player->set_position(Vec3(0, 0, 0));
	player->set_color(Vec3(0, 0, 0));
	player->set_movement_speed(10);
	player->set_scaling(Vec3(1, 1, 1));
	player->set_name("player");

	auto enemy_warrior_1 = agents->create_behavior_agent("enemy_warrior_1", BehaviorTreeTypes::EnemyWarrior,1);
	enemy_warrior_1->set_position(RNG::world_position());
	enemy_warrior_1->set_color(Vec3(1, 0, 0));
	enemy_warrior_1->set_movement_speed(10);
	enemy_warrior_1->set_scaling(Vec3(1, 1, 1));
	enemy_warrior_1->set_name("enemy_warrior_1");
	player->add_target_list(enemy_warrior_1);

	auto enemy_warrior_2 = agents->create_behavior_agent("enemy_warrior_2", BehaviorTreeTypes::EnemyWarrior,1);
	enemy_warrior_2->set_position(RNG::world_position());
	enemy_warrior_2->set_color(Vec3(1, 0, 0));
	enemy_warrior_2->set_movement_speed(10);
	enemy_warrior_2->set_scaling(Vec3(1, 1, 1));
	enemy_warrior_2->set_name("enemy_warrior_2");
	player->add_target_list(enemy_warrior_2);

	auto enemy_ranger_1 = agents->create_behavior_agent("enemy_ranger_1", BehaviorTreeTypes::EnemyRanger,2);
	enemy_ranger_1->set_position(RNG::world_position());
	enemy_ranger_1->set_color(Vec3(0, 1, 0));
	enemy_ranger_1->set_movement_speed(10);
	enemy_ranger_1->set_scaling(Vec3(1, 1, 1));
	enemy_ranger_1->set_name("enemy_ranger_1");
	player->add_target_list(enemy_ranger_1);

	auto enemy_ranger_2 = agents->create_behavior_agent("enemy_ranger_2", BehaviorTreeTypes::EnemyRanger,2);
	enemy_ranger_2->set_position(RNG::world_position());
	enemy_ranger_2->set_color(Vec3(0, 1, 0));
	enemy_ranger_2->set_movement_speed(10);
	enemy_ranger_2->set_scaling(Vec3(1, 1, 1));
	enemy_ranger_2->set_name("enemy_ranger_2");
	player->add_target_list(enemy_ranger_2);

	auto enemy_mage_1 = agents->create_behavior_agent("enemy_mage_1", BehaviorTreeTypes::EnemyMage,3);
	enemy_mage_1->set_position(RNG::world_position());
	enemy_mage_1->set_color(Vec3(0, 0, 1));
	enemy_mage_1->set_movement_speed(10);
	enemy_mage_1->set_scaling(Vec3(1, 1, 1));
	enemy_mage_1->set_name("enemy_mage_1");
	player->add_target_list(enemy_mage_1);

	auto enemy_mage_2 = agents->create_behavior_agent("enemy_mage_2", BehaviorTreeTypes::EnemyMage,3);
	enemy_mage_2->set_position(RNG::world_position());
	enemy_mage_2->set_color(Vec3(0, 0, 1));
	enemy_mage_2->set_movement_speed(10);
	enemy_mage_2->set_scaling(Vec3(1, 1, 1));
	enemy_mage_2->set_name("enemy_mage_2");
	player->add_target_list(enemy_mage_2);




    // you can technically load any map you want, even create your own map file,
    // but behavior agents won't actually avoid walls or anything special, unless you code that yourself
    // that's the realm of project 2 though
    terrain->goto_map(0);

    // you can also enable the pathing layer and set grid square colors as you see fit
    // works best with map 0, the completely blank map
    terrain->pathLayer.set_enabled(true);
    terrain->pathLayer.set_value(0, 0, Colors::Red);

    // camera position can be modified from this default as well
    auto camera = agents->get_camera_agent();
    camera->set_position(Vec3(-62.0f, 70.0f, terrain->mapSizeInWorld * 0.5f));
    camera->set_pitch(0.610865); // 35 degrees
}