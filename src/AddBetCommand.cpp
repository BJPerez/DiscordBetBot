#include "AddBetCommand.h"

#include <functional>

#include "Bet.h"
#include "ICommandReceiver.h"

dpp::message AddBetCommand::ExecuteInternal() const 
{
	if (const std::optional<std::reference_wrapper<const Bet>> bet = m_CommandReceiver.GetBet(m_MatchId, m_BettorName))
	{
		m_CommandReceiver.ModifyBet(m_MatchId, m_Score, m_BettorName);
		return {GetAnswerChannelId(), "You already had a different bet for this match. The score has been modified."};
	}
	m_CommandReceiver.AddBet(m_MatchId, m_Score, m_BettorName);
	return { GetAnswerChannelId(), "Bet added." };
}

bool AddBetCommand::ValidateCommand(std::string& outUserErrMsg) const
{
	if (m_BettorName.empty())
	{
		outUserErrMsg = "Internal error.";
		return false;
	}

	const std::optional<std::reference_wrapper<const Match>> matchOptional = m_CommandReceiver.GetMatch(m_MatchId);
	if (!matchOptional.has_value())
	{
		outUserErrMsg = "No match with the given ID " + std::to_string(m_MatchId);
		return false;
	}
	const Match& match = matchOptional.value().get();

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
		outUserErrMsg = "The winning team must have " + std::to_string(numberOfGamesToWin) + " games. You gave a score [" + std::to_string(m_Score.m_TeamAScore) + "-" + 
			std::to_string(m_Score.m_TeamBScore) + "] without a winning team.";
		return false;
	}

	if (const std::optional<std::reference_wrapper<const Bet>> bet = m_CommandReceiver.GetBet(m_MatchId, m_BettorName))
	{
		if (bet.value().get().GetScore() == m_Score)
		{
			outUserErrMsg = "You already have a bet for this match with the exact same score.";
			return false;
		}
	}

	return true;
}
