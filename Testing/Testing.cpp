#define METAMEASURE_SUPPRESS_CONVERSION_WARNINGS
#include <MetaMeasure.hpp>

#include <fstream>
#include <complex>

namespace MetaMeasure
{

namespace Tests
{

using namespace MetricLiterals;
using namespace CustomaryLiterals;

template<typename T>
using Velocity = Measurement<T, UnitMeters<1>, UnitSeconds<-1>>;

template<typename T>
Meters<T> distanceTravelled(Velocity<T> vel, Seconds<T> time)
{
  return vel * time;
}

void literalCreation(std::ostream& out)
{
  out << "Testing creation of measurements via literals:" << std::endl;

  // Make sure the literals return the correct dimensions
  Meters<long double> x = 10.0_m;
  Inches<long double> y = 12.0_in;

  out << "x = " << x.value() << " m; should be 10 m" << std::endl;
  out << "y = " << y.value() << " in; should be 12 in" << std::endl;
  out << std::endl;
}

void copying(std::ostream& out)
{
  out << "Testing copying of measurements:" << std::endl;

  auto x = 10.0_m;
  auto y = 12.0_in;
  auto a = x;
  auto b = y;

  out << "Copied x to a, and y to b." << std::endl;
  out << "x = " << x.value() << " m;  should be 10 m" << std::endl;
  out << "y = " << y.value() << " in; should be 12 in" << std::endl;
  out << "a = " << a.value() << " m;  should be 10 m" << std::endl;
  out << "b = " << b.value() << " in; should be 12 in" << std::endl;
  out << std::endl;
}

void copyConversion(std::ostream& out)
{
  out << "Testing conversions through copying:" << std::endl;

  auto x = 10.0_m;
  auto y = 12.0_in;
  Inches<long double> a = x;
  Meters<long double> b = y;

  out << "Copied x to a, and y to b." << std::endl;
  out << "x = " << x.value() << " m;  should be 10 m" << std::endl;
  out << "y = " << y.value() << " in; should be 12 in" << std::endl;
  out << "a = " << a.value() << " in; should be 393.701 in" << std::endl;
  out << "b = " << b.value() << " m;  should be 0.3048 m" << std::endl;
  out << std::endl;
}

void addition(std::ostream& out)
{
  out << "Testing addition of measurements:" << std::endl;

  auto x = 10.0_m;
  auto y = 12.0_in;
  auto res = x + y;

  out << "x = " << x.value() << " m; should be 10 m" << std::endl;
  out << "y = " << y.value() << " in; should be 12 in" << std::endl;
  out << "x + y = " << res.value() << " m; should be 10.3048 m" << std::endl;
  out << std::endl;
}

void subtraction(std::ostream& out)
{
  out << "Testing subtraction of measurements:" << std::endl;

  auto x = 10.0_m;
  auto y = 12.0_in;
  auto res = x - y;

  out << "x = " << x.value() << " m; should be 10 m" << std::endl;
  out << "y = " << y.value() << " in; should be 12 in" << std::endl;
  out << "x - y = " << res.value() << " m; should be 9.6952 m" << std::endl;
  out << std::endl;
}

void multiplication(std::ostream& out)
{
  out << "Testing multiplication of measurements by a plain number:" << std::endl;

  auto x = 10.0_m;
  auto y = 12.0_in;
  auto resX = x * 2.L;
  auto resY = y * 2.L;

  out << "x = " << x.value() << " m; should be 10 m" << std::endl;
  out << "y = " << y.value() << " in; should be 12 in" << std::endl;
  out << "2x = " << resX.value() << " m; should be 20 m" << std::endl;
  out << "2y = " << resY.value() << " in; should be 24 in" << std::endl;
  out << std::endl;
}

void division(std::ostream& out)
{
  out << "Testing division of measurements by a plain number:" << std::endl;

  auto x = 10.0_m;
  auto y = 12.0_in;
  auto resX = x / 2.L;
  auto resY = y / 2.L;

  out << "x = " << x.value() << " m; should be 10 m" << std::endl;
  out << "y = " << y.value() << " in; should be 12 in" << std::endl;
  out << "x/2 = " << resX.value() << " m; should be 5 m" << std::endl;
  out << "y/2 = " << resY.value() << " in; should be 6 in" << std::endl;
  out << std::endl;
}

void additionAssignment(std::ostream& out)
{
  out << "Testing addition assignment of measurements:" << std::endl;

  auto x = 10.0_m;
  auto y = 12.0_in;
  x += y;

  auto z = 10.0_m;
  auto w = 12.0_in;
  z = z + w;

  out << "x = " << x.value() << " m; should be 10.3048 m" << std::endl;
  out << "y = " << y.value() << " in; should be 12 in" << std::endl;
  out << "z = " << z.value() << " m; should be 10.3048 m" << std::endl;
  out << "w = " << w.value() << " in; should be 12 in" << std::endl;
  out << std::endl;
}

void subtractionAssignment(std::ostream& out)
{
  out << "Testing subtraction assignment of measurements:" << std::endl;

  auto x = 10.0_m;
  auto y = 12.0_in;
  x -= y;

  auto z = 10.0_m;
  auto w = 12.0_in;
  z = z - w;

  out << "x = " << x.value() << " m; should be 9.6952 m" << std::endl;
  out << "y = " << y.value() << " in; should be 12 in" << std::endl;
  out << "z = " << z.value() << " m; should be 9.6952 m" << std::endl;
  out << "w = " << w.value() << " in; should be 12 in" << std::endl;
  out << std::endl;
}

void multiplicationAssignment(std::ostream& out)
{
  out << "Testing multiplication assignment of measurements by a plain number:" << std::endl;

  auto x = 10.0_m;
  auto y = 12.0_in;
  x *= 2.L; y *= 2.L;

  auto z = 10.0_m;
  auto w = 12.0_in;
  z = z * 2.L;
  w = w * 2.L;

  auto a = 10.0_m;
  auto b = 12.0_in;
  a = 2.L * a;
  b = 2.L * b;

  out << "2x = " << x.value() << " m;  should be 20 m" << std::endl;
  out << "2y = " << y.value() << " in; should be 24 in" << std::endl;
  out << "2w = " << w.value() << " m;  should be 20 m" << std::endl;
  out << "2z = " << z.value() << " in; should be 24 in" << std::endl;
  out << "2a = " << a.value() << " m;  should be 20 m" << std::endl;
  out << "2b = " << b.value() << " in; should be 24 in" << std::endl;
  out << std::endl;
}

void divisionAssignment(std::ostream& out)
{
  out << "Testing division assignment of measurements by a plain number:" << std::endl;

  auto x = 10.0_m;
  auto y = 12.0_in;
  x /= 2.L; y /= 2.L;

  auto z = 10.0_m;
  auto w = 12.0_in;
  z = z / 2.L;
  w = w / 2.L;

  out << "x/2 = " << x.value() << " m;  should be 5 m" << std::endl;
  out << "y/2 = " << y.value() << " in; should be 6 in" << std::endl;
  out << std::endl;
}

void reciprocal(std::ostream& out)
{
  out << "Testing dividing plain number by measurement:" << std::endl;

  auto x = 10.0_m;
  auto y = 12.0_in;
  auto rx = 1.l / x;
  auto ry = 1.l / y;

  out << "x = " << x.value() << " m;  should be 10 m" << std::endl;
  out << "y = " << y.value() << " in; should be 12 in" << std::endl;
  out << "rx = " << rx.value() << " m^-1;  should be 0.1 m^-1" << std::endl;
  out << "ry = " << ry.value() << " in^-1; should be 0.0833333 in^-1" << std::endl;
  out << std::endl;
}

void sameDimensionMultiplication(std::ostream& out)
{
  out << "Testing multiplication of measurements with the same dimensions:" << std::endl;

  auto a = 10.0_m;
  auto b = 10.0_mm;
  auto c = a*b;

  out << "a = " << a.value() << " m; should be 10 m" << std::endl;
  out << "b = " << b.value() << " mm; should be 10 mm" << std::endl;
  out << "a*b = " << c.value() << " m^2; should be 0.1 m^2" << std::endl;
  out << std::endl;
}

void sameDimensionDivision(std::ostream& out)
{
  out << "Testing division of measurements with the same dimensions:" << std::endl;

  auto a = 10.0_m*10.0_m;
  auto b = 1.0_km;
  auto c = a / b;

  out << "a = " << a.value() << " m^2; should be 100 m^2" << std::endl;
  out << "b = " << b.value() << " km; should be 1 km" << std::endl;
  out << "a/b = " << c.value() << " m; should be 0.1 m" << std::endl;
  out << std::endl;
}

void differentDimensionMultiplication(std::ostream& out)
{
  out << "Testing multiplication of measurements with different dimensions:" << std::endl;

  auto current = 10.0_A;
  auto time = 2.0_s;
  auto charge = current*time;

  out << "current = " << current.value() << " A; should be 10 A" << std::endl;
  out << "time = " << time.value() << " s; should be 2 s" << std::endl;
  out << "charge = " << charge.value() << " C; should be 20 C" << std::endl;
  out << std::endl;
}

void differentDimensionDivision(std::ostream& out)
{
  out << "Testing multiplication of measurements with different dimensions:" << std::endl;

  auto distance = 10.0_m;
  auto time = 2.0_s;
  auto velocity = distance/time;

  out << "distance = " << distance.value() << " m; should be 10 m" << std::endl;
  out << "time = " << time.value() << " s; should be 2 s" << std::endl;
  out << "velocity = " << velocity.value() << " m/s; should be 5 m/s" << std::endl;
  out << std::endl;
}

void useFunction(std::ostream& out)
{
  out << "Testing passing parameters to function and getting result back:" << std::endl;

  auto vel = 10.0_m/1.0_s;
  auto time = 5.0_s;
  auto res = distanceTravelled(vel, time);

  out << "vel = " << vel.value() << " m/s; should be 10 m/s" << std::endl;
  out << "time = " << time.value() << " s; should be 5 s" << std::endl;
  out << "res = " << res.value() << " m; should be 50 m" << std::endl;
}

}

}

int main()
{
  std::ofstream out("metameasure_test.txt");

  void(*tests[])(std::ostream&) = {
    MetaMeasure::Tests::literalCreation,
    MetaMeasure::Tests::copying,
    MetaMeasure::Tests::copyConversion,
    MetaMeasure::Tests::addition,
    MetaMeasure::Tests::subtraction,
    MetaMeasure::Tests::multiplication,
    MetaMeasure::Tests::division,
    MetaMeasure::Tests::additionAssignment,
    MetaMeasure::Tests::subtractionAssignment,
    MetaMeasure::Tests::multiplicationAssignment,
    MetaMeasure::Tests::divisionAssignment,
    MetaMeasure::Tests::reciprocal,
    MetaMeasure::Tests::sameDimensionMultiplication,
    MetaMeasure::Tests::sameDimensionDivision,
    MetaMeasure::Tests::differentDimensionMultiplication,
    MetaMeasure::Tests::differentDimensionDivision,
    MetaMeasure::Tests::useFunction
  };

  for (auto& i : tests) i(out);
}
