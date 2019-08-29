#ifndef OPTIONAL_VALUE_HPP
#define OPTIONAL_VALUE_HPP
#include <new>
#include <type_traits>
#include <utility>

#include <optional/bad_optional_access.hpp>
#include <optional/detail/aligned_storage.hpp>
#include <optional/detail/conjunction.hpp>
#include <optional/none.hpp>

namespace opt {

/// \brief Wraps a type to provide an optional 'null', or empty state.
///
/// A wrapped object is accessed by dereferencing an Optional object. Optional
/// objects can be tested as a boolean for whether or not they contain a value.
/// Useful when 0, -1, or default constructed does not suffice for 'no value'.
///
/// Typical usage:
/// \code
/// Optional<int> opt_i{5};
/// if(opt_i) {
///     foo(*opt_i);
/// }
/// \endcode
template <typename T>
class Optional {
    // Type Traits for noexcept expressions
    template <typename X>
    static constexpr auto is_nt_d_cc_ca() -> bool
    {
        return detail::Conjunction<std::is_nothrow_destructible<X>,
                                   std::is_nothrow_copy_constructible<X>,
                                   std::is_nothrow_copy_assignable<X>>::value;
    }

    template <typename X>
    static constexpr auto is_nt_d_mc_ma() -> bool
    {
        return detail::Conjunction<std::is_nothrow_destructible<X>,
                                   std::is_nothrow_move_constructible<X>,
                                   std::is_nothrow_move_assignable<X>>::value;
    }

    template <typename X, typename Y>
    static constexpr auto is_nt_d_cc_a() -> bool
    {
        return detail::Conjunction<
            std::is_nothrow_destructible<X>,
            std::is_nothrow_copy_constructible<X>,
            std::is_nothrow_assignable<X, const Y&>>::value;
    }

    template <typename X, typename Y>
    static constexpr auto is_nt_d_mc_a() -> bool
    {
        return detail::Conjunction<std::is_nothrow_destructible<X>,
                                   std::is_nothrow_move_constructible<X>,
                                   std::is_nothrow_assignable<X, Y&&>>::value;
    }

    template <typename X, typename... Args>
    static constexpr auto is_nt_d_c() -> bool
    {
        return detail::Conjunction<
            std::is_nothrow_destructible<X>,
            std::is_nothrow_constructible<X, Args...>>::value;
    }

    template <typename X>
    static constexpr auto is_nt_cc() -> bool
    {
        return std::is_nothrow_copy_constructible<X>::value;
    }

    template <typename X>
    static constexpr auto is_nt_mc() -> bool
    {
        return std::is_nothrow_move_constructible<X>::value;
    }

    template <typename X>
    static constexpr auto is_nt_d() -> bool
    {
        return std::is_nothrow_destructible<X>::value;
    }

   public:
    using Value_type = T;

    /// \brief Default constructs an Optional.
    ///
    /// *this is _not_ initialized, T's default constructor is _not_ called.
    Optional() noexcept : initialized_{false} {}

    /// \brief Constructs an uninitialized Optional.
    ///
    /// *this is _not_ initialized, T's default constrcutor is _not_ called.
    /// \param n    Use opt::none provided in none.hpp.
    /// \sa none
    Optional(opt::None_t) noexcept : initialized_{false} {}

    /// \brief Constructs an initialized Optional from a T object.
    ///
    /// *this is initialized with a copy of \p value.
    /// \param value    Value which is copied into the Optional.
    Optional(const T& value) noexcept(is_nt_cc<T>()) { this->construct(value); }

    /// \brief Constructs an initialized Optional from a moveable T object.
    ///
    /// \p value is move constructed into the Optional object.
    /// \param value    Value which is moved into the Optional.
    Optional(T&& value) noexcept(is_nt_mc<T>())
    {
        this->construct(std::move(value));
    }

    /// \brief Conditionally constructs an initialized Optional.
    ///
    /// If condition is true, \p value is copy constructed into *this, otherwise
    /// *this is not initialized.
    /// \param condition    Determines if Optional is initialized or not.
    /// \param value        Object copied into *this if \p condition is true.
    Optional(bool condition, const T& value) noexcept(is_nt_cc<T>())
    {
        if (condition)
            this->construct(value);
    }

