#include <gtest/gtest.h>

#include "../src/BotData.h"
#include "../src/BotDataExceptions.h"
#include "../src/Match.h"

TEST(BotData_Tests, AddMatch_ValidParameters)
{
	BotData data;
	EXPECT_EQ(data.GetAllMatches().size(), 0);
	ASSERT_NO_THROW(data.AddMatch({"TestId", "TeamA", "TeamB", 3, std::string{ "31-01-2100 18:00" }}));
	EXPECT_EQ(data.GetAllMatches().size(), 1);
}

TEST(BotData_Tests, AddMatch_InvalidParameters)
{
	BotData data;
	data.AddMatch({"TestId", "TeamA", "TeamB", 3, std::string{ "31-01-2100 18:00" }});

	ASSERT_THROW(data.AddMatch({std::string{ Match::INVALID_ID }, "TeamA", "TeamB", 3, std::string{ "31-01-2100 18:00" }}), InvalidMatchIdException);
	ASSERT_THROW(data.AddMatch({std::string{ "" }, "TeamA", "TeamB", 3, std::string{ "31-01-2100 18:00" }}), InvalidMatchIdException);
	ASSERT_THROW(data.AddMatch({"TestId", "TeamA", "TeamB", 3, std::string{ "31-01-2100 18:00" }}), MatchIdUnavailableException);
	ASSERT_THROW(data.AddMatch({"TestId2", "", "TeamB", 3, std::string{ "31-01-2100 18:00" }}), InvalidTeamNameException);
	ASSERT_THROW(data.AddMatch({"TestId2", "TeamA", "", 3, std::string{ "31-01-2100 18:00" }}), InvalidTeamNameException);
	ASSERT_THROW(data.AddMatch({"TestId2", "TeamA", "TeamB", 0, std::string{ "31-01-2100 18:00" }}), InvalidBoSizeException);
	ASSERT_THROW(data.AddMatch({"TestId2", "TeamA", "TeamB", 2, std::string{ "31-01-2100 18:00" }}), InvalidBoSizeException);
	ASSERT_THROW(data.AddMatch({"TestId2", "TeamA", "TeamB", 4, std::string{ "31-01-2100 18:00" }}), InvalidBoSizeException);
	ASSERT_THROW(data.AddMatch({"TestId2", "TeamA", "TeamB", 3, std::string{ "Invalid" }}), InvalidDateFormat);
	ASSERT_THROW(data.AddMatch({"TestId2", "TeamA", "TeamB", 3, std::string{ "01-01-1990 18:00" }}), DateTimeInThePastException);
}

TEST(BotData_Tests, GetMatch_ValidParameters)
{
	BotData data;
	data.AddMatch({"TestId", "TeamA", "TeamB", 3, std::string{ "31-01-2100 18:00" }});

	auto dataConst = const_cast<const BotData* const>(&data); // to force a call on const getter version which is the only public version
	ASSERT_NO_THROW(const Match& match = dataConst->GetMatch("TestId"));
}

TEST(BotData_Tests, GetMatch_InvalidParameters)
{
	BotData data;
	data.AddMatch({"TestId", "TeamA", "TeamB", 3, std::string{ "31-01-2100 18:00" }});

	auto dataConst = const_cast<const BotData* const>(&data); // to force a call on const getter version which is the only public version
	ASSERT_THROW(const Match & match = dataConst->GetMatch(std::string {Match::INVALID_ID}), InvalidMatchIdException);
	ASSERT_THROW(const Match & match = dataConst->GetMatch(""), InvalidMatchIdException);
	ASSERT_THROW(const Match & match = dataConst->GetMatch("NonExistingId"), MatchNotFoundException);
}

TEST(BotData_Tests, AddResult_ValidParameters)
{
	BotData data;
	data.AddMatch({"TestId", "TeamA", "TeamB", 3, std::string{ "31-01-2100 18:00" }});
	data.AddMatch({"TestId2", "TeamA", "TeamB", 3, std::string{ "31-01-2100 18:00" }});

	auto dataConst = const_cast<const BotData* const>(&data); // to force a call on const getter version which is the only public version
	EXPECT_FALSE(dataConst->GetMatch("TestId").IsPlayed());
	ASSERT_NO_THROW(data.AddResult({"TestId", "TeamA", "TeamB", 2 , 0 }));
	EXPECT_TRUE(dataConst->GetMatch("TestId").IsPlayed());
	EXPECT_TRUE( (dataConst->GetMatch("TestId").GetResult() == MatchScore{2,0}) );

	EXPECT_FALSE(dataConst->GetMatch("TestId2").IsPlayed());
	ASSERT_NO_THROW(data.AddResult({ "TestId2", "TeamB", "TeamA", 2 , 0 }));
	EXPECT_TRUE(dataConst->GetMatch("TestId2").IsPlayed());
	EXPECT_TRUE( (dataConst->GetMatch("TestId2").GetResult() == MatchScore{ 0,2 }) );
}

