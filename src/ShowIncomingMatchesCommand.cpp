#include "ShowIncomingMatchesCommand.h"

#include "Bet.h"
#include "BotDataExceptions.h"
#include "ICommandReceiver.h"
#include "LockableDataAccessors.h"
#include "Match.h"

namespace
{
	constexpr size_t FIELD_TABLE_COLUMN_COUNT = 2;

	MessageBuilder::EmbedParams BuildEmbedParams(const DataReader<ICommandReceiver>& dataReader, const Match& match)
	{
		MessageBuilder::TableContent tableContent(FIELD_TABLE_COLUMN_COUNT);
		for (const std::vector<std::reference_wrapper<const Bet>> bets = dataReader->GetBetsOnMatch(match.GetId());
			const std::reference_wrapper<const Bet>& betRef : bets)
		{
			const Bet& bet = betRef.get();

			tableContent.at(0).emplace_back(bet.GetBettorName());
			tableContent.at(1).emplace_back(bet.GetScore().ToString());
		}

		std::vector<MessageBuilder::Field> embedFields;
		embedFields.emplace_back("Bets", MessageBuilder::BuildTable(tableContent));

		return
		{
			match.ToString(),			
			"ID: " + match.GetId() + ", BO" + std::to_string(match.GetBoSize()),
			embedFields
		};
	}

	MessageBuilder::SelectorParams BuildSelectorParams()
	{
		std::vector<MessageBuilder::SelectorOption> selectorOptions;
		selectorOptions.emplace_back("Place a bet on a match", std::string(ShowIncomingMatchesCommand::BET_OPTION_VALUE));
		selectorOptions.emplace_back("Add a match result", std::string(ShowIncomingMatchesCommand::RESULT_OPTION_VALUE));

		return 
		{
			std::string(ShowIncomingMatchesCommand::SELECT_MENU_ID),
			selectorOptions,
			"What do you want to do?"
		};
	}
}

MessageBuilder::Message ShowIncomingMatchesCommand::Execute() const
{
	try
	{
		const DataReader dataReader = GetDataReader();
		if (const std::vector<std::reference_wrapper<const Match>> matches = dataReader->GetIncomingMatches();
			matches.empty())
		{
			return MessageBuilder::BuildAnswer(GetAnswerChannelId(), "No match to display yet.");
		}
		else
		{
			MessageBuilder::Message msg = MessageBuilder::BuildAnswer(GetAnswerChannelId(), "Here is the list of incoming matches:");

			for (const std::reference_wrapper<const Match>& matchRef : matches)
			{
				const MessageBuilder::EmbedParams params = BuildEmbedParams(dataReader, matchRef.get());
				MessageBuilder::AddEmbedToMessage(params, msg);
			}

			const MessageBuilder::SelectorParams selectorParams = BuildSelectorParams();
			MessageBuilder::AddSelectorToMessage(selectorParams, msg);

			return msg;
		}
	}
	catch (const InvalidMatchIdException& exception)
	{
		return MessageBuilder::BuildAnswer(GetAnswerChannelId(),
			"Internal error: At least one saved match in data has an invalid ID: [" + exception.GetMatchId() + "].");
	}
}