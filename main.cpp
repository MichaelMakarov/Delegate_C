#include <iostream>
#include "Delegate.h"
#include <functional>

int Rep(const int& n) { return n; }

class Func
{
public:
	Func() {};
	int operator () (const int i) { return i * 10; }
	int Mult(const int i) const { return i * 100; }
	int Mult(const int i) { return i * 1000; }
	int Deci(const int i) const { return i * 10; }
	static int Sqr(const int i) { return i * i; }
};

int main()
{
	// тестирование с использованием йункции Rep и класса Func
	auto L = [](const char* srr) -> size_t { return 3; };
	Func func;
	std::hash<const char*> hasher;
	std::function<size_t(const char*)> fn = hasher;
	int (Func:: * fp)(const int) const;
	fp = &Func::Mult;

	std::cout << "...Delegate testing...\n";

	Delegate<size_t(const char*)> var1(L);
	std::cout << "Delegate for free function (lambda): " << var1("first") << std::endl;
	var1 = hasher;
	std::cout << "Delegate for class with overloaded operator(): " << var1("first") << std::endl;
	std::cout << "std::function for std::hash: " << fn("qwerty") << std::endl;
	var1 = fn;
	std::cout << "Delegate for std::function with std::hash: " << var1("qwerty") << std::endl;

	Delegate<int(const int)> var2(fp, &func);
	std::cout << "Delegate for const member function: " << var2(3) << std::endl;
	var2 = &Rep;
	std::cout << "Delegate for free function: " << var2(-8999) << std::endl;

	Delegate<int(const int)> var3(&Func::Deci, &func);
	std::cout << "Delegate for const member function: " << var3(55) << std::endl;
	auto var4 = var3;
	std::cout << "Delegate copied from previous: " << var4(55) << std::endl;

	Delegate<int(const int)> var5(&Func::Mult, &func);
	std::cout << "Delegate for member function: " << var5(-33) << std::endl;
	var5 = func;
	std::cout << "Delegate for class overloaded operator(): " << var5(-77) << std::endl;

	Delegate<size_t(const char*)> var6(hasher);
	std::cout << "Delegate for class with overloaded operator(): " << var6("fff") << std::endl;
	auto var7(var6);
	std::cout << "Delegate initialized from previous: " << var7("fff") << std::endl;

	var2 = var5;
	std::cout << "Delegate copied from another with the same type: " << var2(89) << std::endl;
	var2 = &Func::Sqr;
	std::cout << "Delegate for static function of the class: " << var2(111) << std::endl;

	return 0;
}