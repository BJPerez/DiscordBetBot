#pragma once

#include <string>
#include <dpp/message.h>
#include <dpp/snowflake.h>

class CommandBase
{
public:
	explicit CommandBase(const dpp::snowflake chanelId) : m_AnswerChannelId(chanelId) {}

	CommandBase(const CommandBase& other) = default;
	CommandBase(CommandBase&& other) = default;
	virtual ~CommandBase() = default;
	CommandBase& operator=(const CommandBase& other) = default;
	CommandBase& operator=(CommandBase&& other) = default;

	[[nodiscard]] dpp::message Execute() const;

protected:
	[[nodiscard]] dpp::snowflake GetAnswerChannelId() const { return m_AnswerChannelId; }

	[[nodiscard]] virtual dpp::message ExecuteInternal() const = 0;
	[[nodiscard]] virtual bool ValidateCommand(std::string& outUserErrMsg) const = 0;

private:
	dpp::snowflake m_AnswerChannelId;
};

