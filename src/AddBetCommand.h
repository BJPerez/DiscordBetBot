#pragma once

#include <string>

#include "CommandBase.h"
#include "MatchScore.h"
#include "MessageBuilder.h"

class ICommandReceiver;

class AddBetCommand final : public CommandBase
{
public:
	AddBetCommand(const MessageBuilder::ChannelId channelId, BetBot& bot, std::string matchId, const unsigned int teamAScore, const unsigned int teamBScore, std::string bettorName) noexcept:
		CommandBase(channelId, bot), m_MatchId(std::move(matchId)), m_Score{ teamAScore, teamBScore }, m_BettorName(std::move(bettorName)) {}

	[[nodiscard]] MessageBuilder::Message Execute() const override;

private:
	std::string m_MatchId;
	MatchScore m_Score;
	std::string m_BettorName;
};

