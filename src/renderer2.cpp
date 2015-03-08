#include "renderer2.h"
#include "globals.h"
#include "czmqpp/czmqpp.h"
#include <iostream>
#include "viewport.h"
#include "sdlex.h"
#include <functional>

using namespace std;
using namespace std::placeholders;
using namespace sdl;
typedef std::function<int(zloop_t*, zsock_t*)> zlfn;

int zloop_reader_callback(zloop_t* loop, zsock_t* reader, void* arg) {
    auto func_p = reinterpret_cast<zlfn*>(arg);

    return (*func_p)(loop, reader);
}

void zloop_reader2(zloop_t* loop, zsock_t* socket, zlfn func) {
    zloop_reader(loop, socket, zloop_reader_callback, &func);
}

int renderer2::pipe_handler(zloop_t* loop, zsock_t* socket) {
    cout << "renderer pipe cmd" << endl;
    auto msg = zstr_recv(socket);
    int ret = 0;
    if (msg == nullptr || streq(msg, "$TERM")) ret = -1;

    zstr_free(&msg);

    return ret;
}

void renderer2::callback() {
    win = sdl::Window_ptr(SDL_CreateWindow("Hello asshole!", 100, 100,
                                           SCREEN_WIDTH, SCREEN_HEIGHT,
                                           SDL_WINDOW_SHOWN));
    if (win == nullptr) {
        logSDLError("SDL_CreateWindow");
        return;
    }

    ren = sdl::Renderer_ptr(SDL_CreateRenderer(
        win.get(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC));
    if (ren == nullptr) {
        logSDLError("SDL_CreateRenderer");
        return;
    }

    // if all this hex scares you, check out SDL_PixelFormatEnumToMasks()!
    screen = sdl::Surface_ptr(
        SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, 0x00FF0000,
                             0x0000FF00, 0x000000FF, 0xFF000000));
    texture = sdl::Texture_ptr(SDL_CreateTexture(
        ren.get(), SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING,
        SCREEN_WIDTH, SCREEN_HEIGHT));

    // Select the color for drawing. It is set to red here.
    SDL_SetRenderDrawColor(ren.get(), 255, 255, 255, 255);

    // Clear the entire screen to our selected color.
    SDL_RenderClear(ren.get());
    // Up until now everything was drawn behind the scenes.
    // This will show the new, red contents of the window.
    SDL_RenderPresent(ren.get());

    cout << "Starting renderer" << endl;

    zsock_t* socket = zsock_new_pull(nullptr);

    int ret = zsock_bind(socket, endpoint_result);
    cout << "bind: " << ret << endl;
    auto loop = zloop_new();
    zlfn fn = std::bind(&renderer2::pipe_handler, this, _1, _2);
    zloop_reader2(loop, pipe, fn);
    zloop_reader(loop, socket, result_handler, self);
    //	zloop_reader_set_tolerant(loop, pipe);
    zloop_timer(loop, 17, 0, sdl_event_handler, self);

    // zloop_set_verbose(loop, true);

    zsock_signal(pipe, 0);
    zloop_start(loop);

    zsock_destroy(&socket);
}
