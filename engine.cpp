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
  // delete star;
  // delete spinningStar;
  //sprites.clear();
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
  // star(new Sprite("YellowStar")),
  // spinningStar(new MultiSprite("SpinningStar")),
  player(new Player("Rathian")),
  sprites(std::vector<Drawable*>(0)),
  smartSprites(),
  strategies(),
  nSprites(0),
  currentStrategy(0),
  collision(false),
  currentSprite(0),
  hud(),
  makeVideo( false )
{

  //Viewport::getInstance().setObjectToTrack(star);

  // For player
  nSprites++;

  // sprites.push_back(new TwoWayMultiSprite("Rathian"));
  // nSprites++;
  //
  for(int i = 0; i < Gamedata::getInstance().getXmlInt("Sword/number"); i++) {
    sprites.push_back(new Sprite("Sword"));
    nSprites++;
  }
  // //sprites.push_back(new MultiSprite("Rathian"));
  //
  for(int j = 0; j < Gamedata::getInstance().getXmlInt("Thonking/number"); j++) {
     sprites.push_back(new MultiSprite("Thonking"));
     nSprites++;
  }

  // sprites.push_back(new MultiSprite("Dinovaldo"));
  // nSprites++;

  int n = Gamedata::getInstance().getXmlInt("Sword/number");
  std::cout << n << std::endl;




  smartSprites.reserve(n);
  Vector2f pos = player->getPlayer()->getPosition();
  int w = player->getPlayer()->getScaledWidth();
  int h = player->getScaledHeight();
  for(int i = 0; i < n; ++i) {
    smartSprites.push_back(new SmartSprite("Sword", pos, w, h));
    player->attach(smartSprites[i]);
  }

  strategies.push_back(new RectangularCollisionStrategy);
  strategies.push_back(new PerPixelCollisionStrategy);
  strategies.push_back(new MidPointCollisionStrategy);

  Viewport::getInstance().setObjectToTrack(player->getPlayer());

  //switchSprite();
  std::cout << "Loading complete" << std::endl;
}

void Engine::draw() const {
  sky.draw();
  ground.draw();



  //IoMod::getInstance().writeText("Press m to change strategy", 500, 60, green);
  for(const Drawable* sprite : smartSprites) {
    sprite->draw();
  }

  // std::stringstream strm;
  // strm << smartSprites.size() << " Smart Sprites Remaining";
  // IoMod::getInstance().writeText(strm.str(), 30, 60, green);
  strategies[currentStrategy]->draw();
  if(collision) {
    IoMod::getInstance().writeText("There was a collision", 500, 90, green);
  }
  player->draw();

  for(auto* s : sprites) {
     s->draw();
 }

  int fps = clock.getFps();
  std::stringstream frames;
  frames << "FPS " << fps;
  //io.writeText(frames.str(), 30, 50);

  io.writeText(frames.str(), 3, 3, green);

  // std::string myName = "Lee Sanders";
  // io.writeText(myName, 3, 690, grey);

  // Some test stuff
  // std::stringstream debug;
  // debug << "Test " << Gamedata::getInstance().getRandInRange(100, 50);
  // io.writeText(debug.str(), 30, 80, test);

  hud.draw();
  viewport.draw();
  SDL_RenderPresent(renderer);
}

// update for shooting ******** remember to re-enable function in update
void Engine::checkForCollisions() {
  auto it = smartSprites.begin();
  while(it != smartSprites.end()) {
    if(strategies[currentStrategy]->execute(*player->getPlayer(), **it)) {
      SmartSprite* doa = *it;
      player->detach(doa);
      delete doa;
      it = smartSprites.erase(it);
    }
    else ++it;
  }
}

void Engine::update(Uint32 ticks) {
  // star->update(ticks);
  // spinningStar->update(ticks);

  //hud.update();

  //checkForCollisions();


  player->update(ticks);

  for(Drawable* sprite : smartSprites) {
    sprite->update(ticks);
  }

  for(auto* s : sprites) {
     s->update(ticks);
 }
  sky.update();
  ground.update();
  viewport.update(); // always update viewport last
}

void Engine::switchSprite(){
  ++currentSprite;
  // currentSprite = currentSprite % 2;
  // if ( currentSprite ) {
  //   Viewport::getInstance().setObjectToTrack(sprites.at(currentSprite));
  // }
  // else {
  //   Viewport::getInstance().setObjectToTrack(sprites.at(currentSprite));
  // }
  currentSprite = currentSprite % nSprites;

  if(currentSprite == 0) {
    Viewport::getInstance().setObjectToTrack(player->getPlayer());
  }
  else{
    Viewport::getInstance().setObjectToTrack(sprites.at(currentSprite - 1));
  }
}

void Engine::play() {
  SDL_Event event;
  const Uint8* keystate;
  bool done = false;
  Uint32 ticks = clock.getElapsedTicks();
  FrameGenerator frameGen;

  while ( !done ) {
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
        if ( keystate[SDL_SCANCODE_T] ) {
          switchSprite();
        }
        if(keystate[SDL_SCANCODE_SPACE]) {
          player->shoot();
        }
        if(keystate[SDL_SCANCODE_E]) {
          smartSprites[0]->explode();
          std::cout << "Should have exploded" << std::endl;
          std::cout << "current sprite: " << currentSprite << std::endl;
        }
        if(keystate[SDL_SCANCODE_P]) {
          sprites[0]->explode();
        }
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

}
