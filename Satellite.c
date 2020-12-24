/*****************************************************************************/
/*     ========== ========== ========== ========== ========== ==========     */
/*     --------- Analog/Digital Clock - [Satellite] | ver.1.2.1 --------     */
/*     ========== ========== ========== ========== ========== ==========     */
/* ------------------------------------------------------------------------- */
/* 【 名前 】 Satellite - A Simplified Clock                                  */
/* 【 制作者 】 岡田隼飛                       */
/* 【 開発環境 】 cygwin ver.3.0.7-1 | gcc ver.7.4.0 | GNU make ver.4.2.1     */
/* 【 動作環境 】 Windows10 ver.1909 (OS Build : 18363.1198)                  */
/* 【 バージョン 】 ver.1.2.1                                                 */
/* 【 最終更新日 】 2020年 11月 23日（勤労感謝の日）                            */
/* ------------------------------------------------------------------------- */
/*                                                                           */
/*                           .:-----=:                                       */
/*                       .---:      .-                                       */
/*                     :-:     :-:---+----:                       :-         */
/*                   --.   :--:.    .-    .:--:                 :-.          */
/*                 :-.  .--.        .+::.     .--.            :-.            */
/*               .-.  .-:              .:---:    :-.        :-.              */
/*              .-   -:                     .--:   :-     :-.                */
/*             :-  .-                          .-.  .=. --.                  */
/*            .-  :-                             :-   +=.                    */
/*           .=  .-                                =--.-.                    */
/*           =  .=                                --=.  =                    */
/*          ::  =     .--                       --.  =   =                   */
/*          =  ::       .--                   --.    .-  ::                  */
/*         .-  =           --.              --.       -.  =                  */
/*         -.  =             --.  :-::-:  --.         .-  =                  */
/*         =  ::               :-=.    .+-.            =  ::                 */
/*         =  -.                -:--  --.-             =  .-                 */
/*         ---+---=             -  .+=   -             :--=+--=              */
/*            -.  =             ::  =   ::                ::  =              */
/*            .-  =              :-:- .-:                 -.  =              */
/*             =  -:               +:::                   =  ::              */
/*             =   =               =                     .-  =               */
/*             .=  ::             ::                     =  .-               */
/*              -:  -.            =                     ::  =                */
/*               =.  -.           =                    .-  ::                */
/*                =.  -:         -:                   .=  .-                 */
/*                 -.  .-.       =                   .-  .=                  */
/*                  -:   :-:    .=                  --  .-                   */
/*                   .-:   .--: -.                :-.  :-                    */
/*                     :-:    .:*---::          :-:  .-:                     */
/*                       .--:  .-    =       :--.   --                       */
/*                          .--+-::..=.::---:.   .--                         */
/*                             =  ...=..      .--:                           */
/*                            .-     =  ..:---:                              */
/*                            =      -:::.                                   */
/*                            =                                              */
/*                           ::                                              */
/*                           .                                               */
/*                                                                           */
/*                                                                           */
/*   Kono software wa Satellite (jinkou eisei) wo image shite tsukutte masu  */
/*****************************************************************************/
#include <GL/glut.h>
#include <GL/glpng.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

// ウィンドウサイズの定義
#define WWIDTH 450
#define WHEIGHT 450

//
// HAND構造体
// 時計の針 (HAND) の座標, 長さを格納
//
struct HAND
{
  int x[4]; // HANDのx座標
  int y[4]; // HANDの座標
  double length;  // HANDの長さ
} HAND;

// Prototype
void Reshape(int w, int h);
void ImportImages(void);
void InitClockHands(struct HAND *hand, double length);
void Timer(int value);
void Animation(int value);
void Display(void);
void PutSprite(int, int, int, pngInfo *, int r, int g, int b, int a);
void PutImgChar(int x, int y, char str, int r, int g, int b, int a);
void PutImgStr(int x, int y, char *s, int r, int g, int b, int a);
void setButton(int x, int y, int bw, int bh);
void PassiveMotion(int, int);
void Mouse(int, int, int, int);
void isButtonHovered(int x, int y, int type);
void isButtonClicked(int x, int y, int type);

