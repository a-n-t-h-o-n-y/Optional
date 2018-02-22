#ifndef OPTIONAL_VOID_HPP
#define OPTIONAL_VOID_HPP
#include <optional/optional_fwd.hpp>

namespace opt {

/// Optional<void> specialization.
template <>
class Optional<void> {
   public:
    explicit operator bool() const noexcept { return false; }
    bool operator!() const noexcept { return true; }
};

}  // namespace opt
#endif  // OPTIONAL_VOID_HPP
