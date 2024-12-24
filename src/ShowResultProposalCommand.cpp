#include "ShowResultProposalCommand.h"

#include "BotDataExceptions.h"
#include "DiscordMessageBuilder.h"
#include "DrawUtils.h"
#include "ICommandReceiver.h"
#include "LockableDataAccessors.h"
#include "Match.h"

namespace
{
	std::pair<std::string, std::string> BuildSelectorOption(const std::string& matchId, const unsigned int teamAScore, const unsigned int teamBScore)
	{
		const MatchScore score{ teamAScore, teamBScore };
		const std::string optionId = matchId + "-" + std::to_string(teamAScore) + "-" + std::to_string(teamBScore); // Don't want the spaces from ToString function
		return { score.ToString(), optionId };
	}

	std::string BuildMessageContent(const Match& match)
	{
		return "Choose your bet for " + match.GetTeamAName() + " - " + match.GetTeamBName() + ":";
	}
}

dpp::message ShowResultProposalCommand::Execute() const
{
	try
	{
		const DataReader dataReader = GetDataReader();
		const Match& match = dataReader->GetMatch(m_MatchId);
		const std::string& matchId = match.GetId();
		const unsigned int winningTeamScore = match.GetNumberOfGamesToWin(); // todo mettre en commun avec ShowBetProposalCommand

		std::vector<std::pair<std::string, std::string>> selectorOptions;
		for (unsigned int loosingTeamScore = 0; loosingTeamScore < winningTeamScore; ++loosingTeamScore)
		{
			selectorOptions.push_back(BuildSelectorOption(matchId, winningTeamScore, loosingTeamScore));
			selectorOptions.push_back(BuildSelectorOption(matchId, loosingTeamScore, winningTeamScore));
		}

		const DiscordMessageBuilder::SelectorParams params
		{
			std::string {SELECT_MENU_ID},
			selectorOptions,
			match.GetTeamAName() + " - " + match.GetTeamBName()
		};

		return DiscordMessageBuilder::BuildAnswerWithSelector(GetAnswerChannelId(), BuildMessageContent(match), params);
	}
	catch (const InvalidMatchIdException& exception)
	{
		return DiscordMessageBuilder::BuildBasicAnswer(GetAnswerChannelId(),
			"User error: Given ID [" + exception.GetMatchId() + "] is invalid.");
	}
	catch (const MatchNotFoundException& exception)
	{
		return DiscordMessageBuilder::BuildBasicAnswer(GetAnswerChannelId(),
			"User error: Could not find any match with the given ID [" + exception.GetMatchId() + "].");
	}
}