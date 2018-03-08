#ifndef OPTIONAL_DETAIL_CONJUNCTION_HPP
#define OPTIONAL_DETAIL_CONJUNCTION_HPP
#include <type_traits>

namespace opt {
namespace detail {

// A C++11 implementation of C++17 std::conjunction
// Empty Conjunction is true.
template <typename...>
struct Conjunction : std::true_type {};

// Single argument Conjunction.
template <typename B1>
struct Conjunction<B1> : B1 {};

// Multi-argument case.
template <typename B1, typename... Bn>
struct Conjunction<B1, Bn...>
    : std::conditional_t<bool(B1::value), Conjunction<Bn...>, B1> {};

}  // namespace detail
}  // namespace opt

#endif  // OPTIONAL_DETAIL_CONJUNCTION_HPP
