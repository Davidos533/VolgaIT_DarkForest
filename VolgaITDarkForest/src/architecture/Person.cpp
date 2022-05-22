#include "architecture/Person.hpp"

namespace architecture
{
	Person::Person(Fairyland* fairyland, Character character)
		:m_fairyland(fairyland), m_character(character)
	{
		m_wayTree = new WayTree();
		m_waySequence = nullptr;
		m_startPosition = m_wayTree->getCurrentPosition();
		m_coordiantesOfNotDiscoveredPosition = new std::list<Position>();
		m_previousDirection = Direction::Pass;
	}

	Direction Person::determineMooveParameters()
	{
		// set default value
		Direction reversedDirection = Direction::Pass;

		// get current position
		Position currentPosition = m_wayTree->getCurrentPosition()->coordinates;

		// delete current position from not discovered positions list if it contains there
		m_coordiantesOfNotDiscoveredPosition->remove(currentPosition);

		// get reversed direction (direction tats need to roll back position) by previous vove direction
		reversedDirection = getReversedDirection(m_previousDirection);

		// set choicen direction default value
		Direction choicenDirection = Direction::Pass;

		// if way sequence not equals nullptr is determinied and it's size more than zero get move direction from it and trim size
		if (m_waySequence != nullptr && m_waySequence->size() > 0)
		{
			choicenDirection = m_waySequence->front();
			m_waySequence->pop_front();
		}

		// if way sequence not equals nullptr is determinied and it's size equals zero, free memory and set value for it nullptr
		if (m_waySequence != nullptr && m_waySequence->size() == 0)
		{
			m_waySequence->clear();
			delete m_waySequence;
			m_waySequence = nullptr;
		}

		
		// next path scheduler will check directions by that's rules:
		// will move in direction if: 
		// - this direction not equals reversedDirection (direction to roll back to previous position)
		// - direction not discovered
		// - person can go in this direction

		// if person don't go to direction but it not discovered and can go there person will save this position ...
		// ... to come back later when person will get in position where all directions not avaliable and/or are discovered

		// try to go up
		if (reversedDirection != Direction::Up && !isDiscoveredDirection(Direction::Up) && isCanGo(Direction::Up))
		{
			if (choicenDirection != Direction::Pass)
			{
				emplaceCoordinatesIfNotExist(Position(currentPosition.first, currentPosition.second + 1));
			}
			else
			{
				choicenDirection = Direction::Up;
			}
		}

		// try to go down
		if (reversedDirection != Direction::Down && !isDiscoveredDirection(Direction::Down) && isCanGo(Direction::Down))
		{
			if (choicenDirection != Direction::Pass)
			{
				emplaceCoordinatesIfNotExist(Position(currentPosition.first, currentPosition.second - 1));
			}
			else
			{
				choicenDirection = Direction::Down;
			}
		}

		// try to go left
		if (reversedDirection != Direction::Left && !isDiscoveredDirection(Direction::Left) && isCanGo(Direction::Left))
		{
			if (choicenDirection != Direction::Pass)
			{
				emplaceCoordinatesIfNotExist(Position(currentPosition.first - 1, currentPosition.second));
			}
			else
			{
				choicenDirection = Direction::Left;
			}
		}

		// try to go right
		if (reversedDirection != Direction::Right && !isDiscoveredDirection(Direction::Right) && isCanGo(Direction::Right))
		{
			if (choicenDirection != Direction::Pass)
			{
				emplaceCoordinatesIfNotExist(Position(currentPosition.first + 1, currentPosition.second));
			}
			else
			{
				choicenDirection = Direction::Right;
			}
		}

		// person can't find any not discovered direction therefore should check way sequence stack ...
		// ... if person have no discovered directions if person can return to position with not discovered directions, it will provide opportunity ...
		// ... to discover no discovered nodes
		if (m_waySequence == nullptr && choicenDirection == Direction::Pass && m_coordiantesOfNotDiscoveredPosition->size() > 0)
		{
			int minPathLenght = -1;

			// from all remembered not discovered positions determine nearest position, and change behavior to go there
			for (auto& element : *m_coordiantesOfNotDiscoveredPosition)
			{
				std::list<Direction>* waySequence = m_wayTree->findShortestWayToPositionFromCurrent(element);
				if (waySequence != nullptr)
				{
					waySequence->pop_back();

					if (waySequence->size() < minPathLenght || minPathLenght == -1)
					{
						if (m_waySequence != nullptr)
						{
							m_waySequence->clear();
							delete m_waySequence;
						}

						minPathLenght = waySequence->size();
						m_waySequence = waySequence;
					}
					else
					{
						waySequence->clear();
						delete waySequence;
					}
				}
			}

			// get direction from new builded way
			if (m_waySequence != nullptr && m_waySequence->size() > 0)
			{
				choicenDirection = m_waySequence->front();
				m_waySequence->pop_front();
			}
		}

		// security from not inaccessible direction
		// person can't go in choicen direction set as direction Pass
		if (!isCanGo(choicenDirection))
		{
			choicenDirection = Direction::Pass;
		}

		// set previous direction
		m_previousDirection = choicenDirection;

		// move person in graph data structure
		goToDirection(choicenDirection);

		return choicenDirection;
	}

	void Person::emplaceCoordinatesIfNotExist(Position position)
	{
		if (std::find(m_coordiantesOfNotDiscoveredPosition->begin(), m_coordiantesOfNotDiscoveredPosition->end(), position)
			== m_coordiantesOfNotDiscoveredPosition->end())
		{
			m_coordiantesOfNotDiscoveredPosition->push_back(position);
		}
	}

