/******************************************************************************/
/*!
\file		Agent.h
\project	CS380/CS580 AI Framework
\author		Dustin Holmes
\summary	Base actor declarations

Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#pragma once
#include "../Misc/NiceTypes.h"

// forward declarations
class AgentOrganizer;

class Agent
{
    friend class AgentOrganizer;
public:
    Agent(const char *type, size_t id);

#pragma region Getters
    const Vec3 &get_position() const;
    const Vec3 &get_scaling() const;

    Vec3 get_forward_vector() const;
    Vec3 get_right_vector() const;
    Vec3 get_up_vector() const;

    // in radians
    float get_yaw() const;
    float get_pitch() const;
    float get_roll() const;

    const Vec3 &get_color() const;
    
    const Mat4 &get_local_to_world();

    const char *get_type() const;
    const size_t &get_id() const;

    float get_movement_speed() const;

	int get_role()const;
	bool get_status()const;
	bool get_mode()const;
    int get_health() const;

	int get_current_weapon()const;
	float get_range_limit()const;

	Agent * get_current_target()const;
	Agent * get_nearby_target();

	int get_current_ammos()const;
	int get_current_runes()const;

	std::string get_name()const;

	Agent * get_weapon();
	Agent * get_master()const;

	Agent * get_arrow();
	Agent * get_arrow_on_floor();
	Agent * get_spell();

	bool get_inventory()const;

	bool get_fire()const;


#pragma endregion

#pragma region Setters
    void set_position(const Vec3 &pos);
    void set_scaling(const Vec3 &scale);
    void set_scaling(float scalar);

    void set_yaw(float angleRadians);
    void set_pitch(float angleRadians);
    void set_roll(float angleRadians);

    void set_color(const Vec3 &newColor);
    
    void set_movement_speed(float speed);

	void set_role(int ur_role = 0);
	void set_status(bool value);
	void set_health(int hp);
	void respawn();

	void set_current_weapon(int value);
	void set_range_limit(float limit);

	void set_mode(bool value);
	void set_current_target(Agent* new_target);

	void add_target_list(Agent * target);

	void set_current_ammos(int value);
	void set_current_runes(int value);

	void set_name(std::string value);

	void set_weapon(Agent * wep);
	void set_master(Agent* mas);

	void add_arrow(Agent * arrow);
	void add_spell(Agent * spell);

	void set_inventory(bool value);

	void set_fire(bool value);

#pragma endregion

    virtual void update(float dt);

private:
    Vec3 position;
    Vec3 scaling;
    Vec3 eulerAngles;

    Mat4 localToWorld;
    bool isDirty;

    Vec3 color;
    
    const char *type;
    const size_t id;

    float movementSpeed;

	int role;
	int equipment;
	bool combat;
	bool alive;
	float spawnTime;
	float curTime;

	int healthPoints;
	int maxHealthPoints;

	int energy;
	float running;
	
	int currentAmmo;
	int maxAmmo;
	int currentRune;
	int maxRune;

	int currentFood;
	int maxFood;

	float rangeLimit;

	std::string name;

	Agent * target;

	std::vector<Agent* > target_list;


	Agent * weapon;
	Agent * master;
	bool inInventory;
	std::vector< Agent*> ammo_list;
	std::vector< Agent*> spell_list;

	bool fire;
	

    static std::unique_ptr<DirectX::Model> model;

    void build_transformation();

    void draw_mesh();
    virtual void draw_debug();
    static bool acquire_rendering_resources();
    static void release_rendering_resources();
    static void update_model(const Mat4 &view, const Mat4 &proj);
};