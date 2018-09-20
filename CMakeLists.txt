cmake_minimum_required(VERSION 3.2)
project(liboptional LANGUAGES CXX)
include(GNUInstallDirs)
enable_testing()

# CREATE OPTIONAL LIBRARY
# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
add_library(optional INTERFACE)

target_include_directories(optional
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

if(${CMAKE_VERSION} VERSION_LESS "3.8")
    set(CMAKE_CXX_STANDARD 14)
else()
    target_compile_features(optional INTERFACE cxx_std_14)
endif()

# LIBRARY INSTALLATION
# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(TARGETS optional EXPORT OptionalLibraryConfig)
install(EXPORT OptionalLibraryConfig DESTINATION share/OptionalLibrary/cmake)
export(TARGETS optional FILE OptionalLibraryConfig.cmake)

# ADD TESTS
# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
add_subdirectory(test)

# CLANG TIDY
# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_custom_target(compile_commands ALL
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
