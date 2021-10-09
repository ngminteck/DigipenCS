#include <pch.h>
#include "Terrain/TerrainAnalysis.h"
#include "Terrain/MapMath.h"
#include "Agent/AStarAgent.h"
#include "Terrain/MapLayer.h"
#include "Projects/ProjectThree.h"

#include <iostream>

bool ProjectThree::implemented_fog_of_war() const // extra credit
{
    return false;
}

float distance_to_closest_wall(int row, int col)
{
    /*
        Check the euclidean distance from the given cell to every other wall cell,
        as well as the distance to the edges (they count as walls for this),
        and return the smallest distance.  Make use of the is_wall member function
        in the global terrain to determine if a cell is a wall or not.
    */

    // WRITE YOUR CODE HERE

	float smallest_distance = std::numeric_limits<float>::max();

	for (int x = -1; x <= terrain->get_map_width(); ++x)
	{
		for (int y = -1; y <= terrain->get_map_height(); ++y)
		{
			if (!terrain->is_valid_grid_position(x,y) || terrain->is_wall(x, y))
			{
				float row_dif = static_cast<float>(abs(x - row));
				float col_dif = static_cast<float>(abs(y - col));

				float distance = static_cast<float>(sqrt(row_dif * row_dif + col_dif * col_dif));

				if (distance < smallest_distance)
					smallest_distance = distance;

			}
		}
	}

    return smallest_distance; // REPLACE THIS
}

bool is_clear_path(int row0, int col0, int row1, int col1)
{
    /*
        Two cells (row0, col0) and (row1, col1) are visible to each other 

		if a line between their centerpoints doesn't intersect the four boundary lines of every
        wall cell.  
		
		You should puff out the four boundary lines by a very tiny amount
        so that a diagonal line passing by the corner will intersect it. 
		
		Make use of the line_intersect helper function for the intersection test and the is_wall member
        function in the global terrain to determine if a cell is a wall or not.
    */
	// WRITE YOUR CODE HERE

	if (terrain->is_wall(row0, col0))
		return false;

	if (terrain->is_wall(row1, col1))
		return false;

	Vec3 point0 = terrain->get_world_position(row0, col0);
	Vec3 point1 = terrain->get_world_position(row1, col1);

	int min_x = 0;
	int max_x = 0;
	int min_y = 0;
	int max_y = 0;

	if (row0 < row1)
	{
		min_x = row0;
		max_x = row1;
	}
	else
	{
		min_x = row1;
		max_x = row0;
	}

	if (col0 < col1)
	{
		min_y = col0;
		max_y = col1;
	}
	else
	{
		min_y = col1;
		max_y = col0;
	}

	float half_size = terrain->mapSizeInWorld / terrain->get_map_width() * 0.5f;

	for (int x = min_x; x <= max_x; ++x)
	{
		for (int y = min_y; y <= max_y; ++y)
		{
			if (terrain->is_wall(x, y))
			{

				Vec3 world_pos = terrain->get_world_position(x, y);
				float p_min_x = world_pos.x - half_size;
				float p_max_x = world_pos.x + half_size;
				float p_min_y = world_pos.z - half_size;
				float p_max_y = world_pos.z + half_size;

				// left
				if (line_intersect(Vec2(point0.x, point0.z), Vec2(point1.x, point1.z), Vec2(p_min_x, p_min_y), Vec2(p_min_x, p_max_y)))
				{
					return false;
				}
				//right
				if (line_intersect(Vec2(point0.x, point0.z), Vec2(point1.x, point1.z), Vec2(p_max_x, p_min_y), Vec2(p_max_x, p_max_y)))
				{
					return false;
				}
				// top
				if (line_intersect(Vec2(point0.x, point0.z), Vec2(point1.x, point1.z), Vec2(p_min_x, p_min_y), Vec2(p_max_x, p_min_y)))
				{
					return false;
				}
				// bottom
				if (line_intersect(Vec2(point0.x, point0.z), Vec2(point1.x, point1.z), Vec2(p_min_x, p_max_y), Vec2(p_max_x, p_max_y)))
				{
					return false;
				}
			}
		}
	}

    return true; // REPLACE THIS
}

