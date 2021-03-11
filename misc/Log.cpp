#include "Log.h"

namespace misc
{
	void Log::putLine(std::string str) {
		std::lock_guard<std::mutex> lock(this->mtx);
		this->lines.push_back(str);
	}

	std::vector<std::string> Log::getLines() {
		std::lock_guard<std::mutex> lock(this->mtx);
		auto res = this->lines;
		this->lines.clear();
		return res;
	}

	void Log::putStreamLine(std::stringstream ss) {
		this->putLine(ss.str());
	}
}
