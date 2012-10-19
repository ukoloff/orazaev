#include <iostream>
#include <list>
#include <vector>
#include <sstream>
#include <string>
#include <stdexcept>
#include <algorithm>

#include <ctime>
#include <cstdlib>

const char POP = 'L';
const char PUSH = 'R';
const double TIME_LIMIT = 1.0;

class TStatistic {
    public:
    TStatistic(size_t k, int na)
    : _k(k)
    , NA(na)
    , _size(0)
    , _statistic()
    , _sortedList()
    , _pointerList()
    { }

    void push(const int & elem);
    int pop();

    size_t size() const;
    int getStatistic() const;

    std::string toStr() const;

    private:
    const size_t _k;
    const int NA;
    size_t _size;
    std::list<int>::iterator _statistic;
    std::list<int> _sortedList;
    std::list<std::list<int>::iterator> _pointerList;

    void pushElementInLists(const int & elem);
    void refreshStatisticAfterPush(const int & elem);
    std::list<int>::iterator popElementFromLists();
    void refreshStatisticAfterPop(const std::list<int>::iterator &);
};







// TStatistic realization

void TStatistic::push(const int & elem) {
    pushElementInLists(elem);
    refreshStatisticAfterPush(elem);
}



int TStatistic::pop() {
    std::list<int>::iterator it =
        popElementFromLists();

    refreshStatisticAfterPop(it);

    int elem = *it;
    _sortedList.erase(it);
    return elem;
}



size_t TStatistic::size() const {
    return _size;
}



int TStatistic::getStatistic() const {
    if (size() < _k)
        return NA;
    return *_statistic;
}



std::string TStatistic::toStr() const {
    std::stringstream out(std::stringstream::out);

    out << "{Sorted:[";
    for (std::list<int>::const_iterator it = _sortedList.begin();
         it != _sortedList.end(); ++it) {
        out << (int) *it;

        if (it != --_sortedList.end()) {
            out << ", ";
        }
    }

    out << "], Queue:[";

    for (std::list<std::list<int>::iterator>::const_iterator it =
         _pointerList.begin(); it != _pointerList.end(); ++it) {
        out << (int) **it;

        if (it != --_pointerList.end()) {
            out << ", ";
        }
    }

    out << "], Statistic = " << getStatistic()
        << "}";

    return std::string(out.str());
}



void TStatistic::pushElementInLists(const int & elem) {
    bool elementWasPushed = false;

    for (std::list<int>::iterator it = _sortedList.begin();
         it != _sortedList.end(); ++it) {
        if (*it >= elem) {
            _sortedList.insert(it, elem);
            _pointerList.push_back(--it);

            elementWasPushed = true;
            break;
        }
    }

    if (!elementWasPushed) {
        _sortedList.push_back(elem);
        _pointerList.push_back(--_sortedList.end());
    }

    ++_size;
}



void TStatistic::refreshStatisticAfterPush(const int & elem) {
    if (_size == _k) {
        _statistic = --_sortedList.end();
    } else if (_size >= _k && elem <= *_statistic) {
        --_statistic;
    }
}



std::list<int>::iterator TStatistic::popElementFromLists() {
    std::list<int>::iterator it = *_pointerList.begin();
    _pointerList.pop_front();

    --_size;
    return it;
}



void TStatistic::refreshStatisticAfterPop(
    const std::list<int>::iterator & it) {
    if (_k <= _size)
    if (it == _statistic || *it < *_statistic) {
        ++_statistic;
    }
}






std::vector<int> solveProblem(const std::vector<int> & data
    , const std::string commandString
    , size_t k) {
    std::vector<int> out;
    size_t L = 0;
    size_t R = 0;

    TStatistic queue(k, -1);

    queue.push(data[0]);

    for (std::string::const_iterator it = commandString.begin();
         it != commandString.end(); ++it) {
        //std::cout << queue.toStr() << std::endl;

        if (*it == POP) {
            ++L;
            queue.pop();
        } else if (*it == PUSH) {
            ++R;
            queue.push(data[R]);
        }

        out.push_back(queue.getStatistic());
    }

    return out;
}



