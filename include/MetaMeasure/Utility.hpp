#ifndef METAMEASURE_UTILITY_INCLUDED
#define METAMEASURE_UTILITY_INCLUDED

#include <MetaMeasure/Unit.hpp>

#include <tuple>
#include <ratio>
#include <type_traits>

#define METAMEASURE_FORCE_SEMICOLON \
extern void* MetaMeasureSemicolonForcer_

namespace MetaMeasure
{

namespace Private
{

template<typename Tuple, typename Tuple2>
struct LargerTuple_
{
  using Type = std::conditional_t
  <
    (std::tuple_size<Tuple>::value < std::tuple_size<Tuple2>::value),
    Tuple2,
    Tuple
  >;
};

template<typename Tuple, typename Tuple2>
struct SmallerTuple_
{
  using Type = std::conditional_t
  <
    (std::tuple_size<Tuple>::value < std::tuple_size<Tuple2>::value),
    Tuple,
    Tuple2
  >;
};

template<typename Tuple, typename Tuple2>
using LargerTuple = typename LargerTuple_<Tuple, Tuple2>::Type;

template<typename Tuple, typename Tuple2>
using SmallerTuple = typename SmallerTuple_<Tuple, Tuple2>::Type;

// Checks if a Tuple has a dimension in it
template<typename Dimension, typename Tuple>
struct HasDimension_;

template<typename Dimension>
struct HasDimension_<Dimension, std::tuple<>> : std::false_type {};

template<typename Dimension, typename U, typename... Ts>
struct HasDimension_<Dimension, std::tuple<U, Ts...>> : std::conditional_t
<
  std::is_same_v<Dimension, typename U::Dimension>,
  std::true_type,
  HasDimension_<Dimension, std::tuple<Ts...>>
> {};

template<typename Dimension, typename Tuple>
using HasDimension = typename HasDimension_<Dimension, Tuple>::type;

template<typename Tuple, typename Tuple2>
struct SharesDimensions_;

template<typename Tuple>
struct SharesDimensions_<Tuple, std::tuple<>> : std::true_type {};

template<typename Tuple, typename U, typename... Ts>
struct SharesDimensions_<Tuple, std::tuple<U, Ts...>> : std::conditional_t
<
  HasDimension<typename U::Dimension, Tuple>::value,
  SharesDimensions_<Tuple, std::tuple<Ts...>>,
  std::false_type
> {};

// Checks if Tuple and Tuple2 share dimensions
template<typename Tuple, typename Tuple2>
using SharesDimensions = typename SharesDimensions_
<
  LargerTuple<Tuple, Tuple2>,
  SmallerTuple<Tuple, Tuple2>
>::type;

// Checks if Tuple and Tuple2 have the same exact dimensions
template<typename Tuple, typename Tuple2>
using IdenticalDimensions = typename SharesDimensions_
<
  SmallerTuple<Tuple, Tuple2>,
  LargerTuple<Tuple, Tuple2>
>::type;

// Checks if parameter pack has exactly one of a type
template<typename...>
struct HasOneOf;

template<typename T>
struct HasOneOf<T>
{
  static constexpr bool Value = false;
};

template<typename T, typename U, typename... Ts>
struct HasOneOf<T, U, Ts...>
{
  static constexpr bool Value = std::is_same_v<T, U>
    || HasOneOf<T, Ts...>::Value;
};

// Checks if Tuple has no duplicated dimensions at all
template<typename Tuple>
struct HasNoDuplicates;

template<>
struct HasNoDuplicates<std::tuple<>>
{
  static constexpr bool Value = true;
};

template<typename T, typename... Ts>
struct HasNoDuplicates<std::tuple<T, Ts...>>
{
  static constexpr bool Value = HasNoDuplicates<std::tuple<Ts...>>::Value &&
    !HasOneOf
    <
      typename T::Dimension::Identifier,
      typename Ts::Dimension::Identifier...
    >::Value;
};

// Calculates the reciprocal of a ratio
template<typename Ratio>
struct Reciprocal_
{
private:
  using R = Ratio;

public:
  using Type = std::ratio<R::den, R::num>;
};

template<typename Ratio>
using Reciprocal = typename Reciprocal_<Ratio>::Type;

// Helper ratio
using OneToOne = std::ratio<1, 1>;

// Calculates the unit's ratio to the base unit, accounting for the degree of the unit
// When I say accounting for degree: there are 1000 m in 1 km, and 1000000 m² in 1 km²
// The ratio changes with the degree
template<typename T, typename = std::true_type>
struct UnitRatio_;

template<typename T>
struct UnitRatio_<T, std::integral_constant<bool, (T::Dimension::Exponent == 0)>>
{
  using Type = OneToOne;
};

template<typename T>
struct UnitRatio_<T, std::integral_constant<bool, (T::Dimension::Exponent > 0)>>
{
private:
  using NextDimension = Dimension<typename T::Dimension::Identifier, (T::Dimension::Exponent - 1)>;
  using Ratio = typename T::Ratio;
  using NextUnit = Unit<NextDimension, Ratio>;

public:
  // Multiply the ratio by itself until the degree is 0. Basic exponentation, but done recursively.
  using Type = std::ratio_multiply<Ratio, typename UnitRatio_<NextUnit>::Type>;
};

template<typename T>
struct UnitRatio_<T, std::integral_constant<bool, (T::Dimension::Exponent < 0)>>
{
private:
  using NextDimension = Dimension<typename T::Dimension::Identifier, (-T::Dimension::Exponent)>;
  using Ratio = typename T::Ratio;
  using NextUnit = Unit<NextDimension, Ratio>;

public:
  // Since x^(-y) is really the reciprocal of x^y, we calculate x^y and then take the reciprocal
  using Type = Reciprocal<typename UnitRatio_<NextUnit>::Type>;
};

template<typename T>
using UnitRatio = typename UnitRatio_<T>::Type;

// Calculates the overall ratio of a measurement, which will be used for conversions
// Instead of converting each unit individually, we can do it all at once with this ratio
template<typename Tuple>
struct OverallRatio_;

template<typename T, typename U, typename... Ts>
struct OverallRatio_<std::tuple<T, U, Ts...>>
{
  using Type = std::ratio_multiply
  <
    std::ratio_multiply<UnitRatio<T>, UnitRatio<U>>,
    typename OverallRatio_<std::tuple<Ts...>>::Type
  >;
};

template<typename T, typename U>
struct OverallRatio_<std::tuple<T, U>>
{
  using Type = std::ratio_multiply<UnitRatio<T>, UnitRatio<U>>;
};

template<typename T>
struct OverallRatio_<std::tuple<T>>
{
  using Type = UnitRatio<T>;
};

template<typename Tuple>
using OverallRatio = typename OverallRatio_<Tuple>::Type;

template<typename UnitTuple, typename UnitTuple2>
using ConversionRatio = std::ratio_multiply<OverallRatio<UnitTuple>, OverallRatio<UnitTuple2>>;

}

}

#endif
