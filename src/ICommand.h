#pragma once

#include <string>
#include <dpp/dpp.h>

class ICommand  // NOLINT(cppcoreguidelines-special-member-functions)
{
public:
	virtual ~ICommand() = default;

	[[nodiscard]] dpp::message Execute(const dpp::slashcommand_t& event) const;

protected:
	[[nodiscard]] virtual dpp::message ExecuteInternal(const dpp::slashcommand_t& event) const = 0;
	[[nodiscard]] virtual bool ValidateCommand(std::string& outUserErrMsg) const = 0;
};

