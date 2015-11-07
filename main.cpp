/*
 * Copyright (c) 2015 Jussi Pakkanen.
 *
 * This file is part of mcdemo.
 *
 * Mcdemo is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License.
 *
 * Mcdemo is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with Mcdemo; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include<SDL.h>
#include<cstdio>
#include<cstdlib>
#include<memory>
#include<cassert>
#include<math.h>
#include"blue.h"

const int SCREEN_WIDTH = 1920/2;
const int SCREEN_HEIGHT = 1080/2;
const uint32_t FTIME = (1000/60);

// For simplicity.
const int texw = 100;
const int texh = 100;

SDL_Texture* unpack(SDL_Renderer *rend, const unsigned char* data, size_t data_size) {
    auto *io = SDL_RWFromConstMem(data, data_size);
    std::unique_ptr<SDL_Surface, void(*)(SDL_Surface*)> s(SDL_LoadBMP_RW(io, 1), SDL_FreeSurface);
    assert(s.get());
    return SDL_CreateTextureFromSurface(rend, s.get());
}

struct resources {
    std::unique_ptr<SDL_Texture, void(*)(SDL_Texture *)> blue_tex;

    resources(SDL_Renderer *rend) : blue_tex(unpack(rend, blue, sizeof(blue)), SDL_DestroyTexture) {
        assert(blue_tex.get());
    }
};

void render(SDL_Renderer *rend, const resources &res, const double ratio) {
    SDL_Rect r;
    assert(!SDL_SetRenderDrawColor(rend, 0, 0, 0, 0));
    SDL_RenderClear(rend);
    assert(!SDL_SetRenderDrawColor(rend, 255, 255, 255, 0));
    int centerx = SCREEN_WIDTH/2;
    int centery = SCREEN_HEIGHT/2;
    r.x = centerx - texw/2 + SCREEN_WIDTH*0.4*sin(2*M_PI*ratio);
    r.y = centery - texh/2 + SCREEN_HEIGHT*0.4*cos(2*2*M_PI*(ratio) + M_PI/2);
    r.w = texw;
    r.h = texh;
    assert(!SDL_RenderCopy(rend, res.blue_tex.get(), nullptr, &r));
    SDL_RenderPresent(rend);
}

void mainloop(SDL_Window *win, SDL_Renderer *rend) {
    SDL_Event e;
    resources res(rend);
    auto start_time = SDL_GetTicks();
    const int cycle = 2000;
    auto last_frame = SDL_GetTicks();
    SDL_RendererInfo f;
    SDL_GetRendererInfo(rend, &f);
    bool has_vsync = f.flags & SDL_RENDERER_PRESENTVSYNC;
    while(true) {
        while(SDL_PollEvent(&e)) {
            if(e.type == SDL_QUIT) {
                return;
            } else if(e.type == SDL_KEYDOWN) {
                switch(e.key.keysym.sym) {
                case  SDLK_ESCAPE:
                case  SDLK_q:
                    return;
                }
            } else if(e.type == SDL_JOYBUTTONDOWN) {
                return;
            }
        }
        render(rend, res, ((SDL_GetTicks() - start_time) % cycle)/double(cycle));
        if(!has_vsync) {
            auto time_spent = SDL_GetTicks() - last_frame;
            if(time_spent < FTIME) {
                SDL_Delay(FTIME - time_spent);
            }
        }
        last_frame = SDL_GetTicks(); // Not accurate, but good enough.
    }
}

int main(int /*argc*/, char **/*argv*/) {
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_GAMECONTROLLER |
            SDL_INIT_JOYSTICK | SDL_INIT_AUDIO) != 0) {
        fprintf(stderr, "Could not initialize SDL: %s\n", SDL_GetError());
        return 1;
    }
    SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_DEBUG);
    atexit(SDL_Quit);
    std::unique_ptr<SDL_Window, void (*)(SDL_Window*)>
        win(SDL_CreateWindow("Mcdemo", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0),
                SDL_DestroyWindow);
    std::unique_ptr<SDL_Renderer, void (*)(SDL_Renderer*)> rend(SDL_CreateRenderer(win.get(), -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC),
            SDL_DestroyRenderer);
    mainloop(win.get(), rend.get());
    return 0;
}
