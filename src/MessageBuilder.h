#pragma once

#include <dpp/message.h>
#include <dpp/snowflake.h>

namespace MessageBuilder
{
	using Message = dpp::message;

	using SelectorOption = std::pair<std::string, std::string>; // OptionDisplayedText, OptionId
	struct SelectorParams
	{
		std::string m_Id;
		std::vector<SelectorOption> m_Options;
		std::string m_PlaceHolder;
	};

	using TableContent = std::vector<std::vector<std::string>>;

	using Field = std::pair<std::string, std::string>; // FieldTitle, FieldContent
	struct EmbedParams
	{
		std::string m_Title;
		std::string m_Description;
		std::vector<Field> m_Fields;
	};

	[[nodiscard]] dpp::message BuildAnswer(const dpp::snowflake answerChannelId, const std::string& content);

	void AddSelectorToMessage(const SelectorParams& selectorParams, dpp::message& msg);
	void AddEmbedToMessage(const EmbedParams& embedParams, dpp::message& msg);

	std::string BuildTable(const TableContent& columnsContent);
};