#include <pch.h>
#include "Projects/ProjectTwo.h"
#include "P2_Pathfinding.h"

#pragma region Extra Credit
bool ProjectTwo::implemented_floyd_warshall()
{
    return false;
}

bool ProjectTwo::implemented_goal_bounding()
{
    return false;
}

bool ProjectTwo::implemented_jps_plus()
{
    return false;
}
#pragma endregion

AStarPather::AStarPather()
	:grid_map{ nullptr }, cheapest_node{ nullptr }, square_root_two{static_cast<float>(sqrt(2))}, map_size_x{ 0 }, map_size_y{ 0 }, goal_row{ 0 }, goal_col{ 0 },
	top_legit{ false }, bottom_legit{ false }, left_legit{ false }, right_legit{ false }, grid_distance_rep{0.0f}
{
}

AStarPather::~AStarPather()
{
	DeleteMap();
}

bool AStarPather::initialize()
{
    // handle any one-time setup requirements you have

    /*
        If you want to do any map-preprocessing, you'll need to listen
        for the map change message.  It'll look something like this:

        Callback cb = std::bind(&AStarPather::your_function_name, this);
        Messenger::listen_for_message(Messages::MAP_CHANGE, cb);

        There are other alternatives to using std::bind, so feel free to mix it up.
        Callback is just a typedef for std::function<void(void)>, so any std::invoke'able
        object that std::function can wrap will suffice.
    */

	Callback cb = std::bind(&AStarPather::MapChange, this);
	Messenger::listen_for_message(Messages::MAP_CHANGE, cb);

    return true; // return false if any errors actually occur, to stop engine initialization
}

void AStarPather::shutdown()
{
    /*
        Free any dynamically allocated memory or any other general house-
        keeping you need to do during shutdown.
    */
}

PathResult AStarPather::compute_path(PathRequest &request)
{
    /*
        This is where you handle pathing requests, each request has several fields:

        start/goal - start and goal world positions
        path - where you will build the path upon completion, path should be
            start to goal, not goal to start
        heuristic - which heuristic calculation to use
        weight - the heuristic weight to be applied
        newRequest - whether this is the first request for this path, should generally
            be true, unless single step is on

        smoothing - whether to apply smoothing to the path
        rubberBanding - whether to apply rubber banding
        singleStep - whether to perform only a single A* step
        debugColoring - whether to color the grid based on the A* state:
            closed list nodes - yellow
            open list nodes - blue

            use terrain->set_color(row, col, Colors::YourColor);
            also it can be helpful to temporarily use other colors for specific states
            when you are testing your algorithms

        method - which algorithm to use: A*, Floyd-Warshall, JPS+, or goal bounding,
            will be A* generally, unless you implement extra credit features

        The return values are:
            PROCESSING - a path hasn't been found yet, should only be returned in
                single step mode until a path is found
            COMPLETE - a path to the goal was found and has been built in request.path
            IMPOSSIBLE - a path from start to goal does not exist, do not add start position to path
    */

    // WRITE YOUR CODE HERE

	//terrain->mapSizeInWorld() // divide * 1.5

	if (request.newRequest)
	{
		open_list.clear();
		//ResetVisited();
		close_list.clear();
		
		goal_row = terrain->get_grid_position(request.goal).row;
		goal_col = terrain->get_grid_position(request.goal).col;

		int start_row = terrain->get_grid_position(request.start).row;
		int start_col = terrain->get_grid_position(request.start).col;

		grid_map[start_row][start_col].row = start_row;
		grid_map[start_row][start_col].col = start_col;
		grid_map[start_row][start_col].dijkstra_cost = 0.0f;
		grid_map[start_row][start_col].total_cost = 0.0f;
		grid_map[start_row][start_col].parent = nullptr;

		open_list.push_back(&grid_map[start_row][start_col]);
	}

	while (!open_list.empty())
	{
		cheapest_node = PopCheapest();
		if (cheapest_node->row == goal_row && cheapest_node->col == goal_col)
		{
			terrain->set_color(cheapest_node->row, cheapest_node->col, Colors::Yellow);

			if (request.settings.rubberBanding && request.settings.smoothing)
			{
				grid_distance_rep = 1.5f * terrain->mapSizeInWorld / terrain->get_map_width();

				Rubberbanding(request);
				smoothing_list.clear();
			
				auto it1 = request.path.begin();
				auto it2 = std::next(it1);

				while (it2 != request.path.end())
				{
					
					CheckDistance(request, it1, it2);
						
					it1 = it2;
					++it2;
				}

				smoothing_list.clear();

				for (auto it = request.path.begin(); it != request.path.end(); ++it)
				{
					smoothing_list.push_back(Vec3(it->x, it->y, it->z));
				}


				request.path.clear();

				Smoothing(request);
			}
			else if(request.settings.rubberBanding)
			{
				Rubberbanding(request);
			}
			else if(request.settings.smoothing)
			{
				while (cheapest_node)
				{
					request.path.push_front(terrain->get_world_position(cheapest_node->row, cheapest_node->col));
					cheapest_node = cheapest_node->parent;
				}

				smoothing_list.clear();

				for (auto it = request.path.begin(); it != request.path.end(); ++it)
				{
					smoothing_list.push_back(Vec3(it->x, it->y, it->z));
				}

				request.path.clear();
				Smoothing(request);

			}
			else
			{
				while (cheapest_node)
				{
					request.path.push_front(terrain->get_world_position(cheapest_node->row, cheapest_node->col));
					cheapest_node = cheapest_node->parent;
				}
			}


			return PathResult::COMPLETE;
		}
		
		/* eight neighbour */
		top_legit = false;
	    bottom_legit = false;
		left_legit = false;
		right_legit = false;

		for (int i = 0; i < 8; ++i)
		{
			CheckLegitMove(request, cheapest_node, i);
		}

		//grid_map[cheapest_node->row][cheapest_node->row].visited = true;
		close_list.push_back(cheapest_node);
		terrain->set_color(cheapest_node->row, cheapest_node->col, Colors::Yellow);

		if(request.settings.singleStep)
			return PathResult::PROCESSING;
	}

	return PathResult::IMPOSSIBLE;


}

