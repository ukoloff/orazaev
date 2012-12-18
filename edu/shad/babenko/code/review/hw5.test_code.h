#ifndef _HW5_TESTING_CODE_H
#define _HW5_TESTING_CODE_H


/////////////////////////// NativeTestProcesEvents /////////////////////////


bool operator== (const TParkingAnswer& lhs, const TParkingAnswer& rhs) {
    if (lhs.IsOk() != rhs.IsOk()) {
        return false;
    }

    if (!lhs.IsOk() && !rhs.IsOk()) {
        return lhs.GetErrorCode() == rhs.GetErrorCode();
    }

    return lhs.GetAnswer() == rhs.GetAnswer();
}

bool operator!= (const TParkingAnswer& lhs, const TParkingAnswer& rhs) {
    return !(lhs == rhs);
}

std::ostream& operator<< (std::ostream& out, const TParkingAnswer& ans) {
    if (ans.IsOk()) {
        return out << ans.GetAnswer() + 1;
    }

    return out << ans.GetErrorCode();
}




std::vector<TParkingAnswer> NativeTestProcesEvents(
    size_t parkingSize,
    const std::vector<TEvent>& events
) {
    std::vector<bool> parking(parkingSize, true);

    std::vector<TParkingAnswer> result;
    result.reserve(events.size());

    for (std::vector<TEvent>::const_iterator event = events.begin();
         event != events.end();
         ++event
    ) {
        if ((*event).IsTakePlaceEvent()) {
            std::vector<bool>::iterator place = std::find(
                    parking.begin() + (*event).GetPlace(),
                    parking.end(),
                    true);

            if (place == parking.end()) {
                place = std::find(parking.begin(), parking.end(), true);
            }

            if (place == parking.end()) {
                result.push_back(TParkingAnswer(event->GetPlace(), true, -1));
            } else {
                result.push_back(TParkingAnswer(place - parking.begin(), true));
                *place = false;
            }
        } else {
            if (parking[(*event).GetPlace()]) {
                result.push_back(TParkingAnswer(event->GetPlace(), false, -2));
            } else {
                parking[(*event).GetPlace()] = true;
                result.push_back(TParkingAnswer(0, false));
            }
        }
    }

    return result;
}






/////////////////////////////////// Testing ////////////////////////////////


void test_TParking_construction() {
    TParking parking0(0);
    parking0.Init();
    TParking parking1(1);
    parking1.Init();
    TParking parking2(1023);
    parking2.Init();
}


void test_TParking() {
    test_TParking_construction();
}








////////////////////////////////// Stress Testing //////////////////////////

std::vector<TEvent> createRandomEventVector(
    size_t parkingSize,
    size_t maxEventsNumber,
    size_t minEventsNumber = 10
) {
    size_t size =
        minEventsNumber + (rand() % (maxEventsNumber - minEventsNumber));

    std::vector<TEvent> result;

    result.reserve(size);

    for (size_t i = 0; i < size; ++i) {
        if (rand() % 2) {
            result.push_back(TEvent(rand() % parkingSize, true));
        } else {
            result.push_back(TEvent(rand() % parkingSize, false));
        }
    }

    return result;
}


template <typename T>
void printElements(T begin, T end) {
    while (begin != end) {
        std::cout << *begin++ << " ";
    }
    std::cout << std::endl;
}


template <typename T>
void assertEqual(
    T firstBegin,
    T firstEnd,
    T secondBegin,
    T secondEnd
) {
    T firstStarts = firstBegin;
    T secondStarts = secondBegin;

    while (firstBegin != firstEnd && secondBegin != secondEnd) {
        if (*firstBegin != *secondBegin) {
            std::cerr << "Assert Equal for 2 vectors failed!" << std::endl;
            std::cerr << "vector 1: ";
            printElements(firstStarts, firstEnd);
            std::cerr << "vector 2: ";
            printElements(secondStarts, secondEnd);

            std::cout << "NOT EQUAL ELEMENTS: " << *firstBegin
                      << " " << *secondBegin << std::endl;
            abort();
        }

        ++firstBegin;
        ++secondBegin;
    }

    if (firstBegin != firstEnd || secondBegin != secondEnd) {
        std::cerr << "Assert Equal fails for 2 vectors!" << std::endl;
        std::cerr << "vector 1: ";
        printElements(firstStarts, firstEnd);
        std::cerr << "vector 2: ";
        printElements(secondStarts, secondEnd);

        abort();
    }
}


class TTimer {
    clock_t startClocks;
    clock_t endClocks;

    public:
    TTimer()
        : startClocks(0)
        , endClocks(0) {
    }

    void Start() {
        startClocks = clock();
    }

    void Stop() {
        endClocks = clock();
    }

    double GetSeconds() const {
        return static_cast<double>(
            endClocks - startClocks) / CLOCKS_PER_SEC;
    }

    clock_t GetClocks() const {
        return endClocks - startClocks;
    }
};


void test_ProcessEvents_stress(
    size_t numberOfTests,
    size_t maxParkingSize,
    size_t maxEvets) {
    TTimer timer;

    for (int testNo = 1; testNo <= numberOfTests; ++testNo) {
        std::cout << "Stress test #" << testNo << std::endl;

        size_t parkingSize = 1 + rand() % maxParkingSize;
        std::cout << "    parking size: " << parkingSize << std::endl;

        std::vector<TEvent> events = createRandomEventVector(
            parkingSize,
            maxEvets,
            1);

        // std::cout << "    Event vector: ";
        // printElements(events.begin(), events.end());

        std::vector<TParkingAnswer> nativeResult = NativeTestProcesEvents(
            parkingSize,
            events);

        timer.Start();
        TParking parking(parkingSize);
        parking.Init();
        // std::cout << "Parking: ";
        // parking.PrintTree();
        TParkingManager manager(&parking);

        std::vector<TParkingAnswer> result = manager.Process(events);
        timer.Stop();
        std::cout << "Time: " << timer.GetSeconds() << std::endl;

        assertEqual(
            result.begin(),
            result.end(),
            nativeResult.begin(),
            nativeResult.end());

        std::cout << "---------------------------------------------"
                  << std::endl;
    }
}


#endif // _HW5_TESTING_CODE_H
