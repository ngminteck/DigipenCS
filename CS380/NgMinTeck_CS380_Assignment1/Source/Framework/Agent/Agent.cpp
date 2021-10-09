/******************************************************************************/
/*!
\file		Agent.cpp
\project	CS380/CS580 AI Framework
\author		Dustin Holmes
\summary	Base actor implementation

Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include <pch.h>
#include "Agent.h"

const Vec3 globalUp = Vec3(0.0f, 1.0f, 0.0f);
const Vec3 globalRight = Vec3(-1.0f, 0.0f, 0.0f);
const Vec3 globalForward = Vec3(0.0f, 0.0f, 1.0f);

// TODO: move this temp model stuff to renderer in some capacity, if time permits
std::unique_ptr<DirectX::Model> Agent::model;

Agent::Agent(const char *type, size_t id) : position(0.0f, 0.0f, 0.0f),
    scaling(3.0f, 3.0f, 3.0f), eulerAngles(0.0f, 0.0f, 0.0f), isDirty(true), color(0.7f, 0.7f, 0.7f), type(type), id(id), movementSpeed(2000.0f / 2.3f), healthPoints{100}
{}

#pragma region Getters

const Vec3 &Agent::get_position() const
{
    return position;
}

const Vec3 &Agent::get_scaling() const
{
    return scaling;
}

Vec3 Agent::get_forward_vector() const
{
    const auto rotation = Quat::CreateFromYawPitchRoll(eulerAngles.x, eulerAngles.y, eulerAngles.z);
    return Vec3::Transform(globalForward, rotation);
}

Vec3 Agent::get_right_vector() const
{
    const auto rotation = Quat::CreateFromYawPitchRoll(eulerAngles.x, eulerAngles.y, eulerAngles.z);
    return Vec3::Transform(globalRight, rotation);
}

Vec3 Agent::get_up_vector() const
{
    const auto rotation = Quat::CreateFromYawPitchRoll(eulerAngles.x, eulerAngles.y, eulerAngles.z);
    return Vec3::Transform(globalUp, rotation);
}

float Agent::get_yaw() const
{
    return eulerAngles.x;
}

float Agent::get_pitch() const
{
    return eulerAngles.y;
}

float Agent::get_roll() const
{
    return eulerAngles.z;
}

const Vec3 &Agent::get_color() const
{
    return color;
}

const char *Agent::get_type() const
{
    return type;
}

const size_t &Agent::get_id() const
{
    return id;
}

float Agent::get_movement_speed() const
{
    return movementSpeed;
}

const Mat4 &Agent::get_local_to_world()
{
    if (isDirty == true)
    {
        build_transformation();
        isDirty = false;
    }

    return localToWorld;
}

#pragma endregion

#pragma region Setters
void Agent::set_position(const Vec3 &pos)
{
    position = pos;
    isDirty = true;
}

void Agent::set_scaling(const Vec3 &scale)
{
    scaling = scale;
    isDirty = true;
}

void Agent::set_scaling(float scalar)
{
    scaling = Vec3(scalar, scalar, scalar);
    isDirty = true;
}

void Agent::set_yaw(float angleRadians)
{
    eulerAngles.x = angleRadians;
    isDirty = true;
}

void Agent::set_pitch(float angleRadians)
{
    eulerAngles.y = angleRadians;
    isDirty = true;
}

void Agent::set_roll(float angleRadians)
{
    eulerAngles.z = angleRadians;
    isDirty = true;
}

void Agent::set_color(const Vec3 &newColor)
{
    color = newColor;
}

void Agent::set_movement_speed(float speed)
{
    movementSpeed = speed;
}
#pragma endregion

void Agent::update(float dt)
{}

void Agent::draw_mesh()
{
    model->UpdateEffects([this](DirectX::IEffect *effect)
    {
        auto basic = dynamic_cast<DirectX::BasicEffect *>(effect);
        basic->SetDiffuseColor(color);
    });
    
    auto &resources = renderer->get_resources();
    model->Draw(resources.get_context(), *resources.get_states(), get_local_to_world(), renderer->get_view_matrix(), renderer->get_projection_matrix());
}

void Agent::draw_debug()
{
    // no op
}

bool Agent::acquire_rendering_resources()
{
    std::cout << "Loading Agent model..." << std::endl;
    auto device = renderer->get_resources().get_device();
    auto factory = renderer->get_resources().get_effect_factory();
    try
    {
        model = DirectX::Model::CreateFromSDKMESH(device, L"Assets\\mannequin_rig.sdkmesh", *factory);
    }
    catch (const std::exception &err)
    {
        std::cout << "Unable to load Agent model: " << err.what() << std::endl;
        return false;
    }

    model->UpdateEffects([](DirectX::IEffect *effect)
    {
        auto basic = dynamic_cast<DirectX::BasicEffect*>(effect);
        if (basic)
        {
            basic->SetLightingEnabled(true);
            basic->SetPerPixelLighting(true);
            basic->SetLightEnabled(0, true);
            basic->SetLightDiffuseColor(0, DirectX::Colors::White);

            basic->SetLightEnabled(1, false);
            basic->SetLightEnabled(2, false);
        }
    });

    return true;
}

void Agent::release_rendering_resources()
{
    model.reset();
}

void Agent::update_model(const Mat4 &view, const Mat4 &proj)
{
    model->UpdateEffects([view, proj](DirectX::IEffect *effect)
    {
        auto basic = dynamic_cast<DirectX::BasicEffect *>(effect);
        basic->SetView(view);
        basic->SetProjection(proj);
    });
}

void Agent::build_transformation()
{
    const auto translationMatrix = Mat4::CreateTranslation(position);
    const auto scalingMatrix = Mat4::CreateScale(scaling * globalScalar);
    const auto rotation = Quat::CreateFromYawPitchRoll(eulerAngles.x, eulerAngles.y, eulerAngles.z);
    const auto rotationMatrix = Mat4::CreateFromQuaternion(rotation);

    localToWorld = scalingMatrix * rotationMatrix * translationMatrix;
}

int Agent::get_role()const
{
	return role;
}

void Agent::set_role(int ur_role)
{
	role = ur_role;
	combat = false;
	switch(role)
	{
		case 0: // player 
			equipment = 1; //0 bare hand, 1 abyss whip , 2 crystal bow, 3 ancient staff
			maxHealthPoints = 99; // max hp
			energy = 99;
			maxAmmo = 5000; // crystal bow limit
			maxRune = 166;  // max rune stack 999, 999/ 6 water rune = 166 times
			maxFood = 22; // 28 space - crystal bow, ancient staff, blood, death, water rune and i slot for shield
			spawnTime = 3.0f;
			rangeLimit = 5.0f;
			break;

		case 1: // melee enemy 
			equipment = 1; // rune scimitar
			maxHealthPoints = 99; // max hp
			energy = 99;
			maxAmmo = 0; // don't use bow
			maxRune = 0;  // no runes
			maxFood = 0; // no food
			spawnTime = 15.0f;
			rangeLimit = 5.0f;
			break;

		case 2: // range enemy 
			equipment = 2; // rune scimitar
			maxHealthPoints = 99; // max hp
			energy = 99;
			maxAmmo = 3; // bronze arrow can pick up
			maxRune = 0;  // no runes
			maxFood = 0; // no food
			spawnTime = 15.0f;
			rangeLimit = 20.0f;
			break;

		case 3: // mage enemy 
			equipment = 3; // rune scimitar
			maxHealthPoints = 99; // max hp
			energy = 99;
			maxAmmo = 0; //max bronze arrow
			maxRune = 3;  // 20 time fire strike
			maxFood = 0; // no food
			spawnTime = 15.0f;
			rangeLimit = 15.0f;
			break;
	}

	running = 1.0f;
	alive = true;
	curTime = spawnTime;
	target = nullptr;

	currentAmmo = maxAmmo;
	currentRune = maxRune;
}

int Agent::get_health() const
{
	return healthPoints;
}

void Agent::set_health( int hp)
{
	healthPoints = hp;

	if (healthPoints < 0)
		healthPoints = 0;
}

bool Agent::get_status() const
{
	return alive;
}

void Agent::set_status(bool value)
{
	alive = value;
}

bool Agent::get_mode()const
{
	return combat;
}


void Agent::respawn()
{
	healthPoints = maxHealthPoints;
	energy = 99;
	currentAmmo = maxAmmo;
	currentRune = maxRune;
	currentFood = maxFood;
	target = nullptr;

	combat = false;
	running = 1.0f;

	if (healthPoints <= 0)
		healthPoints = 99;

	if (role == 0)
	{
		equipment = 1;
		rangeLimit = 5.0f;
	}

	if (role == 2)
	{
		for (auto it = ammo_list.begin(); it != ammo_list.end(); )
		{
			if (*it)
			{
				if (!(*it)->inInventory)
				{
					(*it)->set_inventory(true);
					(*it)->set_position(Vec3(0, -1000, 0));

				}
				++it;
			}
			else
			{
				it = ammo_list.erase(it);
			}
		}

	}

	alive = true;
}

void Agent::add_target_list(Agent * target)
{
	if (target)
	{
		if (target->role == 1 || target->role == 2 || target->role == 3)
		{
			bool not_found = true;
			for (const auto & elem : target_list)
			{
				if (elem == target)
				{
					not_found = false;
					break;
				}
			}

			if (not_found)
				target_list.push_back(target);
		}
	}
}

Agent * Agent::get_nearby_target()
{
	Agent * target = nullptr;
	unsigned int shortest_distance = UINT_MAX;

	for (auto it = target_list.begin(); it != target_list.end(); )
	{
		if (*it)
		{
			if ((*it)->get_status())
			{
				unsigned int distance = static_cast<unsigned int>(abs(Vec3::Distance(position, (*it)->get_position())));
				if (shortest_distance > distance)
				{
					shortest_distance = distance;
					target = *it;
				}
			}
			++it;
		}
		else
		{
			it = target_list.erase(it);
		}
	}
	return target;
}

int Agent::get_current_weapon()const
{
	return equipment;
}

void Agent::set_current_weapon(int value)
{
	equipment = value;
}

float Agent::get_range_limit()const
{
	return rangeLimit;
}

void Agent::set_range_limit(float limit)
{
	rangeLimit = limit;
}

Agent *  Agent::get_current_target()const
{
	return target;
}

void Agent::set_current_target(Agent* new_target)
{
	target = new_target;
}

void Agent::set_mode(bool value)
{
	combat = value;
}

int Agent::get_current_ammos()const
{
	return currentAmmo;
}

int Agent::get_current_runes()const
{
	return currentRune;
}

void  Agent::set_current_ammos(int value)
{
	currentAmmo = value;

	if (currentAmmo < 0)
		currentAmmo = 0;
}

void  Agent::set_current_runes(int value)
{
	currentRune = value;

	if (currentRune < 0)
		currentRune = 0;
}

std::string Agent::get_name()const
{
	return name;
}

void Agent::set_name(std::string value)
{
	name = value;
}

Agent *  Agent::get_master()const
{
	return master;
}

Agent *  Agent::get_weapon()
{
	return weapon;
}

void Agent::set_weapon(Agent * wep)
{
	weapon = wep;
}

void Agent::set_master(Agent* mas)
{
	master = mas;
}


Agent *  Agent::get_arrow()
{
	Agent * arrow = nullptr;

	for (auto it = ammo_list.begin(); it != ammo_list.end();  )
	{
		if (*it)
		{
			if ((*it)->inInventory && !(*it)->fire)
			{
				arrow = *it;
				return arrow;
				
			}
			++it;
		}
		else
		{
			it = ammo_list.erase(it);
		}
	}

	return arrow;
}

Agent * Agent::get_arrow_on_floor()
{
	Agent * arrow = nullptr;

	for (auto it = ammo_list.begin(); it != ammo_list.end(); )
	{
		if (*it)
		{
			if (!(*it)->inInventory)
			{
				arrow = *it;
				return arrow;

			}
			++it;
		}
		else
		{
			it = ammo_list.erase(it);
		}
	}

	return arrow;
}

Agent * Agent::get_spell()
{
	Agent * spell = nullptr;

	for (auto it = spell_list.begin(); it != spell_list.end(); )
	{
		if (*it)
		{
			if ((*it)->inInventory && !(*it)->fire)
			{
				spell = *it;
				return spell;

			}
			++it;
		}
		else
		{
			it = ammo_list.erase(it);
		}
	}

	return spell;
}

void Agent::add_arrow(Agent * arrow)
{
	if (arrow)
	{
		ammo_list.push_back(arrow);
	}
}

void Agent::add_spell(Agent * spell)
{
	if (spell)
	{
		spell_list.push_back(spell);
	}
}

bool Agent::get_inventory()const
{
	return inInventory;
}

void  Agent::set_inventory(bool value)
{
	inInventory = value;
}

bool  Agent::get_fire()const
{
	return fire;
}

void Agent::set_fire(bool value)
{
	fire = value;
}