template <bool True>
struct static_assert {
    static_assert() {
    }
};

template <>
struct static_assert<false>;

template <int M>
class C: static_assert<M <= 100> {
};


int main() {
    static_assert<2 * 2 == 5>();
    C<20> c;

    return 0;
}
