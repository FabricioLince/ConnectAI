#ifndef MY_QUEUE_H
#define MY_QUEUE_H

#include <queue>

template < typename T >
class Queue : public std::queue<T>
{
    public:
        void put(T t)
        {
            std::queue<T>::push(t);
        }
        T pop()
        {
            T rt = std::queue<T>::front();
            std::queue<T>::pop();
            return rt;
        }
};

#endif
