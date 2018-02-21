cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project(liboptional LANGUAGES CXX)
include(GNUInstallDirs)
enable_testing()
add_compile_options(-std=c++14)

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

set(TEST_SOURCES 
    "test/none_test.cpp"
    "test/optional_value_test.cpp"
    "test/optional_void_test.cpp"
    "test/optional_reference_test.cpp"
    "test/aligned_storage_test.cpp"
)

add_executable(testoptional ${TEST_SOURCES})

find_package(GTest REQUIRED)
if (GTEST_FOUND)
    target_link_libraries(testoptional ${GTEST_BOTH_LIBRARIES})
endif()

add_test(testoptional testoptional)

# Build System Info For Clang-Tidy
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_custom_target(compile_commands_optional ALL
    COMMAND [ -f ${CMAKE_SOURCE_DIR}/build/compile_commands.json ] && mv
        ${CMAKE_SOURCE_DIR}/build/compile_commands.json
        ${CMAKE_SOURCE_DIR}/compile_commands.json || :)

# Doxygen Target
find_package(Doxygen)
if(DOXYGEN_FOUND)
    add_custom_target(docs_optional
    ${DOXYGEN_EXECUTABLE} ${CMAKE_SOURCE_DIR}/docs/Doxyfile
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/docs
    COMMENT "Generating API documentation with Doxygen" VERBATIM)
endif()
