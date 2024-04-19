#pragma once

#include <string>

#include "ICommand.h"
#include "Match.h"
#include "MatchScore.h"

class ICommandReceiver;

class AddResultCommand final : public ICommand
{
public:
	explicit AddResultCommand(const unsigned int matchId, const unsigned int teamAScore, const unsigned int teamBScore, ICommandReceiver& commandReceiver) :
		m_MatchId(matchId), m_Score(MatchScore{teamAScore, teamBScore}), m_CommandReceiver(commandReceiver) {}

private:
	unsigned int m_MatchId{ Match::INVALID_ID };
	MatchScore m_Score;
	ICommandReceiver& m_CommandReceiver;

	void ExecuteInternal(std::string& outAnswerToUser) const final;
	[[nodiscard]] bool ValidateCommand(std::string& outUserErrMsg) const final;
};

