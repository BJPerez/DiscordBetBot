#include "AddMatchCommand.h"

#include "ICommandReceiver.h"
#include "LockableDataAccessors.h"

dpp::message AddMatchCommand::Execute() const
{
	dpp::message msg{ GetAnswerChannelId(), "" };
	msg.set_flags(dpp::m_ephemeral);

	{
		const DataWriter dataWriter = GetDataWriter();
		if (std::string errorMsg;
			!ValidateCommand(errorMsg))
		{
			msg.set_content("Error: " + errorMsg);
		}
		else
		{
			dataWriter->AddMatch(m_MatchId, m_TeamAName, m_TeamBName, m_BoSize);
			msg.set_content("Match added.");
		}
	}

	return msg;
}

bool AddMatchCommand::ValidateCommand(std::string& outUserErrMsg) const
{
	if (m_MatchId.has_value() && m_MatchId.value().empty())
	{
		outUserErrMsg = "Given match id is empty.";
		return false;
	}

	if (m_TeamAName.empty() || m_TeamBName.empty())
	{
		outUserErrMsg = "Some of the teams has no name.";
		return false;
	}

	if (m_BoSize %2 != 1)
	{
		outUserErrMsg = "The match must have an odd number of games.";
		return false;
	}

	return true;
}