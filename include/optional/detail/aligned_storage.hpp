#ifndef DETAIL_ALIGNED_STORAGE_HPP
#define DETAIL_ALIGNED_STORAGE_HPP
#include <cstddef>

namespace opt {
namespace detail {

template <typename T, std::size_t Align = alignof(T)>
class Aligned_storage {
   public:
    // Return a T pointer to the underlying data.
    const T* ptr_ref() const { return static_cast<const T*>(this->address()); }
    T* ptr_ref() { return static_cast<T*>(this->address()); }

    // Return a reference to the underlying data.
    const T& ref() const { return *this->ptr_ref(); }
    T& ref() { return *this->ptr_ref(); }

    // Return a void pointer to the underlying data.
    const void* address() const { return static_cast<const void*>(data_); }
    void* address() { return static_cast<void*>(data_); }

   private:
    alignas(Align) unsigned char data_[sizeof(T)];
};

}  // namespace detail
}  // namespace opt
#endif  // DETAIL_ALIGNED_STORAGE_HPP
