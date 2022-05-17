#include "architecture/Person.hpp"

namespace architecture
{
	Person::Person(Fairyland* fairyland, Character character)
		:m_fairyland(fairyland), m_character(character)
	{
		m_wayTree = new WayTree();
		m_waySequence = new std::stack<Direction>();
		m_startPosition = m_wayTree->getCurrentPosition();
		m_coordiantesOfNotDiscoveredPosition = new std::list<Position>();
	}

	Direction Person::determineMooveParameters()
	{
		Direction reversedDirection = Direction::Pass;
		Position currentPosition = m_wayTree->getCurrentPosition()->coordinates;
		
		// delete current position from not discovered positions list if it contains there
		m_coordiantesOfNotDiscoveredPosition->remove(currentPosition);
		
		// way sequence has moves hostory
		if (m_waySequence->size() > 0)
		{
			reversedDirection = getReversedDirection(m_waySequence->top());
		}
		
		Direction choicenDirection = Direction::Pass;

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
				emplaceCoordinatesIfNotExist(Position(currentPosition.first -1 , currentPosition.second));
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
		if(choicenDirection == Direction::Pass && m_coordiantesOfNotDiscoveredPosition->size() > 0 && m_waySequence->size()!=0)
		{
			choicenDirection = reversedDirection;
			m_waySequence->pop();
		}
		// person moved therefore add moved direction to way sequence
		else if(choicenDirection != Direction::Pass)
		{
			m_waySequence->push(choicenDirection);
		}

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
		m_waySequence->push(direction);
	}

	void Person::popDirectionFromWaySequence()
	{
		m_waySequence->pop();
	}

	Direction Person::topDirectionInWaySequence()
	{
		return m_waySequence->top();
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
		delete m_coordiantesOfNotDiscoveredPosition;
		delete m_waySequence;
		delete m_wayTree;
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
			default: 
			{
				throw std::runtime_error("unknow direction for reversing");
			}
		}
	}
}