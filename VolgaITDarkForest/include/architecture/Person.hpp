#ifndef PERSON_HPP
#define PERSON_HPP

#include <vector>
#include <list>
#include <string>

#include "common/api/fairy_tail.hpp"
#include "WayTree.hpp"
namespace architecture
{
	class Person
	{
	private:
		
		/// <summary>
		/// character type
		/// </summary>
		Character m_character;

		/// <summary>
		/// maze (map)
		/// </summary>
		Fairyland* m_fairyland;

		/// <summary>
		/// person move graph
		/// </summary>
		WayTree* m_wayTree;

		/// <summary>
		/// contains person start position
		/// </summary>
		WayNode* m_startPosition;
		
		/// <summary>
		/// person previous move direction
		/// </summary>
		Direction m_previousDirection;

		/// <summary>
		/// show how much not discovered(explored) nodes(branches) left
		/// </summary>
		std::list<Position>* m_coordiantesOfNotDiscoveredPosition;
		
		/// <summary>
		/// contains path from current position to nearest not discovered node
		/// </summary>
		std::list<Direction>* m_waySequence;

	public:
		explicit Person(Fairyland* fairyland, Character Character);

		Person() = delete;
		Person(const Person& object) = delete;

		/// <summary>
		/// getter
		/// </summary>
		/// <returns>provide current person position</returns>
		Position getCurrentPosition();

		/// <summary>
		/// getter
		/// </summary>
		/// <returns>provide person max min indents by oX and oY</returns>
		models::PersonIndents& getPersonIndents();

		/// <summary>
		/// setter
		/// </summary>
		/// <param name="waySequence">way sequnce for set in person way sequence</param>
		void setWaySequence(std::list<Direction>* waySequence);

		/// <summary>
		/// find shortest way between current object position and given position
		/// </summary>
		/// <param name="position">given position</param>
		/// <returns>list(front) with directions from current position to given</returns>
		std::list<Direction>* findShortestWayToPositionFromCurrent(Position position);

		/// <summary>
		/// main method - path scheduler, determine where should move persone by went path
		/// </summary>
		/// <returns>move direction</returns>
		Direction determineMooveParameters();

		/// <summary>
		/// checks is person can go to direction
		/// </summary>
		/// <param name="direction">direction for check</param>
		/// <returns>is user can go</returns>
		bool isCanGo(Direction direction);

		/// <summary>
		/// checks is discovered (is haves node) in given direction
		/// </summary>
		/// <param name="direction">direction for check</param>
		/// <returns>is discovered direction</returns>
		bool isDiscoveredDirection(Direction direction);

		/// <summary>
		/// moove persone to concrete direction
		/// </summary>
		/// <param name="direction">mooveable direction</param>
		void goToDirection(Direction direction);

		/// <summary>
		/// provide view of person went path
		/// </summary>
		/// <returns></returns>
		std::vector<std::string>* getMapView(int xSize, int ySize);

		/// <summary>
		/// gets reversed (opposite) direction of given direction
		/// </summary>
		/// <param name="direction">given direction</param>
		/// <returns>reversed direction</returns>
		Direction getReversedDirection(Direction direction);

		virtual ~Person();
	private:

		/// <summary>
		/// add coordinates to list not discovered nodes(squares)
		/// </summary>
		/// <param name="position"></param>
		void emplaceCoordinatesIfNotExist(Position position);
	};
}
#endif