TEST(BotData_Tests, AddResult_InvalidParameters)
{
	BotData data;
	data.AddMatch({"PlayedMatchId", "TeamA", "TeamB", 3, std::string{ "31-01-2100 18:00" }});
	data.AddMatch({"NonPlayedMatchId", "TeamA", "TeamB", 3, std::string{ "31-01-2100 18:00" }});
	data.AddResult({ "PlayedMatchId", "TeamA", "TeamB", 2, 0 });

	ASSERT_THROW(data.AddResult({ "NonExistingId", "TeamA", "TeamB", 2, 0}), MatchNotFoundException);
	ASSERT_THROW(data.AddResult({ "PlayedMatchId", "TeamA", "TeamB", 2, 0 }), MatchAlreadyPlayedException);
	ASSERT_THROW(data.AddResult({ "NonPlayedMatchId", "OtherName", "TeamB", 2, 0 }), UnmatchingTeamNameException);
	ASSERT_THROW(data.AddResult({ "NonPlayedMatchId", "TeamA", "OtherName", 2, 0 }), UnmatchingTeamNameException);
	ASSERT_THROW(data.AddResult({ "NonPlayedMatchId", "OtherName", "OtherName", 2, 0 }), UnmatchingTeamNameException);
}

TEST(BotData_Tests, AddBet_ValidParameters)
{
	BotData data;
	data.AddMatch({"TestId", "TeamA", "TeamB", 3, std::string{ "31-01-2100 18:00" }});

	EXPECT_EQ(data.GetBets().size(), 0);
	ASSERT_NO_THROW(data.AddBet("TestId", { 2,0 }, "Bettor"));
	EXPECT_EQ(data.GetBets().size(), 1);
}

TEST(BotData_Tests, AddBet_InvalidParameters)
{
	BotData data;
	data.AddMatch({"PlayedMatchId", "TeamA", "TeamB", 3, std::string{ "31-01-2100 18:00" }});
	data.AddMatch({"NonPlayedMatchId", "TeamA", "TeamB", 3, std::string{ "31-01-2100 18:00" }});
	data.AddResult({ "PlayedMatchId", "TeamA", "TeamB", 2, 0 });
	data.AddBet("NonPlayedMatchId", { 2,1 }, "BettorWithBet");

	ASSERT_THROW(data.AddBet("NonExistingMatchId", { 2,0 }, "Bettor"), MatchNotFoundException);
	ASSERT_THROW(data.AddBet("PlayedMatchId", { 2,0 }, "Bettor"), MatchAlreadyPlayedException);
	ASSERT_THROW(data.AddBet("NonPlayedMatchId", { 1,0 }, "Bettor"), InvalidScoreException);
	ASSERT_THROW(data.AddBet("NonPlayedMatchId", { 1,1 }, "Bettor"), InvalidScoreException);
	ASSERT_THROW(data.AddBet("NonPlayedMatchId", { 3,0 }, "Bettor"), InvalidScoreException);
	ASSERT_THROW(data.AddBet("NonPlayedMatchId", { 2,0 }, ""), InvalidBettorNameException);
	ASSERT_THROW(data.AddBet("NonPlayedMatchId", { 2,0 }, "BettorWithBet"), BetAlreadyExistException);
}

TEST(BotData_Tests, HasBet_ValidParameters)
{
	BotData data;
	data.AddMatch({"TestId", "TeamA", "TeamB", 3, std::string{ "31-01-2100 18:00" }});

	EXPECT_FALSE(data.HasBet("TestId", "Bettor"));
	data.AddBet("TestId", { 2,0 }, "Bettor");
	EXPECT_TRUE(data.HasBet("TestId", "Bettor"));
}

TEST(BotData_Tests, HasBet_InvalidParameters)
{
	BotData data;
	data.AddMatch({"TestId", "TeamA", "TeamB", 3, std::string{ "31-01-2100 18:00" }});

	ASSERT_THROW(bool res = data.HasBet("NonExistingId", "Bettor"), MatchNotFoundException);
	ASSERT_THROW(bool res = data.HasBet("TestId", ""), InvalidBettorNameException);
}

TEST(BotData_Tests, GetBet_ValidParameters)
{
	BotData data;
	data.AddMatch({"TestId", "TeamA", "TeamB", 3, std::string{ "31-01-2100 18:00" }});
	data.AddBet("TestId", { 2,1 }, "Bettor");

	auto dataConst = const_cast<const BotData* const>(&data); // to force a call on const getter version which is the only public version
	ASSERT_NO_THROW(const Bet& bet = dataConst->GetBet("TestId", "Bettor"));
}

TEST(BotData_Tests, GetBet_InvalidParameters)
{
	BotData data;
	data.AddMatch({"TestId", "TeamA", "TeamB", 3, std::string{ "31-01-2100 18:00" }});
	data.AddBet("TestId", { 2,1 }, "Bettor");

	auto dataConst = const_cast<const BotData* const>(&data); // to force a call on const getter version which is the only public version
	ASSERT_THROW(const Bet & bet = dataConst->GetBet(std::string{ Match::INVALID_ID }, "Bettor"), InvalidMatchIdException);
	ASSERT_THROW(const Bet & bet = dataConst->GetBet("", "Bettor"), InvalidMatchIdException);
	ASSERT_THROW(const Bet & bet = dataConst->GetBet("TestId", ""), InvalidBettorNameException);
	ASSERT_THROW(const Bet & bet = dataConst->GetBet("NonExistingMatch", "Bettor"), BetNotFoundException);
	ASSERT_THROW(const Bet & bet = dataConst->GetBet("TestId", "NonExistingBettor"), BetNotFoundException);
}

