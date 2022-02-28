/**
 * @file    skill.cpp
 * @brief   装备效果与技能
 */

#include "skill.h"

//八卦阵
int Bagua(player_t *recipient)
{
    cleardevice(gui.selector);
    DrawGui();

    if(recipient->controller == HUMAN)
    {
        char str[121] = "";
        strcpy(str, (char*)"是否使用 八卦阵");
        setcolor(WHITE, gui.selector);
        setfont(30, 0, "仿宋", gui.selector);
        outtextxy(600 - 7.5 * strlen(str), 415, str, gui.selector);

        for(; is_run(); delay_fps(10))
        {
            while (mousemsg()) msg = getmouse();
            mousepos(&mouse_x, &mouse_y);

            LineRect(960, 510, 1050, 535, EGERGB(255, 215, 77), gui.selector);
            LineRect(960, 540, 1050, 565, EGERGB(255, 215, 77), gui.selector);

            if(msg.is_down() && mouse_x >= 960 && mouse_x <= 1050 && mouse_y >= 510 && mouse_y <= 535)
            {
                goto BaguaExe;
            }

            if(msg.is_down() && mouse_x >= 960 && mouse_x <= 1050 && mouse_y >= 540 && mouse_y <= 565)
            {
                return 0;
            }

        }

    }

    BaguaExe: printf("%s发动了\"八卦阵\"\n", general_inf[recipient->general].name);

    if( (int)card_inf[Judging(recipient)].suit >> 1 == 1)
    {
        printf("八卦阵生效\n");
        return 1;
    }
    else
    {
        printf("八卦阵失效\n");
        return 0;
    }
}
