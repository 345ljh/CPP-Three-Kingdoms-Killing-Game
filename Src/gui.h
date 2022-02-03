#ifndef DRAW_H_INCLUDED
#define DRAW_H_INCLUDED

#include "graphics.h"
#include "items.h"
#include "app.h"

#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <windows.h>

typedef struct
{
    PIMAGE background;  //背景图层
    PIMAGE frame;  //各类信息与边界、按钮图层
    PIMAGE general;  //武将与势力图层
    PIMAGE playercard;  //各区域卡牌图层
    PIMAGE selector;  //选牌与选定目标图层
    PIMAGE tips;  //提示文字图层
}gui_t;

void ProgramStart(void);
void GameGuiInit(void);
void GeneralSelect(void);
void DrawGui(void);

extern gui_t gui;
#endif // DRAW_H_INCLUDED
