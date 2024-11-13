cmake_minimum_required(VERSION 3.30)
project(StationeersWorldCleaner)

find_package(PkgConfig REQUIRED)
set(CMAKE_CXX_STANDARD 20)

add_executable(StationeersWorldCleaner main.cpp)

pkg_check_modules(libxmlpp REQUIRED IMPORTED_TARGET libxml++-5.0)
target_link_libraries(StationeersWorldCleaner PRIVATE PkgConfig::libxmlpp)