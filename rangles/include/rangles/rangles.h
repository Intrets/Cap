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

namespace ra
{
	enum class Signal
	{
		cont,
		end,
		error
	};

	template<class A>
	using Return = std::variant<A, Signal>;

	template<class R>
	struct held_value;

	template<class A>
	struct held_value<Return<A>>
	{
		using type = A;
	};

	template<class R>
	using held_value_t = typename held_value<R>::type;

	struct map_type {};
	struct filter_type {};
	struct fold_type {};
	struct producer_type {};

	template<class F>
	struct Producer
	{
		using type = producer_type;
		constexpr static bool stateful = true;

		constexpr auto operator()(auto&& x) {
			return 1;
		}
	};


	template<class F>
	struct Map
	{
		using type = map_type;
		constexpr static bool stateful = false;

		F f;

		constexpr auto operator()(auto&& x) {
			return this->f(std::forward<decltype(x)>(x));
		}
	};

	template<class F>
	Map(F const&)->Map<F>;

	template<class F>
	struct Filter
	{
		using type = filter_type;
		constexpr static bool stateful = false;

		F f;

		constexpr bool operator()(auto&& x) const {
			return this->f(std::forward<decltype(x)>(x));
		}
	};

	template<class F>
	Filter(F const&)->Filter<F>;




	template<class F, class Initial>
	struct Fold
	{
		using type = fold_type;
		using value_type = Initial;
		constexpr static bool stateful = true;

		F f;

		Initial initial;

		constexpr bool operator()(auto&& acc, auto&& e) const {
			return this->f(std::forward<decltype(acc)>(acc), std::forward<decltype(e)>(e));
		}
	};

	template<class F, class Initial>
	Fold(F const&, Initial&&)->Fold<F, Initial>;

	template<class A, class E>
	struct Run;

	template<class A, class E>
	requires std::same_as<typename E::type, map_type>
		struct Run<A, E>
	{
		constexpr static auto apply(A&& x, E const& e) {
			return e.f(std::forward<A>(x));
		};
	};

	template<class A, class E>
	requires std::same_as<typename E::type, filter_type>
		struct Run<A, E>
	{
		constexpr static bool apply(A&& x, E const& e) {
			return e.f(std::forward<A>(x));
		};
	};

	template<class A, class... Es>
	struct Whole;

	template<class A>
	struct Whole<A>
	{
		constexpr static Return<A> run(A&& x) {
			return x;
		}
	};

	template<class A, class E, class... Es>
	requires std::same_as<typename E::type, map_type>
		struct Whole<A, E, Es...>
	{
		// TODO: nested shenanigans, ie, E is a tuple of more Es
		//using t_type = typename E::type;

		using b = te::return_type_t_ptr<&Run<A, E>::apply>;

		using B = held_value_t<te::return_type_t_ptr<&Whole<b, Es...>::run>>;

		constexpr Return<B> run(A&& x, E const& e, Es const&... es) const {
			auto y = Run<A, E>::apply(std::forward<A>(x), e);
			using Y = decltype(y);
			return Whole<Y, Es...>().run(std::forward<Y>(y), es...);
		}
	};

	template<class A, class E, class... Es>
	requires std::same_as<typename E::type, filter_type>
		struct Whole<A, E, Es...>
	{
		// TODO: nested shenanigans, ie, E is a tuple of more Es
		//using t_type = typename E::type;

		using b = A;

		using B = held_value_t<te::return_type_t_ptr<&Whole<b, Es...>::run>>;

		constexpr static Return<B> run(A&& x, E const& e, Es const&... es) {
			if (Run<A, E>::apply(std::forward<A>(x), e)) {
				return Whole<A, Es...>().run(std::forward<A>(x), es...);
			}
			else {
				return Signal::cont;
			}
		}
	};

	template<class A, class E, class... Es>
	requires std::same_as<typename E::type, fold_type>
		struct Whole<A, E, Es...>
	{
		typename E::value_type acc;

