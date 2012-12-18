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

typedef size_t TPlaceNumber;
const TPlaceNumber NOT_FREE = static_cast<TPlaceNumber>(-1);


class TNode;



class TNodePointer {
    std::vector<TNode>* pNodeVector;
    size_t index;

    static const size_t NULL_INDEX = static_cast<size_t>(-1);

    public:
    TNodePointer()
        : pNodeVector(NULL)
        , index(NULL_INDEX) {
    }

    TNodePointer(size_t index, std::vector<TNode>* pNodeVector)
        : pNodeVector(pNodeVector)
        , index(index) {
    }

    TNode* operator->() const {
        return &pNodeVector->at(GetIndex());
    }

    TNode* Ptr() const {
        if (GetIndex() == NULL_INDEX) {
            return NULL;
        }

        return &pNodeVector->at(GetIndex());
    }

    size_t GetIndex() const {
        return index;
    }

    bool operator== (const TNode* const pNode) const {
        if (pNode == NULL) {
            return index == NULL_INDEX;
        }

        if (pNodeVector == NULL) {
            return false;
        }

        return &pNodeVector->at(index) == pNode;
    }

    bool operator!= (const TNode* const pNode) const {
        return !this->operator==(pNode);
    }
};






class TNode {
    public:
    TNode(const TPlaceNumber& data, bool isLeaf);

    void SetParent(const TNodePointer&);
    TNodePointer GetParent() const;

    TNodePointer GetBrother() const;

    TPlaceNumber GetData() const;
    void SetData(const TPlaceNumber& newData);

    bool IsLeaf() const;

    bool IsLeftSon() const;
    bool IsRightSon() const;

    TNodePointer GetLeftSon() const;
    TNodePointer GetRightSon() const;

    void SetLeftSon(const TNodePointer&);
    void SetRightSon(const TNodePointer&);

    void Print() const;

    bool IsFree() const;
    void Leave(const TPlaceNumber& placeNumber);
    TPlaceNumber Take();

    private:
    TNodePointer parent;
    TPlaceNumber data;
    bool isLeaf;

    TNodePointer leftSon;
    TNodePointer rightSon;
};


TNode::TNode(const TPlaceNumber& Data, bool isLeaf = false)
    : parent()
    , data(Data)
    , isLeaf(isLeaf)
    , leftSon()
    , rightSon() {
}


void TNode::SetParent(const TNodePointer& newAncestor) {
    parent = newAncestor;
}


TNodePointer TNode::GetParent() const {
    return parent;
}

TNodePointer TNode::GetBrother() const {
    if (GetParent() == NULL) {
        return /* NULL by default */ TNodePointer();
    }

    if (IsRightSon()) {
        return GetParent()->GetLeftSon();
    }

    return GetParent()->GetRightSon();
}

TPlaceNumber TNode::GetData() const {
    return data;
}

void TNode::SetData(const TPlaceNumber& newData) {
    data = newData;
}

bool TNode::IsLeaf() const {
    return isLeaf;
}

bool TNode::IsLeftSon() const {
    if (GetParent() == NULL) {
        return false;
    }

    return GetParent()->GetLeftSon().Ptr() == this ? true : false;
}

bool TNode::IsRightSon() const {
    if (GetParent() == NULL) {
        return false;
    }

    return GetParent()->GetRightSon().Ptr() == this ? true : false;
}

TNodePointer TNode::GetLeftSon() const {
    return leftSon;
}


TNodePointer TNode::GetRightSon() const {
    return rightSon;
}


void TNode::SetLeftSon(const TNodePointer& node) {
    leftSon = node;
}


void TNode::SetRightSon(const TNodePointer& node) {
    rightSon = node;
}



void TNode::Print() const {
    std::cout << "(" << GetData() << ")";
    if (!IsLeaf()) {
        if (GetLeftSon() != NULL) {
            GetLeftSon()->Print();
        } else {
            std::cout << "(null) ";
        }
        if (GetRightSon() != NULL) {
            GetRightSon()->Print();
        } else {
            std::cout << "(null) ";
        }
    }
}


bool TNode::IsFree() const {
    return GetData() != NOT_FREE;
}


void TNode::Leave(const TPlaceNumber& placeNumber) {
    assert(!IsFree());

    SetData(placeNumber);
}


TPlaceNumber TNode::Take() {
    assert(IsFree());

    TPlaceNumber takenPlaceNumber = GetData();
    SetData(NOT_FREE);

    return takenPlaceNumber;
}








////////////////////////////////// TParking ////////////////////////////////


class TParkingAnswer {
    TPlaceNumber answer;
    bool itWasTakeEvent;
    int errorCode;

    public:
    TParkingAnswer()
        : answer(0)
        , errorCode(0) {
    }

    TParkingAnswer(TPlaceNumber answer, bool itWasTakeEvent, int errorCode = 0)
        : answer(answer)
        , itWasTakeEvent(itWasTakeEvent)
        , errorCode(errorCode) {
    }

    bool IsOk() const {
        return GetErrorCode() == 0;
    }

    bool ItWasTakeEvent() const {
        return itWasTakeEvent;
    }

    int GetErrorCode() const {
        return errorCode;
    }

    TPlaceNumber GetAnswer() const {
        return answer;
    }
};




class TParking {
    std::vector<TNode> rangeTreeNodes;
    size_t parkingSize;
    size_t lastInRangeTree;
    TNodePointer root;


