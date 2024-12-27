#include "MessageBuilder.h"

#include "BotDataExceptions.h"

#include <dpp/colors.h>

namespace
{
	constexpr std::size_t SPACES_AROUND_WORD_IN_CELL = 2;
	constexpr char TABLE_LINE_SEPARATOR = '-';
	constexpr char TABLE_COLUMN_SEPARATOR = '|';
	constexpr char TABLE_CELL_ANGLE = '+';

	std::string BuildSeparativeLine(const std::vector<std::size_t>& columnSizes)
	{
		std::string separativeLine;
		separativeLine += TABLE_CELL_ANGLE;
		for (const std::size_t columnSize : columnSizes)
		{
			const std::size_t lineSize = columnSize + (2 * SPACES_AROUND_WORD_IN_CELL);
			for (unsigned int index = 0; index < lineSize; ++index)
			{
				separativeLine += TABLE_LINE_SEPARATOR;
			}
			separativeLine += TABLE_CELL_ANGLE;
		}
		separativeLine += "\n";

		return separativeLine;
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

	std::string BuildTableCell(const std::string& word, const unsigned int columnSize)
	{
		std::string cell;

		const std::size_t nbSpaces = (2 * SPACES_AROUND_WORD_IN_CELL) + columnSize - word.size();
		const std::size_t nbSpacesBefore = nbSpaces / 2;
		const std::size_t nbSpacesAfter = nbSpaces - nbSpacesBefore;

		cell += TABLE_COLUMN_SEPARATOR;
		for (unsigned int spaceIndex = 0; spaceIndex < nbSpacesBefore; ++spaceIndex)
		{
			cell += " ";
		}
		cell += word;
		for (unsigned int spaceIndex = 0; spaceIndex < nbSpacesAfter; ++spaceIndex)
		{
			cell += " ";
		}

		return cell;
	}
}

dpp::message MessageBuilder::BuildAnswer(const dpp::snowflake answerChannelId, const std::string& content)
{
	dpp::message msg{ answerChannelId, content };
	msg.set_flags(dpp::m_ephemeral);

	return msg;
}

void MessageBuilder::AddSelectorToMessage(const SelectorParams& selectorParams, dpp::message& msg)
{
	dpp::component selector;

	selector.set_type(dpp::cot_selectmenu);
	selector.set_placeholder(selectorParams.m_PlaceHolder);
	selector.set_id(selectorParams.m_Id);

	for (auto& [optionText, optionId] : selectorParams.m_Options)
	{
		selector.add_select_option(dpp::select_option(optionText, optionId));
	}

	msg.add_component(selector);
}

std::string MessageBuilder::BuildTable(const TableContent& columnsContent)
{
	const std::vector<std::size_t> columnsSizes = GetColumnSizes(columnsContent);
	if (std::ranges::find(columnsSizes, 0) != columnsSizes.end())
	{
		return ""; // one of the column is empty, we can't draw the table
	}

	const std::size_t nbLines = columnsContent[0].size(); // All vector should have the same number of line
	const std::size_t nbColumns = columnsContent.size();

	std::string table = "```\n";
	table += BuildSeparativeLine(columnsSizes);
	for (unsigned int lineIndex = 0; lineIndex < nbLines; ++lineIndex)
	{
		for (unsigned int columnIndex = 0; columnIndex < nbColumns; ++columnIndex)
		{
			table += BuildTableCell(columnsContent[columnIndex][lineIndex], columnsSizes[columnIndex]);
		}
		table += TABLE_COLUMN_SEPARATOR;
		table += "\n";
		table += BuildSeparativeLine(columnsSizes);
	}
	table += "```\n";

	return table;
}

void MessageBuilder::AddEmbedToMessage(const EmbedParams& embedParams, dpp::message& msg)
{
	dpp::embed embed;

	embed.set_title(embedParams.m_Title);
	embed.set_description(embedParams.m_Description);
	for (auto& [fieldTitle, fieldContent] : embedParams.m_Fields)
	{
		embed.add_field(fieldTitle, fieldContent);
	}

	msg.add_embed(embed);
}