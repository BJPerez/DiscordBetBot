#include "ChooseMatchToSetResultCommand.h"

#include <vector>

#include "DrawUtils.h"
#include "LockableDataAccessors.h"
#include "Match.h"

dpp::message ChooseMatchToSetResultCommand::Execute() const
{
	dpp::message msg{ GetAnswerChannelId(), "" };
	msg.set_flags(dpp::m_ephemeral);

	{
		const DataReader dataReader = GetDataReader();
		if (const std::vector<Match>& matches = dataReader->GetIncomingMatches();
			matches.empty())
		{
			msg.set_content("At the moment, there are no match to add a result to.");
		}
		else
		{
			msg.set_content("Select the match you want to add a result to:");
			msg.add_component(dpp::component().add_component(DrawUtils::CreateMatchSelector("Choose the match you want to add a result to", std::string(SELECT_MENU_ID), matches)));
		}
	}

	return msg;
}