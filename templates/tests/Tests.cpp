#include "tests/Tests.h"

#include "Everything.h"
#include <type_traits>

#define TRUE2(X, Y) static_assert(te::##X##_v<##Y##>)
#define FALSE2(X, Y) static_assert(!te::##X##_v<##Y##>)

#define TRUE(X, Y) static_assert(te::##X##_v<decltype(##Y##)>)
#define FALSE(X, Y) static_assert(!te::##X##_v<decltype(##Y##)>)

#define VERIFY_ARGS(X) static_assert(std::is_same_v<te::list<Arg1, Arg2>, te::arguments_list_t<decltype(##X##)>>);
#define VERIFY_RETURN(X) static_assert(std::is_same_v<ReturnType, te::return_type_t<decltype(##X##)>>);

struct ReturnType {};

struct Arg1 {};
struct Arg2 {};

struct MemberFunTest
{
	ReturnType fun(Arg1, Arg2) {
		return ReturnType{};
	};

	ReturnType operator() (Arg1, Arg2) {
		return ReturnType{};
	};
};

void static_tests() {
	ReturnType(*c_fun)(Arg1, Arg2) = [](Arg1, Arg2) {
		return ReturnType{};
	};

	auto simple_lambda = [](Arg1, Arg2) -> ReturnType {
		return ReturnType{};
	};

	int a = 0;
	auto capture_lambda = [&](Arg1, Arg2) -> ReturnType {
		a++;
		return ReturnType{};
	};

	std::function<ReturnType(Arg1, Arg2)> std_function = [](Arg1, Arg2) -> ReturnType {
		return ReturnType{};
	};

	TRUE(is_c_fun, c_fun);
	FALSE(is_std_fun, c_fun);
	FALSE(is_member_fun, c_fun);
	FALSE(is_lambda_fun, c_fun);

	FALSE(is_c_fun, simple_lambda);
	FALSE(is_std_fun, simple_lambda);
	FALSE(is_member_fun, simple_lambda);
	TRUE(is_lambda_fun, simple_lambda);

	FALSE(is_c_fun, capture_lambda);
	FALSE(is_std_fun, capture_lambda);
	FALSE(is_member_fun, capture_lambda);
	TRUE(is_lambda_fun, capture_lambda);

	FALSE(is_c_fun, std_function);
	TRUE(is_std_fun, std_function);
	FALSE(is_member_fun, std_function);
	FALSE(is_lambda_fun, std_function);

	FALSE(is_c_fun, &MemberFunTest::operator());
	FALSE(is_std_fun, &MemberFunTest::operator());
	TRUE(is_member_fun, &MemberFunTest::operator());
	FALSE(is_lambda_fun, &MemberFunTest::operator());

	FALSE2(is_c_fun, MemberFunTest);
	FALSE2(is_std_fun, MemberFunTest);
	FALSE2(is_member_fun, MemberFunTest);
	TRUE2(is_lambda_fun, MemberFunTest);

	VERIFY_ARGS(c_fun);
	VERIFY_ARGS(simple_lambda);
	VERIFY_ARGS(capture_lambda);
	VERIFY_ARGS(std_function);
	VERIFY_ARGS(&MemberFunTest::operator());

	VERIFY_RETURN(c_fun);
	VERIFY_RETURN(simple_lambda);
	VERIFY_RETURN(capture_lambda);
	VERIFY_RETURN(std_function);
	VERIFY_RETURN(&MemberFunTest::operator());

	static_assert(std::is_same_v<te::member_function_base_class_t<decltype(&MemberFunTest::operator())>, MemberFunTest>);
}