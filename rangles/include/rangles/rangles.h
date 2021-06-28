#pragma once

#include <iostream>
#include <variant>
#include <cassert>
#include <tepp/tepp.h>
#include <optional>
#include <utility>
#include <functional>
#include <numeric>
#include <ranges>
#include <chrono>
#include <string>
#include <vector>
#include <format>


namespace ra
{
	template<template<class... Nexts> class T>
	struct wf {};

	template<class wrapped, class... Nexts>
	struct apply_wf;

	template<template<class...> class unwrapped, class... Nexts>
	struct apply_wf<wf<unwrapped>, Nexts...>
	{
		using type = unwrapped<Nexts...>;
	};

	template<class wrapped, class... Nexts>
	using apply_wf_t = typename apply_wf<wrapped, Nexts...>::type;

	struct Signal_s
	{
		enum Enum : int
		{
			value = 1 << 0,
			end = 1 << 1,
			join = 1 << 2
		};

		int data = 0;

		Signal_s operator& (Signal_s other) const {
			return { other.data & this->data };
		}

		Signal_s operator| (Signal_s other) const {
			return { other.data | this->data };
		}

		Signal_s& operator |=(Signal_s other) {
			this->data |= other.data;
			return *this;
		}

		operator bool() {
			return data;
		}
	};

	namespace Signal
	{
		constexpr Signal_s value = Signal_s{ Signal_s::Enum::value };
		constexpr Signal_s end = Signal_s{ Signal_s::Enum::end };
		constexpr Signal_s join = Signal_s{ Signal_s::Enum::join };
	}

	template<class T>
	struct Return
	{
		using value_type = T;
		T data;

		Signal_s signal;
	};

	struct MapCount
	{
		int count = 0;
	};
	struct FilterCount
	{
		int count = 0;
	};

