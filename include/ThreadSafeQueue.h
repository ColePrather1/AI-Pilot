#include <mbed.h>
#include <queue>

template<typename T>
class ThreadSafeQueue {
private:
    std::queue<T> queue;
    Mutex mutex;
    ConditionVariable cond;

public:
    void push(T item) {
        mutex.lock();
        queue.push(item);
        mutex.unlock();
        cond.notify_one();
    }

    T pop() {
        mutex.lock();
        while (queue.empty()) {
            cond.wait(&mutex);
        }
        T item = queue.front();
        queue.pop();
        mutex.unlock();
        return item;
    }

    bool try_pop(T& item) {
        mutex.lock();
        if (queue.empty()) {
            mutex.unlock();
            return false;
        }
        item = queue.front();
        queue.pop();
        mutex.unlock();
        return true;
    }

    bool empty() {
        mutex.lock();
        bool is_empty = queue.empty();
        mutex.unlock();
        return is_empty;
    }
};
