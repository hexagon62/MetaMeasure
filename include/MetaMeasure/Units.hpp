#ifndef METAMEASURE_UNITS_INCLUDED
#define METAMEASURE_UNITS_INCLUDED

#include <MetaMeasure/Measurement.hpp>

// This macro will make creating a unit much easier
// See below for examples on how to use it
#define METAMEASURE_UNIT(NAME, DIMENSION, RATIO_NUM, RATIO_DEN) \
template<MetaMeasure::ExponentType Exponent = 1> \
using Unit##NAME = MetaMeasure::Unit<MetaMeasure::Dimension<DIMENSION, Exponent>, std::ratio<RATIO_NUM, RATIO_DEN>>; \
template<typename NumT, MetaMeasure::ExponentType Exponent = 1> \
using NAME = MetaMeasure::Measurement<NumT, Unit##NAME<Exponent>>;

#ifndef METAMEASURE_LITERAL_LD_NUMBER_TYPE
#define METAMEASURE_LITERAL_LD_NUMBER_TYPE long double
#endif

#ifndef METAMEASURE_LITERAL_ULL_NUMBER_TYPE
#define METAMEASURE_LITERAL_ULL_NUMBER_TYPE long long int
#endif

// Defines a literal for a unit which uses a long double as its parameter
// UNIT is the name of the unit (not the full type name!)
// SUFFIX is the suffix the literal will use
#define METAMEASURE_LITERAL_LD(UNIT, EXPONENT, SUFFIX) \
UNIT<METAMEASURE_LITERAL_LD_NUMBER_TYPE, EXPONENT> operator"" SUFFIX (long double v) \
{ \
  return static_cast<METAMEASURE_LITERAL_LD_NUMBER_TYPE>(v); \
}

// Defines a literal for a unit which uses an unsigned long long int as its parameter
// UNIT is the MetaMeasure::Measurement that will be returned
// SUFFIX is the suffix the literal will use
#define METAMEASURE_LITERAL_ULL(UNIT, EXPONENT, SUFFIX) \
UNIT<METAMEASURE_LITERAL_ULL_NUMBER_TYPE, EXPONENT> operator"" SUFFIX (unsigned long long int v) \
{ \
  return static_cast<METAMEASURE_LITERAL_ULL_NUMBER_TYPE>(v); \
}

// Combination of METAMEASURE_LITERAL_LD & METAMEASURE_LITERAL_ULL
#define METAMEASURE_LITERAL(UNIT, EXPONENT, SUFFIX) \
METAMEASURE_LITERAL_LD(UNIT, EXPONENT, SUFFIX) \
METAMEASURE_LITERAL_ULL(UNIT, EXPONENT, SUFFIX)

namespace MetaMeasure
{

// SI base units; ratios are relative to these
// I'm aware the base unit for mass is actually kilograms, but for the sake of simplicity, I'm using grams.
// It wouldn't be nice to use std::nano to describe the ratio between micrograms and kilograms, would it?
METAMEASURE_UNIT(Meters, Length, 1, 1)
METAMEASURE_UNIT(Grams, Mass, 1, 1)
METAMEASURE_UNIT(Seconds, Time, 1, 1)
METAMEASURE_UNIT(Kelvins, Temperature, 1, 1)
METAMEASURE_UNIT(Amperes, Current, 1, 1)
METAMEASURE_UNIT(Candelas, Luminosity, 1, 1)
METAMEASURE_UNIT(Moles, SubstanceQuantity, 1, 1)

namespace Literals
{

METAMEASURE_LITERAL(Meters, 1, _m)
METAMEASURE_LITERAL(Grams, 1, _g)
METAMEASURE_LITERAL(Seconds, 1, _s)
METAMEASURE_LITERAL(Kelvins, 1, _K)
METAMEASURE_LITERAL(Amperes, 1, _A)
METAMEASURE_LITERAL(Candelas, 1, _cd)
METAMEASURE_LITERAL(Moles, 1, _mol)

}

// Customary units
// To give an example of how the conversion ratios work, there are exactly 2.54 centimeters in an inch.
// There are 100 centimeters in a meter, and thus 100 inches in 2.54 meters.
// Since std::ratio can only hold whole numbers, the ratio will be 254 meters to 10000 inches.
// We can simplify this down to 127 meters per 5000 inches.
METAMEASURE_UNIT(Inches, Length, 127, 5000)

}

#endif
