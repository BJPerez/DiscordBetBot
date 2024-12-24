#include "AddMatchCommand.h"

#include "BotDataExceptions.h"
#include "DiscordMessageBuilder.h"
#include "ICommandReceiver.h"
#include "LockableDataAccessors.h"

namespace
{
	constexpr std::string_view MATCH_ADDED_TXT = "Match added.";
}

dpp::message AddMatchCommand::Execute() const
{
	try
	{
		const DataWriter dataWriter = GetDataWriter();
		dataWriter->AddMatch(m_MatchId, m_TeamAName, m_TeamBName, m_BoSize);
		return DiscordMessageBuilder::BuildBasicAnswer(GetAnswerChannelId(), std::string{ MATCH_ADDED_TXT });
	}
	catch (const InvalidMatchIdException& exception)
	{
		return DiscordMessageBuilder::BuildBasicAnswer(GetAnswerChannelId(),
			"Error: Given ID [" + m_MatchId.value() + "] is invalid.");
	}
	catch (const MatchIdUnavailableException& exception)
	{
		return DiscordMessageBuilder::BuildBasicAnswer(GetAnswerChannelId(),
			"User error: Given ID [" + exception.GetMatchId() + "] is not available.");
	}
	catch (const InvalidTeamNameException& exception)
	{
		return DiscordMessageBuilder::BuildBasicAnswer(GetAnswerChannelId(),
			"User error: At least one of the given team names is invalid. Team A: [" + exception.GetTeamAName() + "]. Team B: [" 
			+ exception.GetTeamBName() + "].");
	}
	catch (const InvalidBoSizeException& exception)
	{
		return DiscordMessageBuilder::BuildInvalidBoSizeAnswer(GetAnswerChannelId(), exception);
	}
}