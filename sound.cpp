#include <string>
#include <iostream>
#include <ctime>
#include "sound.h"

SDLSound::~SDLSound() {
  std::cout << "Cleaning up sounds..." << std::endl;
  std::clock_t start = std::clock();
  Mix_HaltMusic();
  Mix_FreeMusic(music);
  for(unsigned int i = 0; i < sounds.size(); i++) {
    Mix_FreeChunk(sounds[i]);
  }
  Mix_CloseAudio();
  std::clock_t duration = std::clock() - start;
  std::cout << "Clean up took " << duration << " ticks\n";
}

// SDLSound& SDLSound::operator=(const SDLSound& rhs) {
//   volume = rhs.volume;
//   currentSound = rhs.currentSound;
//   music = rhs.music;
//   audioRate = rhs.audioRate;
//   audioChannels = rhs.audioChannels;
//   audioBuffers = rhs.audioBuffers;
//   sounds = rhs.sounds;
//   channels = rhs.channels;
//
//   return *this;
// }

SDLSound::SDLSound() :
  volume(SDL_MIX_MAXVOLUME/10),
  currentSound(-1),
  music(NULL),
  audioRate(22050),
  audioChannels(2),
  audioBuffers(4096),
  sounds(),
  channels()
  {
    if(Mix_OpenAudio(audioRate, MIX_DEFAULT_FORMAT, audioChannels, audioBuffers)) {
      throw std::string("Unable to open audio!");
    }
    music = Mix_LoadMUS("sound/victory_theme.mp3");

    if(!music) throw std::string("Couldn't load song") + Mix_GetError();

    startMusic();

    //Add sounds
    //sounds.push_back(Mix_LoadWAV("sound/sound.wav"));

    for(unsigned int i = 0; i < sounds.size(); i++) {
      channels.push_back(i);
    }
    std::cout << "Music and Sound is loaded" << std::endl;
  }

// SDLSound::SDLSound(const SDLSound& s) :
//   volume(s.volume),
//   currentSound(s.currentSound),
//   music(s.music),
//   audioRate(s.audioRate),
//   audioChannels(s.audioChannels),
//   audioBuffers(s.audioBuffers),
//   sounds(s.sounds),
//   channels(s.channels)
//   { }



void SDLSound::operator[](int index) {
  if(currentSound >= 0) Mix_HaltChannel(currentSound);
  currentSound = index;
  Mix_VolumeChunk(sounds[index], volume);
  channels[index] = Mix_PlayChannel(-1, sounds[index], 0);
}

void SDLSound::startMusic() {
  Mix_VolumeMusic(volume);
  Mix_PlayMusic(music, -1);
}

void SDLSound::stopMusic() {
  Mix_HaltMusic();
  Mix_FreeMusic(music);
}
