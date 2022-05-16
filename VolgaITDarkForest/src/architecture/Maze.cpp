#include "architecture/Maze.hpp"

namespace architecture
{
	Maze::Maze()
	{
		m_fairiland = new Fairyland();
		m_king = new Person(m_fairiland, Character::Ivan);
		m_queen = new Person(m_fairiland, Character::Elena);
	}

	void Maze::startSearchingMeetPath()
	{

	}

	void Maze::makeMoove()
	{
		Direction kingDirection = m_king->determineMooveParameters();
		Direction queenDirection = m_queen->determineMooveParameters();



	}

	Maze::~Maze()
	{
		delete m_king;
		delete m_queen;
		delete m_fairiland;
	}
}