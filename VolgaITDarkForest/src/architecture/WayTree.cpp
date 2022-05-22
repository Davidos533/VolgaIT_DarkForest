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
		m_countOfPassedNodes = 0;
		m_localWaySequence = nullptr;
		m_waySequence = nullptr;
		m_passedNodes = nullptr;
	}

	WayNode* const WayTree::getCurrentPosition() const
	{
		return m_currentPosition;
	}

	std::map<Position, WayNode*>* const WayTree::getNodesMap() const
	{
		return m_nodesMap;
	}

	int WayTree::getCountOfPassedUniqueNodes()
	{
		return m_countOfPassedNodes;
	}

	bool WayTree::addOrSetNodeInConcreteDirection(Direction direction)
	{
		// determine direction and add node if it's already not exists else just set node in given direction as root
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
					addNewNodeInNodesMap(m_currentPosition);

					// try to find boys next doors and connect them
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
					addNewNodeInNodesMap(m_currentPosition);

					// try to find boys next doors and connect them
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
					addNewNodeInNodesMap(m_currentPosition);

					// try to find boys next doors and connect them
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
					addNewNodeInNodesMap(m_currentPosition);

					// try to find boys next doors and connect them
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

	std::list<Direction>* WayTree::findShortestWayToPositionFromCurrent(Position position, WayNode* node, Direction backDirection)
	{
		// at start automatically set current node as control node 
		if (node == nullptr)
		{
			node = m_currentPosition;

			// at first calling of this method in recurse set static variable like start position to avoid returning in start position
			m_startPosition = node->coordinates;

			if (m_localWaySequence != nullptr)
			{
				m_localWaySequence->clear();
				delete m_localWaySequence;
				m_localWaySequence = nullptr;
			}

			if (m_passedNodes != nullptr)
			{
				m_passedNodes->clear();
				delete m_passedNodes;
				m_passedNodes = nullptr;
			}

			m_localWaySequence = new std::list<Direction>();
			m_passedNodes = new std::list<Position>();
			m_waySequence = nullptr;
		}
		// path returned in start postition, escape from this way branch
		else if (node->coordinates == m_startPosition)
		{
			m_localWaySequence->pop_back();
			return nullptr;
		}

		else
		{
			m_passedNodes->push_back(node->coordinates);
		}

		// new find path lenghest than past path
		if (m_waySequence != nullptr && (m_waySequence->size() - 1)  < m_localWaySequence->size())
		{
			m_localWaySequence->pop_back();
			return nullptr;
		}

		// searching node finded, return back to search shortesd path
		if (isPositionsNear(node->coordinates, position))
		{
			if (m_waySequence == nullptr || (m_waySequence->size() - 1) > m_localWaySequence->size())
			{
				if (m_waySequence != nullptr)
				{
					m_waySequence->clear();
					delete m_waySequence;
					m_waySequence = nullptr;
				}

				m_waySequence = new std::list<Direction>(*m_localWaySequence);
				m_waySequence->push_back(determineDirectionToNearPosition(node->coordinates, position));
			}

			if (m_localWaySequence->size() > 0)
			{
				m_localWaySequence->pop_back();
			}

			return m_waySequence;
		}

		if (node->UpNode != nullptr && backDirection != Direction::Up && !isContainsNodePositionInPositionsList(m_passedNodes, node->UpNode->coordinates))
		{
			m_localWaySequence->push_back(Direction::Up);
			findShortestWayToPositionFromCurrent(position, node->UpNode, Direction::Down);
		}

		if (node->DownNode != nullptr && backDirection != Direction::Down && !isContainsNodePositionInPositionsList(m_passedNodes, node->DownNode->coordinates))
		{
			m_localWaySequence->push_back(Direction::Down);
			findShortestWayToPositionFromCurrent(position, node->DownNode, Direction::Up);
		}

		if (node->LeftNode != nullptr && backDirection != Direction::Left && !isContainsNodePositionInPositionsList(m_passedNodes, node->LeftNode->coordinates))
		{
			m_localWaySequence->push_back(Direction::Left);
			findShortestWayToPositionFromCurrent(position, node->LeftNode, Direction::Right);
		}

		if (node->RightNode != nullptr && backDirection != Direction::Right && !isContainsNodePositionInPositionsList(m_passedNodes, node->RightNode->coordinates))
		{
			m_localWaySequence->push_back(Direction::Right);
			findShortestWayToPositionFromCurrent(position, node->RightNode, Direction::Left);
		}

		if (m_localWaySequence->size())
		{
			// can't go to any branch, remove last direction
			m_localWaySequence->pop_back();
		}

		return m_waySequence;
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

	void WayTree::tryAddBarrierToDirection(Direction direction)
	{
		Position position = m_currentPosition->coordinates;
		switch (direction)
		{
			case Direction::Up:
			{
				tryAddBarrierNodeToPosition(Position(position.first,position.second+1));
			}break;
			case Direction::Down:
			{
				tryAddBarrierNodeToPosition(Position(position.first,position.second-1));
			}break;
			case Direction::Left:
			{
				tryAddBarrierNodeToPosition(Position(position.first-1,position.second));
			}break;
			case Direction::Right:
			{
				tryAddBarrierNodeToPosition(Position(position.first+1,position.second));
			}break;
		}
	}

	void WayTree::tryAddBarrierNodeToPosition(Position position)
	{
		// node with position not added
		if (m_nodesMap->find(position) == m_nodesMap->end())
		{
			WayNode* newNode = new WayNode();
			newNode->isBarrier = true;
			newNode->coordinates = position;

			m_nodesMap->insert(MapNodePair(position, newNode));
		}
	}

	models::PersonIndents& WayTree::getPersonIndents()
	{
		return m_personIndents;
	}

	WayTree::~WayTree()
	{
		if (m_nodesMap != nullptr)
		{
			for (auto& element : *m_nodesMap)
			{
				delete element.second;
				element.second = nullptr;
			}
			delete m_nodesMap;
		}

		if (m_localWaySequence != nullptr)
		{
			delete m_localWaySequence;
			m_localWaySequence = nullptr;
		}

		if (m_passedNodes != nullptr)
		{
			delete m_passedNodes;
			m_passedNodes = nullptr;
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

			if (result != m_nodesMap->end() && !result->second->isBarrier)
			{
				checkingNode->UpNode = result->second;

				if (checkingNode->UpNode->DownNode == nullptr)
				{
					checkingNode->UpNode->DownNode = checkingNode;
				}
			}
		}

		if (checkingNode->DownNode == nullptr)
		{
			auto result = m_nodesMap->find(Position(checkingNode->coordinates.first, checkingNode->coordinates.second - 1));

			if (result != m_nodesMap->end() && !result->second->isBarrier)
			{
				checkingNode->DownNode = result->second;

				if (checkingNode->DownNode->UpNode == nullptr )
				{
					checkingNode->DownNode->UpNode = checkingNode;
				}
			}
		}

		if (checkingNode->LeftNode == nullptr)
		{
			auto result = m_nodesMap->find(Position(checkingNode->coordinates.first - 1, checkingNode->coordinates.second));

			if (result != m_nodesMap->end() && !result->second->isBarrier)
			{
				checkingNode->LeftNode = result->second;
				
				if (checkingNode->LeftNode->RightNode == nullptr)
				{
					checkingNode->LeftNode->RightNode = checkingNode;
				}
			}
		}

		if (checkingNode->RightNode == nullptr)
		{
			auto result = m_nodesMap->find(Position(checkingNode->coordinates.first + 1, checkingNode->coordinates.second));

			if (result != m_nodesMap->end() && !result->second->isBarrier)
			{
				checkingNode->RightNode = result->second;

				if (checkingNode->RightNode->LeftNode == nullptr)
				{
					checkingNode->RightNode->LeftNode = checkingNode;
				}
			}
		}
	}

	bool WayTree::isPositionsNear(Position positionOne, Position positionTwo)
	{
		int diffOne = std::abs(positionOne.first - positionTwo.first);
		int diffTwo = std::abs(positionOne.second - positionTwo.second);

		return  (diffOne == 0 && diffTwo == 1) || (diffOne == 1 && diffTwo == 0);
	}

	bool  WayTree::isContainsNodePositionInPositionsList(std::list<Position>* positions, Position position)
	{
		if (std::find(positions->begin(), positions->end(), position) != positions->end())
		{
			return true;
		}
		return false;
	}

	void WayTree::addNewNodeInNodesMap(WayNode* node)
	{
		m_countOfPassedNodes++;
		updateIndentByNewNode(node);

		m_nodesMap->insert(MapNodePair(node->coordinates, node));
	}

	void WayTree::updateIndentByNewNode(WayNode* node)
	{
		if (m_personIndents.maxIndentX < node->coordinates.first)
		{
			m_personIndents.maxIndentX = node->coordinates.first;
		}

		if (m_personIndents.maxIndentY < node->coordinates.second)
		{
			m_personIndents.maxIndentY = node->coordinates.second;
		}

		if (m_personIndents.minIndentX > node->coordinates.first)
		{
			m_personIndents.minIndentX = node->coordinates.first;
		}

		if (m_personIndents.minIndentY > node->coordinates.second)
		{
			m_personIndents.minIndentY = node->coordinates.second;
		}
	}

	Direction WayTree::determineDirectionToNearPosition(Position currentPosition, Position anotherPosition)
	{
		if (currentPosition.second < anotherPosition.second)
		{
			return Direction::Up;
		}
		else if (currentPosition.second > anotherPosition.second)
		{
			return Direction::Down;
		}
		else if (currentPosition.first > anotherPosition.first)
		{
			return Direction::Left;
		}
		else
		{
			return Direction::Right;
		}
	}

#pragma endregion

}