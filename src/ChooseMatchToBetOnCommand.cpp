#include "ChooseMatchToBetOnCommand.h"

#include <vector>

#include "ICommandReceiver.h"
#include "LockableDataAccessors.h"
#include "Match.h"

MessageBuilder::Message ChooseMatchToBetOnCommand::Execute() const
{
	const DataReader dataReader = GetDataReader();
	if (const std::vector<std::reference_wrapper<const Match>> matches = dataReader->GetIncomingMatches();
		matches.empty())
	{
		return MessageBuilder::BuildAnswer(GetAnswerChannelId(), "There are no match to bet on currently.");
	}
	else
	{
		const MessageBuilder::SelectorParams params
		{
			std::string(SELECT_MENU_ID) ,
			Helper::BuildMatchSelectorOptions(matches),
			"Match"
		};

		MessageBuilder::Message msg = MessageBuilder::BuildAnswer(GetAnswerChannelId(), "Select the match you want to bet on:");
		MessageBuilder::AddSelectorToMessage(params, msg);
		return msg;
	}
}