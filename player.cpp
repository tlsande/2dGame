#include "player.h"
#include "gamedata.h"
#include "smartSprite.h"
#include "renderContext.h"

Player::Player(const std::string& name) :
  player(name),
  initialVelocity(player.getVelocity()),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  observers(),
  bulletName(Gamedata::getInstance().getXmlStr(name+"/bullet")),
  bullets(),
  minSpeed(Gamedata::getInstance().getXmlInt(bulletName+"/speedX")),
  bulletInterval(Gamedata::getInstance().getXmlInt(bulletName+"/interval")),
  timeSinceLastFrame(0),
  currentBullet(0)
  {}

Player::Player(const Player& p) :
  player(p.player),
  initialVelocity(p.player.getVelocity()),
  worldWidth(p.worldWidth),
  worldHeight(p.worldHeight),
  observers(p.observers),
  bulletName(p.bulletName),
  bullets(p.bullets),
  minSpeed(p.minSpeed),
  bulletInterval(p.bulletInterval),
  timeSinceLastFrame(p.timeSinceLastFrame),
  currentBullet(p.currentBullet)
{}

// Player::Player(const Player& p) :
//   player(),
//   initialVelocity(player.getVelocity()),
//   worldWidth(Gamedata::getInstace().getXmlInt("world/width")),
//   worldHeight(Gamedata::getInstace().getXmlInt("world/height")),
//   observers(p.observers)
//   {}

void Player::stop() {
  player.setVelocity(Vector2f(0, 0));
}

void Player::right() {
  if(player.getX() < worldWidth - getScaledWidth()){
    player.setVelocityX(initialVelocity[0]);
  }
}

void Player::left() {
  if(player.getX() > 0) {
    player.setVelocityX(-initialVelocity[0]);
  }
}

void Player::up() {
  if(player.getY() > 0) {
    player.setVelocityY(-initialVelocity[1]);
  }
}

void Player::down() {
  if(player.getY() < worldHeight - getScaledHeight()) {
    player.setVelocityY(initialVelocity[1]);
  }
}

void Player::shoot() {
  if(timeSinceLastFrame < bulletInterval) return;
  float deltaX = getScaledWidth();
  float deltaY = getScaledHeight() / 2;

  Bullet bullet(bulletName);
  bullet.setPosition(player.getPosition() + Vector2f(deltaX, deltaY));
  bullet.setVelocity(player.getVelocity() + Vector2f(minSpeed, 0));
  bullets.push_back(bullet);
  currentBullet++;
  timeSinceLastFrame = 0;
}

void Player::draw() const {
  player.draw();
  for(const Bullet& bullet : bullets) {
    bullet.draw();
  }
}

void Player::update(Uint32 ticks) {
  timeSinceLastFrame += ticks;
  player.update(ticks);

  for(Bullet& bullet: bullets) {
    bullet.update(ticks);
  }

  std::list<SmartSprite*>::iterator ptr = observers.begin();
  while(ptr != observers.end()) {
    (*ptr)->setPlayerPos(player.getPosition());
    ++ptr;
  }

  stop();
}

void Player::detach(SmartSprite* o) {
  std::list<SmartSprite*>::iterator ptr = observers.begin();
  while(ptr != observers.end()) {
    if(*ptr == o) {
      ptr = observers.erase(ptr);
      return;
    }
    ++ptr;
  }
}
