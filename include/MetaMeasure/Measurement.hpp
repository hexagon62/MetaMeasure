#ifndef METAMEASURE_MEASUREMENT_INCLUDED
#define METAMEASURE_MEASUREMENT_INCLUDED

#include <MetaMeasure/Utility.hpp>

namespace MetaMeasure
{

template<typename NumT, typename... Units>
class Measurement;

namespace Private
{

template<typename NumT, typename Tuple>
struct MeasurementThroughTuple_;

template<typename NumT, typename... Ts>
struct MeasurementThroughTuple_<NumT, std::tuple<Ts...>>
{
  using Type = Measurement<NumT, Ts...>;
};

template<typename NumT, typename Tuple>
using MeasurementThroughTuple = typename MeasurementThroughTuple_<NumT, Tuple>::Type;

}

template<typename NumT, typename... Units>
class Measurement
{
public:
  using ValueType = NumT;
  using UnitTuple = Private::RemoveZeroDimensions<std::tuple<Units...>>;

private:
  using ThisType = Measurement<ValueType, Units...>;

  // An example of something that would cause this error
  // Would be specifying inches & meters in the parameter pack.
  // Another example would be specifying seconds and hertz.
  static_assert(Private::HasNoDuplicates<UnitTuple>::Value,
                "Measurement has 2 or more units of the same dimension.");

  // If the measurement has the same units and dimensions, overloads that use this type may be called
  template<typename M, typename T = M>
  using IfIdentical = std::enable_if_t
  <
    Private::IdenticalTypes<UnitTuple, typename M::UnitTuple>::value,
    T
  >;

  // If the measurements have the same dimensions, overloads that use this type may be called
  template<typename M, typename T = M>
  using IfConvertible = std::enable_if_t
  <
    Private::IdenticalDimensions<UnitTuple, typename M::UnitTuple>::value,
    T
  >;

  // If the measurements have the same dimensions, but not the same units
  // Overloads that use this type may be called
  template<typename M, typename T = M>
  using OnlyIfConvertible = std::enable_if_t
  <
    (Private::IdenticalDimensions<UnitTuple, typename M::UnitTuple>::value &&
     !Private::IdenticalTypes<UnitTuple, typename M::UnitTuple>::value),
    T
  >;

  template<typename M>
  using Product = Private::MeasurementThroughTuple<ValueType, Private::MultiplyDimensions<UnitTuple, typename M::UnitTuple>>;

public:

  constexpr Measurement() = default;
  constexpr Measurement(NumT value) : v(value) {}

  template<typename M>
  constexpr Measurement(const IfIdentical<M>& other)
    : v(ThisType::valueOf(other))
  {}

  // If you error at this constructor,
  // Chances are you tried to copy construct from a measurement without identical dimensions.
  template<typename NumU, typename... UnitsU>
  constexpr Measurement(const Measurement<NumU, UnitsU...>& other)
    : v(ThisType::convertedValueOf(other))
  {}

  constexpr const ValueType& value() const { return this->v; }

  // Arithmetic operators
  template<typename M>
  constexpr IfConvertible<M, ThisType> operator+(const M& other)
  {
    return this->v + ThisType::convertedValueOf(other);
  }

  template<typename M>
  constexpr IfConvertible<M, ThisType> operator-(const M& other)
  {
    return this->v - ThisType::convertedValueOf(other);
  }

  template<typename NumU, typename... UnitsU>
  constexpr Product<Measurement<NumU, UnitsU...>> operator*(const Measurement<NumU, UnitsU...>& other)
  {
    return this->v * ThisType::valueOf(other);
  }

  template<typename NumU>
  constexpr ThisType operator*(const NumU& factor)
  {
    return this->v * factor;
  }

  template<typename NumU>
  constexpr ThisType operator/(const NumU& factor)
  {
    return this->v / factor;
  }

  constexpr ThisType operator-()
  {
    return -this->v;
  }

  constexpr ThisType operator+()
  {
    return +this->v;
  }

  // Assignment operators
  template<typename M>
  constexpr IfConvertible<M, ThisType>& operator=(const M& other)
  {
    this->v = ThisType::convertedValueOf(other);
    return *this;
  }

  template<typename M>
  constexpr IfConvertible<M, ThisType>& operator+=(const M& other)
  {
    this->v += ThisType::convertedValueOf(other);
    return *this;
  }

  template<typename M>
  constexpr IfConvertible<M, ThisType>& operator-=(const M& other)
  {
    this->v -= ThisType::convertedValueOf(other);
    return *this;
  }

  template<typename NumU>
  constexpr ThisType& operator*=(const NumU& factor)
  {
    this->v *= factor;
    return *this;
  }

  template<typename NumU>
  constexpr ThisType& operator/=(const NumU& factor)
  {
    this->v *= factor;
    return *this;
  }

  // Comparison operators
  template<typename M>
  constexpr IfConvertible<M, bool> operator<(const M& other)
  {
    return this->v < ThisType::convertedValueOf(other);
  }

  template<typename M>
  constexpr IfConvertible<M, bool> operator>(const M& other)
  {
    return this->v > ThisType::convertedValueOf(other);
  }

  template<typename M>
  constexpr IfConvertible<M, bool> operator<=(const M& other)
  {
    return this->v <= ThisType::convertedValueOf(other);
  }

  template<typename M>
  constexpr IfConvertible<M, bool> operator>=(const M& other)
  {
    return this->v >= ThisType::convertedValueOf(other);
  }

  template<typename M>
  constexpr IfConvertible<M, bool> operator==(const M& other)
  {
    return this->v < ThisType::valueOf(other);
  }

  template<typename M>
  constexpr IfConvertible<M, bool> operator!=(const M& other)
  {
    return this->v < ThisType::valueOf(other);
  }

private:
  ValueType v;

  // This is so I don't have to check if METAMEASURE_SUPPRESS_CONVERSION_WARNINGS is defined more than I need to
  template<typename M>
  static constexpr ValueType valueOf(const M& other)
  {
#ifdef METAMEASURE_SUPPRESS_CONVERSION_WARNINGS
    return static_cast<ValueType>(other.value());
#else
    return other.value();
#endif
  }

  template<typename M>
  static constexpr IfConvertible<M, ValueType> convertedValueOf(const M& other)
  {
    using ConversionRatio = Private::ConversionRatio<UnitTuple, M::UnitTuple>;
    return ThisType::valueOf(other)*ConversionRatio::num / ConversionRatio::den;
  }
};

}

#endif
