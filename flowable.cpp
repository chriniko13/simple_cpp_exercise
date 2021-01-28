//
// Created by nchristidis on 19/1/21.
//

#include "flowable.h"

#include <iostream>


template<class t, class r>
void flowable<t, r>::do_action() {


    // Note: fetch from source.
    t* result = source();
    if (result == nullptr) {
        std:: cout << "source finished!" << std::endl;
        return;
    }

    // Note: emit result via publisher usage.
    publisher<t>::offer(std::ref(*result)); // note#1
    publisher<t>::do_action();


    // Note: do transformation.
    r* mapped_result = map(result);


    subscriber<r>::offer(std::ref(*mapped_result));  // Note: comment/uncomment when virtual inheritance {see note#1}, so one queue instance.
    subscriber<r>::do_action();


    // Note: emit result.
    sink(mapped_result);


}

template<class t, class r>
void flowable<t, r>::identity() {


}
