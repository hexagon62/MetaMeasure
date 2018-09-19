#ifndef METAMEASURE_UNITS_INCLUDED
#define METAMEASURE_UNITS_INCLUDED

#include <MetaMeasure/Measurement.hpp>

#include <cstddef>

namespace MetaMeasure
{

// Want to use different types for literals?
// Change these then!
using LongDoubleLiteralResult = long double;
using UnsignedLongLongLiteralResult = long long int;

}

// Does the same thing as METAMEASURE_UNIT except you pass in a type for RATIO
// Use a typedef for it, as macros hate commas
#define METAMEASURE_UNIT_WITH_RATIO_TYPE(NAME, DIMENSION, RATIO) \
template<MetaMeasure::ExponentType Exponent = 1> \
using Unit##NAME = MetaMeasure::Unit<MetaMeasure::Dimension<DIMENSION, Exponent>, RATIO>; \
template<typename NumT, MetaMeasure::ExponentType Exponent = 1> \
using NAME = MetaMeasure::Measurement<NumT, Unit##NAME<Exponent>>; \
METAMEASURE_FORCE_SEMICOLON

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

// Defines a literal for a single unit measurement which uses a long double as its value type
// UNIT is the name of the unit (not the full type name!)
// SUFFIX is the suffix the literal will use
#define METAMEASURE_LITERAL_LD(UNIT, EXPONENT, SUFFIX) \
UNIT<MetaMeasure::LongDoubleLiteralResult, EXPONENT> operator"" SUFFIX (long double v) \
{ \
  return static_cast<MetaMeasure::LongDoubleLiteralResult>(v); \
} \
METAMEASURE_FORCE_SEMICOLON

// Defines a literal for a single unit measurement which uses an unsigned long long int as its value type
// UNIT is the MetaMeasure::Measurement that will be returned
// SUFFIX is the suffix the literal will use
#define METAMEASURE_LITERAL_ULL(UNIT, EXPONENT, SUFFIX) \
UNIT<MetaMeasure::UnsignedLongLongLiteralResult, EXPONENT> operator"" SUFFIX (unsigned long long int v) \
{ \
  return static_cast<MetaMeasure::UnsignedLongLongLiteralResult>(v); \
} \
METAMEASURE_FORCE_SEMICOLON

// Defines a literal for a measurement which uses a long double as its parameter
// TYPE is the type of the measurement
// SUFFIX is the suffix the literal will use
#define METAMEASURE_LITERAL_LD_WITH_TYPE(TYPE, SUFFIX) \
TYPE operator"" SUFFIX (long double v) \
{ \
  return static_cast<MetaMeasure::LongDoubleLiteralResult>(v); \
} \
METAMEASURE_FORCE_SEMICOLON

// Defines a literal for a unit which uses an unsigned long long int as its parameter
// UNIT is the MetaMeasure::Measurement that will be returned
// SUFFIX is the suffix the literal will use
#define METAMEASURE_LITERAL_ULL_WITH_TYPE(TYPE, SUFFIX) \
TYPE operator"" SUFFIX (unsigned long long int v) \
{ \
  return static_cast<MetaMeasure::UnsignedLongLongLiteralResult>(v); \
} \
METAMEASURE_FORCE_SEMICOLON

// Combination of METAMEASURE_LITERAL_LD & METAMEASURE_LITERAL_ULL
#define METAMEASURE_LITERAL(UNIT, EXPONENT, SUFFIX) \
METAMEASURE_LITERAL_LD(UNIT, EXPONENT, SUFFIX); \
METAMEASURE_LITERAL_ULL(UNIT, EXPONENT, SUFFIX); \
METAMEASURE_FORCE_SEMICOLON

// Combination of METAMEASURE_LITERAL_LD_WITH_TYPE & METAMEASURE_LITERAL_ULL_WITH_TYPE
#define METAMEASURE_LITERAL_WITH_TYPE(TYPE, SUFFIX) \
METAMEASURE_LITERAL_LD_WITH_TYPE(TYPE, SUFFIX); \
METAMEASURE_LITERAL_ULL_WITH_TYPE(TYPE, SUFFIX); \
METAMEASURE_FORCE_SEMICOLON

