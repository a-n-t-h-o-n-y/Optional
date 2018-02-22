#ifndef OPTIONAL_FREE_FUNCTIONS_HPP
#define OPTIONAL_FREE_FUNCTIONS_HPP
#include <optional/optional_value.hpp>

namespace opt {

/// T must have operator== defined.
/// \returns If both x and y are initialized, (*x == *y).
/// \returns If only x _or_ y is initialized, false.
/// \returns If both are uninitialized, true.
template <typename T>
bool operator==(const Optional<T>& x, const Optional<T>& y) {
    if (x && y) {
        return *x == *y;
    }
    return !x && !y;
}

/// \returns !(x == y).
template <typename T>
bool operator!=(const Optional<T>& x, const Optional<T>& y) {
    return !(x == y);
}

/// T must have operator< defined.
/// \returns If both are initialized, *x < *y.
/// \returns If y is empty, false.
/// \returns If x and y are both empty, true.
template <typename T>
bool operator<(const Optional<T>& x, const Optional<T>& y) {
    if (!y) {
        return false;
    }
    if (!x) {
        return true;
    }
    return *x < *y;
}

/// \returns y < x
template <typename T>
bool operator>(const Optional<T>& x, const Optional<T>& y) {
    return (y < x);
}

/// \returns !(y < x)
template <typename T>
bool operator<=(const Optional<T>& x, const Optional<T>& y) {
    return !(y < x);
}

/// \returns !(x < y)
template <typename T>
bool operator>=(const Optional<T>& x, const Optional<T>& y) {
    return !(x < y);
}

/// \returns !x
template <typename T>
bool operator==(const Optional<T>& x, None_t) noexcept {
    return !x;
}

/// \returns !x
template <typename T>
bool operator==(None_t, const Optional<T>& x) noexcept {
    return !x;
}

/// \returns True if x is initialized, false if x is empty.
template <typename T>
bool operator!=(const Optional<T>& x, None_t) noexcept {
    return bool(x);
}

/// \returns True if x is initialized, false if x is empty.
template <typename T>
bool operator!=(None_t, const Optional<T>& x) noexcept {
    return bool(x);
}

/// \param opt  An Optional to extract the value from.
/// \returns Reference to the underlying object
template <typename T>
const T& get(const Optional<T>& opt) {
    return opt.get();
}

/// \param opt  An Optional to extract the value from.
/// \returns Reference to the underlying object
template <typename T>
T& get(Optional<T>& opt) {
    return opt.get();
}

/// \param opt  A pointer to Optional to extract the value from.
/// \returns Pointer to the underlying object
template <typename T>
const T* get(const Optional<T>* opt) {
    return opt->get_ptr();
}

/// \param opt  A pointer to Optional to extract the value from.
/// \returns Pointer to the underlying object
template <typename T>
T* get(Optional<T>* opt) {
    return opt->get_ptr();
}

/// \param opt  Optional object to get underlying object's pointer from.
/// \returns Pointer to the underlying object.
template <typename T>
const T* get_pointer(const Optional<T>& opt) {
    return opt.get_ptr();
}

/// \param opt  Optional object to get underlying object's pointer from.
/// \returns Pointer to the underlying object.
template <typename T>
T* get_pointer(Optional<T>& opt) {
    return opt.get_ptr();
}

}  // namespace opt
#endif  // OPTIONAL_FREE_FUNCTIONS_HPP
