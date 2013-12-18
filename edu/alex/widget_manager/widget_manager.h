/**
 * Alexandrescu 1.8
 */
#pragma once

////////////////////////////////////
// Policies realization
////////////////////////////////////

template <typename T>
class PrototypeCreator {
public:
    PrototypeCreator(T* const proto = 0)
        : prototype_(proto)
    { }

    T* create() {
        return prototype_ == 0 ? 0 : prototype_->clone();
    }

    void setPrototype(T* const proto) { prototype_ = proto; }
    T* getPrototype() const { return prototype_; }

protected:
    ~PrototypeCreator() { }

protected:
    T * prototype_;
};

template <typename T>
class OpNewCreator {
public:
    OpNewCreator() { }

    T* create() { return new T(); }

protected:
    ~OpNewCreator() { }
};


//////////////////////////////////////
// Data realization
//////////////////////////////////////

class Widget {
public:
    Widget()
        : id_(0)
    { }

    int& id() { return id_; }
    const int& id() const { return id_; }

protected:
    int id_;
};

/**
 * Prototype interface prowided
 */
class ClonableWidget {
public:
    ClonableWidget()
        : id_(0)
    { }

    int& id() { return id_; }
    const int& id() const { return id_; }

    ClonableWidget* clone() const {
        return new ClonableWidget(*this);
    }

protected:
    int id_;
};


//////////////////////////////////////
// Host classes realization
//////////////////////////////////////

template <template <class> class CreatorPolicy>
class WidgetManager : public CreatorPolicy<Widget> {
public:
    void switchPrototype(Widget* const proto) {
        //// Would not compile:
        // delete getPrototype();
        // setPrototype(proto);
        CreatorPolicy<Widget>& policy = *this;
        delete policy.getPrototype();
        policy.setPrototype(proto);
    }
};

typedef WidgetManager<OpNewCreator> NewWidgetManager;

/**
 * Use widget with prototype interface instead of primitive Widget.
 */
template <template <class> class CreatorPolicy>
class ClonableWidgetManager : public CreatorPolicy<ClonableWidget> {
public:
    void switchPrototype(ClonableWidget* const proto) {
        //// Would not compile:
        // delete getPrototype();
        // setPrototype(proto);
        CreatorPolicy<ClonableWidget>& policy = *this;
        delete policy.getPrototype();
        policy.setPrototype(proto);
    }
};

typedef ClonableWidgetManager<OpNewCreator> NewClonWidgetManager;
typedef ClonableWidgetManager<PrototypeCreator> ProtoClonWidgetManager;

