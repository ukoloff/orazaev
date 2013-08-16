#include<iostream>
#include<vector>

template <typename T>
class TMatrix {
    size_t _size;
    std::vector<std::vector<T> > _data;



    public:
    TMatrix(size_t size)
    : _size(size)
    , _data(size, std::vector<T>(size, 0)) {
    }

    class EMatrix {};

    TMatrix & operator += (const TMatrix & rhs) {
        if (_size != rhs._size) {
            throw EMatrix();
        }

        for (size_t i = 0; i < _size; ++i)
        for (size_t j = 0; j < _size; ++j)
            _data[i][j] += rhs._data[i][j];

        return *this;
    }

    TMatrix operator + (const TMatrix & rhs) const {
        TMatrix temp(*this);
        return temp += rhs;
    }

    TMatrix & operator -= (const TMatrix & rhs) {
        if (_size != rhs._size) {
            throw EMatrix();
        }

        for (size_t i = 0; i < _size; ++i)
        for (size_t j = 0; j < _size; ++j)
            _data[i][j] -= rhs._data[i][j];

        return *this;
    }

    TMatrix operator - (const TMatrix & rhs) const {
        TMatrix temp(*this);
        return temp -= rhs;
    }

    TMatrix operator * (const TMatrix & rhs) const {
        if (_size != rhs._size) {
            throw EMatrix();
        }

        TMatrix result(_size);

        for (size_t i = 0; i < _size; ++i)
        for (size_t j = 0; j < _size; ++j) {
            for (size_t k = 0; k < _size; ++k) {
                result._data[i][j] +=
                    _data[i][k] * rhs._data[k][j];
            }
        }

        return result;
    }

    TMatrix & operator *= (const TMatrix & rhs) {
        return *this = *this * rhs;
    }

    std::vector<T> & operator [] (size_t i) {
        return _data[i];
    }

    std::vector<T> operator [] (size_t i) const {
        return _data[i];
    }

    size_t size() const {
        return _size;
    }

    template <typename A>
    friend bool operator == (TMatrix<A>, TMatrix<A>);

    template <typename Inc>
    class iterator {
        public:
        size_t i;
        size_t j;
        TMatrix<T> * m;

        iterator(size_t I, size_t J, TMatrix<T> * M)
        : i(I)
        , j(J)
        , m(M) {
        }

        iterator & operator ++() {
            Inc(&i, &j, m);
            return *this;
        }

        iterator & operator ++(int) {
            iterator old(*this);
            Inc(&i, &j, m);
            return old;
        }

        T & operator *() {
            return (*m)[i][j];
        }
    };

    class IncRow {
    public:
    IncRow(size_t * i, size_t * j, TMatrix<T> * m) {
        *i += 1;
        if (*i == m->size()) {
            *i = 0;
            *j += 1;
        }
    }
    };

    class IncLine {
    public:
    IncLine(size_t * i, size_t * j, TMatrix<T> * m) {
        *j += 1;
        if (j == m->size()) {
            *j = 0;
            *i += 1;
        }
    }
    };

    public:
    typedef iterator<IncRow> row_iterator;
    typedef iterator<IncLine> line_iterator;


    row_iterator row_begin() {
        return row_iterator(0, 0, this);
    }

    row_iterator row_end() {
        return row_iterator(_size, _size - 1, this);
    }


    line_iterator line_begin() {
        return line_iterator(0, 0, this);
    }

    line_iterator line_end() {
        return line_iterator(_size - 1, _size, this);
    }
};

template <typename T>
std::ostream & operator << (std::ostream & out, const TMatrix<T> & m) {
    for (int i = 0; i < m.size(); ++i) {
        for (int j = 0; j < m.size(); ++j) {
            out << m[i][j] << " ";
        }
        out << "\n";
    }

    return out;
}

template <typename A>
bool operator == (TMatrix<A> a, TMatrix<A> b) {
    return a._data == b._data;
}

template <typename A>
bool operator != (TMatrix<A> a, TMatrix<A> b) {
    return ! (a == b);
}

template <typename A>
bool operator != (typename TMatrix<A>::row_iterator it0,
    typename TMatrix<A>::row_iterator it1) {
    if (it0.i == it1.i && it0.j == it0.j) {
        return true;
    }

    return false;
}

int main() {
    TMatrix<int> m(2);
    m[0][0] = 1;
    m[0][1] = 2;
    m[1][0] = 3;
    m[1][1] = 4;

    TMatrix<int> j(2);
    j[0][0] = 1;
    j[0][1] = 1;
    j[1][0] = 1;
    j[1][1] = 1;

    std::cout << m << std::endl;
    std::cout << j << std::endl;
    std::cout << m + j << std::endl;
    std::cout << m - j << std::endl;

    std::cout << m * j << std::endl;

    TMatrix<int> e(2);
    e[0][0] = 1;
    e[0][1] = 0;
    e[1][0] = 0;
    e[1][1] = 1;

    if (m * e != m) {
        throw 1;
    }

    TMatrix<int>::row_iterator it1 = m.row_begin(), it2 = m.row_end();
    operator != <int> (it1, it2);

    /*for (TMatrix<int>::row_iterator it = m.row_begin();
         it != m.row_end(); ++it) {
        std::cout << *it << " ";
    }*/


    return 0;
}
