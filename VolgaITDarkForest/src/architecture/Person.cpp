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
		Direction reversedDirection = Direction::Pass;
		Position currentPosition = m_wayTree->getCurrentPosition()->coordinates;

		// delete current position from not discovered positions list if it contains there
		m_coordiantesOfNotDiscoveredPosition->remove(currentPosition);

		reversedDirection = getReversedDirection(m_previousDirection);

		Direction choicenDirection = Direction::Pass;

		if (m_waySequence != nullptr && m_waySequence->size() > 0)
		{
			choicenDirection = m_waySequence->front();
			m_waySequence->pop_front();
		}

		if (m_waySequence != nullptr && m_waySequence->size() == 0)
		{
			delete m_waySequence;
			m_waySequence = nullptr;
		}

		// up direction will discover first
		if (reversedDirection != Direction::Up && !isDiscoveredDirection(Direction::Up) && isCanGo(Direction::Up))
		{
			choicenDirection = Direction::Up;
		}
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

				if (waySequence->size() < minPathLenght || minPathLenght == -1)
				{
					if (m_waySequence != nullptr)
					{
						delete m_waySequence;
					}

					minPathLenght = waySequence->size();
					m_waySequence = waySequence;
				}
				else
				{
					delete waySequence;
				}
			}

			if (m_waySequence != nullptr && m_waySequence->size() > 0)
			{
				choicenDirection = m_waySequence->front();
				m_waySequence->pop_front();
			}
		}

		m_previousDirection = choicenDirection;
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

	void Person::pushDirectionToWaySequence(Direction direction)
	{
		m_waySequence->push_front(direction);
	}

	void Person::popDirectionFromWaySequence()
	{
		m_waySequence->pop_front();
	}

	Direction Person::topDirectionInWaySequence()
	{
		return m_waySequence->front();
	}

	bool Person::isCanGo(Direction direction)
	{
		return m_fairyland->canGo(m_character, direction);
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

	Person::~Person()
	{
		if (m_coordiantesOfNotDiscoveredPosition != nullptr)
		{
			delete m_coordiantesOfNotDiscoveredPosition;
		}

		if (m_waySequence != nullptr)
		{
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

	std::vector<std::string>* Person::getMapView()
	{
		std::vector<std::string> *mapView = new std::vector<std::string>(10);

		std::map<Position, WayNode*>* nodesMap = m_wayTree->getNodesMap();

		int maxX = 0, maxY = 0, minX = 10, minY = 10, x, y;

		for (auto& element : *nodesMap)
		{
			x = element.first.first;
			y = element.first.second;

			if (x > maxX)
			{
				maxX = x;
			}
			if (y > maxY)
			{
				maxY = y;
			}
			if (x < minX)
			{
				minX = x;
			}
			if (y < minY)
			{
				minY = y;
			}
		}

		for (int y = 9; y >= 0; y--)
		{
			std::string row = "";
			for (int x = 0; x < 10; x++)
			{
				auto element = nodesMap->find(Position(x + minX, y + minY));
				
				if (m_startPosition->coordinates.first +minX == x && m_startPosition->coordinates.second  + minY == y)
				{
					row.append("@");
				}
				else if (element != nodesMap->end())
				{
					row.append(".");
				}
				else
				{
					row.append("?");
				}
			}
			mapView->push_back(row);
		}

		return mapView;
	}
}