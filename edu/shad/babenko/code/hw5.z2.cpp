#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

#include <cmath>
#include <cassert>
#include <cstdlib>
#include <cstdio>



////////////////////////////// Nodes description ///////////////////////////


template <typename T>
class TCommonNode {
    TCommonNode* ancestor;
    T data;

    bool IsNeedToUpdateAncestor() const;

    public:
    TCommonNode(const T& data);

    void SetAncestor(TCommonNode<T>* const );
    TCommonNode* GetAncestor() const;

    TCommonNode* GetBrother() const;

    T GetData() const;
    void SetData(const T& newData);

    virtual bool IsLeaf() const = 0;

    bool IsLeftSon() const;
    bool IsRightSon() const;

    virtual TCommonNode<T>* GetLeftSon() const;
    virtual TCommonNode<T>* GetRightSon() const;

    virtual void SetLeftSon(TCommonNode<T>* const);
    virtual void SetRightSon(TCommonNode<T>* const);

    void Update() const;

    void Print() const;

    virtual ~TCommonNode();
};


template <typename T>
TCommonNode<T>::TCommonNode(const T& Data)
    : ancestor(0)
    , data(Data) {
}


template <typename T>
void TCommonNode<T>::SetAncestor(TCommonNode<T>* const newAncestor) {
    ancestor = newAncestor;
}


template <typename T>
TCommonNode<T>* TCommonNode<T>::GetAncestor() const {
    return ancestor;
}

template <typename T>
TCommonNode<T>* TCommonNode<T>::GetBrother() const {
    if (GetAncestor() == 0) {
        return 0;
    }

    if (IsRightSon()) {
        return GetAncestor()->GetLeftSon();
    }

    return GetAncestor()->GetRightSon();
}


template <typename T>
T TCommonNode<T>::GetData() const {
    return data;
}


template <typename T>
void TCommonNode<T>::SetData(const T& newData) {
    data = newData;
}


template <typename T>
bool TCommonNode<T>::IsLeftSon() const {
    if (GetAncestor() == 0) {
        return false;
    }

    return GetAncestor()->GetLeftSon() == this ? true : false;
}

template <typename T>
bool TCommonNode<T>::IsRightSon() const {
    if (GetAncestor() == 0) {
        return false;
    }

    return GetAncestor()->GetRightSon() == this ? true : false;
}

template <typename T>
TCommonNode<T>* TCommonNode<T>::GetLeftSon() const {
    assert(false);
    return 0;
}


template <typename T>
TCommonNode<T>* TCommonNode<T>::GetRightSon() const {
    assert(false);
    return 0;
}


template <typename T>
void TCommonNode<T>::SetLeftSon(TCommonNode<T>* const) {
    assert(false);
}


template <typename T>
void TCommonNode<T>::SetRightSon(TCommonNode<T>* const) {
    assert(false);
}

template <typename T>
bool TCommonNode<T>::IsNeedToUpdateAncestor() const {
    if (GetAncestor() == 0) {
        return false;
    }

    if (GetData() != GetAncestor()->GetData()) {
        if (GetBrother() == 0 ||
            (GetData() < GetBrother()->GetData() ||
            GetBrother()->GetData() != GetAncestor()->GetData())
        ) {
            return true;
        }
    }

    return false;
}

template <typename T>
void TCommonNode<T>::Update() const {
    if (IsNeedToUpdateAncestor()) {
        if (GetBrother() == 0 || GetBrother()->GetData() > GetData()) {
            GetAncestor()->SetData(GetData());
            GetAncestor()->Update();
        } else {
            GetAncestor()->SetData(GetBrother()->GetData());
            GetAncestor()->Update();
        }
    }
}


template <typename T>
void TCommonNode<T>::Print() const {
    std::cout << "(" << GetData() << ", "
              << GetAncestor() << ") ";
    if (!IsLeaf()) {
        if (GetLeftSon() != 0) {
            GetLeftSon()->Print();
        } else {
            std::cout << "(NULL) ";
        }
        if (GetRightSon() != 0) {
            GetRightSon()->Print();
        } else {
            std::cout << "(NULL) ";
        }
    }
}

template <typename T>
TCommonNode<T>::~TCommonNode() {
}







template <typename T>
class TLeafNode: public TCommonNode<T> {
    public:
    TLeafNode(const T& Data);

    bool IsLeaf() const;
};


template <typename T>
TLeafNode<T>::TLeafNode(const T& Data)
    : TCommonNode<T>(Data) {

}


template <typename T>
bool TLeafNode<T>::IsLeaf() const {
    return true;
}








template <typename T>
class TNode: public TCommonNode<T> {
    TCommonNode<T>* leftSon;
    TCommonNode<T>* rightSon;

