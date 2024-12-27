#include "ChooseMatchToSetResultCommand.h"

#include <vector>

#include "MessageBuilder.h"
#include "LockableDataAccessors.h"
#include "Match.h"

namespace
{
	constexpr std::string_view NO_MATCH_TXT = "There are no match to add a result to.";
	constexpr std::string_view SELECT_MATCH_TXT = "Select the match you want to add a result to:";
	constexpr std::string_view SELECTOR_PLACE_HOLDER = "Choose the match you want to add a result to";
}

dpp::message ChooseMatchToSetResultCommand::Execute() const
{
	const DataReader dataReader = GetDataReader();
	if (const std::vector<std::reference_wrapper<const Match>> matches = dataReader->GetIncomingMatches();
		matches.empty())
	{
		return MessageBuilder::BuildAnswer(GetAnswerChannelId(), std::string{ NO_MATCH_TXT });
	}
	else
	{
		std::vector<std::pair<std::string, std::string>> selectorOptions;
		for (const std::reference_wrapper<const Match>& matchRef : matches)
		{
			const Match& match = matchRef.get();
			selectorOptions.emplace_back(match.GetTeamAName() + " - " + match.GetTeamBName(), match.GetId());
		}

		const MessageBuilder::SelectorParams params
		{
			std::string(SELECT_MENU_ID) ,
			selectorOptions,
			std::string{SELECTOR_PLACE_HOLDER}
		};

		dpp::message msg = MessageBuilder::BuildAnswer(GetAnswerChannelId(), std::string{ SELECT_MATCH_TXT });
		MessageBuilder::AddSelectorToMessage(params, msg);
		return msg;
	}
}