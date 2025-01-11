#pragma once

#include <dpp/message.h>
#include <dpp/snowflake.h>

namespace MessageBuilder
{
	using Message = dpp::message;
	using ChannelId = dpp::snowflake;
		
	using SelectorOption = std::pair<std::string, std::string>; // OptionDisplayedText, OptionId
	struct SelectorParams
	{
		std::string m_Id;
		std::vector<SelectorOption> m_Options;
		std::string m_PlaceHolder;
	};

	using TableContent = std::vector<std::vector<std::string>>; // column, line

	using Field = std::pair<std::string, std::string>; // FieldTitle, FieldContent
	struct EmbedParams
	{
		std::string m_Title;
		std::string m_Description;
		std::vector<Field> m_Fields;
	};

	struct ButtonParams
	{
		std::string m_Label;
		std::string m_Id;
	};

	[[nodiscard]] Message BuildAnswer(const ChannelId answerChannelId, const std::string& content);

	void AddSelectorToMessage(const SelectorParams& selectorParams, Message& outMessage);
	void AddEmbedToMessage(const EmbedParams& embedParams, Message& outMessage);
	void AddButtonToMessage(const ButtonParams& buttonParams, Message& outMessage); 

	std::string BuildTable(const TableContent& columnsContent);
};