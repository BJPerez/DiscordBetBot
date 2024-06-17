#include "ChooseMatchToBetOnCommand.h"

#include <vector>

#include "DrawUtils.h"
#include "ICommandReceiver.h"
#include "LockableDataAccessors.h"

dpp::message ChooseMatchToBetOnCommand::Execute() const
{
	dpp::message msg{ GetAnswerChannelId(), "" };
	msg.set_flags(dpp::m_ephemeral);

	{
		const DataReader dataReader = GetDataReader();
		if (const std::vector<Match>& matches = dataReader->GetIncomingMatches();
			matches.empty())
		{
			msg.set_content("There are no match to bet on currently.");
		}
		else
		{
			msg.set_content("Select the match you want to be on:");
			msg.add_component(dpp::component().add_component(DrawUtils::CreateMatchSelector("Choose the match you want to bet on", std::string(SELECT_MENU_ID), matches)));
		}
	}

	return msg;
}