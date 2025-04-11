#include "DateAndTime.h"

#include <chrono>
#include <sstream>

#include "BotDataExceptions.h"

DateAndTime::DateAndTime(const std::string& timeAsString) 
{
    m_Time = {}; 
    m_Time.tm_isdst = -1;

    std::istringstream timeAsStream{ timeAsString };
    timeAsStream >> std::get_time(&m_Time, std::string{ DATE_TIME_FORMAT }.c_str());
    if (timeAsStream.fail())
    {
        throw InvalidDateFormat(timeAsString);
    }
}

std::string DateAndTime::ToString() const noexcept
{
    std::stringstream resultAsStream;
    resultAsStream << std::put_time(&m_Time, std::string{ DATE_TIME_FORMAT }.c_str());

    return resultAsStream.str();
}

bool DateAndTime::IsInFuture() const noexcept
{
    const std::time_t dateInSeconds = std::mktime(const_cast<std::tm*>(&m_Time));

    const std::chrono::time_point now = std::chrono::system_clock::now();
    const std::time_t nowAsSeconds = std::chrono::system_clock::to_time_t(now);

    return dateInSeconds > nowAsSeconds;
}