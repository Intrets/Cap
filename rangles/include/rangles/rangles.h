#pragma once

#include <variant>
#include <cassert>
#include <tepp/tepp.h>

namespace ra
{
	enum class Signal
	{
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

	template<class F>
	struct Map
	{
		using type = map_type;

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
	struct Whole<A, E, Es...>
	{
		// TODO: nested shenanigans, ie, E is a tuple of more Es
		using t_type = typename E::type;

		using b = std::conditional_t<
			std::same_as<t_type, map_type>,
			te::return_type_t_ptr<&Run<A, E>::apply>,
			A
		>;

		using B = held_value_t<te::return_type_t_ptr<&Whole<b, Es...>::run>>;

		constexpr static Return<B> run(A&& x, E const& e, Es const&... es) {
			if constexpr (std::same_as<t_type, map_type>) {
				auto y = Run<A, E>::apply(std::forward<A>(x), e);
				using Y = decltype(y);
				return Whole<Y, Es...>::run(std::forward<Y>(y), es...);
			}
			else if constexpr (std::same_as<t_type, filter_type>) {
				if (Run<A, E>::apply(std::forward<A>(x), e)) {
					return Whole<A, Es...>::run(std::forward<A>(x), es...);
				}
				else {
					return Signal::error;
				}
			}
			else {
				assert(0);
				return Signal::error;
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















