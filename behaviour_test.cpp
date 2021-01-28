//
// Created by nchristidis on 20/1/21.
//

#include "behaviour_test.h"

#include <iostream>

#include <thread>

#include <unordered_map>

#include <random>
//#include <climits> // Note:    std::uniform_int_distribution<long> distribution(LONG_MIN,LONG_MAX);



#include "queue.cpp"

#include "publisher.cpp"
#include "subscriber.cpp"

#include "flowable.cpp"




// ######################################### test 1 ####################################################################

/*
 * The purpose of this test is to just test the behaviour of the queue (single threaded)
 */
template<typename t>
void addToQueue(queue<t *> *q, t *rec) {
    q->offer(rec);
}

void test1() {

    std::cout << "\n\nTEST 1" << std::endl;


    queue<int> *q = new queue<int>;

    int i = 1;
    int i2 = 2;

    std::cout << "(q1) size: " << q->size() << std::endl;
    q->offer(std::ref(i));
    std::cout << "(q1) size: " << q->size() << std::endl;
    q->offer(i2);
    std::cout << "(q1) size: " << q->size() << std::endl;

    while (q->size() != 0) {
        int &r = q->poll();
        std::cout << "r: " << r << std::endl;
    }

    delete q;


    std::cout << "\n\n";


    queue<int *> *q2 = new queue<int *>;
    int *i3 = new int(3);
    q2->offer(i3);

    int *i4 = new int(4);
    addToQueue(q2, i4);

    std::cout << "(q2)size: " << q2->size() << std::endl;
    std::cout << "(q2)poll: " << *(q2->poll()) << std::endl;
    std::cout << "(q2)poll: " << *(q2->poll()) << std::endl;

    delete i3;
    delete i4;

    delete q2;


    std::cout << "\n\n" << std::endl;

}




// #################################### test 2 #########################################################################


/*
 * The purpose of this test, is to test the behaviour of the queue under multiple threads (multi-threaded).
 */
struct sensor_temp {
    int sensorId;
    double temp;
};

long random_long(long &&min, long &&max) {
    std::default_random_engine generator;
    std::uniform_int_distribution<long> distribution(min, max);
    long result = distribution(generator);
    return result;
}


std::mutex counter_mutex;
int thread_operations = 0;

void addTemp(queue<sensor_temp *> *q, int batch) {

    using namespace std;
    std::thread::id this_id = std::this_thread::get_id();

    // # START
    counter_mutex.lock();

    thread_operations += 1;
    cout << "current thread id under work: " << this_id << endl;

    counter_mutex.unlock();
    // # END

    for (int i = 0; i < batch; i++) {

        //if (i % 100 == 0) cout << "thread with id: " << this_id << " working..." << endl;

        sensor_temp *st = new sensor_temp;
        st->sensorId = i + 1;
        st->temp = 2.0 + i;

        q->offer(st);


        long time_to_sleep = random_long(75, 200);
        std::this_thread::sleep_for(std::chrono::milliseconds(time_to_sleep));
    }

}

void test2() {
    std::cout << "\n\nTEST 2" << std::endl;

    int no_of_workers = 400;
    //int no_of_workers = 40;

    int records_to_save = 20000;

    int worker_batch = records_to_save / no_of_workers;
    if (worker_batch % 2 != 0) exit(-1);

    queue<sensor_temp *> *q = new queue<sensor_temp *>;

    std::vector<std::thread> threads;

    for (int i = 0; i < no_of_workers; i++) {

        threads.push_back(
                std::thread(addTemp, q, worker_batch)
        );
    }

    for (auto &th : threads) {
        th.join();
    }


    std::cout << "q size: " << q->size() << std::endl;

    std::cout << "total thread operations: " << thread_operations << std::endl;

    std::cout << "worker batch size: " << worker_batch << std::endl;

    std::cout << "\n\n" << std::endl;
}





// ######################################### test 3 ####################################################################

/*
 * The purpose of this test, is to test the behaviour of the publisher class.
 */

void print_just_an_int(int *input) {
    std::cout << "(version) just an int: " << *input << std::endl;
}

void print_just_an_int2(int *input) {
    std::cout << "(version) just an int2: " << *input << std::endl;
}

