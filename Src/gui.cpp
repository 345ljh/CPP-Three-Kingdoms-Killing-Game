#include "gui.h"

mouse_msg msg;
int mouse_x, mouse_y;//用于获取鼠标坐标
gui_t gui = {newimage(), newimage(), newimage(), newimage(), newimage(), newimage(), newimage(), newimage()};

const int pos[8] = {1070, 430, 1040, 150, 520, 20, 30, 150};  //从玩家逆时针,武将位置

//开始界面
void ProgramStart(void)
{
    initgraph(1200, 600);

    PasteImage((char*)".\\Textures\\origin.png", 0, 0);

    setcolor(EGERGB(236, 95, 0));
    setbkmode(TRANSPARENT);
    setfont(120, 0, "隶书");
    outtextxy(420, 100, "三国杀");

    setfillcolor(EGERGB(136, 0, 21));
    bar(500, 350, 700, 400);

    setcolor(EGERGB(234, 234, 255));
    setbkmode(TRANSPARENT);
    setfont(40, 0, "隶书");
    outtextxy(520, 355, "开始游戏");

    //其他网页链接
    PasteImage((char*)".\\Textures\\bili.png", 0, 500, NULL, TRANSPARENT, BLACK);
    PasteImage((char*)".\\Textures\\github.png", 0, 550, NULL, TRANSPARENT, BLACK);

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

//开始游戏时界面绘制
void GameGuiInit(void)
{
    setbkcolor(BLACK);
    cleardevice();
    //图层初始化
    getimage(gui.background, 0, 0, 1200, 600);
    getimage(gui.frame, 0, 0, 1200, 600);
    getimage(gui.general, 0, 0, 1200, 600);
    getimage(gui.playercard, 0, 0, 1200, 600);
    getimage(gui.selector, 0, 0, 1200, 600);
    getimage(gui.tips, 0, 0, 1200, 600);
    getimage(gui.throwcard, 0, 0, 1200, 600);
    getimage(gui.arrow, 0, 0, 1200, 600);

    setbkmode(TRANSPARENT, gui.background);
    setbkmode(TRANSPARENT, gui.frame);
    setbkmode(TRANSPARENT, gui.general);
    setbkmode(TRANSPARENT, gui.playercard);
    setbkmode(TRANSPARENT, gui.selector);
    setbkmode(TRANSPARENT, gui.tips);


    PasteImage((char*)".\\Textures\\background.png", 0, 0, gui.background);

    //玩家区域
    PIMAGE cardbar = newimage();
    getimage(cardbar, 0, 0, 1200, 600);
    setfillcolor(EGERGB(71, 98, 32), cardbar);
    bar(0, 450, 1070, 600, cardbar);
    putimage_alphatransparent(gui.background, cardbar, 0, 0, BLACK, 150);
    delimage(cardbar);

    //绘制边界
    setcolor(EGERGB(190, 183, 68), gui.frame);
    line(150, 450, 150, 600, gui.frame);
    for(int i = 1; i <= 3; i++)  line(0, 450 + 37.5 * i, 150, 450 + 37.5 * i, gui.background);
    for(int i = 0; i <= 8; i++)  line(150 + 100 * i, 450, 150 + 100 * i, 600, gui.background);

    //绘制按钮
    Rect(960, 510, 1050, 535, EGERGB(190, 183, 68), gui.background);
    Rect(960, 540, 1050, 565, EGERGB(190, 183, 68), gui.background);
    setfillcolor(RED, gui.background);
    floodfill(981, 511, EGERGB(190, 183, 68), gui.background);
    floodfill(981, 541, EGERGB(190, 183, 68), gui.background);
    setbkmode(TRANSPARENT, gui.background);
    setfont(25, 0, "隶书", gui.background);
    setcolor(EGERGB(190, 183, 68), gui.background);
    outtextxy(980, 510, "确定", gui.background);
    outtextxy(980, 540, "取消", gui.background);

    Tri(960, 585, 975, 575, 975, 595, EGERGB(44, 182, 14), gui.background);
    Tri(1000, 585, 985, 575, 985, 595, EGERGB(44, 182, 14), gui.background);
    setfillcolor(EGERGB(181, 230, 29), gui.background);
    floodfill(970, 585, EGERGB(44, 182, 14), gui.background);
    floodfill(990, 585, EGERGB(44, 182, 14), gui.background);

    //绘制牌局信息
    setcolor(EGERGB(255, 127, 39), gui.background);
    setfont(40, 0, "隶书", gui.background);
    outtextxy(1000, 20, "第     轮", gui.background);
    setcolor(WHITE, gui.background);
    setfont(20, 0, "隶书", gui.background);
    outtextxy(1000, 70, "牌堆剩余:", gui.background);

    //绘制其他角色
    for(int i = 0; i <= 3; i++)
    {
        Rect(pos[2 * i], pos[2 * i + 1], pos[2 * i] + 130 ,pos[2 * i + 1] + 170, EGERGB(190, 183, 68), gui.background);
    }


    //信息显示图标
    PasteImage((char*)".\\Textures\\condition.png", 950, 450, gui.background, TRANSPARENT, BLACK);
    PasteImage((char*)".\\Textures\\condition.png", 30, 340, gui.background, TRANSPARENT, BLACK);
    PasteImage((char*)".\\Textures\\condition.png", 1040, 340, gui.background, TRANSPARENT, BLACK);
    PasteImage((char*)".\\Textures\\condition.png", 430, 130, gui.background, TRANSPARENT, BLACK);

    //确定位置
    for(int i = 0; i <= PLAYERS - 1; i++)
    {
        player[i].controller = AI;
    }

    game.humanid = rand() % PLAYERS;
    player[game.humanid].controller = HUMAN;

    char str[] = "一\0二\0三\0四\0";
    setfillcolor(EGERGB(238, 89, 0), gui.background);
    setcolor(RED, gui.frame);
    fillellipse(1050, 585, 15, 15, gui.background);
    setcolor(EGERGB(253, 228, 140), gui.background);
    setfont(20, 0, "隶书", gui.background);
    outtextxy(1040, 575, str + game.humanid * 3, gui.background);
    setcolor(EGERGB(190, 183, 68), gui.background);
    setfont(20, 0, "隶书", gui.background);

    for(int i = 1; i <= 3; i++)
    {
        outtextxy(pos[2 * i] + 40, pos[2 * i + 1] + 170, Link(str + ((game.humanid + i) % PLAYERS) * 3, (char*)"号位"), gui.background);
    }
    //输出图像
    putimage(0, 0, gui.background);
    return;
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
        player[(game.humanid + i) % PLAYERS].general = pointer;
    }
    //绘制已选武将
    for(int i = 0; i <= 3; i++)
    {
        PasteImage(Link( (char*)".\\Textures\\Generals\\",  \
            Link( (char*)Myitoa(player[(game.humanid + i) % PLAYERS].general), (char*)".png")), pos[2 * i], pos[2 * i + 1], gui.general);
    }

    //绘制武将名
    char str[31];
    for(int i = 0; i <= 3; i++)
    {
        switch(general_inf[player[(game.humanid + i) % PLAYERS].general].nation)
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
        setcolor( general_inf[player[(game.humanid + i) % PLAYERS].general].nation == SHEN ? BLACK : WHITE, gui.general);
        setfont(20, 0, "隶书", gui.general);
        setbkmode(TRANSPARENT, gui.general);
        outtextxy(pos[2 * i] + 30, pos[2 * i + 1] - 20, general_inf[player[(game.humanid + i) % PLAYERS].general].name, gui.general);
    }

    //重绘界面
    setbkcolor(BLACK, gui.selector);
    cleardevice(gui.selector);
    putimage_transparent(NULL, gui.general, 0, 0, BLACK);
    putimage(0, 0, gui.background);
    putimage_transparent(NULL, gui.frame, 0, 0, BLACK);
    putimage_transparent(NULL, gui.general, 0, 0, BLACK);
}

