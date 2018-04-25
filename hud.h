#include <iostream>
#include <SDL2/SDL.h>
//#include "ioMod.h"

class Hud{
public:
  Hud();
  void draw() const;
  void toggle();

private:
  SDL_Rect initRect(int, int, int, int);

  SDL_Renderer* const rend;
  bool isOn;
  SDL_Rect box;

  Hud(const Hud&);
  Hud& operator=(const Hud&);
};
