#include "BettorResults.h"

#include <ranges>

namespace
{
	void IncrementValueAt(std::map<unsigned int, unsigned int>& container, const unsigned int key)
	{
		if (container.contains(key))
		{
			++container.at(key);
		}
		else
		{
			container[key] = 1;
		}
	}
}

void BettorResults::AddResult(const bool isPerfectBet, const unsigned int boSize)
{
	if (isPerfectBet)
	{
		IncrementValueAt(m_PerfectBetsByBoSize, boSize);
	}
	else
	{
		IncrementValueAt(m_CorrectBetsByBoSize, boSize);
	}
}

unsigned int BettorResults::GetScore() const
{
	unsigned int result{ 0 };

	for (const auto& nbPerfectBest : m_PerfectBetsByBoSize | std::views::values)
	{
		result += nbPerfectBest;
	}

	for (const auto& [boSize, nbCorrectBets] : m_PerfectBetsByBoSize)
	{
		result += nbCorrectBets * boSize;
	}

	return result;
}

unsigned int BettorResults::GetMaxBoSize() const
{
	if (m_PerfectBetsByBoSize.empty() && m_CorrectBetsByBoSize.empty())
	{
		return 0;
	}

	if (m_PerfectBetsByBoSize.empty())
	{
		return m_CorrectBetsByBoSize.rbegin()->first;
	}

	if (m_CorrectBetsByBoSize.empty())
	{
		return m_PerfectBetsByBoSize.rbegin()->first;
	}

	return std::max(m_CorrectBetsByBoSize.rbegin()->first, m_PerfectBetsByBoSize.rbegin()->first);
}