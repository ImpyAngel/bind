#include "bind.h"
#include <iostream>

struct S
{
	int operator()(int x1, int x2) {
		return x1 * x2 * x1;
	}
};
int main() {
	bind([](int x1, int x2){std::cout << x1 + x2 << '\n';}, std::placeholders::_1,std::placeholders::_1)(1);

	std::cout << bind(S(), std::placeholders::_1, 3)(1) << '\n';
}