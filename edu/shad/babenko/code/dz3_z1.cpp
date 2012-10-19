#include <iostream>
#include <list>
#include <vector>
#include <string>

const int NA = -1;
const char POP = 'L';
const char PUSH = 'R';

template <typename T>
class TStatisitc {
    public:
    TStatisitc(size_t k)
    : _k(k)
    , _size(0)
    , _statistic()
    , _sortedList()
    , _pointerList()
    { }

    void push(const T & elem);
    T pop();

    size_t size() const;
    T getStatistic() const;

    private:
    const size_t _k;
    size_t _size;
    typename std::list<T>::iterator _statistic;
    std::list<T> _sortedList;
    std::list<typename std::list<T>::iterator> _pointerList;

    void pushElementInLists(const T & elem);
    void refreshStatisticAfterPush(const T & elem);
    T popElementFromLists();
    void refreshStatisticAfterPop(const T & elem);
};






template <typename T>
class TStatisitcQueue {
    public:
    TStatisitcQueue(size_t k)
    : _queue()
    , _statistic(k)
    { }

    void push(const T & elem);
    void pop();

    T getStatistic();

    private:
    std::list<T> _queue;
    TStatisitc<T> _statistic;
};






/* ---------- Realization ---------- */

/* ---------- TStatistic ------------ */
template <typename T>
void TStatisitc<T>::push(const T & elem) {
    pushElementInLists(elem);
    refreshStatisticAfterPush(elem);
}

template <typename T>
T TStatisitc<T>::pop() {
    T elem = popElementFromLists();
    refreshStatisticAfterPop(elem);

    return elem;
}

template <typename T>
size_t TStatisitc<T>::size() const {
    return _size;
}

template <typename T>
T TStatisitc<T>::getStatistic() const {
    if (size() <= _k)
        return NA;
    return *_statistic;
}

template <typename T>
void TStatisitc<T>::pushElementInLists(const T & elem) {
    bool elementWasPushed = false;

    for (typename std::list<T>::iterator it = _sortedList.begin();
         it != _sortedList.end(); ++it) {
        if (*it <= elem) {
            _sortedList.insert(it, elem);
            _pointerList.push_back(it);

            elementWasPushed = true;
            break;
        }
    }

    if (!elementWasPushed) {
        _sortedList.push_back(elem);
        _pointerList.push_back(_sortedList.begin());
    }

    ++_size;
}

template <typename T>
void TStatisitc<T>::refreshStatisticAfterPush(const T & elem) {
    if (_size == _k + 1) {
        _statistic = --_sortedList.end();
    } else if (_size > _k) {
        if (elem <= *_statistic) {
            --_statistic;
        }
    }
}

template <typename T>
T TStatisitc<T>::popElementFromLists() {
    typename std::list<T>::iterator it = *_pointerList.begin();
    _pointerList.pop_front();

    T elem = *it;
    _sortedList.erase(it);

    --_size;
    return elem;
}

template <typename T>
void TStatisitc<T>::refreshStatisticAfterPop(const T & elem) {
    if (elem < *_statistic) {
        --_statistic;
    }
}


/* -------------- TStatisticQueue -------------- */
template <typename T>
void TStatisitcQueue<T>::push(const T & elem) {
    _queue.push_back(elem);
    _statistic.push(elem);
}

template <typename T>
void TStatisitcQueue<T>::pop() {
    _queue.pop_front();
    _statistic.pop();
}

template <typename T>
T TStatisitcQueue<T>::getStatistic() {
    return _statistic.getStatistic();
}


void solveProblem(const std::vector<int> & data,
                  const std::string commandString,
                  std::ofstream & out, size_t k) {
    size_t L = 0;
    size_t R = 0;
    TStatisitcQueue<int> queue(k);
    for (std::string::iterator it = commandString.begin();
         it != commandString.end(); ++it) {
        if (*it == POP) {
            queue.pop();
            out << queue.getStatistic();
        } else if (*it == PUSH) {
            queue.push(it - commandString.begin());
            out << queue.getStatistic();
        }
    }
}

int main() {
    size_t k;
    size_t n;
    size_t m;



    return 0;
}
