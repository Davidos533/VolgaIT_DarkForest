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

	public:
		explicit Maze();
		Maze(const Maze& copy) = delete;

		void startSearchingMeetPath();

		virtual ~Maze();

	private:
		void makeMoove();

	};
}
#endif