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
        LineRect(960, 510, 1050, 535, EGERGB(255, 215, 77), gui.selector);
        LineRect(960, 540, 1050, 565, EGERGB(255, 215, 77), gui.selector);
        putimage_transparent(NULL, gui.selector, 0, 0, BLACK);

        for(; is_run(); delay_fps(10))
        {
            while (mousemsg()) msg = getmouse();
            mousepos(&mouse_x, &mouse_y);

            if(msg.is_down() && mouse_x >= 960 && mouse_x <= 1050 && mouse_y >= 510 && mouse_y <= 535)
            {
               break;
            }

            if(msg.is_down() && mouse_x >= 960 && mouse_x <= 1050 && mouse_y >= 540 && mouse_y <= 565)
            {
                goto BaguaExit;
            }

        }

    }

    if(0) BaguaExit: return 0;

    printf("%s发动了\"八卦阵\"\n", general_inf[recipient->general].name);

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

//雌雄双股剑
void Cixiong(player_t* executor, player_t* recipient)
{
    cleardevice(gui.selector);
    DrawGui();

    if(executor->controller == HUMAN)
    {
        char str[121] = "";
        strcpy(str, (char*)"是否使用 雌雄双股剑");
        setcolor(WHITE, gui.selector);
        setfont(30, 0, "仿宋", gui.selector);
        outtextxy(600 - 7.5 * strlen(str), 415, str, gui.selector);
        LineRect(960, 510, 1050, 535, EGERGB(255, 215, 77), gui.selector);
        LineRect(960, 540, 1050, 565, EGERGB(255, 215, 77), gui.selector);
        putimage_transparent(NULL, gui.selector, 0, 0, BLACK);

        for(; is_run(); delay_fps(10))
        {
            while (mousemsg()) msg = getmouse();
            mousepos(&mouse_x, &mouse_y);

            LineRect(960, 510, 1050, 535, EGERGB(255, 215, 77), gui.selector);
            LineRect(960, 540, 1050, 565, EGERGB(255, 215, 77), gui.selector);

            if(msg.is_down() && mouse_x >= 960 && mouse_x <= 1050 && mouse_y >= 510 && mouse_y <= 535)
            {
                break;
            }

            if(msg.is_down() && mouse_x >= 960 && mouse_x <= 1050 && mouse_y >= 540 && mouse_y <= 565)
            {
                goto CixiongExit;
            }

        }
    }

    if(0) CixiongExit: return;
    printf("%s发动了\"雌雄双股剑\"\n", general_inf[executor->general].name);

    int ans = 0;
    if(recipient->controller == HUMAN)
    {
        char str[121] = "";
        strcpy(str, Link( Link( (char*)"点击确定弃1张手牌，或取消", general_inf[executor->general].name), (char*)"摸1张牌") );
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
                ans = Throwcard(recipient, recipient, 1);
                break;
            }

            if(msg.is_down() && mouse_x >= 960 && mouse_x <= 1050 && mouse_y >= 540 && mouse_y <= 565)
            {
                break;
            }

        }
    }
    else
    {
        //TODO:AI
        ;
    }
    if(!ans) Drawcard(executor, 1);
}


//寒冰剑
int Hanbing(player_t* executor, player_t* recipient)
{
    cleardevice(gui.selector);
    DrawGui();

    if(executor->controller == HUMAN)
    {
        char str[121] = "";
        strcpy(str, (char*)"是否使用 寒冰剑");
        setcolor(WHITE, gui.selector);
        setfont(30, 0, "仿宋", gui.selector);
        outtextxy(600 - 7.5 * strlen(str), 415, str, gui.selector);
        LineRect(960, 510, 1050, 535, EGERGB(255, 215, 77), gui.selector);
        LineRect(960, 540, 1050, 565, EGERGB(255, 215, 77), gui.selector);
        putimage_transparent(NULL, gui.selector, 0, 0, BLACK);

        for(; is_run(); delay_fps(10))
        {
            while (mousemsg()) msg = getmouse();
            mousepos(&mouse_x, &mouse_y);

            if(msg.is_down() && mouse_x >= 960 && mouse_x <= 1050 && mouse_y >= 510 && mouse_y <= 535)
            {
                break;
            }

            if(msg.is_down() && mouse_x >= 960 && mouse_x <= 1050 && mouse_y >= 540 && mouse_y <= 565)
            {
                goto HanbingExit;
            }

        }
    }
    else
    {
        //TODO:AI
        ;
    }

    if(0) HanbingExit: return 0;
    printf("%s发动了\"寒冰剑\"\n", general_inf[executor->general].name);
    Throwcard(executor, recipient, 2, 3);
    return 1;

}

//青龙偃月刀
void Qinglong(player_t* executor, player_t* recipient)
{
    int ans = Askcard(executor, SHA, 0x31);
    if(ans != -1) Execard(executor, 1 << recipient->id, ans);
}

//贯石斧
int Guanshi(player_t* executor)
{
    cleardevice(gui.selector);
    DrawGui();

    if(executor->controller == HUMAN)
    {
        char str[121] = "";
        strcpy(str, (char*)"是否使用 贯石斧");
        setcolor(WHITE, gui.selector);
        setfont(30, 0, "仿宋", gui.selector);
        outtextxy(600 - 7.5 * strlen(str), 415, str, gui.selector);
        LineRect(960, 510, 1050, 535, EGERGB(255, 215, 77), gui.selector);
        LineRect(960, 540, 1050, 565, EGERGB(255, 215, 77), gui.selector);
        putimage_transparent(NULL, gui.selector, 0, 0, BLACK);

        for(; is_run(); delay_fps(10))
        {
            while (mousemsg()) msg = getmouse();
            mousepos(&mouse_x, &mouse_y);

            if(msg.is_down() && mouse_x >= 960 && mouse_x <= 1050 && mouse_y >= 510 && mouse_y <= 535)
            {
                break;
            }

            if(msg.is_down() && mouse_x >= 960 && mouse_x <= 1050 && mouse_y >= 540 && mouse_y <= 565)
            {
                goto GuanshiExit;
            }

        }
    }
    else
    {
        //TODO:AI
        ;
    }

    if(0) GuanshiExit: return 0;

    if(Throwcard(executor, executor, 2, 3, 255, 0x30) == 2)
    {
        printf("%s发动了\"贯石斧\"\n", general_inf[executor->general].name);
        return 1;
    }
    return 0;
}

//探囊:你可以选择一名角色并声明一张牌的id,若此牌在牌堆中,其获得之
///开发者技能,在process.cpp的GameRun里面添加
void Tannang(player_t* recipient, int id)
{
    printf("%s发动\"探囊\",声明卡牌", general_inf[player[game.humanid].general].name);
    Printcard(id);
    printf("\n");
    for(int i = 0; i <= 159; i++)
    {
        if(game.card[i] == id)
        {
            recipient->card[recipient->cardamount] = id;
            game.card[i] = -1;
            card_inf[id].owner = recipient->id;
            recipient->cardamount++;

            printf("%s获得", general_inf[recipient->general].name);
            Printcard(id);
            printf("\n");
        }
    }
}
