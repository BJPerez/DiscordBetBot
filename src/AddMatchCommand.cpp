#include "AddMatchCommand.h"

#include "BotDataExceptions.h"
#include "ICommandReceiver.h"
#include "LockableDataAccessors.h"

MessageBuilder::Message AddMatchCommand::Execute() const
{
	try
	{
		const DataWriter dataWriter = GetDataWriter();
		dataWriter->AddMatch(m_MatchId, m_TeamAName, m_TeamBName, m_BoSize);
		return MessageBuilder::BuildAnswer(GetAnswerChannelId(), "Match added.");
	}
	catch (const InvalidMatchIdException& exception)
	{
		return MessageBuilder::BuildAnswer(GetAnswerChannelId(),
			"Error: Given ID [" + exception.GetMatchId() + "] is invalid.");
	}
	catch (const MatchIdUnavailableException& exception)
	{
		return MessageBuilder::BuildAnswer(GetAnswerChannelId(),
			"User error: Given ID [" + exception.GetMatchId() + "] is not available.");
	}
	catch (const InvalidTeamNameException& exception)
	{
		return MessageBuilder::BuildAnswer(GetAnswerChannelId(),
			"User error: At least one of the given team names is invalid. Team A: [" + exception.GetTeamAName() + "]. Team B: [" 
			+ exception.GetTeamBName() + "].");
	}
	catch (const InvalidBoSizeException& exception)
	{
		return MessageBuilder::BuildAnswer(GetAnswerChannelId(),
			"User error: The given BoSize [" + std::to_string(exception.GetBoSize()) + "] is invalid. It must be an odd number.");
	}
}