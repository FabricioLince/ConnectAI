#ifndef PRIORITY_QUEUE
#define PRIORITY_QUEUE

#include <map>
#include <queue>

template < typename T , class Compare = std::less<int> >
class PriorityQueue
{
    std::map<int, std::queue<T>, Compare > container;

    public:
        void put(T t, int priority)
        {
            container[priority].push(t);
        }
        T pop()
        {
            std::queue<T>& front = container.begin()->second;
            T t = front.front();
            front.pop();
            if (front.empty()) container.erase(container.begin());
            return t;
        }
        bool empty() const
        {
            return container.empty();
        }
};
#endif
