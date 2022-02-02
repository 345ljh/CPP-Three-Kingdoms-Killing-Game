#ifndef DRAW_H_INCLUDED
#define DRAW_H_INCLUDED

#include "graphics.h"
#include "items.h"

#include <string.h>
#include <stdlib.h>
#include <malloc.h>

typedef struct
{
    PIMAGE background;  //背景图层
    PIMAGE frame;  //各类信息与边界、按钮图层
    PIMAGE general;  //武将与势力图层
    PIMAGE playercard;  //各区域卡牌图层
    PIMAGE selector;  //选牌与选定目标图层
    PIMAGE tips;  //提示文字图层
}gui_t;

char* Myitoa(int num);
char* MyitoaII(int num);
char* Link(char* str1, char* str2);
void Rect(int left, int top, int right, int bottom, color_t color, PIMAGE img);
void Tri(int x1, int y1, int x2, int y2, int x3, int y3, color_t color, PIMAGE img);

void PasteImage(char* path, int x, int y, PIMAGE img, int mode, color_t color);
void ProgramStart(void);
void GameGuiInit(void);
void GeneralSelect(void);
void DrawGui(void);

extern gui_t gui;
#endif // DRAW_H_INCLUDED