#ifdef METAMEASURE_ZETTA_AND_ZEPTO_SUPPORTED
#define _METAMEASURE_ZETTA_AND_ZEPTO_RATIOS \
using Zetta = std::ratio_divide<Main, std::zetta>; \
using Zepto = std::ratio_divide<Main, std::zepto>; \
METAMEASURE_FORCE_SEMICOLON

#define _METAMEASURE_ZETTA_AND_ZEPTO_UNITS(NAME, DIMENSION) \
METAMEASURE_UNIT_WITH_RATIO_TYPE(Zetta##NAME, DIMENSION, _METAMEASURE_RATIOS_1_##NAME::Zetta); \
METAMEASURE_UNIT_WITH_RATIO_TYPE(Zepto##NAME, DIMENSION, _METAMEASURE_RATIOS_1_##NAME::Zepto); \
METAMEASURE_FORCE_SEMICOLON

#define _METAMEASURE_ZETTA_AND_ZEPTO_LITERALS(UNIT, EXPONENT, SUFFIX) \
METAMEASURE_LITERAL(Zetta##UNIT, EXPONENT, _Z##SUFFIX); \
METAMEASURE_LITERAL(Zepto##UNIT, EXPONENT, _z##SUFFIX); \
METAMEASURE_FORCE_SEMICOLON

#define _METAMEASURE_ZETTA_AND_ZEPTO_LITERALS_WITH_TYPE(TYPE, SUFFIX) \
METAMEASURE_LITERAL_WITH_TYPE(Zetta##TYPE, _Z##SUFFIX); \
METAMEASURE_LITERAL_WITH_TYPE(Zepto##TYPE, _z##SUFFIX); \
METAMEASURE_FORCE_SEMICOLON
#else
#define _METAMEASURE_ZETTA_AND_ZEPTO_RATIOS \
METAMEASURE_FORCE_SEMICOLON

#define _METAMEASURE_ZETTA_AND_ZEPTO_UNITS(NAME, DIMENSION) \
METAMEASURE_FORCE_SEMICOLON

#define _METAMEASURE_ZETTA_AND_ZEPTO_LITERALS(UNIT, EXPONENT, SUFFIX) \
METAMEASURE_FORCE_SEMICOLON

#define _METAMEASURE_ZETTA_AND_ZEPTO_LITERALS_WITH_TYPE(TYPE, SUFFIX) \
METAMEASURE_FORCE_SEMICOLON
#endif

#ifdef METAMEASURE_YOTTA_AND_YOCTO_SUPPORTED
#define _METAMEASURE_YOTTA_AND_YOCTO_RATIOS \
using Yotta = std::ratio_divide<Main, std::yotta>; \
using Yocto = std::ratio_divide<Main, std::yocto>; \
METAMEASURE_FORCE_SEMICOLON

#define _METAMEASURE_YOTTA_AND_YOCTO_UNITS(NAME, DIMENSION) \
METAMEASURE_UNIT_WITH_RATIO_TYPE(Yotta##NAME, DIMENSION, _METAMEASURE_RATIOS_1_##NAME::Yotta); \
METAMEASURE_UNIT_WITH_RATIO_TYPE(Yocto##NAME, DIMENSION, _METAMEASURE_RATIOS_1_##NAME::Yocto); \
METAMEASURE_FORCE_SEMICOLON

#define _METAMEASURE_YOTTA_AND_YOCTO_LITERALS(UNIT, EXPONENT, SUFFIX) \
METAMEASURE_LITERAL(Yotta##UNIT, EXPONENT, _Y##SUFFIX); \
METAMEASURE_LITERAL(Yocto##UNIT, EXPONENT, _y##SUFFIX); \
METAMEASURE_FORCE_SEMICOLON

#define _METAMEASURE_YOTTA_AND_YOCTO_LITERALS_WITH_TYPE(TYPE, SUFFIX) \
METAMEASURE_LITERAL_WITH_TYPE(Yotta##TYPE, _Y##SUFFIX); \
METAMEASURE_LITERAL_WITH_TYPE(Yocto##TYPE, _y##SUFFIX); \
METAMEASURE_FORCE_SEMICOLON
#else
#define _METAMEASURE_YOTTA_AND_YOCTO_RATIOS \
METAMEASURE_FORCE_SEMICOLON

#define _METAMEASURE_YOTTA_AND_YOCTO_UNITS(NAME, DIMENSION) \
METAMEASURE_FORCE_SEMICOLON

#define _METAMEASURE_YOTTA_AND_YOCTO_LITERALS(UNIT, EXPONENT, SUFFIX) \
METAMEASURE_FORCE_SEMICOLON

#define _METAMEASURE_YOTTA_AND_YOCTO_LITERALS_WITH_TYPE(TYPE, SUFFIX) \
METAMEASURE_FORCE_SEMICOLON
#endif

// Macro to make a type with all of the metric prefixes, save for the one without a prefix
// See METAMEASURE_UNIT_WITH_RATIO_TYPE for instructions, as the parameters are the same
#define METAMEASURE_UNIT_WITH_RATIO_TYPE_AND_METRIC_PREFIXES(NAME, DIMENSION, RATIO) \
namespace _METAMEASURE_RATIOS_1_##NAME \
{ \
  using Main  = RATIO; \
  using Exa   = std::ratio_divide<Main, std::exa>; \
  using Peta  = std::ratio_divide<Main, std::peta>; \
  using Tera  = std::ratio_divide<Main, std::tera>; \
  using Giga  = std::ratio_divide<Main, std::giga>; \
  using Mega  = std::ratio_divide<Main, std::mega>; \
  using Kilo  = std::ratio_divide<Main, std::kilo>; \
  using Hecto = std::ratio_divide<Main, std::hecto>; \
  using Deca  = std::ratio_divide<Main, std::deca>; \
  using Deci  = std::ratio_divide<Main, std::deci>; \
  using Centi = std::ratio_divide<Main, std::centi>; \
  using Milli = std::ratio_divide<Main, std::milli>; \
  using Micro = std::ratio_divide<Main, std::micro>; \
  using Nano  = std::ratio_divide<Main, std::nano>; \
  using Pico  = std::ratio_divide<Main, std::pico>; \
  using Femto = std::ratio_divide<Main, std::femto>; \
  using Atto  = std::ratio_divide<Main, std::atto>; \
  _METAMEASURE_ZETTA_AND_ZEPTO_RATIOS; \
  _METAMEASURE_YOTTA_AND_YOCTO_RATIOS; \
} \
METAMEASURE_UNIT_WITH_RATIO_TYPE(Exa##NAME,   DIMENSION, _METAMEASURE_RATIOS_1_##NAME::Exa); \
METAMEASURE_UNIT_WITH_RATIO_TYPE(Peta##NAME,  DIMENSION, _METAMEASURE_RATIOS_1_##NAME::Peta); \
METAMEASURE_UNIT_WITH_RATIO_TYPE(Tera##NAME,  DIMENSION, _METAMEASURE_RATIOS_1_##NAME::Tera); \
METAMEASURE_UNIT_WITH_RATIO_TYPE(Giga##NAME,  DIMENSION, _METAMEASURE_RATIOS_1_##NAME::Giga); \
METAMEASURE_UNIT_WITH_RATIO_TYPE(Mega##NAME,  DIMENSION, _METAMEASURE_RATIOS_1_##NAME::Mega); \
METAMEASURE_UNIT_WITH_RATIO_TYPE(Kilo##NAME,  DIMENSION, _METAMEASURE_RATIOS_1_##NAME::Kilo); \
METAMEASURE_UNIT_WITH_RATIO_TYPE(Hecto##NAME, DIMENSION, _METAMEASURE_RATIOS_1_##NAME::Hecto); \
METAMEASURE_UNIT_WITH_RATIO_TYPE(Deca##NAME,  DIMENSION, _METAMEASURE_RATIOS_1_##NAME::Deca); \
METAMEASURE_UNIT_WITH_RATIO_TYPE(Deci##NAME,  DIMENSION, _METAMEASURE_RATIOS_1_##NAME::Deci); \
METAMEASURE_UNIT_WITH_RATIO_TYPE(Centi##NAME, DIMENSION, _METAMEASURE_RATIOS_1_##NAME::Centi); \
METAMEASURE_UNIT_WITH_RATIO_TYPE(Milli##NAME, DIMENSION, _METAMEASURE_RATIOS_1_##NAME::Milli); \
METAMEASURE_UNIT_WITH_RATIO_TYPE(Micro##NAME, DIMENSION, _METAMEASURE_RATIOS_1_##NAME::Micro); \
METAMEASURE_UNIT_WITH_RATIO_TYPE(Nano##NAME,  DIMENSION, _METAMEASURE_RATIOS_1_##NAME::Nano); \
METAMEASURE_UNIT_WITH_RATIO_TYPE(Pico##NAME,  DIMENSION, _METAMEASURE_RATIOS_1_##NAME::Pico); \
METAMEASURE_UNIT_WITH_RATIO_TYPE(Femto##NAME, DIMENSION, _METAMEASURE_RATIOS_1_##NAME::Femto); \
METAMEASURE_UNIT_WITH_RATIO_TYPE(Atto##NAME,  DIMENSION, _METAMEASURE_RATIOS_1_##NAME::Atto); \
_METAMEASURE_ZETTA_AND_ZEPTO_UNITS(NAME, DIMENSION); \
_METAMEASURE_YOTTA_AND_YOCTO_UNITS(NAME, DIMENSION); \
METAMEASURE_FORCE_SEMICOLON

// Macro to make a type with all of the metric prefixes, save for the one without a prefix
// See METAMEASURE_UNIT for instructions, as the parameters are the same
#define METAMEASURE_UNIT_WITH_METRIC_PREFIXES(NAME, DIMENSION, RATIO_NUM, RATIO_DEN) \
namespace _METAMEASURE_RATIOS_2_##NAME { using Main  = std::ratio<RATIO_NUM, RATIO_DEN>; } \
METAMEASURE_UNIT_WITH_RATIO_TYPE_AND_METRIC_PREFIXES(NAME, DIMENSION, _METAMEASURE_RATIOS_2_##NAME::Main); \
METAMEASURE_FORCE_SEMICOLON

// Creates literals with metric prefixes. Be sure to have already created the units.
// That must be done using either METAMEASURE_UNIT_WITH_METRIC_PREFIXES
// Or METAMEASURE_UNIT_WITH_METRIC_PREFIXES_WITH_RATIO_TYPE
// Note that this puts the underscore in for you for your suffix.
#define METAMEASURE_LITERAL_WITH_METRIC_PREFIXES(UNIT, EXPONENT, SUFFIX) \
METAMEASURE_LITERAL(Exa##UNIT,   EXPONENT, _E##SUFFIX); \
METAMEASURE_LITERAL(Peta##UNIT,  EXPONENT, _P##SUFFIX); \
METAMEASURE_LITERAL(Tera##UNIT,  EXPONENT, _T##SUFFIX); \
METAMEASURE_LITERAL(Giga##UNIT,  EXPONENT, _G##SUFFIX); \
METAMEASURE_LITERAL(Mega##UNIT,  EXPONENT, _M##SUFFIX); \
METAMEASURE_LITERAL(Kilo##UNIT,  EXPONENT, _k##SUFFIX); \
METAMEASURE_LITERAL(Hecto##UNIT, EXPONENT, _h##SUFFIX); \
METAMEASURE_LITERAL(Deca##UNIT,  EXPONENT, _da##SUFFIX); \
METAMEASURE_LITERAL(Deci##UNIT,  EXPONENT, _d##SUFFIX); \
METAMEASURE_LITERAL(Centi##UNIT, EXPONENT, _c##SUFFIX); \
METAMEASURE_LITERAL(Milli##UNIT, EXPONENT, _m##SUFFIX); \
METAMEASURE_LITERAL(Micro##UNIT, EXPONENT, _u##SUFFIX); \
METAMEASURE_LITERAL(Nano##UNIT,  EXPONENT, _n##SUFFIX); \
METAMEASURE_LITERAL(Pico##UNIT,  EXPONENT, _p##SUFFIX); \
METAMEASURE_LITERAL(Femto##UNIT, EXPONENT, _f##SUFFIX); \
METAMEASURE_LITERAL(Atto##UNIT,  EXPONENT, _a##SUFFIX); \
_METAMEASURE_ZETTA_AND_ZEPTO_LITERALS(UNIT, EXPONENT, SUFFIX); \
_METAMEASURE_YOTTA_AND_YOCTO_LITERALS(UNIT, EXPONENT, SUFFIX); \
METAMEASURE_FORCE_SEMICOLON

// See METAMEASURE_LITERAL_WITH_METRIC_PREFIXES
// You specify the type directly through TYPE instead of putting in a unit and exponent.
// The type should be a typedef though, since macros don't like commas
#define METAMEASURE_LITERAL_WITH_TYPE_AND_METRIC_PREFIXES(TYPE, SUFFIX) \
METAMEASURE_LITERAL_WITH_TYPE(Exa##TYPE,   _E##SUFFIX); \
METAMEASURE_LITERAL_WITH_TYPE(Peta##TYPE,  _P##SUFFIX); \
METAMEASURE_LITERAL_WITH_TYPE(Tera##TYPE,  _T##SUFFIX); \
METAMEASURE_LITERAL_WITH_TYPE(Giga##TYPE,  _G##SUFFIX); \
METAMEASURE_LITERAL_WITH_TYPE(Mega##TYPE,  _M##SUFFIX); \
METAMEASURE_LITERAL_WITH_TYPE(Kilo##TYPE,  _k##SUFFIX); \
METAMEASURE_LITERAL_WITH_TYPE(Hecto##TYPE, _h##SUFFIX); \
METAMEASURE_LITERAL_WITH_TYPE(Deca##TYPE,  _da##SUFFIX); \
METAMEASURE_LITERAL_WITH_TYPE(Deci##TYPE,  _d##SUFFIX); \
METAMEASURE_LITERAL_WITH_TYPE(Centi##TYPE, _c##SUFFIX); \
METAMEASURE_LITERAL_WITH_TYPE(Milli##TYPE, _m##SUFFIX); \
METAMEASURE_LITERAL_WITH_TYPE(Micro##TYPE, _u##SUFFIX); \
METAMEASURE_LITERAL_WITH_TYPE(Nano##TYPE,  _n##SUFFIX); \
METAMEASURE_LITERAL_WITH_TYPE(Pico##TYPE,  _p##SUFFIX); \
METAMEASURE_LITERAL_WITH_TYPE(Femto##TYPE, _f##SUFFIX); \
METAMEASURE_LITERAL_WITH_TYPE(Atto##TYPE,  _a##SUFFIX); \
_METAMEASURE_ZETTA_AND_ZEPTO_LITERALS_WITH_TYPE(TYPE, SUFFIX); \
_METAMEASURE_YOTTA_AND_YOCTO_LITERALS_WITH_TYPE(TYPE, SUFFIX); \
METAMEASURE_FORCE_SEMICOLON

namespace MetaMeasure
{

// SI base units; ratios are relative to these
// I'm aware the base unit for mass is actually kilograms, but for the sake of simplicity, I'm using grams.
// It wouldn't be nice to use std::nano to describe the ratio between micrograms and kilograms, would it?

METAMEASURE_UNIT(Meters,   Length,            1, 1);
METAMEASURE_UNIT(Grams,    Mass,              1, 1);
METAMEASURE_UNIT(Seconds,  Time,              1, 1);
METAMEASURE_UNIT(Kelvins,  Temperature,       1, 1);
METAMEASURE_UNIT(Amperes,  Current,           1, 1);
METAMEASURE_UNIT(Candelas, Luminosity,        1, 1);
METAMEASURE_UNIT(Moles,    SubstanceQuantity, 1, 1);
METAMEASURE_UNIT_WITH_METRIC_PREFIXES(meters,   Length,            1, 1);
METAMEASURE_UNIT_WITH_METRIC_PREFIXES(grams,    Mass,              1, 1);
METAMEASURE_UNIT_WITH_METRIC_PREFIXES(seconds,  Time,              1, 1);
METAMEASURE_UNIT_WITH_METRIC_PREFIXES(kelvins,  Temperature,       1, 1);
METAMEASURE_UNIT_WITH_METRIC_PREFIXES(amperes,  Current,           1, 1);
METAMEASURE_UNIT_WITH_METRIC_PREFIXES(candelas, Luminosity,        1, 1);
METAMEASURE_UNIT_WITH_METRIC_PREFIXES(moles,    SubstanceQuantity, 1, 1);

namespace MetricLiterals
{

METAMEASURE_LITERAL(Meters,    1, _m);
METAMEASURE_LITERAL(Grams,     1, _g);
METAMEASURE_LITERAL(Seconds,   1, _s);
METAMEASURE_LITERAL(Seconds,  -1, _Hz);
METAMEASURE_LITERAL(Kelvins,   1, _K);
METAMEASURE_LITERAL(Amperes,   1, _A);
METAMEASURE_LITERAL(Candelas,  1, _cd);
METAMEASURE_LITERAL(Moles,     1, _mol);
METAMEASURE_LITERAL_WITH_METRIC_PREFIXES(meters,   1,  m);
METAMEASURE_LITERAL_WITH_METRIC_PREFIXES(grams,    1,  g);
METAMEASURE_LITERAL_WITH_METRIC_PREFIXES(seconds,  1,  s);
METAMEASURE_LITERAL_WITH_METRIC_PREFIXES(seconds,  -1, Hz);
METAMEASURE_LITERAL_WITH_METRIC_PREFIXES(kelvins,  1,  K);
METAMEASURE_LITERAL_WITH_METRIC_PREFIXES(amperes,  1,  A);
METAMEASURE_LITERAL_WITH_METRIC_PREFIXES(candelas, 1,  cd);
METAMEASURE_LITERAL_WITH_METRIC_PREFIXES(moles,    1,  mol);

}

// Customary units
// To give an example of how the conversion ratios work, there are exactly 2.54 centimeters in an inch.
// There are 100 centimeters for every 1 meter, and thus 100 inches for every 2.54 meters.
// Since std::ratio can only hold whole numbers, the ratio will be 10000 inches to 254 meters.
// We can simplify this down to 5000 inches per 127 meters.

// Lengths
METAMEASURE_UNIT(Points, Length, 360000, 127);
METAMEASURE_UNIT(Pica, Length, 30000, 127);
METAMEASURE_UNIT(Inches, Length, 5000, 127); // ratio from above
METAMEASURE_UNIT(Feet, Length, 1250, 381);
METAMEASURE_UNIT(Yards, Length, 1250, 1143);
METAMEASURE_UNIT(Miles, Length, 2200000, 1143);
METAMEASURE_UNIT(Links, Length, 825, 381);
METAMEASURE_UNIT(SurveyFeet, Length, 3937, 1200);
METAMEASURE_UNIT(Rods, Length, 6875, 127);
METAMEASURE_UNIT(Chains, Length, 27500, 127);
METAMEASURE_UNIT(Furlongs, Length, 275000, 127);
METAMEASURE_UNIT(SurveyMiles, Length, 2200000, 127);
METAMEASURE_UNIT(Leagues, Length, 6600000, 127);
METAMEASURE_UNIT(Fathoms, Length, 2500, 1143);
METAMEASURE_UNIT(Cables, Length, 100000, 381);
METAMEASURE_UNIT(NauticalMiles, Length, 281300, 127);

namespace CustomaryLiterals
{

// Lengths
METAMEASURE_LITERAL(Points, 1, _pt);
METAMEASURE_LITERAL(Pica, 1, _pc);
METAMEASURE_LITERAL(Inches, 1, _in);
METAMEASURE_LITERAL(Feet, 1, _ft);
METAMEASURE_LITERAL(Yards, 1, _yd);
METAMEASURE_LITERAL(Miles, 1, _mi);
METAMEASURE_LITERAL(Links, 1, _li);
METAMEASURE_LITERAL(SurveyFeet, 1, _sft);
METAMEASURE_LITERAL(Rods, 1, _rd);
METAMEASURE_LITERAL(Chains, 1, _ch);
METAMEASURE_LITERAL(Furlongs, 1, _fur);
METAMEASURE_LITERAL(SurveyMiles, 1, _smi);
METAMEASURE_LITERAL(Leagues, 1, _lea);
METAMEASURE_LITERAL(Fathoms, 1, _ftm);
METAMEASURE_LITERAL(Cables, 1, _cb);
METAMEASURE_LITERAL(NauticalMiles, 1, _nm);

}

}

#endif
