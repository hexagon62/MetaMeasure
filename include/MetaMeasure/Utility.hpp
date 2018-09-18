#ifndef METAMEASURE_UTILITY_INCLUDED
#define METAMEASURE_UTILITY_INCLUDED

#include <MetaMeasure/UnitBase.hpp>

#include <tuple>
#include <ratio>
#include <type_traits>

// This is for compilers that warn about extra semi-colons, even though they're perfectly legal
#define METAMEASURE_FORCE_SEMICOLON \
extern void* MetaMeasureSemicolonForcer_

namespace MetaMeasure
{

// Helper ratio
using OneToOne = std::ratio<1, 1>;

namespace Private
{
// Gets the exponent of a unit's dimension
template<typename T>
struct ExponentOf
{
  static constexpr ExponentType Value = T::Dimension::Exponent;
};

// Evaluates to the tuple that's larger
template<typename Tuple, typename Tuple2>
using LargerTuple = std::conditional_t
<
  (std::tuple_size<Tuple>::value < std::tuple_size<Tuple2>::value),
  Tuple2,
  Tuple
>;

// Evaluates to the tuple that's smaller
template<typename Tuple, typename Tuple2>
using SmallerTuple = std::conditional_t
<
  (std::tuple_size<Tuple>::value < std::tuple_size<Tuple2>::value),
  Tuple,
  Tuple2
>;

// Convert tuple of units to tuple of dimensions
template<typename Tuple>
struct DimensionsTuple_;

template<typename... Units>
struct DimensionsTuple_<std::tuple<Units...>>
{
  using Type = std::tuple<typename Units::Dimension...>;
};

template<typename Tuple>
using DimensionsTuple = typename DimensionsTuple_<Tuple>::Type;

// Convert tuple of units to tuple of dimension identifiers
template<typename Tuple>
struct IdentifierTuple_;

template<typename... Units>
struct IdentifierTuple_<std::tuple<Units...>>
{
  using Type = std::tuple<typename Units::Dimension::Identifier...>;
};

template<typename Tuple>
using IdentifierTuple = typename IdentifierTuple_<Tuple>::Type;

// Checks if a parameter pack has a type in it
template<typename T, typename...>
struct HasType;

template<typename T>
struct HasType<T> : std::false_type {};

template<typename T, typename U, typename... Ts>
struct HasType<T, U, Ts...> : std::conditional_t
<
  std::is_same_v<T, U>,
  std::true_type,
  HasType<T, Ts...>
> {};

// Check if Tuple2 only has types Tuple has
template<typename Tuple, typename Tuple2>
struct SharesTypes_;

template<typename... Ts>
struct SharesTypes_<std::tuple<Ts...>, std::tuple<>> : std::true_type {};

template<typename... Ts, typename U, typename... Us>
struct SharesTypes_<std::tuple<Ts...>, std::tuple<U, Us...>> : std::conditional_t
<
  HasType<U, Ts...>::value,
  SharesTypes_<std::tuple<Ts...>, std::tuple<Us...>>,
  std::false_type
> {};

template<typename Tuple, typename Tuple2>
using SharesTypes = SharesTypes_
<
  LargerTuple<Tuple, Tuple2>,
  SmallerTuple<Tuple, Tuple2>
>;

template<typename Tuple, typename Tuple2>
using IdenticalTypes = SharesTypes_
<
  SmallerTuple<Tuple, Tuple2>,
  LargerTuple<Tuple, Tuple2>
>;

// Checks if Tuple and Tuple2 share dimensions
template<typename Tuple, typename Tuple2>
struct SharesDimensions : SharesTypes
<
  DimensionsTuple<Tuple>,
  DimensionsTuple<Tuple2>
> {};

// Checks if Tuple and Tuple2 have the same exact dimensions
template<typename Tuple, typename Tuple2>
struct IdenticalDimensions : IdenticalTypes
<
  DimensionsTuple<Tuple>,
  DimensionsTuple<Tuple2>
> {};

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

// Calculates the unit's ratio to the base unit, accounting for the degree of the unit
// When I say accounting for degree: there are 1000 m in 1 km, and 1000000 m² in 1 km²
// The ratio changes with the degree
template<typename T, typename = std::true_type>
struct UnitRatio_;

template<typename T>
struct UnitRatio_<T, std::integral_constant<bool, (ExponentOf<T>::Value == 0)>>
{
  using Type = OneToOne;
};

template<typename T>
struct UnitRatio_<T, std::integral_constant<bool, (ExponentOf<T>::Value > 0)>>
{
private:
  using NextDimension = Dimension<typename T::Dimension::Identifier, (ExponentOf<T>::Value - 1)>;
  using Ratio = typename T::Ratio;
  using NextUnit = Unit<NextDimension, Ratio>;

public:
  // Multiply the ratio by itself until the degree is 0. Basic exponentation, but done recursively.
  using Type = std::ratio_multiply<Ratio, typename UnitRatio_<NextUnit>::Type>;
};

template<typename T>
struct UnitRatio_<T, std::integral_constant<bool, (ExponentOf<T>::Value < 0)>>
{
private:
  using NextDimension = Dimension<typename T::Dimension::Identifier, (-ExponentOf<T>::Value)>;
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
using ConversionRatio = std::ratio_divide<OverallRatio<UnitTuple>, OverallRatio<UnitTuple2>>;

// The result of a tuple concatenation
template<typename... Ts>
using TupleCat = decltype(std::tuple_cat(std::declval<Ts>()...));

// Removes all dimensions that have degree 0 in Tuple
template<typename Tuple>
struct RemoveZeroDimensions_;

template<>
struct RemoveZeroDimensions_<std::tuple<>>
{
  using Type = std::tuple<>;
};

template<typename T, typename... Ts>
struct RemoveZeroDimensions_<std::tuple<T, Ts...>>
{
  using Type = TupleCat
  <
    std::conditional_t
    <
      (ExponentOf<T>::Value == 0),
      std::tuple<>,
      std::tuple<T>
    >,
    typename RemoveZeroDimensions_<std::tuple<Ts...>>::Type
  >;
};

template<typename Tuple>
using RemoveZeroDimensions = typename RemoveZeroDimensions_<Tuple>::Type;

// Take the reciprocal of the dimensions
// For instance, time (s) becomes 1/time (Hz)
template<typename Tuple>
struct ReciprocalDimensions_;

template<>
struct ReciprocalDimensions_<std::tuple<>>
{
  using Type = std::tuple<>;
};

template<typename T, typename... Ts>
struct ReciprocalDimensions_<std::tuple<T, Ts...>>
{
private:
  using FlippedDimension = Dimension<typename T::Dimension::Identifier, (-ExponentOf<T>::Value)>;
  using FlippedUnit = Unit<FlippedDimension, typename T::Ratio>;

public:
  using Type = TupleCat
  <
    std::tuple<FlippedUnit>,
    typename ReciprocalDimensions_<std::tuple<Ts...>>::Type
  >;
};

template<typename Tuple>
using ReciprocalDimensions = typename ReciprocalDimensions_<Tuple>::Type;

// Multiplies a single dimension from Tuple by T
template<typename Tuple, typename T>
struct MultiplyDimension_;

template<typename T>
struct MultiplyDimension_<std::tuple<>, T>
{
  using Type = std::tuple<>;
};

template<typename T, typename U, typename... Us>
struct MultiplyDimension_<std::tuple<U, Us...>, T>
{
private:
  static constexpr bool SameDimension = std::is_same_v
  <
    typename T::Dimension::Identifier,
    typename U::Dimension::Identifier
  >;

