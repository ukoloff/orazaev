#include <iostream>
#include <iterator>


template <typename T>
struct ListElement {
    T data;
    ListElement * next;

    ListElement(const T & d)
            : data(d)
            , next(0) {
    }

    ListElement(const T & d, ListElement * const n)
            : data(d)
            , next(n) {
    }

};


template <typename T>
struct ListIterator : public std::iterator<std::forward_iterator_tag, T> {
    ListElement<T> * elem;

    ListIterator(ListElement<T> * e)
            : elem(e) {
    }

    ListIterator<T> & operator ++() {
        elem = elem->next;
    }

    ListIterator<T> operator ++(int) {
        ListIterator old(*this);

        ++elem;

        return old;
    }

    T & operator *() {
        return elem->data;
    }

    bool operator != (const ListIterator<T> & rhs) const {
        return rhs.elem != elem;
    }

    bool operator == (const ListIterator<T> & rhs) const {
        return rhs.elem == elem;
    }
};


template <typename T>
class List {
    ListElement<T> * _head;
    size_t _size;



    List(const List &);
    List & operator = (const List &);
    public:

    List() : _head(0)
            , _size(0) {
    }

    List(size_t number, const T & elem)
            : _head(0)
            , _size(0) {
        ListElement<T> * curElem;


        while (_size < number) {
            if (_head == 0) {
                _head = new ListElement<T>(elem);
                curElem = _head;
            } else {
                curElem->next = new ListElement<T>(elem);
                curElem = curElem->next;
            }

            ++_size;
        }
    }



    template <typename Iter>
    List(Iter begin, Iter end);



    ~List() {
        while(_head->next != 0) {
            ListElement<T> * next = _head->next;
            delete _head;
            _head = next;
        }

        delete _head;
    }


    bool empty() const {
        return _size == 0;
    }


    void push_back(const T & elem) {
        if (empty()) {
            _head = new ListElement<T>(elem);
        } else {
            ListElement<T> * curElem = _head;
            while (curElem->next != 0) {
                ++curElem;
            }
            curElem->next = new ListElement<T>(elem);
        }

        ++_size();
    }



    size_t size() const {
        return _size;
    }



    typedef ListIterator<T> iterator;
    typedef ListIterator<const T> const_iterator;



    iterator begin() {
        return ListIterator<T>(_head);
    }



    iterator end() {
        return ListIterator<T>(0);
    }
};



template <typename T>
template <typename Iter>
List<T>::List(
        Iter begin, Iter end)
        : _head(0)
        , _size(0) {
    ListElement<T> * curElem = 0;

    while (begin != end) {
        if (_head == 0) {
            _head = new ListElement<T>(*begin);
            curElem = _head;
        } else {
            curElem->next = new ListElement<T>(*begin);
            curElem = curElem->next;
        }

        ++begin;
        ++_size;
    }
}




int main() {
    int a[5] = {
        1, 2, 3, 4, 5
    };

    List<int> li(a, a + 5);

    for (List<int>::iterator it = li.begin(); it != li.end(); ++it) {
        std::cout << *it << std::endl;
    }
    return 0;
}
