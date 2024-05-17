#include "DrawUtils.h"

#include <algorithm>

#include "Match.h"
#include "MatchScore.h"

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
	if (std::ranges::find(columnsSizes, 0) != columnsSizes.end())
	{
		return; // one of the column is empty, we can't draw the table
	}

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
		if (column.empty())
		{
			result.emplace_back(0);
			continue;
		}

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

dpp::component DrawUtils::CreateMatchSelector(const std::string& placeHolder, const std::string& selectorId, const std::vector<Match>& matches)
{
	dpp::component selectMenu;
	selectMenu.set_type(dpp::cot_selectmenu);
	selectMenu.set_placeholder(placeHolder);
	std::ranges::for_each(matches,
		[&selectMenu](const Match& match)
		{
			selectMenu.add_select_option(dpp::select_option(match.GetTeamAName() + " - " + match.GetTeamBName(), std::to_string(match.GetId())));
		}
	);
	selectMenu.set_id(selectorId);

	return selectMenu;
}

namespace
{
	dpp::select_option CreateSelectMenuOption(const unsigned int matchId, const MatchScore& score)
	{
		const std::string displayedOption = score.ToString();
		const std::string optionValue = std::to_string(matchId) + "-" + std::to_string(score.m_TeamAScore) + "-" + std::to_string(score.m_TeamBScore); // score.ToString has space around the '-' that we don't want in the value
		return { displayedOption, optionValue };
	}
}

dpp::component DrawUtils::CreateMatchResultSelector(const std::string& placeHolder, const std::string& selectorId, const Match& match)
{
	dpp::component selectMenu;
	selectMenu.set_type(dpp::cot_selectmenu);
	selectMenu.set_placeholder(placeHolder);

	const unsigned int winningTeamScore = match.GetNumberOfGamesToWin();
	for (unsigned int loosingTeamScore = 0; loosingTeamScore < winningTeamScore; ++loosingTeamScore)
	{
		selectMenu.add_select_option(CreateSelectMenuOption(match.GetId(), {winningTeamScore, loosingTeamScore}));
		selectMenu.add_select_option(CreateSelectMenuOption(match.GetId(), { loosingTeamScore, winningTeamScore }));
	}

	selectMenu.set_id(selectorId);

	return selectMenu;
}