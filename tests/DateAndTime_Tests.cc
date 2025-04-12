#include <gtest/gtest.h>

#include "../src/BotDataExceptions.h"
#include "../src/DateAndTime.h"

TEST(DateAndTime_Tests, Constructor_Valid)
{
	ASSERT_NO_THROW(DateAndTime("1995-01-10 18:00")); 
}

TEST(DateAndTime_Tests, Constructor_Invalid)
{
	ASSERT_THROW(DateAndTime("1995-01-a 18:00"), InvalidDateFormat);
	ASSERT_THROW(DateAndTime("1995-01 18:00"), InvalidDateFormat);
	ASSERT_THROW(DateAndTime("1995/01/10 18:00"), InvalidDateFormat);
	ASSERT_THROW(DateAndTime("1995-01-10 18/00"), InvalidDateFormat);
}

TEST(DateAndTime_Tests, ToString)
{
	const DateAndTime test{ "1995-01-10 18:00" };
	EXPECT_EQ(test.ToString(), "1995-01-10 18:00");

	const DateAndTime test2{ "1995-01-10 18:00:00" };
	EXPECT_EQ(test2.ToString(), "1995-01-10 18:00");

	const DateAndTime test3{ "2028-01-01 18:00" };
	EXPECT_EQ(test3.ToString(), "2028-01-01 18:00");
}

TEST(DateAndTime_Tests, IsInFuture)
{
	const DateAndTime test{ "1995-01-10 18:00" };
	EXPECT_FALSE(test.IsInFuture());

	const DateAndTime test2{ "2100-01-10 18:00" };
	EXPECT_TRUE(test2.IsInFuture());
}