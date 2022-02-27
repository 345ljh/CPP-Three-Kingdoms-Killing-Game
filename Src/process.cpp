/**
 * @file    process.cpp
 * @brief   游戏初始化与运行的总体框架
 */

#include "process.h"

//程序开始
void ProgramStart(void)
{
    setcaption("三国杀 ThreeKingdomsKilling-CPP");
    srand(time(NULL));
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
    printf("——————此界面用于记录牌局信息，请勿关闭——————\n");
    initgraph(1200, 600);
    ProgramStartGui();

    for(; is_run(); delay_fps(100))
    {
        while (mousemsg()) msg = getmouse();

        mousepos(&mouse_x, &mouse_y);
        if(msg.is_down() && mouse_x >= 0 && mouse_x <= 50 && mouse_y >= 550 && mouse_y <= 600)
        {
            ShellExecute(NULL, "open", "https://github.com/345ljh/CPP-Three-Kingdoms-Killing-Game", NULL, NULL, SW_MINIMIZE);
        }
        if(msg.is_down() && mouse_x >= 0 && mouse_x <= 50 && mouse_y >= 500 && mouse_y <= 550)
        {
            MessageBox( NULL, TEXT("敬请期待!"), TEXT("提示"), 0);
        }
        if(msg.is_down() && mouse_x >= 500 && mouse_x <= 700 && mouse_y >= 350 && mouse_y <= 400)
        {
            return;
        }
    }
}

//选将
void GeneralSelect(void)
{
    int forselect[4];
    int pointer;

    delay_fps(2);

    //确定玩家可选的4将
    for(int i = 0; i <= 3; i++)
    {
        pointer = rand() % GENERALS;
        while(general_inf[pointer].selected) ++pointer %= GENERALS;
        general_inf[pointer].selected = 2;
        forselect[i] = pointer;
    }

    //选将框
    setfillcolor(EGERGB(83, 30, 0), gui.selector);
    bar(300, 200, 900, 450, gui.selector);
    Rect(300, 200, 900, 450, EGERGB(190, 183, 68), gui.selector);

    setcolor(EGERGB(249, 189, 34), gui.selector);
    setfont(40, 0, "隶书", gui.selector);
    outtextxy(520, 200, (char*)"选择武将", gui.selector);

    //武将名与图片
    for(int i = 0; i <= 3; i++)
    {
        PasteImage(Link( (char*)".\\Textures\\Generals\\", Link( (char*)Myitoa(forselect[i]), (char*)".png")), 310 + 150 * i, 240, gui.selector);
        setfont(20, 0, "隶书", gui.selector);
        switch(general_inf[forselect[i]].nation)
        {
        case WEI:
        {
            setcolor(EGERGB(62, 117, 181), gui.selector);
            break;
        }
        case SHU:
        {
            setcolor(EGERGB(226, 54, 22), gui.selector);
            break;
        }
        case WU:
        {
            setcolor(EGERGB(124, 170, 125), gui.selector);
            break;
        }
        case QUN:
        {
            setcolor(EGERGB(141, 139, 95), gui.selector);
            break;
        }
        case SHEN:
        {
            setcolor(EGERGB(240, 248, 148), gui.selector);
            break;
        }
        }
        outtextxy(310 + 150 * i, 410, general_inf[forselect[i]].name, gui.selector);
    }

    putimage_transparent(NULL, gui.selector, 0, 0, BLACK);

    //响应玩家选将
    for(; is_run(); delay_fps(100))
    {
        while (mousemsg()) msg = getmouse();
        mousepos(&mouse_x, &mouse_y);

        if(msg.is_down() && mouse_x >= 310 && mouse_x <= 440 && mouse_y >= 240 && mouse_y <= 410) player[game.humanid].general = forselect[0];
        else if(msg.is_down() && mouse_x >= 460 && mouse_x <= 590 && mouse_y >= 240 && mouse_y <= 410) player[game.humanid].general = forselect[1];
        else if(msg.is_down() && mouse_x >= 610 && mouse_x <= 740 && mouse_y >= 240 && mouse_y <= 410) player[game.humanid].general = forselect[2];
        else if(msg.is_down() && mouse_x >= 760 && mouse_x <= 890 && mouse_y >= 240 && mouse_y <= 410) player[game.humanid].general = forselect[3];
        else continue;
        break;
    }
    for(int i = 0; i <= 3; i++) general_inf[forselect[i]].selected = (forselect[i] == player[game.humanid].general ? 1 : 0);

    //电脑选将
    for(int i = 1; i <= 3; i++)
    {
        pointer = rand() % GENERALS;
        while(general_inf[pointer].selected) ++pointer %= GENERALS;
        general_inf[pointer].selected = 1;
        player[(game.humanid + i) % 4].general = pointer;
    }
    //绘制已选武将
    for(int i = 0; i <= 3; i++)
    {
        PasteImage(Link( (char*)".\\Textures\\Generals\\",  \
                         Link( (char*)Myitoa(player[(game.humanid + i) % 4].general), (char*)".png")), pos[2 * i], pos[2 * i + 1], gui.general);
    }

    //绘制武将名
    char str[31];
    for(int i = 0; i <= 3; i++)
    {
        switch(general_inf[player[(game.humanid + i) % 4].general].nation)
        {
        case WEI:
        {
            setfillcolor(EGERGB(62, 117, 181), gui.general);
            strcpy(str, ".\\Textures\\Nations\\wei.png");
            break;
        }
        case SHU:
        {
            setfillcolor(EGERGB(226, 54, 22), gui.general);
            strcpy(str, ".\\Textures\\Nations\\shu.png");
            break;
        }
        case WU:
        {
            setfillcolor(EGERGB(124, 170, 125), gui.general);
            strcpy(str, ".\\Textures\\Nations\\wu.png");
            break;
        }
        case QUN:
        {
            setfillcolor(EGERGB(141, 139, 95), gui.general);
            strcpy(str, ".\\Textures\\Nations\\qun.png");
            break;
        }
        case SHEN:
        {
            setfillcolor(EGERGB(204, 193, 0), gui.general);
            strcpy(str, ".\\Textures\\Nations\\shen.png");
            break;
        }
        }
        bar(pos[2 * i], pos[2 * i + 1] - 20, pos[2 * i] + 130, pos[2 * i + 1], gui.general);
        PasteImage((char*)str, pos[2 * i], pos[2 * i + 1] - 20, gui.general, TRANSPARENT, BLACK);
        setcolor( general_inf[player[(game.humanid + i) % 4].general].nation == SHEN ? BLACK : WHITE, gui.general);
        setfont(20, 0, "隶书", gui.general);
        setbkmode(TRANSPARENT, gui.general);
        outtextxy(pos[2 * i] + 30, pos[2 * i + 1] - 20, general_inf[player[(game.humanid + i) % 4].general].name, gui.general);
    }
}

