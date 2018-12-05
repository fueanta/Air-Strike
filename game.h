#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include <bits/stdc++.h>
#define ACC 0.30
#define AC2 0.50

#include "backScene.h"
#include "bullet.h"
#include "imagehandler.h"
#include "hero.h"
#include "enemy.h"
#include "soundhandler.h"
#include "texthandler.h"

class Game{
  public:
    bool isPaused;
    bool isStarted;
    bool isGameOver;
    bool isScoreVw;
    bool isWaiting;
    string name;
    int stage,posDu;

    Hero myJet;
    BackScene scene;
    TextHandler makeTxt;
    SoundHandler makeSound;

    queue<Enemy>enemyList;
    queue<Bullet>enemyBullets;

    Game();
    void startRender();
    void createEnemy(float x, float y,int _stg);

    bool makeFightWithEnemy(Bullet thisBullet);
    bool makeFightWithHero(Bullet thisBullet);

    void renderHero();
    void manageStage();

    void setStage1();
    void setStage2();
    void setStage3();

    void renderEnemies();
    void collectBullets();
    void renderBullets();
    void gameOverMode();
};

Game::Game(){
  stage=0; name="";

  isPaused=false;
  isStarted=false;
  isGameOver=false;
  isScoreVw=false;

  //isStarted=true;
  //myJet.dead=true; return;
  //makeSound.background();

  //setStage1();
}

void Game::setStage1()
{
  ///Empty
  Enemy e1;
  e1.curX-=1.5;
  e1.curY+=2.18;
  enemyList.push(e1);

  Enemy e2;
  e2.curX-=0.0;
  e2.curY+=0.73;
  enemyList.push(e2);

  Enemy e3;
  e3.curX-=1.3;
  e3.curY-=0.75;
  enemyList.push(e3);

  Enemy e4;
  e4.curX-=-0.2;
  e4.curY-=2.17;
  enemyList.push(e4);
}

void Game::setStage2()
{
  Enemy e1(2);
  e1.curX-=1.5;
  e1.curY+=2.15;
  enemyList.push(e1);

  Enemy e2(1);
  e2.curX-=0.0;
  e2.curY+=1.10;
  enemyList.push(e2);

  Enemy e3(2);
  e3.curX-=1.3;
  e3.curY+=0.05;
  enemyList.push(e3);

  Enemy e4(1);
  e4.curX-=-0.2;
  e4.curY-=1.00;
  enemyList.push(e4);

  Enemy e5(2);
  e5.curX-=1.5;
  e5.curY-=2.15;
  enemyList.push(e5);
}

void Game::manageStage()
{
  if(myJet.score>0.0 && stage==0)
  {
    posDu=0;
    while(!myJet.bullets.empty()) myJet.bullets.pop();
    while(!enemyList.empty()) enemyList.pop();
    while(!enemyBullets.empty()) enemyBullets.pop();
    myJet.curX=-(MAX_X-1.3); myJet.curY=0.0;
    myJet.life=100; stage=1; setStage1();
  }

  if(myJet.score>10.0 && stage==1)
  {
    posDu=0;
    while(!myJet.bullets.empty()) myJet.bullets.pop();
    while(!enemyList.empty()) enemyList.pop();
    while(!enemyBullets.empty()) enemyBullets.pop();
    myJet.life=100; stage=2;
    myJet.curX=-(MAX_X-1.3); myJet.curY=0.0;
    scene.speed=0.008; setStage2();
  }
}

void Game::startRender(){
  glEnable(GL_DEPTH_TEST);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW); //Switch to the drawing perspective
	glLoadIdentity(); //Reset the drawing perspective


  //int lim=2500;
  int lim=50000;
  if(posDu<lim) posDu++;
  if(posDu>3) isWaiting=true;
  if(posDu==lim) isWaiting=false;

  if(posDu<=3 && stage==1) drawLvl1();
  if(posDu<=3 && stage==2) drawLvl2();
  if(isPaused || isWaiting) return;


  if(!isStarted)
  {
    scene.renderStartGame();
    glutSwapBuffers();
    return;
  }

  if(isGameOver)
  {
    makeTxt.getName(name);
    scene.renderGameOver();

    glutSwapBuffers();
    return;
  }

  myJet.score+=0.0001;
  manageStage();

  makeTxt.putScore(myJet.score);
  makeTxt.putLife(myJet.life);

  renderHero();
  renderEnemies();
  collectBullets();
  renderBullets();
  scene.startRender();

  glutSwapBuffers();
}