void AStarPather::ConstructMap()
{
	map_size_x = terrain->get_map_width();
	map_size_y = terrain->get_map_height();

	grid_map =  new Node *[map_size_x];
    
	for (int i = 0; i < map_size_x; ++i)
	{
		grid_map[i] = new Node[map_size_y];
	}
}

void AStarPather::DeleteMap()
{
	for (int i = 0; i < map_size_y; ++i) 
	{
		if (grid_map[i])
		{
			delete[] grid_map[i];
			grid_map[i] = nullptr;
		}
	}
	if (grid_map)
	{
		delete[] grid_map;
		grid_map = nullptr;
	}
}


void AStarPather::MapChange()
{
	if (terrain->get_map_width() != map_size_x || terrain->get_map_height() != map_size_y)
	{
		DeleteMap();
		ConstructMap();
	}
	
}

AStarPather::Node * AStarPather::PopCheapest()
{
	Node * cheapest_node = nullptr;
	size_t index = 0;
	for (auto iterator = open_list.begin(), end = open_list.end(); iterator != end; ++iterator)
	{
		if (!cheapest_node  || (*iterator)->total_cost < cheapest_node->total_cost)
		{
			cheapest_node = *iterator;
			index = iterator - open_list.begin();
		}
	}


	if (cheapest_node)
	{
		if (open_list.size() > 1)
		{
			std::iter_swap(open_list.begin() +  index, open_list.end() - 1);
			open_list.pop_back();
		}
		else
		{
			open_list.pop_back();
			open_list.clear();
		}
	}
	return cheapest_node;
}


AStarPather::Node * AStarPather::FindNodeInOpenList(int row, int col)
{
	Node * found = nullptr;
	for (auto iterator = open_list.begin(), end = open_list.end(); iterator != end; ++iterator)
	{
		if ((*iterator)->row == row && (*iterator)->col == col)
		{
			found = *iterator;
			break;
		}
	}
	return found;
}
/*
void AStarPather::ResetVisited()
{
	for (int x = 0; x < map_size_x; ++x)
	{
		for (int y = 0; y < map_size_y; ++y)
		{
			grid_map[x][y].visited = false;
		}
	}
}*/

AStarPather::Node * AStarPather::FindNodeInCloseList(int row, int col)
{
	Node * found = nullptr;
	for (auto iterator = close_list.begin(), end = close_list.end(); iterator != end; ++iterator)
	{
		if ((*iterator)->row == row && (*iterator)->col == col)
		{
			found = *iterator;
			break;
		}
	}
	return found;
}

