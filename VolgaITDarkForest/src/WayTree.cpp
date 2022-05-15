#include "WayTree.hpp"

namespace maze
{
	WayTree::WayTree()
	{
		currentPosition = new WayNode();
		currentPosition->coordinates = std::pair<int, int>(0, 0);
	}

	// method to add new way node to current position
	bool WayTree::addNode(Direction direction)
	{
		// determine direction 
		switch (direction)
		{
		case Direction::Up:
		{
			if (currentPosition->UpNode == nullptr)
			{
				currentPosition->UpNode = new WayNode();
				currentPosition->UpNode->DownNode = currentPosition;
				currentPosition->UpNode->coordinates = std::pair<int, int>(currentPosition->coordinates.first, currentPosition->coordinates.second + 1);
				return true;
			}
			else
			{
				return false;
			}
		}break;
		case Direction::Down:
		{
			if (currentPosition->DownNode == nullptr)
			{
				currentPosition->DownNode = new WayNode();
				currentPosition->DownNode->UpNode = currentPosition;
				currentPosition->DownNode->coordinates = std::pair<int, int>(currentPosition->coordinates.first, currentPosition->coordinates.second - 1);
				return true;
			}
			else
			{
				return false;
			}
		}break;
		case Direction::Left:
		{
			if (currentPosition->LeftNode == nullptr)
			{
				currentPosition->LeftNode = new WayNode();
				currentPosition->LeftNode->RightNode = currentPosition;
				currentPosition->LeftNode->coordinates = std::pair<int, int>(currentPosition->coordinates.first - 1, currentPosition->coordinates.second);
				return true;
			}
			else
			{
				return false;
			}
		}break;
		case Direction::Right:
		{
			if (currentPosition->RightNode == nullptr)
			{
				currentPosition->RightNode = new WayNode();
				currentPosition->RightNode->LeftNode = currentPosition;
				currentPosition->RightNode->coordinates = std::pair<int, int>(currentPosition->coordinates.first + 1, currentPosition->coordinates.second);
				return true;
			}
			else
			{
				return false;
			}
		}break;
		}
	}

	/// <summary>
	/// method to find element in way tree by recursion
	/// </summary>
	/// <param name="coordiantes">coordinates that's using for searching</param>
	/// <param name="node">node that base for searching</param>
	/// <returns>finded node with coordinates</returns>
	WayNode* WayTree::findNodeByCoordiantes(std::pair<int, int> coordiantes, WayNode* node)
	{
		if (node == nullptr)
		{
			node = currentPosition;
		}
		if (node->coordinates == coordiantes)
		{
			return node;
		}
		if (node->UpNode != nullptr)
		{
			findNodeByCoordiantes(coordiantes, node->UpNode);
		}
		if (node->DownNode != nullptr)
		{
			findNodeByCoordiantes(coordiantes, node->DownNode);
		}
		if (node->LeftNode != nullptr)
		{
			findNodeByCoordiantes(coordiantes, node->LeftNode);
		}
		if (node->RightNode != nullptr)
		{
			findNodeByCoordiantes(coordiantes, node->RightNode);
		}
		return nullptr;
	}

	void WayTree::updateAllCoordiantes(int up, int down, int left, int right, WayNode* node)
	{
		if (node == nullptr)
		{
			node = currentPosition;
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
}