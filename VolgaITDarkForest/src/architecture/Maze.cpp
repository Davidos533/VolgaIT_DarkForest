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

			// king and queen discovered all maze
			if (kingDirection == Direction::Pass && queenDirection == Direction::Pass)
			{
				// try to make meeting
				buildAndSetPathBetweenPersons(*m_king, *m_queen);

				// again get directions
				kingDirection = m_king->determineMooveParameters();
				queenDirection = m_queen->determineMooveParameters();
			}

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
				buildAndSetPathBetweenPersons(*m_king, *m_queen);
				isMeetingWayBuilded = true;
			}

			std::cerr << stepNumber << ":\t" << "k:" << static_cast<char>(kingDirection) << "\tq:" << static_cast<char>(queenDirection) << "\n";

			stepNumber++;

		} while ((kingDirection != Direction::Pass || queenDirection != Direction::Pass) && !isSuccessfullyMeeting);

		Direction kingPreviousDirection = m_king->getPersonPreviousDirection();
		Direction queenPreviousDirecionReversed = Person::getReversedDirection(m_queen->getPersonPreviousDirection());

		enums::MapViewParseOffset meetingPositionsOffset = enums::MapViewParseOffset::WithoutOffset;

		// determine person meeting in one point or meeting while exchanging positions
		// it's need only when persons meeting by moved in opposite directions when meet
		// to do this need to spend another step in maze
		if (isSuccessfullyMeeting && kingPreviousDirection == queenPreviousDirecionReversed)
		{
			// roll back persons by positions
			isSuccessfullyMeeting = m_fairiland->go(Person::getReversedDirection(kingPreviousDirection), queenPreviousDirecionReversed);

			// if persons meeting again therefore they met by positions exchanging
			if (isSuccessfullyMeeting)
			{
				// determine coordinate axis to determine positions offset
				// offset by oY beacuse direcion up down by oY
				if (kingPreviousDirection == Direction::Up || kingPreviousDirection == Direction::Down)
				{
					meetingPositionsOffset = enums::MapViewParseOffset::OffsetByY;
				}
				// offset by oX beacuse direcion left right by oX
				else
				{
					meetingPositionsOffset = enums::MapViewParseOffset::OffsetByY;
				}
			}
		}

		std::cerr << (isSuccessfullyMeeting ? "Meeting successfully" : "Meeting is impossible") << "\n";
		std::cerr << "Count of steps:" << m_fairiland->getTurnCount() << "\n";

		auto kingMapView = m_king->getMapView(m_xSize, m_ySize);
		auto queenMapView = m_queen->getMapView(m_xSize, m_ySize);

		std::cerr << "\nking map:\n";
		std::cerr << kingMapView << "\n";

		std::cerr << "\nqueen map:\n";
		std::cerr << queenMapView << "\n";

		std::cerr << "\nmerged map:\n@ - king, & - queen\n";
		std::cerr << getMergedMapView(meetingPositionsOffset) << "\n";
	}

	bool Maze::isCoordinatesStabilized(models::PersonIndents& indents)
	{
		return std::abs(indents.maxIndentX) + std::abs(indents.minIndentX) == m_xSize - 1 &&
			std::abs(indents.maxIndentY) + std::abs(indents.minIndentY) == m_ySize - 1;
	}

	void Maze::buildAndSetPathBetweenPersons(Person& firstPerson, Person& secondPerson)
	{
		// build path from king to queen and put it's to scheduler
		// calculate coordinates from queen graph system for king graph system
		Position firstPersonPosition = secondPerson.getCurrentPosition();

		Position firstPersonPositionForSecond;

		if (firstPerson.getPersonIndents().maxIndentX > secondPerson.getPersonIndents().maxIndentX)
		{
			firstPersonPositionForSecond.first = firstPersonPosition.first - (secondPerson.getPersonIndents().maxIndentX - firstPerson.getPersonIndents().maxIndentX);
		}
		else
		{
			firstPersonPositionForSecond.first = firstPersonPosition.first + (firstPerson.getPersonIndents().maxIndentX - secondPerson.getPersonIndents().maxIndentX);
		}
		if (firstPerson.getPersonIndents().maxIndentY > secondPerson.getPersonIndents().maxIndentY)
		{
			firstPersonPositionForSecond.second = firstPersonPosition.second - (secondPerson.getPersonIndents().maxIndentY - firstPerson.getPersonIndents().maxIndentY);
		}
		else
		{
			firstPersonPositionForSecond.second = firstPersonPosition.second + (firstPerson.getPersonIndents().maxIndentY - secondPerson.getPersonIndents().maxIndentY);
		}

		std::list<Direction>* waySequenceSecondPerson = firstPerson.findShortestWayToPositionFromCurrent(firstPersonPositionForSecond);
		firstPerson.setWaySequence(waySequenceSecondPerson);
		std::list<Direction>* waySequenceFirstPerson = new std::list<Direction>(*waySequenceSecondPerson);
		waySequenceFirstPerson->reverse();

		for (auto& element : *waySequenceFirstPerson)
		{
			element = secondPerson.getReversedDirection(element);
		}

		secondPerson.setWaySequence(waySequenceFirstPerson);

	}

	models::PositionsDifference Maze::getStartPositionsDifferenceByMeetingPositionForPersons(Person& firstPerson, Person& secondPerson)
	{
		models::PositionsDifference differenceByCurrent;

		// get's current positions
		Position firstPersonCurrentPosition = firstPerson.getCurrentPosition();
		Position secondPersonCurrentPosition = secondPerson.getCurrentPosition();

		// calculating difference by current positions
		differenceByCurrent.byOx = firstPersonCurrentPosition.first - secondPersonCurrentPosition.first;
		differenceByCurrent.byOy = firstPersonCurrentPosition.second - secondPersonCurrentPosition.second;

		return differenceByCurrent;
	}

	std::string Maze::getMergedMapView(enums::MapViewParseOffset parseOffset)
	{
		int offsetX = parseOffset == enums::MapViewParseOffset::WithoutOffset ? 0 : parseOffset == enums::MapViewParseOffset::OffsetByY ? 0 : 1;
		int offsetY = parseOffset == enums::MapViewParseOffset::WithoutOffset ? 0 : parseOffset == enums::MapViewParseOffset::OffsetByX ? 0 : 1;

		// parsed map view
		std::string mapView;

		// get's all nodes of every person contained in map (was collected by every person while discovering maze)
		std::map<Position, WayNode*>* kingNodesMap = m_king->getWayTreeNodesMap();
		std::map<Position, WayNode*>* queenNodesMap = m_queen->getWayTreeNodesMap();

		// get persons start positions
		Position kingStartPosition = m_king->getPersonStartPosition();
		Position queenStartPosition = m_queen->getPersonStartPosition();

		//this works only if this method was called after successfully meeting
		Position kingMeetingPosition = m_king->getCurrentPosition();
		Position queenMeetingPosition = m_queen->getCurrentPosition();

		// get local indents for every person
		models::PersonIndents kingIndents = m_king->getPersonIndents();
		models::PersonIndents queenIndents = m_queen->getPersonIndents();

		// calculate position difference by meeting point to merge first person map view and second person map view
		models::PositionsDifference positionDifferenceForKing = getStartPositionsDifferenceByMeetingPositionForPersons(*m_king, *m_queen);
		positionDifferenceForKing.byOx += offsetX;
		positionDifferenceForKing.byOy += offsetY;

		Position kingStartPositionInMapCoords = Position(kingStartPosition.first - kingIndents.minIndentX + 1, kingStartPosition.second - kingIndents.minIndentY + 1);
		Position queenStartPositionInMapCoords = Position(queenStartPosition.first - kingIndents.minIndentX + 1 + positionDifferenceForKing.byOx, queenStartPosition.second - kingIndents.minIndentY + 1 + positionDifferenceForKing.byOy);

		models::PersonIndents globalIndents;
		
		globalIndents.maxIndentX = kingIndents.maxIndentX >= queenIndents.maxIndentX ? kingIndents.maxIndentX : queenIndents.maxIndentX;
		globalIndents.maxIndentY = kingIndents.maxIndentY >= queenIndents.maxIndentY ? kingIndents.maxIndentY : queenIndents.maxIndentY;
		globalIndents.minIndentX = kingIndents.minIndentX <= queenIndents.minIndentX ? kingIndents.minIndentX : queenIndents.minIndentX;
		globalIndents.minIndentY = kingIndents.minIndentY >= queenIndents.minIndentY ? kingIndents.minIndentY : queenIndents.minIndentY;

		// parse nodes to map view
		for (int y = m_ySize + 1; y >= 0; y--)
		{
			if (y < m_ySize + 1 && y>0)
			{
				mapView.append(std::to_string(y - 1));
			}
			else
			{
				mapView.append(" ");
			}

			for (int x = 0; x < m_xSize + 2; x++)
			{
				if (x == 0 || x == m_xSize + 1 || y == 0 || y == m_ySize + 1)
				{
					mapView.append(" ");
				}
				else
				{
					if (kingStartPositionInMapCoords.first == x && kingStartPositionInMapCoords.second == y)
					{
						mapView.append("@");
					}
					else if (queenStartPositionInMapCoords.first == x && queenStartPositionInMapCoords.second == y)
					{
						mapView.append("&");
					}
					else
					{
						auto element = kingNodesMap->find(Position(x + kingIndents.minIndentX - 1, y + kingIndents.minIndentY - 1));

						if (element == kingNodesMap->end())
						{
							auto element = queenNodesMap->find(Position(x + kingIndents.minIndentX - 1 - positionDifferenceForKing.byOx, y + kingIndents.minIndentY - 1 - positionDifferenceForKing.byOy));
							if (element == queenNodesMap->end())
							{
								mapView.append("?");
							}
							else if (element->second->isBarrier)
							{
								mapView.append("#");
							}
							else
							{
								mapView.append(".");
							}
						}
						else if (element->second->isBarrier)
						{
							mapView.append("#");
						}
						else
						{
							mapView.append(".");
						}
					}
				}
			}

			if (y != 0)
			{
				mapView += "\n";
			}
		}

		mapView += "\n  0123456789";

		return mapView;
	}

	Maze::~Maze()
	{
		delete m_king;
		delete m_queen;
		delete m_fairiland;
	}
}