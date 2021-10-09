#pragma once
#include "Misc/PathfindingDetails.hpp"
#include <limits>
#include <bitset>
#include <array>
#include <list>

class AStarPather
{
public:
    /* 
        The class should be default constructible, so you may need to define a constructor.
        If needed, you can modify the framework where the class is constructed in the
        initialize functions of ProjectTwo and ProjectThree.
    */

	AStarPather();
	~AStarPather();

    /* ************************************************** */
    // DO NOT MODIFY THESE SIGNATURES
    bool initialize();
    void shutdown();
    PathResult compute_path(PathRequest &request);
    /* ************************************************** */

    /*
        You should create whatever functions, variables, or classes you need.
        It doesn't all need to be in this header and cpp, structure it whatever way
        makes sense to you.
    */

	class Node
	{
	public:
		Node( Node * parent_node = nullptr, float dijkstra_value = std::numeric_limits<float>::max(),  float total_value = std::numeric_limits<float>::max(), int row_value = 0, int col_value = 0);

		Node * parent;

		float dijkstra_cost;
		float total_cost;

		int row;
		int col;

		//bool visited;

		bool operator==(const Node& rhs)const;
		bool operator<(const Node& rhs)const;
		void *operator new(std::size_t size, void* buffer);
	

	};


	Node ** grid_map;
	Node * cheapest_node;

	float square_root_two;

	int map_size_x;
	int map_size_y;

	int goal_row;
	int goal_col;

	bool top_legit;
	bool bottom_legit;
	bool left_legit;
	bool right_legit;

	float grid_distance_rep;

	std::vector <Node *>open_list;
	std::vector <Node *>close_list;

	std::vector<Vec3> smoothing_list;


	void ConstructMap();
	void DeleteMap();

	void MapChange();
	
	Node* PopCheapest();
	

	Node* FindNodeInOpenList(int row, int col);
	Node* FindNodeInCloseList(int row, int col);
	//void ResetVisited();
	void CheckLegitMove( PathRequest &request , Node * parent,int size);

	void UpdateNode(PathRequest & request, int row, int col, float dijkstra);
	float CalculateHeuristic(int row, int col, PathRequest & request);

	void Rubberbanding(PathRequest & request);

	void Smoothing(PathRequest & request);

	void CheckDistance(PathRequest & request ,WaypointList::iterator first, WaypointList::iterator second);

	Vec3 Midpoint(Vec3 & point1, Vec3 & point2);
	
};


	