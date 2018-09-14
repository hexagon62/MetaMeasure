#ifndef METAMEASURE_UNITS_INCLUDED
#define METAMEASURE_UNITS_INCLUDED

#include <ratio>
#include <type_traits>

namespace MetaMeasure
{
using OneToOne = std::ratio<1, 1>;
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

// SI base units; ratios are relative to these
template<ExponentType Exponent = 1> using UnitMeters = Unit<Dimension<Length, Exponent>, OneToOne>;
template<ExponentType Exponent = 1> using UnitKilograms = Unit<Dimension<Mass, Exponent>, std::kilo>;
template<ExponentType Exponent = 1> using UnitSeconds = Unit<Dimension<Time, Exponent>, OneToOne>;
template<ExponentType Exponent = 1> using UnitKelvins = Unit<Dimension<Temperature, Exponent>, OneToOne>;
template<ExponentType Exponent = 1> using UnitAmperes = Unit<Dimension<Current, Exponent>, OneToOne>;
template<ExponentType Exponent = 1> using UnitCandelas = Unit<Dimension<Luminosity, Exponent>, OneToOne>;
template<ExponentType Exponent = 1> using UnitMoles = Unit<Dimension<SubstanceQuantity, Exponent>, OneToOne>;
}

#endif
