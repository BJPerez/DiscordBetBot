#include "DrawUtils.h"

constexpr std::size_t SPACES_AROUND_WORD_IN_CELL = 2;

void DrawUtils::DrawSeparativeLine(const std::vector<std::size_t>& columnSizes, std::string& outLine)
{
	outLine += "+";
	for (const std::size_t columnSize : columnSizes)
	{
		const std::size_t lineSize = columnSize + (2 * SPACES_AROUND_WORD_IN_CELL);
		for (unsigned int index = 0; index < lineSize; ++index)
		{
			outLine += "-";
		}
		outLine += "+";
	}
	outLine += "\n";
}

void DrawUtils::DrawTable(const std::vector<std::vector<std::string>>& columnsContent, std::string& outTable)
{
	const std::vector<std::size_t> columnsSizes = GetColumnSizes(columnsContent);
	const std::size_t nbLines = columnsContent[0].size(); // All vector should have the same number of line
	const std::size_t nbColumns = columnsContent.size();

	outTable += "```\n";

	DrawUtils::DrawSeparativeLine(columnsSizes, outTable);
	for (unsigned int lineIndex = 0; lineIndex < nbLines; ++lineIndex)
	{
		for (unsigned int columnIndex = 0; columnIndex < nbColumns; ++columnIndex)
		{
			const std::string& currentWord = columnsContent[columnIndex][lineIndex];
			const std::size_t nbSpaces = (2 * SPACES_AROUND_WORD_IN_CELL) + columnsSizes[columnIndex] - currentWord.size();
			const std::size_t nbSpacesBefore = nbSpaces / 2;
			const std::size_t nbSpacesAfter = nbSpaces - nbSpacesBefore;

			outTable += "|";
			for (unsigned int spaceIndex = 0; spaceIndex < nbSpacesBefore; ++spaceIndex)
			{
				outTable += " ";
			}
			outTable += currentWord;
			for (unsigned int spaceIndex = 0; spaceIndex < nbSpacesAfter; ++spaceIndex)
			{
				outTable += " ";
			}
		}
		outTable += "|\n";
		DrawUtils::DrawSeparativeLine(columnsSizes, outTable);
	}
	outTable += "```\n";
}

std::vector<std::size_t> DrawUtils::GetColumnSizes(const std::vector<std::vector<std::string>>& columnsContent)
{
	std::vector<std::size_t> result;

	for (const std::vector<std::string>& column : columnsContent)
	{
		const std::string& longestWord = *std::ranges::max_element(column,
           [](const std::string& first, const std::string& second)
           {
               return first.size() < second.size();
           }
		);
		result.push_back(longestWord.size());
	}

	return result;
}