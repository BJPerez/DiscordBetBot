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
	explicit AddResultCommand(const dpp::snowflake channelId, const unsigned int matchId, const unsigned int teamAScore, const unsigned int teamBScore, ICommandReceiver& commandReceiver) :
		CommandBase(channelId), m_MatchId(matchId), m_Score(MatchScore{teamAScore, teamBScore}), m_CommandReceiver(commandReceiver) {}

private:
	unsigned int m_MatchId{ Match::INVALID_ID };
	MatchScore m_Score;
	ICommandReceiver& m_CommandReceiver;

	[[nodiscard]] dpp::message ExecuteInternal() const final;
	[[nodiscard]] bool ValidateCommand(std::string& outUserErrMsg) const final;
};

