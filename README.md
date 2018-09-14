# MetaMeasure
A header only library for storing measurements and doing simple dimensional analysis with them at compile-time.

# To use in your project
In the MetaMeasure folder in this repo, you'll find two files, Measurements.hpp and Units.hpp.

Include those in your project and you'll be good to go.

# Simple program that converts from meters to inches
```cpp
#include <MetaMeasure/Units.hpp>

#include <iostream>

int main()
{
  MetaMeasure::Meters<float> measurement = 5.f;
  MetaMeasure::Inches<float> measurement2 = measurement;
  
  std::cout << measurement.value() << " m = " << measurement2.value() << " in\n";
}```
