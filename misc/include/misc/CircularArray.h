#pragma once

#include <vector>

namespace misc
{
	template <class T>
	class CircularArray
	{
	private:
		T zero();

	public:
		int32_t index;
		int32_t size;
		//double* arr;
		std::vector<T> arr;
		T total;

		CircularArray(T, int32_t);
		CircularArray(int32_t size);

		void insert(T);
		T sum(T);
		T sum();
		T average(T initial);
		T average();
		T sumLast(int32_t steps);

		CircularArray() = delete;
	};

	template<class T>
	inline T CircularArray<T>::zero() {
		return static_cast<T>(0);
	}

	template<class T>
	CircularArray<T>::CircularArray(T value, int32_t s) {
		size = s;
		index = 0;
		total = s * value;
		arr.assign(s, value);
	}

	template<class T>
	inline CircularArray<T>::CircularArray(int32_t size) : CircularArray(this->zero(), size) {
	}

	template<class T>
	void CircularArray<T>::insert(T value) {
		index = (index + 1) % size;
		total -= arr[index];
		arr[index] = value;
		total += arr[index];
	}

	template<class T>
	inline T CircularArray<T>::sum(T) {
		return total;
	}

	template<class T>
	inline T CircularArray<T>::sum() {
		return total;
	}

	template<class T>
	inline T CircularArray<T>::average(T initial) {
		return sum() / static_cast<T>(this->size);
	}

	template<class T>
	inline T CircularArray<T>::average() {
		return sum() / static_cast<T>(this->size);
	}

	template<class T>
	inline T CircularArray<T>::sumLast(int32_t steps) {
		T sum = this->zero();
		for (int32_t i = 0; i < steps; i++) {
			int32_t j = (i + this->index) % this->size;
			sum += arr[j];
		}
		return sum;
	}
}