/* _GLOBAL VARIABLES__________________________________________________________________________________________________________________________________________________ */
//
//
// ====== 現在時間算出用変数 ======
// ------ int ------
// y ... 年
// mon ... 月
// d ... 日
// youbi ... 曜日
// h ... 時
// m ... 分
// s ... 秒
// -----------------
//
// ------ time_t ------
// tt ... epochからの経過秒数
// --------------------
//
// ------ tm構造体 ------
// *ts ... time_tを変換し, 時間情報を格納（変換および格納はlocaltime関数, Timer関数で実行）
// ---------------------
//
// ------ int[][] ------
// cal[行][列] ... カレンダーの日付を格納する二次元配列 変数の場所が実際のカレンダーにおける各日付の位置に対応している
// ---------------------
int y = 0, mon = 0, d = 0, youbi = 0, h = 0, m = 0, s = 0;
time_t tt;
struct tm *ts;
int cal[6][7] = {{0}};
// ===============================

//
// ====== 現在時刻, 年月日の文字列 ======
// ------ char[] ------
// monthStr[] ... 現在の月を文字列として保存(1~12)
// dateStr[] ... 現在の年月日をYYYY/MM/DDの文字列で保存
// timeStr[] ... 現在時刻をHH:MM:SSの文字列で保存
// ------------------
char monthStr[3];
char dateStr[11];
char timeStr[9];
// ====================================

//
// ====== 衛星 (Satellite) アニメーション用変数 ======
// ------ int[] ------
// satelliteSpx[], satelliteSpy[] ... 衛星が描画される始点の座標
// satellitex[], satellitey[] ... ... 衛星が描画される終点の座標
// -------------------
int satelliteSpx[10] = {0}, satelliteSpy[10] = {0};
int satellitex[10] = {0}, satellitey[10] = {0};
// =================================================

//
// ====== ボタン用変数 ======
// ------ int ------
// bx, by ... ボタンの座標
// buttonWidth, buttonHeight ... ボタンの幅, 高さ
// bf ... ボタンが押されたか判定するためのフラグ
// -----------------
int bx = 0, by = 0;
int buttonWidth = 0, buttonHeight = 0;
int bf = 0;
// =========================

//
// ====== カレンダー画面のアニメーション用変数 ======
// ------ int ------
// aniInfox ... カレンダーボタンへマウスオーバー（ホバー）した際に出現するテキストのx座標（初期値はテキストが隠れる座標に設定）
// opacity ... フェードイン / アウトするカレンダー画面の不透明度
// infoOpacity ... カレンダーボタンへマウスオーバー（ホバー）した際に出現するテキストの不透明度
// hovf ... ボタンにホバーしているかを判定するフラグ
// ovf ... カレンダー画面の表示（オーバーレイ）を判定するフラグ
// aniDatex ... カレンダーの日付のx座標
// -----------------
//
// ------ int[] ------
// aniDatey[] ... カレンダーの日付の各行におけるy座標（初期値は画面外のy座標に設定）
// -------------------
int aniInfox = 380;
int opacity = 0, infoOpacity = 0;
int hovf = 0;
int ovf = 0;
int aniDatex = 0, aniDatey[6] = {1000};
// ==============================================

//
// ====== 時計の針 ======
// ------ HAND構造体 ------
// hour, minute, second ... 短針, 長針, 秒針の構造体
// -----------------------
struct HAND hour, minute, second;
// =====================

//
// ====== 画像用変数・構造体 ======
// ------ GLuint変数 ------
// bg ... アプリケーションの背景として読み込む画像の識別番号
// ov ... カレンダー画面の背景（オーバーレイ）として読み込む画像の識別番号
// --------------------
//
// ------ GLuint変数配列 ------
// ring[] ... 時計の文字盤画像として読み込む画像の識別番号
// t_str[] ... 各テキストに対応する画像の識別番号（ASCIIコードの47('/')から122('z')までが格納可能）
// ---------------------------
//
// ------ pngInfo構造体 ------
// bg_info ... 背景画像の情報
// ov_info ... オーバーレイ画像の情報
// ---------------------
//
// ------ pngInfo[]構造体配列 ------
// ring_info[] ... 各文字盤画像の情報
// t_str_info[] ...各テキストに対応する画像の情報
// --------------------------------
GLuint bg, ov;
pngInfo bg_info, ov_info;
GLuint ring[4];
pngInfo ring_info[4];
GLuint t_str[76];
pngInfo t_str_info[76];
// ==============================
//
/* _______________________________________________________________________________________________________________________________________________________________ */

