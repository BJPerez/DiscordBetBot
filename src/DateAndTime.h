#pragma once

#include <ctime>
#include <string>
#include <string_view>

class DateAndTime
{
public:
	DateAndTime() noexcept = default; // Should only be used by serialization
	explicit DateAndTime(const std::string& timeAsString);

	[[nodiscard]] std::string ToString() const noexcept;
	[[nodiscard]] bool IsInFuture() const noexcept;

private:
	static constexpr std::string_view DATE_TIME_FORMAT = "%Y-%m-%d %H:%M";

	std::tm m_Time;
};