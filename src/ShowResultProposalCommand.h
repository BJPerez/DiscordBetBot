#pragma once

#include <string>
#include <dpp/message.h>
#include <dpp/snowflake.h>

#include "CommandBase.h"
#include "Match.h"

class ICommandReceiver;

class ShowResultProposalCommand final : public CommandBase
{
public:
	static constexpr std::string_view SELECT_MENU_ID = "Choose-Result";

	ShowResultProposalCommand(const dpp::snowflake channelId, BetBot& bot, std::string matchId) noexcept : CommandBase(channelId, bot), m_MatchId(std::move(matchId)) {}

	[[nodiscard]] dpp::message Execute() const override;

private:
	std::string m_MatchId{ Match::INVALID_ID };
};
