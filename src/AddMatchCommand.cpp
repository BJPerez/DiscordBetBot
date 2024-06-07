#include "AddMatchCommand.h"

#include "ICommandReceiver.h"

dpp::message AddMatchCommand::ExecuteInternal() const
{
	m_CommandReceiver.AddMatch(m_TeamAName, m_TeamBName, m_BoSize);

	dpp::message msg{ GetAnswerChannelId(), "Match added." };
	msg.set_flags(dpp::m_ephemeral);
	return msg;
}

bool AddMatchCommand::ValidateCommand(std::string& outUserErrMsg) const
{
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