# MetaMeasure
A header only library for storing measurements and doing simple dimensional analysis with them at compile-time.

# Simple program that converts from meters to inches
```cpp
#include <MetaMeasure/Units.hpp>

#include <iostream>

int main()
{
  MetaMeasure::Meters<float> measurement = 5.f;
  MetaMeasure::Inches<float> measurement2 = measurement;
  
  std::cout << measurement.value() << " m = " << measurement2.value() << " in\n";
}
```

# Creating your own units
If you need to do this for some reason, it's easy. Just include Units.hpp!

```cpp
METAMEASURE_UNIT(Foo, Length, 2, 1) // For every 2 meters, there is 1 foo
```

You can also easily create literals for your unit, like so:
```cpp
METAMEASURE_LITERAL(Foo, 1, _foo)

// ...

 // Literal for MetaMeasure::Measurement<long double, UnitFoo<1>>
 // Redefine METAMEASURE_LITERAL_LD_NUMBER_TYPE if you want something other than long double
auto measurement = 12.34_foo;

// Literal for MetaMeaesure::Measurement<long long int, UnitFoo<1>>
// Redefine METAMEASURE_LITERAL_ULL_NUMBER_TYPE if you want something other than long long int
auto intMeasurement = 12_foo;
```
