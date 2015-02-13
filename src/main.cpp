
#include <czmq.h>
#include <iostream>

#include "czmqpp/czmqpp.h"

#include "globals.h"
#include "worker.h"
#include "controller.h"

using namespace std;

int main() {
    controller c;
    //	zactor_ptr controller_actor(zactor_new( old_controller, nullptr));
    zactor_ptr worker_actor(zactor_new(worker, nullptr));

    zpoller_ptr poller(
        zpoller_new(worker_actor.get(), c.actor(), nullptr));
    zpoller_wait(poller.get(), -1);
}
