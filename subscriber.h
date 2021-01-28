//
// Created by nchristidis on 19/1/21.
//

#ifndef AK3_M135_CHRISTIDIS_SUBSCRIBER_H
#define AK3_M135_CHRISTIDIS_SUBSCRIBER_H

#include "queue.h"
#include "publisher.h"

/*
 * The purpose of this class, is to provide an abstraction of notification in the form of subscription.
 *
 * We can bind a subscriber to a publisher via bind_publisher and when this publisher has notifications/pending records
 * we can call do_action from the subscriber in order to consume them via the attribute: records_handler.
 *
 *
 */

template<class t>
class subscriber : /*virtual*/ public queue<t> {

private:
    int id;

    void (*records_handler)(t *);


    // metrics
    std::atomic<int> successful_operations;
    std::atomic<int> failed_operations;


public:
    explicit subscriber(void (*handler)(t *), int _id)
            : records_handler(handler),
              id(_id) {}

    void do_action() override;

    void identity() override;

    int &get_id();

    int get_successful_operations();

    int get_failed_operations();

};


#endif //AK3_M135_CHRISTIDIS_SUBSCRIBER_H
