#include "AddResultCommand.h"

#include "ICommandReceiver.h"
#include "LockableDataAccessors.h"

dpp::message AddResultCommand::Execute() const
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
			dataWriter->AddResult(m_MatchId, m_Score);
			msg.set_content("Result added and bettors' scores updated.");
		}
	}
	return msg;
}

bool AddResultCommand::ValidateCommand(const DataWriter<ICommandReceiver>& dataWriter, std::string& outUserErrMsg) const
{
	const std::optional<std::reference_wrapper<const Match>> matchOpt = dataWriter->GetMatch(m_MatchId);
	if (!matchOpt.has_value())
	{
		outUserErrMsg = "No match with the given ID " + m_MatchId;
		return false;
	}

	const Match& match = matchOpt.value().get();
	if (match.IsPlayed())
	{
		outUserErrMsg = "The match result has already been set.";
		return false;
	}

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
