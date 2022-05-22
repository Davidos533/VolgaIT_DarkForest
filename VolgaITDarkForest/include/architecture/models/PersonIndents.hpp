#ifndef PERSON_INDENTS_HPP
#define PERSON_INDENTS_HPP

namespace architecture
{
	namespace models
	{
		/// <summary>
		/// transfer object for data about maximum and minimum person indents in coordinate plane
		/// </summary>
		struct PersonIndents
		{
		public:
			int maxIndentX = 0;
			int maxIndentY = 0;
			int minIndentX = 0;
			int minIndentY = 0;

			/// <summary>
			/// compare objects by every field
			/// </summary>
			/// <param name="comparedObject">right value object</param>
			/// <returns>comparing result</returns>
			bool operator == (const PersonIndents& comparedObject)
			{
				return maxIndentX == comparedObject.maxIndentX && maxIndentY == comparedObject.maxIndentY &&
					minIndentX == comparedObject.minIndentX && minIndentY == comparedObject.minIndentY;
			}
		};
	}
}

#endif