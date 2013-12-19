/**
 * Alexandrescu 2.1
 */
#pragma once


template <bool>
struct CompileTimeChecker { };

template<> struct CompileTimeChecker<false>;

#define STATIC_ASSERT(expr, msg) \
{\
    CompileTimeChecker<(expr) != 0> ERROR_##msg; \
}
