#include "CommandBase.h"

namespace
{
	std::pair<std::string, std::string> BuildOneScoreSelectorOption(const std::string& matchId, const unsigned int teamAScore,
		const unsigned int teamBScore)
	{
		const MatchScore score{ teamAScore, teamBScore };
		const std::string optionId = matchId + "-" + std::to_string(teamAScore) + "-" + std::to_string(teamBScore); // Don't want the spaces from ToString function
		return { score.ToString(), optionId };
	}
}

std::vector<std::pair<std::string, std::string>> CommandBase::Helper::BuildAllScoreScoreSelectorOptions(const Match& match)
{
	const std::string& matchId = match.GetId();
	const unsigned int winningTeamScore = match.GetNumberOfGamesToWin();

	std::vector<std::pair<std::string, std::string>> selectorOptions;
	for (unsigned int loosingTeamScore = 0; loosingTeamScore < winningTeamScore; ++loosingTeamScore)
	{
		selectorOptions.push_back(BuildOneScoreSelectorOption(matchId, winningTeamScore, loosingTeamScore));
		selectorOptions.push_back(BuildOneScoreSelectorOption(matchId, loosingTeamScore, winningTeamScore));
	}

	return selectorOptions;
}