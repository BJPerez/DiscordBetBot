#include "ShowIncomingMatchesCommand.h"

#include "Bet.h"
#include "BotDataExceptions.h"
#include "ICommandReceiver.h"
#include "LockableDataAccessors.h"
#include "Match.h"

namespace
{
	constexpr size_t FIELD_TABLE_COLUMN_COUNT = 2;
	constexpr size_t MAX_MATCH_TO_DISPLAY_COUNT = 10;

	MessageBuilder::EmbedParams BuildEmbedParams(const DataReader<ICommandReceiver>& dataReader, const Match& match)
	{
		std::vector<MessageBuilder::Field> embedFields;
		if (const std::vector<std::reference_wrapper<const Bet>> bets = dataReader->GetBetsOnMatch(match.GetId()); 
			bets.empty())
		{
			embedFields.emplace_back("Bets", "No bet yet.");
		}
		else
		{
			MessageBuilder::TableContent tableContent(FIELD_TABLE_COLUMN_COUNT);
			for (const std::reference_wrapper<const Bet>& betRef : bets)
			{
				const Bet& bet = betRef.get();

				tableContent.at(0).emplace_back(bet.GetBettorName());
				tableContent.at(1).emplace_back(bet.GetScore().ToString());
			}

			embedFields.emplace_back("Bets", MessageBuilder::BuildTable(tableContent));
		}

		return
		{
			match.ToString(),			
			"ID: " + match.GetId() + ",\n BO" + std::to_string(match.GetBoSize()),
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

	void AddSelectorToMessage(MessageBuilder::Message& outMsg)
	{
		const MessageBuilder::SelectorParams selectorParams = BuildSelectorParams();
		MessageBuilder::AddSelectorToMessage(selectorParams, outMsg);
	}

	MessageBuilder::ButtonParams BuildButtonParams(const size_t lastIndexUsed)
	{
		std::string id = std::string{ ShowIncomingMatchesCommand::BUTTON_ID } + std::to_string(lastIndexUsed);

		return
		{
			"Show next matches",
			std::move(id)
		};
	}

	void AddButtonToMessage(const std::vector<std::reference_wrapper<const Match>>& matches, const size_t lastIndexUsed, 
		MessageBuilder::Message& outMsg)
	{
		if (matches.size() > lastIndexUsed + 1)
		{
			const MessageBuilder::ButtonParams params = BuildButtonParams(lastIndexUsed);
			MessageBuilder::AddButtonToMessage(params, outMsg);
		}
	}

	std::string BuildMessageText(const std::vector<std::reference_wrapper<const Match>>& matches, const size_t lastIndexUsed)
	{
		if (lastIndexUsed == 0)
		{
			if (matches.size() <= MAX_MATCH_TO_DISPLAY_COUNT)
			{
				return "Here is the list of incoming matches:";
			}
			return "Here are the first " + std::to_string(MAX_MATCH_TO_DISPLAY_COUNT) + " matches. Click on the button to show more.";
		}

		if (const size_t remainingMatchCount = matches.size() - (lastIndexUsed + 1); 
			remainingMatchCount <= MAX_MATCH_TO_DISPLAY_COUNT)
		{
			if (remainingMatchCount == 1)
			{
				return "Here is the last match.";
			}
			return "Here are the last " + std::to_string(remainingMatchCount) + " matches.";
		}
		return "Here are the next " + std::to_string(MAX_MATCH_TO_DISPLAY_COUNT) + " matches. Click on the button to show more.";
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
			MessageBuilder::Message msg = MessageBuilder::BuildAnswer(GetAnswerChannelId(), BuildMessageText(matches, m_LastIndexUsed));
			const size_t lastIndexUsed = AddMatchesToMsg(dataReader, matches, msg);
			AddSelectorToMessage(msg);
			AddButtonToMessage(matches, lastIndexUsed, msg);

			return msg;
		}
	}
	catch (const InvalidMatchIdException& exception)
	{
		return MessageBuilder::BuildAnswer(GetAnswerChannelId(),
			"Internal error: At least one saved match in data has an invalid ID: [" + exception.GetMatchId() + "].");
	}
	catch (const IndexInvalidException& exception)
	{
		return MessageBuilder::BuildAnswer(GetAnswerChannelId(),
			"Internal error: Tried to display matches starting at index [" + std::to_string(exception.GetIndex()) + 
			" outside of match list range [" + std::to_string(exception.GetArraySize()) + "].");
	}
}

size_t ShowIncomingMatchesCommand::AddMatchesToMsg(const DataReader<ICommandReceiver>& dataReader, 
	const std::vector<std::reference_wrapper<const Match>>& matches, MessageBuilder::Message& outMsg) const
{
	const size_t matchCount = matches.size();

	const size_t startIndex = m_LastIndexUsed == 0 ? 0 : m_LastIndexUsed + 1;
	if (startIndex >= matchCount)
	{
		throw IndexInvalidException(startIndex, matchCount);
	}

	const size_t matchToDisplayCount = std::min(matchCount - startIndex, MAX_MATCH_TO_DISPLAY_COUNT);
	const size_t endIndex = startIndex + matchToDisplayCount - 1;

	for (size_t index = startIndex; index <= endIndex; ++index)
	{
		const std::reference_wrapper<const Match>& matchRef = matches[index];
		const MessageBuilder::EmbedParams params = BuildEmbedParams(dataReader, matchRef.get());
		MessageBuilder::AddEmbedToMessage(params, outMsg);
	}

	return endIndex;
}
