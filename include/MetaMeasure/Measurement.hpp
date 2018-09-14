#ifndef METAMEASURE_MEASUREMENT_INCLUDED
#define METAMEASURE_MEASUREMENT_INCLUDED

#include <MetaMeasure/Utility.hpp>

namespace MetaMeasure
{

template<typename NumT, typename... Units>
class Measurement
{
public:
  using ValueType = NumT;

private:
  using UnitTuple = std::tuple<Units...>;
  using ThisType = Measurement<NumT, Units...>;

  // An example of something that would cause this error
  // Would be specifying inches & meters in the parameter pack.
  // Another example would be specifying seconds and hertz.
  static_assert(Private::HasNoDuplicates<UnitTuple>::Value,
                "Measurement has 2 or more units of the same dimension.");

  template<typename T, typename... OtherUnits>
  using EnableIfIdentical = std::enable_if_t
  <
    Private::IdenticalDimensions<UnitTuple, std::tuple<OtherUnits...>>::value,
    T
  >;

public:
  constexpr Measurement() = default;
  constexpr Measurement(NumT value) : v(value) {}
  constexpr Measurement(ThisType& other) : v(other.v) {}
  constexpr Measurement(ThisType&& other) : v(std::move(other.v)) {}

  // If you error at this constructor,
  // Chances are you tried to copy construct from a measurement without identical dimensions.
  template<typename NumU, typename... OtherUnits>
  constexpr Measurement(const Measurement<NumU, OtherUnits...>& other) : v(this->convert(other)) {}

  constexpr const ValueType& value() const { return this->v; }

  constexpr ThisType& operator=(const ThisType& other)
  {
    this->v = other.value();
    return *this;
  }

  template<typename NumU, typename... OtherUnits>
  constexpr EnableIfIdentical<ThisType, OtherUnits...>& operator=(const Measurement<NumU, OtherUnits...>& other)
  {
    this->v = this->convert(other);
    return *this;
  }

  template<typename... OtherUnits>
  constexpr EnableIfIdentical<bool, OtherUnits...> operator<(const Measurement<NumT, OtherUnits...>& other)
  {
    return this->v < other.value();
  }

  constexpr ThisType operator-()
  {
    return -this->v;
  }

private:
  ValueType v;

  template<typename NumU, typename... OtherUnits>
  constexpr EnableIfIdentical<ValueType, OtherUnits...> convert(const Measurement<NumU, OtherUnits...>& other)
  {
    using ConversionRatio = Private::ConversionRatio<UnitTuple, std::tuple<OtherUnits...>>;
#ifdef METAMEASURE_SUPPRESS_CONVERSION_WARNINGS
    return static_cast<NumT>(other.value())*ConversionRatio::num / ConversionRatio::den;
#else
    return other.value()*ConversionRatio::num / ConversionRatio::den;
#endif
  }
};

}

#endif
