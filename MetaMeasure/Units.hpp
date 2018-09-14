#ifndef METAMEASURE_UNITS_INCLUDED
#define METAMEASURE_UNITS_INCLUDED

#include <ratio>
#include <type_traits>

namespace MetaMeasure
{
using ExponentType = short;

template<typename IdentifierT, ExponentType ExponentV = 1>
struct Dimension
{
  using Identifier = IdentifierT;
  static constexpr ExponentType Exponent = ExponentV;
};

template<typename DimensionT, typename RatioT>
struct Unit
{
  using Dimension = DimensionT;
  using Ratio = RatioT;
};

// The 7 SI dimensions
struct Length {};
struct Mass {};
struct Time {};
struct Temperature {};
struct Current {};
struct Luminosity {};
struct SubstanceQuantity {};

// This macro will make creating a unit much easier
// See below for examples on how to use it
#define METAMEASURE_UNIT(NAME, DIMENSION, RATIO_NUM, RATIO_DEN) \
template<ExponentType Exponent = 1> \
using NAME = Unit<Dimension<DIMENSION, Exponent>, std::ratio<RATIO_NUM, RATIO_DEN>>

// SI base units; ratios are relative to these
// I'm aware the base unit for mass is actually kilograms, but for the sake of simplicity, I'm using grams.
// It wouldn't be nice to use std::nano to describe the ratio between micrograms and kilograms, would it?
METAMEASURE_UNIT(UnitMeters, Length, 1, 1);
METAMEASURE_UNIT(UnitGrams, Mass, 1, 1);
METAMEASURE_UNIT(UnitSeconds, Time, 1, 1);
METAMEASURE_UNIT(UnitKelvins, Temperature, 1, 1);
METAMEASURE_UNIT(UnitAmperes, Current, 1, 1);
METAMEASURE_UNIT(UnitCandelas, Luminosity, 1, 1);
METAMEASURE_UNIT(UnitMoles, SubstanceQuantity, 1, 1);

// Customary units
// To give an example of how the conversion ratios work, there are exactly 2.54 centimeters in an inch.
// There are 100 centimeters in a meter, and thus 100 inches in 2.54 meters.
// Since std::ratio can only hold whole numbers, the ratio will be 254 meters to 10000 inches.
// We can simplify this down to 127 meters per 5000 inches.
METAMEASURE_UNIT(UnitInches, Length, 127, 5000);
}

#endif
