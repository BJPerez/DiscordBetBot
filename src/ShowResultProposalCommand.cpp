#include "ShowResultProposalCommand.h"

#include "BotDataExceptions.h"
#include "ICommandReceiver.h"
#include "LockableDataAccessors.h"
#include "Match.h"

MessageBuilder::Message ShowResultProposalCommand::Execute() const
{
	try
	{
		const DataReader dataReader = GetDataReader(); 
		const Match& match = dataReader->GetMatch(m_MatchId);

		const std::string content = "Choose your bet for " + match.ToString() + ":";
		MessageBuilder::Message msg = MessageBuilder::BuildAnswer(GetAnswerChannelId(), content);

		const MessageBuilder::SelectorParams params
		{
			std::string {SELECT_MENU_ID},
			Helper::BuildScoreSelectorOptions(match),
			match.ToString()
		};
		MessageBuilder::AddSelectorToMessage(params, msg);

		return msg;
	}
	catch (const InvalidMatchIdException& exception)
	{
		return MessageBuilder::BuildAnswer(GetAnswerChannelId(),
			"User error: Given ID [" + exception.GetMatchId() + "] is invalid.");
	}
	catch (const MatchNotFoundException& exception)
	{
		return MessageBuilder::BuildAnswer(GetAnswerChannelId(),
			"User error: Could not find any match with the given ID [" + exception.GetMatchId() + "].");
	}
}