void Game::renderHero(){
  if(stage==1) myJet.drawHeroGreen();
  else myJet.drawHeroBlue();
}

void Game::createEnemy(float x, float y, int _stg){
  if(stage==1)
  {
    Enemy newEnemy;
    newEnemy.curY=y;
    newEnemy.curX=x+4.5;
    newEnemy.makeSound.mute=myJet.gameSound.mute;
    enemyList.push(newEnemy);
  }
  if(stage==2)
  {
    if(_stg==1)
    {
      Enemy newEnemy(2);
      newEnemy.curY=y;
      newEnemy.curX=x+4.5;
      newEnemy.makeSound.mute=myJet.gameSound.mute;
      enemyList.push(newEnemy);
    }
    else
    {
      Enemy newEnemy(1);
      newEnemy.curY=y;
      newEnemy.curX=x+4.5;
      newEnemy.makeSound.mute=myJet.gameSound.mute;
      enemyList.push(newEnemy);
    }
  }
}

bool Game::makeFightWithEnemy(Bullet thisBullet){
  bool gotHit=false;

  int hasEnemy=0;
  hasEnemy=enemyList.size();

  while(hasEnemy--)
  {
    Enemy thisEnemy=enemyList.front();
    enemyList.pop();

    float dx=fabs(thisBullet.posX-thisEnemy.curX);
    float dy=fabs(thisBullet.posY-thisEnemy.curY);
    if(dx<ACC && dy<ACC) thisEnemy.life-=100;

    if(thisEnemy.life<=0) gotHit=true;
    if(gotHit) makeSound.enemyDead();
    if(gotHit) myJet.score+=2.5;

    int stg=thisEnemy.stage;
    if(thisEnemy.life<=0)
       createEnemy(thisEnemy.curX, thisEnemy.curY, stg);
    if(thisEnemy.life<=0) break;

    else enemyList.push(thisEnemy);
  }

  if(gotHit) return true;
  else return false;
}

bool Game::makeFightWithHero(Bullet thisBullet){
  bool gotHit=true;

  float dx=fabs(thisBullet.posX-myJet.curX);
  float dy=fabs(thisBullet.posY-myJet.curY);

  if(dx<AC2 && dy<AC2 && dy>=ACC) myJet.life-=5;
  else if(dx<ACC && dy<ACC) myJet.life-=20;
  else gotHit=false;

  if(gotHit) makeSound.heroGotHit();

  if(gotHit) return true;
  else return false;
}

void Game::renderEnemies(){
  int hasEnemy=0;
  hasEnemy=enemyList.size();

  while(hasEnemy--)
  {
    Enemy thisEnemy=enemyList.front();
    enemyList.pop();

    if(thisEnemy.life<=0) continue;
    thisEnemy.startRender();
    enemyList.push(thisEnemy);
  }
}

void Game::collectBullets(){
  int hasEnemy=0;
  hasEnemy=enemyList.size();

  while(hasEnemy--)
  {
    Enemy thisEnemy=enemyList.front();
    enemyList.pop();

    int hasEnemyBullet=0;
    hasEnemyBullet=thisEnemy.bullets.size();

    while(hasEnemyBullet--)
    {
      Bullet thisBullet=thisEnemy.bullets.front();
      thisEnemy.bullets.pop();
      if(thisBullet.posX<-(MAX_X+1.0)) continue;
      enemyBullets.push(thisBullet);
    }
    enemyList.push(thisEnemy);
  }
}

void Game::renderBullets(){
  ///Hero Turn:
  int hasHeroBullet=0;
  hasHeroBullet=myJet.bullets.size();

  while(hasHeroBullet--)
  {
    Bullet thisBullet=myJet.bullets.front();
    myJet.bullets.pop();
    if(thisBullet.posX>MAX_X+1.0) continue;
    if(makeFightWithEnemy(thisBullet)) continue;

    thisBullet.drawHeroBullet();
    myJet.bullets.push(thisBullet);
  }

  ///Enemy Turn:
  int hasEnemyBullets=0;
  hasEnemyBullets=enemyBullets.size();

  while(hasEnemyBullets--)
  {
    Bullet thisBullet=enemyBullets.front();
    enemyBullets.pop();

    if(makeFightWithHero(thisBullet)) continue;

    thisBullet.drawEnemyBullet();
    enemyBullets.push(thisBullet);

    if(myJet.life<=0) myJet.dead=true;
    if(myJet.life<=0) isGameOver=true;
    if(myJet.life<=0) break;
  }
}

#endif // GAME_H_INCLUDED
