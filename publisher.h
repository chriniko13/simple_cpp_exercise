//
// Created by nchristidis on 19/1/21.
//

#ifndef AK3_M135_CHRISTIDIS_PUBLISHER_H
#define AK3_M135_CHRISTIDIS_PUBLISHER_H

#include "queue.h"
#include <vector>
#include <atomic>

/*
 * The purpose of this class, is to provide an abstraction of notification in the form of subscription.
 *
 * We can add subscribers to this publisher in the form of function pointers which accept record with type same as queue,
 * with the method: add_subscriber
 *
 * When the queue gets some entries (gets populated) we can call method: do_action, and notify all registered subscribers.
 *
 */
template<class t>
class publisher : /*virtual*/ public queue<t> {

private:

    int id;

    /*
     * Keep all the function pointers to methods which accept record with type same as queue (subscribers)
     */
    std::vector<void (*)(t *)> subscribers;


    // metrics
    std::atomic<int> successful_operations;
    std::atomic<int> failed_operations;

public:
    explicit publisher(int _id) : id(_id), subscribers(), successful_operations(0), failed_operations(0) {
    }

    void do_action() override;

    void identity() override;

    void add_subscriber(
            void (*handler)(t *) /* Note: a function pointer to a method which handles entries from the queue */
    );

    int & get_id();

    int get_successful_operations();

    int get_failed_operations();
};

#endif //AK3_M135_CHRISTIDIS_PUBLISHER_H
