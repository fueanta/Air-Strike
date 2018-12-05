#ifndef ENEMY_H_INCLUDED
#define ENEMY_H_INCLUDED

#define FRW 0
#define BKW 1

#include "fighter.h"
#include "bullet.h"
#include "soundhandler.h"

class Enemy
{
  public:
    int life, stage, shootInterval;
    bool dir, gotHit;
    int shootProbability, shootDuration;

    Fighter enemyFighter;
    SoundHandler makeSound;
    float dx, curX, curY;
    queue<Bullet> bullets;

    Enemy();
    Enemy(int _stage);

    void drawEnemy();
    void startRender();
    void shoot();
};

Enemy::Enemy()
{
  stage=1;
  dx=0.002f; life=100;
  dir=false; gotHit=false;
  shootDuration=900;

  shootInterval=0;
  curX=MAX_X+2.0; curY=0.0;

  shootProbability=3;
}

Enemy::Enemy(int _stage)
{
  stage=_stage;
  dx=0.006f; life=100;
  dir=false; gotHit=false;
  shootDuration=500;

  shootInterval=0;
  curX=MAX_X+2.0; curY=0.0;

  shootProbability=2;
}

void Enemy::startRender()
{
  if(dir==FRW){curX-=dx; if(curX<MAX_X-3.0) dir=BKW;}
  else{curX+=dx; if(curX>MAX_X) dir=FRW;}

  drawEnemy();

  shootInterval++;
  if(shootInterval%shootDuration==0)
  {
    bool decide=rand()%shootProbability;
    if(curX<=MAX_X && decide)
    {
      shoot();
      makeSound.enemyShoot();
    }
    shootInterval=0;
  }
}

void Enemy::drawEnemy()
{
  glPushMatrix();
    glTranslatef(curX,curY,-7.0);
    if(stage==1) enemyFighter.drawEnemyRed();
    if(stage==2) enemyFighter.drawEnemyGreen();
  glPopMatrix();
}

void Enemy::shoot()
{
  Bullet thisBullet;
  thisBullet.posX=curX;
  thisBullet.posY=curY;
  if(stage==1) thisBullet.enemySpeed=0.020;
  if(stage==2) thisBullet.enemySpeed=0.030;
  bullets.push(thisBullet);
  //enemyBullets.push(thisBullet);
}

#endif // ENEMY_H_INCLUDED
