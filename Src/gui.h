#ifndef DRAW_H_INCLUDED
#define DRAW_H_INCLUDED

#include "graphics.h"
#include "items.h"
#include "app.h"

#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <windows.h>
#include <time.h>

typedef struct
{
    PIMAGE background;  //背景图层
    PIMAGE frame;  //各类信息与边界、按钮图层
    PIMAGE general;  //武将与势力图层
    PIMAGE playercard;  //各区域卡牌图层
    PIMAGE selector;  //选牌与选定目标图层
    PIMAGE tips;  //提示文字图层
    PIMAGE throwcard;  //弃牌堆绘制图层
    PIMAGE arrow;  //指向目标图层
}gui_t;

void ProgramStart(void);
void GameGuiInit(void);
void GeneralSelect(void);
void DrawGui(void);

extern gui_t gui;
extern mouse_msg msg;
extern int mouse_x, mouse_y;
extern const int pos[8];

#endif // DRAW_H_INCLUDED
