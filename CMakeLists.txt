cmake_minimum_required(VERSION 3.2)
project(liboptional LANGUAGES CXX)
enable_testing()

include(GNUInstallDirs)
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# ADD TESTS
# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
add_subdirectory(test)

# CLANG TIDY
# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_custom_target(compile_commands_optional ALL
    COMMAND [ -f ${CMAKE_SOURCE_DIR}/build/compile_commands.json ] && mv
        ${CMAKE_SOURCE_DIR}/build/compile_commands.json
        ${CMAKE_SOURCE_DIR}/compile_commands.json || :)

# DOXYGEN
# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
# Doxyfile in project/doc : make doc
find_package(Doxygen)
if(DOXYGEN_FOUND)
    add_custom_target(docs
        ${DOXYGEN_EXECUTABLE} ${CMAKE_SOURCE_DIR}/docs/Doxyfile
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/docs
        COMMENT "Generating API documentation with Doxygen" VERBATIM)
endif()
