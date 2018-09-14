#include <MetaMeasure.hpp>

#include <iostream>
#include <iomanip>

int main()
{
  using namespace MetaMeasure::Literals;
  
  MetaMeasure::Meters<float> measurement = 5.f;
  MetaMeasure::Inches<float> measurement2 = measurement;
  auto measurement3 = 42.0_m;
  
  std::cout << measurement.value() << " m = " << measurement2.value() << " in\n";
  std::cout << "Measurement from literal: " << measurement3.value() << " m\n";

  system("pause");
}
