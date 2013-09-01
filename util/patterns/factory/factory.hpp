/* @brief Realization of factory pattern.
 *
 * :author: Aman Orazaev
 * :email: aorazaev@gmail.com
 *
 *
 * Usage Example
 * =============
 *
 * .. code:: c++
 *
 *      // MyBaseClass.hpp
 *      class MyBaseClass { };
 *
 *      Factory<int, MyBaseClass>& getMyFactory() {
 *          static Factory<int, MyBaseClass> factory;
 *          return factory;
 *      }
 *
 *      // MyDerivedClassOne.hpp
 *      class MyDerivedClassOne : public MyBaseClass { };
 *      namespace {
 *          RegisterToFactory<MyDerivedClassOne> registerOne(getMyFactory(), 1);
 *      }
 *
 *      // MyDerivedClassTwo.hpp
 *      class MyDerivedClassTwo : public MyBaseClass { };
 *      namespace {
 *          RegisterToFactory<MyDerivedClassTwo> registerOne(getMyFactory(), 2);
 *      }
 *
 *      // Somewhere in souces.cpp
 *      // ...
 *      getMyFactory().create(1);
 *      getMyFactory().create(2);
 *
 */
#pragma once

#include <tr1/memory>
#include <map>


/* @brief Factory class. Create derived from Base class by given id.
 *
 * .. TODO::
 *
 *      * error policies.
 */
template <typename Id, typename Base>
class Factory {
private:
    // FIXME: use noncopyable inheritance.
    Factory(const Factory&);
    const Factory& operator=(const Factory&);

public:
    typedef Id IdType;
    typedef std::tr1::shared_ptr<Base> BasePtr;

    Factory() { }

    BasePtr create(const IdType& id) const {
        typename FactoryMap::const_iterator cit = map_.find(id);
        return cit != map_.end() ? cit->second() : BasePtr();
    }

    void add(const IdType& id, BasePtr (*func)()) {
        typename FactoryMap::iterator it = map_.find(id);
        if (it == map_.end()) {
            map_.insert(typename FactoryMap::value_type(id, func));
        } else {
            it->second = func;
        }
    }

protected:
    typedef std::map<Id, BasePtr(*)()> FactoryMap;

private:
    FactoryMap map_;
};


template <typename T>
class RegisterToFactory {
public:
    typedef std::tr1::shared_ptr<T> TPtr;

    template <typename TFactory>
    RegisterToFactory(TFactory & factory, const typename TFactory::IdType& id) {
        if (class_registered_++ == 0) {
            factory.add(id, createImpl<typename TFactory::BasePtr>);
        }
    }

private:
    template <typename TBasePtr>
    static TBasePtr createImpl() { return TPtr(new T()); }

    static int class_registered_;
};

template <typename T>
int RegisterToFactory<T>::class_registered_ = 0;
