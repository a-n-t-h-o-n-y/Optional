# FIND GTEST
# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
find_package(GTest)
find_package(Threads)
add_library(gtest INTERFACE IMPORTED)
set_property(TARGET gtest PROPERTY
    INTERFACE_INCLUDE_DIRECTORIES ${GTEST_INCLUDE_DIRS})
set_property(TARGET gtest PROPERTY
    INTERFACE_LINK_LIBRARIES ${GTEST_BOTH_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})

# CREATE TEST
# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
add_executable(optional_tests EXCLUDE_FROM_ALL
    none_test.cpp
    optional_value_test.cpp
    optional_void_test.cpp
    optional_reference_test.cpp
    aligned_storage_test.cpp
)

target_link_libraries(optional_tests PRIVATE gtest optional)

if(${CMAKE_VERSION} VERSION_LESS "3.8")
    set(CMAKE_CXX_STANDARD 14)
else()
    target_compile_features(optional_tests INTERFACE cxx_std_14)
endif()

add_test(optional_tests optional_tests)