//游戏过程中重绘信息
void DrawGui(void)
{
    cleardevice(gui.frame);
    cleardevice(gui.playercard);

    while(game.page * 8 + 1 > player[game.humanid].cardamount && game.page) game.page--;

    //轮数与剩余牌量
    setcolor(EGERGB(255, 127, 39), gui.frame);
    setfont(40, 0, "Lucida Handwriting", gui.frame);
    outtextxy(1070, 20, Myitoa(game.turn), gui.frame);
    setcolor(WHITE, gui.frame);
    setfont(20, 0, "Lucida Handwriting", gui.frame);
    outtextxy(1100, 70, Myitoa(game.nowpile), gui.frame);

    setcolor(WHITE, gui.frame);
    setfont(20, 0, "Lucida Handwriting", gui.frame);
    outtextxy(1100, 70, Myitoa(game.nowpile), gui.frame);

    //手牌翻页
    setcolor(EGERGB(181, 230, 29), gui.frame);
    setfont(15, 0, "Lucida Handwriting", gui.frame);
    outtextxy(1000, 580, Link(Link(Link(Myitoa(game.page + 1), (char*)"/"), MyitoaII((player[game.humanid].cardamount- 1) / 8 + 1) ), (char*)"页"), gui.frame);

    //体力与手牌信息
    for(int i = 0; i <= 3; i++)
    {
        int infpos[2 * PLAYERS] = {950, 450, 1040, 340, 430, 130, 30, 340};
        setfont(30, 0, "Lucida Handwriting", gui.frame);
        setcolor(player[(game.humanid + i) % 4].health / (float)player[(game.humanid + i) % 4].maxhealth > 2 / 3.0 ? EGERGB(34, 177, 76) :
                 player[(game.humanid + i) % 4].health / (float)player[(game.humanid + i) % 4].maxhealth > 1 / 3.0 ? EGERGB(255, 128, 0) :
                 EGERGB(255, 0, 0), gui.frame);
        outtextxy(infpos[2 * i] + 30, infpos[2 * i + 1], Link(Link(Myitoa(player[(game.humanid + i) % 4].health), (char*)"/"),
                                                         MyitoaII(player[(game.humanid + i) % 4].maxhealth) ), gui.frame);

        setcolor(EGERGB(251, 227, 134), gui.frame);
        outtextxy(infpos[2 * i] + 30, infpos[2 * i + 1] + 30, Link(Link(Myitoa(player[(game.humanid + i) % 4].cardamount), (char*)"/"),
                                                         MyitoaII(player[(game.humanid + i) % 4].maxcard) ), gui.frame);
    }

    //翻面,横置,限定,觉醒
    {
        for(int i = 0; i <= 3; i++)
        {
            if(player[(game.humanid + i) % 4].turned) PasteImage((char*)".\\Textures\\general_back.png", pos[2 * i], pos[2 * i + 1], gui.frame,
                    TRANSPARENT, BLACK);
            if(player[(game.humanid + i) % 4].chained) PasteImage((char*)".\\Textures\\chain.png", pos[2 * i], pos[2 * i + 1] + 25, gui.frame,
                    TRANSPARENT, EGERGB(255, 255, 255));
            if(player[(game.humanid + i) % 4].limit) PasteImage((char*)".\\Textures\\limit.png",
                    i ? pos[2 * i] + 132 : 1055 , i ? pos[2 * i + 1] : 420, gui.frame, TRANSPARENT, EGERGB(255, 255, 255));
            if(player[(game.humanid + i) % 4].awaken) PasteImage((char*)".\\Textures\\awake.png",
                    i ? pos[2 * i] + 132 : 1055, i ? pos[2 * i + 1] + 15 : 435, gui.frame, TRANSPARENT, EGERGB(255, 255, 255));

        }
    }

    //回合内酒效果
    setfillcolor(EGERGB(255, 81, 81), gui.frame);
    setcolor(EGERGB(91, 0, 0), gui.frame);
    setfont(20, 0, "隶书", gui.frame);

    for(int i = 0; i <= 3; i++)
    {
        if(player[(game.humanid + i) % 4].spirits == 1)
        {
            fillellipse(pos[2 * i] + 115, pos[2 * i + 1] + 155, 15, 15, gui.frame);
            outtextxy(pos[2 * i] + 105, pos[2 * i + 1] + 145, (char*)"酒", gui.frame);
        }
    }

    //玩家主动技
    ///当前仅考虑主动技(不包括限定技)只有1个的情况
    if(player[game.humanid].activeskill)
    {
        setfillcolor(EGERGB(36, 174, 154), gui.frame);
        bar(1110, 570, 1160, 590, gui.frame);
        Rect(1110, 570, 1160, 590, EGERGB(146, 233, 220), gui.frame);

        setcolor(EGERGB(19, 89, 79), gui.frame);
        setfont(23, 0, "隶书", gui.frame);
        outtextxy(1111, 568, general_inf[player[game.humanid].general].activeskill, gui.frame);
    }

    //玩家手牌
    for(int i = 0; i <= 7; i++) PasteCard(160 + 100 * i, 465, player[game.humanid].card[game.page * 8 + i], gui.playercard);

    //玩家装备
    for(int i = 0; i <= 3; i++)
    {
        if(player[game.humanid].equips[i] != -1)
        {
            PasteImage( Link( Link( (char*)".\\Textures\\Equipments\\", Myitoa( (int)card_inf[player[game.humanid].equips[i]].type) ), (char*)".png"),
                   5, 453.75 + 37.5 * i, gui.playercard, TRANSPARENT, BLACK);
            PasteImage( Link( Link( (char*)".\\Textures\\Suits\\", Myitoa( (int)card_inf[player[game.humanid].equips[i]].suit) ), (char*)".png"),
                   123, 453.75 + 37.5 * i, gui.playercard, TRANSPARENT, EGERGB(255, 255, 255));

            setcolor((int)card_inf[player[game.humanid].equips[i]].suit & 2 ? EGERGB(255, 0, 0) : EGERGB(11, 11, 11), gui.playercard);
            setfont(15, 0, "Lucida Handwriting", gui.playercard);
            outtextxy(135, 453.75 + 37.5 * i,
                  card_inf[player[game.humanid].equips[i]].num == 1 ? (char*)"A" :
                  card_inf[player[game.humanid].equips[i]].num == 10 ? (char*)"⒑" :
                  card_inf[player[game.humanid].equips[i]].num == 11 ? (char*)"J" :
                  card_inf[player[game.humanid].equips[i]].num == 12 ? (char*)"Q" :
                  card_inf[player[game.humanid].equips[i]].num == 13 ? (char*)"K" :
                  Myitoa( (int)card_inf[player[game.humanid].equips[i]].num),
                  gui.playercard);
        }
    }

    //其他角色装备
    for(int i = 1; i <= 3; i++)
    {
        for(int j = 1; j <= 3; j++)
        {
            if(player[(game.humanid + i) % 4].equips[j] != -1)
            {
                PasteImage( Link( Link( (char*)".\\Textures\\OtherEquipments\\", Myitoa( (int)card_inf[player[(game.humanid + i) % 4].equips[j]].type) ),
                            (char*)".png"), pos[2 * i], pos[2 * i + 1] + 110 + 15 * j, gui.playercard, TRANSPARENT, BLACK);
                PasteImage( Link( Link( (char*)".\\Textures\\Suits\\", Myitoa( (int)card_inf[player[(game.humanid + i) % 4].equips[j]].suit) ),
                            (char*)".png"), pos[2 * i] + 105, pos[2 * i + 1] + 110 + 15 * j, gui.playercard, TRANSPARENT, EGERGB(255, 255, 255));

                setcolor((int)card_inf[player[(game.humanid + i) % 4].equips[j]].suit & 2 ? EGERGB(255, 0, 0) : EGERGB(11, 11, 11), gui.playercard);
                setfont(15, 0, "Lucida Handwriting", gui.playercard);
                outtextxy(pos[2 * i] + 120, pos[2 * i + 1] + 110 + 15 * j,
                      card_inf[player[(game.humanid + i) % 4].equips[j]].num == 1 ? (char*)"A" :
                      card_inf[player[(game.humanid + i) % 4].equips[j]].num == 10 ? (char*)"⒑" :
                      card_inf[player[(game.humanid + i) % 4].equips[j]].num == 11 ? (char*)"J" :
                      card_inf[player[(game.humanid + i) % 4].equips[j]].num == 12 ? (char*)"Q" :
                      card_inf[player[(game.humanid + i) % 4].equips[j]].num == 13 ? (char*)"K" :
                      Myitoa( (int)card_inf[player[(game.humanid + i) % 4].equips[j]].num),
                      gui.playercard);
            }
        }
    }

    //判定区
    for(int i = 0; i <= 3; i++)
    {
        for(int j = 0; j <= 2; j++)
        {
            if(player[(game.humanid + i) % 4].judges[j][1] != -1)
            {
                PasteImage( Link( Link( (char*)".\\Textures\\Judges\\", Myitoa( (int)player[(game.humanid + i) % 4].judges[j][1]) ),
                            (char*)".png"), i ? pos[2 * i] + 130 : 20 * (j + 1), i ? pos[2 * i + 1] + 150 - 20 * j : 430, gui.playercard, TRANSPARENT, BLACK);

            }
        }
    }

    //其他角色阶段提示
    if(game.active != game.humanid)
    {
        char str[] = "准备\0判定\0摸牌\0出牌\0弃牌\0结束\0";
        setcolor(LIGHTGREEN, gui.frame);
        setfont(20, 0, "隶书", gui.background);
        outtextxy(pos[2 * ((game.active - game.humanid + PLAYERS) % PLAYERS)] + 30, pos[2 * ((game.active - game.humanid + PLAYERS) % PLAYERS) + 1] + 170,
                  Link(str + game.period * 5, (char*)"阶段"), gui.frame);
    }

    putimage(0, 0, gui.background);
    putimage_transparent(NULL, gui.throwcard, 0, 0, BLACK);
    putimage_transparent(NULL, gui.general, 0, 0, BLACK);
    putimage_transparent(NULL, gui.frame, 0, 0, BLACK);
    putimage_transparent(NULL, gui.playercard, 0, 0, BLACK);
}
