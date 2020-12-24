#include <GL/glut.h>
#include <GL/glpng.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define WWIDTH 450
#define WHEIGHT 450

struct HAND
{
  int x[4];
  int y[4];
  double length;
  int color[3];
}HAND;

void Reshape(int w, int h);
void ImportImages(void);
void InitClockHands(struct HAND *hand, double length);
void Timer(int value);
void Animation(int value);
void Display(void);
void PrintText(int x, int y, char *s);
void PutSprite(int, int, int, pngInfo *, int r, int g, int b, int a);
void PutSpriteWithRot(int, int[], int[], pngInfo *, int r, int g, int b, int a);
void PutCharImg(int x, int y, char str, int r, int g, int b, int a);
void PutImgStr(int x, int y, char *s, int r, int g, int b, int a);
void setButton(int x, int y, int bw, int bh);
void PassiveMotion(int, int);
void Mouse(int, int, int, int);
void isButtonHovered(int x, int y, int type);
void isButtonClicked(int x, int y, int type);

int y = 0, mon = 0, d = 0, youbi = 0, h = 0, m = 0, s = 0;
time_t tt;
struct tm *ts;
double thh = 0.0, thm = 0.0, ths = 0.0;
int xc = WWIDTH/2, yc = WHEIGHT/2 + 20;
int hx[4] = {0}, hy[4] = {0}, mx[4] = {0}, my[4] = {0},sx[4] = {0}, sy[4] = {0};
double lh = 96.0, lm = 132.0, ls = 160.0;
char dateStr[11];
char timeStr[9];
char monthStr[3];
int xdh = 0, ydh = 0;
double dth = 0.0;
char days[7][16] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
int satelliteSpx[10] = {0}, satelliteSpy[10] = {0};
int satellitex[10] = {0}, satellitey[10] = {0};
int bx = 0, by = 0;
int buttonWidth = 0, buttonHeight = 0;
int bf = 0;
int opacity = 0, infoOpacity = 0;
int hovf = 0;
int ovf = 0;
int aniDatex = 0, aniDatey[6] = {1000};
int aniRingx[4] = {0}, aniRingy[4] = {0};
int aniInfox = 380;
int aniRingCnt = 0;
int aniHovCnt = 0;
int aniOvCnt = 0;
int cal[6][7] = {{0}};
char dayStr[3];
int datex = 20, datey = 125;

struct HAND hour, minute, second;

GLuint bg, ov;
pngInfo bg_info, ov_info;
GLuint ring[4];
pngInfo ring_info[4];
GLuint t_str[76];
pngInfo t_str_info[76];

int main(int argc, char **argv)
{
  int i;
  int scrWidth = 0, scrHeight = 0;

  srandom(12345);
  glutInit(&argc, argv);
  scrWidth = glutGet(GLUT_SCREEN_WIDTH);
  scrHeight = glutGet(GLUT_SCREEN_HEIGHT);
  glutInitWindowSize(WWIDTH, WHEIGHT);
  glutCreateWindow("Satellite - A Simplified Clock");
  glutPositionWindow((scrWidth-WWIDTH)/2, (scrHeight-WHEIGHT)/2);
  glutReshapeFunc(Reshape);
  glutTimerFunc(200, Timer, 0);
  glutTimerFunc(20, Animation, aniOvCnt);
  glutDisplayFunc(Display);
  glutInitDisplayMode(GLUT_RGBA | GLUT_ALPHA);
  glClearColor(0.0, 0.0, 1.0, 1.0);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

  ImportImages();

  glutPassiveMotionFunc(PassiveMotion);
  glutMouseFunc(Mouse);

  InitClockHands(&hour, lh);
  InitClockHands(&minute, lm);
  InitClockHands(&second, ls);

  for (i = 0; i < 10; i++) {
    satelliteSpx[i] = WWIDTH;
  }

  glutMainLoop();

  return 0;
}

void Reshape(int w, int h)
{
  //printf("%d x %d\n", w, h);

  glutReshapeWindow(WWIDTH, WHEIGHT);

  //printf("af : %d x %d\n", w, h);

  glViewport(0, 0, w, h);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluOrtho2D(0, w, 0, h);
  glScaled(1, -1, 1);
  glTranslated(0, -h, 0);
  //glutReshapeWindow(int w, int h);
  // int glutGet(GLUT_WINDOW?WIDTH);
}

