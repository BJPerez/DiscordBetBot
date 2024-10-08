#include "AddBetCommand.h"

#include "Bet.h"
#include "ICommandReceiver.h"
#include "LockableDataAccessors.h"

dpp::message AddBetCommand::Execute() const 
{
	dpp::message msg{ GetAnswerChannelId(), "" };
	msg.set_flags(dpp::m_ephemeral);

	{
		const DataWriter dataWriter = GetDataWriter();
		if (std::string errorMsg;
			!ValidateCommand(dataWriter, errorMsg))
		{
			msg.set_content("Error: " + errorMsg);
		}
		else
		{
			if (const std::optional<std::reference_wrapper<const Bet>> bet = dataWriter->GetBet(m_MatchId, m_BettorName))
			{
				if (m_Score == bet->get().GetScore())
				{
					msg.set_content("You already have the exact same bet for this match.");
				}
				else
				{
					dataWriter->ModifyBet(m_MatchId, m_Score, m_BettorName);
					msg.set_content("You already had a different bet for this match. Your bet has been modified.");
				}
			}
			else
			{
				dataWriter->AddBet(m_MatchId, m_Score, m_BettorName);
				msg.set_content("Bet added.");
			}
		}
	}

	return msg; 
}

bool AddBetCommand::ValidateCommand(const DataWriter<ICommandReceiver>& dataWriter, std::string& outUserErrMsg) const
{
	if (m_BettorName.empty())
	{
		outUserErrMsg = "Internal error.";
		return false;
	}

	const std::optional<std::reference_wrapper<const Match>> matchOptional = dataWriter->GetMatch(m_MatchId);
	if (!matchOptional.has_value())
	{
		outUserErrMsg = "No match with the given ID " + m_MatchId;
		return false;
	}

	const Match& match = matchOptional.value().get();
	if (match.IsPlayed())
	{
		outUserErrMsg = "The match has already been played. You can't bet on it.";
		return false;
	}

	if (!match.IsValidScore(m_Score))
	{
		outUserErrMsg = "The match is a BO" + std::to_string(match.GetBoSize()) + ". You gave a non valid score [" + std::to_string(m_Score.m_TeamAScore) + "-" + std::to_string(m_Score.m_TeamBScore) +
			"].";
		return false;
	}

	if (const std::optional<std::reference_wrapper<const Bet>> bet = dataWriter->GetBet(m_MatchId, m_BettorName))
	{
		if (bet.value().get().GetScore() == m_Score)
		{
			outUserErrMsg = "You already have a bet for this match with the exact same score.";
			return false;
		}
	}

	return true;
}
