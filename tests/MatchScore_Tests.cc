#include <gtest/gtest.h>

#include "../src/MatchScore.h"

TEST(MatchScore_Compare, Perfect)
{
	constexpr MatchScore aScore{ 1,0 };
	constexpr MatchScore bScore{ 1,0 };
	EXPECT_EQ(aScore.Compare(bScore), MatchScore::EComparingResult::Perfect);
	EXPECT_EQ(bScore.Compare(aScore), MatchScore::EComparingResult::Perfect);

	constexpr MatchScore cScore{ 0,1 };
	constexpr MatchScore dScore{ 0,1 };
	EXPECT_EQ(cScore.Compare(dScore), MatchScore::EComparingResult::Perfect);
	EXPECT_EQ(dScore.Compare(cScore), MatchScore::EComparingResult::Perfect);

	constexpr MatchScore eScore{ 3,1 };
	constexpr MatchScore fScore{ 3,1 };
	EXPECT_EQ(eScore.Compare(fScore), MatchScore::EComparingResult::Perfect);
	EXPECT_EQ(fScore.Compare(eScore), MatchScore::EComparingResult::Perfect);

	constexpr MatchScore gScore{ 1,2 };
	constexpr MatchScore hScore{ 1,2 };
	EXPECT_EQ(gScore.Compare(hScore), MatchScore::EComparingResult::Perfect);
	EXPECT_EQ(hScore.Compare(gScore), MatchScore::EComparingResult::Perfect);
}

TEST(MatchScore_Compare, Correct)
{
	constexpr MatchScore aScore{ 3,1 };
	constexpr MatchScore bScore{ 3,2 };
	EXPECT_EQ(aScore.Compare(bScore), MatchScore::EComparingResult::Correct);
	EXPECT_EQ(bScore.Compare(aScore), MatchScore::EComparingResult::Correct);

	constexpr MatchScore cScore{ 1,3 };
	constexpr MatchScore dScore{ 2,3 };
	EXPECT_EQ(cScore.Compare(dScore), MatchScore::EComparingResult::Correct);
	EXPECT_EQ(dScore.Compare(cScore), MatchScore::EComparingResult::Correct);
}

TEST(MatchScore_Compare, Incorrect)
{
	constexpr MatchScore aScore{ 1,0 };
	constexpr MatchScore bScore{ 0,1 };
	EXPECT_EQ(aScore.Compare(bScore), MatchScore::EComparingResult::Incorrect);
	EXPECT_EQ(bScore.Compare(aScore), MatchScore::EComparingResult::Incorrect);

	constexpr MatchScore cScore{ 3,2 };
	constexpr MatchScore dScore{ 2,3};
	EXPECT_EQ(cScore.Compare(dScore), MatchScore::EComparingResult::Incorrect);
	EXPECT_EQ(dScore.Compare(cScore), MatchScore::EComparingResult::Incorrect);

	constexpr MatchScore eScore{ 3,2 };
	constexpr MatchScore fScore{ 0,3 };
	EXPECT_EQ(eScore.Compare(fScore), MatchScore::EComparingResult::Incorrect);
	EXPECT_EQ(fScore.Compare(eScore), MatchScore::EComparingResult::Incorrect);
}

TEST(MatchScore_Compare, Invalid)
{
	constexpr MatchScore aScore{ 1,0 };
	constexpr MatchScore bScore{ 2,0 };
	EXPECT_EQ(aScore.Compare(bScore), MatchScore::EComparingResult::Invalid);
	EXPECT_EQ(bScore.Compare(aScore), MatchScore::EComparingResult::Invalid);

	constexpr MatchScore cScore{ 0,2 };
	constexpr MatchScore dScore{ 2,3 };
	EXPECT_EQ(cScore.Compare(dScore), MatchScore::EComparingResult::Invalid);
	EXPECT_EQ(dScore.Compare(cScore), MatchScore::EComparingResult::Invalid);

	constexpr MatchScore eScore{ 3,2 };
	constexpr MatchScore fScore{ 1,2 };
	EXPECT_EQ(eScore.Compare(fScore), MatchScore::EComparingResult::Invalid);
	EXPECT_EQ(fScore.Compare(eScore), MatchScore::EComparingResult::Invalid);
}

