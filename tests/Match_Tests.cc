#include <gtest/gtest.h>

#include "../src/BotDataExceptions.h"
#include "../src/DateAndTime.h"
#include "../src/Match.h"
#include "../src/MatchScore.h"

TEST(Match_Tests, ToString)
{
	const Match matchA{ {}, "TeamA", "TeamB", 3, DateAndTime{ "2100-01-31 18:00" } };
	EXPECT_EQ(matchA.ToString(), "TeamA - TeamB");

	const Match matchB{ {}, "TeamB", "TeamA", 3, DateAndTime{ "2100-01-31 18:00" } };
	EXPECT_EQ(matchB.ToString(), "TeamB - TeamA");
}

TEST(Match_Tests, SetId_WithCustomId)
{
	Match match;
	match.SetId(std::string(Match::ID_PREFIX) + "2");

	// We don't test m_Id because it is a regular setter
	// We only want to test to value of s_NextId;
	EXPECT_EQ(match.s_NextId, 3);
}

TEST(Match_Tests, SetId_WithoutCustomId)
{
	const unsigned previousNextId = Match::s_NextId;

	Match match;
	match.SetId("f3aaecb4e1fdc600dea9c90c792cf26e"); // arbitrary value taken from samples

	// We don't test m_Id because it is a regular setter
	// We only want to test to value of s_NextId;
	EXPECT_EQ(match.s_NextId, previousNextId);
}

TEST(Match_Tests, SetResult_Invalid)
{
	Match matchA{ {}, "TeamA", "TeamB", 3, DateAndTime{ "2100-01-31 18:00" } };
	ASSERT_THROW(matchA.SetResult({ 1, 1 }) , InvalidScoreException);
	ASSERT_THROW(matchA.SetResult({ 1, 0 }), InvalidScoreException);
	ASSERT_THROW(matchA.SetResult({ 0, 1 }), InvalidScoreException);
	ASSERT_THROW(matchA.SetResult({ 2, 2 }), InvalidScoreException);
	ASSERT_THROW(matchA.SetResult({ 3, 1 }), InvalidScoreException);
	ASSERT_THROW(matchA.SetResult({ 0, 3 }), InvalidScoreException);
}

TEST(Match_Tests, SetResult_Valid)
{
	Match matchA{ {}, "TeamA", "TeamB", 3, DateAndTime{ "2100-01-31 18:00" } };
	ASSERT_NO_THROW(matchA.SetResult({ 2, 0 }));
	ASSERT_NO_THROW(matchA.SetResult({ 0, 2 }));
	ASSERT_NO_THROW(matchA.SetResult({ 2, 1 }));
	ASSERT_NO_THROW(matchA.SetResult({ 1, 2 }));
}

TEST(Match_Tests, IsValidScore_Invalid)
{
	const Match matchA{ {}, "TeamA", "TeamB", 3, DateAndTime{ "2100-01-31 18:00" } };
	EXPECT_FALSE(matchA.IsValidScore({ 1, 1 }));
	EXPECT_FALSE(matchA.IsValidScore({ 1, 0 }));
	EXPECT_FALSE(matchA.IsValidScore({ 0, 1 }));
	EXPECT_FALSE(matchA.IsValidScore({ 2, 2 }));
	EXPECT_FALSE(matchA.IsValidScore({ 3, 1 }));
	EXPECT_FALSE(matchA.IsValidScore({ 0, 3 }));
}

TEST(Match_Tests, IsValidScore_Valid)
{
	const Match matchA{ {}, "TeamA", "TeamB", 3, DateAndTime{ "2100-01-31 18:00" } };
	EXPECT_TRUE(matchA.IsValidScore({ 2, 0 }));
	EXPECT_TRUE(matchA.IsValidScore({ 0, 2 }));
	EXPECT_TRUE(matchA.IsValidScore({ 2, 1 }));
	EXPECT_TRUE(matchA.IsValidScore({ 1, 2 }));
}

TEST(Match_Tests, GetResult_WithoutResult)
{
	const Match matchA{ {}, "TeamA", "TeamB", 3, DateAndTime{ "2100-01-31 18:00" } };
	ASSERT_THROW(MatchScore score = matchA.GetResult(), MatchNotPlayedException);
}

TEST(Match_Tests, GetResult_WithResult)
{
	Match matchA{ {}, "TeamA", "TeamB", 3, DateAndTime{ "2100-01-31 18:00" } };
	constexpr MatchScore resultSet{ 2,0 };
	matchA.SetResult(resultSet);

	MatchScore resultGet;
	ASSERT_NO_THROW(resultGet = matchA.GetResult());
	EXPECT_EQ(resultGet, resultSet);
}

TEST(Match_Tests, GetNumberOfGamesToWin)
{
	const Match matchA{ {}, "TeamA", "TeamB", 3, DateAndTime{ "2100-01-31 18:00" } };
	EXPECT_EQ(matchA.GetNumberOfGamesToWin(), 2);

	const Match matchB{ {}, "TeamA", "TeamB", 1, DateAndTime{ "2100-01-31 18:00" } };
	EXPECT_EQ(matchB.GetNumberOfGamesToWin(), 1);

	const Match matchC{ {}, "TeamA", "TeamB", 5, DateAndTime{ "2100-01-31 18:00" } };
	EXPECT_EQ(matchC.GetNumberOfGamesToWin(), 3);

	const Match matchD{ {}, "TeamA", "TeamB", 7, DateAndTime{ "2100-01-31 18:00" } };
	EXPECT_EQ(matchD.GetNumberOfGamesToWin(), 4);
}

TEST(Match_Tests, IsPlayed)
{
	Match matchA{ {}, "TeamA", "TeamB", 3, DateAndTime{ "2100-01-31 18:00" } };
	EXPECT_EQ(matchA.IsPlayed(), false);

	matchA.SetResult({ 2,0 });
	EXPECT_EQ(matchA.IsPlayed(), true);
}

TEST(Match_Tests, OperatorEquals)
{
	// Only Id is important
	Match matchA{ "1", "TeamA", "TeamB", 3, DateAndTime{ "2100-01-31 18:00" } };
	Match matchB{ "1", "TeamC", "TeamD", 5, DateAndTime{ "2100-01-31 18:00" } };
	Match matchC{ "2", "TeamA", "TeamB", 3, DateAndTime{ "2100-01-31 18:00" } };

	EXPECT_EQ(matchA, matchB);
	EXPECT_NE(matchA, matchC);
}