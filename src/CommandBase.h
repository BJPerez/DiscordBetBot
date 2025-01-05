#pragma once

#include "BetBot.h"
#include "ICommandReceiver.h"
#include "LockableDataAccessors.h"
#include "MessageBuilder.h"

class CommandBase
{
public:
	CommandBase(const MessageBuilder::ChannelId chanelId, BetBot& bot) noexcept : m_AnswerChannelId(chanelId), m_Bot(&bot) {}

	CommandBase(const CommandBase& other) = default;
	CommandBase(CommandBase&& other) = default;
	virtual ~CommandBase() = default;
	CommandBase& operator=(const CommandBase& other) = default;
	CommandBase& operator=(CommandBase&& other) = default;

	[[nodiscard]] virtual MessageBuilder::Message Execute() const = 0;

protected:
	[[nodiscard]] MessageBuilder::ChannelId GetAnswerChannelId() const noexcept { return m_AnswerChannelId; }
	[[nodiscard]] DataReader<ICommandReceiver> GetDataReader() const { return m_Bot->GetDataReader(); }
	[[nodiscard]] DataWriter<ICommandReceiver> GetDataWriter() const { return m_Bot->GetDataWriter(); }

	class Helper
	{
	public:
		static std::vector<MessageBuilder::SelectorOption> BuildScoreSelectorOptions(const Match& match);
		static std::vector<MessageBuilder::SelectorOption> BuildMatchSelectorOptions(const std::vector<std::reference_wrapper<const Match>>& matches);
	};

private:
	MessageBuilder::ChannelId m_AnswerChannelId;
	BetBot* m_Bot;
};