void AStarPather::CheckLegitMove(PathRequest &request , Node * parent , int side)
{
	int row = 0;
	int col = 0;

	switch (side)
	{
	case 0: 
		row = parent->row - 1;
		col = parent->col;
		if (terrain->is_valid_grid_position(row, col) && !terrain->is_wall(row, col))
		{
			top_legit = true;
			UpdateNode(request, row, col, parent->dijkstra_cost + 1.0f);
		}
		break;
	case 1: 
		row = parent->row + 1;
		col = parent->col;
		if (terrain->is_valid_grid_position(row, col) && !terrain->is_wall(row, col))
		{
			bottom_legit = true;
			UpdateNode(request, row, col, parent->dijkstra_cost + 1.0f);
		}
		break;
	case 2: 
		row = parent->row ;
		col = parent->col - 1;
		if (terrain->is_valid_grid_position(row, col) && !terrain->is_wall(row, col))
		{
			left_legit = true;
			UpdateNode(request, row, col, parent->dijkstra_cost +  1.0f);
		}
		break;
	case 3: 
		row = parent->row ;
		col = parent->col + 1;
		if (terrain->is_valid_grid_position(row, col) && !terrain->is_wall(row, col))
		{
			right_legit = true;
			UpdateNode(request, row, col, parent->dijkstra_cost + 1.0f);
		}
		break;
	case 4:
		row = parent->row - 1;
		col = parent->col - 1;
		if (top_legit && left_legit && terrain->is_valid_grid_position(row, col) && !terrain->is_wall(row, col))
		{
			UpdateNode(request, row, col, parent->dijkstra_cost + square_root_two);
		}
		break;
	case 5:
		row = parent->row - 1;
		col = parent->col + 1;
		if (top_legit && right_legit && terrain->is_valid_grid_position(row, col) && !terrain->is_wall(row, col))
		{
			UpdateNode(request, row, col, parent->dijkstra_cost + square_root_two);
		}
		break;
	case 6:
		row = parent->row + 1;
		col = parent->col - 1;
		if (bottom_legit && left_legit && terrain->is_valid_grid_position(row, col) && !terrain->is_wall(row, col))
		{
			UpdateNode(request, row, col, parent->dijkstra_cost + square_root_two);
		}
		break;
	case 7:
		row = parent->row + 1;
		col = parent->col + 1;
		if (bottom_legit && right_legit && terrain->is_valid_grid_position(row, col) && !terrain->is_wall(row, col))
		{
			UpdateNode(request, row, col, parent->dijkstra_cost + square_root_two);
		}
		break;
	}

}

void AStarPather::UpdateNode(PathRequest & request, int row, int col, float dijkstra)
{
	float heuristic = CalculateHeuristic(row, col, request) * request.settings.weight;

	float total_cost = dijkstra + heuristic;

	auto closefound = FindNodeInCloseList(row, col);

	/*if (grid_map[row][col].visited)
	{
		if (grid_map[row][col].total_cost > total_cost)
		{
			grid_map[row][col].parent = cheapest_node->parent;
			grid_map[row][col].dijkstra_cost = dijkstra;
			grid_map[row][col].total_cost = total_cost;
		}
	}*/
	if (closefound)
	{
		if (closefound->total_cost > total_cost)
		{
			closefound->parent = cheapest_node;
			closefound->dijkstra_cost = dijkstra;
			closefound->total_cost = total_cost;
		}
	}
	else
	{
		auto openfound = FindNodeInOpenList(row, col);

		if (openfound)
		{
			if (openfound->total_cost > total_cost)
			{
				openfound->parent = cheapest_node;
				openfound->dijkstra_cost = dijkstra;
				openfound->total_cost = total_cost;
			}
		}
		else
		{
			grid_map[row][col].row = row;
			grid_map[row][col].col = col;
			grid_map[row][col].dijkstra_cost = dijkstra;
			grid_map[row][col].total_cost = total_cost;
			grid_map[row][col].parent = cheapest_node;

			open_list.push_back(&grid_map[row][col]);
			terrain->set_color(row, col, Colors::Blue);
		}
	}
}

float AStarPather::CalculateHeuristic(int row, int col, PathRequest & request)
{
	float row_dif = static_cast<float>(abs(goal_row - row));
	float col_dif = static_cast<float>(abs(goal_col - col));

	if (request.settings.heuristic == Heuristic::MANHATTAN)
	{
		return  row_dif + col_dif;
	}
	else if (request.settings.heuristic == Heuristic::CHEBYSHEV)
	{
		if (row_dif < col_dif)
			return  col_dif;
		else
			return   row_dif;
	}
	else if (request.settings.heuristic == Heuristic::EUCLIDEAN)
	{
		return static_cast<float>(sqrt(row_dif * row_dif + col_dif * col_dif));
	}
	else if (request.settings.heuristic == Heuristic::OCTILE)
	{
		float min = 0.0f;
		float max = 0.0f;

		if (row_dif < col_dif)
		{
			min = row_dif;
			max = col_dif;
		}
		else
		{
			max = row_dif;
			min = col_dif;
		}
		return  static_cast<float>(min * sqrt(2) + max - min);
	}
	return 0.0f;
}

