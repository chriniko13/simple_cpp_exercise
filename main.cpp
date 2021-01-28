#include <iostream>

#include <thread>

#include "behaviour_test.h"

#include "mem_investigation.h"

int main() {


    unsigned int no_threads = std::thread::hardware_concurrency();
    std::cout << "maximum threads system can support: " << no_threads << std::endl;


    // ==========================================================================
    // TEST FUNCTIONS TO TEST BEHAVIOUR (instead for example using Boost.test)
//    test1();
//    test2();
//    test3();
//    test4();
//    test5();
    // ==========================================================================


    // ==========================================================================
    // Investigate memory representation section
    do_mem_investigation();
    // ==========================================================================



    return 0;
}

