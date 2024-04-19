#include "ShowBettorsResultsCommand.h"

#include <algorithm>

#include "BettorResults.h"
#include "ICommandReceiver.h"

void ShowBettorsResultsCommand::ExecuteInternal(std::string& outAnswerToUser) const
{
	outAnswerToUser.clear();

	const std::vector<BettorResults>& allResults = m_CommandReceiver.GetBettorsResults();
	if (allResults.empty())
	{
		outAnswerToUser += "No result to display yet.";
		return;
	}

	unsigned int maxBoSize = 0;
	std::ranges::for_each(allResults,
		[&maxBoSize](const BettorResults& results)
		{
			maxBoSize = std::max(maxBoSize, results.GetMaxBoSize());
		}
	);

	std::ranges::for_each(allResults,
		[&outAnswerToUser, maxBoSize](const BettorResults& results)
		{
			outAnswerToUser += results.GetBettorName() + ": ";

			const std::map<unsigned int, unsigned int>& perfectBetsByBoSize = results.GetPerfectBetsByBoSize();
			const std::map<unsigned int, unsigned int>& correctBetsByBoSize = results.GetCorrectBetsByBoSize();

			int currentBoSize = static_cast<int>(maxBoSize);
			while (currentBoSize >= 1)
			{
				const unsigned int perfectBets = perfectBetsByBoSize.contains(currentBoSize) ? perfectBetsByBoSize.at(currentBoSize) : 0;
				const unsigned int correctBets = correctBetsByBoSize.contains(currentBoSize) ? correctBetsByBoSize.at(currentBoSize) : 0;

				outAnswerToUser += "BO" + std::to_string(currentBoSize) + ": " + std::to_string(perfectBets) + " PB (" + std::to_string(perfectBets * currentBoSize) + "pts)" +
					" and " + std::to_string(correctBets) + " CB (" + std::to_string(correctBets) + "pts)   |   ";
				currentBoSize -= 2;
			}

			outAnswerToUser += "Total: " + std::to_string(results.GetScore()) + "pts\n";
		}
	);

	outAnswerToUser += "\nPB = Perfect Bet (winning team + exact score)    |     CB = Correct Bet (winning team only)";
}

bool ShowBettorsResultsCommand::ValidateCommand(std::string&) const
{
	return true;
}