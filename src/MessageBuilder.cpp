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
			const std::size_t cellSize = columnSize + (2 * SPACES_AROUND_WORD_IN_CELL);
			for (unsigned int index = 0; index < cellSize; ++index)
			{
				separativeLine += TABLE_LINE_SEPARATOR;
			}
			separativeLine += TABLE_CELL_ANGLE;
		}
		separativeLine += "\n";

		return separativeLine;
	}

	std::vector<std::size_t> GetColumnSizes(const MessageBuilder::TableContent& tableContent)
	{
		std::vector<std::size_t> result;

		for (const std::vector<std::string>& column : tableContent)
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

	std::string BuildTableCell(const std::string& word, const size_t columnSize)
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

MessageBuilder::Message MessageBuilder::BuildAnswer(const ChannelId answerChannelId, const std::string& content)
{
	Message msg{ answerChannelId, content };
	msg.set_flags(dpp::m_ephemeral);

	return msg;
}

void MessageBuilder::AddSelectorToMessage(const SelectorParams& selectorParams, Message& outMessage)
{
	dpp::component selector;
	selector.set_type(dpp::cot_selectmenu);
	selector.set_placeholder(selectorParams.m_PlaceHolder);
	selector.set_id(selectorParams.m_Id);

	for (auto& [optionText, optionId] : selectorParams.m_Options)
	{
		selector.add_select_option(dpp::select_option(optionText, optionId));
	}

	outMessage.add_component(dpp::component().add_component(selector));
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

void MessageBuilder::AddEmbedToMessage(const EmbedParams& embedParams, Message& outMessage)
{
	dpp::embed embed;

	embed.set_title(embedParams.m_Title);
	embed.set_description(embedParams.m_Description);
	for (auto& [fieldTitle, fieldContent] : embedParams.m_Fields)
	{
		embed.add_field(fieldTitle, fieldContent);
	}

	outMessage.add_embed(embed);
}

void MessageBuilder::AddButtonToMessage(const ButtonParams& buttonParams, Message& outMessage)
{
	dpp::component button;
	button.set_label(buttonParams.m_Label);
	button.set_id(buttonParams.m_Id);

	outMessage.add_component(dpp::component().add_component(button));
}