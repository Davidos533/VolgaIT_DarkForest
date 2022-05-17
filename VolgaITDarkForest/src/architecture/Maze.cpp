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
		makeMoove();
	}

	void Maze::makeMoove()
	{
		Direction kingDirection;
		Direction queenDirection;
		bool isSuccessfully;
		int stepNumber = 0;

		do
		{
			kingDirection = m_king->determineMooveParameters();
			queenDirection = m_queen->determineMooveParameters();

			isSuccessfully = m_fairiland->go(kingDirection, queenDirection);

			m_king->goToDirection(kingDirection);
			m_queen->goToDirection(queenDirection);

			std::cerr << stepNumber<<":\t" << "k:" << static_cast<char>(kingDirection) << "\tq:" << static_cast<char>(queenDirection) << "\n";
			
			stepNumber++;

		} while ((kingDirection != Direction::Pass || queenDirection!= Direction::Pass) && !isSuccessfully);

		std::cerr << m_fairiland->getTurnCount()<<"\n";
		std::cerr << (isSuccessfully ? "Meeting successfully" : "No meeting") << "\n";
	}

	Maze::~Maze()
	{
		delete m_king;
		delete m_queen;
		delete m_fairiland;
	}
}