void analyze_openness(MapLayer<float> &layer)
{
    /*
        Mark every cell in the given layer with the value 1 / (d * d),
        where d is the distance to the closest wall or edge.  Make use of the
        distance_to_closest_wall helper function.  Walls should not be marked.
    */

    // WRITE YOUR CODE HERE 
	for (int x = 0; x < terrain->get_map_width(); ++x)
	{
		for (int y = 0; y < terrain->get_map_height(); ++y)
		{
			float d = distance_to_closest_wall(x, y);

			layer.set_value(x,y, 1 / (d*d));
		}
	}
}

void analyze_visibility(MapLayer<float> &layer)
{
    /*
        Mark every cell in the given layer with the number of cells that are visible to it, 
		divided by 160 (a magic number that looks good). 
		Make sure to cap the value at 1.0 as well.

        Two cells are visible to each other if a line between their centerpoints doesn't
        intersect the four boundary lines of every wall cell.  Make use of the is_clear_path
        helper function.
    */

    // WRITE YOUR CODE HERE

	for (int x1 = 0; x1 < terrain->get_map_width(); ++x1)
	{
		for (int y1 = 0; y1 < terrain->get_map_height(); ++y1)
		{
			if (terrain->is_wall(x1, y1))
				continue;

			float value = 0.0f ;
			for (int x2 = 0; x2 < terrain->get_map_width(); ++x2)
			{
				for (int y2 = 0; y2 < terrain->get_map_height(); ++y2)
				{
					if (terrain->is_wall(x2, y2) || (x1 == x2 && y1 == y2))
						continue;

				//	std::cout << value << std::endl;

					if (is_clear_path(x1, y1, x2, y2))
					{
						value += 1.0f / 160.0f;

						if (value > 1.0f)
							value = 1.0f;
					}
				}
			}
			layer.set_value(x1, y1, value);
		}
	}
}

void analyze_visble_to_cell(MapLayer<float> &layer, int row, int col)
{
    /*
        For every cell in the given layer mark it with 1.0 if it is visible to the given cell, 
		0.5 if it isn't visible but is next to a visible cell,
        or 0.0 otherwise.

        Two cells are visible to each other if a line between their centerpoints doesn't
        intersect the four boundary lines of every wall cell.  Make use of the is_clear_path
        helper function.
    */

    // WRITE YOUR CODE HERE

	for (int x1 = 0; x1 < terrain->get_map_width(); ++x1)
	{
		for (int y1 = 0; y1 < terrain->get_map_height(); ++y1)
		{
			if (terrain->is_wall(x1, y1))
				continue;

			if (is_clear_path(row, col, x1, y1))
			{
				layer.set_value(x1, y1, 1.0f);
			}
			else
			{
				static const int offsetX[] =  { -1, 1, 0, 0, -1,  1, -1 , 1};
				static const int offsetY[] =  { 0 , 0,-1, 1, -1, -1,  1 , 1};
			    bool legit[] = { false , false , false, false };

				bool zero = true;
				for (int i = 0; i < 8; ++i)
				{
					unsigned int neighborRow = x1 + offsetX[i];
					unsigned int neighborCol = y1 + offsetY[i];

					if (terrain->is_valid_grid_position(neighborRow, neighborCol) && !terrain->is_wall(neighborRow, neighborCol))
					{
						if (i < 4)
						{
							if (layer.get_value(neighborRow, neighborCol) == 1.0f)
							{
								zero = false;
								break;
							}
							
							legit[i] = true;
						}
						else if (i == 4 && legit[0] && legit[2] && layer.get_value(neighborRow, neighborCol) == 1.0f)
						{
							zero = false;
							break;
						}
						else if (i == 5 && legit[1] && legit[2] && layer.get_value(neighborRow, neighborCol) == 1.0f)
						{
							zero = false;
							break;
						}
						else if (i == 6 && legit[0] && legit[3] && layer.get_value(neighborRow, neighborCol) == 1.0f)
						{
							zero = false;
							break;
						}
						else if (i == 7 && legit[1] && legit[3] && layer.get_value(neighborRow, neighborCol) == 1.0f)
						{
							zero = false;
							break;
						}
					}
				}

				if(zero)
					layer.set_value(x1, y1, 0.0f);
				else
					layer.set_value(x1, y1, 0.5f);
			}
		}
	}
}

