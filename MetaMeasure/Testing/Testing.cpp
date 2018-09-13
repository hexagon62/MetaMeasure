#include <MetaMeasure/Measurement.hpp>
#include <iostream>

int main()
{
  MetaMeasure::Meters<float> measurement = 2.f;
  
  std::cout << measurement.value() << '\n';
  system("pause");
}
