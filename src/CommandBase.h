#pragma once

#include <dpp/message.h>
#include <dpp/snowflake.h>

#include "BetBot.h"
#include "ICommandReceiver.h"
#include "LockableDataAccessors.h"

class CommandBase
{
public:
	CommandBase(const dpp::snowflake chanelId, BetBot& bot ) noexcept : m_AnswerChannelId(chanelId), m_Bot(&bot) {}

	CommandBase(const CommandBase& other) = default;
	CommandBase(CommandBase&& other) = default;
	virtual ~CommandBase() = default;
	CommandBase& operator=(const CommandBase& other) = default;
	CommandBase& operator=(CommandBase&& other) = default;

	[[nodiscard]] virtual dpp::message Execute() const = 0;

protected:
	[[nodiscard]] dpp::snowflake GetAnswerChannelId() const noexcept { return m_AnswerChannelId; }
	[[nodiscard]] DataReader<ICommandReceiver> GetDataReader() const { return m_Bot->GetDataReader(); }
	[[nodiscard]] DataWriter<ICommandReceiver> GetDataWriter() const { return m_Bot->GetDataWriter(); }

	class Helper
	{
	public:
		static std::vector<std::pair<std::string, std::string>> BuildAllScoreScoreSelectorOptions(const Match& match);
	};

private:
	dpp::snowflake m_AnswerChannelId;
	BetBot* m_Bot;
};