void analyze_agent_vision(MapLayer<float> &layer, const Agent *agent)
{
    /*
        For every cell in the given layer that is visible to the given agent, mark it as 1.0,
		otherwise don't change the cell's current value.

        You must consider the direction the agent is facing.  All of the agent data is in three dimensions,
		but to simplify you should operate in two dimensions, the XZ plane.

        Take the dot product between the view vector and the vector from the agent to the cell,
        both normalized,
		and compare the cosines directly instead of taking the arccosine to
        avoid introducing floating-point inaccuracy (larger cosine means smaller angle).

        Give the agent a field of view slighter larger than 180 degrees.

        Two cells are visible to each other if a line between their centerpoints doesn't
        intersect the four boundary lines of every wall cell.  Make use of the is_clear_path
        helper function.
    */

    // WRITE YOUR CODE HERE

	GridPos agent_grid_pos = terrain->get_grid_position(agent->get_position());
	Vec3 agent_pos = agent->get_position();
	Vec3 agent_view = agent->get_forward_vector();

	Vec2 agent_view_2d(agent_view.x, agent_view.z);

	agent_view_2d.Normalize();

	for (int x1 = 0; x1 < terrain->get_map_width(); ++x1)
	{
		for (int y1 = 0; y1 < terrain->get_map_height(); ++y1)
		{
			Vec3 world_pos = terrain->get_world_position(x1, y1);
			Vec2 agent_pos_2d(world_pos.x -agent_pos.x, world_pos.z - agent_pos.z);
			agent_pos_2d.Normalize();

			float angle = (agent_view_2d.Dot(agent_pos_2d)) / (agent_view_2d.Length() * agent_pos_2d.Length());

			if (angle > 0.0f && is_clear_path(agent_grid_pos.row , agent_grid_pos.col, x1, y1))
			{
				layer.set_value(x1, y1, 1.0f);
			}
		}
	}
}

void propagate_solo_occupancy(MapLayer<float> &layer, float decay, float growth)
{
    /*
        For every cell in the given layer:

            1) Get the value of each neighbor and apply decay factor
            2) Keep the highest value from step 1
            3) Linearly interpolate from the cell's current value to the value from step 2
               with the growing factor as a coefficient.  Make use of the lerp helper function.
            4) Store the value from step 3 in a temporary layer.
               A float[40][40] will suffice, no need to dynamically allocate or make a new MapLayer.

        After every cell has been processed into the temporary layer, write the temporary layer into
        the given layer;
    */
    
    // WRITE YOUR CODE HERE

	float temp_layer[40][40] = { 0 };

	for (int x1 = 0; x1 < terrain->get_map_width(); ++x1)
	{
		for (int y1 = 0; y1 < terrain->get_map_height(); ++y1)
		{
			if (terrain->is_wall(x1, y1))
				continue;

			static const int offsetX[] = { -1, 1, 0, 0, -1,  1, -1 , 1 };
			static const int offsetY[] = { 0 , 0,-1, 1, -1, -1,  1 , 1 };
			bool legit[] = { false , false , false, false };

			for (int i = 0; i < 8; ++i)
			{
				unsigned int neighborRow = x1 + offsetX[i];
				unsigned int neighborCol = y1 + offsetY[i];

				if (terrain->is_valid_grid_position(neighborRow, neighborCol) && !terrain->is_wall(neighborRow, neighborCol))
				{
					if (i < 4)
					{
						float new_influence = layer.get_value(neighborRow, neighborCol) * exp(-1 * 1 * decay);

						if (temp_layer[x1][y1] < new_influence)
							temp_layer[x1][y1] = new_influence;
		
						legit[i] = true;
					}
					else if (i == 4 && legit[0] && legit[2])
					{
						float new_influence = layer.get_value(neighborRow, neighborCol) * exp(-1 * 1.4142f * decay);

						if (temp_layer[x1][y1] < new_influence)
							temp_layer[x1][y1] = new_influence;
					}
					else if (i == 5 && legit[1] && legit[2] )
					{
						float new_influence = layer.get_value(neighborRow, neighborCol) * exp(-1 * 1.4142f * decay);

						if (temp_layer[x1][y1] < new_influence)
							temp_layer[x1][y1] = new_influence;
					}
					else if (i == 6 && legit[0] && legit[3])
					{
						float new_influence = layer.get_value(neighborRow, neighborCol) * exp(-1 * 1.4142f * decay);

						if (temp_layer[x1][y1] < new_influence)
							temp_layer[x1][y1] = new_influence;
					}
					else if (i == 7 && legit[1] && legit[3] )
					{
						float new_influence = layer.get_value(neighborRow, neighborCol) * exp(-1 * 1.4142f * decay);

						if (temp_layer[x1][y1] < new_influence)
							temp_layer[x1][y1] = new_influence;
					}
				}
			}
		}
	}

	for (int x1 = 0; x1 < terrain->get_map_width(); ++x1)
	{
		for (int y1 = 0; y1 < terrain->get_map_height(); ++y1)
		{
			layer.set_value(x1, y1, lerp(layer.get_value(x1, y1), temp_layer[x1][y1], growth));
		}
	}

}

