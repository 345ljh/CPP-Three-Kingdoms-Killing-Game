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

            if(msg.is_down() && mouse_x >= 960 && mouse_x <= 1050 && mouse_y >= 510 && mouse_y <= 535) break;
            if(msg.is_down() && mouse_x >= 960 && mouse_x <= 1050 && mouse_y >= 540 && mouse_y <= 565) goto BaguaExit;
        }

    }

    if(0)
    {
        BaguaExit: delay_fps(2);
        return 0;
    }

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

            if(msg.is_down() && mouse_x >= 960 && mouse_x <= 1050 && mouse_y >= 510 && mouse_y <= 535) break;
            if(msg.is_down() && mouse_x >= 960 && mouse_x <= 1050 && mouse_y >= 540 && mouse_y <= 565) return;

        }
    }

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
            if(msg.is_down() && mouse_x >= 960 && mouse_x <= 1050 && mouse_y >= 540 && mouse_y <= 565) break;
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

            if(msg.is_down() && mouse_x >= 960 && mouse_x <= 1050 && mouse_y >= 510 && mouse_y <= 535) break;
            if(msg.is_down() && mouse_x >= 960 && mouse_x <= 1050 && mouse_y >= 540 && mouse_y <= 565) return 0;

        }
    }
    else
    {
        //TODO:AI
        ;
    }

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

            if(msg.is_down() && mouse_x >= 960 && mouse_x <= 1050 && mouse_y >= 510 && mouse_y <= 535) break;
            if(msg.is_down() && mouse_x >= 960 && mouse_x <= 1050 && mouse_y >= 540 && mouse_y <= 565) return 0;
        }
    }
    else
    {
        //TODO:AI
        ;
    }

    if(Throwcard(executor, executor, 2, 3, 255, 0x30) == 2)
    {
        printf("%s发动了\"贯石斧\"\n", general_inf[executor->general].name);
        return 1;
    }
    return 0;
}

