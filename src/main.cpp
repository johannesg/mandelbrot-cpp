
#include <czmq.h>
#include <iostream>

#include "czmqpp/czmqpp.h"

#include "globals.h"
#include "worker.h"
#include "controller.h"
#include "renderer.h"

#include <SDL2/SDL.h>
#include "sdlex.h"

using namespace std;
using namespace sdl;

void run_tests();
int run_in_window();

int main(int argc, const char* argv[]) {
    if (argc > 1 && streq("test", argv[1])) {
        run_tests();
        return 0;
    }

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    return run_in_window();

    SDL_Quit();
}

void run_tests() { cout << "Testing" << endl; }

int run_in_window() {
    controller c;
    renderer r;
    worker w(1);
    worker w2(2);
    worker w3(3);
    worker w4(4);
    //	zactor_ptr controller_actor(zactor_new( old_controller, nullptr));
    //
    zstr_send(c.actor(), "START");

    zpoller_ptr poller(zpoller_new((void*)w.actor(), (void*)w2.actor(),
                                   (void*)w3.actor(), (void*)w4.actor(),
                                   c.actor(), r.actor(), nullptr));

    zpoller_wait(poller.get(), -1);

    return 0;
}
