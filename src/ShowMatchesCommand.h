#pragma once

#include "ICommand.h"

class ICommandReceiver;

class ShowMatchesCommand final : public ICommand
{
public:
	explicit ShowMatchesCommand(ICommandReceiver& commandReceiver) : m_CommandReceiver(commandReceiver) {}

private:
	ICommandReceiver& m_CommandReceiver;

	dpp::message ExecuteInternal(const dpp::slashcommand_t& event) const final;
	[[nodiscard]] bool ValidateCommand(std::string& outUserErrMsg) const final;
};

