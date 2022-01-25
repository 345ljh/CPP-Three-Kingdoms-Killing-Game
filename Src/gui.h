#ifndef DRAW_H_INCLUDED
#define DRAW_H_INCLUDED

#include "graphics.h"

typedef struct
{
    PIMAGE background;  //背景图层
    PIMAGE playercard;  //各区域卡牌图层
    PIMAGE bottom;  //按键图层
    PIMAGE selector;  //选牌与选定目标图层
    PIMAGE tips;  //提示文字图层
}gui_t;

void ProgramStart(void);
void GameGuiInit(void);

#endif // DRAW_H_INCLUDED
