//
// Created by nchristidis on 19/1/21.
//

#include "publisher.h"
#include <iostream>

template<class t>
void publisher<t>::do_action() {

    try {

        queue<t>::do_action();
        std::cout << "do_action from publisher \n" << std::endl;

        std::cout << "will notify subscribers if any value(s) exist " << std::endl;

        while (this->size() > 0) {
            t &elem = this->poll();

            // Note: notify all registered subscribers with the just polled value.
            for (auto &subscriber : subscribers) {

                // Note: do the casting.
                void (*f)(t *);
                f = subscriber;

                // Note: use pointer to target reference.
                t *elem_p = &elem;

                // Note: call registered subscriber (in the form of a method)
                f(elem_p);
            }

        }


        successful_operations.fetch_add(1, std::memory_order_seq_cst);

    } catch (const std::exception &e) {

        failed_operations.fetch_add(1, std::memory_order_seq_cst);

        std::cerr << "publisher#do_action error occurred:" << e.what() << std::endl;
    }

}

template<class t>
void publisher<t>::add_subscriber(void (*handler)(t *)) {

    this->subscribers.push_back(handler);
}

template<class t>
int &publisher<t>::get_id() {
    return id;
}

template<class t>
int publisher<t>::get_successful_operations() {
    return successful_operations;
}

template<class t>
int publisher<t>::get_failed_operations() {
    return failed_operations;
}

template<class t>
void publisher<t>::identity() {

}
