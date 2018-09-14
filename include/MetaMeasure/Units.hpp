#ifndef METAMEASURE_UNITS_INCLUDED
#define METAMEASURE_UNITS_INCLUDED

#include <MetaMeasure/Measurement.hpp>

// Creates 2 type definitions for a unit:
// The first type definition will be called Unit##NAME.
// This is just used to specify that a measurement uses the unit.
// The other type definition will be called NAME, which is a MetaMeasure::Measurement that only uses the unit.
// See the examples in the header below
// DIMENSION is the quantity that is measured (i.e. MetaMeasure::Length)
// RATIO_NUM is the conversion ratio's numerator
// RATIO_DEN is the conversion ratio's denominator
// The conversion ratio is a ratio that, if multiplied by your measurement, converts the measurement to base units.
// In the case of MetaMeasure::Length, it would convert your measurement to meters.
#define METAMEASURE_UNIT(NAME, DIMENSION, RATIO_NUM, RATIO_DEN) \
template<MetaMeasure::ExponentType Exponent = 1> \
using Unit##NAME = MetaMeasure::Unit<MetaMeasure::Dimension<DIMENSION, Exponent>, std::ratio<RATIO_NUM, RATIO_DEN>>; \
template<typename NumT, MetaMeasure::ExponentType Exponent = 1> \
using NAME = MetaMeasure::Measurement<NumT, Unit##NAME<Exponent>>; \
METAMEASURE_FORCE_SEMICOLON

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
} \
METAMEASURE_FORCE_SEMICOLON

// Defines a literal for a unit which uses an unsigned long long int as its parameter
// UNIT is the MetaMeasure::Measurement that will be returned
// SUFFIX is the suffix the literal will use
#define METAMEASURE_LITERAL_ULL(UNIT, EXPONENT, SUFFIX) \
UNIT<METAMEASURE_LITERAL_ULL_NUMBER_TYPE, EXPONENT> operator"" SUFFIX (unsigned long long int v) \
{ \
  return static_cast<METAMEASURE_LITERAL_ULL_NUMBER_TYPE>(v); \
} \
METAMEASURE_FORCE_SEMICOLON

// Combination of METAMEASURE_LITERAL_LD & METAMEASURE_LITERAL_ULL
#define METAMEASURE_LITERAL(UNIT, EXPONENT, SUFFIX) \
METAMEASURE_LITERAL_LD(UNIT, EXPONENT, SUFFIX); \
METAMEASURE_LITERAL_ULL(UNIT, EXPONENT, SUFFIX); \
METAMEASURE_FORCE_SEMICOLON

namespace MetaMeasure
{

// SI base units; ratios are relative to these
// I'm aware the base unit for mass is actually kilograms, but for the sake of simplicity, I'm using grams.
// It wouldn't be nice to use std::nano to describe the ratio between micrograms and kilograms, would it?
METAMEASURE_UNIT(Meters, Length, 1, 1);
METAMEASURE_UNIT(Grams, Mass, 1, 1);
METAMEASURE_UNIT(Seconds, Time, 1, 1);
METAMEASURE_UNIT(Kelvins, Temperature, 1, 1);
METAMEASURE_UNIT(Amperes, Current, 1, 1);
METAMEASURE_UNIT(Candelas, Luminosity, 1, 1);
METAMEASURE_UNIT(Moles, SubstanceQuantity, 1, 1);

namespace Literals
{

METAMEASURE_LITERAL(Meters, 1, _m);
METAMEASURE_LITERAL(Grams, 1, _g);
METAMEASURE_LITERAL(Seconds, 1, _s);
METAMEASURE_LITERAL(Kelvins, 1, _K);
METAMEASURE_LITERAL(Amperes, 1, _A);
METAMEASURE_LITERAL(Candelas, 1, _cd);
METAMEASURE_LITERAL(Moles, 1, _mol);

}

// Customary units
// To give an example of how the conversion ratios work, there are exactly 2.54 centimeters in an inch.
// There are 100 centimeters for every 1 meter, and thus 100 inches for every 2.54 meters.
// Since std::ratio can only hold whole numbers, the ratio will be 10000 inches to 254 meters.
// We can simplify this down to 5000 inches per 127 meters.
METAMEASURE_UNIT(Inches, Length, 5000, 127);

}

#endif
