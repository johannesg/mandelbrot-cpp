#pragma once

#include <czmq.h>
#include <boost/core/noncopyable.hpp>
#include <SDL2/SDL.h>
#include "sdlex.h"

class renderer2 : boost::noncopyable {
    public:
    renderer2(zsock_t * pipe): pipe(pipe) {}
    private:
    zsock_t *pipe;

    sdl::Window_ptr win;
    sdl::Renderer_ptr ren;
    sdl::Surface_ptr screen;
    sdl::Texture_ptr texture;

    void callback();
    int pipe_handler(zloop_t* loop, zsock_t* socket);
};