    /// \brief Conditionally constructs an Optional from a moveable T object.
    ///
    /// If condition is true, \p value is move constructed into *this, otherwise
    /// *this is not initialized.
    /// \param condition    Determines if Optional is initialized or not.
    /// \param value        Object moved into *this if \p condition is true.
    Optional(bool condition, T&& value) noexcept(is_nt_mc<T>())
    {
        if (condition)
            this->construct(std::move(value));
    }

    /// Destructs the underlying object
    ~Optional() noexcept(is_nt_d<T>()) { this->destroy(); }

    /// \brief Copy constructs an Optional.
    ///
    /// If \p rhs is initialized, then a copy of its value will be constructed
    /// within *this, and *this will be initialized, otherwise *this is default
    /// constructed.
    /// \param rhs  Optional to copy, T must be copy constructible.
    Optional(const Optional& rhs) noexcept(is_nt_cc<T>())
    {
        if (rhs.initialized_)
            this->construct(rhs.get());
    }

    /// \brief Move constructs an Optional.
    ///
    /// If \p rhs is initialized, then its internally held value is moved into
    /// *this, and \p rhs will be empty, otherwise *this is default constructed.
    /// \param rhs  Optional to move, T must be move constructible.
    Optional(Optional&& rhs) noexcept(is_nt_mc<T>())
    {
        if (rhs.initialized_) {
            this->construct(std::move(rhs.get()));
            rhs.initialized_ = false;
        }
    }

    /// \brief Copy constructs from implicitly convertible type.
    ///
    /// If \p rhs is initialized and U is implicitly convertible to T, then
    /// *this is initialized with a copy of \p rhs.
    /// \param rhs  Object to be copied into *this.
    template <typename U>
    explicit Optional(const Optional<U>& rhs) noexcept(
        std::is_nothrow_constructible<T, const U&>::value)
    {
        if (rhs.initialized_)
            this->construct(rhs.get());
    }

    /// \brief Move constructs from implicitly convertible type.
    ///
    /// If \p rhs is initialized and U is implicitly convertible to T, then
    /// *this is initialized with \p rhs. T must have a move constructor defined
    /// from type U.
    /// \param rhs  Object to be moved into *this.
    template <typename U>
    explicit Optional(Optional<U>&& rhs) noexcept(
        std::is_nothrow_constructible<T, U&&>::value)
    {
        if (rhs.initialized_) {
            this->construct(std::move(rhs.get()));
            rhs.initialized_ = false;
        }
    }

    /// \brief Copy assignment operator.
    ///
    /// The object held by *this is destroyed and replaced with the contents of
    /// \p rhs. If \p rhs is empty, *this is left empty.
    /// \param rhs  Object to be copied into *this.
    auto operator=(const Optional& rhs) noexcept(is_nt_d_cc_ca<T>())
        -> Optional&
    {
        if (this->is_initialized() && rhs.is_initialized())
            this->get() = rhs.get();
        else if (!this->is_initialized() && rhs.is_initialized())
            this->construct(rhs.get());
        else if (!rhs.is_initialized())
            this->destroy();
        return *this;
    }

    /// \brief Move assignement operator.
    ///
    /// The object held by *this is destroyed and replaced with the contents of
    /// \p rhs. If \p rhs is empty, *this is left empty. rhs is left in an
    /// invalid state.
    /// \param rhs  Object to be moved into *this.
    auto operator=(Optional&& rhs) noexcept(is_nt_d_mc_ma<T>()) -> Optional&
    {
        if (this->is_initialized() && rhs.is_initialized())
            this->get() = std::move(rhs.get());
        else if (!this->is_initialized() && rhs.is_initialized())
            this->construct(std::move(rhs.get()));
        else if (!rhs.is_initialized())
            this->destroy();
        rhs.initialized_ = false;
        return *this;
    }

    /// \brief Converting copy assignment operator.
    ///
    /// If *this is initialized, the object held is destroyed and replaced with
    /// a copy of \p rhs. U must be implicitly convertible to T.
    /// \param rhs  Optional to be copied to *this.
    template <typename U>
    auto operator=(const Optional<U>& rhs) noexcept(is_nt_d_cc_a<T, U>())
        -> Optional&
    {
        if (this->is_initialized() && rhs.is_initialized())
            this->get() = rhs.get();
        else if (!this->is_initialized() && rhs.is_initialized())
            this->construct(rhs.get());
        else if (!rhs.is_initialized())
            this->destroy();
        return *this;
    }

