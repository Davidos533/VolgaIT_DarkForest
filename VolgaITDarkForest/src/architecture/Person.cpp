#include "architecture/Person.hpp"

namespace architecture
{
	Person::Person(Fairyland& fairyland, Character character, Direction startDirection)
		:m_fairyland(fairyland), m_character(character), m_currentMooveDirection(startDirection)
	{

	}

	void Person::moove()
	{
		while (!m_fairyland.canGo(m_character, m_currentMooveDirection))
		{
			m_currentMooveDirection = switchToNextDirection();
		}
	}

	Direction Person::switchToNextDirection()
	{
		switch (m_currentMooveDirection)
		{
		case Direction::Up:
		case Direction::Down:
		{
			return chooseDirection(Direction::Left, Direction::Right, getOppositeDirection(m_currentMooveDirection));
		}
		case Direction::Left:
		case Direction::Right:
		{
			return chooseDirection(Direction::Up, Direction::Down, getOppositeDirection(m_currentMooveDirection));
		}
		}
	}

	Direction Person::chooseDirection(Direction firstDirection, Direction secondDirection, Direction oppositeDirection)
	{
		if (canGo(firstDirection))
		{
			return firstDirection;
		}
		else if (canGo(secondDirection))
		{
			return secondDirection;
		}
		else
		{
			return oppositeDirection;
		}
	}

	bool Person::canGo(Direction direction)
	{
		return m_fairyland.canGo(m_character, direction);
	}

	Direction Person::getOppositeDirection(Direction direction)
	{
		switch (direction)
		{
		case Direction::Up: return Direction::Down;
		case Direction::Down: return Direction::Up;
		case Direction::Right: return Direction::Left;
		case Direction::Left: return Direction::Right;
		}
	}

	Direction Person::getDirection() const
	{
		return m_currentMooveDirection;
	}

	Person::~Person()
	{

	}
}