void propagate_dual_occupancy(MapLayer<float> &layer, float decay, float growth)
{
    /*
        Similar to the solo version, but the values range from -1.0 to 1.0, instead of 0.0 to 1.0

        For every cell in the given layer:

        1) Get the value of each neighbor and apply decay factor
        2) Keep the highest ABSOLUTE value from step 1
        3) Linearly interpolate from the cell's current value to the value from step 2
           with the growing factor as a coefficient.  Make use of the lerp helper function.
        4) Store the value from step 3 in a temporary layer.
           A float[40][40] will suffice, no need to dynamically allocate or make a new MapLayer.

        After every cell has been processed into the temporary layer, write the temporary layer into
        the given layer;
    */

    // WRITE YOUR CODE HERE
	float temp_layer[40][40] = { 0 };

	for (int x1 = 0; x1 < terrain->get_map_width(); ++x1)
	{
		for (int y1 = 0; y1 < terrain->get_map_height(); ++y1)
		{
			if (terrain->is_wall(x1, y1))
				continue;

			static const int offsetX[] = { -1, 1, 0, 0, -1,  1, -1 , 1 };
			static const int offsetY[] = { 0 , 0,-1, 1, -1, -1,  1 , 1 };
			bool legit[] = { false , false , false, false };

			for (int i = 0; i < 8; ++i)
			{
				unsigned int neighborRow = x1 + offsetX[i];
				unsigned int neighborCol = y1 + offsetY[i];

				if (terrain->is_valid_grid_position(neighborRow, neighborCol) && !terrain->is_wall(neighborRow, neighborCol))
				{
					if (i < 4)
					{
						float new_influence = layer.get_value(neighborRow, neighborCol) * exp(-1 * 1 * decay);

						if (abs(temp_layer[x1][y1]) < abs(new_influence))
							temp_layer[x1][y1] = new_influence;

						legit[i] = true;
					}
					else if (i == 4 && legit[0] && legit[2] )
					{
						float new_influence = layer.get_value(neighborRow, neighborCol) * exp(-1 * 1.4142f * decay);

						if (abs(temp_layer[x1][y1]) < abs(new_influence))
							temp_layer[x1][y1] = new_influence;
					}
					else if (i == 5 && legit[1] && legit[2] )
					{
						float new_influence = layer.get_value(neighborRow, neighborCol) * exp(-1 * 1.4142f * decay);

						if (abs(temp_layer[x1][y1]) < abs(new_influence))
							temp_layer[x1][y1] = new_influence;
					}
					else if (i == 6 && legit[0] && legit[3] )
					{
						float new_influence = layer.get_value(neighborRow, neighborCol) * exp(-1 * 1.4142f * decay);

						if (abs(temp_layer[x1][y1]) < abs(new_influence))
							temp_layer[x1][y1] = new_influence;
					}
					else if (i == 7 && legit[1] && legit[3] )
					{
						float new_influence = layer.get_value(neighborRow, neighborCol) * exp(-1 * 1.4142f * decay);

						if (abs(temp_layer[x1][y1]) < abs(new_influence))
							temp_layer[x1][y1] = new_influence;
					}
				}
			}
		}
	}

	for (int x1 = 0; x1 < terrain->get_map_width(); ++x1)
	{
		for (int y1 = 0; y1 < terrain->get_map_height(); ++y1)
		{
			layer.set_value(x1, y1, lerp(layer.get_value(x1, y1), temp_layer[x1][y1], growth));
		}
	}
}

