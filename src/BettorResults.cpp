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

	for (const auto& nbCorrectBets : m_CorrectBetsByBoSize | std::views::values)
	{
		result += nbCorrectBets; // 1 pt by correct better
	}

	for (const auto& [boSize, nbPerfectBets] : m_PerfectBetsByBoSize)
	{
		result += nbPerfectBets * (boSize + 1); // 1 pt by perfect bet + 1 pt for each game possible in the BO (BO5 will gives 1 + 5 pts)
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