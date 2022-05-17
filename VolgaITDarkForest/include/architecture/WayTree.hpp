#ifndef WAY_TREE_HPP
#define WAY_TREE_HPP

#include <map>
#include <stack>
#include <list>

#include "WayNode.hpp"

namespace architecture
{
	/// <summary>
	/// NOT tree, historical name
	/// person way graph
	/// </summary>
	class WayTree
	{
		using Position = std::pair<int, int>;
		using MapNodePair = std::pair<std::pair<int, int>, WayNode*>;

	private:
		WayNode* m_currentPosition;
		std::map<Position, WayNode*>* m_nodesMap;
	public:
		explicit WayTree();
		WayTree(const WayTree& object) = delete;

		// getters
		WayNode* const getCurrentPosition() const;
		std::map<Position, WayNode*>* const getNodesMap() const;

		/// <summary>
		/// add new way node to current position and set it's like current(root) node 
		/// if node in moovable direction already added, just set this node like current(root) node
		/// </summary>
		/// <param name="direction">new node direction</param>
		/// <returns>is node added</returns>
		bool addOrSetNodeInConcreteDirection(Direction direction);

		/// <summary>
		/// method to find element in way tree by recursion
		/// </summary>
		/// <param name="coordiantes">coordinates that's using for searching</param>
		/// <param name="node">node that base for searching has default value = nullptr for using inside recursion</param>
		/// <returns>finded node with coordinates</returns>
		WayNode* findNodeByPosition(Position position, WayNode* node = nullptr, Direction backDirection = Direction::Pass);

		void updateAllCoordiantes(int up, int down, int left, int right, WayNode* node = nullptr);

		std::stack<Direction>* findShortestWayToPositionFromCurrent(Position position, WayNode* node = nullptr, Direction backDirection = Direction::Pass);

		virtual ~WayTree();

	private:

		/// <summary>
		/// try to find nearest nodes by coordinates in node map for checking node and connect it
		/// </summary>
		/// <param name="checkingNode">checking node</param>
		void findAndConnectNearestNodes(WayNode* checkingNode);

		bool isPositionsNear(Position positionOne, Position positionTwo);
	};
}

#endif