void normalize_solo_occupancy(MapLayer<float> &layer)
{
    /*
        Determine the maximum value in the given layer, and then divide the value
        for every cell in the layer by that amount.  This will keep the values in the
        range of [0, 1].  Negative values should be left unmodified.
    */

    // WRITE YOUR CODE HERE

	float largest = 0;
	for (int x1 = 0; x1 < terrain->get_map_width(); ++x1)
	{
		for (int y1 = 0; y1 < terrain->get_map_height(); ++y1)
		{
			if (terrain->is_wall(x1, y1))
				continue;

			float value = layer.get_value(x1, y1);
			if (largest < value)
				largest = value;
		}
	}

	if (largest != 0.0f)
	{
		for (int x1 = 0; x1 < terrain->get_map_width(); ++x1)
		{
			for (int y1 = 0; y1 < terrain->get_map_height(); ++y1)
			{
				if (terrain->is_wall(x1, y1))
					continue;

				float value = layer.get_value(x1, y1);
				if (value > 0.0f)
					layer.set_value(x1, y1, layer.get_value(x1, y1) / largest);
			}
		}
	}
	
}

void normalize_dual_occupancy(MapLayer<float> &layer)
{
    /*
        Similar to the solo version, but you need to track greatest positive value AND 
        the least (furthest from 0) negative value.

        For every cell in the given layer, if the value is currently positive divide it by the
        greatest positive value,
		or if the value is negative divide it by -1.0 * the least negative value
        (so that it remains a negative number).  This will keep the values in the range of [-1, 1].
    */

    // WRITE YOUR CODE HERE

	float largest = 0.0f;
	float smallest = 0.0f;
	for (int x1 = 0; x1 < terrain->get_map_width(); ++x1)
	{
		for (int y1 = 0; y1 < terrain->get_map_height(); ++y1)
		{
			if (terrain->is_wall(x1, y1))
				continue;

			float value = layer.get_value(x1, y1);
			if (largest < value)
				largest = value;

			if (smallest > value)
				smallest = value;
		}
	}


	for (int x1 = 0; x1 < terrain->get_map_width(); ++x1)
	{
		for (int y1 = 0; y1 < terrain->get_map_height(); ++y1)
		{
			if (terrain->is_wall(x1, y1))
				continue;

			float value = layer.get_value(x1, y1);
			if (largest != 0.0f && value > 0.0f)
				layer.set_value(x1, y1, layer.get_value(x1, y1) / largest);

			if (smallest != 0.0f && value < 0.0f)
				layer.set_value(x1, y1, layer.get_value(x1, y1) / -smallest);
			
		}
	}
}

