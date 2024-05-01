#include "ShowMatchesCommand.h"

#include <algorithm>

#include "ICommandReceiver.h"
#include "Match.h"

void ShowMatchesCommand::ExecuteInternal(std::string& outAnswerToUser) const
{
	outAnswerToUser.clear();

	const std::vector<Match>& matches = m_CommandReceiver.GetIncomingMatches();
	if (matches.empty())
	{
		outAnswerToUser += "No match to display yet.";
		return;
	}

	std::ranges::for_each(matches,
		[&outAnswerToUser](const Match& match)
		{
			outAnswerToUser += "ID: " + std::to_string(match.GetId()) + " | Team A: " + match.GetTeamAName() + " | Team B: " + match.GetTeamBName() + 
				" | BO" + std::to_string(match.GetBoSize()) + "\n";
		}
	);
}

bool ShowMatchesCommand::ValidateCommand(std::string&) const
{
	return true;
}