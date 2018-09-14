#ifndef METAMEASURE_UNIT_INCLUDED
#define METAMEASURE_UNIT_INCLUDED

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

}

#endif
