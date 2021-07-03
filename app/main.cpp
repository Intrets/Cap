#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <filesystem>
#include <chrono>

#include "MainLoop.h"
#include "Setup.h"

#include <sstream>

#include <tepp/tepp.h>
#include <tepp/bind.h>
#include <tuple>

// TODO: keep runtime option, get value from config/command line argument
bool OPENGL_DEBUG = true;

GLFWwindow* window;

struct function_struct
{
	std::string s = "a struct";
	int count = 0;
	void fun(int a, int b, int c, int d) {
		count++;
		std::cout << count << " " << "this is: " << s << " " << a << " " << b << " " << c << " " << d << "\n";
	};

	function_struct() = default;
	function_struct(function_struct const&) {
		std::cout << "copy constructed\n";
	}
	function_struct(function_struct&&) {
		std::cout << "move constructed\n";
	}

	function_struct& operator=(function_struct const&) {
		std::cout << "copy assigned\n";
		return *this;
	}
	function_struct& operator=(function_struct&&) {
		std::cout << "move assigned\n";
		return *this;
	}

	~function_struct() {
		std::cout << "destructed\n";
	}
};
using namespace std::placeholders;

int main(int argc, char* argv[]) {
	/*[[maybe_unused]]
	constexpr auto sum = [](auto a, auto b, auto c) {
		return a + b + c;
	};



	[[maybe_unused]]
	auto wat2 = te::bind(lambda, 1);
	auto whataboutnesting = te::bind(wat2, "nest");

	wat2(2, "three", 4.01f);
	whataboutnesting(11, 22);*/

	//auto ptr = &function_struct::fun;

	[[maybe_unused]]
	auto lambda = [](auto a, auto b, auto c, auto d) {
		std::cout << a << " " << b << " " << c << " " << d << "\n";
	};

	{
		auto member_test = [=] {
			function_struct ok;
			ok.s = "123";
			//return te::bind(lambda);
			return te::bind(&function_struct::fun, ok, te::hole());
			//return std::bind(&function_struct::fun, ok, _1, _2, _3, _4);
		}();

		std::cout << "exited\n";

		member_test(1, 2, 3, 4);
		member_test(1, 2, 3, 4);
		member_test(1, 2, 3, 4);

		auto copy = member_test;

		copy(2, 3, 4, 5);
	}

	{
		constexpr auto constexpr_lambda_test = [](auto a, auto b, auto c) {
			return a + b + c;
		};

		[[maybe_unused]]
		constexpr auto bind1 = te::bind(constexpr_lambda_test, 1, 2);
		[[maybe_unused]]
		constexpr auto bind2 = te::bind(bind1)(1);
		//constexpr auto bind3 = te::bind(bind2, 3);

		//[[maybe_unused]]
		//constexpr auto v = bind2();
		rand();

	}

	std::cout << "--------\n\n";

	{
		[[maybe_unused]]
		auto ll = [a = function_struct()](){ return 1; }();
		rand();
	}





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
