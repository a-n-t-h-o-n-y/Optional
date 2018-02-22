#include <gtest/gtest.h>

#include <optional/none.hpp>

TEST(NoneTest, BoolConversion) {
    EXPECT_FALSE(opt::none);
}
