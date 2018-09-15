#define METAMEASURE_SUPPRESS_CONVERSION_WARNINGS
#include <MetaMeasure.hpp>

#include <iostream>

int main()
{
  using namespace MetaMeasure;
  using namespace MetaMeasure::MetricLiterals;
  using namespace MetaMeasure::CustomaryLiterals;

  Inches<long double> measurement = 5.0_m;

  std::cout << measurement.value() << "\n";

  system("pause"); // I'm testing it on Windows, give me a break
}