	bool Person::isCanGo(Direction direction)
	{
		bool isCanGo = m_fairyland->canGo(m_character, direction);

		////  can't go tehre, add node as a barrier 
		if (!isCanGo)
		{
			m_wayTree->tryAddBarrierToDirection(direction);
		}

		return isCanGo;
	}

	bool Person::isDiscoveredDirection(Direction direction)
	{
		switch (direction)
		{
			case Direction::Up:
			{
				return m_wayTree->getCurrentPosition()->UpNode != nullptr;
			}
			case Direction::Down:
			{
				return m_wayTree->getCurrentPosition()->DownNode != nullptr;
			}
			case Direction::Left:
			{
				return m_wayTree->getCurrentPosition()->LeftNode != nullptr;
			}
			case Direction::Right:
			{
				return m_wayTree->getCurrentPosition()->RightNode != nullptr;
			}
			default: throw std::runtime_error("unknown direction");
		}
	}

	void Person::goToDirection(Direction direction)
	{
		m_wayTree->addOrSetNodeInConcreteDirection(direction);
	}

	std::list<Direction>* Person::findShortestWayToPositionFromCurrent(Position position)
	{
		return m_wayTree->findShortestWayToPositionFromCurrent(position);
	}

	Person::~Person()
	{
		if (m_coordiantesOfNotDiscoveredPosition != nullptr)
		{
			m_coordiantesOfNotDiscoveredPosition->clear();
			delete m_coordiantesOfNotDiscoveredPosition;
		}

		if (m_waySequence != nullptr)
		{
			m_waySequence->clear();
			delete m_waySequence;
		}

		if (m_wayTree != nullptr)
		{
			delete m_wayTree;
		}
	}

	Direction Person::getReversedDirection(Direction direction)
	{
		switch (direction)
		{
			case Direction::Up:
			{
				return Direction::Down;
			}
			case Direction::Down:
			{
				return Direction::Up;
			}
			case Direction::Left:
			{
				return Direction::Right;
			}
			case Direction::Right:
			{
				return Direction::Left;
			}
			case Direction::Pass:
			{
				return Direction::Pass;
			}
			default:
			{
				throw std::runtime_error("unknow direction for reversing");
			}
		}
	}

	models::PersonIndents& Person::getPersonIndents()
	{
		return m_wayTree->getPersonIndents();
	}

	Position Person::getCurrentPosition()
	{
		return m_wayTree->getCurrentPosition()->coordinates;
	}

	Character Person::getCharacter()
	{
		return m_character;
	}

	void Person::setWaySequence(std::list<Direction>* waySequence)
	{
		if (m_waySequence != nullptr)
		{
			m_waySequence->clear();
			delete m_waySequence;
		}

		m_waySequence = waySequence;
	}

	std::map<Position, WayNode*>* Person::getWayTreeNodesMap()
	{
		return m_wayTree->getNodesMap();
	}

	Position Person::getPersonStartPosition()
	{
		return m_startPosition->coordinates;
	}

	Position Person::getPersonPreviousPosition()
	{
		WayNode* currentPosition = m_wayTree->getCurrentPosition();
		
		switch (m_previousDirection)
		{
			case Direction::Up:return currentPosition->DownNode->coordinates;
			case Direction::Down:return currentPosition->UpNode->coordinates;
			case Direction::Left:return currentPosition->RightNode->coordinates;
			case Direction::Right:return currentPosition->LeftNode->coordinates;
			default: return Position(0, 0);
		}
	}

	Direction Person::getPersonPreviousDirection()
	{
		return m_previousDirection;
	}

	int Person::getCountOfPassedUniqueNodes()
	{
		return m_wayTree->getCountOfPassedUniqueNodes();
	}

	std::string Person::getMapView(int xSize, int ySize)
	{
		// parsed map view
		std::string mapView;

		// get nodes map
		std::map<Position, WayNode*>* nodesMap = m_wayTree->getNodesMap();

		// get indents to convert nodes map coordinates to normal coordinates (0,0)
		models::PersonIndents personIndents = m_wayTree->getPersonIndents();

		// parsin by oY from top to bottom
		for (int y = xSize + 1; y >= 0; y--)
		{
			if (y < ySize + 1 && y>0)
			{
				mapView.append(std::to_string(y - 1));
			}
			else
			{
				mapView.append(" ");
			}

			// parsing by oX from left to rigth
			for (int x = 0; x < xSize + 2; x++)
			{
				if (x == 0 || x == xSize + 1 || y == 0 || y == ySize + 1)
				{
					mapView.append(" ");
				}
				else
				{
					auto element = nodesMap->find(Position(x + personIndents.minIndentX - 1, y + personIndents.minIndentY - 1));

					if (element == nodesMap->end())
					{
						mapView.append("?");
					}
					else if (element->second->isBarrier)
					{
						mapView.append("#");
					}
					else if (m_startPosition->coordinates.first == element->second->coordinates.first &&
						m_startPosition->coordinates.second == element->second->coordinates.second)
					{
						mapView.append(m_character == Character::Ivan ? "@" : "&");
					}
					else
					{
						mapView.append(".");
					}
				}

			}

			if (y != 0)
			{
				mapView += "\n";
			}
		}

		// add bottom map legend
		mapView+="\n  0123456789";

		return mapView;
	}
}