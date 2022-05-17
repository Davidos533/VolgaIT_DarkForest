#include "architecture/WayTree.hpp"

namespace architecture
{
#pragma region public methods

	WayTree::WayTree()
	{
		m_currentPosition = new WayNode();
		m_currentPosition->coordinates = Position(0, 0);
		m_nodesMap = new std::map<Position, WayNode*>();
		m_nodesMap->insert(MapNodePair(m_currentPosition->coordinates, m_currentPosition));
	}

	WayNode* const WayTree::getCurrentPosition() const
	{
		return m_currentPosition;
	}

	std::map<WayTree::Position, WayNode*>* const WayTree::getNodesMap() const
	{
		return m_nodesMap;
	}

	bool WayTree::addOrSetNodeInConcreteDirection(Direction direction)
	{
		// determine direction 
		switch (direction)
		{
			case Direction::Up:
			{
				if (m_currentPosition->UpNode == nullptr)
				{
					m_currentPosition->UpNode = new WayNode();
					m_currentPosition->UpNode->DownNode = m_currentPosition;
					m_currentPosition->UpNode->coordinates = Position(m_currentPosition->coordinates.first, m_currentPosition->coordinates.second + 1);

					// set root to new node
					m_currentPosition = m_currentPosition->UpNode;

					// add node in map
					m_nodesMap->insert(MapNodePair(m_currentPosition->coordinates, m_currentPosition));

					findAndConnectNearestNodes(m_currentPosition);

					return true;
				}
				else
				{
					// set root to new node
					m_currentPosition = m_currentPosition->UpNode;
					return false;
				}
			}break;
			case Direction::Down:
			{
				if (m_currentPosition->DownNode == nullptr)
				{
					m_currentPosition->DownNode = new WayNode();
					m_currentPosition->DownNode->UpNode = m_currentPosition;
					m_currentPosition->DownNode->coordinates = Position(m_currentPosition->coordinates.first, m_currentPosition->coordinates.second - 1);

					// set root to new node
					m_currentPosition = m_currentPosition->DownNode;

					// add node in map
					m_nodesMap->insert(MapNodePair(m_currentPosition->coordinates, m_currentPosition));

					findAndConnectNearestNodes(m_currentPosition);

					return true;
				}
				else
				{
					// set root to new node
					m_currentPosition = m_currentPosition->DownNode;
					return false;
				}
			}break;
			case Direction::Left:
			{
				if (m_currentPosition->LeftNode == nullptr)
				{
					m_currentPosition->LeftNode = new WayNode();
					m_currentPosition->LeftNode->RightNode = m_currentPosition;
					m_currentPosition->LeftNode->coordinates = Position(m_currentPosition->coordinates.first - 1, m_currentPosition->coordinates.second);

					// set root to new node
					m_currentPosition = m_currentPosition->LeftNode;

					// add node in map
					m_nodesMap->insert(MapNodePair(m_currentPosition->coordinates, m_currentPosition));

					findAndConnectNearestNodes(m_currentPosition);

					return true;
				}
				else
				{
					// set root to new node
					m_currentPosition = m_currentPosition->LeftNode;
					return false;
				}
			}break;
			case Direction::Right:
			{
				if (m_currentPosition->RightNode == nullptr)
				{
					m_currentPosition->RightNode = new WayNode();
					m_currentPosition->RightNode->LeftNode = m_currentPosition;
					m_currentPosition->RightNode->coordinates = Position(m_currentPosition->coordinates.first + 1, m_currentPosition->coordinates.second);

					// set root to new node
					m_currentPosition = m_currentPosition->RightNode;

					// add node in map
					m_nodesMap->insert(MapNodePair(m_currentPosition->coordinates, m_currentPosition));

					findAndConnectNearestNodes(m_currentPosition);

					return true;
				}
				else
				{
					// set root to new node
					m_currentPosition = m_currentPosition->RightNode;
					return false;
				}
			}break;
		}
	}

	std::stack<Direction>* WayTree::findShortestWayToPositionFromCurrent(Position position, WayNode* node, Direction backDirection)
	{
		static Position startPosition;
		static std::stack<Direction>* localWaySequence = nullptr;
		static std::stack<Direction>* waySequence = nullptr;
		static std::list<Position>* passedNodes = nullptr;

		// at start automatically set current node as control node 
		if (node == nullptr)
		{
			node = m_currentPosition;

			// at first calling of this method in recurse set static variable like start position to avoid returning in start position
			startPosition = node->coordinates;

			if (localWaySequence != nullptr)
			{
				delete localWaySequence;
			}

			if (passedNodes != nullptr)
			{
				delete passedNodes;
			}

			localWaySequence = new std::stack<Direction>();
			passedNodes = new std::list<Position>();
			waySequence = nullptr;
		}
		// path returned in start postition, escape from this way branch
		else if (node->coordinates == startPosition)
		{
			localWaySequence->pop();
			return nullptr;
		}
		else if (std::find(passedNodes->begin(), passedNodes->end(), node->coordinates) != passedNodes->end())
		{
			localWaySequence->pop();
			return nullptr;
		}
		else
		{
			passedNodes->push_back(node->coordinates);
		}

		// new find path lenghest than past path
		if (waySequence != nullptr && waySequence->size() < localWaySequence->size())
		{
			localWaySequence->pop();
			return nullptr;
		}

		// searching node finded, return back to search shortesd path
		if (isPositionsNear(node->coordinates, position))
		{
			if (waySequence == nullptr || waySequence->size()>localWaySequence->size())
			{
				if (waySequence != nullptr)
				{
					delete waySequence;
					waySequence = nullptr;
				}

				waySequence = new std::stack<Direction>(*localWaySequence);
			}

			if (localWaySequence->size() > 0)
			{
				localWaySequence->pop();
			}

			return waySequence;
		}

		if (node->UpNode != nullptr && backDirection != Direction::Up)
		{
			localWaySequence->push(Direction::Up);
			findShortestWayToPositionFromCurrent(position, node->UpNode, Direction::Down);
		}

		if (node->DownNode != nullptr && backDirection != Direction::Down)
		{
			localWaySequence->push(Direction::Down);
			findShortestWayToPositionFromCurrent(position, node->DownNode, Direction::Up);
		}

		if (node->LeftNode != nullptr && backDirection != Direction::Left)
		{
			localWaySequence->push(Direction::Left);
			findShortestWayToPositionFromCurrent(position, node->LeftNode, Direction::Right);
		}

		if (node->RightNode != nullptr && backDirection != Direction::Right)
		{
			localWaySequence->push(Direction::Right);
			findShortestWayToPositionFromCurrent(position, node->RightNode, Direction::Left);
		}

		if (localWaySequence->size())
		{
			// can't go to any branch, remove last direction
			localWaySequence->pop();
		}

		return waySequence;
	}

