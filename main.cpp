#include <GL/glu.h>
#include <GL/glut.h>
#include <Windows.h>
#include <Mmsystem.h>
/*-------END Of OpenGL Libs------*/

#include <bits/stdc++.h>
#define PB push_back
#define MP make_pair
#define PI acos(-1.0)
#define EPS 0.000005
#define FL float
#define F first
#define S second
#define NL "\n"
#define PIS pair<int,string>
#define REP(_i,_n) for(int _i=0;_i<_n;_i++)
#define FOR(_i,_v,_n) for(int _i=_v;_i<=_n;_i++)
#define ALL(typ) typ.begin(),typ.end()
using namespace std;

#define MAX_X 5.20
#define MAX_Y 2.90

#define SPACEBAR 32
#define BS 8
#define CR 13

#include "game.h"
#include "imagehandler.h"
#include "soundhandler.h"

Game game;
SoundHandler gameSound;

BackScene thisScene;
int menuSelected=1;
/******************************/

/******************************/

///Called when the window is resized
void windowResize(int w, int h){
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (double)w / (double)h, 1.0, 200.0);
}

void displayMenu(){
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW); //Switch to the drawing perspective
	glLoadIdentity(); //Reset the drawing perspective

	thisScene.renderStartGame();
	//drawPlane();

	glPushMatrix();
    if(menuSelected==1) glTranslated(+1.2,+0.64,-7.0);
    if(menuSelected==2) glTranslated(+1.2,+0.14,-7.0);
    if(menuSelected==3) glTranslated(+1.2,-0.36,-7.0);
    glColor4f(0.937, 0.717, 0.0, 0.55);

    glBegin(GL_QUADS);
      glVertex3f(0.0,0.0,0.0);
      glVertex3f(0.0,-0.5,0.0);
      glVertex3f(+4.2,-0.5,0.0);
      glVertex3f(+4.2,0.0,0.0);
    glEnd();
	glPopMatrix();

	//thisScene.renderStartGame();

	glutSwapBuffers();
}

void displayScores(){
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW); //Switch to the drawing perspective
	glLoadIdentity(); //Reset the drawing perspective

	thisScene.renderStartScores();

	glutSwapBuffers();
}
///Draws the 3D scene
void display(){
  if(!game.isStarted)
  {
    if(!game.isScoreVw) displayMenu();
    else displayScores();
  }
  else game.startRender();
}

void myTimer(int value){
	//cout<<"*******is any update?*******\n";
	///Something..
	glutPostRedisplay(); //Tell GLUT that the display has changed

	///Tell GLUT to call update again in 25 milliseconds
	glutTimerFunc(25, myTimer, 0);
}

void enterFullScreen(){
  glutGameModeString("1024x600:32@60");
	glutEnterGameMode();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
}

void processSpecialKeys(int key, int x, int y){
	if(!game.isStarted)
  {
    switch(key)
    {
      case GLUT_KEY_UP: menuSelected--; break;
      case GLUT_KEY_DOWN: menuSelected++; break;
      default: break;
    }
    if(menuSelected<1) menuSelected++;
    if(menuSelected>3) menuSelected--;
  }

	if(game.isPaused) return;
	switch(key)
	{
    case GLUT_KEY_UP: game.myJet.moveUp(); break;
    case GLUT_KEY_DOWN: game.myJet.moveDown(); break;
    case GLUT_KEY_LEFT: game.myJet.moveLeft(); break;
    case GLUT_KEY_RIGHT: game.myJet.moveRight(); break;
    default: break;
	}
}

void toggleSound()
{
   game.makeSound.mute^=1;
   game.myJet.gameSound.mute^=1;
   int hasEnemy=0;
   hasEnemy=game.enemyList.size();
   while(hasEnemy--)
   {
     Enemy thisEnemy=game.enemyList.front();
     game.enemyList.pop();
     thisEnemy.makeSound.mute^=1;
     game.enemyList.push(thisEnemy);
   }
}

static void processKeys(unsigned char key, int x, int y){
  if(!game.isStarted)
  {
    if(game.isScoreVw)
    {
      if(key==CR) game.isScoreVw=false;
      return;
    }
    if(key==CR && menuSelected==1) game.isStarted=true;
    if(key==CR && menuSelected==2) game.isScoreVw=true;
    if(key==CR && menuSelected==3) exit(0); return;
  }

  if(game.isGameOver)
  {
    if((int)key==BS)
    {
      int pos=game.name.length()-1;
      if(pos==-1) return;
      game.name.erase(pos);
    }
    else if((int)key==CR)
    {
      fstream file;
      file.open("scores.txt",ios::in);

      vector<PIS>data;
      string name=game.name;
      if(name=="")name="****";

      int score=(int)game.myJet.score;
      data.PB(MP(score,name));

      while(!file.eof())
      {
         file>>name; file>>score;
         data.PB(MP(score,name));
      }

      file.close();

      sort(ALL(data), greater<PIS>());

      file.open("scores.txt",ios::out);
      REP(i,data.size())
      {
        if(i>9) break;
        if(i>0) if(data[i].S==data[i-1].S && data[i].F==data[i-1].F) continue;
        file<<data[i].S<<" "<<data[i].F<<NL;
      }
      file.close();

      Game newGame; game=newGame;
      glDisable(GL_DEPTH_TEST);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glEnable( GL_BLEND ); menuSelected=1;
    }
    else
    {
      if(key==' ')key='+';
      game.name+=key;
    }
    return;
  }

  //cout<<"HIT\n";
  if((int)key==SPACEBAR)
  {
    game.isPaused^=1;
    //if(game.isPaused) game.isPaused=false;
    //else game.isPaused=true;
  }

  if(game.isPaused) return;
  switch(key)
  {
    case 'm': toggleSound();
      break;
    case 'q':
       game.isGameOver=true;
       break;
    case 's':
      if(game.myJet.dead) break;
      game.myJet.shoot();
    break;

    default: break;
  }
  glutPostRedisplay();
}

void processMouse(int button, int state, int x, int y){
	//if(button == GLUT_LEFT_BUTTON) control.clicked((float)((x/currentWidth)-1), (float) ((y/currentHeight)-1));
}

static void idle(void){
  glutPostRedisplay();
}

int main(int argc, char** argv){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1280,720);
  glutInitWindowPosition(50,50);

  enterFullScreen();
	//glutCreateWindow("Air Combat");
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable( GL_BLEND );

	//glEnable(GL_DEPTH_TEST);
	initializeTexture();


  glutDisplayFunc(display);
	glutReshapeFunc(windowResize);
	glutIdleFunc(idle);
  glutKeyboardFunc(processKeys);
  glutSpecialFunc(processSpecialKeys);
  glutMouseFunc(processMouse);
	//glutTimerFunc(30, myTimer, 0);

	srand(time(0));
	glutMainLoop();
	return 0;
}
