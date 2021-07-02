#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <filesystem>
#include <chrono>

#include "MainLoop.h"
#include "Setup.h"

#include <sstream>

#include <tepp/tepp.h>
#include <tuple>

// TODO: keep runtime option, get value from config/command line argument
bool OPENGL_DEBUG = true;

GLFWwindow* window;

template<int I, class Tuple>
struct get_t
{
	using type = decltype(std::get<I>(std::declval<Tuple>));
};


struct hole_type {};
struct arg_type {};

template<class T>
concept is_hole_concept = requires (T t) {
	std::same_as<typename T::type, hole_type>;
};

struct uhole_
{

};

template<class T>
struct hole
{
	using type = hole_type;
	using value_type = T;

	hole(uhole_) {};
	hole() = default;
};

constexpr auto uhole = uhole_{};

template<class T>
struct arg
{
	using type = arg_type;
	using value_type = T;
};

//template<int I, class T>
//struct Info
//{
//	using type = T;
//	static constexpr size_t index = I;
//};
//

struct Apply
{
	//template<class F, class Info, class HoleArgs, class BoundArgs>
	//auto run(F f, HoleArgs&& holeArgs, BoundArgs&& boundArgs) {


	//}
};

struct Select
{
	template<class I, class Holes, class Args>
	static auto run(Holes&& holes, Args&& args) {
		if constexpr (std::same_as<I::type, uhole_>) {
			return std::get<I::index>(std::forward<Holes>(holes));
		}
		else {
			return std::get<I::index>(std::forward<Args>(args));
		}
	}
};


template<class F, class Info, class Holes, class Args>
struct Bind2;

template<class F, class Infos, class... Holes, class... Args>
struct Bind2<F, Infos, std::tuple<Holes...>, std::tuple<Args...>>
{
	template<class Info2, class Holes2, class Args2, class... Ordered>
	static auto order(Holes2&& holes, Args2&& args, Ordered&&... ordered) {
		if constexpr (Info2::is_empty) {
			return std::make_tuple(ordered...);
		}
		else {
			using I = typename Info2::head;
			using Next = typename Info2::tail;

			return order<Next, Holes2, Args2>(
				std::forward<Holes2>(holes),
				std::forward<Args2>(args),
				std::forward<Ordered>(ordered)...,
				Select::run<I>(std::forward<Holes2>(holes), std::forward<Args2>(args))
				);
		}
	}

	static auto bind(F const& f, std::tuple<Holes...>&& holes, std::tuple<Args...>&& args) {
		return[&, args = std::move(args)](typename Holes::value_type&&... hole) {
			auto holes2 = std::make_tuple(hole...);
			return std::apply(f,
				order<Infos>(
					holes2,
					args
					)
			);
		};
	}
};

template<class T>
struct is_hole
{
	static constexpr bool value = is_hole_concept<T>;
};

template<class T>
struct is_not_hole
{
	static constexpr bool value = !is_hole_concept<T>;
};


template<class T>
struct is_enumerated_hole
{
	static constexpr bool value = is_hole_concept<typename T::type>;
};

template<class T>
struct is_not_enumerated_hole
{
	static constexpr bool value = !is_hole_concept<typename T::type>;
};

template<int HleCount, int ArgCount, class List>
struct enumerate_holes_args;

template<int HoleCount, int ArgCount, class Arg>
struct enumerate_holes_args<HoleCount, ArgCount, te::list<Arg>>
{
	using type = std::conditional_t<
		std::same_as<Arg, uhole_>,
		te::list<te::enumeration<Arg, HoleCount>>,
		te::list<te::enumeration<Arg, ArgCount>>
	>;
};

template<int HoleCount, int ArgCount, class Arg, class... Args>
struct enumerate_holes_args<HoleCount, ArgCount, te::list<Arg, Args...>>
{
	static constexpr int next_HoleCount = HoleCount + (std::same_as<Arg, uhole_> ? 1 : 0);
	static constexpr int next_ArgCount = ArgCount + (std::same_as<Arg, uhole_> ? 0 : 1);

	using next = typename enumerate_holes_args<next_HoleCount, next_ArgCount, te::list<Args...>>::type;
	using type = std::conditional_t<
		std::same_as<Arg, uhole_>,
		typename next::template prepend_t<te::enumeration<Arg, HoleCount>>,
		typename next::template prepend_t<te::enumeration<Arg, ArgCount>>
	>;
};

