#include <gtest/gtest.h>

#include <optional/detail/aligned_storage.hpp>

TEST(AlignedStorageTest, Default) {
    opt::detail::Aligned_storage<int> as_i;
    as_i.ref() = 5;
    EXPECT_EQ(5, as_i.ref());
    EXPECT_EQ(5, *as_i.ptr_ref());
    EXPECT_EQ(5, *static_cast<int*>(as_i.address()));
}