    /// \brief Converting move assignment operator.
    ///
    /// If *this is initialized, the object held is destroyed and replace with
    /// \p rhs. T must have a move constructor from type U.
    /// \param rhs  Optional to be moved to *this.
    template <typename U>
    auto operator=(Optional<U>&& rhs) noexcept(is_nt_d_mc_a<T, U>())
        -> Optional&
    {
        if (this->is_initialized() && rhs.is_initialized())
            this->get() = std::move(rhs.get());
        else if (!this->is_initialized() && rhs.is_initialized())
            this->construct(std::move(rhs.get()));
        else if (!rhs.is_initialized())
            this->destroy();
        rhs.initialized_ = false;
        return *this;
    }

    /// \brief Value copy assignment operator.
    ///
    /// If *this is initialized, the object held is destroyed and replaced
    /// with a copy of \p value. T must be copy constructible. \param value
    /// Value to be copied into *this.
    auto operator=(const T& value) noexcept(is_nt_d_cc_ca<T>()) -> Optional&
    {
        if (this->is_initialized())
            this->get() = value;
        else
            this->construct(value);
        return *this;
    }

    /// \brief Value move assignment operator.
    ///
    /// If *this is initialized, the object held is destroyed and replaced
    /// with \p value. T must be move constructible. \param value    Value
    /// to be moved into *this.
    auto operator=(T&& value) noexcept(is_nt_d_mc_ma<T>()) -> Optional&
    {
        if (this->is_initialized())
            this->get() = std::move(value);
        else
            this->construct(std::move(value));
        return *this;
    }

    /// \brief None_t assignement operator.
    ///
    /// Leaves *this in an uninitialized state. If *this was previously
    /// initialized, the held object is destroyed.
    /// \param n    Use opt::none provided in none.hpp.
    /// \sa none
    auto operator=(opt::None_t) noexcept(is_nt_d<T>()) -> Optional&
    {
        this->destroy();
        return *this;
    }

    /// \brief Directly construct a value inside of an existing Optional.
    ///
    /// If *this is initialized, the held object is destroyed. The \p args
    /// are forwarded to the constructor of T.
    template <typename... Args>
    auto emplace(Args&&... args) noexcept(is_nt_d_c<T, Args...>()) -> void
    {
        this->destroy();
        this->emplace_construct(std::forward<Args>(args)...);
    }

    /// \brief Return a reference to the held value.
    ///
    /// Undefined if *this is uninitialized.
    /// \returns const reference to the underlying object.
    auto get() const -> const T& { return storage_.ref(); }

    /// \brief Return a reference to the held value.
    ///
    /// Undefined if *this is uninitialized.
    /// \returns Reference to the underlying object.
    auto get() -> T& { return storage_.ref(); }

    /// \brief Member access overload to underlying object.
    ///
    /// Undefined if *this is uninitialized.
    /// \returns const pointer to the underlying object.
    auto operator-> () const -> const T* { return storage_.ptr_ref(); }

    /// \brief Member access overload to underlying object.
    ///
    /// Undefined if *this is uninitialized.
    /// \returns Pointer to the underlying object.
    auto operator-> () -> T* { return storage_.ptr_ref(); }

    /// \brief Provides direct access to the underlying object.
    ///
    /// Undefined if *this is uninitialized. Overloaded on const &.
    /// \returns const l-value reference to the held object.
    auto operator*() const& -> const T& { return storage_.ref(); }

    /// \brief Provides direct access to the underlying object.
    ///
    /// Undefined if *this is uninitialized. Overloaded on &.
    /// \returns l-value reference to the held object.
    auto operator*() & -> T& { return storage_.ref(); }

    /// \brief Provides direct access to the underlying object.
    ///
    /// Undefined if *this is uninitialized. Overloaded on &&.
    /// \returns r-value reference to the underlying object
    auto operator*() && -> T&& { return std::move(storage_.ref()); }

    /// \brief Direct access to the underlying object, or throw exception.
    ///
    /// Throws Bad_optional_access if *this is uninitialized. Overloaded on
    /// const &.
    /// \returns const l-value reference to the underlying object.
    auto value() const& -> const T&
    {
        if (initialized_)
            return storage_.ref();
        throw Bad_optional_access();
    }