    TNode& operator=(const TNode& );
    TNode(const TNode& );


    public:
    TNode(const T& Data);

    void SetLeftSon(TCommonNode<T>* const newLeftSon);
    void SetRightSon(TCommonNode<T>* const newRightSon);

    TCommonNode<T>* GetLeftSon() const;
    TCommonNode<T>* GetRightSon() const;

    bool IsLeaf() const;
};


template <typename T>
TNode<T>::TNode(const T& Data)
    : TCommonNode<T>(Data)
    , leftSon(0)
    , rightSon(0) {
}


template <typename T>
bool TNode<T>::IsLeaf() const {
    return false;
}


template <typename T>
void TNode<T>::SetLeftSon(TCommonNode<T>* const newLeftSon) {
    leftSon = newLeftSon;
}


template <typename T>
void TNode<T>::SetRightSon(TCommonNode<T>* const newRightSon) {
    rightSon = newRightSon;
}


template <typename T>
TCommonNode<T>* TNode<T>::GetRightSon() const {
    return rightSon;
}


template <typename T>
TCommonNode<T>* TNode<T>::GetLeftSon() const {
    return leftSon;
}







///////////////////////////// struct TParkingPlace  ////////////////////////

typedef size_t TPlaceNumber;
const TPlaceNumber NOT_FREE = static_cast<TPlaceNumber>(-1);



class TParkingPlace: public TLeafNode<TPlaceNumber> {
    public:
    TParkingPlace(const TPlaceNumber& Data);

    bool IsFree() const;

    void Leave(const TPlaceNumber& placeNumber);
    TPlaceNumber Take();
};


TParkingPlace::TParkingPlace(const TPlaceNumber& Data)
    : TLeafNode<TPlaceNumber>(Data) {
}

bool TParkingPlace::IsFree() const {
    return GetData() != NOT_FREE;
}


void TParkingPlace::Leave(const TPlaceNumber& placeNumber) {
    assert(!IsFree());

    SetData(placeNumber);
    Update();
}


TPlaceNumber TParkingPlace::Take() {
    assert(IsFree());

    TPlaceNumber takenPlaceNumber = GetData();
    SetData(NOT_FREE);
    Update();

    return takenPlaceNumber;
}
















////////////////////////////////// TParking ////////////////////////////////


const int NO_PLACE_TO_BE_TAKEN_ERROR = -1;
const int PLACE_WAS_FREE_ERROR = -2;


class TParking {
    std::vector<TParkingPlace> places;
    TCommonNode<TPlaceNumber>* root;


    void ConstructTree();
    TCommonNode<TPlaceNumber>* ConstructNode(
        TCommonNode<TPlaceNumber>* const ancestor,
        size_t rangeStart,
        size_t rangeSize
    );
    void DeleteNode(TCommonNode<TPlaceNumber>* const node);

    TCommonNode<TPlaceNumber>* const GetNearestPlaceNode(TCommonNode<TPlaceNumber>* const) const;
    TParkingPlace* GetNextFreePlace(const TCommonNode<TPlaceNumber>* const node);

    TParking(const TParking& );
    TParking& operator= (const TParking& );
    public:

    TParking(size_t NumberOfPlaces);

    int takePlace(const TPlaceNumber& placeNumber);
    int leavePlace(const TPlaceNumber& placeNumber);

    void PrintTree() const;

    ~TParking();
};




TParking::TParking(size_t NumberOfPlaces)
    : places(NumberOfPlaces, TParkingPlace(0))
    , root(0)
{
    for (size_t i = 0; i < places.size(); ++i) {
        places[i].SetData(i + 1);
    }

    ConstructTree();
}



void TParking::ConstructTree() {
    size_t size = places.size();

    if (size == 0) {
        return;
    }

    size_t rangeSize = pow(2, ceil(log(size) / log(2)));

    root = ConstructNode(
        0,
        1,
        rangeSize
    );
}


TCommonNode<TPlaceNumber>* TParking::ConstructNode(
    TCommonNode<TPlaceNumber>* const ancestor,
    size_t rangeStart,
    size_t rangeSize
) {
    if (rangeStart > places.size()) {
        return 0;
    }

    if (rangeSize == 1) {
        places[rangeStart -1].SetAncestor(ancestor);
        return &places[rangeStart - 1];
    }

    assert(rangeSize % 2 == 0);

    TCommonNode<TPlaceNumber>* result = new TNode<TPlaceNumber>(rangeStart);
    result->SetAncestor(ancestor);


    rangeSize /= 2;

    result->SetLeftSon(
        ConstructNode(result, rangeStart, rangeSize)
    );

    result->SetRightSon(
        ConstructNode(result, rangeStart + rangeSize, rangeSize)
    );

    return result;
}


