#ifndef DRAW_H_INCLUDED
#define DRAW_H_INCLUDED

#include "graphics.h"

#define FRAMELINE EGERGB(255, 255, 145)

typedef struct
{
    PIMAGE background;  //背景图层
    PIMAGE frame;  //各类信息与边界、按钮图层
    PIMAGE playercard;  //各区域卡牌图层
    PIMAGE selector;  //选牌与选定目标图层
    PIMAGE tips;  //提示文字图层
}gui_t;

void Rect(int left, int top, int right, int bottom, color_t color, PIMAGE img);
void Tri(int x1, int y1, int x2, int y2, int x3, int y3, color_t color, PIMAGE img);

void PasteImage(char* path, int x, int y, PIMAGE img, int mode, color_t color);
void ProgramStart(void);
void GameGuiInit(void);

#endif // DRAW_H_INCLUDED
