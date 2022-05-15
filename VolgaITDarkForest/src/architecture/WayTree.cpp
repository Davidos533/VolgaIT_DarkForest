#include "architecture/WayTree.hpp"

namespace architecture
{
#pragma region public methods

	WayTree::WayTree()
	{
		m_currentPosition = new WayNode();
		m_currentPosition->coordinates = Position(0, 0);
		m_nodesMap = std::map<Position, WayNode*>();
		m_nodesMap.insert(MapNodePair(m_currentPosition->coordinates, m_currentPosition));
	}

	/// <summary>
	/// add new way node to current position
	/// </summary>
	/// <param name="direction">new node direction</param>
	/// <returns>is node added</returns>
	bool WayTree::addNode(Direction direction)
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
				m_nodesMap.insert(MapNodePair(m_currentPosition->coordinates, m_currentPosition));

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
				m_nodesMap.insert(MapNodePair(m_currentPosition->coordinates, m_currentPosition));

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
				m_nodesMap.insert(MapNodePair(m_currentPosition->coordinates, m_currentPosition));

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
				m_nodesMap.insert(MapNodePair(m_currentPosition->coordinates, m_currentPosition));

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

	/// <summary>
	/// method to find element in way tree by recursion
	/// </summary>
	/// <param name="coordiantes">coordinates that's using for searching</param>
	/// <param name="node">node that base for searching has default value = nullptr for using inside recursion</param>
	/// <returns>finded node with coordinates</returns>
	WayNode* WayTree::findNodeByCoordiantes(Position coordiantes, WayNode* node, Direction backDirection)
	{
		static Position startPosition;

		if (node == nullptr)
		{
			node = m_currentPosition;

			//startPosition = node->coordinates;
		}
		else if (startPosition == node->coordinates)
		{
			return nullptr;
		}

		std::cerr << "x:" << node->coordinates.first << "\ty:" << node->coordinates.second << "\n";

		if (node->coordinates == coordiantes)
		{
			return node;
		}

		if (node->UpNode != nullptr && backDirection != Direction::Up)
		{
			WayNode* findedNode = findNodeByCoordiantes(coordiantes, node->UpNode, Direction::Down);

			if (findedNode != nullptr)
			{
				return findedNode;
			}
		}

		if (node->DownNode != nullptr && backDirection != Direction::Down)
		{
			WayNode* findedNode = findNodeByCoordiantes(coordiantes, node->DownNode, Direction::Up);

			if (findedNode != nullptr)
			{
				return findedNode;
			}
		}

		if (node->LeftNode != nullptr && backDirection != Direction::Left)
		{
			WayNode* findedNode = findNodeByCoordiantes(coordiantes, node->LeftNode, Direction::Right);

			if (findedNode != nullptr)
			{
				return findedNode;
			}
		}

		if (node->RightNode != nullptr && backDirection != Direction::Right)
		{
			WayNode* findedNode = findNodeByCoordiantes(coordiantes, node->RightNode, Direction::Left);

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


	bool WayTree::removeNode(WayNode* wayNode)
	{
		return true;
	}

	WayTree::~WayTree()
	{
	}

#pragma endregion

#pragma region  private methods

	void WayTree::findAndConnectNearestNodes(WayNode* checkingNode)
	{
		auto mapEnd = m_nodesMap.end();
		// successively checks node relations, and try to connect not connected nodes
		if (checkingNode->UpNode == nullptr)
		{
			auto result = m_nodesMap.find(Position(checkingNode->coordinates.first,checkingNode->coordinates.second+1));
			
			if (result != mapEnd)
			{
				checkingNode->UpNode = result->second;
			}
		}

		if (checkingNode->DownNode == nullptr)
		{
			auto result = m_nodesMap.find(Position(checkingNode->coordinates.first, checkingNode->coordinates.second - 1));

			if (result != mapEnd)
			{
				checkingNode->DownNode = result->second;
			}
		}

		if (checkingNode->LeftNode == nullptr)
		{
			auto result = m_nodesMap.find(Position(checkingNode->coordinates.first - 1, checkingNode->coordinates.second ));

			if (result != mapEnd)
			{
				checkingNode->LeftNode = result->second;
			}
		}

		if (checkingNode->RightNode == nullptr)
		{
			auto result = m_nodesMap.find(Position(checkingNode->coordinates.first + 1, checkingNode->coordinates.second ));

			if (result != mapEnd)
			{
				checkingNode->RightNode = result->second;
			}
		}
	}

#pragma endregion

}