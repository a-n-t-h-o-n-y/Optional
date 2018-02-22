#include <gtest/gtest.h>

#include <optional/none.hpp>
#include <optional/optional_free_functions.hpp>
#include <optional/optional_value.hpp>

using opt::Optional;

TEST(OptionalFreeFunctionTest, OperatorBool) {
    const Optional<int> opt1{8};
    EXPECT_TRUE(bool(opt1));

    const Optional<int> opt2{};
    EXPECT_FALSE(bool(opt2));

    Optional<int> opt3{8};
    EXPECT_TRUE(bool(opt3));

    Optional<int> opt4{};
    EXPECT_FALSE(bool(opt4));

    EXPECT_TRUE(bool(Optional<int>{4}));
    EXPECT_FALSE(bool(Optional<int>{}));
    const Optional<int> opt5{4};
    EXPECT_TRUE(bool(std::move(opt5)));
    const Optional<int> opt6{};
    EXPECT_FALSE(bool(std::move(opt6)));
}

TEST(OptionalFreeFunctionTest, OperatorNotEqual) {
    const Optional<int> opt1{8};
    EXPECT_FALSE(!opt1);

    const Optional<int> opt2{};
    EXPECT_TRUE(!opt2);

    Optional<int> opt3{8};
    EXPECT_FALSE(!opt3);

    Optional<int> opt4{};
    EXPECT_TRUE(!opt4);

    EXPECT_FALSE(!Optional<int>{4});
    EXPECT_TRUE(!Optional<int>{});
    const Optional<int> opt5{4};
    EXPECT_FALSE(!std::move(opt5));
    const Optional<int> opt6{};
    EXPECT_TRUE(!std::move(opt6));
}

TEST(OptionalFreeFunctionTest, OperatorEqualTo) {
    Optional<int> opt1{5};
    Optional<int> opt2{5};

    EXPECT_TRUE(opt1 == opt2);

    Optional<int> opt3{};
    Optional<int> opt4{};

    EXPECT_TRUE(opt3 == opt4);

    Optional<int> opt5{};
    Optional<int> opt6{8};

    EXPECT_FALSE(opt5 == opt6);
    EXPECT_FALSE(opt6 == opt5);

    EXPECT_FALSE(opt6 == opt1);

    EXPECT_TRUE(opt1 == opt1);
    EXPECT_TRUE(opt3 == opt3);
}

TEST(OptionalFreeFunctionTest, OperatorNotEqualTo) {
    Optional<int> opt1{5};
    Optional<int> opt2{5};

    EXPECT_FALSE(opt1 != opt2);

    Optional<int> opt3{};
    Optional<int> opt4{};

    EXPECT_FALSE(opt3 != opt4);

    Optional<int> opt5{};
    Optional<int> opt6{8};

    EXPECT_TRUE(opt5 != opt6);
    EXPECT_TRUE(opt6 != opt5);

    EXPECT_TRUE(opt6 != opt1);

    EXPECT_FALSE(opt1 != opt1);
    EXPECT_FALSE(opt3 != opt3);
}

TEST(OptionalFreeFunctionTest, OperatorLessThan) {
    Optional<int> opt1{1};
    Optional<int> opt2{2};
    Optional<int> opt3{};
    Optional<int> opt4{};

    EXPECT_TRUE(opt1 < opt2);
    EXPECT_FALSE(opt2 < opt1);

    EXPECT_FALSE(opt1 < opt1);
    EXPECT_FALSE(opt3 < opt3);

    EXPECT_FALSE(opt3 < opt4);
    EXPECT_TRUE(opt3 < opt2);
    EXPECT_FALSE(opt1 < opt3);
}

TEST(OptionalFreeFunctionTest, OperatorGreaterThan) {
    Optional<int> opt1{1};
    Optional<int> opt2{2};
    Optional<int> opt3{};
    Optional<int> opt4{};

    EXPECT_FALSE(opt1 > opt2);
    EXPECT_TRUE(opt2 > opt1);

    EXPECT_FALSE(opt1 > opt1);
    EXPECT_FALSE(opt3 > opt3);

    EXPECT_FALSE(opt3 > opt4);
    EXPECT_FALSE(opt3 > opt2);
    EXPECT_TRUE(opt1 > opt3);
}

TEST(OptionalFreeFunctionTest, OperatorLessThanOrEqualTo) {
    Optional<int> opt1{1};
    Optional<int> opt2{2};
    Optional<int> opt3{};
    Optional<int> opt4{};
    Optional<int> opt5{2};

    EXPECT_TRUE(opt1 <= opt2);
    EXPECT_FALSE(opt2 <= opt1);
    EXPECT_TRUE(opt5 <= opt2);
    EXPECT_TRUE(opt2 <= opt5);

    EXPECT_TRUE(opt2 <= opt2);
    EXPECT_TRUE(opt3 <= opt3);

    EXPECT_TRUE(opt3 <= opt4);
    EXPECT_TRUE(opt3 <= opt2);
    EXPECT_FALSE(opt1 <= opt3);
}

