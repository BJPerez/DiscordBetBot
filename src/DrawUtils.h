#pragma once

#include <string>
#include <vector>

namespace DrawUtils
{
	void DrawSeparativeLine(const std::vector<std::size_t>& columnSizes, std::string& outLine);
	void DrawTable(const std::vector<std::vector<std::string>>& columnsContent, std::string& outTable);
	std::vector<std::size_t> GetColumnSizes(const std::vector<std::vector<std::string>>& columnsContent);
}