		using b = A;

		using B = held_value_t<te::return_type_t_ptr<&Whole<b, Es...>::run>>;

		constexpr static Return<B> run(A&& x, E const& e, Es const&... es) {
			if (Run<A, E>::apply(std::forward<A>(x), e)) {
				return Whole<A, Es...>().run(std::forward<A>(x), es...);
			}
			else {
				return Signal::cont;
			}
		}
	};

	template<class A, class T>
	struct Whole2;

	template<class A, class... Es>
	struct Whole2<A, std::tuple<Es...>>
	{
		using type = Whole<A, Es...>;
	};

	template<class A, class T>
	using WholeT = typename Whole2<A, T>::type;
}

namespace ra2
{
	template<template<class...> class T, class List, class... Args>
	struct apply;

	template<template<class...> class T, class... Ls, class... Args>
	struct apply<T, te::list<Ls...>, Args...>
	{
		using type = T<Ls..., Args...>;
	};

	template<template<class...> class T, class List, class... Args>
	using apply_t = typename apply<T, List, Args...>::type;


	template<template<class List, class... Nexts> class T>
	struct wf {};

	template<class wrapped, class List, class... Nexts>
	struct apply_wf;

	template<template<class, class...> class unwrapped, class List, class... Nexts>
	struct apply_wf<wf<unwrapped>, List, Nexts...>
	{
		using type = unwrapped<List, Nexts...>;
	};

	template<class wrapped, class List, class... Nexts>
	using apply_wf_t = typename apply_wf<wrapped, List, Nexts...>::type;

	enum class Signal
	{
		value,
		end,
		cont
	};

	template<class T>
	struct Return
	{
		T data;

		Signal signal;
	};

	//template<class T>
	//using Return = std::variant<T, end, cont>;

