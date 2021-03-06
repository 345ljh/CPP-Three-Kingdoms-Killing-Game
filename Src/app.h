#ifndef APP_H_INCLUDED
#define APP_H_INCLUDED

#include "items.h"
#include "gui.h"

#include <graphics.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>

#define DELAY_TIME 5 //LineAppear中绘制每段直线的时间间隔

int Fact(int n);
double Binomial(int n, double p, int m);
int TypeIdentify(type_e type);
char* Myitoa(int num);
char* MyitoaII(int num);
char* Link(char* str1, char* str2);

void Rect(int left, int top, int right, int bottom, color_t color, PIMAGE img = NULL);
void Tri(int x1, int y1, int x2, int y2, int x3, int y3, color_t color, PIMAGE img = NULL);
void LineRect(int left, int top, int right, int bottom, color_t color, PIMAGE img = NULL, int wide = 3);
void LineAppear(int startx, int starty, int endx, int endy, int appear_time, int disappear_time, color_t color, PIMAGE img = gui.arrow);
void LineAppear(int startx, int starty, int* endx, int* endy, int line_amount, int appear_time, int disappear_time, color_t color, PIMAGE img = gui.arrow);

void PasteImage(char* path, int x, int y, PIMAGE img = NULL, int mode = 0, color_t color = BLACK);
void Pastecard(int x, int y, int id, PIMAGE img);
void Putcard(int id, PIMAGE img = gui.throwcard);
void Printcard(int id);
void ShowTarget(int executor_id, int target);

int ArrayOccupied(int* buf, int len);
void IndexAlign(int* buf, int len, int blen);

#endif // APP_H_INCLUDED
