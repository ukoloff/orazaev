#include <assert.h>
#include <sstream>
#include <iostream>

#include "string_expression.h"


using namespace NStringExpression;

void test_Character() {
    Character ch('A');
    assert(ch.str() == "A");

    try {
        ch.append(TokenHolder(new Character('B')));
        assert(false);
    } catch (const std::logic_error& le) {
        // OK
    }
}

void test_InnerExpression() {
    TokenHolder expr(new InnerExpression());
    expr->append(TokenHolder(new Character('A')));
    expr->append(TokenHolder(new Character('B')));
    expr->append(TokenHolder(new Character('C')));

    StringExpression main_expr;
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

void test_StringExpression() {
    StringExpression expr = build_expression("B+A");
    expr.sort();
    assert(expr.str() == "A+B");

    expr = build_expression("(X+(V+M+A)+C+(M+L+(C+C)+C))");
    expr.sort();
    assert(expr.str() == "((A+M+V)+C+(C+(C+C)+L+M)+X)");
}

void test_sort_string() {
    assert(sort_string("") == "");
    assert(sort_string("B+A") == "A+B");
    assert(sort_string("(X+(V+M+A)+C+(M+L+(C+C)+C))") == "((A+M+V)+C+(C+(C+C)+L+M)+X)");
}

int main() {
    test_Character();
    test_InnerExpression();
    test_build_expression();
    test_StringExpression();
    test_sort_string();
    return 0;
}
