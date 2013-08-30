#include "string_expression.h"

#include <assert.h>
#include <sstream>
#include <iostream>

void test_Character() {
    Character ch('A');
    assert(ch.str() == "A");
}

void test_StringExpression() {
    TokenHolder expr(new Expression());
    expr->append(TokenHolder(new Character('A')));
    expr->append(TokenHolder(new Character('B')));
    expr->append(TokenHolder(new Character('C')));

    MainExpression main_expr;
    main_expr.append(expr);
    assert(main_expr.str() == "(A+B+C)");
}

void test_build_expression() {
    assert(build_expression("A").str() == "A");
    assert(build_expression("(A+B)").str() == "(A+B)");
    assert(build_expression("C+(A+B)").str() == "C+(A+B)");
    assert(build_expression("((A+B)+C+(D+E))").str() == "((A+B)+C+(D+E))");

    try {
        build_expression(")");
        assert(false);
    } catch (const std::invalid_argument& ia) {
        // OK
    }

    try {
        build_expression("(A+B+C)((");
        assert(false);
    } catch (const std::invalid_argument& ia) {
        // OK
    }
}

void test_sorting() {
    MainExpression expr = build_expression("B+A");
    expr.sort();
    assert(expr.str() == "A+B");

    expr = build_expression("(X+(V+M+A)+C+(M+L+(C+C)+C))");
    expr.sort();
    assert(expr.str() == "((A+M+V)+C+(C+(C+C)+L+M)+X)");
}

int main() {
    test_Character();
    test_StringExpression();
    test_build_expression();
    test_sorting();
    return 0;
}
