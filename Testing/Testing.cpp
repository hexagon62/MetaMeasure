#define METAMEASURE_SUPPRESS_CONVERSION_WARNINGS
#include <MetaMeasure.hpp>

#include <iostream>
#include <iomanip>

METAMEASURE_UNIT(Foo, MetaMeasure::Length, 2, 1);

int main()
{
  using namespace MetaMeasure::Literals;
  
  Foo<float> measurement = 5_m;
  MetaMeasure::Meters<float> measurement2 = measurement;
  
  std::cout << measurement.value() << " foo = " << measurement2.value() << " m\n";

  system("pause");
}