    void ConstructTree();
    TNodePointer ConstructNode(
        const TNodePointer& parent,
        size_t rangeStart,
        size_t rangeSize);

    TNodePointer GetNextFreePlace(
        const TNodePointer& node);

    void Update(TNodePointer node);

    TParking(const TParking&);
    TParking& operator= (const TParking&);
    public:
    explicit TParking(size_t numberOfPlaces);
    void Init();

    TParkingAnswer TakePlace(const TPlaceNumber& placeNumber);
    TParkingAnswer LeavePlace(const TPlaceNumber& placeNumber);

    void PrintTree() const;

    static const int NO_PLACE_TO_BE_TAKEN_ERROR = -1;
    static const int PLACE_WAS_FREE_ERROR = -2;
    static const int LEAVE_PLACE_OK = 0;
};


TParking::TParking(size_t numberOfPlaces)
    : rangeTreeNodes()
    , parkingSize(numberOfPlaces)
    , root() {
    size_t rangeTreeNodesSize =
        pow(2, ceil(log(numberOfPlaces) / log(2)));

    if (rangeTreeNodesSize != 0) {
        rangeTreeNodes.reserve(parkingSize + rangeTreeNodesSize);
    }
}



void TParking::Init() {
    ConstructTree();
}



void TParking::ConstructTree() {
    if (parkingSize == 0) {
        return;
    }

    for (size_t i = 0; i < parkingSize; ++i) {
        rangeTreeNodes.push_back(TNode(i, /* IsLeaf */ true));
    }

    size_t rangeSize = pow(2, ceil(log(parkingSize) / log(2)));

    root = ConstructNode(
        /* by default NULL */ TNodePointer(),
        0,
        rangeSize);
}



TNodePointer TParking::ConstructNode(
    const TNodePointer& parent,
    size_t rangeStart,
    size_t rangeSize
) {
    if (rangeStart >= parkingSize) {
        return /* by default NULL */ TNodePointer();
    }

    if (rangeSize == 1) {
        rangeTreeNodes.at(rangeStart).SetParent(parent);
        return TNodePointer(rangeStart, &rangeTreeNodes);
    }

    assert(rangeSize % 2 == 0);

    rangeTreeNodes.push_back(TNode(rangeStart));

    TNodePointer result(rangeTreeNodes.size() - 1, &rangeTreeNodes);
    result->SetParent(parent);


    rangeSize /= 2;

    result->SetLeftSon(
        ConstructNode(result, rangeStart, rangeSize));

    result->SetRightSon(
        ConstructNode(result, rangeStart + rangeSize, rangeSize));

    return result;
}



TNodePointer TParking::GetNextFreePlace(
    const TNodePointer& node
) {
    if (!node->IsLeftSon()) {
        if (node->GetParent() == NULL) {
            assert(node->GetData() != NOT_FREE);
            return TNodePointer(node->GetData(), &rangeTreeNodes);
        }

        return GetNextFreePlace(node->GetParent());
    }

    TNodePointer rightBrother =
            node->GetBrother();
    if (rightBrother == NULL ||
        rightBrother->GetData() == NOT_FREE) {
        return GetNextFreePlace(node->GetParent());
    }

    return TNodePointer(rightBrother->GetData(), &rangeTreeNodes);
}



void TParking::Update(TNodePointer node) {
    while (node->GetParent() != NULL) {
        TNodePointer brother = node->GetBrother();
        TNodePointer parent = node->GetParent();

        if (brother == NULL ||
            brother->GetData() > node->GetData()) {
            parent->SetData(node->GetData());
        } else {
            parent->SetData(brother->GetData());
        }

        node = parent;
    }
}



TParkingAnswer TParking::TakePlace(const TPlaceNumber& placeNumber) {
    if (root->GetData() == NOT_FREE) {
        return TParkingAnswer(
                placeNumber,
                /* itWasTakeEvent */ true,
                NO_PLACE_TO_BE_TAKEN_ERROR);
    }

    TNodePointer placeLeaf(placeNumber, &rangeTreeNodes);

    if (placeLeaf->IsFree()) {
        placeLeaf->Take();
        Update(placeLeaf);
        return TParkingAnswer(placeNumber, /* itWasTakeEvent */ true);
    }

    TNodePointer nextFreeLeaf = GetNextFreePlace(placeLeaf);
    TParkingAnswer ans(nextFreeLeaf->Take(), /* itWasTakeEvent */ true);
    Update(nextFreeLeaf);

    return ans;
}


TParkingAnswer TParking::LeavePlace(const TPlaceNumber& placeNumber) {
    TNodePointer placeLeaf(placeNumber, &rangeTreeNodes);

    if (placeLeaf->IsFree()) {
        return TParkingAnswer(
                placeNumber,
                /* itWasTakeEvent */ false,
                PLACE_WAS_FREE_ERROR);
    }

    placeLeaf->Leave(placeNumber);
    Update(placeLeaf);

    return TParkingAnswer(LEAVE_PLACE_OK, /* itWasTakeEvent */ false);
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
            if (cit->ItWasTakeEvent()) {
                std::cout << cit->GetAnswer() + 1 << std::endl;
            } else {
                std::cout << cit->GetAnswer() << std::endl;
            }
        } else {
            std::cout << cit->GetErrorCode() << std::endl;
        }
    }

    return 0;
}
