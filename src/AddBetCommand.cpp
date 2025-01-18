#include "AddBetCommand.h"

#include "BotDataExceptions.h"
#include "ICommandReceiver.h"
#include "LockableDataAccessors.h"

MessageBuilder::Message AddBetCommand::Execute() const  
{
	try 
	{
		if (const DataWriter dataWriter = GetDataWriter(); 
			dataWriter->HasBet(m_MatchId, m_BettorName))
		{
			dataWriter->ModifyBet(m_MatchId, m_Score, m_BettorName);
			return MessageBuilder::BuildAnswer(GetAnswerChannelId(), "Your already existing bet has been modified.");
		}
		else
		{
			dataWriter->AddBet(m_MatchId, m_Score, m_BettorName);
			return MessageBuilder::BuildAnswer(GetAnswerChannelId(), "Bet added.");
		}
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
			"User error: Can't add a bet on a match already played. MatchID: [" + exception.GetMatchId() + "].");
	}
	catch (const InvalidScoreException& exception)
	{
		return MessageBuilder::BuildAnswer(GetAnswerChannelId(),
			"User error: Given score [" + exception.GetScore().ToString() + "] is not valid for a BO"
			+ std::to_string(exception.GetBoSize()) + ".");
	}
	catch (const InvalidBettorNameException& exception)
	{
		return MessageBuilder::BuildAnswer(GetAnswerChannelId(),
			"Internal error: Extracted bettor's name is invalid: [" + exception.GetBettorName() + "].");
	}
	catch (const BetAlreadyExistException& exception)
	{
		// Should never end up here since we check first if we already have a bet. 
		return MessageBuilder::BuildAnswer(GetAnswerChannelId(),
			"Internal error: A bet already exist for [" + exception.GetBettorName() + "] on the match with ID: [" + exception.GetMatchId() + "].");
	}
	catch (const BetNotFoundException& exception)
	{
		// Should never end up here since we check first if we already have a bet.
		return MessageBuilder::BuildAnswer(GetAnswerChannelId(),
			"Internal error: Tried to modify a non existing bet for [" + exception.GetBettorName() + "] on the match with ID: [" + exception.GetMatchId() + "].");
	}
}
