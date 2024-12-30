#include "AddResultCommand.h"

#include "BotDataExceptions.h"
#include "ICommandReceiver.h"
#include "LockableDataAccessors.h"

MessageBuilder::Message AddResultCommand::Execute() const
{
	try
	{
		const DataWriter dataWriter = GetDataWriter();
		dataWriter->AddResult(m_MatchId, m_Score);
		return MessageBuilder::BuildAnswer(GetAnswerChannelId(), "Result added.");
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
	catch (const MatchAlreadyPlayedException& exception)
	{
		return MessageBuilder::BuildAnswer(GetAnswerChannelId(),
			"User error: Can't add a result to a match already played. MatchID: [" + exception.GetMatchId() + "].");
	}
	catch (const InvalidScoreException& exception)
	{
		return MessageBuilder::BuildAnswer(GetAnswerChannelId(),
			"User error: Given score [" + exception.GetScore().ToString() + "] is not valid for a BO" 
			+ std::to_string(exception.GetBoSize()) + ".");
	}
}
