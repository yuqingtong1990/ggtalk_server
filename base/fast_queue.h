//
// Created by yqt on 18-11-8.
//

#ifndef ALL_FAST_QUEUE_H
#define ALL_FAST_QUEUE_H

#include <deque>
#include <mutex>

template<typename T, unsigned Size = 0>
class FastQueue
{
public:
    typedef std::deque<T>   Cont;

    FastQueue() = default;
    ~FastQueue() = default;

    /*
    * push
    */
    inline void push(const T& elem)
    {
        std::unique_lock<std::mutex> ulk(mutex_);
        if (_flag == 0)
            _containerB.push_back(elem);
        else
            _containerA.push_back(elem);
    }

    /*
    * pop
    */
    bool pop(T& elem)
    {
        if (_flag == 0)
        {
            if (!_containerA.empty())
            {
                elem = _containerA.front();
                _containerA.pop_front();
                return true;
            }
            else
            {
                std::unique_lock<std::mutex> ulk(mutex_);
                if (!_containerB.empty())
                {
                    _flag = 1;
                    elem = _containerB.front();
                    _containerB.pop_front();
                    return true;
                }
            }
        }
        else
        {
            if (!_containerB.empty())
            {
                elem = _containerB.front();
                _containerB.pop_front();
                return true;
            }
            else
            {
                std::unique_lock<std::mutex> ulk(mutex_);
                if (!_containerA.empty())
                {
                    _flag = 0;
                    elem = _containerA.front();
                    _containerA.pop_front();
                    return true;
                }
            }
        }

        return false;
    }

    /*
    * total
    */
    inline unsigned getSize()
    {
        std::unique_lock<std::mutex> ulk(mutex_);
        unsigned int size = 0;
        size = _containerA.size() + _containerB.size();
        return size;
    }

    /*
    * 移除未使用的空间
    */
    inline void reserved()
    {
         std::unique_lock<std::mutex> ulk(mutex_);
        _containerA.shrink_to_fit();
        _containerB.shrink_to_fit();
    }

    /*
    * 消费者调用
    */
    inline unsigned getSizeR()
    {
        if (_flag == 0)
            return (unsigned)_containerA.size();
        else
            return (unsigned)_containerB.size();
    }

    /*
    * 生产者调用
    */
    inline unsigned getSizeW()
    {
        std::unique_lock<std::mutex> ulk(mutex_);
        unsigned int size = 0;
        if (_flag == 0)
            size = _containerB.size();
        else
            size = _containerA.size();
        return size;
    }

    /*
    * 消费者调用
    */
    inline int getFlag(){ return _flag; }

private:
    Cont _containerA;
    Cont _containerB;
    int _flag = 0;         //读线程切换队列状态
    std::mutex mutex_;
};


#endif //ALL_FAST_QUEUE_H