void ImportImages(void)
{
  int i = 0;
  char t[24];

  sprintf(t, "./images/bg.png");
  bg = pngBind(t, PNG_NOMIPMAP, PNG_ALPHA, &bg_info, GL_CLAMP, GL_NEAREST, GL_NEAREST);
  //printf("bg : id=%d, W=%d, H=%d, D=%d, A=%d\n", t_str[i], bg_info.Width, bg_info.Height, bg_info.Depth, bg_info.Alpha);

  sprintf(t, "./images/ov.png");
  ov = pngBind(t, PNG_NOMIPMAP, PNG_ALPHA, &ov_info, GL_CLAMP, GL_NEAREST, GL_NEAREST);
  //printf("ov : id=%d, W=%d, H=%d, D=%d, A=%d\n", t_str[i], ov_info.Width, ov_info.Height, ov_info.Depth, ov_info.Alpha);

  for (i = 0; i < 4; i++) {
    sprintf(t, "./images/ring/ri%d.png", i);
    ring[i] = pngBind(t, PNG_NOMIPMAP, PNG_ALPHA, &ring_info[i], GL_CLAMP, GL_NEAREST, GL_NEAREST);
    //printf("ring%d : id=%d, W=%d, H=%d, D=%d, A=%d\n", i, ring[i], ring_info[i].Width, ring_info[i].Height, ring_info[i].Depth, ring_info[i].Alpha);
  }

  for (i = 0; i < 76; i++) {
    sprintf(t, "./images/fonts/%d.png", i+47);
    t_str[i] = pngBind(t, PNG_NOMIPMAP, PNG_ALPHA, &t_str_info[i], GL_CLAMP, GL_NEAREST, GL_NEAREST);
    //printf("img%d : id=%d, W=%d, H=%d, D=%d, A=%d\n", i, t_str[i], t_str_info[i].Width, t_str_info[i].Height, t_str_info[i].Depth, t_str_info[i].Alpha);
  }
}

void InitClockHands(struct HAND *hand, double length)
{
  int i;

  hand->x[0] = xc;
  hand->y[0] = yc;

  for (i = 0; i < 3; i++) {
    hand->x[i+1] = xc;
    hand->y[i+1] = yc;
    hand->color[i] = 0;
  }

  hand->length = length;
}

void Timer(int value)
{
  int row, col, daynum = 1;
  int diff = 0;
  int ldate = 31;

  // 日付・時刻の取得
  time(&tt);
  ts = localtime(&tt);  // tsに時間情報を代入
  y = ts->tm_year + 1900; // 年
  mon = ts->tm_mon + 1; // 月
  d = ts->tm_mday;  // 日
  youbi = ts->tm_wday;  // 曜日
  h = ts->tm_hour;  // 時
  m = ts->tm_min; // 分
  s = ts->tm_sec; // 秒

  // printf("%d年%d月%d日（%.3s）  %d時%d分%d秒\n", y, mon, d, days[youbi], h, m, s);

  // 日付・時刻を配列に格納
  sprintf(dateStr, "%04d/%02d/%02d", y, mon, d);
  sprintf(timeStr, "%02d:%02d:%02d", h, m, s);

  diff = youbi - (d-1) % 7;
  if (diff < 0) {
    diff = 7 + diff;
  }

  row = diff;
  for (col = 0; col < 6 && daynum < ldate; col++) {
    for (; row < 7 && daynum < ldate; row++) {
      cal[col][row] = daynum;
      daynum++;
    }
    row = 0;
  }

  // 針の座標を算出
  // 秒針
  ths = (2 * M_PI * s) / 60;  // 角度
  second.x[1] = xc + (int)(second.length * sin(ths)); // x
  second.y[1] = yc - (int)(second.length * cos(ths)); // y
  second.x[2] = xc + (int)(10.0 * sin(ths - M_PI/6)); // x
  second.y[2] = yc - (int)(10.0 * cos(ths - M_PI/6)); // y
  second.x[3] = xc + (int)(10.0 * sin(ths + M_PI/6)); // x
  second.y[3] = yc - (int)(10.0 * cos(ths + M_PI/6)); // y

  // 長針
  thm = (2 * M_PI * (60 * m + s)) / 3600; // 角度
  minute.x[1] = xc + (int)(minute.length * sin(thm)); // x
  minute.y[1] = yc - (int)(minute.length * cos(thm)); // y
  minute.x[2] = xc + (int)(10.0 * sin(thm - M_PI/6)); // x
  minute.y[2] = yc - (int)(10.0 * cos(thm - M_PI/6)); // y
  minute.x[3] = xc + (int)(10.0 * sin(thm + M_PI/6)); // x
  minute.y[3] = yc - (int)(10.0 * cos(thm + M_PI/6)); // y

  // 短針
  thh = (2 * M_PI * (3600 * h + 60 * m + s)) / 43200; // 角度
  hour.x[1] = xc + (int)(hour.length * sin(thh)); // x
  hour.y[1] = yc - (int)(hour.length * cos(thh)); // y
  hour.x[2] = xc + (int)(10.0 * sin(thh - M_PI/6)); // x
  hour.y[2] = yc - (int)(10.0 * cos(thh - M_PI/6)); // y
  hour.x[3] = xc + (int)(10.0 * sin(thh + M_PI/6)); // x
  hour.y[3] = yc - (int)(10.0 * cos(thh + M_PI/6)); // y

  // Display関数を強制呼び出し
  glutPostRedisplay();

  // 200ms後に再び自分自身を呼び出す
  glutTimerFunc(200, Timer, 0);
}

