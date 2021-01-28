//
// Created by nchristidis on 19/1/21.
//

#include "subscriber.h"

#include <iostream>

template<class t>
void subscriber<t>::do_action() {

    try {
        queue<t>::do_action();
        std::cout << "do_action from subscriber \n" << std::endl;


        if (this->size() > 0) {
            t rec = this->see();
            t *rec_pointer = &rec;
            records_handler(rec_pointer);
        } else {
            std::cout << "subscriber#do_action: empty queue" << std::endl;
        }

    } catch (const std::exception &e) {

        failed_operations.fetch_add(1, std::memory_order_seq_cst);

        std::cerr << "subscriber#do_action error occurred:" << e.what() << std::endl;
    }

}

template<class t>
int &subscriber<t>::get_id() {
    return id;
}

template<class t>
int subscriber<t>::get_successful_operations() {
    return successful_operations;
}

template<class t>
int subscriber<t>::get_failed_operations() {
    return failed_operations;
}

template<class t>
void subscriber<t>::identity() {


}

