#ifndef SOUND_H
#define SOUND_H
#include <vector>
#include <SDL.h>
#include <SDL_mixer.h>

class SDLSound{
public:
  SDLSound();
  ~SDLSound();

  SDLSound(const SDLSound&) = delete;
  SDLSound& operator=(const SDLSound&) = delete;

  void startMusic();
  void stopMusic();
  void toggleMusic();
  void operator[](int);

private:
  int volume;
  int currentSound;
  Mix_Music *music;

  int audioRate;
  int audioChannels;
  int audioBuffers;
  std::vector<Mix_Chunk*> sounds;
  std::vector<int> channels;
  // SDLSound(const SDLSound&);
  // SDLSound& operator=(const SDL_Surface&);


};
#endif
