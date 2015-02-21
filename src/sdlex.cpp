#include "sdlex.h"
#include <iostream>

namespace sdl {

Texture_ptr loadTexture(const std::string &file, SDL_Renderer *ren) {
    auto bmp = sdl::Surface_ptr(SDL_LoadBMP(file.c_str()));
    if (bmp == nullptr) {
        return nullptr;
    }

    return sdl::Texture_ptr(SDL_CreateTextureFromSurface(ren, bmp.get()));
}

void logSDLError(const char* msg) {
    std::cout << msg << " Error : " << SDL_GetError() << std::endl;
}
}
