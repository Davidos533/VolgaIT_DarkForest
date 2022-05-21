#ifndef MAZE_HPP
#define MAZE_HPP

#include "common/api/fairy_tail.hpp"
#include "architecture/Person.hpp"
#include "models/PositionsDifference.hpp"
#include "enums/MapViewParseOffset.hpp"

namespace architecture
{
	class Maze
	{
	private:
		Fairyland* m_fairiland;
		Person* m_king;
		Person* m_queen;

		int m_xSize;
		int m_ySize;

	public:
		explicit Maze(int xSize = 10, int ySize = 10);
		Maze(const Maze& copy) = delete;

		void startSearchingMeetPath();

		virtual ~Maze();

	private:

		void moove();

		bool isCoordinatesStabilized(models::PersonIndents& indents);

		void buildAndSetPathBetweenPersons(Person &firstPerson, Person &secondPerson);

		std::string getMergedMapView(enums::MapViewParseOffset parseOffset);

		/// <summary>
		/// get's positions difference between two persons by meeting fact
		/// </summary>
		/// <param name="firstPerson">first person difference for this</param>
		/// <param name="secondPerson">second person difference by this</param>
		/// <returns>key value pair, first - oX, second oY - differences</returns>
		models::PositionsDifference getStartPositionsDifferenceByMeetingPositionForPersons(Person &firstPerson, Person &secondPerson);
	};
}
#endif