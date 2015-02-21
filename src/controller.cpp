#include "controller.h"

#include "globals.h"
#include "czmqpp/czmqpp.h"
#include <iostream>
#include "viewport.h"
#include "sdlex.h"

using namespace std;
using namespace sdl;

int count = 0;

controller::controller() : _actor(zactor_new(actor_callback, this)) {}

controller::~controller() { zactor_destroy(&_actor); }

int controller::pipe_handler(zloop_t* loop, zsock_t* reader, void* arg) {
    controller* self = reinterpret_cast<controller*>(arg);
    cout << "controller pipe cmd" << endl;
    auto msg = zstr_recv(reader);
    int ret = 0;
    if (msg == nullptr || streq(msg, "$TERM")) ret = -1;

    if (streq(msg,"START"))
        self->send_messages();

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

void controller::send_messages() {
    for (int y = 0; y < SCREEN_HEIGHT; y++)
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            zsock_send(socket, "ii", x, y);
        }
}

void controller::actor_callback(zsock_t* pipe, void* args) {
    controller* self = reinterpret_cast<controller*>(args);
    cout << "Starting controller" << endl;

    self->socket = zsock_new_push(nullptr);

    auto ret = zsock_bind(self->socket, endpoint_items);
    cout << "bind: " << ret << endl;

    auto loop = zloop_new();

    zloop_reader(loop, pipe, pipe_handler, self);
    //	zloop_reader_set_tolerant(loop, pipe);
//    zloop_timer(loop, 300, 0, timer_handler, socket);

    // zloop_set_verbose(loop, true);
    //

    zsock_signal(pipe, 0);
    zloop_start(loop);

    zsock_destroy(&self->socket);
    return;
}
