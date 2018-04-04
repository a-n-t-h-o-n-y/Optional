include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include)

# FIND GTEST
# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
find_package(GTest REQUIRED)
add_library(gtest INTERFACE IMPORTED)
set_property(TARGET gtest PROPERTY
    INTERFACE_INCLUDE_DIRECTORIES ${GTEST_INCLUDE_DIRS})
set_property(TARGET gtest PROPERTY
    INTERFACE_LINK_LIBRARIES ${GTEST_BOTH_LIBRARIES})

# CREATE TESTS
# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
set(TEST_SOURCES 
    none_test.cpp
    optional_value_test.cpp
    optional_void_test.cpp
    optional_reference_test.cpp
    aligned_storage_test.cpp
)

add_executable(test_optional EXCLUDE_FROM_ALL ${TEST_SOURCES})
target_link_libraries(test_optional PRIVATE gtest)
target_compile_features(test_optional PRIVATE cxx_std_14)
add_test(test_optional test_optional)

add_custom_target(tests DEPENDS test_optional)
