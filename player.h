#ifndef PLAYER_H
#define PLAYER_H
#include <iostream>
#include <SDL.h>
#include <string>
#include <list>
#include <vector>
#include <cmath>

#include "twoWayMultiSprite.h"
#include "bullet.h"
#include "collisionStrategy.h"

class SmartSprite;

class Player{
public:
  Player(const std::string&);
  Player(const Player&);

  void draw() const;
  void update(Uint32 ticks);

  //Observer pattern functions
  void attach(SmartSprite* o) {observers.push_back(o);}
  void detach(SmartSprite* o);

  const TwoWayMultiSprite* getPlayer() const {return &player;}

  const std::string& getName() const {return player.getName();}
  int getX() const {return player.getX();}
  int getY() const {return player.getY();}
  const Image* getImage() const{
    return player.getImage();
  }
  int getScaledWidth() const{
    return player.getScaledWidth();
  }
  int getScaledHeight() const{
    return player.getScaledHeight();
  }
  const SDL_Surface* getSurface() const{
    return player.getSurface();
  }

  // need to get sprite from bullet
  // const Drawable* getBullet() const {
  //   return bullets[currentBullet]
  // }

  bool checkCollision(const Drawable&);

  void explode() {player.explode();}

  void right();
  void left();
  void up();
  void down();
  void stop();

  void shoot();

  Player& operator=(const Player&) = delete;

private:
  TwoWayMultiSprite player;
  Vector2f initialVelocity;
  int worldWidth;
  int worldHeight;

  //Observer pattern variables
  std::list<SmartSprite*> observers;

  std::string bulletName;
  std::list<Bullet> bullets;
  float minSpeed;
  float bulletInterval;
  float timeSinceLastFrame;
  int currentBullet;

  Vector2f oldVelocity;

  CollisionStrategy* strategy;

  //Player& operator=(const Player&);
};
#endif
