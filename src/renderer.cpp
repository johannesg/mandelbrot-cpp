#include "renderer.h"

#include "globals.h"
#include "czmqpp/czmqpp.h"
#include <iostream>
#include "viewport.h"
#include "sdlex.h"

using namespace std;
using namespace sdl;

renderer::renderer() : _actor(zactor_new(actor_callback, this)) {}

renderer::~renderer() { zactor_destroy(&_actor); }

int renderer::pipe_handler(zloop_t* loop, zsock_t* reader, void* arg) {
    cout << "renderer pipe cmd" << endl;
    auto msg = zstr_recv(reader);
    int ret = 0;
    if (msg == nullptr || streq(msg, "$TERM")) ret = -1;

    zstr_free(&msg);

    return ret;
}

int renderer::result_handler(zloop_t* loop, zsock_t* reader, void* arg) {
    auto self = reinterpret_cast<renderer*>(arg);
    int x;
    int y;
    int n;
    zsock_recv(reader, "iii", &x, &y, &n);

    auto buffer = reinterpret_cast<uint8_t*>(self->screen->pixels);
    auto offset = self->screen->pitch * y + x * self->screen->format->BytesPerPixel;

    auto color = 255 - n;

    buffer[offset] = color;
    buffer[offset + 1] = color;
    buffer[offset + 2] = color;
    buffer[offset + 3] = 255;

    // cout << "render: x = " << x << ", y = " << y << endl;
//    SDL_SetRenderDrawColor(g_ren, c, 0, 0, 255);
//    SDL_RenderDrawPoint(g_ren, x, y);

    return 0;
}

int renderer::sdl_event_handler(zloop_t* loop, int timerId, void* arg) {
    auto self = reinterpret_cast<renderer*>(arg);
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        switch (e.type) {
            case SDL_QUIT:
            case SDL_KEYDOWN:
            case SDL_MOUSEBUTTONDOWN:
                return -1;
        }
    }
    SDL_UpdateTexture(self->texture, nullptr, self->screen->pixels, self->screen->pitch);
    SDL_RenderClear(self->ren);
    SDL_RenderCopy(self->ren, self->texture, nullptr, nullptr);
    SDL_RenderPresent(self->ren);

//    SDL_RenderPresent(g_ren);
    return 0;
}

void renderer::start() { _actor = zactor_new(actor_callback, this); }

void renderer::actor_callback(zsock_t* pipe, void* args) {
    auto self = reinterpret_cast<renderer*>(args);
    auto win = sdl::Window_ptr(SDL_CreateWindow("Hello asshole!", 100, 100,
                                                SCREEN_WIDTH, SCREEN_HEIGHT,
                                                SDL_WINDOW_SHOWN));
    if (win == nullptr) {
        logSDLError("SDL_CreateWindow");
        return;
    }

    self->win = win.get();

    auto ren = sdl::Renderer_ptr(SDL_CreateRenderer(
        self->win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC));
    if (ren == nullptr) {
        logSDLError("SDL_CreateRenderer");
        return;
    }

    self->ren = ren.get();

    // if all this hex scares you, check out SDL_PixelFormatEnumToMasks()!
    auto screen = sdl::Surface_ptr(
        SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, 0x00FF0000,
                             0x0000FF00, 0x000000FF, 0xFF000000));
    auto texture = sdl::Texture_ptr(SDL_CreateTexture(
        self->ren, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING,
        SCREEN_WIDTH, SCREEN_HEIGHT));

    self->screen = screen.get();
    self->texture = texture.get();
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

    zloop_reader(loop, pipe, pipe_handler, self);
    zloop_reader(loop, socket, result_handler, self);
    //	zloop_reader_set_tolerant(loop, pipe);
    zloop_timer(loop, 17, 0, sdl_event_handler, self);

    // zloop_set_verbose(loop, true);

    zsock_signal(pipe, 0);
    zloop_start(loop);

    zsock_destroy(&socket);
    return;
}
