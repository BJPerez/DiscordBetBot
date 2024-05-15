#include "ChooseMatchToBetOnCommand.h"

#include <vector>

#include "DrawUtils.h"
#include "ICommandReceiver.h"

dpp::message ChooseMatchToBetOnCommand::ExecuteInternal() const
{
	const std::vector<Match>& matches = m_CommandReceiver.GetIncomingMatches();
	if (matches.empty())
	{
		return { GetAnswerChannelId(), "There are no match to bet on currently." };
	}

	dpp::message msg{ GetAnswerChannelId(), "Select the match you want to be on:" };
	msg.add_component(dpp::component().add_component(DrawUtils::CreateMatchSelector("Choose the match you want to bet on", std::string(SELECT_MENU_ID), matches)));
	return msg;
}

bool ChooseMatchToBetOnCommand::ValidateCommand(std::string&) const
{
	return true;
}