	template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };

	template<class Current, class Next, class... Nexts>
	struct Map
	{
		using NextType = apply_wf_t<typename Next::wrapped_type, Next, Nexts...>;
		using F = typename Current::function_type;

		using a = typename te::return_type_t_ptr<&NextType::run>::value_type;
		using ReturnType = decltype(std::declval<F>()(std::declval<a>()));

		F f;

		NextType next;

		Return<ReturnType> run() {
			auto r = this->next.run();
			if (r.signal & Signal::value) {
				return { .data = this->f(r.data), .signal = Signal::value };
			}
			return { .signal = Signal::end };
		}

		template<class Arg, class... Args>
		Map(Arg&& arg, Args&&... args) :
			f(arg.f),
			next(std::forward<Args>(args)...
			) {
		}
	};

	template<class Next, class... Nexts>
	struct Id
	{
		using NextType = apply_wf_t<typename Next::wrapped_type, Next, Nexts...>;
		using ReturnType = typename te::return_type_t_ptr<&NextType::run>::value_type;

		NextType next;

		Return<ReturnType> run() {
			return this->next.run();
		}

		template<class... Args>
		Id(Args&&... args) : next(std::forward<Args>(args)...) {}
	};

	template<class Current, class Next, class... Nexts>
	struct Filter
	{
		using NextType = apply_wf_t<typename Next::wrapped_type, Next, Nexts...>;
		using ReturnType = typename te::return_type_t_ptr<&NextType::run>::value_type;
		using F = typename Current::function_type;

		F f;

		NextType next;

		Return<ReturnType> run() {
			while (true) {
				auto r = this->next.run();
				if (r.signal & Signal::value) {
					if (this->f(r.data)) {
						return r;
					}
					else {
						continue;
					}
				}
				else {
					return r;
				}
			}
		}

		template<class Arg, class... Args>
		Filter(Arg&& arg, Args&&... args) :
			f(arg.f),
			next(std::forward<Args>(args)...
			) {
		}
	};

	template<class Current, class Next, class... Nexts>
	struct Length
	{
		using NextType = apply_wf_t<typename Next::wrapped_type, Next, Nexts...>;
		using ReturnType = int;

		int count = 0;

		NextType next;

		Return<ReturnType> run() {
			auto r = this->next.run();
			while (true) {
				if (r.signal & Signal::value) {
					this->count++;
				}
				else if (r.signal & (Signal::join | Signal::value)) {
					auto val = ++this->count;
					this->count = 0;
					return { .data = val };
				}
				else {
					return { .signal = r.signal };
				}
				r = this->next.run();
			}
		}

		template<class Arg, class... Args>
		Length(Arg&&, Args&&... args) :
			next(std::forward<Args>(args)...
			) {
		}
	};

	template<class Current, class Next, class... Nexts>
	struct SplitEvery
	{
		using NextType = apply_wf_t<typename Next::wrapped_type, Next, Nexts...>;
		using ReturnType = typename te::return_type_t_ptr<&NextType::run>::value_type;

		int every;
		int count = 0;

		NextType next;

		Return<ReturnType> run() {
			auto r = this->next.run();
			if (r.signal & Signal::value) {
				if (++count == every) {
					count = 0;
					r.signal |= Signal::join;
				}
				return r;
			}
			return r;
		}

		template<class Arg, class... Args>
		SplitEvery(Arg&& arg, Args&&... args) :
			every(arg.every),
			next(std::forward<Args>(args)...
			) {
		}
	};

	template<class Current, class Next, class... Nexts>
	struct Drop
	{
		using NextType = apply_wf_t<typename Next::wrapped_type, Next, Nexts...>;
		using ReturnType = typename te::return_type_t_ptr<NextType::run>::value_type;

		int count = 0;

		NextType next;

		Return<ReturnType> run() {
			while (this->count > 0) {
				this->next.run();
				count--;
			}
			return this->next.run();
		}

		template<class Arg, class... Args>
		Drop(Arg&& arg, Args&&... args) :
			count(arg.count),
			next(std::forward<Args>(args)...
			) {
		}
	};

	template<class Current>
	struct End
	{
		typename Current::iterator_type begin;
		typename Current::iterator_type end;

		using ReturnType = typename Current::iterator_type::value_type;

		Return<ReturnType> run() {
			if (this->begin == this->end) {
				return { .signal = Signal::end };
			}
			else {
				return { .data = *(this->begin++), .signal = Signal::value };
			}
		}

		template<class Arg>
		End(Arg&& arg) :
			begin(arg.begin),
			end(arg.end) {
		}
	};

	template<class F>
	struct map
	{
		using wrapped_type = wf<Map>;
		using function_type = F;
		F f;

		map(F f_) : f(f_) {};
	};

	template<class F>
	struct filter
	{
		using wrapped_type = wf<Filter>;
		using function_type = F;
		F f;

		filter(F f_) : f(f_) {};
	};

	struct drop
	{
		using wrapped_type = wf<Drop>;
		int count;

		drop(int count_) :count(count_) {};
	};

	template<class It>
	struct source
	{
		using wrapped_type = wf<End>;
		using iterator_type = It;
		It begin;
		It end;

		source(It begin_, It end_) : begin(begin_), end(end_) {};
	};

	struct split_every
	{
		using wrapped_type = wf<SplitEvery>;
		int every;

		split_every(int every_) :every(every_) {};
	};

	struct length
	{
		using wrapped_type = wf<Length>;

		length() {};
	};



	template<class... Args>
	auto make_rangle(Args&&... args) {
		return Id<Args...>(std::forward<Args>(args)...);
	};

	void test() {
		//auto f = [](int i) { return i + 1; };
		auto even = [](auto i) -> bool { return i % 2 == 0; };

		[[maybe_unused]]
		auto add3 = [](auto i) { return i + 3; };
		[[maybe_unused]]
		auto add1 = [](auto i) { return i + 1; };
		[[maybe_unused]]
		auto mult2 = [](auto i) { return i * 2; };
		[[maybe_unused]]
		auto multiple3 = [](auto i) -> bool { return i % 3 == 0; };

		//constexpr auto vie = transform([](auto x) { return x + 3; }) | transform(add1) | transform(mult2) | transform(add1) | transform(mult2) | transform(add1) | filter([](auto x) { return x % 3 == 0; }) | transform(toDouble);
		//Map<te::list<void, void>, wf<End>> wat;

		std::vector<int> ints;
		ints.resize(1000000000);
		//ints.resize(100);
		std::iota(ints.begin(), ints.end(), 1);
		//std::for_each(ints.begin(), ints.end(), [](int& i) { i = rand(); });

		//auto sourcePair = std::make_pair(ints.begin(), ints.end());


		double weird = 0.0;
		double normal = 0.0;

		auto rangle1 = make_rangle(length(), split_every(10), source(ints.begin(), ints.end()));

		std::vector<int> result0;
		while (true) {
			auto r = rangle1.run();
			if (r.signal & Signal::value) {
				result0.push_back(r.data);
			}
			else {
				break;
			}
		}

		rand();

		//std::vector<int> result2;
		for (size_t i = 0; i < 10; i++) {
			{
				auto rangle = make_rangle(filter(multiple3), map(add1), map(mult2), map(add1), map(mult2), map(add1), map(add3), filter(even), source(ints.begin(), ints.end()));

				//map(map(add1));

				std::vector<int> result1;
				auto start = std::chrono::system_clock::now();
				while (true) {
					auto r = rangle.run();
					if (r.signal & Signal::value) {
						result1.push_back(r.data);
					}
					else {
						break;
					}
				}

				std::chrono::duration<double> duration = std::chrono::system_clock::now() - start;
				weird += duration.count();
				std::cout << std::format("weird duration: {} length: {}\n", duration.count(), result1.size());
			}
			{

				using namespace std::ranges;

				//auto vi = ints | transform(f) | filter(even) | transform(f);
				auto vi = ints | views::filter(even) | views::transform(add3) | views::transform(add1) | views::transform(mult2) | views::transform(add1) | views::transform(mult2) | views::transform(add1) | views::filter(multiple3);
				//auto vi = ints | views::filter(multiple3);

				auto start = std::chrono::system_clock::now();
				std::vector<int> result2(vi.begin(), vi.end());

				std::chrono::duration<double> duration = std::chrono::system_clock::now() - start;
				normal += duration.count();
				std::cout << std::format("stl ranges duration: {} length: {}\n", duration.count(), result2.size());

			}
		}

		std::cout << std::format("normal: {} weird: {}\n", normal, weird);

		rand();

	}

}

