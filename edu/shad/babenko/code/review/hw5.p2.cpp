/*
 *  Copyright (c) 2012 Aman Orazaev
 *
 *  Homework 5, Problem 2
 *
 *  Parking problem.
 *
 */
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
class TNode {
    TNode* parent;
    T data;
    bool isLeaf;

    TNode<T>* leftSon;
    TNode<T>* rightSon;

    public:
    TNode(const T& data, bool isLeaf);

    void SetParent(TNode<T>* const);
    TNode* GetParent() const;

    TNode* GetBrother() const;

    T GetData() const;
    void SetData(const T& newData);

    bool IsLeaf() const;

    bool IsLeftSon() const;
    bool IsRightSon() const;

    TNode<T>* GetLeftSon() const;
    TNode<T>* GetRightSon() const;

    void SetLeftSon(TNode<T>* const);
    void SetRightSon(TNode<T>* const);

    void Print() const;

    virtual ~TNode();
};


template <typename T>
TNode<T>::TNode(const T& Data, bool isLeaf = false)
    : parent(NULL)
    , data(Data)
    , isLeaf(isLeaf)
    , leftSon(NULL)
    , rightSon(NULL) {
}


template <typename T>
void TNode<T>::SetParent(TNode<T>* const newAncestor) {
    parent = newAncestor;
}


template <typename T>
TNode<T>* TNode<T>::GetParent() const {
    return parent;
}

template <typename T>
TNode<T>* TNode<T>::GetBrother() const {
    if (GetParent() == NULL) {
        return NULL;
    }

    if (IsRightSon()) {
        return GetParent()->GetLeftSon();
    }

    return GetParent()->GetRightSon();
}

template <typename T>
T TNode<T>::GetData() const {
    return data;
}

template <typename T>
void TNode<T>::SetData(const T& newData) {
    data = newData;
}

template <typename T>
bool TNode<T>::IsLeaf() const {
    return isLeaf;
}

template <typename T>
bool TNode<T>::IsLeftSon() const {
    if (GetParent() == NULL) {
        return false;
    }

    return GetParent()->GetLeftSon() == this ? true : false;
}

template <typename T>
bool TNode<T>::IsRightSon() const {
    if (GetParent() == NULL) {
        return false;
    }

    return GetParent()->GetRightSon() == this ? true : false;
}

template <typename T>
TNode<T>* TNode<T>::GetLeftSon() const {
    return leftSon;
}


template <typename T>
TNode<T>* TNode<T>::GetRightSon() const {
    return rightSon;
}


template <typename T>
void TNode<T>::SetLeftSon(TNode<T>* const node) {
    leftSon = node;
}


template <typename T>
void TNode<T>::SetRightSon(TNode<T>* const node) {
    rightSon = node;
}



template <typename T>
void TNode<T>::Print() const {
    std::cout << "(" << GetData() << ", "
              << GetParent() << ") ";
    if (!IsLeaf()) {
        if (GetLeftSon() != NULL) {
            GetLeftSon()->Print();
        } else {
            std::cout << "(NULL) ";
        }
        if (GetRightSon() != NULL) {
            GetRightSon()->Print();
        } else {
            std::cout << "(NULL) ";
        }
    }
}

template <typename T>
TNode<T>::~TNode() {
}








///////////////////////////// struct TParkingPlace  ////////////////////////

typedef size_t TPlaceNumber;
const TPlaceNumber NOT_FREE = static_cast<TPlaceNumber>(-1);



class TParkingPlace: public TNode<TPlaceNumber> {
    public:
    explicit TParkingPlace(const TPlaceNumber& Data);

    bool IsFree() const;

    void Leave(const TPlaceNumber& placeNumber);
    TPlaceNumber Take();
};


TParkingPlace::TParkingPlace(const TPlaceNumber& Data)
    : TNode<TPlaceNumber>(Data, true) {
}

bool TParkingPlace::IsFree() const {
    return GetData() != NOT_FREE;
}


void TParkingPlace::Leave(const TPlaceNumber& placeNumber) {
    assert(!IsFree());

    SetData(placeNumber);
}


TPlaceNumber TParkingPlace::Take() {
    assert(IsFree());

    TPlaceNumber takenPlaceNumber = GetData();
    SetData(NOT_FREE);

    return takenPlaceNumber;
}














////////////////////////////////// TParking ////////////////////////////////


class TParkingAnswer {
    TPlaceNumber answer;
    int errorCode;

    public:
    TParkingAnswer()
        : answer(0)
        , errorCode(0) {
    }

    TParkingAnswer(TPlaceNumber answer, int errorCode = 0)
        : answer(answer)
        , errorCode(errorCode) {
    }

    bool IsOk() const {
        return GetErrorCode() == 0;
    }

    int GetErrorCode() const {
        return errorCode;
    }

