#pragma once

#include <string>
#include <string_view>
#include <dpp/message.h>
#include <dpp/snowflake.h>

#include "CommandBase.h"
#include "Match.h"

class ICommandReceiver;

class ShowBetProposalCommand final : public CommandBase
{
public:
	static constexpr std::string_view SELECT_MENU_ID = "Choose-Bet";

	ShowBetProposalCommand(const dpp::snowflake channelId, BetBot& bot, const unsigned int matchId) noexcept : CommandBase(channelId, bot), m_MatchId(matchId) {}

	[[nodiscard]] dpp::message Execute() const override;

private:
	unsigned int m_MatchId{ Match::INVALID_ID };

	bool ValidateCommand(const DataReader<ICommandReceiver>& dataReader, std::string& outUserErrMsg) const;
};
