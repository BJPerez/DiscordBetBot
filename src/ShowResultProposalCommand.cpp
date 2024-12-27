#include "ShowResultProposalCommand.h"

#include "BotDataExceptions.h"
#include "MessageBuilder.h"
#include "ICommandReceiver.h"
#include "LockableDataAccessors.h"
#include "Match.h"

dpp::message ShowResultProposalCommand::Execute() const
{
	try
	{
		const DataReader dataReader = GetDataReader(); 
		const Match& match = dataReader->GetMatch(m_MatchId);

		const MessageBuilder::SelectorParams params
		{
			std::string {SELECT_MENU_ID},
			CommandBase::Helper::BuildAllScoreScoreSelectorOptions(match),
			match.GetTeamAName() + " - " + match.GetTeamBName()
		};

		std::string content = "Choose your bet for " + match.GetTeamAName() + " - " + match.GetTeamBName() + ":";
		dpp::message msg = MessageBuilder::BuildAnswer(GetAnswerChannelId(), std::move(content));
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