template<class Arg, class MaybeHole>
struct fill_hole
{
	using type = std::conditional_t<
		std::same_as<MaybeHole, uhole_>,
		hole<Arg>,
		Arg>;
};

template<class Arg, class MaybeHole>
using fill_hole_t = fill_hole<Arg, MaybeHole>::type;

template<class F, class... Args>
struct Bind
{
	static auto bind(F f, Args&&... args) {
		using arguments = te::arguments_list_t<F>;
		using enumerated_list = typename enumerate_holes_args<0, 0, te::list<Args...>>::type;

		using filled_holes = te::to_tuple_t<te::zip_t<fill_hole_t, arguments, te::list<Args...>>>;

		[[maybe_unused]]
		enumerated_list aaaaaa2;

		[[maybe_unused]]
		filled_holes aaaaaa;

		//rand();

		//[[maybe_unused]]
		auto args_values = te::split_tuple<is_not_hole, filled_holes>::run(std::make_tuple(args...));
		auto holes_values = te::split_tuple<is_hole, filled_holes>::run(std::make_tuple(args...));


		rand();

		return Bind2<F, enumerated_list, decltype(holes_values), decltype(args_values)>::bind(f, std::move(holes_values), std::move(args_values));
		//return 1;

		//[[maybe_unused]]
		//oooo oo;


	}
};


template<class F, class... Args>
auto wat_bind(F f, Args&&... args) {
	return Bind<F, Args...>::bind(f, std::forward<Args>(args)...);
}


template<class T, int I>
using Info = te::enumeration<T, I>;

int main(int argc, char* argv[]) {
	//using ww = te::enumerate_t<te::list<int, hole<int>, int>>;

	//using saveme = enumerate_holes_args<0, 0, te::list<int, int, hole<int>, hole<int>>>::type;


	auto lambda = [](auto a, int b, int c, float d) {
		std::cout << a << b << c << d;
	};

	using namespace std::placeholders;
	auto test = std::bind(lambda, _1, _2, _3, _4);

	//[[maybe_unused]]
	//auto wat2 = Bind<decltype(lambda), int, uhole_, int>::bind(lambda, 1, uhole_(), 1);
	//auto wat2 = wat_bind(lambda, 1, uhole_(), 3, uhole_());

	//wat2(2, 4.0f);



	rand();


	//auto wat = Bind2<decltype(lambda),
	//	te::list<Info<hole<int>, 0>, Info<arg<int>, 0>, Info<hole<int>, 1>>,
	//	std::tuple<hole<int>, hole<int>>,
	//	std::tuple<int>>::bind(

	//		lambda,
	//		std::make_tuple(hole<int>(), hole<int>()),
	//		std::make_tuple(1));

	//[[maybe_unused]]
	//auto test = wat(1, 2);


	std::cout << "hello madne\n";
	rand();












	if (rand() > 0.00000001) {
		exit(0);
	}
	std::chrono::steady_clock::time_point startTime = std::chrono::steady_clock::now();
	std::filesystem::path resourcesPath;
	if (argc > 1) {
		resourcesPath = std::filesystem::path(argv[1]);
	}
	else {
		std::cout << "No custom path specified, walking up current dir and looking for a resources folder.\n";
		std::cout << "Can take a single nameless argument to set resources folder.\n";

		resourcesPath = std::filesystem::current_path();
		auto rootPath = resourcesPath.root_path();

		// Limit the search in case something goes wrong somehow
		constexpr size_t max_search = 100;
		size_t i = 0;
		for (; i < max_search; i++) {
			if (std::filesystem::exists(resourcesPath / "resources")) {
				resourcesPath /= "resources";
				std::cout << "Found Resources path.\n";
				break;
			}

			resourcesPath = resourcesPath.parent_path();
		}
		if (i == max_search) {
			std::cerr << "Fatal error: could not find Resources folder.";
			return 0;
		}
	}

	std::cout << "Resources path: " << std::filesystem::absolute(resourcesPath) << "\n";

	if (!std::filesystem::exists(resourcesPath)) {
		std::cerr << "Fatal error: resources path does not specify an existing folder.\n";
		return 0;
	}

	if (!initGLFW(window, OPENGL_DEBUG)) {
		std::cerr << "Fatal error: initGLFW failed.\n";
		glfwTerminate();
		return 0;
	}

	initManagers(std::filesystem::absolute(resourcesPath).string());

	mainLoop(window, startTime);

	glfwTerminate();
}
