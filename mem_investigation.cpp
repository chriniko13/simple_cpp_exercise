//
// Created by nchristidis on 21/1/21.
//

#include "mem_investigation.h"


#include "queue.cpp"

#include "publisher.cpp"
#include "subscriber.cpp"

#include "flowable.cpp"


#include <vector>
#include <unordered_map>
#include <utility>
#include <string>
#include <algorithm>
#include <list>


// ----

std::vector<int *> unprocessed_ints;
std::vector<int *>::iterator source_iterator_ints;

// source
void init_source_ints();

int *get_next_ints();


// map
int *process_ints(int *input);


// sink
std::unordered_map<int, int *> processed_ints_by_id;

void save_ints(int *input);


void print_processed_int(int *input);

void print_unprocessed_int(int *input);


// ----

bool sort_by_second(const std::pair<std::string, unsigned long> &a,
                    const std::pair<std::string, unsigned long> &b) {
    return (a.second < b.second);
}

// ----

void do_mem_investigation() {


    // Note: create source.
    init_source_ints();
    int *(*source)();
    source = get_next_ints;

    // Note: create map.
    int *(*map)(int *);
    map = process_ints;

    // Note: create sink.
    void (*sink)(int *);
    sink = save_ints;

    flowable<int, int> *f = new flowable<int, int>(
            source,
            map,
            sink,

            print_processed_int,
            1);



    f->add_subscriber(print_unprocessed_int);


    ((publisher<int>*)f)->offer(1);
    ((publisher<int>*)f)->offer(2);

    ((subscriber<int>*)f)->offer(1);
    ((subscriber<int>*)f)->offer(2);


    // Note: check hierarchy / size of pointer.
    /*
     *                 [   queue  ]
     *                   /       \
     *                  /         \
     *                 /           \
     *        [ publisher ]      [ subscriber ]
     *             \                /
     *              \              /
     *               \            /
     *                \          /
     *               [  flowable  ]
     */


    // Note: first calculate memory positions.

    // flowable
    unsigned long *seeker = (unsigned long *) f;
    std::cout << std::endl;

    std::cout << "size of int *: " << sizeof(seeker)
              << std::endl; // in 32 bits architectures -> 4 bytes and in 64 bits architectures -> 8 bytes.

    std::cout << std::endl;
    std::cout << "mem loc flowable: " << *seeker << std::endl;
    std::pair<std::string, unsigned long> flowable_mem_loc("flowable", *seeker);



    // publisher
    publisher<int> *p = f;
    seeker = (unsigned long *) p;
    std::cout << std::endl;
    unsigned long mem_pos = *seeker;
    std::cout << "mem loc publisher: " << mem_pos << std::endl;
    std::pair<std::string, unsigned long> publisher_mem_loc("publisher", mem_pos);


    // subscriber
    subscriber<int> *s = f;
    seeker = (unsigned long *) s;
    std::cout << std::endl;
    mem_pos = *seeker;
    std::cout << "mem loc subscriber: " << mem_pos << std::endl;
    std::pair<std::string, unsigned long> subscriber_mem_loc("subscriber", mem_pos);


    // publisher::queue
    queue<int> *publisher_queue = (publisher<int> *) f;
    seeker = (unsigned long *) publisher_queue;
    std::cout << std::endl;
    mem_pos = *seeker;
    std::cout << "mem loc publisher_queue: " << mem_pos << std::endl;
    std::pair<std::string, unsigned long> publisher_queue_mem_loc("publisher_queue", mem_pos);


    // subscriber::queue
    queue<int> *subscriber_queue = (subscriber<int> *) f;
    seeker = (unsigned long *) subscriber_queue;
    std::cout << std::endl;
    mem_pos = *seeker;
    std::cout << "mem loc subscriber_queue: " << mem_pos << std::endl;
    std::pair<std::string, unsigned long> subscriber_queue_mem_loc("subscriber_queue", mem_pos);



    // Note: save all 5 memory locations of our object hierarchy.
    std::cout << "\n\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
    std::vector<std::pair<std::string /* object name */, unsigned long /* mem location */>> pairs;
    pairs.push_back(flowable_mem_loc);
    pairs.push_back(publisher_mem_loc);
    pairs.push_back(subscriber_mem_loc);
    pairs.push_back(publisher_queue_mem_loc);
    pairs.push_back(subscriber_queue_mem_loc);


    // Note: sort by mem location
    std::sort(pairs.begin(), pairs.end(), sort_by_second);


    // Note: we group them by mem location in order to display them nicely later.
    std::unordered_map<unsigned long /* mem location */, std::list<std::string> /* object names */> names_by_mem_location;

    for (int i = 0; i < pairs.size(); i++) {

        std::pair<std::string, unsigned long> &current = pairs[i];

        std::string name = current.first;
        unsigned long mem_location = current.second;

        std::unordered_map<unsigned long, std::list<std::string>>::iterator map_it = names_by_mem_location.find(mem_location);

        if (map_it == names_by_mem_location.end()) {

            std::list<std::string> names;
            names.push_back(name);

            names_by_mem_location.insert({{mem_location, names}});

        } else {

            // Note: if already an entry in map, just store the name in the list.
            map_it->second.push_back(name);
        }

    }



    // Note: print them.
    std::cout << "The memory locations after sort and group operation are:\n";

    for (auto &kv : names_by_mem_location) {

        unsigned long loc = kv.first;
        std::list<std::string> obj_names = kv.second;

        std::cout << "[";
        for (std::list<std::string>::iterator it = obj_names.begin(); it != obj_names.end(); it++) {

            if (std::distance(it, obj_names.end()) == 1) {
                std::cout << *it;
            } else {
                std::cout << *it << ", ";
            }

        }
        std::cout << "   ---->   " << loc << "]" << std::endl;

    }

    delete f;
}


// ----
void init_source_ints() {
    // init vector
    for (int i = 0; i < 100; i++) {

        int *r = new int(i + 500);

        unprocessed_ints.push_back(r);
    }

    // init iterator
    source_iterator_ints = unprocessed_ints.begin();
}

int *get_next_ints() {
    if (source_iterator_ints == unprocessed_ints.end()) return nullptr;

    int *result = *source_iterator_ints;

    source_iterator_ints++;

    return result;
}


int *process_ints(int *input) {
    int *pr = new int(*input + 700);
    return pr;
}


void save_ints(int *input) {
    processed_ints_by_id[*input] = input;
}

void print_processed_int(int *input) {
    std::cout << "processed int: " << *input << std::endl;
}

void print_unprocessed_int(int *input) {
    std::cout << "unprocessed int: " << *input << std::endl;

}

