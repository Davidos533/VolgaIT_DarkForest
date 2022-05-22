#ifndef NODE_TYPE_HPP
#define NODE_TYPE_HPP

#include "common/api/fairy_tail.hpp"

namespace architecture
{
	namespace enums
	{
		/// <summary>
		/// node type - element for view map of maze
		/// </summary>
		enum class NodeType
		{
			Way,
			Barrier,
			PersonQueen,
			PersonKing
		};

		/// <summary>
		/// function to cast Character from fairy_tail to node type
		/// </summary>
		/// <param name="character">character for cast</param>
		/// <returns>cast result of character</returns>
		inline NodeType characterToNodeType(Character character)
		{
			return character == Character::Ivan ? NodeType::PersonKing : NodeType::PersonQueen;
		}
	}
}

#endif