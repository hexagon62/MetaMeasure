#define METAMEASURE_SUPPRESS_CONVERSION_WARNINGS
#include <MetaMeasure.hpp>

#include <fstream>

int main()
{
  using namespace MetaMeasure;
  using namespace MetaMeasure::MetricLiterals;
  using namespace MetaMeasure::CustomaryLiterals;

  std::ofstream out("metameasure_test.txt");

  Meters<long double> x = 10.0_m;
  Inches<long double> y = 12.0_in;

  out << "Testing creation of measurements via literals:" << std::endl;
  out << "x = " << x.value() << " m; should be 10 m" << std::endl;
  out << "y = " << y.value() << " in; should be 12 in" << std::endl;
  out << std::endl;

  out << "Testing multiplication of measurements by a factor:" << std::endl;
  out << "x*2 = " << (x * 2.L).value() << " m; should be 20 m" << std::endl;
  out << "y*2 = " << (y * 2.L).value() << " in; should be 24 in" << std::endl;
  out << std::endl;

  out << "Testing division of measurements by a factor:" << std::endl;
  out << "x/2 = " << (x / 2.L).value() << " m; should be 5 m" << std::endl;
  out << "y/2 = " << (y / 2.L).value() << " in; should be 6 in" << std::endl;
  out << std::endl;

  auto a = x;
  auto b = y;
  out << "Testing copying of measurements:" << std::endl;
  out << "Copied x to a, and y to b." << std::endl;
  out << "x = " << x.value() << " m; should be 10 m" << std::endl;
  out << "y = " << y.value() << " in; should be 12 in" << std::endl;
  out << "a = " << a.value() << " m; should be 10 m" << std::endl;
  out << "b = " << b.value() << " in; should be 12 in" << std::endl;
  out << std::endl;

  Inches<long double> c = x;
  Meters<long double> d = y;
  out << "Testing conversions through copying:" << std::endl;
  out << "Copied x to c, and y to d." << std::endl;
  out << "x = " << x.value() << " m; should be 10 m" << std::endl;
  out << "y = " << y.value() << " in; should be 12 in" << std::endl;
  out << "c = " << c.value() << " in; should be 393.701 in" << std::endl;
  out << "d = " << d.value() << " m; should be 0.3048 m" << std::endl;

  out << "Testing division of measurements by a factor:" << std::endl;
  out << "x/2 = " << (x / 2.L).value() << " m; should be 5 m" << std::endl;
  out << "y/2 = " << (y / 2.L).value() << " in; should be 6 in" << std::endl;
  out << std::endl;

  auto e = 10.0_A * 2.0_s;
  out << "Testing division of measurements with different dimensions:" << std::endl;
  out << "e = " << e.value() << " C; should be 20 C" << std::endl;

  e += 1.0_daA * 1.0_s;
  out << "Testing addition of measurement with more than one unit:" << std::endl;
  out << "e = " << e.value() << " C; should be 30 C" << std::endl;
}