TEST(MatchScore_GetTotalNumberOfGames, GetTotalNumberOfGames)
{
	constexpr MatchScore aScore{ 1,0 };
	EXPECT_EQ(aScore.GetTotalNumberOfGames(), 1);

	constexpr MatchScore bScore{ 1,3 };
	EXPECT_EQ(bScore.GetTotalNumberOfGames(), 4);

	constexpr MatchScore cScore{ 2,1 };
	EXPECT_EQ(cScore.GetTotalNumberOfGames(), 3);
}

TEST(MatchScore_GetWinningTeamScore, GetWinningTeamScore)
{
	constexpr MatchScore aScore{ 1,0 };
	EXPECT_EQ(aScore.GetWinningTeamScore(), 1);

	constexpr MatchScore bScore{ 2,3 };
	EXPECT_EQ(bScore.GetWinningTeamScore(), 3);

	constexpr MatchScore cScore{ 2,1 };
	EXPECT_EQ(cScore.GetWinningTeamScore(), 2);
}

TEST(MatchScore_GetLoosingTeamScore, GetLoosingTeamScore)
{
	constexpr MatchScore aScore{ 1,0 };
	EXPECT_EQ(aScore.GetLoosingTeamScore(), 0);

	constexpr MatchScore bScore{ 2,3 };
	EXPECT_EQ(bScore.GetLoosingTeamScore(), 2);

	constexpr MatchScore cScore{ 2,1 };
	EXPECT_EQ(cScore.GetLoosingTeamScore(), 1);
}

TEST(MatchScore_ToString, ToString)
{
	constexpr MatchScore aScore{ 1,0 };
	EXPECT_EQ(aScore.ToString(), "1 - 0");

	constexpr MatchScore bScore{ 2,3 };
	EXPECT_EQ(bScore.ToString(), "2 - 3");

	constexpr MatchScore cScore{ 2,1 };
	EXPECT_EQ(cScore.ToString(), "2 - 1");
}

TEST(MatchScore_Equals, Equals)
{
	constexpr MatchScore aScore{ 1,0 };
	constexpr MatchScore bScore{ 1,0 };
	EXPECT_EQ(aScore == bScore, true);

	constexpr MatchScore cScore{ 0,1 };
	constexpr MatchScore dScore{ 0,1 };
	EXPECT_EQ(cScore == dScore, true);

	constexpr MatchScore eScore{ 3,1 };
	constexpr MatchScore fScore{ 3,1 };
	EXPECT_EQ(eScore == fScore, true);

	constexpr MatchScore gScore{ 1,2 };
	constexpr MatchScore hScore{ 1,2 };
	EXPECT_EQ(gScore == hScore, true);
}

TEST(MatchScore_Equals, NonEquals)
{
	constexpr MatchScore aScore{ 3,1 };
	constexpr MatchScore bScore{ 3,2 };
	EXPECT_EQ(aScore == bScore, false);

	constexpr MatchScore cScore{ 1,3 };
	constexpr MatchScore dScore{ 2,3 };
	EXPECT_EQ(cScore == dScore, false);

	constexpr MatchScore eScore{ 1,0 };
	constexpr MatchScore fScore{ 0,1 };
	EXPECT_EQ(eScore == fScore, false);

	constexpr MatchScore gScore{ 3,2 };
	constexpr MatchScore hScore{ 2,3 };
	EXPECT_EQ(gScore == hScore, false);

	constexpr MatchScore iScore{ 3,2 };
	constexpr MatchScore jScore{ 0,3 };
	EXPECT_EQ(iScore == jScore, false);
}
