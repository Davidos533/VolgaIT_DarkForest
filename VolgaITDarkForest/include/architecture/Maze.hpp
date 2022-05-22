#ifndef MAZE_HPP
#define MAZE_HPP

#include "common/api/fairy_tail.hpp"
#include "architecture/Person.hpp"
#include "models/PositionsDifference.hpp"
#include "architecture/enums/NodeType.hpp"

namespace architecture
{
	class Maze
	{
	private:
		/// <summary>
		/// maze api to manipulate by persons
		/// </summary>
		Fairyland* m_fairyland;

		/// <summary>
		/// king person is Ivan
		/// </summary>
		Person* m_king;

		/// <summary>
		/// queen person is Elena
		/// </summary>
		Person* m_queen;

		/// <summary>
		/// meeting position in coordinate system of Ivan
		/// </summary>
		Position m_meetingKingPosition;

		/// <summary>
		/// meeting position in coordinate system of queen
		/// </summary>
		Position m_meetingQueenPosition;

		/// <summary>
		/// maze size by oX axis
		/// </summary>
		int m_xSize;

		/// <summary>
		/// maze size by oY axis
		/// </summary>
		int m_ySize;

	public:
		explicit Maze(int xSize = 10, int ySize = 10);

		Maze(const Maze& copy) = delete;

		/// <summary>
		/// start meeting path searching and maze exploring
		/// </summary>
		void startSearchingMeetPath();

		virtual ~Maze();

	private:

		/// <summary>
		/// check is coordinates of Person's node graph is stabilized namely sum of absolute values of max and min indents are equals maze size
		/// </summary>
		/// <param name="indents">person indents</param>
		/// <returns>is stabilized</returns>
		bool isCoordinatesStabilized(models::PersonIndents& indents);

		/// <summary>
		/// builds and sets path between two persons for each other to each other (in Person's way sequence)
		/// </summary>
		/// <param name="firstPerson">first persons from here scheduler will build path</param>
		/// <param name="secondPerson">second person to here scheduler will build path</param>
		void buildAndSetPathBetweenPersons(Person &firstPerson, Person &secondPerson);

		/// <summary>
		/// parse merged nodes map view to string for screen map
		/// </summary>
		/// <returns>string with parsed map</returns>
		std::string getMergedMapView(std::map<Position, enums::NodeType>& mergedNodesViewMap);

		/// <summary>
		/// get's positions difference between two persons by meeting fact
		/// </summary>
		/// <param name="firstPerson">first person difference for this</param>
		/// <param name="secondPerson">second person difference by this</param>
		/// <returns>key value pair, first - oX, second oY - differences</returns>
		models::PositionsDifference getStartPositionsDifferenceByMeetingPositionForPersons(Position firstPersonPosition, Position secondPersonPosition);

		/// <summary>
		/// merge node maps of given persons in new map with data for view map
		/// </summary>
		/// <param name="firstPerson">person whose map and coordinate system will as base</param>
		/// <param name="secondPerson">person whose map and coordinate system will merge</param>
		/// <returns>pointer on new merged map (key - Position, value - enum of node type)</returns>
		std::map<Position, enums::NodeType>* mergeNodesMap(Person& firstPerson, Person& secondPerson, Position firstPersonMeetingPosition, Position secondPersonMeetingPosition);
	};
}
#endif