#ifndef APP_H_INCLUDED
#define APP_H_INCLUDED

#include "items.h"
#include "gui.h"

#include <graphics.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>

char* Myitoa(int num);
char* MyitoaII(int num);
char* Link(char* str1, char* str2);

void Rect(int left, int top, int right, int bottom, color_t color, PIMAGE img);
void Tri(int x1, int y1, int x2, int y2, int x3, int y3, color_t color, PIMAGE img);
void LineRect(int left, int top, int right, int bottom, color_t color, PIMAGE img = NULL, int wide = 3);

void PasteImage(char* path, int x, int y, PIMAGE img = NULL, int mode = 0, color_t color = BLACK);
void PasteCard(int x, int y, int id, PIMAGE img);
void Putcard(int id);

int ArrayOccupied(int* buf, int len);
void IndexAlign(player_t* player);

#endif // APP_H_INCLUDED