void Animation(int value)
{
  int i = 0;
  //double theta = 0;

  for (i = 0; i < 10; i++) {
    if (satelliteSpx[i] < WWIDTH && satelliteSpy[i] < WHEIGHT) {
      satellitex[i]++;
      satellitey[i]++;
      if ((satellitex[i] - satelliteSpx[i] > 100) || (satellitex[i] - satelliteSpx[i] > 100) ) {
        satelliteSpx[i]++;
        satelliteSpy[i]++;
      }
    }
    else {
      if (random() % 2 == 0) {
        satelliteSpx[i] = random() % WWIDTH;
        satelliteSpy[i] = 0;
      }
      else {
        satelliteSpy[i] = random() % WWIDTH;
        satelliteSpx[i] = 0;
      }
      satellitex[i] = satelliteSpx[i];
      satellitey[i] = satelliteSpy[i];
    }
  }

  // if (aniRingCnt > 1000) {
  //   aniRingCnt = 0;
  // }
  // for (i = 0; i < 4; i++) {
  //   theta = ((2 * M_PI * aniRingCnt) / 1000) - ((M_PI / 2) * i);
  //   aniRingx[i] = xc + (int)(200 * sin(theta));
  //   aniRingy[i] = yc + (int)(200 * cos(theta));
  //   //printf("%d : %d, %d\n", i, aniRingx[i], aniRingy[i]);
  //   aniRingCnt++;
  // }

  if (hovf == 1) {
    infoOpacity+=8;
    if (infoOpacity > 255) {
      infoOpacity = 255;
    }
    aniInfox -= aniInfox / 108 + 1;
    if (aniInfox < 330) {
      aniInfox = 330;
    }
  }
  else {
    infoOpacity-=16;
    if (infoOpacity < 0) {
      infoOpacity = 0;
    }
    if (infoOpacity <= 0) {
        aniInfox = 380;
    }
  }

  if (ovf == 1) {
    opacity+=16;
    if (opacity > 255) {
      opacity = 255;
    }
    aniDatey[0] -= aniDatey[0] / 8 + 1;
    if (aniDatey[0] < 0) {
      aniDatey[0] = 0;
    }
    for (i = 1; i < 6; i++) {
      if (aniDatey[i-1] <= 500) {
        aniDatey[i] -= aniDatey[i] / 8 + 1;
      }
      if (aniDatey[i] < 0) {
        aniDatey[i] = 0;
      }
    }
  }
  else {
    opacity-=16;
    if (opacity < 0) {
      opacity = 0;
    }
    for (i = 0; i < 6; i++) {
      if (opacity <= 0) {
        aniDatey[i] = 1000;
      }
    }
  }

  // Display関数を強制呼び出し
  glutPostRedisplay();

  glutTimerFunc(20, Animation, aniOvCnt);
}

