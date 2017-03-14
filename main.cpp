#include "bind.h"
#include <iostream>
#include <typeinfo>
#include <cassert>

struct S
{
	int operator()(int x1, int x2) {
		return x1 * x2 * x1;
	}
};

int add(int& lhs, int rhs)
{
    lhs += rhs;
    return lhs;
}

int main() {
	my_bind([](int x1, int x2){std::cout << x1 + x2;}, std::placeholders::_1,std::placeholders::_1)(1);
	auto ex = my_bind([](int x1, int x2){return x1 + x2;}, std::placeholders::_1,std::placeholders::_1);
	std::cout << ex(1) << '\n';
	std::cout << my_bind(S(), std::placeholders::_1,
	my_bind([](int x1, int x2){
		std::cout << "hi\n" << x1 + x2; return x1 + x2;}, std::placeholders::_1,std::placeholders::_1))(1) << '\n';
	std::cout << "\n" << typeid(std::placeholders::_1).name() << "\n";
	int val = 1;
       auto my_bound = my_bind([](int x1, int x2){return x1 + x2;}, 0, 0);
       // std::cout << [](int x1, int x2){return x1 + x2;}(0, 0) << ' ' << val << "\n";
       // std:: cout <<  my_bound() << "\n";
       auto bound = my_bind(add, val, 1);
       std::cout << bound();
       std:: cout << val << ' ' << (val == 1) <<'\n';
       assert(val == 1);
   }