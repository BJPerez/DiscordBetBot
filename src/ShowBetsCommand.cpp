#include "ShowBetsCommand.h"

#include <algorithm>

#include "Bet.h"
#include "DrawUtils.h"
#include "ICommandReceiver.h"
#include "Match.h"

namespace
{
	constexpr unsigned int TABLE_COLUMN_COUNT = 5;
}

dpp::message ShowBetsCommand::ExecuteInternal(const dpp::slashcommand_t& event) const
{
	if (const std::vector<Bet>& bets = m_CommandReceiver.GetBets(); 
		bets.empty())
	{
		return { event.command.channel_id, "Not bet to display yet." };
	}

	std::vector<std::vector<std::string>> columnsContents (TABLE_COLUMN_COUNT);
	FillColumnsWithBetInfos(columnsContents);

	std::string msgText;
	DrawUtils::DrawTable(columnsContents, msgText);
	return { event.command.channel_id, msgText };
}

bool ShowBetsCommand::ValidateCommand(std::string&) const
{
	return true;
}

void ShowBetsCommand::FillColumnsWithBetInfos(std::vector<std::vector<std::string>>& outColumnsContent) const
{
	if (outColumnsContent.size() != TABLE_COLUMN_COUNT)
	{
		return;
	}

	outColumnsContent[0].emplace_back("Bettor");
	outColumnsContent[1].emplace_back("Team A");
	outColumnsContent[2].emplace_back("Score A");
	outColumnsContent[3].emplace_back("Score B");
	outColumnsContent[4].emplace_back("Team B");

	std::ranges::for_each(m_CommandReceiver.GetBets(),
		[this, &outColumnsContent](const Bet& bet)
		{
			const std::optional<std::reference_wrapper<const Match>> matchOpt = m_CommandReceiver.GetMatch(bet.GetMatchId());
			if (!matchOpt.has_value())
			{
				return;
			}
			const Match& match = matchOpt.value().get();

			outColumnsContent[0].emplace_back(bet.GetBettorName());
			outColumnsContent[1].emplace_back(match.GetTeamAName());
			outColumnsContent[2].emplace_back(std::to_string(bet.GetScore().m_TeamAScore));
			outColumnsContent[3].emplace_back(std::to_string(bet.GetScore().m_TeamBScore));
			outColumnsContent[4].emplace_back(match.GetTeamBName());
		}
	);
}
