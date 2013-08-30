#cmake_minimum_required (VERSION 2.8)

set (HEADERS
    src/string_expression.h
)

set (SOURCES
    src/string_expression.cpp
)

add_executable (ut src/string_expression_ut.cpp ${HEADERS} ${SOURCES})
add_executable (app1 src/app1.cpp ${HEADERS} ${SOURCES})
add_executable (app2 src/app2.cpp ${HEADERS} ${SOURCES})