// ===================> Testing

template <typename A>
void printElements(A begin, A end, std::string space = " ") {
    while (begin != end) {
        std::cout << *begin++ << space;
    }

    std::cout << std::endl;
}



template <typename A, typename B>
double checkAndTime(A begin, A end
    , B ansBegin, B ansEnd
    , const std::string commandString
    , size_t k) {

    std::vector<int> inputArray(begin, end);
    std::vector<int> expect(ansBegin, ansEnd);

    clock_t startTime = clock();
    std::vector<int> result =
        solveProblem(inputArray, commandString, k);
    clock_t endTime = clock();


    if (result != expect) {
        std::cout << std::endl
            << "====================================" << std::endl;
        std::cout << "K = " << k << std::endl;
        std::cout << "Input: ";
        printElements(begin, end);
        std::cout << "commandString: " << commandString << std::endl;
        std::cout << "Result: ";
        printElements(result.begin(), result.end());
        std::cout << "Expect: ";
        printElements(expect.begin(), expect.end());

        throw std::runtime_error("TEST FAILED!");
    }


    double duration =
        static_cast<double> (endTime - startTime) / CLOCKS_PER_SEC;

    if (duration >= TIME_LIMIT) {
        std::cout << "Time: " << duration << std::endl;
        std::cout << "Limit: " << TIME_LIMIT << std::endl;
        throw std::runtime_error("TIME LIMIT FAILED!");
    }

    return duration;
}



void simpleTest0() {
    std::string c("RRLL");
    int a[7] = {
        4, 2, 1, 3, 6, 5, 7
    };
    int ans[4] = {
        4, 2, 2, -1
    };

    std::cout << "Simple Test vol.0 ... ";
    std::cout << checkAndTime(a, a + 7, ans, ans + 4, c, 2)
        << " sec." << std::endl;
}



void simpleTest1() {
    std::string c("RLRLRL");
    int a[4] = {
        1, 2, 3, 4
    };
    int ans[6] = {
        1, 2, 2, 3, 3, 4
    };

    std::cout << "Simple Test vol.1 ... ";
    std::cout << checkAndTime(a, a + 4, ans, ans + 6, c, 1)
        << " sec." << std::endl;
}



void trivialTest() {
    std::string c("RRRRRRRRRLLLLLLLLL");
    int a[10] = {
        1, 1, 1, 1, 1,
        1, 1, 1, 1, 1
    };
    int ans[18] = {
        1, 1, 1, 1, 1,
        1, 1, 1, 1, 1,
        1, 1, 1, 1, 1,
        1, 1, 1
    };

    std::cout << "Simple Test vol.1 ... ";
    std::cout << checkAndTime(a, a + 10, ans, ans + 18, c, 1)
        << " sec." << std::endl;
}



void bigTest0() {
    const size_t size = 10000;
    std::vector<int> v(size, 0);
    for (size_t i = 0; i < size; ++i)
        v[i] = i;

    std::vector<int> ans(2 * size - 2, 0);
    std::string c;
    for (size_t i = 0; i < (2 * size - 2); ++i) {
        if (i < size - 1) {
            ans[i] = 0;
            c += "R";
        } else {
            ans[i] = i - size + 2;
            c += "L";
        }
    }

    std::cout << "Big Test vol.0 ... ";
    std::cout <<
        checkAndTime(v.begin(), v.end(), ans.begin(), ans.end(), c, 1)
        << " sec." << std::endl;
}



void bigTest1() {
    const size_t size = 10000;
    std::vector<int> v(size, 1000000000);

    std::vector<int> ans(2 * size - 2, 1000000000);
    std::string c;
    for (size_t i = 0; i < (2 * size - 2); ++i) {
        if (i < size - 1) {
            c += "R";
        } else {
            c += "L";
        }
    }

    std::cout << "Big Test vol.1 ... ";
    std::cout <<
        checkAndTime(v.begin(), v.end(), ans.begin(), ans.end(), c, 1)
        << " sec." << std::endl;
}



