#pragma once

#include <dpp/message.h>
#include <dpp/snowflake.h>

class MatchNotPlayedException;
class InvalidBoSizeException;
class InvalidTeamNameException;
class BetNotFoundException;
class BetAlreadyExistException;
class InvalidBettorNameException;
class InvalidScoreException;
class MatchAlreadyPlayedException;
class MatchNotFoundException;
class MatchIdUnavailableException;
class InvalidMatchIdException;

namespace DiscordMessageBuilder
{
	struct SelectorParams
	{
		std::string m_Id;
		std::vector<std::pair<std::string, std::string>> m_Options;
		std::string m_PlaceHolder;
	};

	struct TableParams
	{
		std::optional<std::string> m_TextBeforeTable;
		const std::vector<std::vector<std::string>>& m_TableColumnsContent;
		std::optional<std::string> m_TextAfterTable;
	};

	struct EmbedParams
	{
		
	};

	[[nodiscard]]  dpp::message BuildBasicAnswer(const dpp::snowflake answerChannelId, const std::string& content);
	[[nodiscard]]  dpp::message BuildAnswerWithSelector(const dpp::snowflake answerChannelId, const std::string& content,
		const SelectorParams& selectorParams);
	[[nodiscard]] dpp::message BuildAnswerWithTable(const dpp::snowflake answerChannelId, const TableParams& tableParams);

	[[nodiscard]] dpp::message BuildInvalidScoreAnswer(const dpp::snowflake answerChannelId, const InvalidScoreException& exception);
	[[nodiscard]] dpp::message BuildInvalidBettorNameAnswer(const dpp::snowflake answerChannelId, const InvalidBettorNameException& exception);
	[[nodiscard]] dpp::message BuildBetNotFoundAnswer(const dpp::snowflake answerChannelId, const BetNotFoundException& exception);
	[[nodiscard]] dpp::message BuildInvalidBoSizeAnswer(const dpp::snowflake answerChannelId, const InvalidBoSizeException& exception);
};