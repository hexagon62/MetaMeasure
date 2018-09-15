#define METAMEASURE_SUPPRESS_CONVERSION_WARNINGS
#include <MetaMeasure.hpp>

#include <iostream>

int main()
{
  using namespace MetaMeasure;
  using namespace MetaMeasure::MetricLiterals;
  using namespace MetaMeasure::CustomaryLiterals;

  Meters<long double> x = 10.0_m;
  Inches<long double> y = 12.0_in;

  std::cout << "Testing creation of measurements via literals:" << std::endl;
  std::cout << "x = " << x.value() << " m; should be 10 m" << std::endl;
  std::cout << "y = " << y.value() << " in; should be 12 in" << std::endl;
  std::cout << std::endl;

  std::cout << "Testing multiplication of measurements:" << std::endl;
  std::cout << "x*2 = " << (x*2.l).value() << " m; should be 20 m" << std::endl;
  std::cout << "y*2 = " << (y*2.l).value() << " in; should be 24 in" << std::endl;
  std::cout << std::endl;

  std::cout << "Testing division of measurements:" << std::endl;
  std::cout << "x/2 = " << (x/2.l).value() << " m; should be 5 m" << std::endl;
  std::cout << "y/2 = " << (y/2.l).value() << " in; should be 6 in" << std::endl;
  std::cout << std::endl;

  auto a = x;
  auto b = y;
  std::cout << "Testing copying of measurements:" << std::endl;
  std::cout << "Copied x to a, and y to b." << std::endl;
  std::cout << "x = " << x.value() << " m; should be 10 m" << std::endl;
  std::cout << "y = " << y.value() << " in; should be 12 in" << std::endl;
  std::cout << "a = " << a.value() << " m; should be 10 m" << std::endl;
  std::cout << "b = " << b.value() << " in; should be 12 in" << std::endl;
  std::cout << std::endl;

  Inches<long double> c = x;
  Meters<long double> d = y;
  std::cout << "Testing conversions through copies:" << std::endl;
  std::cout << "Copied x to c, and y to d." << std::endl;
  std::cout << "x = " << x.value() << " m; should be 10 m" << std::endl;
  std::cout << "y = " << y.value() << " in; should be 12 in" << std::endl;
  std::cout << "c = " << c.value() << " in; should be 393.701 in" << std::endl;
  std::cout << "d = " << d.value() << " m; should be 0.3048 m" << std::endl;

  system("pause"); // I'm testing it on Windows, give me a break
}
