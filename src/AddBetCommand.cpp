#include "AddBetCommand.h"

#include "BotDataExceptions.h"
#include "DiscordMessageBuilder.h"
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
			return DiscordMessageBuilder::BuildBasicAnswer(GetAnswerChannelId(), std::string{ BET_MODIFIED_TXT });
		}
		else
		{
			dataWriter->AddBet(m_MatchId, m_Score, m_BettorName);
			return DiscordMessageBuilder::BuildBasicAnswer(GetAnswerChannelId(), std::string{ BET_ADDED_TXT });
		}
	}
	catch (const InvalidMatchIdException& exception) 
	{
		return DiscordMessageBuilder::BuildBasicAnswer(GetAnswerChannelId(), 
			"User error: Given ID [" + exception.GetMatchId() + "] is invalid.");
	}
	catch (const MatchNotFoundException& exception)
	{
		return DiscordMessageBuilder::BuildBasicAnswer(GetAnswerChannelId(),
			"User error: Could not find any match with the given ID [" + exception.GetMatchId() + "].");
	}
	catch (const MatchAlreadyPlayedException& exception)
	{
		return DiscordMessageBuilder::BuildBasicAnswer(GetAnswerChannelId(),
			"User error: Can't add a bet on a match already played. MatchID: [" + exception.GetMatchId() + "].");
	}
	catch (const InvalidScoreException& exception)
	{
		return DiscordMessageBuilder::BuildInvalidScoreAnswer(GetAnswerChannelId(), exception);
	}
	catch (const InvalidBettorNameException& exception)
	{
		return DiscordMessageBuilder::BuildInvalidBettorNameAnswer(GetAnswerChannelId(), exception);
	}
	catch (const BetAlreadyExistException& exception)
	{
		return DiscordMessageBuilder::BuildBasicAnswer(GetAnswerChannelId(),
			"Internal error: A bet already exist for [" + exception.GetBettorName() + "] on the match with ID: [" + exception.GetMatchId() + "].");
	}
	catch (const BetNotFoundException& exception)
	{
		return DiscordMessageBuilder::BuildBetNotFoundAnswer(GetAnswerChannelId(), exception);
	}
}
