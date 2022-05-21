#ifndef WAY_TREE_HPP
#define WAY_TREE_HPP

#include <map>
#include <list>

#include "WayNode.hpp"
#include "models/PersonIndents.hpp"

namespace architecture
{
	/// <summary>
	/// NOT tree, historical name
	/// person way GRAPH
	/// </summary>
	class WayTree
	{
		using MapNodePair = std::pair<std::pair<int, int>, WayNode*>;

	private:
		/// <summary>
		/// current node(position), root node
		/// </summary>
		WayNode* m_currentPosition;

		/// <summary>
		/// map with all nodes of graph, key - position, value - node
		/// </summary>
		std::map<Position, WayNode*>* m_nodesMap;

		/// <summary>
		/// model with relative data about max min indents by coordinates oX oY
		/// </summary>
		models::PersonIndents m_personIndents;
	public:
		explicit WayTree();
		WayTree(const WayTree& object) = delete;

		/// <summary>
		/// getter
		/// </summary>
		/// <returns>current way tree node(position)</returns>
		WayNode* const getCurrentPosition() const;

		/// <summary>
		/// getter
		/// </summary>
		/// <returns>map with all nodes of graph, key - position, value - node</returns>
		std::map<Position, WayNode*>* const getNodesMap() const;

		/// <summary>
		/// getter
		/// </summary>
		/// <returns>model with relative data about max min indents by coordinates oX oY</returns>
		models::PersonIndents& getPersonIndents();

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

		/// <summary>
		/// find shortest way between current object position and given position
		/// </summary>
		/// <param name="position">given position</param>
		/// <param name="node">control parameter way node</param>
		/// <param name="backDirection">control parameter direcion to back move</param>
		/// <returns>list(front) with directions from current position to given</returns>
		std::list<Direction>* findShortestWayToPositionFromCurrent(Position position, WayNode* node = nullptr, Direction backDirection = Direction::Pass);


		/// <summary>
		/// adds new barrier node to direction if it not exists
		/// </summary>
		/// <param name="direction">direction to try add barrier</param>
		void tryAddBarrierToDirection(Direction direction);

		/// <summary>
		/// add new node as a barrier
		/// </summary>
		/// <param name="position">new node position</param>
		void tryAddBarrierNodeToPosition(Position position);

		virtual ~WayTree();

	private:

		/// <summary>
		/// try to find nearest nodes by coordinates in node map for checking node and connect it
		/// </summary>
		/// <param name="checkingNode">checking node</param>
		void findAndConnectNearestNodes(WayNode* checkingNode);

		/// <summary>
		/// determine is position one near by position two
		/// </summary>
		/// <param name="positionOne">first position</param>
		/// <param name="positionTwo">second position</param>
		/// <returns>is position near</returns>
		bool isPositionsNear(Position positionOne, Position positionTwo);

		/// <summary>
		/// check is contains position in list with positions
		/// </summary>
		/// <param name="positions">list of positions</param>
		/// <param name="position">position for check</param>
		/// <returns>is list contains value</returns>
		bool isContainsNodePositionInPositionsList(std::list<Position> *positions, Position position);

		/// <summary>
		/// add's new node in nodes map
		/// </summary>
		/// <param name="node">new node</param>
		void addNewNodeInNodesMap(WayNode* node);

		/// <summary>
		/// updates coordinate indents by given node
		/// </summary>
		/// <param name="node">given node</param>
		void updateIndentByNewNode(WayNode* node);

		/// <summary>
		/// determine direction to near position
		/// </summary>
		/// <param name="currentPosition">current position</param>
		/// <param name="anotherPosition">another position</param>
		/// <returns></returns>
		Direction determineDirectionToNearPosition(Position currentPosition, Position anotherPosition);
	};
}

#endif