#pragma once

#include <string>
#include <dpp/message.h>
#include <dpp/snowflake.h>

#include "CommandBase.h"
#include "Match.h"
#include "MatchScore.h"

class ICommandReceiver;

class AddResultCommand final : public CommandBase
{
public:
	AddResultCommand(const dpp::snowflake channelId, BetBot& bot, const unsigned int matchId, const unsigned int teamAScore, const unsigned int teamBScore) noexcept :
		CommandBase(channelId, bot), m_MatchId(matchId), m_Score(MatchScore{teamAScore, teamBScore}) {}

	[[nodiscard]] dpp::message Execute() const override;

private:
	unsigned int m_MatchId{ Match::INVALID_ID };
	MatchScore m_Score;

	bool ValidateCommand(const DataWriter<ICommandReceiver>& dataWriter, std::string& outUserErrMsg) const;
};

