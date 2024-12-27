#pragma once

#include <string>

#include "CommandBase.h"
#include "Match.h"
#include "MatchScore.h"
#include "MessageBuilder.h"

class ICommandReceiver;

class AddBetCommand final : public CommandBase
{
public:
	AddBetCommand(const dpp::snowflake channelId, BetBot& bot, std::string matchId, const unsigned int teamAScore, const unsigned int teamBScore, std::string bettorName) noexcept:
		CommandBase(channelId, bot), m_MatchId(std::move(matchId)), m_Score{ teamAScore, teamBScore }, m_BettorName(std::move(bettorName)) {}

	[[nodiscard]] MessageBuilder::Message Execute() const override;

private:
	std::string m_MatchId{ Match::INVALID_ID };
	MatchScore m_Score;
	std::string m_BettorName;
};

