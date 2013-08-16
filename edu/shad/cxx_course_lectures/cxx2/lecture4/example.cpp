        #include <iostream>
        #include <vector>
        #include <list>
        #include <typeinfo>

        template <typename T>
        class Allocator: public std::allocator<T> {
        public:
            Allocator() throw() {
                std::cout << "Allocator() for "
                          << typeid(T).name() << std::endl;
            }
            ~Allocator() throw() {
                std::cout << "~Allocator() for "
                          << typeid(T).name() << std::endl;
            }
            template <typename U>
            struct rebind {
                typedef Allocator<U> other;
            };
        };

        int main() {
            std::vector<int, Allocator<int> > v;
            v.push_back(123);

            return 0;
        }
