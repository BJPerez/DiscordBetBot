#pragma once

#include <map>
#include <string>

class BettorResults
{
public:
	explicit BettorResults(std::string bettorName): m_BettorName(std::move(bettorName)){}

	[[nodiscard]] const std::string& GetBettorName() const { return m_BettorName; }
	[[nodiscard]] const std::map<unsigned int, unsigned int>& GetPerfectBetsByBoSize() const { return m_PerfectBetsByBoSize; }
	[[nodiscard]] const std::map<unsigned int, unsigned int>& GetCorrectBetsByBoSize() const { return m_CorrectBetsByBoSize; }
	[[nodiscard]] unsigned int GetMaxBoSize() const; 
	[[nodiscard]] unsigned int GetScore() const;

	void AddResult(const bool isPerfectBet, const unsigned int boSize);

private:
	std::string m_BettorName;
	std::map<unsigned int, unsigned int> m_PerfectBetsByBoSize;
	std::map<unsigned int, unsigned int> m_CorrectBetsByBoSize;
};

