
#include <czmq.h>
#include <iostream>

#include "czmqpp/czmqpp.h"

#include "globals.h"
#include "worker.h"
#include "controller.h"
#include "renderer.h"

#include <SDL2/SDL.h>
#include "sdlex.h"
#include "czmqpp/actor.h"

using namespace std;
using namespace sdl;

int main(int argc, const char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    czmq::actor<renderer2> r2; 

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
    
    SDL_Quit();

    return 0;
}
