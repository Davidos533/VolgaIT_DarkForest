#ifndef PERSON_HPP
#define PERSON_HPP

#include "common/api/fairy_tail.hpp"

namespace architecture
{
	class Person
	{
	private:
		Character m_character;
		Fairyland& m_fairyland;
		Direction m_currentMooveDirection;
		Direction m_avoidDirection;

	public:
		Person(Fairyland& fairyland, Character Character, Direction startDirection = Direction::Up);

		Person() = delete;
		Person(const Person& object) = delete;

		/// <summary>
		/// method to moove person on a fairyland
		/// </summary>
		void moove();
		Direction getDirection() const;

		~Person();
	private:
		Direction switchToNextDirection();
		Direction getOppositeDirection(Direction direction);
		bool canGo(Direction direction);
		Direction chooseDirection(Direction firstDirection, Direction secondDirection, Direction oppositeDirection);

	};
}
#endif