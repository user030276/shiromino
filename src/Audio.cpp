/**
 * Copyright (c) 2020 Brandon McGriff
 *
 * Licensed under the MIT license; see the LICENSE-src file at the top level
 * directory for the full text of the license.
 */
#include "Audio.hpp"
#include "core.h"
#include "SDL_mixer.h"
#include "Config.hpp"

using namespace Shiro;
using namespace std;

Music::Music() : volume(0), data(nullptr) {
}

Music::~Music() {
    if(data) {
        Mix_FreeMusic(data);
    }
}

bool Music::load(string filenameNoExt) {
    data = nullptr;
    volume = MIX_MAX_VOLUME;

    string path = filenameNoExt + ".ogg";
    data = Mix_LoadMUS(path.c_str());
    if(data) {
        return true;
    }

    path = filenameNoExt + ".wav";
    data = Mix_LoadMUS(path.c_str());

    return data != nullptr;
}

bool Music::play(coreState* cs) {
    if (!data) {
        return false;
    }

    Mix_VolumeMusic(static_cast<int>(volume * (cs->mus_volume / 100.0f) * (cs->master_volume / 100.0f)));
    Mix_PlayMusic(data, -1);

    return true;
}

Sfx::Sfx() : volume(0), data(nullptr) {}

Sfx::~Sfx() {
    if (data) {
        Mix_FreeChunk(data);
    }
}

bool Sfx::load(string filenameNoExt) {
    data = nullptr;
    volume = MIX_MAX_VOLUME;

    string path = filenameNoExt + ".wav";
    data = Mix_LoadWAV(path.c_str());
    filename = path;

    return data != nullptr;
}

bool Sfx::play(coreState* cs) {
    if(!data) {
        return false;
    }

    Mix_VolumeChunk(data, static_cast<int>(volume * (cs->sfx_volume / 100.0f) * (cs->master_volume / 100.0f)));
    if(Mix_PlayChannel(-1, data, 0) < 0) {
        printf("Mix_PlayChannel() error: %s\n", Mix_GetError());
    }

    return true;
}
