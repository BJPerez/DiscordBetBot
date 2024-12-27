#include "ShowBettorsResultsCommand.h"

#include <algorithm>

#include "BettorResults.h"
#include "BotDataExceptions.h"
#include "MessageBuilder.h"
#include "ICommandReceiver.h"
#include "LockableDataAccessors.h"

namespace
{
	constexpr size_t COLUMNS_COUNT_OUTSIDE_OF_BO_SIZE_COLUMNS = 3; // One for place, one for bettor's name and one for total score
	constexpr int JUMP_BETWEEN_BO_SIZE = 2;
	constexpr int MINIMAL_BO_SIZE = 1;

	void FillColumnHeaders(const unsigned int maxBoSize, std::vector<std::vector<std::string>>& outColumns)
	{
		unsigned int columnIndex{ 0 };
		outColumns[columnIndex++].emplace_back("Place");
		outColumns[columnIndex++].emplace_back("Bettor");

		for (int currentBoSize = static_cast<int>(maxBoSize); currentBoSize > MINIMAL_BO_SIZE; currentBoSize -= JUMP_BETWEEN_BO_SIZE)
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
			for (int currentBoSize = static_cast<int>(maxBoSize); currentBoSize >= MINIMAL_BO_SIZE; currentBoSize -= JUMP_BETWEEN_BO_SIZE)
			{
				FillBoSizeColumns(resultsByBoSize, currentBoSize, columnIndex, outColumns);
			}

			outColumns[columnIndex++].emplace_back(std::to_string(bettorResults.GetScore()) + " pts");
		}
	}

	unsigned int EvaluateMaxBoSize(const std::vector<BettorResults>& allResults) noexcept
	{
		unsigned int maxBoSize{ 0 };
		for (const BettorResults& results : allResults)
		{
			maxBoSize = std::max(maxBoSize, results.GetMaxBoSize());
		}
		return maxBoSize;
	}

	std::vector<std::vector<std::string>> GenerateColumnsWithResultsInfos(const std::vector<BettorResults>& allResults)
	{
		const unsigned int maxBoSize = EvaluateMaxBoSize(allResults);
		const unsigned int boSizesCount = (maxBoSize + 1) / 2; // We go two by two from 1 to maxBoSize. So if maxBoSize = 5, we have 1, 3 and 5 so 3 different bo sizes
		const unsigned int columnsCount = COLUMNS_COUNT_OUTSIDE_OF_BO_SIZE_COLUMNS + (boSizesCount * 2 - 1); // *2 -1 because every bosize (except for BO1s thus the -1) will have one column for perfect bet and one for correct bet

		// Note that Bettors result should already be sorted from the bettor with the most point to the less.
		std::vector<std::vector<std::string>> columnsContents(columnsCount);
		FillColumnHeaders(maxBoSize, columnsContents);
		FillColumns(allResults, maxBoSize, columnsContents);

		return columnsContents;
	}

	BettorResults& GetOrCreateBettorResults(const std::string& bettorName, std::vector<BettorResults>& allResults)
	{
		const auto pred =
			[&bettorName](const BettorResults& results)
			{
				return results.GetBettorName() == bettorName;
			}
		;

		if (const auto it = std::ranges::find_if(allResults, pred);
			it == allResults.end())
		{
			return allResults.emplace_back(bettorName);
		}
		else
		{
			return *it;
		}
	}

	std::vector<BettorResults> GenerateResults(const DataReader<ICommandReceiver>& data)
	{
		std::vector<BettorResults> allResults;
		for (const Bet& bet : data->GetBets())
		{
			const Match& match = data->GetMatch(bet.GetMatchId());
			if (!match.IsPlayed())
			{
				continue;
			}

			BettorResults& results = GetOrCreateBettorResults(bet.GetBettorName(), allResults);
			results.AddResult(match.GetBoSize(), match.GetResult(), bet.GetScore());
		}
		std::ranges::sort(allResults, std::greater());
		return allResults;
	}
}

dpp::message ShowBettorsResultsCommand::Execute() const
{
	try
	{
		const DataReader dataReader = GetDataReader();
		if (const std::vector<BettorResults> allResults = GenerateResults(dataReader);
			allResults.empty())
		{
			return MessageBuilder::BuildAnswer(GetAnswerChannelId(), "No result to display yet.");
		}
		else
		{
			std::string msgContent = MessageBuilder::BuildTable(GenerateColumnsWithResultsInfos(allResults));
			msgContent += "PB = Perfect Bet (winning team + exact score)    |     CB = Correct Bet (winning team only)";

			return MessageBuilder::BuildAnswer(GetAnswerChannelId(), msgContent);
		}
	}
	catch (const InvalidMatchIdException& exception)
	{
		return MessageBuilder::BuildAnswer(GetAnswerChannelId(),
			"Internal error: At least one bet saved in data has an invalid MatchId [." + exception.GetMatchId() + "].");
	}
	catch (const MatchNotFoundException& exception)
	{
		return MessageBuilder::BuildAnswer(GetAnswerChannelId(),
			"Internal error: At least one bet saved in data reference a non existing MatchId [." + exception.GetMatchId() + "].");
	}
	catch (const InvalidScoreException& exception)
	{
		return MessageBuilder::BuildAnswer(GetAnswerChannelId(),
			"Internal error: A least one bet saved in data has a score [" + exception.GetScore().ToString() + 
			"] not valid for its BOSize [" + std::to_string(exception.GetBoSize()) + "].");
	}
}