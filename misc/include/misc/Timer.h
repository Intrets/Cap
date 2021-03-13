#pragma	once

#include <mutex>

#include "PeriodAverage.h"
#include "Option.h"

namespace misc
{
	struct Timing
	{
		PeriodAvarage<double> history;
		double timing;
		std::optional<double> maybeStart;

		Timing() : history(5, 0.5), timing(0.0), maybeStart(std::nullopt) {
		};
	};


	class Timer
	{
	public:
		std::mutex mtx;

		std::unordered_map<std::string, Timing> timings;

		void startTiming(std::string timing);
		void endTiming(std::string timing);
		void newTiming(std::string timing);

		std::vector<std::string> print();
	};
}
