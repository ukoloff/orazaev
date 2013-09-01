cmake_minimum_required (VERSION 2.8)

set (HEADERS
        factory.hpp)

set (SOURCES
        factory_ut.cpp)

add_executable (factory_ut ${HEADERS} ${SOURCES})