void AStarPather::Rubberbanding(PathRequest & request)
{
	if (cheapest_node->parent->parent)
	{
		Node * first_point = cheapest_node;
		cheapest_node = cheapest_node->parent;
		Node * second_point = cheapest_node;
		cheapest_node = cheapest_node->parent;
		Node * third_point = nullptr;

		while (cheapest_node)
		{
			int min_x = 0;
			int max_x = 0;
			int min_y = 0;
			int max_y = 0;

			third_point = cheapest_node;

			if (first_point->row < third_point->row)
			{
				min_x = first_point->row;
				max_x = third_point->row;
			}
			else
			{
				max_x = first_point->row;
				min_x = third_point->row;
			}

			if (first_point->col < third_point->col)
			{
				min_y = first_point->col;
				max_y = third_point->col;
			}
			else
			{
				max_y = first_point->col;
				min_y = third_point->col;
			}

			bool safe = true;
			for (int x = min_x; x <= max_x; ++x)
			{
				for (int y = min_y; y <= max_y; ++y)
				{
					if ( terrain->is_wall(x, y))
					{
						safe = false;
						break;
					}
				}
			}

			if (safe)
			{
				second_point = third_point;
				third_point = nullptr;
			}
			else
			{
				request.path.push_front(terrain->get_world_position(first_point->row, first_point->col));
				first_point = second_point;
				second_point = nullptr;
				second_point = third_point;
				third_point = nullptr;
			}

			cheapest_node = cheapest_node->parent;
		}

		// remainder
		if (first_point)
			request.path.push_front(terrain->get_world_position(first_point->row, first_point->col));

		if (second_point)
			request.path.push_front(terrain->get_world_position(second_point->row, second_point->col));
	}
	else
	{
		request.path.push_front(terrain->get_world_position(cheapest_node->row, cheapest_node->col));
		cheapest_node = cheapest_node->parent;

		if(cheapest_node)
			request.path.push_front(terrain->get_world_position(cheapest_node->row, cheapest_node->col));
	}

}

void AStarPather::Smoothing(PathRequest & request)
{

	if (smoothing_list.size() > 1)
	{
		bool first = true;
		size_t first_index = 0;
		size_t second_index = 0;
		size_t third_index = 1;
		size_t fourth_index = 2;

		for (int i = 0; i < smoothing_list.size() - 1 ;++i)
		{
			if (second_index > smoothing_list.size() - 1)
				second_index = smoothing_list.size() - 1;

			if (third_index > smoothing_list.size() - 1)
				third_index = smoothing_list.size() - 1;

			if (fourth_index > smoothing_list.size() - 1)
				fourth_index = smoothing_list.size() - 1;

			request.path.push_back(smoothing_list[second_index]);
			request.path.push_back(Vec3::CatmullRom(smoothing_list[first_index], smoothing_list[second_index], smoothing_list[third_index], smoothing_list[fourth_index], 0.25f));
			request.path.push_back(Vec3::CatmullRom(smoothing_list[first_index], smoothing_list[second_index], smoothing_list[third_index], smoothing_list[fourth_index], 0.5f));
			request.path.push_back(Vec3::CatmullRom(smoothing_list[first_index], smoothing_list[second_index], smoothing_list[third_index], smoothing_list[fourth_index], 0.75f));

			if (first)
			{
				second_index = 1;
				third_index = 2;
				fourth_index = 3;
				first = false;
			}
			else
			{
				++first_index;
				++second_index;
				++third_index;
				++fourth_index;
			}
		}
		request.path.push_back(smoothing_list.back());
	}
	else 
	{
		request.path.push_back(smoothing_list[0]);
	}

}


void AStarPather::CheckDistance(PathRequest & request, WaypointList::iterator first, WaypointList::iterator second)
{
	float distance = static_cast<float>(sqrt((second->x - first->x) *(second->x - first->x) + (second->y - first->y) * (second->y - first->y)));

	if ((*first - *second).Length() > grid_distance_rep)
	{
		auto new_insert_it = request.path.insert(second, (*first + *second) * 0.5f);
		CheckDistance(request, first, new_insert_it);
		CheckDistance(request, new_insert_it, second);
	}
}



Vec3 AStarPather::Midpoint(Vec3 & point1, Vec3 & point2)
{
	return Vec3((point1.x + point2.x) * 0.5f, (point1.y + point2.y) * 0.5f, (point1.z + point2.z) * 0.5f);
}



AStarPather::Node::Node(Node * parent_node, float dijkstra_value, float total_value, int row_value, int col_value)
	:parent {parent_node}, dijkstra_cost{ dijkstra_value }, total_cost {total_value}, row{row_value}, col {col_value}
{
}

bool AStarPather::Node::operator==(const Node & rhs) const
{
	return total_cost == rhs.total_cost;
}

bool AStarPather::Node::operator<(const Node & rhs) const
{
	return total_cost < rhs.total_cost;
}

void *AStarPather::Node::operator new(std::size_t size, void* buffer)
{
	return buffer;
}


