#include "architecture/Maze.hpp"

namespace architecture
{
	Maze::Maze(int xSize, int ySize)
		:m_xSize(xSize), m_ySize(ySize)
	{
		m_fairiland = new Fairyland();
		m_king = new Person(m_fairiland, Character::Ivan);
		m_queen = new Person(m_fairiland, Character::Elena);
	}

	void Maze::startSearchingMeetPath()
	{
		moove();
	}

	void Maze::moove()
	{
		Direction kingDirection;
		Direction queenDirection;
		bool isSuccessfullyMeeting;
		bool isMeetingWayBuilded = false;



		int stepNumber = 1;

		do
		{
			kingDirection = m_king->determineMooveParameters();
			queenDirection = m_queen->determineMooveParameters();

			isSuccessfullyMeeting = m_fairiland->go(kingDirection, queenDirection);
			
			bool isKingCoordinatesStabilized = false;
			bool isQueenCoordinatesStabilized = false;

			if (!isMeetingWayBuilded)
			{
				// check total indent by oX and oY is equals maze sizes
				isKingCoordinatesStabilized = isCoordinatesStabilized(m_king->getPersonIndents());
				isQueenCoordinatesStabilized = isCoordinatesStabilized(m_king->getPersonIndents());
			}
			
			// total indent are equals maze sizes for king and queen
			if (isKingCoordinatesStabilized && isQueenCoordinatesStabilized)
			{
				// build path from king to queen and put it's to scheduler
				// calculate coordinates from queen graph system for king graph system
				Position queenPosition = m_queen->getCurrentPosition();

				Position queenPositionForKing;

				if (m_king->getPersonIndents().maxIndentX > m_queen->getPersonIndents().maxIndentX)
				{
					queenPositionForKing.first = queenPosition.first - (m_queen->getPersonIndents().maxIndentX - m_king->getPersonIndents().maxIndentX);
				}
				else
				{
					queenPositionForKing.first = queenPosition.first + (m_king->getPersonIndents().maxIndentX - m_queen->getPersonIndents().maxIndentX);
				}
				if (m_king->getPersonIndents().maxIndentY > m_queen->getPersonIndents().maxIndentY)
				{
					queenPositionForKing.second = queenPosition.second - (m_queen->getPersonIndents().maxIndentY - m_king->getPersonIndents().maxIndentY);
				}
				else
				{
					queenPositionForKing.second = queenPosition.second + (m_king->getPersonIndents().maxIndentY - m_queen->getPersonIndents().maxIndentY);
				}

				std::list<Direction>* waySequenceForKing = m_king->findShortestWayToPositionFromCurrent(queenPositionForKing);
				m_king->setWaySequence(waySequenceForKing);
				std::list<Direction>* waySequenceForQueen = new std::list<Direction>(*waySequenceForKing);
				waySequenceForQueen->reverse();

				for (auto& element : *waySequenceForQueen)
				{
					element = m_queen->getReversedDirection(element);
				}

				m_queen->setWaySequence(waySequenceForQueen);

				isMeetingWayBuilded = true;
			}

			std::cerr << stepNumber << ":\t" << "k:" << static_cast<char>(kingDirection) << "\tq:" << static_cast<char>(queenDirection) << "\n";

			stepNumber++;

		} while ((kingDirection != Direction::Pass || queenDirection != Direction::Pass) && !isSuccessfullyMeeting);

		std::cerr << (isSuccessfullyMeeting ? "Meeting successfully" : "Meeting is impossible") << "\n";
		std::cerr << "Count of steps:" << m_fairiland->getTurnCount() << "\n";

		auto kingMapView = m_king->getMapView(m_xSize, m_ySize);
		auto queenMapView = m_queen->getMapView(m_xSize, m_ySize);

		std::cerr << "\nking map:\nX - start position\n";
		for (auto& row : *kingMapView)
		{
			std::cerr << row << "\n";
		}

		std::cerr << "\nqueen map:\nX mark - start position\n";
		for (auto& row : *queenMapView)
		{
			std::cerr << row << "\n";
		}

		delete kingMapView;
		delete queenMapView;
	}

	bool Maze::isCoordinatesStabilized(models::PersonIndents& indents)
	{
		return std::abs(indents.maxIndentX) + std::abs(indents.minIndentX) == m_xSize-1 &&
			std::abs(indents.maxIndentY)+std::abs(indents.minIndentY) == m_ySize-1;
	}

	Maze::~Maze()
	{
		delete m_king;
		delete m_queen;
		delete m_fairiland;
	}
}