void GameStart(void)
{
    delay_fps(4);

    //初始化
    for(int i = 0; i <= 3; i++)
    {
        player[i].id = i;

        player[i].maxhealth = general_inf[player[i].general].maxhealth;
        player[i].health = player[i].maxhealth;
        player[i].maxcard = player[i].health;
        player[i].cardamount = 0;

        player[i].takecard = 2;
        player[i].nowslash = 0;
        player[i].maxslash = 1;
        player[i].slashlimit = 1;
        player[i].spirits = 0;
        player[i].targets = 1;

        player[i].chained = 0;
        player[i].turned = 0;

        player[i].activeskill = strcmp(general_inf[player[i].general].activeskill, "");
        player[i].limit = 0;
        player[i].awaken = 0;

        memset(player[i].card, 0xFF, sizeof(player[i].card));
        memset(player[i].equips, 0xFF, sizeof(player[i].equips));
        memset(player[i].judges, 0xFF, sizeof(player[i].judges));

        memset(player[i].temp, 0, sizeof(player[i].temp));
        memset(player[i].other, 0, sizeof(player[i].other));

        player[i].skillcard = (int*)calloc(1, sizeof(int));
        player[i].skillcard[0] = -1;
    }

    for(int i = 0; i <= 3; i++)
    {
        Drawcard(&player[i], 4);
    }

    game.turn = 1;
    game.active = 0;
    game.period = 0;
    game.page = 0;

    DrawGui();

}

