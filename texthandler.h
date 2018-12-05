#ifndef TEXTHANDLER_H_INCLUDED
#define TEXTHANDLER_H_INCLUDED

class TextHandler
{
  public:
    TextHandler();

    string toString(int n);
    void putLife(int n);
    void putScore(int n);
    void putRank();
    void getName(string name);
};

TextHandler::TextHandler()
{
  ///Default Empty..
}

void TextHandler::putLife(int n)
{
  string life="LIFE";
  glPushMatrix();
    glColor3f(1, 0.270, 0);
    glRasterPos3f( -4.9,2.65,-7.0);
    for(int i=0; i < life.length(); i++){
       glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, life[i]);
    }
  glPopMatrix();

  life=toString(n)+"%";
  glPushMatrix();
    glColor3f(1, 0.270, 0);
    glRasterPos3f( -1.4,2.65,-7.0);
    for(int i=0; i < life.length(); i++){
       glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, life[i]);
    }
  glPopMatrix();

  glPushMatrix();
  glTranslated(-4.5,2.60,-7);


  glColor3f(0.0,0.0,0.0);
  glBegin(GL_LINE_LOOP);
    glVertex3f(0.0,0.0,0.0);
    glVertex3f(3.0,0.0,0.0);
    glVertex3f(3.0,0.25,0.0);
    glVertex3f(0.0,0.25,0.0);
  glEnd();

  float cal=(3.0*(float)n)/100.0;
  glColor3f(1.0,0.0,0.0);
  glBegin(GL_POLYGON);
    glVertex3f(0.0,0.0,0.0);
    glVertex3f(cal,0.0,0.0);
    glVertex3f(cal,0.25,0.0);
    glVertex3f(0.0,0.25,0.0);
  glEnd();

  glPopMatrix();
}

void TextHandler::putScore(int n)
{
  string life="SCORE: " + toString(n);
  glPushMatrix();
    glColor3f(1, 0.270, 0);
    glRasterPos3f( -0.3,2.7,-7.0);
    for(int i=0; i < life.length(); i++){
       glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, life[i]);
    }
  glPopMatrix();
}

void TextHandler::putRank()
{
  fstream file;
  file.open("scores.txt",ios::in);

  vector<PIS>data;
  string name; int score;

  while(!file.eof())
  {
     file>>name; file>>score;
     data.PB(MP(score,name));
     //cout<<score<<" "<<name<<NL;
  }

  file.close();

  //file.open("scores.txt",ios::out);
  //REP(i,data.size()) file<<data[i].S<<" "<<data[i].F<<NL;
  //file.close();

  sort(ALL(data), greater<PIS>());

  string str="NULL";
  float cury=0.3;
  REP(k,data.size())
  {
    if(k>6) break; name=data[k].S;
    string _score=toString(data[k].F);

    cury-=0.3;
    glPushMatrix();
      glColor3f(0.901, 0.674, 0.0);
      glRasterPos3f( -0.7,cury,-7.0);
      for(int i=0; i < name.length(); i++){
        char cur=name[i]; if(cur=='+') cur=' ';
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, cur);
      }
    glPopMatrix();

    glPushMatrix();
      glColor3f(0.901, 0.674, 0.0);
      glRasterPos3f( 1.0,cury,-7.0);
      for(int i=0; i < _score.length(); i++){
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, _score[i]);
      }
    glPopMatrix();
  }
}

string TextHandler::toString(int n)
{
  if(n==0) return "0";
  string str="",res="";
  while(n) {str+=(n%10)+'0'; n/=10;}
  for(int i=str.length()-1;i>=0;i--) res+=str[i];
  return res;
}

void TextHandler::getName(string name)
{
  string life="NAME: " + name;
  glPushMatrix();
    glColor3f(0.901, 0.674, 0.0);
    glRasterPos3f( -0.7,-0.1,-7.0);
    for(int i=0; i < life.length(); i++){
      char cur=life[i]; if(cur=='+') cur=' ';
      glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, cur);
    }
  glPopMatrix();
}

#endif // TEXTHANDLER_H_INCLUDED
