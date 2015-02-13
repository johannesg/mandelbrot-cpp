#pragma once

#include <czmq.h>
#include <boost/core/noncopyable.hpp>

//void old_controller(zsock_t* pipe, void* args);

class controller : boost::noncopyable {
   public:
    controller();
    ~controller();

    zactor_t* actor() { return _actor; }

    void start();

   private:
    zactor_t* _actor;

    static void actor_callback(zsock_t* pipe, void* args);

    static int pipe_handler(zloop_t* loop, zsock_t* reader, void* arg);
    static int timer_handler(zloop_t* loop, int timerId, void* arg); 
};
