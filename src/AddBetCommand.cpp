#include "AddBetCommand.h"

#include "BotDataExceptions.h"
#include "ICommandReceiver.h"
#include "LockableDataAccessors.h"

namespace
{
	constexpr std::string_view BET_ADDED_TXT = "Bet added.";
	constexpr std::string_view BET_MODIFIED_TXT = "Your already existing bet has been modified.";
}

dpp::message AddBetCommand::Execute() const 
{
	try 
	{
		if (const DataWriter dataWriter = GetDataWriter(); 
			dataWriter->HasBet(m_MatchId, m_BettorName))
		{
			dataWriter->ModifyBet(m_MatchId, m_Score, m_BettorName);
			return MessageBuilder::BuildAnswer(GetAnswerChannelId(), std::string{ BET_MODIFIED_TXT });
		}
		else
		{
			dataWriter->AddBet(m_MatchId, m_Score, m_BettorName);
			return MessageBuilder::BuildAnswer(GetAnswerChannelId(), std::string{ BET_ADDED_TXT });
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
		return MessageBuilder::BuildAnswer(GetAnswerChannelId(),
			"Internal error: A bet already exist for [" + exception.GetBettorName() + "] on the match with ID: [" + exception.GetMatchId() + "].");
	}
	catch (const BetNotFoundException& exception)
	{
		return MessageBuilder::BuildAnswer(GetAnswerChannelId(),
			"Internal error: Tried to modify a non existing bet for [" + exception.GetBettorName() + "] on the match with ID: [" + exception.GetMatchId() + "].");
	}
}
