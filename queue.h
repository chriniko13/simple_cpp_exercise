//
// Created by nchristidis on 19/1/21.
//

#ifndef AK3_M135_CHRISTIDIS_QUEUE_H
#define AK3_M135_CHRISTIDIS_QUEUE_H

#include <queue>
#include <mutex>
#include <condition_variable>

/*
 * The purpose of this class is to provide a thread safe queue implementation, by using lock constructs.
 */
template<class t>
class queue {

private:
    std::queue<t> q;
    std::mutex mutex;
    std::condition_variable queue_condition;

protected:
    virtual void do_action();
    virtual void identity();

public:
    queue() : q(), mutex(), queue_condition() {};

    void offer(t &elem);

    void offer(t &&elem);

    t &poll();

    t see();

    int size();


};


#endif //AK3_M135_CHRISTIDIS_QUEUE_H