    TPlaceNumber GetAnswer() const {
        return answer;
    }
};




class TParking {
    std::vector<TParkingPlace> places;
    std::vector<TNode<TPlaceNumber> > rangeTreeNodes;
    TNode<TPlaceNumber>* root;


    void ConstructTree();
    TNode<TPlaceNumber>* ConstructNode(
        TNode<TPlaceNumber>* const parent,
        size_t rangeStart,
        size_t rangeSize);

    TParkingPlace* GetNextFreePlace(
        const TNode<TPlaceNumber>* const node);

    bool IsNeedToUpdate(TNode<TPlaceNumber>* const parent) const;
    void Update(TNode<TPlaceNumber>* const node);

    TParking(const TParking&);
    TParking& operator= (const TParking&);
    public:
    explicit TParking(size_t NumberOfPlaces);
    void Init();

    TParkingAnswer TakePlace(const TPlaceNumber& placeNumber);
    TParkingAnswer LeavePlace(const TPlaceNumber& placeNumber);

    void PrintTree() const;

    static const int NO_PLACE_TO_BE_TAKEN_ERROR = -1;
    static const int PLACE_WAS_FREE_ERROR = -2;
};


TParking::TParking(size_t NumberOfPlaces)
    : places(NumberOfPlaces, TParkingPlace(0))
    , rangeTreeNodes()
    , root(NULL) {
    for (size_t i = 0; i < places.size(); ++i) {
        places.at(i).SetData(i);
    }

    size_t rangeTreeNodesSize =
        pow(2, ceil(log(NumberOfPlaces) / log(2)));

    if (rangeTreeNodesSize != 0) {
        rangeTreeNodes.reserve(rangeTreeNodesSize);
    }
}



void TParking::Init() {
    ConstructTree();
}



void TParking::ConstructTree() {
    size_t size = places.size();

    if (size == 0) {
        return;
    }

    size_t rangeSize = pow(2, ceil(log(size) / log(2)));

    root = ConstructNode(
        NULL,
        0,
        rangeSize);
}



TNode<TPlaceNumber>* TParking::ConstructNode(
    TNode<TPlaceNumber>* const parent,
    size_t rangeStart,
    size_t rangeSize
) {
    if (rangeStart >= places.size()) {
        return NULL;
    }

    if (rangeSize == 1) {
        places.at(rangeStart).SetParent(parent);
        return &places.at(rangeStart);
    }

    assert(rangeSize % 2 == 0);

    rangeTreeNodes.push_back(TNode<TPlaceNumber>(rangeStart));

    TNode<TPlaceNumber>* result = &rangeTreeNodes.back();
    result->SetParent(parent);


    rangeSize /= 2;

    result->SetLeftSon(
        ConstructNode(result, rangeStart, rangeSize));

    result->SetRightSon(
        ConstructNode(result, rangeStart + rangeSize, rangeSize));

    return result;
}



TParkingPlace* TParking::GetNextFreePlace(
    const TNode<TPlaceNumber>* const node
) {
    if (!node->IsLeftSon()) {
        if (node->GetParent() == NULL) {
            assert(node->GetData() != NOT_FREE);
            return &places.at(node->GetData());
        }

        return GetNextFreePlace(node->GetParent());
    }

    TNode<TPlaceNumber>* rightBrother =
            node->GetBrother();
    if (rightBrother == NULL || rightBrother->GetData() == NOT_FREE) {
        return GetNextFreePlace(node->GetParent());
    }

    return &places.at(rightBrother->GetData());
}



bool TParking::IsNeedToUpdate(TNode<TPlaceNumber>* const parent) const {
    if (parent == NULL) {
        return false;
    }

    TPlaceNumber leftSonData = parent->GetLeftSon() == NULL ?
        static_cast<size_t>(-1) :
        parent->GetLeftSon()->GetData();
    TPlaceNumber rightSonData = parent->GetRightSon() == NULL ?
        static_cast<size_t>(-1) :
        parent->GetRightSon()->GetData();

    if (parent->GetData() != std::min(leftSonData, rightSonData)) {
        return true;
    }

    return false;
}



void TParking::Update(TNode<TPlaceNumber>* const node) {
    TNode<TPlaceNumber>* brother = node->GetBrother();
    TNode<TPlaceNumber>* parent = node->GetParent();

    if (IsNeedToUpdate(parent)) {
        if (brother == NULL || brother->GetData() > node->GetData()) {
            parent->SetData(node->GetData());
        } else {
            parent->SetData(brother->GetData());
        }
        Update(parent);
    }
}



