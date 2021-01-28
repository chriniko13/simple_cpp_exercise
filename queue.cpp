//
// Created by nchristidis on 19/1/21.
//

#include "queue.h"
#include <iostream>

template<class t>
void queue<t>::offer(t &elem) {

    std::lock_guard<std::mutex> lock(mutex);

    q.push(elem);

    queue_condition.notify_one();
}

template<class t>
void queue<t>::offer(t &&elem) {
    std::lock_guard<std::mutex> lock(mutex);

    q.push(elem);

    queue_condition.notify_one();
}


template<class t>
t &queue<t>::poll() {

    // Note: for using condition variable we need unique_lock because we want to lock and unlock.
    //       lock_guard is locked only once on construction and unlocked on destruction.
    std::unique_lock<std::mutex> lock(mutex);

    while (q.empty()) {
        queue_condition.wait(lock);
    }

    t &e = q.front();
    q.pop();

    return e;
}

template<class t>
t queue<t>::see() {

    // Note: for using condition variable we need unique_lock because we want to lock and unlock.
    //       lock_guard is locked only once on construction and unlocked on destruction.
    std::unique_lock<std::mutex> lock(mutex);

    while (q.empty()) {
        queue_condition.wait(lock);
    }

    t &e = q.front();
    return e;
}

template<class t>
int queue<t>::size() {
    std::lock_guard<std::mutex> lock(mutex);

    int s = q.size();
    return s;
}

template<class t>
void queue<t>::do_action() {

    std::cout << "do_action parent class ---> queue " << std::endl;


}

template<class t>
void queue<t>::identity() {

}

