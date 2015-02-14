
#include <czmq.h>
#include <iostream>

#include "czmqpp/czmqpp.h"

#include "globals.h"
#include "worker.h"
#include "controller.h"

using namespace std;

int main() {
    controller c;
    worker w(1);
    worker w2(2);
    worker w3(3);
    worker w4(4);
    //	zactor_ptr controller_actor(zactor_new( old_controller, nullptr));

    zpoller_ptr poller(zpoller_new(
                (void*)w.actor(),
                (void*)w2.actor(),
                (void*)w3.actor(),
                (void*)w4.actor(),
                c.actor(), nullptr));
    zpoller_wait(poller.get(), -1);
}
