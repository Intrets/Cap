#include <iostream>

#include <ui/ui.h>
#include <mem/ReferenceManager.h>

class TestClass
{
public:
	int32_t num = 123;
	Handle selfHandle;

	TestClass(Handle h) : selfHandle(h) {
	};
};

int main() {
	ReferenceManager<TestClass> test;

	auto test2 = test.makeUniqueRef<TestClass>();

	std::cout << "hello app\n";

	std::cout << "ref test: " << test2.get()->num << '\n';

	testUI();

	int tt;

	std::cin >> tt;
}
