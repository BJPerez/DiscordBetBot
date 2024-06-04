#include "BettorResults.h"

#include <algorithm>
#include <functional>
#include <iostream>
#include <numeric>

#include "MatchScore.h"

constexpr unsigned int CORRECT_WINNING_TEAM_SCORE = 2;

std::strong_ordering BettorResults::operator<=>(const BettorResults& other) const noexcept
{
	if (const std::strong_ordering res = GetScore() <=> other.GetScore(); res != std::strong_ordering::equal)
	{
		return res;
	}

	if (const std::strong_ordering res = GetPerfectBetsCount() <=> other.GetPerfectBetsCount(); res != std::strong_ordering::equal)
	{
		return res;
	}
	return m_BettorName <=> other.m_BettorName;
}

void BettorResults::AddResult(const unsigned int boSize, const MatchScore& matchScore, const MatchScore& betScore)
{
	auto& [resultBoSize, perfectBetsCount, correctBetsCount, incorrectBetsCount, score] = GetOrCreateBoSizeResults(boSize);

	// The formula to evaluate the score works like this:
	// - If the bet is at least correct (correct winning team), the bettor wins CORRECT_WINNING_TEAM_SCORE
	// - Then you have bonus point depending on if you are close to the exact result or not.
	//   The minimal bonus is 0
	//   The maximal bonus is ((boSize + 1) / 2) - 1. For a BO5 it would be 2 for example. 
	switch (matchScore.Compare(betScore))
	{
		case MatchScore::ComparingResult::Perfect:
		{
			++perfectBetsCount;
			score += CORRECT_WINNING_TEAM_SCORE + ((boSize + 1) / 2) - 1;
			break;
		}
		case MatchScore::ComparingResult::Correct:
		{
			++correctBetsCount;

			const unsigned int matchLoosingTeamScore = matchScore.GetLoosingTeamScore();
			const unsigned int betLoosingTeamScore = betScore.GetLoosingTeamScore();
			const unsigned int loosingTeamDelta = std::max(matchLoosingTeamScore, betLoosingTeamScore) - std::min(matchLoosingTeamScore, betLoosingTeamScore);
			score += CORRECT_WINNING_TEAM_SCORE + ((boSize + 1) / 2)  - (loosingTeamDelta + 1);
		}
		case MatchScore::ComparingResult::Incorrect:
		{
			++incorrectBetsCount;
		}
	}
}

unsigned int BettorResults::GetScore() const noexcept
{
	unsigned int score{ 0 };
	for (const ResultsByBoSize& result : m_Results)
	{
		score += result.m_Score;
	}
	return score;
}

unsigned int BettorResults::GetMaxBoSize() const noexcept
{
	unsigned int maxBoSize{ 0 };
	for (const ResultsByBoSize& result : m_Results)
	{
		maxBoSize = std::max(maxBoSize, result.m_BoSize);
	}
	return maxBoSize;
}

BettorResults::ResultsByBoSize& BettorResults::GetOrCreateBoSizeResults(const unsigned int boSize)
{
	const auto predicate =
		[boSize](const ResultsByBoSize& result)
		{
			return result.m_BoSize == boSize;
		};

	if (const auto it = std::ranges::find_if(m_Results, predicate); it != m_Results.end())
	{
		return *it;
	}
	else
	{
		return m_Results.emplace_back(ResultsByBoSize{ boSize, 0, 0, 0 });
	}
}

unsigned int BettorResults::GetPerfectBetsCount() const noexcept
{
	unsigned int count{ 0 };
	for (const ResultsByBoSize& result : m_Results)
	{
		count += result.m_PerfectBetsCount;
	}
	return count;
}