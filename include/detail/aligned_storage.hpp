#ifndef ALIGNED_STORAGE_HPP
#define ALIGNED_STORAGE_HPP

#include <new>

namespace opt {
namespace detail {

template <typename T>
class Aligned_storage {
   public:
    // Aligned_storage() = default;

    // Aligned_storage(const T& value) { ::new (this->address()) T(value); }

    const T* ptr_ref() const { return static_cast<const T*>(this->address()); }

    T* ptr_ref() { return static_cast<T*>(this->address()); }

    const T& ref() const { return *this->ptr_ref(); }

    T& ref() { return *this->ptr_ref(); }

    const void* address() const { return data_; }

    void* address() { return data_; }

   private:
    char data_[sizeof(T)];
};

}  // namespace detail
}  // namespace opt
#endif  // ALIGNED_STORAGE_HPP
