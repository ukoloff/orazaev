#include<iostream>
#include<string>
#include<stdexcept>
#include"aorazaev_z2.h"

template <typename T>
void assertEqual(const T & p, const T & q) {
    if (p != q) {
        std::cout << std::endl
            << " ========================== " << std::endl;
        std::cout << "Value1 = " << p << std::endl;
        std::cout << "Value2 = " << q << std::endl;

        throw std::runtime_error("ASSERT FAILED!");
    }
}

void startTest(const std::string & s) {
    std::cout << s << "... ";
}

void endTest() {
    std::cout << "[ OK ]" << std::endl;
}

int main() {
    {
    int a[5] = {0, 1, 2, 3, 4};
    Polynomial<int> p(1);
    Polynomial<int> q(a, a + 5);
    Polynomial<int> z(a, a + 5);
    Polynomial<int> t(q);

    startTest("Operator << :");
    std::cout << q <<" ";
    endTest();

    startTest("Operator == test");
    assertEqual(q, t);
    endTest();


    startTest("Operator != test");
    if (q == p) {
        std::runtime_error("FAILED!");
    }
    endTest();


    startTest("Operator += test");
    q += p;
    {
        int expect[5] = {1, 1, 2, 3, 4};
        assertEqual(
            q, Polynomial<int>(expect, expect + 5));
    }
    endTest();


    startTest("Operator += test2");
    p += q;
    {
        int expect[5] = {2, 1, 2, 3, 4};
        assertEqual(
            p, Polynomial<int>(expect, expect + 5));
    }
    endTest();


    startTest("Operator -= test");
    p -= q;
    {
        int expect[1] = {1};
        assertEqual(
            p, Polynomial<int>(expect, expect + 1));
    }
    endTest();


    startTest("Operator -= test 2");
    {
        assertEqual(t -= z, Polynomial<int>(0));
    }
    endTest();


    startTest("Operator += int test");
    p += (Polynomial<int>) 1;
    {
        int expect[1] = {2};
        assertEqual(
            p, Polynomial<int>(expect, expect + 1));
    }
    endTest();
    }



    {
    int a[2] = {0, 1};
    int b[4] = {1, 2, 3 ,4};


    {
    startTest("Multiplication 0");
    Polynomial<int> p(a, a + 2);
    Polynomial<int> q(b, b + 4);
    p *= q;

    int expect[5] = {0, 1, 2, 3, 4};
    assertEqual(
        p, Polynomial<int>(expect, expect + 5));
    endTest();
    }


    {
    startTest("Multiplication 1");
    Polynomial<int> p(a, a + 2);
    Polynomial<int> q(b, b + 4);
    q *= p;
    int expect[5] = {0, 1, 2, 3, 4};
    assertEqual(
        q, Polynomial<int>(expect, expect + 5));
    endTest();
    }



    {
    startTest("Multiplication by zero");
    Polynomial<int> p;
    Polynomial<int> q(b, b + 4);
    q *= p;
    assertEqual(q, Polynomial<int>(0));
    endTest();
    }



    {
    startTest("Multiplication by zero 2");
    Polynomial<int> p;
    Polynomial<int> q(b, b + 4);
    p *= q;
    assertEqual(p, Polynomial<int>(0));
    endTest();
    }




    {
    startTest("Operator *");
    Polynomial<int> p(a, a + 2);
    Polynomial<int> q(b, b + 4);

    int expect[5] = {0, 1, 2, 3, 4};
    assertEqual(
        p * q, Polynomial<int>(expect, expect + 5));
    endTest();
    }


    {
    startTest("Operator * 2");
    Polynomial<int> p(a, a + 2);
    Polynomial<int> q(b, b + 4);
    int expect[5] = {0, 1, 2, 3, 4};
    assertEqual(
        q * p, Polynomial<int>(expect, expect + 5));
    endTest();
    }



    {
    startTest("Operator * by 0");
    Polynomial<int> p;
    Polynomial<int> q(b, b + 4);
    assertEqual(q * p, Polynomial<int>(0));
    endTest();
    }



    {
    startTest("Operator * by 0 ver2");
    Polynomial<int> p;
    Polynomial<int> q(b, b + 4);
    assertEqual(p * q, Polynomial<int>(0));
    endTest();
    }
    }

    {
        int a[3] = {3, 2, 1};
        Polynomial<int> p(a, a + 3);

        startTest("Operator (0)");
        assertEqual(3, p(0));
        endTest();

        startTest("Operator (1)");
        assertEqual(6, p(1));
        endTest();

        startTest("Operator (3)");
        assertEqual(18, p(3));
        endTest();
    }

    {
        int a[4] = {-42, 0, -12, 1};
        int b[2] = {-3, 1};
        Polynomial<int> p(a, a + 4);
        Polynomial<int> q(b, b + 2);
        int e[3] = {-27, -9, 1};

        startTest("Operator /");
        assertEqual(p / q, Polynomial<int>(e, e + 3));
        endTest();
    }

    {
        int a[4] = {-42, 0, -12, 1};
        Polynomial<int> p(a, a + 4);
        Polynomial<int> q(1);
        int e[4] = {-42, 0, -12, 1};

        startTest("Operator / vol.2");
        assertEqual(p / q, Polynomial<int>(e, e + 4));
        endTest();
    }

    {
        int a[4] = {0, 0, 2, 2};
        Polynomial<int> p(a, a + 4);
        Polynomial<int> q(2);
        int e[4] = {0, 0, 1, 1};

        startTest("Operator / vol.3");
        assertEqual(p / q, Polynomial<int>(e, e + 4));
        endTest();
    }

    {
        int a[4] = {-42, 0, -12, 1};
        int b[2] = {-3, 1};
        Polynomial<int> p(a, a + 4);
        Polynomial<int> q(b, b + 2);
        int e[3] = {-27, -9, 1};

        startTest("Operator /=");
        assertEqual(p /= q, Polynomial<int>(e, e + 3));
        endTest();
    }

    {
        int a[4] = {-42, 0, -12, 1};
        Polynomial<int> p(a, a + 4);
        Polynomial<int> q(1);
        int e[4] = {-42, 0, -12, 1};

        startTest("Operator /= vol.2");
        assertEqual(p /= q, Polynomial<int>(e, e + 4));
        endTest();
    }

    {
        int a[4] = {0, 0, 2, 2};
        Polynomial<int> p(a, a + 4);
        Polynomial<int> q(2);
        int e[4] = {0, 0, 1, 1};

        startTest("Operator /= vol.3");
        assertEqual(p /= q, Polynomial<int>(e, e + 4));
        endTest();
    }

    {
        int a[4] = {0, 0, 2, 2};
        Polynomial<int> p(a, a + 4);
        int b[4] = {0, 0, 4, 5};
        Polynomial<int> q(b, b + 4);
        int c[1] = {2};
        Polynomial<int> expect(c, c + 1);

        startTest("Operator /= vol.4");
        assertEqual(q /= p, expect);
        endTest();
    }

    {
        int a[4] = {0, 0, 0, 1};
        Polynomial<int> p(a, a + 4);
        int b[3] = {0, 0, 2};
        Polynomial<int> q(b, b + 3);
        int c[1] = {0};
        Polynomial<int> expect(c, c + 1);

        startTest("Operator /= vol.5");
        assertEqual(p /= q, expect);
        endTest();
    }

    {
        int a[4] = {-42, 0, -12, 1};
        int b[2] = {-3, 1};
        Polynomial<int> p(a, a + 4);
        Polynomial<int> q(b, b + 2);
        int e[3] = {-27, -9, 1};

        startTest("Operator %=");
        assertEqual(p %= q, Polynomial<int>(-123));
        endTest();
    }

    {
        int a[4] = {-42, 0, -12, 1};
        int b[2] = {-3, 1};
        Polynomial<int> p(a, a + 4);
        Polynomial<int> q(b, b + 2);
        int e[3] = {-27, -9, 1};

        startTest("Operator %");
        assertEqual(p % q, Polynomial<int>(-123));
        endTest();
    }

    {
        int a[4] = {0, 0, 2, 2};
        Polynomial<int> p(a, a + 4);
        Polynomial<int> q(2);
        int e[1] = {0};

        startTest("Operator %= vol.2");
        assertEqual(p %= q, Polynomial<int>(e, e + 1));
        endTest();
    }

    {
        int a[4] = {0, 0, 4, 5};
        Polynomial<int> p(a, a + 4);
        int b[4] = {0, 0, 2, 2};
        Polynomial<int> q(b, b + 4);
        int c[4] = {0, 0, 0, 1};
        Polynomial<int> expect(c, c + 4);

        startTest("Operator %= vol.3");
        assertEqual(p %= q, expect);
        endTest();
    }

    {
        int a[3] = {0, 2, 2};
        Polynomial<int> p(a, a + 3);
        int b[4] = {0, 0, 4, 5};
        Polynomial<int> q(b, b + 4);
        int c[4] = {0, 2, 2};
        Polynomial<int> expect(c, c + 3);

        startTest("Operator %= vol.4");
        assertEqual(p %= q, expect);
        endTest();
    }

    {
        int a[4] = {0, 0, 2, 2};
        Polynomial<int> p(a, a + 4);
        int b[4] = {0, 0, 4, 4};
        Polynomial<int> q(b, b + 4);
        int c[4] = {0, 0, 2, 2};
        Polynomial<int> expect(c, c + 4);

        startTest("Operator , vol.0");
        assertEqual((p, q), expect);
        endTest();
    }

    {
        int a[4] = {0, 0, 2, 2};
        Polynomial<int> p(a, a + 4);
        int b[4] = {0, 0, 4, 5};
        Polynomial<int> q(b, b + 4);
        int c[4] = {0, 0, 1};
        Polynomial<int> expect(c, c + 3);

        startTest("Operator , vol.1");
        assertEqual((p, q), expect);
        endTest();
    }
    {
        int a[4] = {0, 0, 2, 2};
        Polynomial<int> p(a, a + 4);
        int b[4] = {0, 0, 4, 5};
        Polynomial<int> q(b, b + 4);
        int c[4] = {0, 0, 1};
        Polynomial<int> expect(c, c + 3);

        startTest("Operator , vol.1");
        assertEqual((p, q), expect);
        endTest();
    }

    {
        int a[3] = {0, 2, 2};
        Polynomial<int> p(a, a + 3);
        int b[4] = {0, 0, 4, 5};
        Polynomial<int> q(b, b + 4);
        int c[2] = {0, 1};
        Polynomial<int> expect(c, c + 2);

        startTest("Operator , vol.2");
        assertEqual((p, q), expect);
        endTest();
    }
    return 0;
}
