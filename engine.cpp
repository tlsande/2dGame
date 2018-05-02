#include <iostream>
#include <algorithm>
#include <sstream>
#include <string>
#include <random>
#include <iomanip>
//#include "sprite.h"
#include "multisprite.h"
#include "gamedata.h"
#include "engine.h"
#include "frameGenerator.h"
//#include "twoWayMultiSprite.h"
#include "player.h"
#include "collisionStrategy.h"
//#include "smartSprite.h"

const SDL_Color green = {0, 255, 0, 255};
const SDL_Color grey = {200, 200, 200, 255};

Engine::~Engine() {
  for(std::vector<Drawable*>::iterator it = sprites.begin(); it != sprites.end(); ++it){
     delete (*it);
  }
  for(Drawable* sprite : smartSprites) {
    delete sprite;
  }
  for(CollisionStrategy* strategy : strategies) {
    delete strategy;
  }
  sprites.clear();
  std::cout << "Terminating program" << std::endl;
}

Engine::Engine() :
  rc( RenderContext::getInstance() ),
  io( IoMod::getInstance() ),
  clock( Clock::getInstance() ),
  renderer( rc->getRenderer() ),
  sky("sky", Gamedata::getInstance().getXmlInt("sky/factor") ),
  ground("ground", Gamedata::getInstance().getXmlInt("ground/factor") ),
  viewport( Viewport::getInstance() ),
  player(new Player("Rathian")),
  sprites(std::vector<Drawable*>(0)),
  smartSprites(),
  strategies(),
  currentStrategy(0),
  collision(false),
  currentSprite(0),
  hud(),
  makeVideo( false ),
  sound(),
  timeLeft(10000)
{
  sound.startMusic();


  int n = Gamedata::getInstance().getXmlInt("Target/number");




  smartSprites.reserve(n);
  Vector2f pos = player->getPlayer()->getPosition();
  int w = player->getPlayer()->getScaledWidth();
  int h = player->getScaledHeight();
  for(int i = 0; i < n; ++i) {
    smartSprites.push_back(new SmartSprite("Target", pos, w, h));
    player->attach(smartSprites[i]);
  }

  strategies.push_back(new RectangularCollisionStrategy);
  strategies.push_back(new PerPixelCollisionStrategy);
  strategies.push_back(new MidPointCollisionStrategy);

  Viewport::getInstance().setObjectToTrack(player->getPlayer());

}

void Engine::draw() const {
  if(timeLeft < 0) {
    io.writeText("Press R to Restart the game", 560, 330, green);
    clock.pause();
  }

  sky.draw();
  ground.draw();



  for(const Drawable* sprite : smartSprites) {
    sprite->draw();
  }

  strategies[currentStrategy]->draw();
  if(collision) {
    IoMod::getInstance().writeText("There was a collision", 500, 90, green);
  }
  player->draw();

  for(auto* s : sprites) {
     s->draw();
 }

 int remaining = smartSprites.size();
 std::stringstream sRemain;
 sRemain << "Targets remaining " << remaining;
 io.writeText(sRemain.str(), 100, 3, green);

  int fps = clock.getFps();
  std::stringstream frames;
  frames << "FPS " << fps;
  io.writeText(frames.str(), 3, 3, green);

  std::stringstream timeCounter;
  timeCounter << "Time left " << timeLeft/1000;
  io.writeText(timeCounter.str(), 3, 690, green);

  hud.draw();
  viewport.draw();
  SDL_RenderPresent(renderer);
}

void Engine::update(Uint32 ticks) {

  timeLeft -= ticks;

  player->update(ticks);


  auto it = smartSprites.begin();
  while(it != smartSprites.end()) {
    (*it)->update(ticks);
    if(player->checkCollision(**it)) {
      (*it)->explode();
    }
    if((*it)->isExplosionDone()) {
      it = smartSprites.erase(it);
    }
    else ++it;
  }

  // for(auto* s : sprites) {
  //    s->update(ticks);
  // }

  sky.update();
  ground.update();
  viewport.update(); // always update viewport last
}

bool Engine::play() {
  SDL_Event event;
  const Uint8* keystate;
  bool done = false;
  Uint32 ticks = clock.getElapsedTicks();
  FrameGenerator frameGen;


  while ( !done ) {
  // if(timeLeft < 0) {
  //     done = true;
  //   }
    // The next loop polls for events, guarding against key bounce:
    while ( SDL_PollEvent(&event) ) {
      keystate = SDL_GetKeyboardState(NULL);
      if (event.type ==  SDL_QUIT) { done = true; break; }
      if(event.type == SDL_KEYDOWN) {
        if (keystate[SDL_SCANCODE_ESCAPE] || keystate[SDL_SCANCODE_Q]) {
          done = true;
          break;
        }
        if ( keystate[SDL_SCANCODE_P] ) {
          if ( clock.isPaused() ) clock.unpause();
          else clock.pause();
        }
        if(keystate[SDL_SCANCODE_SPACE]) {
          player->shoot();
        }
        if(keystate[SDL_SCANCODE_R]) {
          clock.unpause();
          return true;
        }
        // if(keystate[SDL_SCANCODE_E]) {
        //   smartSprites[0]->explode();
        //   std::cout << "Should have exploded" << std::endl;
        //   std::cout << "current sprite: " << currentSprite << std::endl;
        // }
        // if(keystate[SDL_SCANCODE_G]) {
        //   sprites[0]->explode();
        // }
        if(keystate[SDL_SCANCODE_M]) {
          currentStrategy = (1 + currentStrategy) % strategies.size();
        }
        if(keystate[SDL_SCANCODE_F1]) {
          hud.toggle();
        }
        if (keystate[SDL_SCANCODE_F4] && !makeVideo) {
          std::cout << "Initiating frame capture" << std::endl;
          makeVideo = true;
        }
        else if (keystate[SDL_SCANCODE_F4] && makeVideo) {
          std::cout << "Terminating frame capture" << std::endl;
          makeVideo = false;
        }
      }
    }

    // In this section of the event loop we allow key bounce:

    ticks = clock.getElapsedTicks();
    if ( ticks > 0 ) {
      clock.incrFrame();
      if(keystate[SDL_SCANCODE_A]) {
        static_cast<Player*>(player)->left();
      }
      if(keystate[SDL_SCANCODE_D]) {
        static_cast<Player*>(player)->right();
      }
      if(keystate[SDL_SCANCODE_W]) {
        static_cast<Player*>(player)->up();
      }
      if(keystate[SDL_SCANCODE_S]) {
        static_cast<Player*>(player)->down();
      }
      draw();
      update(ticks);
      if ( makeVideo ) {
        frameGen.makeFrame();
      }
    }
  }
  return false;

}
