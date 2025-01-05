#pragma once

#include <string>

#include "CommandBase.h"
#include "MatchScore.h"
#include "MessageBuilder.h"

class ICommandReceiver;

class AddResultCommand final : public CommandBase
{
public:
	AddResultCommand(const MessageBuilder::ChannelId channelId, BetBot& bot, std::string matchId, const unsigned int teamAScore, const unsigned int teamBScore) noexcept :
		CommandBase(channelId, bot), m_MatchId(std::move(matchId)), m_Score(MatchScore{teamAScore, teamBScore}) {}

	MessageBuilder::Message Execute() const override; // NOLINT(modernize-use-nodiscard)

private:
	std::string m_MatchId;
	MatchScore m_Score;
};