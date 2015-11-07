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
#include<mutex>
#include"blue.h"
#include"red.h"
#include"green.h"
#include"startup.h"

const int SCREEN_WIDTH = 1920/2;
const int SCREEN_HEIGHT = 1080/2;
const uint32_t FTIME = (1000/60);
SDL_AudioSpec want, have;

// For simplicity.
const int texw = 100;
const int texh = 100;

SDL_Texture* unpack_image(SDL_Renderer *rend, const unsigned char* data, size_t data_size) {
    auto *io = SDL_RWFromConstMem(data, data_size);
    std::unique_ptr<SDL_Surface, void(*)(SDL_Surface*)> s(SDL_LoadBMP_RW(io, 1), SDL_FreeSurface);
    assert(s.get());
    return SDL_CreateTextureFromSurface(rend, s.get());
}

void unpack_wav(const unsigned char *data, size_t data_size, Uint8 **audio_buf, Uint32 *audio_len) {
    auto *io = SDL_RWFromConstMem(data, data_size);
    auto res = SDL_LoadWAV_RW(io, 1, &want, audio_buf, audio_len);
    assert(res);

}

struct audiocontrol {
    std::mutex m;
    Uint8 *sample;
    int sample_size;
    int played_bytes;

    audiocontrol() : sample(nullptr), sample_size(0), played_bytes(0) {
    }

    void play_sample(Uint8 *new_sample, Uint32 new_size) {
        std::lock_guard<std::mutex> l(m);
        sample = new_sample;
        sample_size = new_size;
        played_bytes = 0;
    }

    void produce(Uint8 *stream, int len) {
        std::lock_guard<std::mutex> l(m);
        int written_bytes = 0;
        if(played_bytes < sample_size) {
            written_bytes = std::min(len, sample_size - played_bytes);
            SDL_memmove(stream, sample, written_bytes);
            played_bytes += written_bytes;
        }
        if(written_bytes < len) {
            SDL_memset(stream + written_bytes, 0, len - written_bytes);
        }
    }
};

struct resources {
    std::unique_ptr<SDL_Texture, void(*)(SDL_Texture *)> blue_tex;
    std::unique_ptr<SDL_Texture, void(*)(SDL_Texture *)> red_tex;
    std::unique_ptr<SDL_Texture, void(*)(SDL_Texture *)> green_tex;

    std::unique_ptr<Uint8, void(*)(Uint8*)> startup_sound;
    Uint32 startup_size;

    resources(SDL_Renderer *rend) : blue_tex(unpack_image(rend, blue, sizeof(blue)), SDL_DestroyTexture),
            red_tex(unpack_image(rend, red, sizeof(red)), SDL_DestroyTexture),
            green_tex(unpack_image(rend, green, sizeof(green)), SDL_DestroyTexture),
            startup_sound(nullptr, SDL_FreeWAV) {
        assert(blue_tex.get());
        assert(red_tex.get());
        assert(green_tex.get());
        Uint8 *tmp = nullptr;
        unpack_wav(startup, sizeof(startup), &tmp, &startup_size);
        startup_sound.reset(tmp);
        tmp = nullptr;
    }

};


void audiocallback(void *data, Uint8* stream, int len) {
    auto control = reinterpret_cast<audiocontrol*>(data);
    control->produce(stream, len);
}

void draw_single(SDL_Renderer *rend, SDL_Texture *tex, const double ratio) {
    SDL_Rect r;
    int centerx = SCREEN_WIDTH/2;
    int centery = SCREEN_HEIGHT/2;
    r.x = centerx - texw/2 + SCREEN_WIDTH*0.4*sin(2*M_PI*ratio);
    r.y = centery - texh/2 + SCREEN_HEIGHT*0.4*cos(2*2*M_PI*(ratio) + M_PI/2);
    r.w = texw;
    r.h = texh;
    assert(!SDL_RenderCopy(rend, tex, nullptr, &r));
}

void render(SDL_Renderer *rend, const resources &res, const double ratio) {
    assert(!SDL_SetRenderDrawColor(rend, 0, 0, 0, 0));
    SDL_RenderClear(rend);
    assert(!SDL_SetRenderDrawColor(rend, 255, 255, 255, 0));
    draw_single(rend, res.blue_tex.get(), ratio);
    draw_single(rend, res.red_tex.get(), ratio + 0.3333);
    draw_single(rend, res.green_tex.get(), ratio + 0.6666);
    SDL_RenderPresent(rend);
}

void mainloop(SDL_Window *win, SDL_Renderer *rend, SDL_AudioDeviceID adev, audiocontrol &control) {
    SDL_Event e;
    resources res(rend);
    auto start_time = SDL_GetTicks();
    const int cycle = 2000;
    auto last_frame = SDL_GetTicks();
    SDL_RendererInfo f;
    SDL_GetRendererInfo(rend, &f);
    bool has_vsync = f.flags & SDL_RENDERER_PRESENTVSYNC;
    control.play_sample(res.startup_sound.get(), res.startup_size);
    SDL_PauseAudioDevice(adev, 0);
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
        win(SDL_CreateWindow("Monocoque demo", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0),
                SDL_DestroyWindow);
    assert(win.get());
    std::unique_ptr<SDL_Renderer, void (*)(SDL_Renderer*)> rend(SDL_CreateRenderer(win.get(), -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC),
            SDL_DestroyRenderer);
    assert(rend.get());
    SDL_AudioDeviceID dev; // Not a pointer, so using with std::unique_ptr is not easy.
    audiocontrol control;
    // All our wav files are in this format so hardcode it and have SDL do all conversions.
    want.freq = 44100;
    want.format = AUDIO_S16LSB;
    want.channels = 1;
    want.samples = 4096;
    want.callback = audiocallback;
    want.userdata = &control;
    dev = SDL_OpenAudioDevice(nullptr, 0, &want, &have, 0);
    assert(dev);

    mainloop(win.get(), rend.get(), dev, control);
    SDL_CloseAudioDevice(dev);
    return 0;
}
