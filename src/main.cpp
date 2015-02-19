
#include <czmq.h>
#include <iostream>

#include "czmqpp/czmqpp.h"

#include "globals.h"
#include "worker.h"
#include "controller.h"

using namespace std;

void run_tests();

int main(int argc, const char * argv[]) {

    if (argc > 1 && streq("test", argv[1])) {
        run_tests();
        return 0;
    }
        
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

void run_tests() {

        cout << "Testing" << endl;
}
