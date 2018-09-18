#define METAMEASURE_SUPPRESS_CONVERSION_WARNINGS
#include <MetaMeasure.hpp>

#include <fstream>

template<typename T>
using Velocity = MetaMeasure::Measurement<T, MetaMeasure::UnitMeters<1>, MetaMeasure::UnitSeconds<-1>>;

template<typename T>
MetaMeasure::Meters<T> distanceTravelled(Velocity<T> vel, MetaMeasure::Seconds<T> time)
{
  return vel * time;
}

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
  out << "Testing multiplication of measurements with different dimensions:" << std::endl;
  out << "e = " << e.value() << " C; should be 20 C" << std::endl;
  out << std::endl;

  e += 1.0_daA * 1.0_s;
  out << "Testing addition of measurement with more than one unit:" << std::endl;
  out << "e = " << e.value() << " C; should be 30 C" << std::endl;
  out << std::endl;

  auto f = 5.0_m / 2.0_s;
  out << "Testing division of measurements with different dimensions:" << std::endl;
  out << "f = " << f.value() << " m/s; should be 2.5 m/s" << std::endl;
  out << std::endl;

  f -= 10.0_cm / 1.0_s;
  out << "Testing subtraction of measurement with more than one unit:" << std::endl;
  out << "f = " << f.value() << " m/s; should be 2.4 m/s" << std::endl;
  out << std::endl;

  Meters<long double> g = 10.0_m;
  Meters<long double, 3> h = g*g*g;
  out << "Testing multiplying a measurement by iself:" << std::endl;
  out << "h = " << g.value() << " m^3; should be 1000 m^3" << std::endl;
  out << std::endl;

  Seconds<long double, -1> freq = 1_kHz;
  auto i = 2*h*freq;
  out << "Testing hertz literal:" << std::endl;
  out << "i = " << i.value() << " m^3*Hz; should be 2000 m^3*Hz" << std::endl;
  out << std::endl;

  out << "Testing passing stuff to function:" << std::endl;
  out << "result = " << distanceTravelled<long double>(5_m/1_s, 10_s).value() << " m; should be 50 m" << std::endl;
  out << std::endl;

  auto wut = 5_m/5_m;
  out << wut.value() << std::endl;
  out << std::endl;
}
