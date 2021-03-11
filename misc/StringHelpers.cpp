#include "StringHelpers.h"

namespace misc
{
	void split(size_t n, std::string const& in, std::vector<std::string>& out, char c, bool emptySegments, bool keepSplitChars) {
		if (n <= 0) {
			n = in.size() + 1;
		}

		auto it = in.begin();

		while (it != in.end()) {
			auto prev = it;
			it = std::find(it, in.end(), c);

			if (it == in.end()) {
				out.emplace_back(prev, it);
				break;
			}

			if (prev != it || emptySegments) {
				if (keepSplitChars) {
					out.emplace_back(prev, it + 1);
				}
				else {
					out.emplace_back(prev, it);
				}

			}
			++it;

			if (out.size() == n) {
				out.emplace_back(it, in.end());
				return;
			}
		}
	}

	std::vector<std::string> split(size_t n, std::string const& in, char c, bool emptySegments, bool keepSplitChars) {
		std::vector<std::string> out;
		split(n, in, out, c, emptySegments, keepSplitChars);
		return out;
	}

	std::string join(std::vector<std::string> const& in, std::optional<char> c) {
		if (in.size() == 0) {
			return "";
		}
		std::stringstream out;
		for (int32_t i = 0; i < in.size() - 1; i++) {
			out << in[i];
			if (c.has_value()) {
				out << c.value();
			}
		}
		out << in.back();

		return out.str();
	}
}
