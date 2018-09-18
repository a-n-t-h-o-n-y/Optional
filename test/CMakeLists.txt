# FIND GTEST
# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
find_package(GTest)
add_library(gtest INTERFACE IMPORTED)
set_property(TARGET gtest PROPERTY
    INTERFACE_INCLUDE_DIRECTORIES ${GTEST_INCLUDE_DIRS})
set_property(TARGET gtest PROPERTY
    INTERFACE_LINK_LIBRARIES ${GTEST_BOTH_LIBRARIES})

# CREATE TEST
# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
add_executable(test_optional EXCLUDE_FROM_ALL
    none_test.cpp
    optional_value_test.cpp
    optional_void_test.cpp
    optional_reference_test.cpp
    aligned_storage_test.cpp
)

target_link_libraries(test_optional PRIVATE gtest optional)
target_compile_features(test_optional PRIVATE cxx_std_14)

add_custom_target(tests DEPENDS test_optional)

add_test(test_optional test_optional)