//丈八蛇矛-使用杀
void Zhangba(player_t* executor)
{
    //由于使用的2张牌在之后还可能参与结算,所以此处不直接使用Throwcard
    if(executor->controller == HUMAN)
    {
        int tothrow[2] = {-1, -1};

        for(; is_run(); delay_fps(10))
        {
            while (mousemsg()) msg = getmouse();
            mousepos(&mouse_x, &mouse_y);

            DrawGui();
            cleardevice(gui.selector);

            char str[121] = "";
            strcpy(str, (char*)"将2张牌当做杀使用");
            setcolor(WHITE, gui.selector);
            setfont(30, 0, "仿宋", gui.selector);
            outtextxy(600 - 7.5 * strlen(str), 415, str, gui.selector);
            LineRect(5, 453.75, 145, 483.75, EGERGB(255, 57, 57), gui.selector);

            //全部绘制为未选定状态
            for(int i = 0; i <= 7; i++)
            {
                if(executor->card[game.page * 8 + i] != -1)
                    LineRect(160 + 100 * i, 465, 240 + 100 * i, 585, EGERGB(255, 215, 77), gui.selector);
            }

            //绘制已选定状态
            for(int i = 0; i <= 1; i++)
            {
                if(tothrow[i] != -1 && executor->card[tothrow[i]] != -1 && tothrow[i] >> 8 == 0 && game.page == tothrow[i] / 8)
                    LineRect(160 + 100 * (tothrow[i] % 8), 465, 240 + 100 * (tothrow[i] % 8), 585, EGERGB(255, 57, 57), gui.selector);
            }

            //确定键
            if(ArrayOccupied(tothrow, 2) == 2) LineRect(960, 510, 1050, 535, EGERGB(255, 215, 77), gui.selector);

            //取消键
            LineRect(960, 540, 1050, 565, EGERGB(255, 215, 77), gui.selector);

            putimage_transparent(NULL, gui.selector, 0, 0, BLACK);

            //检测按键
            if(msg.is_down() && mouse_x >= 150 && mouse_x <= 950 && mouse_y >= 450 && mouse_y <= 600)
            {
                int sel = (mouse_x - 150) / 100;
                if(executor->cardamount > game.page * 8 + sel)
                {
                    int found = 0;
                    for(int i = 0; i <= 1; i++)
                    {
                        //若已选中则取消
                        if(tothrow[i] == game.page * 8 + sel)
                        {
                            tothrow[i] = -1;
                            found++;
                            break;
                        }
                    }

                    //若未选中则选定,将tothrow中目前下标最小的-1改为该牌id
                    if(!found && ArrayOccupied(tothrow, 2) <= 2)
                    {
                        for(int i = 0; i<= 1; i++)
                        {
                            if(tothrow[i] == -1)
                            {
                                tothrow[i] = game.page * 8 + sel;
                                break;
                            }
                        }
                    }
                }
            }

            //翻页
            if(msg.is_down() && mouse_x >= 960 && mouse_x <= 970 && mouse_y >= 575 && mouse_y <= 595)
            {
                if(game.page > 0) game.page--;
            }
            if(msg.is_down() && mouse_x >= 985 && mouse_x <= 1000 && mouse_y >= 575 && mouse_y <= 595)
            {
                if(executor->cardamount > (game.page + 1) * 8) game.page++;
            }

            //取消键
            if(msg.is_down() && mouse_x >= 960 && mouse_x <= 1050 && mouse_y >= 540 && mouse_y <= 565)
            {
                longjmp(Circ,1);
            }

            //确定键
            if(ArrayOccupied(tothrow, 2) == 2 && msg.is_down() && mouse_x >= 960 && mouse_x <= 1050 && mouse_y >= 510 && mouse_y <= 535)
            {
                //计算合法目标
                int allowtar = 15 ^ (1 << executor->id);  //除去自己
                for(int i = 1; i <= 3; i++)
                {
                    //对位且两侧有间隔时基础距离为2,否则为1
                    int distance = (i == 2 && player[(executor->id + 1) % 4].controller != DEAD && player[(executor->id + 3) % 4].controller != DEAD) ? 2 : 1;
                    //+1与-1马的计算
                    distance += player[(executor->id + i) % 4].equips[3] != -1;
                    distance -= executor->equips[2] != -1;
                    //比较攻击范围
                    int range = executor->equips[0] != -1 ? (int)card_inf[executor->equips[0]].type >> 4 : 1;
                    if(distance > range) allowtar &= (15 ^ (1 << ((executor->id + i) % 4)));
                }

                int tar = SelectTarget(allowtar, executor->targets, 0x32);
                if(tar)
                {
                    //输出信息与使用牌
                    printf("%s使用丈八蛇矛将", general_inf[executor->general].name);
                    for(int i = 0; i <= 1; i++)
                    {
                        card_inf[executor->card[tothrow[i]]].owner = -1;
                        Putcard(executor->card[tothrow[i]]);
                        Printcard(executor->card[tothrow[i]]);
                        executor->card[tothrow[i]] = -1;
                        executor->cardamount--;
                    }
                    IndexAlign(executor->card, executor->cardamount, 160);
                    printf("当做杀使用,目标是");

                    int printed = 0;
                    for(int i = 1; i <= 3; i++)
                        if(tar & (1 << (executor->id + i) % 4))
                        {
                            if(printed++) printf(",");
                            printf("%s", general_inf[player[(executor->id + i) % 4].general].name);
                        }
                    printf("\n");

                    //执行杀的效果
                    int basdamage = 1;
                    if(executor->spirits == 1)
                    {
                        basdamage++;
                        executor->spirits = 2;
                    }
                    if(game.active == executor->id) executor->nowslash++;

                    for(int i = 1; i <= 3; i++)
                    {
                        if(tar & (1 << (executor->id + i) % 4) )
                        {
                            //仁王盾效果
                            if( (type_e)player[(executor->id + i) % 4].equips[1] == RENWANG && (int)card_inf[executor->card[tothrow[0]]].suit >> 1 == 0 && (int)card_inf[executor->card[tothrow[1]]].suit >> 1 == 0)
                            {
                                printf("%s发动了\"仁王盾\"\n", general_inf[player[(executor->id + i) % 4].general].name);
                                continue;
                            }
                            //藤甲效果
                            if( (type_e)player[(executor->id + i) % 4].equips[1] == TENGJIA)
                            {
                                printf("%s发动了\"藤甲\"\n", general_inf[player[(executor->id + i) % 4].general].name);
                                continue;
                            }
                            //询问闪
                            if(Askcard(&player[(executor->id + i) % 4], SHAN, 0))  continue;
                            //造成伤害
                            Damage(executor, &player[(executor->id + i) % 4], basdamage, COMMON, 1);
                        }
                    }
                    cleardevice(gui.selector);
                    DrawGui();
                    return;
                }
                else longjmp(Circ,1);  //直接跳转到Circ语句
            }
            putimage_transparent(NULL, gui.selector, 0, 0, BLACK);

        }
    }
    else
    {
        ;
    }
    return;
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