void enemy_field_of_view(MapLayer<float> &layer, float fovAngle, float closeDistance, float occupancyValue, AStarAgent *enemy)
{
    /*
        First, clear out the old values in the map layer by setting any negative value to 0.

        Then, for every cell in the layer that is within the field of view cone,
		from the enemy agent, mark it with the occupancy value.  

		Take the dot product between the view
        vector and the vector from the agent to the cell, both normalized, and compare the
        cosines directly instead of taking the arccosine to avoid introducing floating-point
        inaccuracy (larger cosine means smaller angle).

        If the tile is close enough to the enemy (less than closeDistance),
        you only check if it's visible to enemy.  Make use of the is_clear_path
        helper function.  Otherwise, you must consider the direction the enemy is facing too.
        This creates a radius around the enemy that the player can be detected within, as well
        as a fov cone.
    */

	Vec3 enemy_pos = enemy->get_position();
	GridPos agent_grid_pos = terrain->get_grid_position(enemy->get_position());
	Vec3 enemy_view = enemy->get_forward_vector();
	Vec2 enemy_view_2d(enemy_view.x, enemy_view.z);
	enemy_view_2d.Normalize();

    // WRITE YOUR CODE HERE
	for (int x1 = 0; x1 < terrain->get_map_width(); ++x1)
	{
		for (int y1 = 0; y1 < terrain->get_map_height(); ++y1)
		{
			if (terrain->is_wall(x1, y1))
				continue;

			float value = layer.get_value(x1, y1);
			if (value < 0.0f)
			{
				layer.set_value(x1, y1, 0.0f);
			}
			

			Vec3 world_pos = terrain->get_world_position(x1, y1);

			// the vector from the agent to the cell
			Vec2 enemy_pos_2d(world_pos.x - enemy_pos.x, world_pos.z - enemy_pos.z);

			float row_dif = (abs(enemy_pos_2d.x));
			float col_dif = (abs(enemy_pos_2d.y));
			float distance = static_cast<float>(sqrt(row_dif * row_dif + col_dif * col_dif));

			enemy_pos_2d.Normalize();

			float angle = (enemy_view_2d.Dot(enemy_pos_2d)) / (enemy_view_2d.Length() * enemy_pos_2d.Length());

			

			if ((angle > cos((fovAngle / 2)* 3.1415 /180) || distance < closeDistance * terrain->mapSizeInWorld / terrain->get_map_width())&& is_clear_path(x1, y1, agent_grid_pos.row, agent_grid_pos.col))
			{
					layer.set_value(x1, y1, occupancyValue);
			}
			
		}
	}
}

bool enemy_find_player(MapLayer<float> &layer, AStarAgent *enemy, Agent *player)
{
    /*
        Check if the player's current tile has a negative value, ie in the fov cone
        or within a detection radius.
    */

    const auto &playerWorldPos = player->get_position();

    const auto playerGridPos = terrain->get_grid_position(playerWorldPos);

    // verify a valid position was returned
    if (terrain->is_valid_grid_position(playerGridPos) == true)
    {
        if (layer.get_value(playerGridPos) < 0.0f)
        {
            return true;
        }
    }

    // player isn't in the detection radius or fov cone, OR somehow off the map
    return false;
}

bool enemy_seek_player(MapLayer<float> &layer, AStarAgent *enemy)
{
    /*
        Attempt to find a cell with a value of 1.0, and then set it as the new target,
        using enemy->path_to.

        If there are multiple cells with 1.0, then pick the cell closest to the enemy.

        Return whether a target cell was found.
    */

    // WRITE YOUR CODE HERE
	Vec3 enemy_pos = enemy->get_position();

	float closest = std::numeric_limits<float>::max();
	Vec3 target_path(0.0f, 0.0f, 0.0f);
	bool have_path = false;
	for (int x1 = 0; x1 < terrain->get_map_width(); ++x1)
	{
		for (int y1 = 0; y1 < terrain->get_map_height(); ++y1)
		{
			if (terrain->is_wall(x1, y1))
				continue;

			if (layer.get_value(x1, y1) == 1.0f)
			{
				have_path = true;
				Vec3 world_pos = terrain->get_world_position(x1, y1);

				float row_dif = static_cast<float>(abs(x1 - world_pos.x));
				float col_dif = static_cast<float>(abs(y1 - world_pos.z));

				float distance = static_cast<float>(sqrt(row_dif * row_dif + col_dif * col_dif));

				if (distance < closest)
				{
					closest = distance;
					target_path = world_pos;
				}
			}
		}
	}

	if (have_path)
		enemy->path_to(target_path);

    return have_path; // REPLACE THIS
}


