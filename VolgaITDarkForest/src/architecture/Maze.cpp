#include "architecture/Maze.hpp"

namespace architecture
{
	Maze::Maze(int xSize, int ySize)
		:m_xSize(xSize), m_ySize(ySize)
	{
		m_fairyland = new Fairyland();
		m_king = new Person(m_fairyland, Character::Ivan);
		m_queen = new Person(m_fairyland, Character::Elena);
	}

	void Maze::startSearchingMeetPath()
	{
		Direction kingDirection;
		Direction queenDirection;
		bool isSuccessfullyMeeting = false;
		bool isMeetingWayBuilded = false;

		int stepNumber = 1;

		do
		{
			// determine move direction of persons and move persons in inside data structure
			kingDirection = m_king->determineMooveParameters();
			queenDirection = m_queen->determineMooveParameters();

			// king and queen discovered all maze, try to build meeting way
			if (!isMeetingWayBuilded && kingDirection == Direction::Pass && queenDirection == Direction::Pass && m_king->getPersonIndents() == m_queen->getPersonIndents())
			{
				// try to make meeting
				buildAndSetPathBetweenPersons(*m_king, *m_queen);

				// again get directions and move persons in inside data structure
				kingDirection = m_king->determineMooveParameters();
				queenDirection = m_queen->determineMooveParameters();

				// set flag that's meeting way already builded
				isMeetingWayBuilded = true;
			}

			if (isMeetingWayBuilded && (kingDirection == Direction::Pass || queenDirection == Direction::Pass))
			{
				break;
			}

			// if one person move and the other does not therefore checks count of passed unique nodes and compares it to determine meeting act possible
			if ((kingDirection == Direction::Pass && m_king->getCountOfPassedUniqueNodes() < m_queen->getCountOfPassedUniqueNodes()) ||
				(queenDirection == Direction::Pass && m_king->getCountOfPassedUniqueNodes() > m_queen->getCountOfPassedUniqueNodes()))
			{
				break;
			}

			if (kingDirection != Direction::Pass || queenDirection != Direction::Pass)
			{
				// get's move result from api
				isSuccessfullyMeeting = m_fairyland->go(kingDirection, queenDirection);
			}

			bool isKingCoordinatesStabilized = false;
			bool isQueenCoordinatesStabilized = false;

			// if meeting way not yet builded trie to determine shouldly build meeting way
			if (!isMeetingWayBuilded)
			{
				// check total indent by oX and oY is equals maze sizes
				isKingCoordinatesStabilized = isCoordinatesStabilized(m_king->getPersonIndents());
				isQueenCoordinatesStabilized = isCoordinatesStabilized(m_queen->getPersonIndents());
			}

			// total indent are equals maze sizes for king and queen
			if (isKingCoordinatesStabilized && isQueenCoordinatesStabilized)
			{
				// build meeting way and set it in pesrons
				buildAndSetPathBetweenPersons(*m_king, *m_queen);

				// set flag that's meeting way already builded
				isMeetingWayBuilded = true;
			}

			// cout move directions
			std::cerr << stepNumber << ":\t" << "k:" << static_cast<char>(kingDirection) << "\tq:" << static_cast<char>(queenDirection) << "\n";

			stepNumber++;

			// if persons are both have pass directions or successfully meeting therefore stop maze discovering cycle
		} while ((kingDirection != Direction::Pass && queenDirection != Direction::Pass) && !isSuccessfullyMeeting);
		
		// get persons directions to will determine is met in one point or met by points exchange
		Direction kingPreviousDirection = m_king->getPersonPreviousDirection();
		Direction queenPreviousDirecionReversed = Person::getReversedDirection(m_queen->getPersonPreviousDirection());

		// set persons meeting position
		m_meetingKingPosition = m_king->getCurrentPosition();
		m_meetingQueenPosition = m_queen->getCurrentPosition();

		// determine person meeting in one point or meeting while exchanging positions
		// it's need only when persons meeting by moved in opposite directions when meet
		// to do this need to spend another step in maze
		if (isSuccessfullyMeeting && kingPreviousDirection == queenPreviousDirecionReversed)
		{
			// roll back persons by positions
			bool isMeeting = m_fairyland->go(Person::getReversedDirection(kingPreviousDirection), queenPreviousDirecionReversed);

			// if persons meeting again therefore they met by positions exchanging
			if (isMeeting)
			{
				// roll back king to old position to set persons in one position and after correctly calculate offset
				m_meetingKingPosition = m_king->getPersonPreviousPosition();
			}
		}

		std::cerr << (isSuccessfullyMeeting ? "\nMeeting successfully" : "Meeting is impossible") << "\n";
		std::cerr << "Count of steps:" << m_fairyland->getTurnCount() << "\n";

		auto kingMapView = m_king->getMapView(m_xSize, m_ySize);
		auto queenMapView = m_queen->getMapView(m_xSize, m_ySize);

		std::cerr << "\nking map:\n";
		std::cerr << kingMapView << "\n";

		std::cerr << "\nqueen map:\n";
		std::cerr << queenMapView << "\n";

		std::map<Position, enums::NodeType>* mergedNodesViewMap = mergeNodesMap(*m_king, *m_queen, m_meetingKingPosition, m_meetingQueenPosition);

		std::cerr << "\nmerged map:\n@ - king, & - queen\n";
		
		if (!isSuccessfullyMeeting)
		{
			std::cerr << "BECAUSE PERSONS NOT MEETING, MERGED MAP CAN BE INCORRECT\n";
		}

		std::cerr << getMergedMapView(*mergedNodesViewMap) << "\n";

		mergedNodesViewMap->clear();

		mergedNodesViewMap->clear();
		delete mergedNodesViewMap;
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

	models::PositionsDifference Maze::getStartPositionsDifferenceByMeetingPositionForPersons(Position firstPersonPosition, Position secondPersonPosition)
	{
		models::PositionsDifference differenceByCurrent;

		// calculating difference by current positions
		differenceByCurrent.byOx = firstPersonPosition.first - secondPersonPosition.first;
		differenceByCurrent.byOy = firstPersonPosition.second - secondPersonPosition.second;

		return differenceByCurrent;
	}

	std::map<Position, enums::NodeType>* Maze::mergeNodesMap(Person& firstPerson, Person& secondPerson,
		Position firstPersonMeetingPosition, Position secondPersonMeetingPosition)
	{
		// initialize new map with nodes map of first person as a content
		std::map<Position, enums::NodeType>* mergedNodesMap = new std::map<Position, enums::NodeType>();

		enums::NodeType nodeType;

		// get indents to discard excess coordinates (coordinates that are a maze borders)
		models::PersonIndents firstPersonIndents = firstPerson.getPersonIndents();

		// convert first person 
		for (auto& mapElement : *firstPerson.getWayTreeNodesMap())
		{
			if (mapElement.second->coordinates.first <= firstPersonIndents.maxIndentX && mapElement.second->coordinates.first >= firstPersonIndents.minIndentX
				&& mapElement.second->coordinates.second <= firstPersonIndents.maxIndentY && mapElement.second->coordinates.second >= firstPersonIndents.minIndentY)
			{
				// node is barrier
				if (mapElement.second->isBarrier)
				{
					nodeType = enums::NodeType::Barrier;
				}
				// node is person start position
				else if (mapElement.second->coordinates == firstPerson.getPersonStartPosition())
				{
					nodeType = enums::characterToNodeType(firstPerson.getCharacter());
				}
				// node is way
				else
				{
					nodeType = enums::NodeType::Way;
				}

				mergedNodesMap->insert(std::pair<Position, enums::NodeType>(mapElement.first, nodeType));
			}
		}

		// determine position difference between start positions of way nodes trees by common point to merge second person map with first person map
		models::PositionsDifference positionDifference = getStartPositionsDifferenceByMeetingPositionForPersons(firstPersonMeetingPosition, secondPersonMeetingPosition);

		// get indents to discard excess coordinates (coordinates that are a maze borders)
		models::PersonIndents secondPersonIndents = secondPerson.getPersonIndents();

		// sort out nodes map of second person and try to add nodes from it's to first person map 
		for (auto& mapElement : *secondPerson.getWayTreeNodesMap())
		{
			if (mapElement.second->coordinates.first <= secondPersonIndents.maxIndentX && mapElement.second->coordinates.first >= secondPersonIndents.minIndentX
				&& mapElement.second->coordinates.second <= secondPersonIndents.maxIndentY && mapElement.second->coordinates.second >= secondPersonIndents.minIndentY)
			{
				// node is barrier
				if (mapElement.second->isBarrier)
				{
					nodeType = enums::NodeType::Barrier;
				}
				// node is person start position
				else if (mapElement.second->coordinates == secondPerson.getPersonStartPosition())
				{
					nodeType = enums::characterToNodeType(secondPerson.getCharacter());
				}
				// node is way
				else
				{
					nodeType = enums::NodeType::Way;
				}

				auto result = mergedNodesMap->insert(std::pair<Position, enums::NodeType>(Position(mapElement.first.first + positionDifference.byOx, mapElement.first.second + positionDifference.byOy), nodeType));

				if ((nodeType == enums::NodeType::PersonKing || nodeType == enums::NodeType::PersonQueen) && !result.second)
				{
					mergedNodesMap->at(Position(mapElement.first.first + positionDifference.byOx, mapElement.first.second + positionDifference.byOy)) = nodeType;
				}
			}
		}

		return mergedNodesMap;
	}

	std::string Maze::getMergedMapView(std::map<Position, enums::NodeType>& mergedNodesViewMap)
	{
		models::PersonIndents mapIndents;

		// parsed map view
		std::string mapView;

		// find min max indents
		for (auto& element : mergedNodesViewMap)
		{
			if (mapIndents.maxIndentX < element.first.first)
			{
				mapIndents.maxIndentX = element.first.first;
			}
			if (mapIndents.maxIndentY < element.first.second)
			{
				mapIndents.maxIndentY = element.first.second;
			}
			if (mapIndents.minIndentX > element.first.first)
			{
				mapIndents.minIndentX = element.first.first;
			}
			if (mapIndents.minIndentY > element.first.second)
			{
				mapIndents.minIndentY = element.first.second;
			}
		}

		// parsing by oY from top to bottom
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

			// parsing by oX from left to right
			for (int x = 0; x < m_xSize + 2; x++)
			{
				if (x == 0 || x == m_xSize + 1 || y == 0 || y == m_ySize + 1)
				{
					mapView.append(" ");
				}
				else
				{
				 	auto node = mergedNodesViewMap.find(Position(x + mapIndents.minIndentX - 1, y + mapIndents.minIndentY - 1));
					
					if (node == mergedNodesViewMap.end())
					{
						mapView += "?";
					}
					else if (node->second == enums::NodeType::Barrier)
					{
						mapView += "#";
					}
					else if (node->second == enums::NodeType::Way)
					{
						mapView += ".";
					}
					else if (node->second == enums::NodeType::PersonKing)
					{
						mapView += "@";
					}
					else if (node->second == enums::NodeType::PersonQueen)
					{
						mapView += "&";
					}
				}
			}

			if (y != 0)
			{
				mapView += "\n";
			}
		}

		// add map legend
		mapView += "\n  0123456789";

		return mapView;
	}

	Maze::~Maze()
	{
		delete m_king;
		delete m_queen;
		delete m_fairyland;
	}
}