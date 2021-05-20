#pragma once

#include <type_traits>
#include <functional>

namespace te
{
	struct ctype_base
	{
		static inline size_t t = 1;
	};

	template<class T>
	struct ctype : ctype_base
	{
		static inline size_t val = t++;
	};

	struct Void;

	template<class... Ts>
	struct List
	{
		static constexpr bool is_empty = true;
		using head = Void;
		using tail = Void;
	};

	template<class E, class F>
	struct prepend;

	template<class E>
	struct prepend<E, Void>
	{
		using val = List<E>;
	};

	template<class E, class... Args>
	struct prepend<E, List<Args...>>
	{
		using val = List<E, Args...>;
	};

	template<class E, class F>
	using prepend_t = typename prepend<E, F>::val;

	template<class Head, class... Tail>
	struct List<Head, Tail...>
	{
		static constexpr bool is_empty = false;
		static constexpr int size = 1 + sizeof...(Tail);
		using head = Head;
		using tail = typename List<Tail...>;
	};

	template<int I, class L, class R>
	struct reverse2;

	template<class L, class R>
	struct reverse2<0, L, R>
	{
		using val = R;
	};

	template<int I, class L, class R>
	struct reverse2
	{
		using val = typename reverse2<
			I - 1,
			typename L::tail,
			typename prepend_t<typename L::head, R>
		>::val;
	};

	template<class T>
	using reverse_t = typename reverse2<T::size, T, Void>::val;

	template<class T, class L>
	struct Contains
	{
		constexpr static bool val() {
			if constexpr (L::is_empty) {
				return false;
			}
			else if constexpr (std::is_same_v<T, L::head>) {
				return true;
			}
			else {
				return Contains<T, L::tail>::val();
			}
		}
	};

	template<class T>
	struct unwrap_fun;

	template<class R, class B, class... Args>
	struct unwrap_fun<R(B::*)(Args...) const>
	{
		using std_function_type = std::function<R(Args...)>;
	};

	template<class T>
	struct is_std_fun;

	template<class R, class... Args>
	struct is_std_fun<std::function<R(Args...)>>
	{
		static constexpr bool val = true;
	};

	template<class T>
	struct is_std_fun
	{
		static constexpr bool val = false;
	};

	template<class T>
	struct unwrap_std_fun;

	template<class R, class... Args>
	struct unwrap_std_fun<std::function<R(Args...)>>
	{
		using return_type = R;
		using args = List<Args...>;
	};

	template<class T>
	auto wrap_in_std_fun(T f) {
		using t = typename unwrap_fun<decltype(&T::operator())>::std_function_type;
		if constexpr (is_std_fun<T>::val) {
			return f;
		}
		else {
			return t(f);
		}
	}

	struct Loop
	{
		template<class E, class F>
		static inline void run(E& e, F f) {
			using A = typename te::reverse_t<te::unwrap_std_fun<decltype(f)>::args>;
			Loop::run<E, decltype(f), A>(e, f);
		}

		template<class E, class F, class L, class... Args>
		static inline void run(E& e, F f, Args... args) {
			if constexpr (L::is_empty) {
				f(args...);
			}
			else {
				using head_stripped_ref = std::remove_reference_t<L::head>;
				head_stripped_ref::run<F, typename L::tail, Args...>(e, f, args...);
			}
		}
	};
}