void bigTest2() {
    const size_t size = 10000;
    std::vector<int> v(size, 0);
    for (size_t i = 0; i < size; ++i)
        v[i] = size - i - 1;

    std::vector<int> ans(2 * size - 2, 0);
    std::string c;
    for (size_t i = 0; i < (2 * size - 2); ++i) {
        if (i < size - 1) {
            ans[i] = size - i - 2;
            c += "R";
        } else {
            ans[i] = 0;
            c += "L";
        }
    }

    std::cout << "Big Test vol.2 ... ";
    std::cout <<
        checkAndTime(v.begin(), v.end(), ans.begin(), ans.end(), c, 1)
        << " sec." << std::endl;
}




// ===================> Stress testing

std::vector<int> randomVector(int maxValue
    , int minValue
    , size_t maxSize) {

    size_t size = rand() % maxSize + 1;
    std::vector<int> res(size, 0);

    for (int i = 0; i < size; ++i) {
        res[i] = rand() % (maxValue - minValue) + minValue;
    }

    return res;
}



std::string randomCommandString(size_t maxLength) {
    if (!maxLength)
        return std::string();

    size_t size = rand() % maxLength + 1;
    size_t rCount = 0;
    size_t lCount = 0;

    std::string result;
    while (rCount + lCount < size) {
        if (rand() % 2 == 0) {
            if (rCount < maxLength / 2) {
                result += "R";
                ++rCount;
            }
        } else if (lCount < rCount) {
            result += "L";
            ++lCount;
        }
    }

    return result;
}



std::vector<int> trivialSolving(const std::vector<int> & inputArray
    , const std::string & commandString
    , size_t k) {
    std::vector<int> queue;
    std::vector<int> result;

    size_t L = 0;
    size_t R = 0;

    queue.push_back(inputArray[0]);

    for (std::string::const_iterator it = commandString.begin()
        ; it != commandString.end(); ++it) {
        if (*it == PUSH) {
            ++R;
            queue.push_back(inputArray[R]);

        } else {
            ++L;
            queue.erase(queue.begin());
        }


        if (queue.size() < k) {
            result.push_back(-1);

        } else {
            std::vector<int> sortedQueue(queue);
            std::sort(sortedQueue.begin(), sortedQueue.end());

            result.push_back(sortedQueue[k - 1]);
        }
    }


    return result;
}



void stressTest(size_t N) {
    for (size_t i = 0; i < N; ++i) {
        std::cout << "Stress test N "
            << i + 1 << " ... ";

        std::vector<int> in = randomVector(10000, 0, 10000);
        std::string c = randomCommandString(in.size() * 2 - 2);
        size_t k = rand() % 10000 + 1;

        std::vector<int> ans =
            trivialSolving(in, c, k);
//        std::cout << "DEBUG:: K = " << k << std::endl;
//        std::cout << "DEBUG::" << c << std::endl;
//        printElements(in.begin(), in.end());
//        printElements(ans.begin(), ans.end());

        double duration =
            checkAndTime(in.begin(), in.end(), ans.begin(), ans.end(), c, k);

        std::cout << duration << " sec." << std::endl;
    }
}


int main() {
    std::srand(360);
    simpleTest0();
    simpleTest1();
    trivialTest();
    bigTest0();
    bigTest1();
    bigTest2();
    stressTest(10000);
    /*
    size_t k;
    size_t n;
    size_t m;

    std::cin >> n >> m >> k;

    std::vector<int> inputArray;

    for (size_t i = 0; i < n; ++i) {
        std::cin >> m;
        inputArray.push_back(m);
    }

    std::string commandString;
    std::cin >> commandString;

    solveProblem(inputArray, commandString, std::cout, k);
    */

    return 0;
}
