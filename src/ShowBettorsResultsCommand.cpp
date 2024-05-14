#include "ShowBettorsResultsCommand.h"

#include <algorithm>

#include "BettorResults.h"
#include "DrawUtils.h"
#include "ICommandReceiver.h"

namespace
{
	constexpr unsigned int TABLE_COLUMN_COUNT = 9;
	constexpr unsigned int MAX_BO_SIZE = 5;

	unsigned int GetValueFromMap(const std::map<unsigned int, unsigned int>& map, const unsigned int key)
	{
		if (map.contains(key))
		{
			return map.at(key);
		}
		return 0;
	}
}

dpp::message ShowBettorsResultsCommand::ExecuteInternal() const
{
	if (const std::vector<BettorResults>& allResults = m_CommandReceiver.GetBettorsResults(); 
		allResults.empty())
	{
		return { GetAnswerChannelId(), "No result to display yet." };
	}

	std::vector<std::vector<std::string>> columnsContents(TABLE_COLUMN_COUNT);
	FillColumnsWithResultsInfos(columnsContents);

	std::string msgText;
	DrawUtils::DrawTable(columnsContents, msgText);
	msgText += "PB = Perfect Bet (winning team + exact score)    |     CB = Correct Bet (winning team only)";

	return { GetAnswerChannelId(), msgText };
}

bool ShowBettorsResultsCommand::ValidateCommand(std::string&) const
{
	return true;
}

void ShowBettorsResultsCommand::FillColumnsWithResultsInfos(std::vector<std::vector<std::string>>& outColumnsContent) const
{
	if (outColumnsContent.size() != TABLE_COLUMN_COUNT)
	{
		return;
	}

	outColumnsContent[0].emplace_back("Place");
	outColumnsContent[1].emplace_back("Bettor");
	outColumnsContent[2].emplace_back("BO5 PB");
	outColumnsContent[3].emplace_back("BO5 CB");
	outColumnsContent[4].emplace_back("BO3 PB");
	outColumnsContent[5].emplace_back("BO3 CB");
	outColumnsContent[6].emplace_back("BO1 PB");
	outColumnsContent[7].emplace_back("BO1 CB");
	outColumnsContent[8].emplace_back("Total");

	unsigned int place = 1;
	std::ranges::for_each(m_CommandReceiver.GetBettorsResults(),
		[this, &outColumnsContent, &place](const BettorResults& results)
		{
			outColumnsContent[0].emplace_back(std::to_string(place++));
			outColumnsContent[1].emplace_back(results.GetBettorName());

			unsigned int columnIndex = 2;
			for (int boSize = MAX_BO_SIZE; boSize > 0; boSize -= 2)
			{
				const unsigned int perfectBets = GetValueFromMap(results.GetPerfectBetsByBoSize(), boSize);
				const unsigned int perfectBetsPoints = perfectBets * (boSize + 1);
				const std::string perfectBetsStr = std::to_string(perfectBets) + " (" + std::to_string(perfectBetsPoints) + "pts)";
				outColumnsContent[columnIndex++].emplace_back(perfectBetsStr);

				const unsigned int correctBet = GetValueFromMap(results.GetCorrectBetsByBoSize(), boSize);
				const std::string correctBetStr = std::to_string(correctBet) + " (" + std::to_string(correctBet) + "pts)";
				outColumnsContent[columnIndex++].emplace_back(correctBetStr);
			}

			outColumnsContent[8].emplace_back(std::to_string(results.GetScore()));
		}
	);
}