void GameRun(void)
{
    //从牌堆获得特定id的牌,用于测试
    {
        int get = 1;
        for(int i = 0; i <= 159; i++) if(game.card[i] == get)
            {
                player[game.humanid].card[player[game.humanid].cardamount] = get;
                game.card[i] = -1;
                card_inf[get].owner = game.humanid;
                player[game.humanid].cardamount++;
            }
    }
    {
        int get = 76;
        for(int i = 0; i <= 159; i++) if(game.card[i] == get)
            {
                player[game.humanid].card[player[game.humanid].cardamount] = get;
                game.card[i] = -1;
                card_inf[get].owner = game.humanid;
                player[game.humanid].cardamount++;
            }
    }

    ///Remember to DrawGui after EVERY change!!!
    while(1)
    {
        if(game.active == 0)
        {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);
            printf("\n——————第%d轮——————\n\n", game.turn);
        }

        if(player[game.active].controller != DEAD)
        {
            if(player[game.active].turned == 0)
            {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
                printf("——————%s的回合开始——————\n", general_inf[player[game.active].general].name);
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);

                //准备阶段
                delay_fps(8);
                game.period = 0;
                DrawGui();
                /* skill here */

                //判定阶段
                delay_fps(4);
                game.period = 1;
                DrawGui();

                int skip = 0; //1位=1为[乐]判定成功,0位=1为[兵]判定成功

                for(int i = 2; i >= 0; i--) //从后向前判定
                {
                    if(player[game.active].judges[i][1] != -1)
                    {
                        switch ((type_e)player[game.active].judges[i][1])
                        {
                        case LE:
                        {
                            if(!AskWuxie(game.active, game.active) && card_inf[Judging(&player[game.active])].suit != HEART)
                            {
                                skip |= 2;
                                printf("%s的乐不思蜀生效\n", general_inf[player[game.active].general].name);
                            }
                            else printf("%s的乐不思蜀失效", general_inf[player[game.active].general].name);
                            break;
                        }
                        case BING:
                        {
                            if(!AskWuxie(game.active, game.active) && card_inf[Judging(&player[game.active])].suit != CLUB)
                            {
                                skip |= 1;
                                printf("%s的兵粮寸断生效\n", general_inf[player[game.active].general].name);
                            }
                            else printf("%s的兵粮寸断失效\n", general_inf[player[game.active].general].name);
                            break;
                        }
                        case SHANDIAN:
                        {
                            if(!AskWuxie(game.active, game.active))
                            {
                                int res = Judging(&player[game.active]);
                                if(card_inf[res].suit == SPADE && card_inf[res].num >= 2 && card_inf[res].num <= 9)
                                {
                                    Damage(NULL, &player[game.active], 3, THUNDER);
                                    printf("%s的闪电生效\n", general_inf[player[game.active].general].name);
                                    break;
                                }
                                else printf("%s的闪电失效\n", general_inf[player[game.active].general].name);
                            }

                            //选择合适的下家,若无合适下家(如场上仅有2人且都有闪电)则不转移
                            int next = 1;
                            while(next < 4 &&
                                  (player[(game.active + next) % 4].controller == DEAD ||
                                   (type_e)player[(game.active + next) % 4].judges[0][1] == SHANDIAN ||
                                   (type_e)player[(game.active + next) % 4].judges[1][1] == SHANDIAN ||
                                   (type_e)player[(game.active + next) % 4].judges[2][1] == SHANDIAN) )
                                next++;

                            //转移闪电
                            int judgearea[3];
                            for(int i = 0; i <= 2; i++) judgearea[i] = player[(game.active + next) % 4].judges[i][0];
                            player[(game.active + next) % 4].judges[ArrayOccupied(judgearea, 3)][0] = player[game.active].judges[i][0];
                            player[(game.active + next) % 4].judges[ArrayOccupied(judgearea, 3)][1] = SHANDIAN;
                            break;
                        }
                        default:
                            ;
                        }
                    }

                    card_inf[player[game.active].judges[i][0]].owner = -1;
                    player[game.active].judges[i][0] = -1;
                    player[game.active].judges[i][1] = -1;

                    DrawGui();

                }

                //摸牌阶段
                delay_fps(4);
                game.period = 2;
                if(!(skip & 1) )
                {
                    Drawcard(&player[game.active], player[game.active].takecard + (game.turn == 1 ? (game.active == 3) - !game.active : 0));
                    DrawGui();
                }

                //出牌阶段
                delay_fps(4);
                game.period = 3;
                if(!(skip & 2))
                {
                    Playcard(&player[game.active]);
                }

                //弃牌阶段
                delay_fps(4);
                game.period = 4;
                DrawGui();
                if(player[game.active].cardamount > player[game.active].maxcard)
                {
                    Throwcard(&player[game.active], &player[game.active], player[game.active].cardamount - player[game.active].maxcard);
                    DrawGui();
                }
                //结束阶段
                delay_fps(4);
                game.period = 5;
                DrawGui();
                /* skill here */
            }
            else
            {
                player[game.active].turned = 0;
                printf("%s从背面翻至正面\n", general_inf[player[game.active].general].name);
            }
        }

        //重置杀和酒的次数
        for(int i = 0; i <= 3; i++)
        {
            player[i].spirits = 0;
            player[i].nowslash = 0;
        }

        //轮数与当前回合角色更新
        (++game.active %= 4) ? : game.turn++;
    }
}
