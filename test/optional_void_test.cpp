#include <gtest/gtest.h>

#include <optional/optional_void.hpp>

using opt::Optional;

TEST(OptionalVoidTest, Validity) {
    Optional<void> ov;
    EXPECT_FALSE(ov);
    EXPECT_TRUE(!ov);
}
