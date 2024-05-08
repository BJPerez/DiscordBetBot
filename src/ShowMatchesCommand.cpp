#include "ShowMatchesCommand.h"

#include <algorithm>

#include "ICommandReceiver.h"
#include "Match.h"

dpp::message ShowMatchesCommand::ExecuteInternal(const dpp::slashcommand_t& event) const
{
	const std::vector<Match>& matches = m_CommandReceiver.GetIncomingMatches();
	if (matches.empty())
	{
		return { event.command.channel_id, "No match to display yet." };
	}

	std::string msgText;
	std::ranges::for_each(matches,
		[&msgText](const Match& match)
		{
			msgText += "ID: " + std::to_string(match.GetId()) + " | Team A: " + match.GetTeamAName() + " | Team B: " + match.GetTeamBName() +
				" | BO" + std::to_string(match.GetBoSize()) + "\n";
		}
	);

	return { event.command.channel_id, msgText };
}

bool ShowMatchesCommand::ValidateCommand(std::string&) const
{
	return true;
}