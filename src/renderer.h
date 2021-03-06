#pragma once

#include <czmq.h>
#include <boost/core/noncopyable.hpp>
#include <SDL2/SDL.h>
#include "sdlex.h"

class renderer : boost::noncopyable {
   public:
    renderer();
    ~renderer();

    zactor_t* actor() { return _actor; }

    void start();

   private:
    zactor_t* _actor;

    SDL_Window* win;
    SDL_Renderer* ren;
    SDL_Surface* screen;
    SDL_Texture* texture;

    static void actor_callback(zsock_t* pipe, void* args);
    static int pipe_handler(zloop_t* loop, zsock_t* reader, void* arg);

    static int result_handler(zloop_t* loop, zsock_t* reader, void* arg);
    static int sdl_event_handler(zloop_t* loop, int timerId, void* arg);
};

