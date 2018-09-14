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

  template<typename... OtherUnits>
  using AllowAssignment = std::enable_if_t
  <
    Private::SharesDimensions<UnitTuple, std::tuple<OtherUnits...>>::value,
    ThisType
  >;

  template<typename... OtherUnits>
  using AllowConversion = std::enable_if_t
  <
    Private::SharesDimensions<UnitTuple, std::tuple<OtherUnits...>>::value,
    ValueType
  >;

  template<typename... OtherUnits>
  using AllowComparison = std::enable_if_t
  <
    Private::IdenticalDimensions<UnitTuple, std::tuple<OtherUnits...>>::value,
    bool
  >;

public:
  constexpr Measurement() = default;
  constexpr Measurement(NumT value) : v(value) {}
  constexpr Measurement(ThisType& other) : v(other.v) {}
  constexpr Measurement(ThisType&& other) : v(std::move(other.v)) {}

  template<typename NumU, typename... OtherUnits>
  constexpr Measurement(const Measurement<NumU, OtherUnits...>& other)
    : v(this->convert(other))
  {

  }

  constexpr const ValueType& value() const { return this->v; }

  constexpr ThisType& operator=(const ThisType& other)
  {
    this->v = other.value();
    return *this;
  }

  template<typename NumU, typename... OtherUnits>
  constexpr AllowAssignment<OtherUnits...>& operator=(const Measurement<NumU, OtherUnits...>& other)
  {
    this->v = this->convert(other);
    return *this;
  }

  template<typename... OtherUnits>
  constexpr AllowComparison<OtherUnits...> operator<(const Measurement<NumT, OtherUnits...>& other)
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
  AllowConversion<OtherUnits...> convert(const Measurement<NumU, OtherUnits...>& other)
  {
    using ConversionRatio = Private::ConversionRatio<UnitTuple, std::tuple<OtherUnits...>>;
#ifdef METAMEASURE_SUPPRESS_CONVERSION_WARNINGS
    return static_cast<NumT>(other.value())*ConversionRatio::den / ConversionRatio::num;
#else
    return other.value()*ConversionRatio::den / ConversionRatio::num;
#endif
  }
};

}

#endif
