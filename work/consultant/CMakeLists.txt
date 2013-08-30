cmake_minimum_required (VERSION 2.8)

set (HEADERS
    string_expression.h
)

set (SOURCES
    string_expression.cpp
)


add_executable (ut string_expression_ut.cpp ${HEADERS} ${SOURCES})
add_executable (app1 app1.cpp ${HEADERS} ${SOURCES})
