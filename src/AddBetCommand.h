#pragma once

#include <string>

#include "ICommand.h"
#include "Match.h"
#include "MatchScore.h"

class ICommandReceiver;

class AddBetCommand final : public ICommand
{
public:
	explicit AddBetCommand(const unsigned int matchId, const unsigned int teamAScore, const unsigned int teamBScore, std::string bettorName, ICommandReceiver& commandReceiver):
		m_MatchId(matchId), m_Score{ teamAScore, teamBScore }, m_BettorName(std::move(bettorName)), m_CommandReceiver(commandReceiver) {}

private:
	unsigned int m_MatchId{ Match::INVALID_ID };
	MatchScore m_Score;
	std::string m_BettorName;
	ICommandReceiver& m_CommandReceiver;

	void ExecuteInternal(std::string& outAnswerToUser) const final;
	[[nodiscard]] bool ValidateCommand(std::string& outUserErrMsg) const final;
};