TParkingAnswer TParking::TakePlace(const TPlaceNumber& placeNumber) {
    if (root->GetData() == NOT_FREE) {
        return TParkingAnswer(placeNumber, NO_PLACE_TO_BE_TAKEN_ERROR);
    }

    TParkingPlace* placeLeaf = &places.at(placeNumber);

    if (placeLeaf->IsFree()) {
        placeLeaf->Take();
        Update(placeLeaf);
        return TParkingAnswer(placeNumber);
    }

    TParkingPlace* nextFreeLeaf = GetNextFreePlace(placeLeaf);
    TParkingAnswer ans = nextFreeLeaf->Take();
    Update(nextFreeLeaf);

    return ans;
}


TParkingAnswer TParking::LeavePlace(const TPlaceNumber& placeNumber) {
    TParkingPlace* placeLeaf = &places.at(placeNumber);

    if (placeLeaf->IsFree()) {
        return TParkingAnswer(placeNumber, PLACE_WAS_FREE_ERROR);
    }

    placeLeaf->Leave(placeNumber);
    Update(placeLeaf);

    return TParkingAnswer(-1);
}


void TParking::PrintTree() const {
    root->Print();
    std::cout << std::endl;
}








////////////////////////////////// TEvent //////////////////////////////////

class TEvent {
    bool isTakePlaceEvent;
    protected:
    TPlaceNumber place;

    public:
    explicit TEvent(const TPlaceNumber&, bool isTakePlaceEvent);

    bool IsTakePlaceEvent() const;

    /* For stress testing */
    std::string Str() const;
    TPlaceNumber GetPlace() const;

    virtual ~TEvent();
};


TEvent::TEvent(const TPlaceNumber& place, bool isTakePlaceEvent)
    : isTakePlaceEvent(isTakePlaceEvent)
    , place(place) {
}


bool TEvent::IsTakePlaceEvent() const {
    return isTakePlaceEvent;
}


std::string TEvent::Str() const {
    char eventString[20];

    if (IsTakePlaceEvent()) {
        sprintf(eventString, "(+ %d)", GetPlace());
    } else {
        sprintf(eventString, "(- %d)", GetPlace());
    }

    return std::string(eventString);
}


TPlaceNumber TEvent::GetPlace() const {
    return place;
}


TEvent::~TEvent() {
}




std::ostream& operator<< (std::ostream& out, const TEvent event) {
    out << event.Str();

    return out;
}







/////////////////////////////// ParkingManager //////////////////////////////


class TParkingManager {
    TParking* pParking;

    public:
    explicit TParkingManager(TParking* const pParking)
        : pParking(pParking) {
    }

    TParkingAnswer Process(const TEvent& event) {
        TParkingAnswer ans;

        if (event.IsTakePlaceEvent()) {
            return pParking->TakePlace(event.GetPlace());
        }

        return pParking->LeavePlace(event.GetPlace());
    }

    std::vector<TParkingAnswer> Process(const std::vector<TEvent>& events) {
        std::vector<TParkingAnswer> result;
        result.reserve(events.size());

        for (std::vector<TEvent>::const_iterator event = events.begin();
             event != events.end();
             ++event
        ) {
            result.push_back(Process(*event));
        }

        return result;
    }
};







/////////////////////////////// ProcessEvents ///////////////////////////////




std::vector<TParkingAnswer> ProcessEvents(
    TParkingManager& parkingManager,
    const std::vector<TEvent>& events
) {
    std::vector<TParkingAnswer> result;
    result.reserve(events.size());

    for (std::vector<TEvent>::const_iterator event = events.begin();
         event != events.end();
         ++event
    ) {
        result.push_back(parkingManager.Process(*event));
    }

    return result;
}




std::vector<TParkingAnswer> ReadAndProcessEvents(std::istream& in) {
    size_t parkingSize;
    size_t eventsSize;

    in >> parkingSize >> eventsSize;

    std::vector<TEvent> events;
    events.reserve(eventsSize);

    char eventType = 0;
    size_t eventPlaceNo = 0;
    for (size_t event = 0; event < eventsSize; ++event) {
        in >> eventType >> eventPlaceNo;

        switch (eventType) {
            case '+':
                events.push_back(
                    TEvent(eventPlaceNo - 1, /*IsTakePlaceEevnt*/ true));
                break;
            case '-':
                events.push_back(
                    TEvent(eventPlaceNo - 1, /*IsTakePlaceEvent*/ false));
                break;
            default:
                assert(false);
        }
    }

    TParking parking(parkingSize);
    parking.Init();

    TParkingManager manager(&parking);

    return manager.Process(events);
}











int main() {
    std::vector<TParkingAnswer> result(ReadAndProcessEvents(std::cin));

    for (std::vector<TParkingAnswer>::const_iterator cit = result.begin();
         cit != result.end();
         ++cit) {
        if (cit->IsOk()) {
            std::cout << cit->GetAnswer() + 1 << std::endl;
        } else {
            std::cout << cit->GetErrorCode() << std::endl;
        }
    }

    return 0;
}