/* _FUNCTIONS_____________________________________________________________________________________________________________________________________________________ */
//
// ====== main関数 ======
// ------ 機能 ------
// ウィンドウの初期化, コールバック関数の登録, アルファチャネルの有効化, 画像読み込み, 時計の針の初期化, 衛星の座標の初期化, イベントループ突入
// -----------------
// =====================
//
int main(int argc, char **argv)
{
  int i;
  int scrWidth = 0, scrHeight = 0; // 画面全体のサイズ
  double lh = 96.0, lm = 132.0, ls = 160.0; // 針の長さ

  // ウィンドウ初期化, 乱数初期化
  srandom(12345);
  glutInit(&argc, argv);
  scrWidth = glutGet(GLUT_SCREEN_WIDTH);
  scrHeight = glutGet(GLUT_SCREEN_HEIGHT);
  glutInitWindowSize(WWIDTH, WHEIGHT);
  glutCreateWindow("Satellite - A Simplified Clock");
  glutPositionWindow((scrWidth - WWIDTH) / 2, (scrHeight - WHEIGHT) / 2);
  glutInitDisplayMode(GLUT_RGBA | GLUT_ALPHA);
  glClearColor(0.0, 0.0, 1.0, 1.0);

  // アルファチャネルの有効化
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

  // 画像読み込み
  ImportImages();

  // コールバック関数の登録
  glutReshapeFunc(Reshape);
  glutTimerFunc(200, Timer, 0);
  glutTimerFunc(20, Animation, 0);
  glutDisplayFunc(Display);
  glutPassiveMotionFunc(PassiveMotion);
  glutMouseFunc(Mouse);

  // 各針の初期化
  InitClockHands(&hour, lh);
  InitClockHands(&minute, lm);
  InitClockHands(&second, ls);

  // 衛星座標の初期化
  for (i = 0; i < 10; i++) {
    satelliteSpx[i] = WWIDTH;
  }

  // イベントループ突入
  glutMainLoop();

  return 0;
}

//
// ====== Reshape関数 ======
// ------ 機能 ------
// ウィンドウサイズ変更時にウィンドウを再描画（サイズは固定）
// -----------------
// ========================
//
void Reshape(int w, int h)
{
  // ウィンドウサイズ固定
  glutReshapeWindow(WWIDTH, WHEIGHT);

  // 座標系の再設定
  glViewport(0, 0, w, h);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluOrtho2D(0, w, 0, h);
  glScaled(1, -1, 1);
  glTranslated(0, -h, 0);
}

//
// ====== ImportImages関数 ======
// ------ 機能 ------
// 画像の読み込み
// -----------------
// =============================
void ImportImages(void)
{
  int i = 0;
  char t[24]; // 画像のファイル名・パスを格納

  // 背景画像の読み込み
  sprintf(t, "./images/bg.png");
  bg = pngBind(t, PNG_NOMIPMAP, PNG_ALPHA, &bg_info, GL_CLAMP, GL_NEAREST, GL_NEAREST);

  // オーバーレイ画像の読み込み
  sprintf(t, "./images/ov.png");
  ov = pngBind(t, PNG_NOMIPMAP, PNG_ALPHA, &ov_info, GL_CLAMP, GL_NEAREST, GL_NEAREST);

  // 文字盤画像の読み込み
  for (i = 0; i < 4; i++) {
    sprintf(t, "./images/ring/ri%d.png", i);
    ring[i] = pngBind(t, PNG_NOMIPMAP, PNG_ALPHA, &ring_info[i], GL_CLAMP, GL_NEAREST, GL_NEAREST);
  }

  // テキストに対応する画像の読み込み
  for (i = 0; i < 76; i++) {
    sprintf(t, "./images/fonts/%d.png", i + 47); // ファイル名はすべてASCIIコードに対応
    t_str[i] = pngBind(t, PNG_NOMIPMAP, PNG_ALPHA, &t_str_info[i], GL_CLAMP, GL_NEAREST, GL_NEAREST);
  }
}

