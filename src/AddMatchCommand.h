#pragma once

#include <string>

#include "ICommand.h"

class ICommandReceiver;

class AddMatchCommand final : public ICommand
{
public:
	explicit AddMatchCommand(std::string teamAName, std::string teamBName, const unsigned int boSize, ICommandReceiver& commandReceiver):
		m_TeamAName(std::move(teamAName)), m_TeamBName(std::move(teamBName)), m_BoSize(boSize), m_CommandReceiver(commandReceiver) {}

private:
	std::string m_TeamAName;
	std::string m_TeamBName;
	unsigned int m_BoSize{ 0 };
	ICommandReceiver& m_CommandReceiver;

	dpp::message ExecuteInternal(const dpp::slashcommand_t& event) const final;
	[[nodiscard]] bool ValidateCommand(std::string& outUserErrMsg) const final;
};

