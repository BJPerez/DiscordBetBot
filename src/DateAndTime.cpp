#include "DateAndTime.h"

#include <chrono>
#include <sstream>

#include "BotDataExceptions.h"

DateAndTime::DateAndTime(const std::string& timeAsString) 
{
    std::istringstream timeAsStream{ timeAsString };
    timeAsStream >> std::chrono::parse(std::string{ DATE_TIME_FORMAT }, m_Time);
    if (timeAsStream.fail())
    {
        throw InvalidDateFormat(timeAsString);
    }
}

std::string DateAndTime::ToString() const noexcept
{
	return std::vformat("{:" + std::string{ DATE_TIME_FORMAT } + '}', std::make_format_args(m_Time));
}

bool DateAndTime::IsInFuture() const noexcept
{
    const auto dateInSeconds = std::chrono::current_zone()->to_sys(m_Time);

    const auto now = std::chrono::system_clock::now();
    const auto nowAsSeconds = std::chrono::floor<std::chrono::seconds>(now);

    return dateInSeconds > nowAsSeconds;
}