# MetaMeasure
A work-in-progress header only library for storing measurements and doing simple dimensional analysis with them at compile-time.

# Simple program that converts from meters to inches
```cpp
#include <MetaMeasure.hpp>

#include <iostream>

int main()
{
  MetaMeasure::Meters<float> measurement = 5.f;
  MetaMeasure::Inches<float> measurement2 = measurement;
  
  std::cout << measurement.value() << " m = " << measurement2.value() << " in\n";
}
```
# Simple example of dimensional analysis
```cpp
// Just typedef away the ugly stuff
template<typename T>
using Velocity = MetaMeasure::Measurement<T, MetaMeasure::UnitMeters<1>, MetaMeasure::UnitSeconds<-1>>;

template<typename T>
MetaMeasure::Meters<T> distanceTravelled(Velocity<T> vel, MetaMeasure::Seconds<T> time)
{
  return vel*time;
}
```
# Using literals
```cpp
auto volume = 5_m*3_m*2_m; // 30 m³
```

# Creating your own units
If you need to do this for some reason, it's easy.

```cpp
// Every unit has the ratio to corresponding base unit in its type.
// When your measurement is multiplied by this ratio, you should get the measurement in those base units.
// The 7 SI base units are used for the 7 SI dimensions/quantities.
// For instance, the base unit for length is a meter.
METAMEASURE_UNIT(Foos, MetaMeasure::Length, 1, 2); // For every 1 foo, there is 2 meters
```

You can also easily create literals for your unit, like so:
```cpp
METAMEASURE_LITERAL(Foos, 1, _foo);
METAMEASURE_LITERAL(Foos, 2, _foo2); // Square foos
METAMEASURE_LITERAL(Foos, 3, _foo3); // Cubic foos

// ...

// Literal for MetaMeasure::Measurement<long double, UnitFoo<1>>
// Redefine METAMEASURE_LITERAL_LD_NUMBER_TYPE if you want something other than long double
auto measurement = 12.34_foo;

// Literal for MetaMeaesure::Measurement<long long int, UnitFoo<1>>
// Redefine METAMEASURE_LITERAL_ULL_NUMBER_TYPE if you want something other than long long int
auto intMeasurement = 12_foo;
```

# Creating your own physical quantities/dimensions
You can also do this if you really need to, and it's even easier than creating a unit.
Bear in mind what you're using as the base unit, as that's what a 1:1 ratio will correspond to.

```cpp
struct MyDimension {};

METAMEASURE_UNIT(MyUnits, MyDimension, 1, 1); // MyUnits are the base unit for MyDimension
```
