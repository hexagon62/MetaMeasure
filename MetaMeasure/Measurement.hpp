#ifndef METAMEASURE_MEASUREMENT_INCLUDED
#define METAMEASURE_MEASUREMENT_INCLUDED

#include <MetaMeasure/Units.hpp>

namespace MetaMeasure
{
template<typename Tuple, typename Tuple2>
struct LargerTuple_
{
  using Type = std::conditional_t
  <
    std::tuple_size<Tuple>::value < std::tuple_size<Tuple2>::value,
    Tuple2,
    Tuple
  >;
};

template<typename Tuple, typename Tuple2>
struct SmallerTuple_
{
  using Type = std::conditional_t
  <
    std::tuple_size<Tuple>::value < std::tuple_size<Tuple2>::value,
    Tuple,
    Tuple2
  >;
};

template<typename Tuple, typename Tuple2>
using LargerTuple = typename LargerTuple_<Tuple, Tuple2>::Type;

template<typename Tuple, typename Tuple2>
using SmallerTuple = typename SmallerTuple_<Tuple, Tuple2>::Type;

// For use in checking if operations are valid
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
>{};

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

// Calculates the reciprocal of a ratio
template<typename Ratio>
using Reciprocal = std::ratio<Ratio::den, Ratio::num>;

// Calculates the unit's ratio to the base unit, accounting for the degree of the unit
// When I say accounting for degree: there are 1000 m in 1 km, and 1000000 m² in 1 km²
// The ratio changes with the degree

template<typename Ratio, DegreeType Degree>
struct UnitRatio_ : std::conditional_t
<
  (Degree == 0),
  OneToOne,
  std::conditional_t
  <
    (Degree > 0),

    // If the degree is more than 1, multiply the ratio by itself until the degree is 0
    std::ratio_multiply<Ratio, UnitRatio_<Ratio, (Degree - 1)>>,

    // If the degree is less than 1, take the absolute value of the degree,
    // And do same as you would with a positive degree. Then take the reciprocal of the ratio.
    Reciprocal<UnitRatio_<Ratio, (-Degree)>>
  >
>;

template<typename T>
using UnitRatio = typename UnitRatio_<typename T::Ratio, T::Dimension::Degree>;

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

template<typename NumT, typename UnitTuple>
class Measurement
{
  using ThisType = Measurement<NumT, UnitTuple>;

  template<typename UnitTupleU>
  using AllowAssignmentAndArithmetic = std::enable_if_t
  <
    SharesDimensions<UnitTuple, UnitTupleU>::value,
    ThisType
  >;

  template<typename UnitTupleU>
  using AllowComparison = std::enable_if_t
  <
    IdenticalDimensions<UnitTuple, UnitTupleU>::value,
    bool
  >;

  template<typename UnitTupleU>
  using TranslationRatio = std::ratio_multiply<OverallRatio<UnitTuple>, OverallRatio<UnitTupleU>>;

public:
  using ValueType = NumT;
  using Units = UnitTuple;

  Measurement() = default;
  Measurement(NumT value) : v(value) {}

  const ValueType& value() const { return this->v; }
  ValueType& value() { return this->v; }

  template<typename UnitTupleU>
  AllowAssignmentAndArithmetic<UnitTupleU>& operator=(const Measurement<NumT, UnitTupleU>& r)
  {
    this->v = r.value()*TranslationRatio<UnitTupleU>::num/TranslationRatio<UnitTupleU>::den;
    this->copyDegrees(r.degrees());

    return *this;
  }

  template<typename UnitTupleU>
  AllowComparison<UnitTupleU> operator<(const Measurement<NumT, UnitTupleU>& other)
  {
    return this->value < other.value;
  }

private:
  ValueType v = 0;
};

template<typename NumT, DegreeType DegreeV = 1>
using Meters = Measurement<NumT, std::tuple<UnitMeters<DegreeV>>>;

}

#endif
