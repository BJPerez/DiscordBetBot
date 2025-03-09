#include <gtest/gtest.h>

#include "../src/BotDataExceptions.h"
#include "../src/DateAndTime.h"

TEST(DateAndTime_Tests, Constructor_Valid)
{
	ASSERT_NO_THROW(DateAndTime("10-01-1995 18:00")); 
}

TEST(DateAndTime_Tests, Constructor_Invalid)
{
	ASSERT_THROW(DateAndTime("a-01-1995 18:00"), InvalidDateFormat);
	ASSERT_THROW(DateAndTime("01-1995 18:00"), InvalidDateFormat);
	ASSERT_THROW(DateAndTime("10/01/1995 18:00"), InvalidDateFormat);
	ASSERT_THROW(DateAndTime("10-01-1995 18/00"), InvalidDateFormat);
}

TEST(DateAndTime_Tests, ToString)
{
	const DateAndTime test{ "10-01-1995 18:00" };
	EXPECT_EQ(test.ToString(), "10-01-1995 18:00");

	const DateAndTime test2{ "10-01-1995 18:00:00" };
	EXPECT_EQ(test2.ToString(), "10-01-1995 18:00");

	const DateAndTime test3{ "01-01-2028 18:00" };
	EXPECT_EQ(test3.ToString(), "01-01-2028 18:00");
}

TEST(DateAndTime_Tests, IsInFuture)
{
	const DateAndTime test{ "10-01-1995 18:00" };
	EXPECT_FALSE(test.IsInFuture());

	const DateAndTime test2{ "10-01-2100 18:00" };
	EXPECT_TRUE(test2.IsInFuture());
}