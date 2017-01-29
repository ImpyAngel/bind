#include "bind.h"
#include <iostream>

struct S
{
	int operator()(int x1, int x2) {
		return x1 * x2 * x1;
	}
};
int main() {
	my_bind([](int x1, int x2){std::cout << x1 + x2;}, std::placeholders::_1,std::placeholders::_1)(1);
	// auto ex = my_bind([](int x1, int x2){return x1 + x2;}, std::placeholders::_1,std::placeholders::_1);
	// std::cout << ex(1) << '\n';
	std::cout << my_bind(S(), std::placeholders::_1, my_bind([](int x1, int x2){return x1 + x2;}, std::placeholders::_1,std::placeholders::_1))(1) << '\n';
}