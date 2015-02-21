#include "worker.h"
#include "globals.h"
#include "czmqpp/czmqpp.h"
#include <iostream>
#include <sstream>

using namespace std;

worker::worker(int id) : id(id), _actor(zactor_new(actor_callback, this)) {}
worker::~worker() { /*zactor_destroy(&_actor);*/
}

int worker::pipe_handler(zloop_t* loop, zsock_t* reader, void* arg) {
    worker* self = reinterpret_cast<worker*>(arg);
    cout << "worker " << self->id << ": pipe cmd" << endl;
    auto msg = zstr_recv(reader);

    int ret = 0;
    if (msg == nullptr || streq(msg, "$TERM")) ret = -1;

    zstr_free(&msg);

    return ret;
}

int worker::socket_handler(zloop_t* loop, zsock_t* reader, void* arg) {
    worker* self = reinterpret_cast<worker*>(arg);
    int x;
    int y;
    zsock_recv(reader, "ii", &x, &y);
//    cout << "worker " << self->id << ": x = " << x << ", y = " << y << endl;

    zsock_send(self->_socket_result, "iii", x, y, 0);

    return 0;
}

void worker::actor_callback(zsock_t* pipe, void* args) {
    worker* self = reinterpret_cast<worker*>(args);
    cout << "Starting worker" << endl;

    zsock_ptr worker(zsock_new_pull(nullptr));
    zsock_ptr worker_result(zsock_new_push(nullptr));

    self->_socket_result = worker_result.get();

    auto ret = zsock_connect(worker.get(), endpoint_items);
    cout << "connect: " << ret << endl;
    ret = zsock_connect(worker_result.get(), endpoint_result);
    cout << "connect: " << ret << endl;

    auto loop = zloop_new();

    zloop_reader(loop, pipe, pipe_handler, self);
    //	zloop_reader_set_tolerant(loop, pipe);
    zloop_reader(loop, worker.get(), socket_handler, self);

    // zloop_set_verbose(loop, true);

    zsock_signal(pipe, 0);
    zloop_start(loop);
}