//
// ====== InitClockHands関数 ======
// ------ 機能 ------
// 時計の各針を初期化
// -----------------
// ===============================
//
void InitClockHands(struct HAND *hand, double length)
{
  int i;

  // 針の座標を初期化
  for (i = 0; i < 4; i++) {
    hand->x[i] = WWIDTH / 2;
    hand->y[i] = WHEIGHT / 2 + 20;
  }

  // 針の長さを初期化
  hand->length = length;
}

//
// ====== Timer関数 ======
// ------ 機能 ------
// 現在の日付, 時刻を取得, 時計の各針の座標を算出, カレンダーの日付情報を格納
// -----------------
// ======================
//
void Timer(int value)
{
  int row, col; // row, column
  int daynum = 1; // 日付
  int diff = 0; // 現在の曜日と1日の曜日の差
  int ldate = 31; // 現在の月の日数
  double thh = 0.0, thm = 0.0, ths = 0.0; // 時計の各針における角度
  int xc = WWIDTH / 2, yc = WHEIGHT / 2 + 20; // 時計の中心座標

  // 日付・時刻の取得
  time(&tt);
  ts = localtime(&tt);    // tsに時間情報を代入
  y = ts->tm_year + 1900; // 年
  mon = ts->tm_mon + 1;   // 月
  d = ts->tm_mday;        // 日
  youbi = ts->tm_wday;    // 曜日
  h = ts->tm_hour;        // 時
  m = ts->tm_min;         // 分
  s = ts->tm_sec;         // 秒

  // 日付・時刻を配列に格納
  sprintf(dateStr, "%04d/%02d/%02d", y, mon, d);
  sprintf(timeStr, "%02d:%02d:%02d", h, m, s);

  // 現在の月の合計日数を判別（デフォルトは31日）
  if (mon == 4 || mon == 6 || mon == 9 || mon == 11) {
    ldate = 30;
  }
  if (mon == 2) {
    if (y % 400 == 0 || (y % 4 == 0 && y % 100 != 0)) {
      ldate = 29;
    }
    else {
      ldate = 28;
    }
  }

  // 現在の月における1日が何曜日であるかを算出
  diff = youbi - (d - 1) % 7;
  if (diff < 0) {
    diff = 7 + diff;
  }

  row = diff; // 1日の位置を設定
  // 擬似的にカレンダーを作成
  for (col = 0; col < 6 && daynum <= ldate; col++) {
    for (; row < 7 && daynum <= ldate; row++) {
      cal[col][row] = daynum;
      daynum++;
    }
    row = 0;
  }

  // 針の座標を算出
  // 秒針
  ths = (2 * M_PI * s) / 60;                            // 角度
  second.x[1] = xc + (int)(second.length * sin(ths));   // x
  second.y[1] = yc - (int)(second.length * cos(ths));   // y
  second.x[2] = xc + (int)(10.0 * sin(ths - M_PI / 6)); // x
  second.y[2] = yc - (int)(10.0 * cos(ths - M_PI / 6)); // y
  second.x[3] = xc + (int)(10.0 * sin(ths + M_PI / 6)); // x
  second.y[3] = yc - (int)(10.0 * cos(ths + M_PI / 6)); // y

  // 長針
  thm = (2 * M_PI * (60 * m + s)) / 3600;               // 角度
  minute.x[1] = xc + (int)(minute.length * sin(thm));   // x
  minute.y[1] = yc - (int)(minute.length * cos(thm));   // y
  minute.x[2] = xc + (int)(10.0 * sin(thm - M_PI / 6)); // x
  minute.y[2] = yc - (int)(10.0 * cos(thm - M_PI / 6)); // y
  minute.x[3] = xc + (int)(10.0 * sin(thm + M_PI / 6)); // x
  minute.y[3] = yc - (int)(10.0 * cos(thm + M_PI / 6)); // y

  // 短針
  thh = (2 * M_PI * (3600 * h + 60 * m + s)) / 43200; // 角度
  hour.x[1] = xc + (int)(hour.length * sin(thh));     // x
  hour.y[1] = yc - (int)(hour.length * cos(thh));     // y
  hour.x[2] = xc + (int)(10.0 * sin(thh - M_PI / 6)); // x
  hour.y[2] = yc - (int)(10.0 * cos(thh - M_PI / 6)); // y
  hour.x[3] = xc + (int)(10.0 * sin(thh + M_PI / 6)); // x
  hour.y[3] = yc - (int)(10.0 * cos(thh + M_PI / 6)); // y

  // Display関数を強制呼び出し
  glutPostRedisplay();

  // 200ms後に再び自分自身を呼び出す
  glutTimerFunc(200, Timer, 0);
}