TEST(OptionalFreeFunctionTest, OperatorGreaterThanOrEqualTo) {
    Optional<int> opt1{1};
    Optional<int> opt2{2};
    Optional<int> opt3{};
    Optional<int> opt4{};
    Optional<int> opt5{2};

    EXPECT_FALSE(opt1 >= opt2);
    EXPECT_TRUE(opt2 >= opt1);
    EXPECT_TRUE(opt5 >= opt2);
    EXPECT_TRUE(opt2 >= opt5);

    EXPECT_TRUE(opt2 >= opt2);
    EXPECT_TRUE(opt3 >= opt3);

    EXPECT_TRUE(opt3 >= opt4);
    EXPECT_FALSE(opt3 >= opt2);
    EXPECT_TRUE(opt1 >= opt3);
}

TEST(OptionalFreeFunctionTest, OperatorEqualToNoneT) {
    Optional<int> opt1{7};
    Optional<int> opt2{};

    EXPECT_FALSE(opt1 == opt::none);
    EXPECT_TRUE(opt2 == opt::none);
    EXPECT_FALSE(opt::none == opt1);
    EXPECT_TRUE(opt::none == opt2);

    opt2 = std::move(opt1);

    EXPECT_FALSE(opt2 == opt::none);
    EXPECT_TRUE(opt1 == opt::none);
    EXPECT_FALSE(opt::none == opt2);
    EXPECT_TRUE(opt::none == opt1);
}

TEST(OptionalFreeFunctionTest, OperatorNotEqualToNoneT) {
    Optional<int> opt1{7};
    Optional<int> opt2{};

    EXPECT_TRUE(opt1 != opt::none);
    EXPECT_FALSE(opt2 != opt::none);
    EXPECT_TRUE(opt::none != opt1);
    EXPECT_FALSE(opt::none != opt2);

    opt2 = std::move(opt1);

    EXPECT_TRUE(opt2 != opt::none);
    EXPECT_FALSE(opt1 != opt::none);
    EXPECT_TRUE(opt::none != opt2);
    EXPECT_FALSE(opt::none != opt1);
}

TEST(OptionalFreeFunctionTest, GetConst) {
    int i = 7;
    const Optional<int> opt(i);

    ASSERT_TRUE(bool(opt));
    EXPECT_EQ(7, get(opt));
}

TEST(OptionalFreeFunctionTest, Get) {
    int i = 7;
    Optional<int> opt(i);

    ASSERT_TRUE(bool(opt));
    EXPECT_EQ(7, get(opt));

    get(opt) = 3;
    ASSERT_TRUE(bool(opt));
    EXPECT_EQ(3, get(opt));
}

TEST(OptionalFreeFunctionTest, ConstGetFromPointer) {
    const Optional<int> opt{4};
    ASSERT_TRUE(bool(opt));
    EXPECT_EQ(4, *(get(&opt)));
}

TEST(OptionalFreeFunctionTest, GetFromPointer) {
    Optional<int> opt{4};
    ASSERT_TRUE(bool(opt));
    EXPECT_EQ(4, *(get(&opt)));

    *(get(&opt)) = 5;
    EXPECT_EQ(5, opt.get());
}

TEST(OptionalFreeFunctionTest, ConstGetPointer) {
    const Optional<int> opt{8};
    ASSERT_TRUE(bool(opt));
    EXPECT_EQ(8, *get_pointer(opt));
}

TEST(OptionalFreeFunctionTest, GetPointer) {
    Optional<int> opt{8};
    ASSERT_TRUE(bool(opt));
    EXPECT_EQ(8, *get_pointer(opt));

    *get_pointer(opt) = 4;
    ASSERT_TRUE(bool(opt));
    EXPECT_EQ(4, opt.get());
}

TEST(OptionalFreeFunctionTest, Swap) {
    using std::swap;
    Optional<int> opt1{};
    Optional<int> opt2{};
    Optional<int> opt3{6};
    Optional<int> opt4{32};

    swap(opt1, opt2);
    EXPECT_FALSE(bool(opt1));
    EXPECT_FALSE(bool(opt2));

    swap(opt3, opt4);
    EXPECT_TRUE(bool(opt3));
    EXPECT_TRUE(bool(opt3));
    EXPECT_EQ(32, opt3.get());
    EXPECT_EQ(6, opt4.get());

    swap(opt3, opt2);
    EXPECT_FALSE(bool(opt3));
    EXPECT_TRUE(bool(opt2));
    EXPECT_EQ(32, opt2.get());

    swap(opt1, opt4);
    EXPECT_FALSE(bool(opt4));
    EXPECT_TRUE(bool(opt1));
    EXPECT_EQ(6, opt1.get());
}
