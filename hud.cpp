#include <sstream>
#include <string>
#include "hud.h"
#include "renderContext.h"
#include "ioMod.h"
#include "gamedata.h"

const SDL_Color green = {0, 255, 0, 255};

Hud::Hud() :
  rend(RenderContext::getInstance()->getRenderer()),
  isOn(true),
  box(initRect(Gamedata::getInstance().getXmlInt("Hud/x"),
               Gamedata::getInstance().getXmlInt("Hud/y"),
               Gamedata::getInstance().getXmlInt("Hud/w"),
               Gamedata::getInstance().getXmlInt("Hud/h"))) {
  }

void Hud::draw() const {
  if(isOn) {
    SDL_SetRenderDrawColor(rend, 208, 209, 210, 255);

    // box.x = 150;
    // box.y = 150;
    // box.w = 150;
    // box.h = 150;

    SDL_SetRenderDrawBlendMode(rend, SDL_BLENDMODE_BLEND);

    SDL_SetRenderDrawColor(rend, 255, 255, 255, 255/2);

    SDL_RenderFillRect(rend, &box);

    SDL_SetRenderDrawColor(rend, 255, 0, 0, 255/2);
    SDL_RenderDrawRect(rend, &box);

    SDL_RenderPresent(rend);

    std::string message;
    std::stringstream nMes;
    for(int i = 0; i < Gamedata::getInstance().getXmlInt("Hud/nMes"); i++) {

      nMes << "Hud/mes" << i;
      message = Gamedata::getInstance().getXmlStr(nMes.str());
      IoMod::getInstance().writeText(message, 1130,
                                    6 + (i * Gamedata::getInstance().getXmlInt("font/size") + 3),
                                    green);
      nMes.str("");
    }

    // std::stringstream test;
    // test << "test 1 \n test2 \n";
    //
    // IoMod::getInstance().writeText(test.str(), 1130, 6, green);

  }
  else{
    //SDL_RenderClear(rend);
  }
}

SDL_Rect Hud::initRect(int xi, int yi, int wi, int hi) {
  SDL_Rect rect = {xi, yi, wi, hi};
  return rect;
}

void Hud::toggle() {
  if(isOn) {
    isOn = false;
    SDL_RenderClear(rend);
  }
  else{
    isOn = true;
  }
}
