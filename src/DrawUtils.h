#pragma once

#include <string>
#include <vector>
#include <dpp/message.h>

class Match;

namespace DrawUtils
{
	void DrawSeparativeLine(const std::vector<std::size_t>& columnSizes, std::string& outLine);
	void DrawTable(const std::vector<std::vector<std::string>>& columnsContent, std::string& outTable);
	[[nodiscard]] std::vector<std::size_t> GetColumnSizes(const std::vector<std::vector<std::string>>& columnsContent);
	[[nodiscard]] dpp::component CreateMatchSelector(const std::string& placeHolder, const std::string& selectorId, const std::vector<std::reference_wrapper<const Match>>& matches);
	[[nodiscard]] dpp::component CreateMatchResultSelector(const std::string& placeHolder, const std::string& selectorId, const Match& match);
}
