#define METAMEASURE_SUPPRESS_CONVERSION_WARNINGS
#include <MetaMeasure.hpp>

#include <iostream>

METAMEASURE_UNIT(HalfSeconds, MetaMeasure::Time, 1, 2);

int main()
{
  using namespace MetaMeasure;

  Measurement<float, UnitMeters<1>, UnitSeconds<-1>> measurement = 1.f;
  Measurement<float, UnitHalfSeconds<-1>, UnitInches<1>> measurement2 = measurement;
  
  std::cout << measurement2.value() << "\n";

}
