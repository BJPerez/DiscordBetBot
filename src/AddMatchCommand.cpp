#include "AddMatchCommand.h"

#include "ICommandReceiver.h"

void AddMatchCommand::ExecuteInternal(std::string& outAnswerToUser) const
{
	m_CommandReceiver.AddMatch(m_TeamAName, m_TeamBName, m_BoSize);
	outAnswerToUser = "Match added.";
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