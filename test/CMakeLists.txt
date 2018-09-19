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

if(${CMAKE_MAJOR_VERSION} VERSION_LESS "3.8")
    set(CMAKE_CXX_STANDARD 14)
else()
    target_compile_features(optional INTERFACE cxx_std_14)
endif()

add_custom_target(tests DEPENDS test_optional)

add_test(test_optional test_optional)
