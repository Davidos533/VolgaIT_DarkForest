#ifndef WAY_TREE_HPP
#define WAY_TREE_HPP

#include "WayNode.hpp"
#include <map>

namespace architecture
{


	class WayTree
	{
		using Position = std::pair<int, int>;
		using MapNodePair = std::pair<std::pair<int, int>, WayNode*>;

	private:
		WayNode* m_currentPosition;
		std::map<Position, WayNode*> m_nodesMap;
	public:
		WayTree();

		//WayTree() = delete;
		WayTree(const WayTree& object) = delete;

		// method to add new way node to current position
		bool addNode(Direction direction);
		WayNode* findNodeByCoordiantes(Position coordiantes, WayNode* node = nullptr, Direction backDirection = Direction::Pass);
		void updateAllCoordiantes(int up, int down, int left, int right, WayNode* node = nullptr);

		bool removeNode(WayNode* wayNode);

		~WayTree();

	private:
		void findAndConnectNearestNodes(WayNode* checkingNode);
	};
}

#endif