//
// ====== Animation関数 ======
// ------ 機能 ------
// 動きのあるオブジェクトの座標や不透明度を算出
// -----------------
// ==========================
//
void Animation(int value)
{
  int i = 0;

  // 衛星の始点, 終点
  for (i = 0; i < 10; i++) {
    if (satelliteSpx[i] < WWIDTH && satelliteSpy[i] < WHEIGHT) {
      satellitex[i]++;
      satellitey[i]++;
      if ((satellitex[i] - satelliteSpx[i] > 100) || (satellitex[i] - satelliteSpx[i] > 100)) { // 一定の長さで衛星を進める
        satelliteSpx[i]++;
        satelliteSpy[i]++;
      }
    }
    else {
      if (random() % 2 == 0) {  // ランダムに始点を決定
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

  // ボタンへのマウスホバー時のアニメーション
  if (hovf == 1) {
    infoOpacity += 8;
    if (infoOpacity > 255) {
      infoOpacity = 255;
    }
    aniInfox -= aniInfox / 108 + 1;
    if (aniInfox < 330) {
      aniInfox = 330;
    }
  }
  else {
    infoOpacity -= 16;
    if (infoOpacity <= 0) {
      infoOpacity = 0;
      aniInfox = 380;
    }
  }

  // カレンダー画面（オーバーレイ）のアニメーション
  if (ovf == 1) {
    opacity += 16;
    if (opacity > 255) {
      opacity = 255;
    }
    aniDatey[0] -= aniDatey[0] / 8 + 1;
    if (aniDatey[0] < 0) {
      aniDatey[0] = 0;
    }
    for (i = 1; i < 6; i++) {
      if (aniDatey[i - 1] <= 500) { // 前の行よりも遅れて表示
        aniDatey[i] -= aniDatey[i] / 8 + 1;
      }
      if (aniDatey[i] < 0) {
        aniDatey[i] = 0;
      }
    }
  }
  else {
    opacity -= 16;
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

  // 自身をコールバック
  glutTimerFunc(20, Animation, 0);
}

//
// ====== Display関数 ======
// ------ 機能 ------
// ウィンドウの表示内容を更新
// -----------------
// ========================
//
void Display(void)
{
  int i = 0, j = 0;
  int r = 0, g = 0, b = 0; // 色情報 (RGB)
  char days[7][16] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"}; // 曜日
  char dayStr[3]; // 日付を一時的に格納
  int datex = 20, datey = 125; // カレンダーの座標

  // ウィンドウの背景色
  glClear(GL_COLOR_BUFFER_BIT);

  // 背景画像を表示
  PutSprite(bg, 0, 0, &bg_info, 255, 255, 255, 255);

  // 衛星を表示
  glLineWidth(1.0);
  glBegin(GL_LINES);
  glColor4ub(255, 255, 255, 128);
  for (i = 0; i < 10; i++) {
    glVertex2i(satelliteSpx[i], satelliteSpy[i]);
    glVertex2i(satellitex[i], satellitey[i]);
  }
  glEnd();

  // 文字盤を表示
  for (i = 0; i < 4; i++) {
    PutSprite(ring[i], 65, 85, &ring_info[i], 255, 255, 255, 128);
  }

  // 時計の針を表示
  glEnable(GL_LINE_SMOOTH);
  glLineWidth(2.0);
  glBegin(GL_LINES);
  glColor3ub(237, 219, 157);
  // 秒針
  glVertex2i(second.x[0], second.y[0]);
  glVertex2i(second.x[2], second.y[2]);
  glVertex2i(second.x[2], second.y[2]);
  glVertex2i(second.x[1], second.y[1]);
  glVertex2i(second.x[0], second.y[0]);
  glVertex2i(second.x[3], second.y[3]);
  glVertex2i(second.x[3], second.y[3]);
  glVertex2i(second.x[1], second.y[1]);

  // 長針
  glVertex2i(minute.x[0], minute.y[0]);
  glVertex2i(minute.x[2], minute.y[2]);
  glVertex2i(minute.x[2], minute.y[2]);
  glVertex2i(minute.x[1], minute.y[1]);
  glVertex2i(minute.x[0], minute.y[0]);
  glVertex2i(minute.x[3], minute.y[3]);
  glVertex2i(minute.x[3], minute.y[3]);
  glVertex2i(minute.x[1], minute.y[1]);

  // 短針
  glVertex2i(hour.x[0], hour.y[0]);
  glVertex2i(hour.x[2], hour.y[2]);
  glVertex2i(hour.x[2], hour.y[2]);
  glVertex2i(hour.x[1], hour.y[1]);
  glVertex2i(hour.x[0], hour.y[0]);
  glVertex2i(hour.x[3], hour.y[3]);
  glVertex2i(hour.x[3], hour.y[3]);
  glVertex2i(hour.x[1], hour.y[1]);

  glEnd();

  // 年月日を表示
  for (i = 0; i < strlen(dateStr); i++) {
    PutImgChar(202 + (i * 24), 8, dateStr[i], 255, 255, 255, 255);
  }

  // 現在時刻を表示
  for (i = 0; i < strlen(timeStr); i++) {
    PutImgChar(250 + (i * 24), 48, timeStr[i], 255, 255, 255, 255);
  }

  // 曜日を表示
  PutImgStr(8, 408, days[youbi], 255, 255, 255, 255);

  // カレンダーボタンホバー時にテキストを表示
  PutImgStr(aniInfox, 408, "Cal", 255, 255, 255, infoOpacity);

  // オーバーレイ画像を表示
  PutSprite(ov, 0, 0, &ov_info, 255, 255, 255, opacity);

  // カレンダーの月部分に下線を表示
  glEnable(GL_LINE_SMOOTH);
  glLineWidth(2.0);
  glColor4ub(255, 255, 255, opacity / 2);
  glBegin(GL_LINES);
  glVertex2i(190, 97);
  glVertex2i(260, 97);
  glVertex2i(190, 101);
  glVertex2i(260, 101);
  glEnd();

  // 二桁の月との場合分け
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

  // カレンダーの日付を表示
  for (i = 0; i < 6; i++) {
    for (j = 0; j < 7; j++) {
      if (j == 0) { // 日曜日は赤色に設定
        r = 244;
        g = 41;
        b = 102;
      }
      else if (j == 6) { // 土曜日は青色に設定
        r = 41;
        g = 143;
        b = 244;
      }
      else { // 平日は白色に設定
        r = 255;
        g = 255;
        b = 255;
      }
      if (cal[i][j] != 0) { // 日付の情報がある場合に日付を表示
        sprintf(dayStr, "%02d", cal[i][j]);
        PutImgChar(datex + (j * 60), datey + (i * 50) + aniDatey[i], dayStr[0], r, g, b, opacity);
        PutImgChar(datex + (j * 60) + 24, datey + (i * 50) + aniDatey[i], dayStr[1], r, g, b, opacity);
        if (cal[i][j] == d) { // 今日の日付には下線を表示
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

  // ボタンの座標, 大きさを設定
  buttonWidth = 42;
  buttonHeight = 42;
  bx = 400;
  by = 400;
  if (bf == 1) {  // フラグ発生時にボタンの色を濃くする
    glColor3ub(146, 96, 34);
    setButton(bx, by, buttonWidth, buttonHeight);
  }
  else {
    glColor3ub(196, 146, 84);
    setButton(bx, by, buttonWidth, buttonHeight);
  }

  // カレンダー画面を表示しているときはボタンに「x」マークを表示
  glEnable(GL_LINE_SMOOTH);
  glLineWidth(1.0);
  glColor4ub(255, 255, 255, opacity);
  glBegin(GL_LINES);
  glVertex2i(bx + 8, by + 8);
  glVertex2i(bx + buttonWidth - 8, by + buttonHeight - 8);
  glVertex2i(bx + buttonWidth - 8, by + 8);
  glVertex2i(bx + 8, by + buttonHeight - 8);
  glEnd();

  // キューの吐き出し
  glFlush();
}

//
// ====== PutSprite ======
// ------ 機能 ------
// 画像を表示
// -----------------
// =======================
//
void PutSprite(int num, int x, int y, pngInfo *info, int r, int g, int b, int a)
{
  int w, h; // テクスチャの幅と高さ

  w = info->Width; // テクスチャの幅と高さを取得
  h = info->Height;

  // 描画した四角形にテクスチャを貼り付け
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

//
// ====== PutImgChar関数 ======
// ------ 機能 ------
// 文字列の一文字分を対応する画像で表示
// -----------------
// ===========================
//
void PutImgChar(int x, int y, char str, int r, int g, int b, int a)
{
  PutSprite(t_str[((int)str) - 47], x, y, &t_str_info[((int)str) - 47], r, g, b, a);  // ASCIIコードに対応する画像を表示
}

//
// ====== PutImgStr関数 ======
// ------ 機能 ------
// 与えられた文字列を順番に表示
// -----------------
// ==========================
//
void PutImgStr(int x, int y, char *s, int r, int g, int b, int a)
{
  int i = 0;
  int px = 0, py = 0; // 座標調整用変数

  // はじめの一文字を表示
  PutImgChar(x + (i * 20) + px, y + py, s[0], r, g, b, a);
  px = 0;
  py = 0;
  for (i = 1; i < strlen(s); i++) { // 残りの文字を表示
    // 特定の文字の場合は座標を調整
    if (s[i - 1] >= 'A' && s[i - 1] <= 'Z') { // 直前の文字が大文字
      if (s[i - 1] == 'M' || s[i - 1] == 'W') {
        px += 8;
      }
      if (s[i - 1] == 'C') {
        px += 6;
      }
      px += 2;
    }
    if (s[i] == 'y') {
      py += 10;
    }
    if (s[i - 1] == 'u') {
      px -= 2;
    }
    if (s[i - 1] == 'i') {
      px -= 8;
    }
    if (s[i - 1] == 'r') {
      px -= 3;
    }
    if (s[i - 1] == 's') {
      px -= 3;
    }
    if (s[i] == 's') {
      px -= 3;
    }
    PutImgChar(x + (i * 20) + px, y + py, s[i], r, g, b, a);
    py = 0;
  }
}

//
// ====== SetButton関数 ======
// ------ 機能 ------
// 与えられた座標にボタンを表示
// -----------------
// ==========================
//
void setButton(int x, int y, int bw, int bh)
{
  glBegin(GL_QUADS);
  glVertex2i(x, y);
  glVertex2i(x + bw, y);
  glVertex2i(x + bw, y + bh);
  glVertex2i(x, y + bh);
  glEnd();
}

//
// ====== PassiveMotion関数 ======
// ------ 機能 ------
// マウスの動きに応じてポインタの座標を検出
// -----------------
// ==============================
//
void PassiveMotion(int x, int y)
{
  isButtonHovered(x, y, 0);
}

//
// ====== Mouse関数 ======
// ------ 機能 ------
// マウスがクリックされた場合にその座標を検出
// -----------------
// ======================
//
void Mouse(int b, int s, int x, int y)
{
  if (b == GLUT_LEFT_BUTTON) {
    if (s == GLUT_UP) {
      if (bf == 1) { // マウスのボタンが離されたらフラグクリア
        bf = 0;
      }
    }
    if (s == GLUT_DOWN) {
      isButtonClicked(x, y, 0);
    }
  }

  // 再描画
  glutPostRedisplay();
}

//
// ====== isButtonHovered関数 ======
// ------ 機能 ------
// ボタンの上にマウスポインタがあるかどうかを検出
// -----------------
// ================================
//
void isButtonHovered(int x, int y, int type)
{
  if (x >= bx && x <= bx + buttonWidth && y >= by && y <= by + buttonHeight) { // ボタンの上にマウスポインタがある場合
    hovf = 1; // フラグ発生
  }
  else {
    hovf = 0; // フラグクリア
  }
}

//
// ====== isButtonClicked関数 ======
// ------ 機能 ------
// ボタンがクリックされたかを判定
// -----------------
// ================================
//
void isButtonClicked(int x, int y, int type)
{
  if (x >= bx && x <= bx + buttonWidth && y >= by && y <= by + buttonHeight) {  // クリックされた座標がボタンの上だった場合
    ovf ^= 1; // フラグ反転
    bf = 1; // フラグ発生
  }
}
/* _______________________________________________________________________________________________________________________________________________________________ */
