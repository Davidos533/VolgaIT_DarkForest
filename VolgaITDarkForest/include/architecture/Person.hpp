#ifndef PERSON_HPP
#define PERSON_HPP

#include <stack>

#include "common/api/fairy_tail.hpp"
#include "WayTree.hpp"
namespace architecture
{
	class Person
	{
	private:
		Character m_character;
		Fairyland* m_fairyland;
		WayTree* m_wayTree;
		
		std::stack<Direction>* m_waySequence;

	public:
		explicit Person(Fairyland* fairyland, Character Character);

		Person() = delete;
		Person(const Person& object) = delete;

		void pushDirectionToWaySequence(Direction direction);

		void popDirectionFromWaySequence();

		Direction topDirectionInWaySequence();

		Direction determineMooveParameters();

		/// <summary>
		/// checks is person can go to direction
		/// </summary>
		/// <param name="direction">direction</param>
		/// <returns>is user can go</returns>
		bool isCanGo(Direction direction);

		/// <summary>
		/// checks is discovered (is haves node) in given direction
		/// </summary>
		/// <param name="direction">direction to check</param>
		/// <returns>is discovered direction</returns>
		bool isDiscoveredDirection(Direction direction);

		/// <summary>
		/// moove persone to concrete direction
		/// 
		/// if direction is not avaliable:
		///		throws exception std::runtime_error
		/// </summary>
		/// <param name="direction">mooveable direction</param>
		void goToDirection(Direction direction);

		virtual ~Person();
	private:

		/// <summary>
		/// gets reversed (opposite) direction of given direction
		/// </summary>
		/// <param name="direction">given direction</param>
		/// <returns>reversed direction</returns>
		Direction getReversedDirection(Direction direction);
	};
}
#endif