//
// ウィンドウの表示内容を更新する関数
//
void Display(void)
{
  int i = 0, j = 0;
  int r = 0, g = 0, b = 0;

  glClear(GL_COLOR_BUFFER_BIT);

  PutSprite(bg, 0, 0, &bg_info, 255, 255, 255, 255);

  glLineWidth(1.0);
  glBegin(GL_LINES);
  glColor4ub(255, 255, 255, 128);
  for (i = 0; i < 10; i++) {
    glVertex2i(satelliteSpx[i], satelliteSpy[i]);
    glVertex2i(satellitex[i], satellitey[i]);
  }
  glEnd();

  for (i = 0; i < 4; i++) {
    PutSprite(ring[i], 65, 85, &ring_info[i], 255, 255, 255, 128);
  }

  glEnable(GL_LINE_SMOOTH);
  glLineWidth(2.0);
  glBegin(GL_LINES);
  glColor3ub(237, 219, 157);
  // glVertex2i(second.x[0], second.y[0]);
  // glVertex2i(second.x[1], second.y[1]);
  glVertex2i(second.x[0], second.y[0]);
  glVertex2i(second.x[2], second.y[2]);
  glVertex2i(second.x[2], second.y[2]);
  glVertex2i(second.x[1], second.y[1]);
  glVertex2i(second.x[0], second.y[0]);
  glVertex2i(second.x[3], second.y[3]);
  glVertex2i(second.x[3], second.y[3]);
  glVertex2i(second.x[1], second.y[1]);

  //glColor3ub(0, 255, 0);
  // glVertex2i(minute.x[0], minute.y[0]);
  // glVertex2i(minute.x[1], minute.y[1]);
  glVertex2i(minute.x[0], minute.y[0]);
  glVertex2i(minute.x[2], minute.y[2]);
  glVertex2i(minute.x[2], minute.y[2]);
  glVertex2i(minute.x[1], minute.y[1]);
  glVertex2i(minute.x[0], minute.y[0]);
  glVertex2i(minute.x[3], minute.y[3]);
  glVertex2i(minute.x[3], minute.y[3]);
  glVertex2i(minute.x[1], minute.y[1]);

  //glColor3ub(255, 255, 255);
  // glVertex2i(hour.x[0], hour.y[0]);
  // glVertex2i(hour.x[1], hour.y[1]);
  glVertex2i(hour.x[0], hour.y[0]);
  glVertex2i(hour.x[2], hour.y[2]);
  glVertex2i(hour.x[2], hour.y[2]);
  glVertex2i(hour.x[1], hour.y[1]);
  glVertex2i(hour.x[0], hour.y[0]);
  glVertex2i(hour.x[3], hour.y[3]);
  glVertex2i(hour.x[3], hour.y[3]);
  glVertex2i(hour.x[1], hour.y[1]);

  glEnd();

  // for (i = 0; i < strlen(t); i++)
  // {
  //   PutCharImg(81 + (i * 24), 311, t[i], 0, 0, 0, 255);
  // }

  for (i = 0; i < strlen(dateStr); i++)
  {
    PutCharImg(202 + (i * 24), 8, dateStr[i], 255, 255, 255, 255);
  }

  for (i = 0; i < strlen(timeStr); i++)
  {
    PutCharImg(250 + (i * 24), 48, timeStr[i], 255, 255, 255, 255);
  }

  PutImgStr(8, 408, days[youbi], 255, 255, 255, 255);

  PutImgStr(aniInfox, 408, "Cal", 255, 255, 255, infoOpacity);

  // glColor3ub(255, 255, 255);
  // for (i = 0; i < 12; i++)
  // {
  //   dth = (2 * M_PI * (3600 * i)) / 43200;
  //   xdh = xc + (int)((ls + 15) * sin(dth)) - 4;
  //   ydh = yc - (int)((ls + 15) * cos(dth)) + 5;

  //   glRasterPos2i(xdh, ydh);
  //   glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, dh[i][0]);
  //   glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, dh[i][1]);
  // }

  // PutImgStr(100, 400, days[0], 255, 255, 255, 255);
  // PutImgStr(100, 440, days[1], 255, 255, 255, 255);
  // PutImgStr(100, 480, days[2], 255, 255, 255, 255);
  // PutImgStr(100, 520, days[3], 255, 255, 255, 255);
  // PutImgStr(100, 560, days[4], 255, 255, 255, 255);
  // PutImgStr(100, 600, days[5], 255, 255, 255, 255);
  // PutImgStr(300, 400, days[6], 255, 255, 255, 255);

  // glColor4ub(246, 135, 63, opacity);
  // glBegin(GL_QUADS);
  // glVertex2i(0, 0);
  // glVertex2i(0, WHEIGHT);
  // glVertex2i(WWIDTH, WHEIGHT);
  // glVertex2i(WWIDTH, 0);
  // glEnd();

  PutSprite(ov, 0, 0, &ov_info, 255, 255, 255, opacity);

  glEnable(GL_LINE_SMOOTH);
  glLineWidth(2.0);
  glColor4ub(255, 255, 255, opacity/2);
  glBegin(GL_LINES);
  glVertex2i(190, 97);
  glVertex2i(260, 97);
  glVertex2i(190, 101);
  glVertex2i(260, 101);
  glEnd();

  if (mon > 9) {
    sprintf(monthStr, "%d", mon);
    PutImgStr(200, 60, monthStr, 255, 255, 255, opacity);
    PutImgStr(201, 61, monthStr, 255, 255, 255, opacity);
  }
  else {
    sprintf(monthStr, "%d", mon);
    PutImgStr(212, 60, monthStr, 255, 255, 255, opacity);
    PutImgStr(213, 61, monthStr, 255, 255, 255, opacity);
  }

  for (i = 0; i < 6; i++) {
    for (j = 0; j < 7; j++) {
      if (j == 0) {
        r = 244;
        g = 41;
        b = 102;
      }
      else if (j == 6) {
        r = 41;
        g = 143;
        b = 244;
      }
      else {
        r = 255;
        g = 255;
        b = 255;
      }
      if (cal[i][j] != 0) {
        sprintf(dayStr, "%02d", cal[i][j]);
        PutCharImg(datex + (j * 60), datey + (i * 50) + aniDatey[i], dayStr[0], r, g, b, opacity);
        PutCharImg(datex + (j * 60) + 24, datey + (i * 50) + aniDatey[i], dayStr[1], r, g, b, opacity);
        if (cal[i][j] == d) {
          glEnable(GL_LINE_SMOOTH);
          glLineWidth(1.5);
          glColor4ub(255, 255, 255, opacity / 2);
          glBegin(GL_LINES);
          glVertex2i(datex + (j * 60), datey + (i * 50) + aniDatey[i] + 38);
          glVertex2i(datex + (j * 60) + 50, datey + (i * 50) + aniDatey[i] + 38);
          glEnd();
        }
      }
    }
  }

  buttonWidth = 42;
  buttonHeight = 42;
  bx = 400;
  by = 400;
  if (bf == 1) {
    glColor3ub(146, 96, 34);
    setButton(bx, by, buttonWidth, buttonHeight);
  }
  else {
    glColor3ub(196, 146, 84);
    setButton(bx, by, buttonWidth, buttonHeight);
  }

  glEnable(GL_LINE_SMOOTH);
  glLineWidth(1.0);
  glColor4ub(255, 255, 255, opacity);
  glBegin(GL_LINES);
  glVertex2i(bx + 8, by + 8);
  glVertex2i(bx + buttonWidth - 8, by + buttonHeight - 8);
  glVertex2i(bx + buttonWidth - 8, by + 8);
  glVertex2i(bx + 8, by + buttonHeight - 8);
  glEnd();

  glFlush();
}

