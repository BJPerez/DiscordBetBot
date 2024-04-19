#pragma once

#include <string>

class ICommand  // NOLINT(cppcoreguidelines-special-member-functions)
{
public:
	virtual ~ICommand() = default;

	bool Execute(std::string& outAnswerToUser) const;  // NOLINT(modernize-use-nodiscard)

protected:
	virtual void ExecuteInternal(std::string& outAnswerToUser) const = 0;
	[[nodiscard]] virtual bool ValidateCommand(std::string& outUserErrMsg) const = 0;
};

