#include "AddResultCommand.h"

#include <functional>

#include "ICommandReceiver.h"

dpp::message AddResultCommand::ExecuteInternal() const
{
	m_CommandReceiver.AddResult(m_MatchId, m_Score);
	return { GetAnswerChannelId(), "Result added and bettors' scores updated." };
}

bool AddResultCommand::ValidateCommand(std::string& outUserErrMsg) const
{
	const std::optional<std::reference_wrapper<const Match>> matchOpt = m_CommandReceiver.GetMatch(m_MatchId);
	if (!matchOpt.has_value())
	{
		outUserErrMsg = "No match with the given ID " + std::to_string(m_MatchId);
		return false;
	}
	const Match& match = matchOpt.value().get();

	if (const unsigned int boSize = match.GetBoSize();
		boSize < m_Score.GetTotalNumberOfGames())
	{
		outUserErrMsg = "The match is a BO" + std::to_string(boSize) + ". You gave a score [" + std::to_string(m_Score.m_TeamAScore) + "-" + std::to_string(m_Score.m_TeamBScore) +
			"] with too many games.";
		return false;
	}

	if (const unsigned int numberOfGamesToWin = match.GetNumberOfGamesToWin();
		m_Score.m_TeamAScore != numberOfGamesToWin && m_Score.m_TeamBScore != numberOfGamesToWin)
	{
		outUserErrMsg = "The winning team must have " + std::to_string(numberOfGamesToWin) + "games. You gave a score [" + std::to_string(m_Score.m_TeamAScore) + "-" +
			std::to_string(m_Score.m_TeamBScore) + "] without a winning team.";
		return false;
	}

	return true;
}