	WayNode* WayTree::findNodeByPosition(Position position, WayNode* node, Direction backDirection)
	{
		static Position startPosition;

		if (node == nullptr)
		{
			node = m_currentPosition;

			startPosition = node->coordinates;
		}
		else if (startPosition == node->coordinates)
		{
			return nullptr;
		}

		if (node->coordinates == position)
		{
			return node;
		}

		if (node->UpNode != nullptr && backDirection != Direction::Up)
		{
			WayNode* findedNode = findNodeByPosition(position, node->UpNode, Direction::Down);

			if (findedNode != nullptr)
			{
				return findedNode;
			}
		}

		if (node->DownNode != nullptr && backDirection != Direction::Down)
		{
			WayNode* findedNode = findNodeByPosition(position, node->DownNode, Direction::Up);

			if (findedNode != nullptr)
			{
				return findedNode;
			}
		}

		if (node->LeftNode != nullptr && backDirection != Direction::Left)
		{
			WayNode* findedNode = findNodeByPosition(position, node->LeftNode, Direction::Right);

			if (findedNode != nullptr)
			{
				return findedNode;
			}
		}

		if (node->RightNode != nullptr && backDirection != Direction::Right)
		{
			WayNode* findedNode = findNodeByPosition(position, node->RightNode, Direction::Left);

			if (findedNode != nullptr)
			{
				return findedNode;
			}
		}
		return nullptr;
	}

	void WayTree::updateAllCoordiantes(int up, int down, int left, int right, WayNode* node)
	{
		if (node == nullptr)
		{
			node = m_currentPosition;
		}
		if (node->UpNode != nullptr)
		{
			updateAllCoordiantes(up, down, left, right, node->UpNode);
		}
		if (node->DownNode != nullptr)
		{
			updateAllCoordiantes(up, down, left, right, node->DownNode);
		}
		if (node->LeftNode != nullptr)
		{
			updateAllCoordiantes(up, down, left, right, node->LeftNode);
		}
		if (node->RightNode != nullptr)
		{
			updateAllCoordiantes(up, down, left, right, node->RightNode);
		}
	}

	WayTree::~WayTree()
	{
		if (m_nodesMap != nullptr)
		{
			for (auto& element : *m_nodesMap)
			{
				delete element.second;
			}
			delete m_nodesMap;
		}
	}

#pragma endregion

#pragma region  private methods

	void WayTree::findAndConnectNearestNodes(WayNode* checkingNode)
	{

		// successively checks node relations, and try to connect not connected nodes
		if (checkingNode->UpNode == nullptr)
		{
			auto result = m_nodesMap->find(Position(checkingNode->coordinates.first, checkingNode->coordinates.second + 1));

			if (result != m_nodesMap->end())
			{
				checkingNode->UpNode = result->second;
			}
		}

		if (checkingNode->DownNode == nullptr)
		{
			auto result = m_nodesMap->find(Position(checkingNode->coordinates.first, checkingNode->coordinates.second - 1));

			if (result != m_nodesMap->end())
			{
				checkingNode->DownNode = result->second;
			}
		}

		if (checkingNode->LeftNode == nullptr)
		{
			auto result = m_nodesMap->find(Position(checkingNode->coordinates.first - 1, checkingNode->coordinates.second));

			if (result != m_nodesMap->end())
			{
				checkingNode->LeftNode = result->second;
			}
		}

		if (checkingNode->RightNode == nullptr)
		{
			auto result = m_nodesMap->find(Position(checkingNode->coordinates.first + 1, checkingNode->coordinates.second));

			if (result != m_nodesMap->end())
			{
				checkingNode->RightNode = result->second;
			}
		}
	}

	bool WayTree::isPositionsNear(Position positionOne, Position positionTwo)
	{
		int diffOne = std::abs(positionOne.first - positionTwo.first);
		int diffTwo = std::abs(positionOne.second - positionTwo.second);

		return  (diffOne == 0 && diffTwo == 1) || (diffOne == 1 && diffTwo == 0);
	}

#pragma endregion

}