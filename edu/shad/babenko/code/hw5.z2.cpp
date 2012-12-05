#include <iostream>
#include <vector>

#include <cmath>
#include <cassert>



////////////////////////////// Nodes description ///////////////////////////


template <typename T>
class TCommonNode {
    TCommonNode* ancestor;
    T data;


    public:
    TCommonNode(const T& data);

    void SetAncestor(TCommonNode<T>* const );
    TCommonNode* GetAncestor() const;

    T GetData() const;
    void SetData(const T& newData);

    virtual bool IsLeaf() const = 0;

    virtual TCommonNode<T>* GetLeftSon() const;
    virtual TCommonNode<T>* GetRightSon() const;

    virtual void SetLeftSon(TCommonNode<T>* const);
    virtual void SetRightSon(TCommonNode<T>* const);

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
T TCommonNode<T>::GetData() const {
    return data;
}


template <typename T>
void TCommonNode<T>::SetData(const T& newData) {
    data = newData;
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

template <typename T>
class TParkingPlace: public TLeafNode<T> {
    bool isFree;

    public:
    TParkingPlace(const T& Data);

    bool IsFree() const;

    void Leave();
    void Take();
};


template <typename T>
TParkingPlace<T>::TParkingPlace(const T& Data)
    : TLeafNode<T>(Data)
    , isFree(true) {
}

template <typename T>
bool TParkingPlace<T>::IsFree() const {
    return isFree;
}


template <typename T>
void TParkingPlace<T>::Leave() {
    assert(!IsFree());

    isFree = true;
}


template <typename T>
void TParkingPlace<T>::Take() {
    assert(IsFree());

    isFree = false;
}






typedef size_t TPlaceNumber;










////////////////////////////////// TParking ////////////////////////////////


class TParking {
    std::vector<TParkingPlace<TPlaceNumber> > places;
    TCommonNode<TPlaceNumber>* root;

    static const size_t NOT_FREE;

    void ConstructTree();
    TCommonNode<TPlaceNumber>* ConstructNode(
        TCommonNode<TPlaceNumber>* const ancestor,
        size_t rangeStart,
        size_t rangeSize
    );
    void DeleteNode(TCommonNode<TPlaceNumber>* node);

    TParking(const TParking& );
    TParking& operator= (const TParking& );
    public:

    TParking(size_t NumberOfPlaces);

    ~TParking();
};


const size_t TParking::NOT_FREE = static_cast<size_t>(-1);


TParking::TParking(size_t NumberOfPlaces)
    : places(NumberOfPlaces, TParkingPlace<TPlaceNumber>(0))
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
}


void TParking::DeleteNode(TCommonNode<TPlaceNumber>* node) {
    if (node == 0) {
        return;
    }

    if (!node->IsLeaf()) {
        DeleteNode(node->GetLeftSon());
        DeleteNode(node->GetRightSon());

        delete node;
    }
}


TParking::~TParking() {
    DeleteNode(root);
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
    TParkingPlace<int> place(10);

    assert(place.IsFree() == true);

    place.Take();
    assert(place.IsFree() == false);

    place.Leave();
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









int main() {
    test_Nodes();
    test_TParking();

    return 0;
}