void test3() {

    std::cout << "\n\nTEST 3" << std::endl;

    void (*my_func)(int *);


    publisher<int> *p = new publisher<int>(1);

    std::cout << "success operations: " << p->get_successful_operations() << std::endl;
    std::cout << "failure operations: " << p->get_failed_operations() << std::endl;

    // Note: add subscribers first.
    my_func = print_just_an_int;
    p->add_subscriber(my_func);

    my_func = print_just_an_int2;
    p->add_subscriber(my_func);


    // Note: populate queue.
    p->offer(3);
    p->offer(4);
    p->offer(5);
    p->offer(6);


    // Note: do notification.
    p->do_action();

    std::cout << "success operations: " << p->get_successful_operations() << std::endl;

    delete p;

    std::cout << "\n\n" << std::endl;

}


// ######################################### test 4 ####################################################################


/*
 * The purpose of this test, is to test the behaviour of the subscriber class.
 */

struct foo_bar {
    int x;

    explicit foo_bar(int x) {
        this->x = x;
    }
};

void print_foo_bar(foo_bar *f) {
    std::cout << "[subscribe notify via seek] f.x == " << f->x << std::endl;
}

void test4() {

    std::cout << "\n\nTEST 4" << std::endl;

    void (*my_func)(foo_bar *);
    my_func = print_foo_bar;


    subscriber<foo_bar> *s = new subscriber<foo_bar>(my_func, 1);


    // Note: populate queue.
    s->offer(foo_bar(1));
    s->offer(foo_bar(2));
    s->offer(foo_bar(3));


    // Note: do notification.
    s->do_action();
    s->do_action();
    s->do_action();


    delete s;

    std::cout << "\n\n" << std::endl;

}



// ######################################### test 5 ####################################################################

struct unprocessed_record {
    int x;
};

struct processed_record {
    int x;
};

std::vector<unprocessed_record *> unprocessed_records;
std::vector<unprocessed_record *>::iterator source_iterator;

void init_source() {
    // init vector
    for (int i = 0; i < 100; i++) {

        unprocessed_record *r = new unprocessed_record;
        r->x = i + 500;

        unprocessed_records.push_back(r);
    }

    // init iterator
    source_iterator = unprocessed_records.begin();
}

// ---source---
unprocessed_record *get_next() {
    if (source_iterator == unprocessed_records.end()) return nullptr;

    unprocessed_record *result = *source_iterator;

    source_iterator++;

    return result;
}


// ---map---
processed_record *process(unprocessed_record *ur) {
    processed_record *pr = new processed_record;
    pr->x = ur->x;
    return pr;
}


// ---sink---
std::unordered_map<int, processed_record *> processed_records_by_id;

void save_processed_record(processed_record *pr) {
    processed_records_by_id[pr->x] = pr;
}

void print_processed_record(processed_record *pr) {
    std::cout << "processed record: " << pr->x << std::endl;
}


void print_unprocessed_record(unprocessed_record *upr) {
    std::cout << "[add_subscriber => notification] unprocessed record: " << upr->x << std::endl;
}

void test5() {

    std::cout << "\n\nTEST 5" << std::endl;

    // Note: create source.
    init_source();
    unprocessed_record *(*source)();
    source = get_next;


    // Note: create map.
    processed_record *(*map)(unprocessed_record *);
    map = process;

    // Note: create sink.
    void (*sink)(processed_record *);
    sink = save_processed_record;

    flowable<unprocessed_record, processed_record> *f = new flowable<unprocessed_record, processed_record>(
            source,
            map,
            sink,

            print_processed_record,
            1);

    f->add_subscriber(print_unprocessed_record);


    std::cout << "processed_records_by_id.size(): " << processed_records_by_id.size() << std::endl;
    f->do_action();
    f->do_action();
    std::cout << "processed_records_by_id.size(): " << processed_records_by_id.size() << std::endl;

    int i = 0;
    while (i++ < 3 /*106*/ ) {
        f->do_action();
    }
    std::cout << "processed_records_by_id.size(): " << processed_records_by_id.size() << std::endl;



    //===============================================================================================
    // Note: time to do the cleaning of memory - housekeeping stuff.
    std::vector<unprocessed_record *>::iterator it = unprocessed_records.begin();
    while (it != unprocessed_records.begin()) {
        delete *it;
    }

    std::unordered_map<int, processed_record *>::iterator it2 = processed_records_by_id.begin();
    while (it2 != processed_records_by_id.end()) {

        processed_record *to_delete = (*it2).second;

        delete to_delete;
        it2++;
    }


    delete f;

    std::cout << "\n\n" << std::endl;

}