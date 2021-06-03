#pragma once

#include <sstream>
#include <vector>
#include <optional>
#include <string_view>

namespace misc
{
	// split string on spaces
	// maximum number of n splits, n <= 0 -> no limit
	void split(size_t n, std::string const& in, std::vector<std::string>& out, char c = ' ', bool emptySegments = false, bool keepSplitChars = false);
	std::vector<std::string> split(size_t n, std::string const& in, char c = ' ', bool emptySegments = false, bool keepSplitChars = false);

	std::string join(std::vector<std::string> const& in, std::optional<char> c = std::nullopt);

	using namespace std::literals;
	std::string_view trim(std::string_view in, std::string_view c = "\n"sv);
}