#ifndef OPTIONAL_VOID_HPP
#define OPTIONAL_VOID_HPP
#include <optional/optional_fwd.hpp>

namespace opt {

/// Optional<void> specialization.
template <>
class Optional<void> {
   public:
    bool is_initialized() const { return false; }
    explicit operator bool() const noexcept { return this->is_initialized(); }
    bool operator!() const noexcept { return !this->is_initialized(); }
};

}  // namespace opt
#endif  // OPTIONAL_VOID_HPP
