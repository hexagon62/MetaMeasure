#ifndef METAMEASURE_UNITS_INCLUDED
#define METAMEASURE_UNITS_INCLUDED

#include <ratio>
#include <type_traits>
#include <tuple>

namespace MetaMeasure
{
using OneToOne = std::ratio<1, 1>;
using DegreeType = short;

template<typename IdentifierT, DegreeType DegreeV = 1>
struct Dimension
{
  using Identifier = IdentifierT;
  static constexpr DegreeType Degree = DegreeV;
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
template<DegreeType DegreeV = 1> using UnitMeters = Unit<Dimension<Length, DegreeV>, OneToOne>;
template<DegreeType DegreeV = 1> using UnitKilograms = Unit<Dimension<Mass, DegreeV>, std::kilo>;
template<DegreeType DegreeV = 1> using UnitSeconds = Unit<Dimension<Time, DegreeV>, OneToOne>;
template<DegreeType DegreeV = 1> using UnitKelvins = Unit<Dimension<Temperature, DegreeV>, OneToOne>;
template<DegreeType DegreeV = 1> using UnitAmperes = Unit<Dimension<Current, DegreeV>, OneToOne>;
template<DegreeType DegreeV = 1> using UnitCandelas = Unit<Dimension<Luminosity, DegreeV>, OneToOne>;
template<DegreeType DegreeV = 1> using UnitMoles = Unit<Dimension<SubstanceQuantity, DegreeV>, OneToOne>;
}

#endif