void TParking::DeleteNode(TCommonNode<TPlaceNumber>* const node) {
    if (node == 0) {
        return;
    }

    if (!node->IsLeaf()) {
        DeleteNode(node->GetLeftSon());
        DeleteNode(node->GetRightSon());

        delete node;
    }
}


TCommonNode<TPlaceNumber>* const TParking::GetNearestPlaceNode(
    TCommonNode<TPlaceNumber>* const node
) const {
    if (node->GetData() != NOT_FREE) {
        return node;
    }
    
    if (node->GetAncestor() == 0) {
        return 0;
    }

    return GetNearestPlaceNode(node->GetAncestor());
}

TParkingPlace* TParking::GetNextFreePlace(
    const TCommonNode<TPlaceNumber>* const node
) {
    if (!node->IsLeftSon()) {
        if (node->GetAncestor() == 0) {
            assert(node->GetData() != NOT_FREE);
            return &places[node->GetData() - 1];
        }

        return GetNextFreePlace(node->GetAncestor());
    }

    TCommonNode<TPlaceNumber>* rightBrother =
            node->GetBrother();
    if (rightBrother == 0 || rightBrother->GetData() == NOT_FREE) {
        return GetNextFreePlace(node->GetAncestor());
    }

    return &places[rightBrother->GetData() - 1];
}

int TParking::takePlace(const TPlaceNumber& placeNumber) {
    if (root->GetData() == NOT_FREE) {
        return NO_PLACE_TO_BE_TAKEN_ERROR;
    }

    TParkingPlace* placeLeaf = &places[placeNumber - 1];
    
    if (placeLeaf->IsFree()) {
        placeLeaf->Take();
        return placeNumber;
    }

    return static_cast<int>(GetNextFreePlace(placeLeaf)->Take());
}


int TParking::leavePlace(const TPlaceNumber& placeNumber) {
    TParkingPlace* placeLeaf = &places[placeNumber - 1];

    if (placeLeaf->IsFree()) {
        return PLACE_WAS_FREE_ERROR;
    }

    placeLeaf->Leave(placeNumber);

    return 0;
}


void TParking::PrintTree() const {
    root->Print();
    std::cout << std::endl;
}


TParking::~TParking() {
    DeleteNode(root);
}






////////////////////////////////// TEvent //////////////////////////////////

class TEvent {
    protected:
    TPlaceNumber place;
    public:
    TEvent(const TPlaceNumber&);
    
    virtual int Process(TParking&) const = 0;

    /* For stress testing */
    virtual bool IsTakePlaceEvent() const = 0;
    virtual std::string Str() const = 0;
    TPlaceNumber GetPlace() const;
    
    virtual ~TEvent();
};

TEvent::TEvent(const TPlaceNumber& Place)
    : place(Place) {
}


TPlaceNumber TEvent::GetPlace() const {
    return place;
}


TEvent::~TEvent() {
}




class TLeavePlaceEvent: public TEvent {
    public:
    TLeavePlaceEvent(const TPlaceNumber&);
    
    virtual int Process(TParking&) const;

    bool IsTakePlaceEvent() const;
    std::string Str() const;
};


TLeavePlaceEvent::TLeavePlaceEvent(const TPlaceNumber& Place)
    : TEvent(Place) {
}


bool TLeavePlaceEvent::IsTakePlaceEvent() const {
    return false;
}


std::string TLeavePlaceEvent::Str() const {
    char eventString[20];
    sprintf(eventString, "(- %d)", place);

    return std::string(eventString);
}


int TLeavePlaceEvent::Process(TParking& parking) const {
    return parking.leavePlace(place);
}




class TTakePlaceEvent: public TEvent {
    public:
    TTakePlaceEvent(const TPlaceNumber&);

    virtual int Process(TParking&) const;

    bool IsTakePlaceEvent() const;
    std::string Str() const;
};


TTakePlaceEvent::TTakePlaceEvent(const TPlaceNumber& Place)
    : TEvent(Place) {
}

bool TTakePlaceEvent::IsTakePlaceEvent() const {
    return true;
}


std::string TTakePlaceEvent::Str() const {
    char eventString[20];
    sprintf(eventString, "(+ %d)", place);

    return std::string(eventString);
}


int TTakePlaceEvent::Process(TParking& parking) const {
    return parking.takePlace(place);
}




std::ostream& operator<< (std::ostream& out, const TEvent* const pEvent) {
    out << pEvent->Str();

    return out;
}








/////////////////////////////// ProcessEvents ///////////////////////////////


