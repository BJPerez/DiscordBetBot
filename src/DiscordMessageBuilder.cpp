#include "DiscordMessageBuilder.h"

#include "BotDataExceptions.h"
namespace
{
	constexpr std::size_t SPACES_AROUND_WORD_IN_CELL = 2;

	void AddSeparativeLine(const std::vector<std::size_t>& columnSizes, std::string& outLine)
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

	std::vector<std::size_t> GetColumnSizes(const std::vector<std::vector<std::string>>& columnsContent)
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

	void BuildTable(const std::vector<std::vector<std::string>>& columnsContent, std::string& outTable)
	{
		const std::vector<std::size_t> columnsSizes = GetColumnSizes(columnsContent);
		if (std::ranges::find(columnsSizes, 0) != columnsSizes.end())
		{
			return; // one of the column is empty, we can't draw the table
		}

		const std::size_t nbLines = columnsContent[0].size(); // All vector should have the same number of line
		const std::size_t nbColumns = columnsContent.size();

		outTable += "```\n";

		AddSeparativeLine(columnsSizes, outTable);
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
			AddSeparativeLine(columnsSizes, outTable);
		}
		outTable += "```\n";
	}

	dpp::component BuildSelector(const std::string& placeHolder, const std::string& selectorId, 
		const std::vector<std::pair<std::string, std::string>>& options)
	{
		dpp::component selectMenu;
		selectMenu.set_type(dpp::cot_selectmenu);
		selectMenu.set_placeholder(placeHolder);
		selectMenu.set_id(selectorId);

		for (auto& [optionText, optionId] : options)
		{
			selectMenu.add_select_option(dpp::select_option(optionText, optionId));
		}


		return selectMenu;
	}
}

dpp::message DiscordMessageBuilder::BuildBasicAnswer(const dpp::snowflake answerChannelId, const std::string& content)
{
	dpp::message msg{ answerChannelId, content };
	msg.set_flags(dpp::m_ephemeral);

	return msg;
}

dpp::message DiscordMessageBuilder::BuildAnswerWithSelector(const dpp::snowflake answerChannelId, const std::string& content,
	const DiscordMessageBuilder::SelectorParams& selectorParams)
{
	dpp::message msg = BuildBasicAnswer(answerChannelId, content);

	const dpp::component selector = BuildSelector(selectorParams.m_PlaceHolder, selectorParams.m_Id, selectorParams.m_Options);
	msg.add_component(selector);

	return msg;
}

dpp::message DiscordMessageBuilder::BuildAnswerWithTable(const dpp::snowflake answerChannelId, const DiscordMessageBuilder::TableParams& tableParams)
{
	std::string content;

	if (tableParams.m_TextBeforeTable.has_value())
	{
		content += tableParams.m_TextBeforeTable.value();
	}

	BuildTable(tableParams.m_TableColumnsContent, content);

	if (tableParams.m_TextAfterTable.has_value())
	{
		content += tableParams.m_TextAfterTable.value();
	}

	return BuildBasicAnswer(answerChannelId, content);
}

dpp::message DiscordMessageBuilder::BuildInvalidScoreAnswer(const dpp::snowflake answerChannelId, const InvalidScoreException& exception)
{
	const std::string errorMessage = "Error: Given score [" + exception.GetScore().ToString() + "] not not valid for a BO" + std::to_string(exception.GetBoSize()) + ".";
	return BuildBasicAnswer(answerChannelId, errorMessage);
}

dpp::message DiscordMessageBuilder::BuildInvalidBettorNameAnswer(const dpp::snowflake answerChannelId, const InvalidBettorNameException& exception)
{
	const std::string errorMessage = "Error: Given bettor's name [" + exception.GetBettorName() + "] is invalid.";
	return BuildBasicAnswer(answerChannelId, errorMessage);
}

dpp::message DiscordMessageBuilder::BuildBetNotFoundAnswer(const dpp::snowflake answerChannelId, const BetNotFoundException& exception)
{
	const std::string errorMessage = "Error: Coudn't find any bet for [" + exception.GetBettorName() + "] on the match with MatchID [" + exception.GetMatchId() + "].";
	return BuildBasicAnswer(answerChannelId, errorMessage);
}

dpp::message DiscordMessageBuilder::BuildInvalidBoSizeAnswer(const dpp::snowflake answerChannelId, const InvalidBoSizeException& exception)
{
	const std::string errorMessage = "Error: Given BO size [" + std::to_string(exception.GetBoSize()) + "]. is invalid. It must be an odd number.";
	return BuildBasicAnswer(answerChannelId, errorMessage);
}