void PrintText(int x, int y, char *s)
{
  int i = 0;

  glRasterPos2i(x, y);
  for (i = 0; i < strlen(s); i++)
  {
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, s[i]);
  }
}

void PutSprite(int num, int x, int y, pngInfo *info, int r, int g, int b, int a)
{
  int w, h; // テクスチャの幅と高さ

  w = info->Width; // テクスチャの幅と高さを取得する
  h = info->Height;

  glPushMatrix();
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, num);
  glColor4ub(r, g, b, a);

  glBegin(GL_QUADS); // 幅w, 高さhの四角形

  glTexCoord2i(0, 0);
  glVertex2i(x, y);

  glTexCoord2i(0, 1);
  glVertex2i(x, y + h);

  glTexCoord2i(1, 1);
  glVertex2i(x + w, y + h);

  glTexCoord2i(1, 0);
  glVertex2i(x + w, y);

  glEnd();

  glDisable(GL_TEXTURE_2D);
  glPopMatrix();
}

void PutSpriteWithRot(int num, int *x, int *y, pngInfo *info, int r, int g, int b, int a)
{
  // int w, h; // テクスチャの幅と高さ

  // w = info->Width; // テクスチャの幅と高さを取得する
  // h = info->Height;

  glPushMatrix();
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, num);
  glColor4ub(r, g, b, a);

  glBegin(GL_QUADS); // 幅w, 高さhの四角形

  glTexCoord2i(0, 0);
  glVertex2i(x[0], y[0]);

  glTexCoord2i(0, 1);
  glVertex2i(x[1], y[1]);

  glTexCoord2i(1, 1);
  glVertex2i(x[2], y[2]);

  glTexCoord2i(1, 0);
  glVertex2i(x[3], y[3]);

  glEnd();

  glDisable(GL_TEXTURE_2D);
  glPopMatrix();
}

