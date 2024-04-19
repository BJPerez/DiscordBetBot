#include "ShowBetsCommand.h"

#include <algorithm>

#include "Bet.h"
#include "ICommandReceiver.h"
#include "Match.h"

void ShowBetsCommand::ExecuteInternal(std::string& outAnswerToUser) const
{
	outAnswerToUser.clear();

	const std::vector<Bet>& bets = m_CommandReceiver.GetBets();
	if (bets.empty())
	{
		outAnswerToUser += "Not bet to display yet.";
		return;
	}

	std::ranges::for_each(bets,
		[this, &outAnswerToUser](const Bet& bet)
		{
			const std::optional<std::reference_wrapper<const Match>> matchOpt = m_CommandReceiver.GetMatch(bet.GetMatchId());
			if (!matchOpt.has_value())
			{
				return;
			}
			const Match& match = matchOpt.value().get();

			const auto& [teamAScore, teamBScore] = bet.GetScore();
			outAnswerToUser += bet.GetBettorName() + ": " + match.GetTeamAName() + " " + std::to_string(teamAScore) + " - " + std::to_string(teamBScore) +
				" " + match.GetTeamBName() + "\n";
		}
	);
}

bool ShowBetsCommand::ValidateCommand(std::string&) const
{
	return true;
}