    /// \brief Direct access to the underlying object, or throw exception.
    ///
    /// Throws Bad_optional_access if *this is uninitialized. Overloaded on
    /// &.
    /// \returns l-value reference to the underlying object.
    auto value() & -> T&
    {
        if (initialized_)
            return storage_.ref();
        throw Bad_optional_access();
    }

    /// \brief Direct access to the underlying object, or throw exception.
    ///
    /// Throws Bad_optional_access if *this is uninitialized. Overloaded on
    /// &&.
    /// \returns r-value reference to the underlying object.
    auto value() && -> T&&
    {
        if (initialized_)
            return std::move(storage_.ref());
        throw Bad_optional_access();
    }

    /// \brief Direct access to the underlying object, or return \p val.
    /// Return \p val if *this is uninitialized, otherwise returns a
    /// reference
    /// to the value stored in *this. Overloaded on const &.
    /// \param val Value to be returned if *this is uninitialized.
    /// \returns Either the value stored in *this, or \p val.
    template <typename U>
    auto value_or(U&& val) const& -> T
    {
        if (initialized_)
            return storage_.ref();
        return val;
    }

    /// \brief Direct access to the underlying object, or return \p val.
    /// Return \p val if *this is uninitialized, otherwise returns an
    /// r-value
    /// reference to the value stored in *this. Overloaded on &&.
    /// \param val Value to be returned if *this is uninitialized.
    /// \returns Either the value stored in *this, or \p val.
    template <typename U>
    auto value_or(U&& val) && -> T
    {
        if (initialized_) {
            initialized_ = false;
            return std::move(storage_.ref());
        }
        return val;
    }

    /// \brief Direct access to the underlying object, or evaluate \p func
    /// and return its value.
    ///
    /// If *this is uninitialized, \p func is evaluated and it's value
    /// returned. Otherwise returns a reference to the underlying object in
    /// *this. Overloaded on const &.
    /// \param func    Function with signature T func().
    /// \returns The value stored in *this, or the result of \p func().
    template <typename F>
    auto value_or_eval(F f) const& -> T
    {
        if (initialized_)
            return storage_.ref();
        return f();
    }

    /// \brief Direct access to the underlying object, or evaluate \p func
    /// and return its value.
    ///
    /// If *this is uninitialized, \p func is evaluated and it's value
    /// returned. Otherwise returns an r-value reference to the underlying
    /// object in *this. Overloaded on &&.
    /// \param func    Function with signature T func().
    /// \returns The value stored in *this, or the result of \p func().
    template <typename F>
    auto value_or_eval(F f) && -> T
    {
        if (initialized_) {
            initialized_ = false;
            return std::move(storage_.ref());
        }
        return f();
    }

    /// \brief Access to the underlying object's pointer.
    ///
    /// *this still owns the object, do not delete the object via this
    /// pointer. \returns const pointer to the underlying object.
    auto get_ptr() const -> const T* { return storage_.ptr_ref(); }

    /// \brief Access to the underlying object's pointer.
    ///
    /// *this still owns the object, do not delete the object via this
    /// pointer. \returns Pointer to the underlying object.
    auto get_ptr() -> T* { return storage_.ptr_ref(); }

    /// \brief Safe conversion to bool.
    /// \returns True if object contains a value, false otherwise.
    explicit operator bool() const noexcept { return this->is_initialized(); }

    /// \brief Convinience function
    ///
    /// Explicit operator bool makes ! operation verbose with !bool(opt).
    /// \returns Opposite of operator bool.
    bool operator!() const noexcept { return !this->is_initialized(); }

    template <typename U>
    friend class Optional;

   private:
    bool initialized_{false};
    opt::detail::Aligned_storage<T> storage_;

    auto is_initialized() const -> bool { return initialized_; }

    auto construct(const T& value) -> void
    {
        ::new (storage_.address()) T(value);
        initialized_ = true;
    }

    auto construct(T&& value) -> void
    {
        ::new (storage_.address()) T(std::move(value));
        initialized_ = true;
    }

    template <typename... Args>
    auto emplace_construct(Args&&... args) -> void
    {
        ::new (storage_.address()) T(std::forward<Args>(args)...);
        initialized_ = true;
    }

    auto destroy() -> void
    {
        if (initialized_) {
            storage_.ptr_ref()->~T();
            initialized_ = false;
        }
    }
};

}  // namespace opt
#endif  // OPTIONAL_VALUE_HPP
