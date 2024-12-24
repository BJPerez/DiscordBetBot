#include "AddResultCommand.h"

#include "BotDataExceptions.h"
#include "DiscordMessageBuilder.h"
#include "ICommandReceiver.h"
#include "LockableDataAccessors.h"

namespace
{
	constexpr std::string_view RESULT_ADDED_TXT = "Result added.";
}

dpp::message AddResultCommand::Execute() const
{
	try
	{
		const DataWriter dataWriter = GetDataWriter();
		dataWriter->AddResult(m_MatchId, m_Score);
		return DiscordMessageBuilder::BuildBasicAnswer(GetAnswerChannelId(), std::string{RESULT_ADDED_TXT});
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
			"User error: Can't add a result to a match already played. MatchID: [" + exception.GetMatchId() + "].");
	}
	catch (const InvalidScoreException& exception)
	{
		return DiscordMessageBuilder::BuildInvalidScoreAnswer(GetAnswerChannelId(), exception);
	}
}
