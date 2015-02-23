#pragma once

#include <czmq.h>
#include <boost/core/noncopyable.hpp>

namespace czmq {

template <class T>
class actor : boost::noncopyable {
   public:
    actor();
    ~actor();

   private:
    zactor_t* za;
    T* impl;
    static void actor_callback(zsock_t* pipe, void* args);
};

template <class T>
actor<T>::actor() {
    za = zactor_new(actor_callback, this);
}

template <class T>
actor<T>::~actor() {
    if (za) zactor_destroy(&za);
}

template <class T>
void actor<T>::actor_callback(zsock_t* pipe, void* args) {
    T impl(pipe);
}
}
