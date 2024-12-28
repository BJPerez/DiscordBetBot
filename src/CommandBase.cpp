#include "CommandBase.h"

namespace
{
	MessageBuilder::SelectorOption BuildOneScoreSelectorOption(const std::string& matchId, const unsigned int teamAScore,
		const unsigned int teamBScore)
	{
		const MatchScore score{ teamAScore, teamBScore };
		const std::string optionId = matchId + "-" + std::to_string(teamAScore) + "-" + std::to_string(teamBScore); // Don't want the spaces from ToString function //todo
		return { score.ToString(), optionId };
	}
}

std::vector<MessageBuilder::SelectorOption> CommandBase::Helper::BuildScoreSelectorOptions(const Match& match)
{
	const std::string& matchId = match.GetId();
	const unsigned int winningTeamScore = match.GetNumberOfGamesToWin();

	std::vector<MessageBuilder::SelectorOption> selectorOptions;
	for (unsigned int loosingTeamScore = 0; loosingTeamScore < winningTeamScore; ++loosingTeamScore)
	{
		selectorOptions.push_back(BuildOneScoreSelectorOption(matchId, winningTeamScore, loosingTeamScore));
		selectorOptions.push_back(BuildOneScoreSelectorOption(matchId, loosingTeamScore, winningTeamScore));
	}

	return selectorOptions;
}

std::vector<MessageBuilder::SelectorOption> CommandBase::Helper::BuildMatchSelectorOptions(const std::vector<std::reference_wrapper<const Match>>& matches)
{
	std::vector<MessageBuilder::SelectorOption> options;

	for (const std::reference_wrapper<const Match>& matchRef : matches)
	{
		const Match& match = matchRef.get();
		options.emplace_back(match.ToString(), match.GetId());  //todo to string pour les matches
	}

	return options;
}