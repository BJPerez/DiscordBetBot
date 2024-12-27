#include "ShowIncomingMatchesCommand.h"

#include <algorithm>
#include <dpp/colors.h>

#include "Bet.h"
#include "MessageBuilder.h"
#include "ICommandReceiver.h"
#include "LockableDataAccessors.h"
#include "Match.h"

dpp::message ShowIncomingMatchesCommand::Execute() const
{
	const DataReader dataReader = GetDataReader();
	if (const std::vector<std::reference_wrapper<const Match>> matches = dataReader->GetIncomingMatches();
		matches.empty())
	{
		return MessageBuilder::BuildAnswer(GetAnswerChannelId(), "No match to display yet.");
	}
	else
	{
		dpp::message msg = MessageBuilder::BuildAnswer(GetAnswerChannelId(), "Here is the list of incoming matches:");

		for (const Match& match : matches)
		{
			std::vector<std::vector<std::string>> columnsContent(2);
			const std::vector<std::reference_wrapper<const Bet>> bets = dataReader->GetBetsOnMatch(match.GetId());
			for (const Bet& bet : bets)
			{
				columnsContent.at(0).emplace_back(bet.GetBettorName());
				columnsContent.at(1).emplace_back(bet.GetScore().ToString());
			}

			std::vector<MessageBuilder::Field> embedFields;
			embedFields.emplace_back( "Bets", MessageBuilder::BuildTable(columnsContent) );

			MessageBuilder::EmbedParams embedParams =
			{
				match.GetTeamAName() + " - " + match.GetTeamBName(),			// todo add un to string pour les matchs
				"ID: " + match.GetId() + ", BO" + std::to_string(match.GetBoSize()),
				embedFields
			};

			MessageBuilder::AddEmbedToMessage(embedParams, msg);
		}

		std::vector<MessageBuilder::SelectorOption> selectorOptions;
		selectorOptions.emplace_back("Place a bet on a match", std::string(ShowIncomingMatchesCommand::BET_OPTION_VALUE));
		selectorOptions.emplace_back("Add a match result", std::string(ShowIncomingMatchesCommand::RESULT_OPTION_VALUE));
		MessageBuilder::SelectorParams selectorParams =
		{
			std::string(ShowIncomingMatchesCommand::SELECT_MENU_ID),
			selectorOptions,
			"What do you want to do?"
		};
		MessageBuilder::AddSelectorToMessage(selectorParams, msg);

		return msg;
	}
}