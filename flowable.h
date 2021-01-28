//
// Created by nchristidis on 19/1/21.
//

#ifndef AK3_M135_CHRISTIDIS_FLOWABLE_H
#define AK3_M135_CHRISTIDIS_FLOWABLE_H

#include "publisher.h"
#include "subscriber.h"

/*
 * Flowable is a subscriber and publisher at the same time (not the best design --- for the exercise only)
 *
 *
 * It consumes from a provided source, it can do some processing on top of these consumed records, and then emits them
 * to the specified sink.
 *
 *
 * Flowable diagram:
 *
 *          [source]
 *
 *              ===> {publisher::offer, publisher::do_action}
 *
 *              ===> [map]
 *
 *              ===> {subscriber::offer, subscriber::do_action}
 *
 *              ===> [sink]
 *
 *
 *
 *
 *  IMPORTANT NOTE: in this design virtual inheritance makes a lot of sense.
 *                  Check publisher and subscriber class to toggle on-off virtual inheritance (via comment-uncomment),
 *                  so that to have only one queue instance in inheritance hierarchy.
 *
 */

template<class t /* input */, class r /* output */>
class flowable : public publisher<t>, public subscriber<r> {

private:
    long stream_id;
    double precision_loss;
    bool parallel;

    t *(*source)();

    r *(*map)(t *);

    void (*sink)(r *);

public:
    flowable(
            t *(*source_fun)(),
            r *(*map_fun)(t *),
            void (*sink_fun)(r *),

            void (*handler)(r *),
            int id) : publisher<t>(id + 10), subscriber<r>(handler, id + 20) {

        source = source_fun;
        map = map_fun;
        sink = sink_fun;

        stream_id = id + 1111;
        precision_loss = 0.0123;
        parallel = false;

    }

    void do_action() override;

    void identity() override;

};


#endif //AK3_M135_CHRISTIDIS_FLOWABLE_H
