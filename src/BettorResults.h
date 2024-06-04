#pragma once

#include <string>
#include <vector>

struct MatchScore;

class BettorResults
{
public:
	struct ResultsByBoSize
	{
		unsigned int m_BoSize{ 1 };
		unsigned int m_PerfectBetsCount{ 0 };
		unsigned int m_CorrectBetsCount{ 0 };
		unsigned int m_IncorrectBetsCount{ 0 };
		unsigned int m_Score{ 0 };

		[[nodiscard]] unsigned int GetTotalBetCount() const noexcept { return m_CorrectBetsCount + m_IncorrectBetsCount + m_PerfectBetsCount; }
	};

	BettorResults() = default; // should only be used by serialization
	explicit BettorResults(std::string bettorName): m_BettorName(std::move(bettorName)){}

	std::strong_ordering operator<=>(const BettorResults& other) const noexcept;

	[[nodiscard]] const std::string& GetBettorName() const noexcept { return m_BettorName; }
	[[nodiscard]] const std::vector<ResultsByBoSize>& GetResults() const noexcept { return m_Results; }
	[[nodiscard]] unsigned int GetMaxBoSize() const noexcept; 
	[[nodiscard]] unsigned int GetScore() const noexcept;
	[[nodiscard]] unsigned int GetPerfectBetsCount() const noexcept;

	void SetBettorName(std::string bettorName) noexcept { m_BettorName = std::move(bettorName); }
	void SetResults(std::vector<ResultsByBoSize> results) noexcept { m_Results = std::move(results); }

	void AddResult(const unsigned int boSize, const MatchScore& matchScore, const MatchScore& betScore );

private:
	std::string m_BettorName;
	std::vector<ResultsByBoSize> m_Results;

	[[nodiscard]] ResultsByBoSize& GetOrCreateBoSizeResults(const unsigned int boSize);
};