	template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };

	template<class Lists, class Next, class... Nexts>
	struct Map
	{
		using F = typename Lists::head;
		using NextLists = typename Lists::tail;

		using ReturnType = int;

		F f;

		apply_wf_t<Next, NextLists, Nexts...> next;

		Return<ReturnType> run() {
			auto r = this->next.run();
			if (r.signal == Signal::value) {
				return { .data = this->f(r.data), .signal = Signal::value };
			}
			return r;
		}

		template<class FArg, class... Args>
		Map(FArg&& farg, Args&&... args) :
			f(farg),
			next(std::forward<Args>(args)...
			) {
		}
	};

	template<class Lists, class Next, class... Nexts>
	struct Id
	{
		apply_wf_t<Next, Lists, Nexts...> next;

		using ReturnType = int;

		Return<ReturnType> run() {
			return this->next.run();
		}
	};

	template<class Lists, class Next, class... Nexts>
	struct Filter
	{
		using F = typename Lists::head;
		using NextLists = typename Lists::tail;

		using ReturnType = int;

		F f;

		apply_wf_t<Next, NextLists, Nexts...> next;

		Return<ReturnType> run() {
			while (true) {
				auto r = this->next.run();
				if (r.signal == Signal::value) {
					if (this->f(r.data)) {
						return r;
					}
				}
				else {
					return r;
				}
			}
		}

		template<class FArg, class... Args>
		Filter(FArg&& farg, Args&&... args) :
			f(farg),
			next(std::forward<Args>(args)...
			) {
		}
	};

	template<class List>
	struct End
	{
		using IteratorPair = typename List::head;

		using BeginIt = typename IteratorPair::first_type;
		using EndIt = typename IteratorPair::second_type;

		BeginIt begin;
		EndIt end;

		Return<int> run() {
			if (this->begin == this->end) {
				return { .signal = Signal::end };
			}
			else {
				return { .data = *(begin++), .signal = Signal::value };
			}
		}


		template<class FBeginIt, class FEndIt>
		End(std::pair<FBeginIt, FEndIt>pair) :
			begin(pair.first),
			end(pair.second) {
		}
	};

	template<class F>
	struct map
	{
		using wrapped_type = wf<Map>;
		F f;

		map(F f_) : f(f_) {};
	};

	template<class F>
	struct filter
	{
		using wrapped_type = wf<Filter>;
		F f;

		filter(F f_) : f(f_) {};
	};

	template<class It>
	struct source
	{
		using wrapped_type = wf<End>;
		It begin;
		It end;

		source(It begin_, It end_) : begin(begin_), end(end_) {};
	};


	template<class Arg, class... Args>
	auto make_rangle(Arg&& arg, Args&&... args) {
		//Id<te::list<Arg::

	};

	void test() {
		//auto f = [](int i) { return i + 1; };
		//auto even = [](auto i) { return i % 2 == 0; };

		auto add3 = [](auto i) { return i + 3; };
		auto add1 = [](auto i) { return i + 1; };
		auto mult2 = [](auto i) { return i * 2; };
		auto multiple3 = [](auto i) { return i % 3 == 0; };

		//constexpr auto vie = transform([](auto x) { return x + 3; }) | transform(add1) | transform(mult2) | transform(add1) | transform(mult2) | transform(add1) | filter([](auto x) { return x % 3 == 0; }) | transform(toDouble);
		//Map<te::list<void, void>, wf<End>> wat;

		std::vector<int> ints;
		ints.resize(10000000);
		std::iota(ints.begin(), ints.end(), 1);

		auto sourcePair = std::make_pair(ints.begin(), ints.end());


		double weird = 0.0;
		double normal = 0.0;

		{

			using namespace std::ranges;

			//auto vi = ints | transform(f) | filter(even) | transform(f);
			auto vi = ints | views::transform(add3) | views::transform(add1) | views::transform(mult2) | views::transform(add1) | views::transform(mult2) | views::transform(add1) | views::filter(multiple3);

			auto start = std::chrono::system_clock::now();
			std::vector<int> result(vi.begin(), vi.end());

			std::chrono::duration<double> duration = std::chrono::system_clock::now() - start;
			normal += duration.count();
			//std::cout << std::format("stl ranges duration: {} length: {}\n", duration.count(), result.size());

		}
		{
			Filter<te::list<decltype(multiple3), decltype(add1), decltype(mult2), decltype(add1), decltype(mult2), decltype(add1), decltype(add3), decltype(sourcePair)>,
				wf<Map>, wf<Map>, wf<Map>, wf<Map>, wf<Map>, wf<Map>, wf<End>
			> wat2(multiple3, add1, mult2, add1, mult2, add1, add3, sourcePair);

			auto start = std::chrono::system_clock::now();
			std::vector<int> result;
			while (true) {
				auto r = wat2.run();
				if (r.signal == Signal::value) {
					result.push_back(r.data);
				}
				else {
					break;
				}
			}

			std::chrono::duration<double> duration = std::chrono::system_clock::now() - start;
			weird += duration.count();
			//std::cout << std::format("weird duration: {} length: {}\n", duration.count(), result.size());
		}

		std::cout << std::format("normal: {} weird: {}\n", normal, weird);



		rand();

	}

}











////template<class F, template <class...> class Next, class List, class... Nexts>
//template<
//	class Lists,
//	//template<class, template<class> class...> class Next,
//	template<class, template<class> class...> class... Nexts>
////template<class F, template<class...> class Rangle, class... Rangles>
////template<class F, template<class Current, class...> class Rangle, class... Rangles>
//struct Map
//{
//	using Next = typename te::list<Nexts...>::head;
//	using NextTail = typename te::list<Nexts...>::tail;

//	using List = typename Lists::head;
//	using NextLists = typename Lists::tail;

//	//typename Next<Lists, NextTail> next;
//	apply_t<Next, typename NextTail::prepend<Lists>> next;

//	//apply_t<Next
//	//apply_t<Next, List, Nexts...> next;

//	//F f;

//	std::optional<int> run() {
//	}

//};