void PutCharImg(int x, int y, char str, int r, int g, int b, int a)
{
  PutSprite(t_str[((int)str) - 47], x, y, &t_str_info[((int)str) - 47], r, g, b, a);
}

void PutImgStr(int x, int y, char *s, int r, int g, int b, int a)
{
  int i = 0, px = 0, py = 0;

  PutCharImg(x + (i * 20) + px, y + py, s[0], r, g, b, a);
  px = 0;
  py = 0;
  for (i = 1; i < strlen(s); i++)
  {
    if (s[i] == 'y') {
      py += 10;
    }
    if (s[i-1] >= 'A' && s[i-1] <= 'Z') {
      if (s[i-1] == 'M' || s[i-1] == 'W') {
        px += 8;
      }
      if (s[i-1] == 'C') {
        px += 6;
      }
      px += 2;
    }
    if (s[i-1] == 'u') {
      px -= 2;
    }
    if (s[i-1] == 'i') {
      px -= 8;
    }
    if (s[i-1] == 'r') {
      px -= 3;
    }
    if (s[i-1] == 's') {
      px -= 3;
    }
    if (s[i] == 's') {
      px -= 3;
    }
    PutCharImg(x + (i * 20) + px, y + py, s[i], r, g, b, a);
    //px = 0;
    py = 0;
  }
}

void setButton(int x, int y, int bw, int bh)
{
  glBegin(GL_QUADS);
  glVertex2i(x, y);
  glVertex2i(x+bw, y);
  glVertex2i(x+bw, y+bh);
  glVertex2i(x, y+bh);
  glEnd();
}

void PassiveMotion(int x, int y)
{
  //printf("PassiveMotion : (x, y)=(%d, %d)\n", x, y);
  isButtonHovered(x, y, 0);
}

void Mouse (int b, int s, int x, int y)
{
  if (b == GLUT_LEFT_BUTTON) {
    if (s == GLUT_UP) {
      if (bf == 1) {
        //printf("[Left button up]");
        bf = 0;
      }
    }
    if (s == GLUT_DOWN) {
      //printf("[Left button down]");
      isButtonClicked(x, y, 0);
    }
  }

  //printf(" at (%d, %d)\n", x, y);
  glutPostRedisplay();
}

void isButtonHovered(int x, int y, int type)
{
  if (x >= bx && x <= bx+buttonWidth && y >= by && y <= by+buttonHeight) {
    //printf("button\n");
    hovf = 1;
    aniHovCnt = 0;
  }
  else {
    hovf = 0;
  }
}

void isButtonClicked(int x, int y, int type)
{
  if (x >= bx && x <= bx+buttonWidth && y >= by && y <= by+buttonHeight) {
    //printf("button\n");
    ovf ^= 1;
    bf = 1;
    aniOvCnt = 0;
  }
}