#ifndef MAZE_HPP
#define MAZE_HPP

#include "common/api/fairy_tail.hpp"
#include "architecture/Person.hpp"

namespace architecture
{
	class Maze
	{
	private:
		Fairyland *m_fairiland;
		Person *m_king;
		Person *m_queen;
		
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
	};
}
#endif