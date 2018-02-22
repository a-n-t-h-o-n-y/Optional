#ifndef OPTIONAL_REFERENCE_HPP
#define OPTIONAL_REFERENCE_HPP
#include <type_traits>
#include <utility>

#include <optional/bad_optional_access.hpp>
#include <optional/none.hpp>
#include <optional/optional_fwd.hpp>

namespace opt {

/// Reference Specialization
template <typename T>
class Optional<T&> {
   private:
    template <typename Reference_type, typename Templated_type>
    using If_compatible = typename std::enable_if<
        std::is_lvalue_reference<Reference_type>::value &&
            std::is_convertible<
                typename std::add_pointer<Reference_type>::type,
                typename std::add_pointer<Templated_type>::type>::value,
        int>::type;

   public:
    Optional() noexcept = default;

    explicit Optional(opt::None_t) noexcept {}

    // L-value Reference Constructor
    template <typename R, If_compatible<R, T> = 0>
    Optional(R&& ref) noexcept {
        this->construct(std::forward<R>(ref));
    };

    template <typename R, If_compatible<R, T> = 0>
    Optional(bool condition, R&& ref) noexcept {
        if (condition) {
            this->construct(std::forward<R>(ref));
        }
    }

    Optional(const Optional& rhs) noexcept = default;

    template <typename U>
    explicit Optional(const Optional<U&>& rhs) noexcept {
        if (rhs) {
            this->construct(*rhs);
        }
    }

    Optional& operator=(opt::None_t) noexcept {
        if (initialized_) {
            this->destroy();
        }
        return *this;
    }

    Optional& operator=(const Optional& rhs) noexcept {
        if (this != &rhs) {
            if (rhs) {
                this->construct(*rhs);
            } else {
                this->destroy();
            }
        }
        return *this;
    }

    template <typename U>
    Optional& operator=(const Optional<U&>& rhs) noexcept {
        if (rhs) {
            this->construct(*rhs);
        } else {
            this->destroy();
        }
        return *this;
    }

    template <typename R, If_compatible<R, T> = 0>
    Optional& operator=(R&& r) noexcept {
        this->construct(r);
        return *this;
    }

    ~Optional() noexcept = default;

    template <typename R, If_compatible<R, T> = 0>
    void emplace(R&& args) noexcept {
        this->construct(args);
    }

    T& get() const { return *ref_; }

    T& operator*() const { return this->get(); }

    T* operator->() const { return ref_; }

    T& value() const {
        if (!*this) {
            throw opt::Bad_optional_access();
        }
        return this->get();
    }

    template <typename R, If_compatible<R, T> = 0>
    T& value_or(R&& value) const noexcept {
        if (!*this) {
            return value;
        }
        return this->get();
    }

    template <typename F>
    T& value_or_eval(F f) const {
        if (!*this) {
            return f();
        }
        return this->get();
    }

    T* get_ptr() const noexcept { return ref_; }

    explicit operator bool() const noexcept { return initialized_; }

    bool operator!() const noexcept { return !initialized_; }

   private:
    bool initialized_{false};
    T* ref_{nullptr};

    template <typename R>
    void construct(R&& value) {
        ref_ = &value;
        initialized_ = true;
    }

    void destroy() {
        if (initialized_) {
            ref_ = nullptr;
            initialized_ = false;
        }
    }
};

}  // namespace opt
#endif  // OPTIONAL_REFERENCE_HPP
