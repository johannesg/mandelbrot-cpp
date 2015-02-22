#pragma once

#include <czmq.h>
#include <boost/core/noncopyable.hpp>
#include "czmqpp/czmqpp.h"
#include "viewport.h"

class worker : boost::noncopyable {
   public:
    worker(int id);
    ~worker();

    const zactor_t* actor() { return _actor.get(); }

    void start();

   private:
    int id;
    Mandlebrot m;
    zactor_ptr _actor;
    zsock_t* _socket_result;

    static void actor_callback(zsock_t* pipe, void* args);

    static int pipe_handler(zloop_t* loop, zsock_t* reader, void* arg);
    static int socket_handler(zloop_t* loop, zsock_t* reader, void* arg);
};


