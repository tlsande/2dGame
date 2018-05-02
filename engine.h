#include <vector>
#include <SDL.h>
#include "ioMod.h"
#include "renderContext.h"
#include "clock.h"
#include "world.h"
#include "viewport.h"
//#include "player.h"
#include "smartSprite.h"
#include "hud.h"
#include "sound.h"

class Player;
class CollisionStrategy;
//class SmartSprite;
//class SubjectSprite;

class Engine {
public:
  Engine(const Engine&) = delete;
  Engine& operator=(const Engine&) = delete;
  Engine ();
  ~Engine ();
  void play();

private:
  const RenderContext* rc;
  const IoMod& io;
  Clock& clock;

  SDL_Renderer * const renderer;
  World sky;
  World ground;
  Viewport& viewport;

  // Drawable* star;
  // Drawable* spinningStar;

  Player* player;

  std::vector<Drawable*> sprites;
  std::vector<SmartSprite*> smartSprites;
  std::vector<CollisionStrategy*> strategies;
  int currentStrategy;
  bool collision;

  int currentSprite;

  Hud hud;

  bool makeVideo;

  SDLSound sound;

  int timeLeft;

  void draw() const;
  void update(Uint32);


  void printScales() const;
  void checkForCollisions();
};
