#include "ChooseMatchToSetResultCommand.h"

#include <vector>

#include "ICommandReceiver.h"
#include "LockableDataAccessors.h"
#include "Match.h"

MessageBuilder::Message ChooseMatchToSetResultCommand::Execute() const
{
	const DataReader dataReader = GetDataReader();
	if (const std::vector<std::reference_wrapper<const Match>> matches = dataReader->GetIncomingMatches();
		matches.empty())
	{
		return MessageBuilder::BuildAnswer(GetAnswerChannelId(), "There are no match to add a result to.");
	}
	else
	{
		const MessageBuilder::SelectorParams params
		{
			std::string(SELECT_MENU_ID) ,
			Helper::BuildMatchSelectorOptions(matches),
			"Match"
		};

		MessageBuilder::Message msg = MessageBuilder::BuildAnswer(GetAnswerChannelId(), "Select the match you want to add a result to:");
		MessageBuilder::AddSelectorToMessage(params, msg);
		return msg;
	}
}