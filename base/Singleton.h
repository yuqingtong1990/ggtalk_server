//
// Created by yqt on 18-11-8.
//

#ifndef ALL_SINGLETON_H
#define ALL_SINGLETON_H

template < class type >
class Singleton
{
public:
    Singleton(){}
    ~Singleton(){}

    inline static type & getSingleton()
    {
        if (!_singleton)
        {
            _singleton = new type();
        }
        return *_singleton;
    }
    inline static type * getSingletonPtr()
    {
        if (!_singleton)
        {
            _singleton = new type();
        }
        return _singleton;
    }protected:
    static type* _singleton;
};

template < class type >
type* Singleton<type>::_singleton = nullptr;

template < class type, class param >
class SingletonFirst
{
public:
    SingletonFirst(){}
    ~SingletonFirst(){}

    inline static type & getSingleton()
    {
        if (!_singleton)
        {
            _singleton = new type(new param());
        }
        return *_singleton;
    }
    inline static type * getSingletonPtr()
    {
        if (!_singleton)
        {
            _singleton = new type(new param());
        }
        return _singleton;
    }
protected:
    static type* _singleton;
};

template < class type, class param>
type* SingletonFirst<type, param>::_singleton = nullptr;

#endif //ALL_SINGLETON_H
