#pragma once

#include <SDL2/SDL.h>
#include <memory>

namespace sdl {

#define __SDL_UNIQUE_PTR(name, freefn)                             \
    class __sdl_##name##_deleter {                                 \
       public:                                                     \
        void operator()(SDL_##name * p) { SDL_##freefn##name(p); } \
    };                                                             \
    typedef std::unique_ptr<SDL_##name, __sdl_##name##_deleter> name##_ptr;

#define SDL_UNIQUE_PTR(name) __SDL_UNIQUE_PTR(name, Destroy)
#define SDL_UNIQUE_PTR2(name, freefn) __SDL_UNIQUE_PTR(name, freefn)

SDL_UNIQUE_PTR(Window)
SDL_UNIQUE_PTR(Renderer)
SDL_UNIQUE_PTR(Texture)
SDL_UNIQUE_PTR2(Surface, Free)

Texture_ptr loadTexture(const std::string &file, SDL_Renderer *ren);

void logSDLError(const char *msg);
}
