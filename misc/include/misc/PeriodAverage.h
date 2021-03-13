#pragma once

#include "CircularArray.h"

namespace misc
{
	template<class T>
	class PeriodAvarage
	{
	private:
		T zero();

	public:
		CircularArray<T> history;

		double currentTime;
		T current;
		int32_t measurements;

		double timePeriod;

		PeriodAvarage(int32_t historySize, double timePeriod_);
		PeriodAvarage() = default;
		~PeriodAvarage() = default;

		void insert(T val, double time);
		T getAvarege(int32_t timePeriods);

	};

	template<class T>
	inline T PeriodAvarage<T>::zero() {
		return static_cast<T>(0);
	}

	template<class T>
	inline PeriodAvarage<T>::PeriodAvarage(int32_t historySize, double timePeriod_) :
		history(historySize),
		timePeriod(timePeriod_),
		current(this->zero()),
		currentTime(0.0),
		measurements(0) {
	}

	template<class T>
	inline void PeriodAvarage<T>::insert(T val, double time) {
		if (time - this->currentTime >= this->timePeriod) {
			if (this->measurements != 0) {
				this->history.insert(this->current / static_cast<T>(this->measurements));
			}
			this->current = val;
			this->currentTime = time;
			this->measurements = 1;
		}
		else {
			this->current += val;
			this->measurements++;
		}
	}

	template<class T>
	inline T PeriodAvarage<T>::getAvarege(int32_t timePeriods) {
		return this->history.sumLast(timePeriods) / static_cast<T>(timePeriods);
	}
}
