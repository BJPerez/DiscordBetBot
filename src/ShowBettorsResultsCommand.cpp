#include "ShowBettorsResultsCommand.h"

#include <algorithm>

#include "BettorResults.h"
#include "DrawUtils.h"
#include "ICommandReceiver.h"
#include "LockableDataAccessors.h"

namespace
{
	constexpr unsigned int COLUMNS_COUNT_OUTSIDE_OF_BOSIZE_COLUMNS = 3; // One for place, one for bettor's name and one for total score

	void FillColumnHeaders(const unsigned int maxBoSize, std::vector<std::vector<std::string>>& outColumns)
	{
		unsigned int columnIndex{ 0 };
		outColumns[columnIndex++].emplace_back("Place");
		outColumns[columnIndex++].emplace_back("Bettor");

		for (int currentBoSize = static_cast<int>(maxBoSize); currentBoSize > 1; currentBoSize -= 2)
		{
			outColumns[columnIndex++].emplace_back("BO" + std::to_string(currentBoSize) + " PB");
			outColumns[columnIndex++].emplace_back("BO" + std::to_string(currentBoSize) + " CB");
		}
		outColumns[columnIndex++].emplace_back("BO1 PB"); // BO1s can only be perfect

		outColumns[columnIndex++].emplace_back("Total"); 
	}

	void FillBoSizeColumns(const std::vector<BettorResults::ResultsByBoSize>& resultsByBoSize, const unsigned int boSize, unsigned int& columnIndex, std::vector<std::vector<std::string>>& outColumns)
	{
		const auto predicate =
			[boSize](const BettorResults::ResultsByBoSize& results)
			{
				return results.m_BoSize == static_cast<unsigned int>(boSize);
			};

		if (const auto resultsIt = std::ranges::find_if(resultsByBoSize, predicate); resultsIt == resultsByBoSize.end())
		{
			outColumns[columnIndex++].emplace_back("0/0");
			if (boSize != 1)
			{
				outColumns[columnIndex++].emplace_back("0/0"); // BO1s can only be perfect
			}
		}
		else
		{
			const unsigned int totalBetsCount = resultsIt->GetTotalBetCount();
			outColumns[columnIndex++].emplace_back(std::to_string(resultsIt->m_PerfectBetsCount) + "/" + std::to_string(totalBetsCount));
			if (boSize != 1)
			{
				outColumns[columnIndex++].emplace_back(std::to_string(resultsIt->m_CorrectBetsCount) + "/" + std::to_string(totalBetsCount)); // BO1s can only be perfect
			}
		}
	}
	 
	void FillColumns(const std::vector<BettorResults>& allBettorResults, const unsigned int maxBoSize, std::vector<std::vector<std::string>>& outColumns)
	{
		unsigned int place = 1;
		for (const BettorResults& bettorResults : allBettorResults)
		{
			unsigned int columnIndex = 0;
			outColumns[columnIndex++].emplace_back(std::to_string(place++));
			outColumns[columnIndex++].emplace_back(bettorResults.GetBettorName());

			const std::vector<BettorResults::ResultsByBoSize>& resultsByBoSize = bettorResults.GetResults();
			for (int currentBoSize = static_cast<int>(maxBoSize); currentBoSize > 0; currentBoSize -= 2)
			{
				FillBoSizeColumns(resultsByBoSize, currentBoSize, columnIndex, outColumns);
			}

			outColumns[columnIndex++].emplace_back(std::to_string(bettorResults.GetScore()) + " pts");
		}
	}

	unsigned int EvaluateMaxBoSize(const DataReader<ICommandReceiver>& dataReader) noexcept
	{
		unsigned int maxBoSize{ 0 };
		for (const BettorResults& results : dataReader->GetBettorsResults())
		{
			maxBoSize = std::max(maxBoSize, results.GetMaxBoSize());
		}
		return maxBoSize;
	}

	std::vector<std::vector<std::string>> GenerateColumnsWithResultsInfos(const DataReader<ICommandReceiver>& dataReader)
	{
		const unsigned int maxBoSize = EvaluateMaxBoSize(dataReader);
		const unsigned int boSizesCount = (maxBoSize + 1) / 2; // We go two by two from 1 to maxBoSize. So if maxBoSize = 5, we have 1, 3 and 5 so 3 different bo sizes
		const unsigned int columnsCount = COLUMNS_COUNT_OUTSIDE_OF_BOSIZE_COLUMNS + (boSizesCount * 2 - 1); // *2 -1 because every bosize (except for BO1s thus the -1) will have one column for perfect bet and one for correct bet

		// Note that Bettors result should already be sorted from the bettor with the most point to the less.
		std::vector<std::vector<std::string>> columnsContents(columnsCount);
		FillColumnHeaders(maxBoSize, columnsContents);
		FillColumns(dataReader->GetBettorsResults(), maxBoSize, columnsContents);

		return columnsContents;
	}
}

dpp::message ShowBettorsResultsCommand::Execute() const
{
	dpp::message msg{ GetAnswerChannelId(), "" };
	msg.set_flags(dpp::m_ephemeral);

	{
		const DataReader dataReader = GetDataReader();
		if (const std::vector<BettorResults>& allResults = dataReader->GetBettorsResults();
			allResults.empty())
		{
			msg.set_content("No result to display yet.");
		}
		else
		{
			std::string msgText;
			const std::vector<std::vector<std::string>> columnsContents = GenerateColumnsWithResultsInfos(dataReader);
			DrawUtils::DrawTable(columnsContents, msgText);
			msgText += "PB = Perfect Bet (winning team + exact score)    |     CB = Correct Bet (winning team only)";

			msg.set_content(msgText);
		}
	}

	return msg;
}