std::vector<int> ProcessEvents(
    TParking& parking,
    const std::vector<TEvent*>& events
) {
    std::vector<int> result;
    result.reserve(events.size());

    for (std::vector<TEvent*>::const_iterator event = events.begin();
         event != events.end();
         ++event
    ) {
        result.push_back((*event)->Process(parking));
    }

    return result;
}







/////////////////////////// NativeTestProcesEvents /////////////////////////


std::vector<int> NativeTestProcesEvents(
    size_t parkingSize,
    const std::vector<TEvent*>& events
) {
    std::vector<bool> parking(parkingSize, true);

    std::vector<int> result;
    result.reserve(events.size());

    for (std::vector<TEvent*>::const_iterator event = events.begin();
         event != events.end();
         ++event
    ) {
        if ((*event)->IsTakePlaceEvent()) {
            std::vector<bool>::iterator place = std::find(
                    parking.begin() + (*event)->GetPlace() - 1,
                    parking.end(),
                    true
            );

            if (place == parking.end()) {
                place = std::find(parking.begin(), parking.end(), true);
            }

            if (place == parking.end()) {
                result.push_back(-1);
            } else {
                result.push_back(place - parking.begin() + 1);
                *place = false;
            }
        } else {
            if (parking[(*event)->GetPlace() - 1]) {
                result.push_back(-2);
            } else {
                parking[(*event)->GetPlace() - 1] = true;
                result.push_back(0);
            }
        }
    }

    return result;
}



/////////////////////////////////// Testing ////////////////////////////////


void test_Nodes_IsLeaf() {
    TCommonNode<int>* pFirstNode = new TLeafNode<int>(10);
    TCommonNode<int>* pSecondNode = new TNode<int>(100);

    assert(pFirstNode->IsLeaf() == true);
    assert(pSecondNode->IsLeaf() == false);

    delete pFirstNode;
    delete pSecondNode;
}


void test_Nodes_Data() {
    TCommonNode<int>* pFirstNode = new TLeafNode<int>(1);
    assert(pFirstNode->GetData() == 1);

    pFirstNode->SetData(20);
    assert(pFirstNode->GetData() == 20);

    delete pFirstNode;

    pFirstNode = new TNode<int>(99);
    assert(pFirstNode->GetData() == 99);

    pFirstNode->SetData(66);
    assert(pFirstNode->GetData() == 66);

    delete pFirstNode;
}


void test_TParkingPlace() {
    TParkingPlace place(10);

    assert(place.IsFree() == true);

    place.Take();
    assert(place.IsFree() == false);

    place.Leave(10);
    assert(place.IsFree() == true);
}


void test_Nodes() {
    test_Nodes_IsLeaf();
    test_Nodes_Data();
    test_TParkingPlace();
}


void test_TParking_construction() {
    TParking parking0(0);
    TParking parking1(1);
    TParking parking2(1023);
}


void test_TParking() {
    test_TParking_construction();
}








////////////////////////////////// Stress Testing //////////////////////////

std::vector<TEvent*> createRandomEventVector(
    size_t parkingSize,
    size_t maxEventsNumber,
    size_t minEventsNumber = 10
) {
    size_t size = minEventsNumber + (rand() % (maxEventsNumber - minEventsNumber));

    std::vector<TEvent*> result;

    result.reserve(size);

    for (size_t i = 0; i < size; ++i) {
        if (rand() % 2) {
            result.push_back(new TTakePlaceEvent(1 + rand() % parkingSize));
        } else {
            result.push_back(new TLeavePlaceEvent(1 + rand() % parkingSize));
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
            endClocks - startClocks
        ) / CLOCKS_PER_SEC;
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
        
        std::vector<TEvent*> events = createRandomEventVector(
            parkingSize,
            maxEvets,
            1
        );

        // std::cout << "    Event vector: ";
        // printElements(events.begin(), events.end());

        std::vector<int> nativeResult = NativeTestProcesEvents(
            parkingSize,
            events
        );

        timer.Start();
        TParking parking(parkingSize);
        // std::cout << "Parking: ";
        // parking.PrintTree();
        std::vector<int> result = ProcessEvents(
            parking,
            events
        );
        timer.Stop();
        std::cout << "Time: " << timer.GetSeconds() << std::endl;

        assertEqual(
            result.begin(),
            result.end(),
            nativeResult.begin(),
            nativeResult.end()
        );

        for (std::vector<TEvent*>::iterator it = events.begin();
             it != events.end();
             ++it
        ) {
            delete *it;
        }
        std::cout << "---------------------------------------------" << std::endl;
    }
}




int main() {
    std::srand(360);
    test_Nodes();
    test_TParking();
    test_ProcessEvents_stress(30, 100000, 100000);

    return 0;
}