  using MultipliedDimension = Dimension
  <
    typename T::Dimension::Identifier,
    (ExponentOf<T>::Value + ExponentOf<U>::Value)
  >;

  using MultipliedUnit = Unit
  <
    MultipliedDimension,
    typename T::Ratio
  >;

public:
  using Type = TupleCat
  <
    std::conditional_t
    <
      SameDimension,
      std::tuple<MultipliedUnit>,
      std::tuple<U>
    >,
    typename MultiplyDimension_<std::tuple<Us...>, T>::Type
  >;
};

template<typename Tuple, typename T>
using MultiplyDimension = typename MultiplyDimension_<Tuple, T>::Type;

// Multiplies Tuple's dimensions by Tuple2's dimensions
template<typename Tuple, typename Tuple2>
struct MultiplyDimensions_;

template<typename... Ts>
struct MultiplyDimensions_ <std::tuple<Ts...>, std::tuple<>>
{
  using Type = std::tuple<Ts...>;
};

template<typename... Ts, typename U, typename... Us>
struct MultiplyDimensions_<std::tuple<Ts...>, std::tuple<U, Us...>>
{
private:
  static constexpr bool TupleHasDimension = HasType
  <
    typename U::Dimension::Identifier,
    typename Ts::Dimension::Identifier...
  >::value;

public:
  using Type = typename MultiplyDimensions_
  <
    std::conditional_t
    <
      TupleHasDimension,
      MultiplyDimension<std::tuple<Ts...>, U>,
      TupleCat<std::tuple<Ts...>, std::tuple<U>>
    >,
    std::tuple<Us...>
  >::Type;
};

template<typename Tuple, typename Tuple2>
using MultiplyDimensions = 
  typename MultiplyDimensions_
  <
    LargerTuple<Tuple, Tuple2>,
    SmallerTuple<Tuple, Tuple2>
  >::Type;

// Divides Tuple's dimensions by Tuple2's dimensions
template<typename Tuple, typename Tuple2>
using DivideDimensions = MultiplyDimensions<Tuple, ReciprocalDimensions<Tuple2>>;

}

}

#endif
