#include "architecture/Person.hpp"

namespace architecture
{
	Person::Person(Fairyland* fairyland, Character character)
		:m_fairyland(fairyland), m_character(character)
	{
		m_wayTree = new WayTree();
		m_waySequence = new std::stack<Direction>();
	}

	// TO DO implement method to determine person future direction and moove parameters
	Direction Person::determineMooveParameters()
	{
		Direction reversedDirection = getReversedDirection(m_waySequence->top());
		Direction choicenDirection = Direction::Up;

		if (reversedDirection != Direction::Up && !isDiscoveredDirection(Direction::Up) && isCanGo(Direction::Up))
		{
			choicenDirection = Direction::Up;
		}
		else if (reversedDirection != Direction::Down && !isDiscoveredDirection(Direction::Down) && isCanGo(Direction::Down))
		{
			choicenDirection = Direction::Down;
		}
		else if (reversedDirection != Direction::Left && !isDiscoveredDirection(Direction::Left) && isCanGo(Direction::Left))
		{
			choicenDirection = Direction::Left;
		}
		else if (reversedDirection != Direction::Right && !isDiscoveredDirection(Direction::Right) && isCanGo(Direction::Right))
		{
			choicenDirection = Direction::Right;
		}

		return choicenDirection;
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
		if (!isCanGo(direction))
		{
			throw std::runtime_error("tries to move in not allowed direction");
		}

		m_wayTree->addOrSetNodeInConcreteDirection(direction);
	}

	Person::~Person()
	{
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