TEST(BotData_Tests, ModifyBet_ValidParameters)
{
	BotData data;
	data.AddMatch({"TestId", "TeamA", "TeamB", 3, std::string{ "31-01-2100 18:00" }});
	constexpr MatchScore initialScore{ 2,1 };
	data.AddBet("TestId", initialScore, "Bettor");

	constexpr MatchScore newScore{ 2,0 };
	auto dataConst = const_cast<const BotData* const>(&data); // to force a call on const getter version which is the only public version
	EXPECT_NE(dataConst->GetBet("TestId", "Bettor").GetScore(), newScore);
	ASSERT_NO_THROW(data.ModifyBet("TestId", newScore, "Bettor"));
	EXPECT_EQ(dataConst->GetBet("TestId", "Bettor").GetScore(), newScore);
}

TEST(BotData_Tests, ModifyBet_InvalidParameters)
{
	BotData data;

	data.AddMatch({"PlayedMatchId", "TeamA", "TeamB", 3, std::string{ "31-01-2100 18:00" }});
	data.AddBet("PlayedMatchId", { 2,1 }, "Bettor");
	data.AddResult({ "PlayedMatchId", "TeamA", "TeamB", 2, 0 });

	data.AddMatch({"NonPlayedMatchId", "TeamA", "TeamB", 3, std::string{ "31-01-2100 18:00" }});
	data.AddBet("NonPlayedMatchId", {2,1}, "Bettor");

	ASSERT_THROW(data.ModifyBet("NonExistingId", { 2,0 }, "Bettor"), MatchNotFoundException);
	ASSERT_THROW(data.ModifyBet("NonPlayedMatchId", { 2,0 }, "NonExistingBettor"), BetNotFoundException);
	ASSERT_THROW(data.ModifyBet("PlayedMatchId", { 2,0 }, "Bettor"), MatchAlreadyPlayedException);
	ASSERT_THROW(data.ModifyBet("NonPlayedMatchId", { 5,0 }, "Bettor"), InvalidScoreException);
}

TEST(BotData_Tests, GetBetsOnMatch_ValidParameters)
{
	BotData data;
	data.AddMatch({"TestId", "TeamA", "TeamB", 3, std::string{ "31-01-2100 18:00" }});

	EXPECT_EQ(data.GetBetsOnMatch("TestId").size(), 0);
	data.AddBet("TestId", { 2,0 }, "Bettor");
	EXPECT_EQ(data.GetBetsOnMatch("TestId").size(), 1);
	data.AddBet("TestId", { 2,0 }, "Bettor2");
	EXPECT_EQ(data.GetBetsOnMatch("TestId").size(), 2);
}

TEST(BotData_Tests, GetBetsOnMatch_InvalidParameters)
{
	BotData data;
	data.AddMatch({"TestId", "TeamA", "TeamB", 3, std::string{ "31-01-2100 18:00" }});

	ASSERT_THROW(auto matches = data.GetBetsOnMatch("NonExistingId"), MatchNotFoundException);
}

TEST(BotData_Tests, GetIncomingMatches)
{
	BotData data;
	data.AddMatch({"TestId", "TeamA", "TeamB", 3, std::string{ "31-01-2100 18:00" }});

	EXPECT_EQ(data.GetIncomingMatches().size(), 1);
	data.AddResult({ "TestId", "TeamA", "TeamB", 2, 0 });
	EXPECT_EQ(data.GetIncomingMatches().size(), 0);
}

TEST(BotData_Tests, GetPastMatches)
{
	BotData data;
	data.AddMatch({"TestId", "TeamA", "TeamB", 3, std::string{ "31-01-2100 18:00" }});

	EXPECT_EQ(data.GetPastMatches().size(), 0);
	data.AddResult({ "TestId", "TeamA", "TeamB", 2, 0 });
	EXPECT_EQ(data.GetPastMatches().size(), 1);
}

TEST(BotData_Tests, HasMatch_ValidParameters)
{
	BotData data;

	EXPECT_FALSE(data.HasMatch("TestId"));
	data.AddMatch({"TestId", "TeamA", "TeamB", 3, std::string{ "31-01-2100 18:00" }});
	EXPECT_TRUE(data.HasMatch("TestId"));
}

TEST(BotData_Tests, HasMatch_InvalidParameters)
{
	BotData data;
	data.AddMatch({"ExistingId", "TeamA", "TeamB", 3, std::string{ "31-01-2100 18:00" }});

	ASSERT_THROW(bool res = data.HasMatch(std::string{ Match::INVALID_ID }), InvalidMatchIdException);
	ASSERT_THROW(bool res = data.HasMatch(""), InvalidMatchIdException);
}
