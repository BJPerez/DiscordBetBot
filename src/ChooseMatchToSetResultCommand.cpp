#include "ChooseMatchToSetResultCommand.h"

#include <vector>

#include "DrawUtils.h"
#include "ICommandReceiver.h"

dpp::message ChooseMatchToSetResultCommand::ExecuteInternal() const
{
	const std::vector<Match>& matches = m_CommandReceiver.GetIncomingMatches();
	if (matches.empty())
	{
		return { GetAnswerChannelId(), "At the moment, there are no match to add a result to." };
	}

	dpp::message msg{ GetAnswerChannelId(), "Select the match you want to add a result to:" };
	msg.add_component(dpp::component().add_component(DrawUtils::CreateMatchSelector("Choose the match you want to add a result to", std::string(SELECT_MENU_ID), matches)));
	return msg;
}

bool ChooseMatchToSetResultCommand::ValidateCommand(std::string&) const
{
	return true;
}