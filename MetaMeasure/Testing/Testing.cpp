#include <MetaMeasure/Measurement.hpp>
#include <iostream>

int main()
{
  auto measurement = MetaMeasure::Meters<float>(1.f);
  MetaMeasure::Measurement<float, MetaMeasure::UnitInches<>> measurement2;
  measurement2 = measurement;
  
  std::cout << measurement.value() << " m\n";
  std::cout << measurement2.value() << " in\n";
  system("pause");
}
