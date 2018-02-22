#include <gtest/gtest.h>

#include <optional/bad_optional_access.hpp>
#include <optional/none.hpp>
#include <optional/optional_reference.hpp>

using opt::Optional;

namespace {
struct Base {
    int foo() const { return 1; }
};
struct Derived : Base {};
}  // namespace

TEST(OptionalReferenceTest, DefaultConstructor) {
    Optional<int&> oi;
    EXPECT_FALSE(oi);
    EXPECT_TRUE(!oi);
}

TEST(OptionalReferenceTest, NoneTypeConstructor) {
    Optional<char&> oc{opt::none};
    EXPECT_FALSE(oc);
    EXPECT_TRUE(!oc);
}

TEST(OptionalReferenceTest, ReferenceConstructor) {
    int i{5};
    int& ir{i};
    Optional<int&> oi1{ir};
    Optional<int&> oi2{i};

    ASSERT_TRUE(oi1);
    ASSERT_TRUE(oi2);
    EXPECT_EQ(&*oi1, &*oi2);
    EXPECT_EQ(5, *oi1);
    EXPECT_EQ(5, *oi2);

    Base b;
    Derived d;
    Optional<Base&> ob1{b};
    Optional<Base&> ob2{d};

    EXPECT_TRUE(ob1);
    EXPECT_TRUE(ob2);
}

TEST(OptionalReferenceTest, ConditionalReferenceConstructor) {
    int i{5};
    int& ir{i};
    Optional<int&> oi1{true, ir};
    Optional<int&> oi2{false, i};

    EXPECT_FALSE(oi2);
    ASSERT_TRUE(oi1);
    EXPECT_EQ(5, *oi1);
}

TEST(OptionalReferenceTest, CopyConstructor) {
    int i{1};
    Optional<int&> oi1;
    Optional<int&> oi2{oi1};
    Optional<int&> oi3{i};
    Optional<int&> oi4{oi3};

    EXPECT_FALSE(oi2);
    ASSERT_TRUE(oi3);
    ASSERT_TRUE(oi4);
    EXPECT_EQ(&*oi3, &*oi4);

    Derived d;
    Optional<Derived&> od{d};
    Optional<Base&> ob{od};

    ASSERT_TRUE(od);
    ASSERT_TRUE(ob);
    EXPECT_EQ(&*od, &*ob);
}

TEST(OptionalReferenceTest, NoneTypeAssignmentOperator) {
    int i{5};
    Optional<int&> oi{i};
    EXPECT_TRUE(oi);
    oi = opt::none;
    EXPECT_FALSE(oi);
}

TEST(OptionalReferenceTest, CopyAssignmentOperator) {
    int i1{5};
    int i2{7};
    Optional<int&> oi1{i1};
    Optional<int&> oi2{i2};
    Optional<int&> oi3;
    EXPECT_TRUE(oi1);
    EXPECT_TRUE(oi2);
    EXPECT_FALSE(oi3);

    oi1 = oi2;
    ASSERT_TRUE(oi1);
    ASSERT_TRUE(oi2);
    EXPECT_EQ(&*oi1, &i2);
    EXPECT_EQ(&*oi2, &i2);

    oi2 = oi3;
    EXPECT_FALSE(oi2);
    EXPECT_FALSE(oi3);

    Derived d;
    Optional<Derived&> od{d};
    Optional<Derived&> od_empty;
    Optional<Base&> ob;

    EXPECT_FALSE(ob);
    ob = od;
    ASSERT_TRUE(ob);
    ASSERT_TRUE(od);
    EXPECT_EQ(&*ob, &*od);

    ob = od_empty;
    EXPECT_FALSE(ob);

    Derived d2;
    ob = d2;
    ASSERT_TRUE(ob);
    EXPECT_EQ(&*ob, &d2);

    int i3{3};
    oi1 = i3;
    ASSERT_TRUE(oi1);
    EXPECT_EQ(&*oi1, &i3);
}

TEST(OptionalReferenceTest, Emplace) {
    int i1{2};
    int i2{4};
    Optional<int&> oi1;
    Optional<int&> oi2{i1};

    oi1.emplace(i1);
    ASSERT_TRUE(oi1);
    EXPECT_EQ(&*oi1, &i1);

    oi2.emplace(i2);
    ASSERT_TRUE(oi1);
    EXPECT_EQ(&*oi2, &i2);
}

TEST(OptionalReferenceTest, Access) {
    int i{5};
    Optional<int&> io{i};
    ASSERT_TRUE(io);
    EXPECT_EQ(5, io.get());

    io.get() = 7;
    ASSERT_TRUE(io);
    EXPECT_EQ(7, *io);

    *io = 2;
    ASSERT_TRUE(io);
    EXPECT_EQ(2, io.get());

    Base b;
    Optional<Base&> ob{b};
    ASSERT_TRUE(ob);
    EXPECT_EQ(1, ob->foo());
}

TEST(OptionalReferenceTest, ConstAccess) {
    const int i{5};
    Optional<const int&> io{i};
    ASSERT_TRUE(io);
    EXPECT_EQ(5, io.get());
    EXPECT_EQ(5, *io);

    Base b;
    Optional<const Base&> ob{b};
    ASSERT_TRUE(ob);
    EXPECT_EQ(1, ob->foo());

    int i2{5};
    const Optional<int&> io2{i2};
    ASSERT_TRUE(io2);
    EXPECT_EQ(5, io2.get());
    io2.get() = 7;
    ASSERT_TRUE(io2);
    EXPECT_EQ(7, *io2);
    *io2 = 2;
    ASSERT_TRUE(io2);
    EXPECT_EQ(2, io2.get());

    Base b2;
    const Optional<Base&> ob2{b2};
    ASSERT_TRUE(ob2);
    EXPECT_EQ(1, ob2->foo());
}

TEST(OptionalReferenceTest, ValueMembers) {
    int i{5};
    Optional<int&> oi{i};
    Optional<int&> oi_empty;

    ASSERT_TRUE(oi);
    ASSERT_FALSE(oi_empty);
    EXPECT_EQ(5, oi.value());

    EXPECT_THROW(oi_empty.value(), opt::Bad_optional_access);

    int i2{1};
    EXPECT_EQ(5, oi.value_or(i2));
    EXPECT_EQ(1, oi_empty.value_or(i2));

    EXPECT_EQ(5, oi.value_or_eval([&i2]() -> int& { return i2; }));
    EXPECT_EQ(1, oi_empty.value_or_eval([&i2]() -> int& { return i2; }));
}

TEST(OptionalReferenceTest, GetPtr) {
    int i{5};
    Optional<int&> oi{i};
    Optional<int&> oi_empty;

    ASSERT_TRUE(oi);
    ASSERT_FALSE(oi_empty);
    EXPECT_EQ(&i, oi.get_ptr());
    EXPECT_EQ(nullptr, oi_empty.get_ptr());
}

TEST(OptionalReferenceTest, BoolConversion) {
    int i{5};
    Optional<int&> oi{i};
    Optional<int&> oi_empty;

    EXPECT_TRUE(oi);
    EXPECT_FALSE(oi_empty);

    EXPECT_FALSE(!oi);
    EXPECT_TRUE(!oi_empty);
}
