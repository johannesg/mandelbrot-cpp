#include "controller.h"

#include "globals.h"
#include "czmqpp/czmqpp.h"
#include <iostream>
#include "viewport.h"

using namespace std;

int count = 0;

controller::controller() : _actor(zactor_new(actor_callback, this)) {}

controller::~controller() { zactor_destroy(&_actor); }

int controller::pipe_handler(zloop_t* loop, zsock_t* reader, void* arg) {
    cout << "controller pipe cmd" << endl;
    auto msg = zstr_recv(reader);
    int ret = 0;
    if (msg == nullptr || streq(msg, "$TERM")) ret = -1;

    zstr_free(&msg);

    return ret;
}

int controller::result_handler(zloop_t* loop, zsock_t* reader, void* arg) {
    cout << "controller result: ";
    auto msg = zstr_recv(reader);
    int ret = 0;
    if (msg == nullptr)
        ret = -1;
    else {
        std::string result(msg);
        std::cout << result << endl;
    }

    zstr_free(&msg);

    return ret;
}

int controller::timer_handler(zloop_t* loop, int timerId, void* arg) {
    auto topLeft = Point(-2, 1);
    auto bottomRight = Point(1, -1);


    Viewport v(topLeft, bottomRight, 640, 480); 


    cout << "controller: sending" << endl;
    zstr_send(arg, "HEJ");

    if (++count > 30) return -1;

    return 0;
}

void controller::start() { _actor = zactor_new(actor_callback, this); }

void controller::actor_callback(zsock_t* pipe, void* args) {
    cout << "Starting controller" << endl;

    zsock_t* socket = zsock_new_push(nullptr);
    zsock_t* socket_result = zsock_new_pull(nullptr);

    auto ret = zsock_bind(socket, endpoint);
    cout << "bind: " << ret << endl;
    ret = zsock_bind(socket_result, endpoint_worker);
    cout << "bind: " << ret << endl;

    auto loop = zloop_new();

    zloop_reader(loop, pipe, pipe_handler, nullptr);
    zloop_reader(loop, socket_result, result_handler, nullptr);
    //	zloop_reader_set_tolerant(loop, pipe);
    zloop_timer(loop, 300, 0, timer_handler, socket);

    //zloop_set_verbose(loop, true);

    zsock_signal(pipe, 0);
    zloop_start(loop);

    zsock_destroy(&socket);
    zsock_destroy(&socket_result);
    return;
}
