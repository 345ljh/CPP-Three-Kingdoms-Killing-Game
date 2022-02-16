#include "actions.h"

//牌堆初始化
void PileInit(void)
{
    int temp[160];
    int pointer;

    memset(temp, 0, sizeof(temp));

    for(int i = 0; i <= 159; i++)
    {
        pointer = rand() % 160;
        while(temp[pointer]) ++pointer %= 160;
        game.card[i] = pointer;
        temp[pointer] = 1;
    }

    game.nowpile = 160;
}

//洗牌
void Shuffle(void)
{
    int pilecards = 0;
    int pointer;

    cleardevice(gui.throwcard);

    //统计弃牌堆内牌的数量
    for(int i = 0; i <= 150; i++)
    {
        if(card_inf[i].owner == -1) pilecards++;
    }

    for(int i = 0; i <= pilecards - 1; i++)
    {
        pointer = rand() % 160;
        while(card_inf[pointer].owner != -1) ++pointer %= 160;
        game.card[i] = pointer;
        card_inf[pointer].owner = -2;
    }

    game.nowpile = pilecards;
}

//从牌堆顶摸牌
void Takecard(player_t *recipient, int amount)
{
    for(int i = 1; i <= amount; i++)
    {
        if(!game.nowpile) Shuffle();
        card_inf[game.card[0]].owner = recipient->id;
        recipient->card[recipient->cardamount] = game.card[0];
        recipient->cardamount++;
        game.nowpile--;

        for(int j = 0; j <= game.nowpile - 1; j++)
        {
            game.card[j] = game.card[j + 1];
        }
        game.card[game.nowpile] = -1;
    }

    /* skills here */
}

//弃牌,其中executor为弃牌者,recipient为被弃牌者
///当executor与recipient均为玩家时返回值为弃置牌数,否则为0
///area表示可选其牌区域,0位为1表示手牌区,1位为装备区,2位为判定区,默认值为1(即只可弃置手牌)
///mode为8位整数,0~3位为可弃置的花色(对应suit_e).4~6位为可弃置的类型(低至高分别为基本,装备,锦囊),7位为cancel变量,默认为127(0b01111111)
///cancel=1时可点击取消,此时返回值为0,amount为最大弃牌数
///cancel=0时无法取消,且当区域内牌不足时则全弃
///mode当且仅当executor与recipient为同一角色时生效
///add为附加变量,如[贯石斧]触发或技能等,默认值为0
int Throwcard(player_t *executor, player_t *recipient, int amount, int area, int mode, int add)
{
    //玩家操作弃牌
    if(executor->controller == HUMAN)
    {
        //对玩家自己弃牌(如弃牌阶段)
        //此时并未涉及判定区
        if(recipient->controller == HUMAN)
        {
            //解算mode
            int suit = mode & 15;
            int type = (mode & 112) >> 4;
            int cancel = (mode & 128) >> 7;

            //cancel=0时修改amount实现"若不足则全弃"
            if(!cancel)
            {
                int accord = 0;

                if(area | 1) for(int i = 0; i <= recipient->cardamount - 1; i++)
                        if (suit & (1 << (int)card_inf[recipient->card[i]].suit) && (type & (1 << TypeIdentify(card_inf[recipient->card[i]].type)))) accord++;

                if(area | 2) for(int i = 0; i <= 3; i++)
                        if (recipient->equips[i] != -1 && suit & (1 << (int)card_inf[recipient->equips[i]].suit) && (type & (1 << TypeIdentify(card_inf[recipient->equips[i]].type)))) accord++;

                if(area | 4) for(int i = 0; i <= 2; i++)
                    if(recipient->judges[i][0] != -1 && suit & (1 << (int)card_inf[recipient->judges[i][0]].suit) && (type & (1 << TypeIdentify(card_inf[recipient->judges[i][0]].type)))) accord++;

                if(accord < amount) amount = accord;
            }

            //判断对应区域有牌
            area |= 1 & (recipient->card[0] != -1);
            area |= 2 & (recipient->equips[0] != -1 || recipient->equips[1] != -1 || recipient->equips[2] != -1 || recipient->equips[3] != -1);
            area |= 4 & (recipient->judges[0][0] != -1 || recipient->judges[1][0] != -1 || recipient->judges[2][0] != -1);

            int *tothrow = NULL;
            tothrow = (int*)calloc(amount, sizeof(int));   //储存所弃牌在区域中的位置id
            memset(tothrow, 0xFF, amount * sizeof(int));

            for(; is_run(); delay_fps(10))
            {
                while (mousemsg()) msg = getmouse();
                mousepos(&mouse_x, &mouse_y);

                DrawGui();
                cleardevice(gui.selector);

                //弃牌数提示
                setcolor(WHITE, gui.selector);
                setfont(30, 0, "仿宋", gui.selector);
                outtextxy(550, 415, Link( Link( Link( Link( (char*)"弃置", Myitoa(ArrayOccupied(tothrow, amount) )), (char*)"/"), MyitoaII(amount)), (char*)"张牌"), gui.selector);

                //全部绘制为未选定状态
                if(area & 1) for(int i = 0; i <= 7; i++)
                        if(recipient->card[game.page * 8 + i] != -1 && (suit & (1 << (int)card_inf[recipient->card[game.page * 8 + i]].suit)) &&
                                (type & (1 << TypeIdentify(card_inf[recipient->card[game.page * 8 + i]].type))) )
                            LineRect(160 + 100 * i, 465, 240 + 100 * i, 585, EGERGB(255, 215, 77), gui.selector);
                if(area & 2) for(int i = 0; i <= 3; i++)
                        if(recipient->equips[i] != -1) LineRect(5, 453.75 + 37.5 * i, 145, 483.75 + 37.5 * i, EGERGB(255, 215, 77), gui.selector);
                if(area & 4) for(int i = 0; i <= 2; i++)
                        if(recipient->judges[i][0] != -1) LineRect(20 * (i + 1), 430, 40 * (i + 1), 430, EGERGB(255, 215, 77), gui.selector);

                //绘制已选定状态,tothrow低8位为选定牌在card/equips/judges的下标,高8位代表选定区域,0=手牌,1=装备区,2=判定区
                for(int i = 0; i <= amount - 1; i++)
                {
                    if(tothrow[i] != -1 && recipient->card[tothrow[i]] != -1 && tothrow[i] >> 8 == 0 && game.page == tothrow[i] / 8)
                        LineRect(160 + 100 * (tothrow[i] % 8), 465, 240 + 100 * (tothrow[i] % 8), 585, EGERGB(255, 57, 57), gui.selector);
                    if(tothrow[i] != -1 && tothrow[i] >> 8 == 1 && recipient->equips[tothrow[i] % 0x100] != -1)
                        LineRect(5, 453.75 + 37.5 * (tothrow[i] & 0xF), 145, 483.75 + 37.5 * (tothrow[i] & 0xF), EGERGB(255, 57, 57), gui.selector);
                    if(tothrow[i] != -1 && tothrow[i] >> 8 == 2 && recipient->judges[tothrow[i] % 0x200][0] != -1)
                        if(recipient->judges[i][0] != -1) LineRect(20 * (i + 1), 430, 40 * (i + 1), 430, EGERGB(255, 57, 57), gui.selector);
                }

                //确定键
                if((cancel && ArrayOccupied(tothrow, amount)) || ArrayOccupied(tothrow, amount) == amount) LineRect(960, 510, 1050, 535, EGERGB(255, 215, 77), gui.selector);

                //取消键
                if(cancel) LineRect(960, 540, 1050, 565, EGERGB(255, 215, 77), gui.selector);

                //检测按键
                //手牌区
                if( (area & 1) && msg.is_down() && mouse_x >= 150 && mouse_x <= 950 && mouse_y >= 450 && mouse_y <= 600)
                {
                    int sel = (mouse_x - 150) / 100;
                    if(recipient->cardamount > game.page * 8 + sel && (suit & (1 << (int)card_inf[recipient->card[game.page * 8 + sel]].suit)) &&
                            (type & (1 << TypeIdentify(card_inf[recipient->card[game.page * 8 + sel]].type))) )  //确定对应位置有手牌且花色,类型符合
                    {
                        int found = 0;
                        for(int i = 0; i <= amount - 1; i++)
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
                        if(!found && ArrayOccupied(tothrow, amount) <= amount)
                        {
                            for(int i = 0; i <= amount - 1; i++)
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

                //装备区
                if( (area & 2) && msg.is_down() && mouse_x >= 0 && mouse_x <= 150 && mouse_y >= 450 && mouse_y <= 600)
                {
                    int sel = (int)( (mouse_y - 450) / 37.5);
                    if(recipient->equips[sel] != -1)  //确定对应位置有装备
                    {
                        int found = 0;
                        for(int i = 0; i <= amount - 1; i++)
                        {
                            //若已选中则取消
                            if(tothrow[i] == (0x100 | sel))
                            {
                                tothrow[i] = -1;
                                found++;
                                break;
                            }
                        }

                        //若未选中则选定,将tothrow中目前下标最小的-1改为该牌id
                        if(ArrayOccupied(tothrow, amount) <= amount)
                        {
                            for(int i = 0; i <= amount - 1; i++)
                            {
                                if(tothrow[i] == -1)
                                {
                                    tothrow[i] = 0x100 | sel;
                                    break;
                                }
                            }
                        }
                    }
                }

                //判定区
                if( (area & 4) && msg.is_down() && mouse_x >= 20 && mouse_x <= 80 && mouse_y >= 430 && mouse_y <= 450)
                {
                    int sel = (int)( (mouse_x - 20) / 20);
                    if(recipient->judges[sel][0] != -1)  //确定对应位置有装备
                    {
                        int found = 0;
                        for(int i = 0; i <= amount - 1; i++)
                        {
                            //若已选中则取消
                            if(tothrow[i] == (0x200 | sel))
                            {
                                tothrow[i] = -1;
                                found++;
                                break;
                            }
                        }

                        //若未选中则选定,将tothrow中目前下标最小的-1改为该牌id
                        if(ArrayOccupied(tothrow, amount) <= amount)
                        {
                            for(int i = 0; i <= amount - 1; i++)
                            {
                                if(tothrow[i] == -1)
                                {
                                    tothrow[i] = 0x200 | sel;
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
                if(cancel && msg.is_down() && mouse_x >= 960 && mouse_x <= 1050 && mouse_y >= 540 && mouse_y <= 565)
                {
                    return 0;
                }

                //确定键
                if( (cancel || ArrayOccupied(tothrow, amount) == amount) && msg.is_down() && mouse_x >= 960 && mouse_x <= 1050 && mouse_y >= 510 && mouse_y <= 535)
                {
                    for(int i = 0; i <= amount - 1; i++)
                    {
                        if(tothrow[i] != -1)
                            switch(tothrow[i] >> 8)
                            {
                            case 0:
                            {
                                card_inf[recipient->card[tothrow[i]]].owner = -1;
                                Putcard(recipient->card[tothrow[i]]);
                                recipient->card[tothrow[i]] = -1;
                                recipient->cardamount--;
                                break;
                            }
                            case 1:
                            {
                                card_inf[recipient->equips[tothrow[i] & 0xFF]].owner = -1;
                                Putcard(recipient->equips[tothrow[i] & 0xFF]);
                                recipient->equips[tothrow[i] & 0xFF] = -1;
                                break;
                            }
                            case 2:
                            {
                                card_inf[recipient->judges[tothrow[i] & 0xFF][0]].owner = -1;
                                Putcard(recipient->judges[tothrow[i] & 0xFF][0]);
                                recipient->judges[tothrow[i] & 0xFF][0] = -1;
                                recipient->judges[tothrow[i] & 0xFF][1] = -1;

                                for(int j = 0; j <= 1; j++)
                                {
                                    int temp[3];
                                    for(int k = 0; k <= 2; k++) temp[k] = recipient->judges[k][j];
                                    IndexAlign(temp, ArrayOccupied(temp, 3), 3);
                                    for(int k = 0; k <= 2; k++) recipient->judges[k][j] = temp[k];
                                }
                            }
                            }
                    }
                    //修改手牌下标
                    IndexAlign(recipient->card, recipient->cardamount, 160);
                    DrawGui();
                    return amount;
                }
                putimage_transparent(NULL, gui.selector, 0, 0, BLACK);
            }
        }
        //对其他角色弃牌
        else
        {
            for(int times = 1; times <= amount; times++)
            {
                delay_fps(3);

                DrawGui();
                cleardevice(gui.selector);

                //绘制区域
                setfillcolor(EGERGB(83, 30, 0), gui.selector);
                bar(415, 145, 785, 435, gui.selector);

                char topic[21] = "";
                strcpy(topic, Link( Link( (char*)"弃置", general_inf[recipient->general].name), (char*)"的牌"));
                setcolor(EGERGB(249, 189, 34), gui.selector);
                setfont(20, 0, "隶书", gui.selector);
                outtextxy(600 - strlen(topic) * 5, 145, topic, gui.selector);

                setfont(12, 0, "仿宋", gui.selector);

                setcolor( area & 1 ? EGERGB(190, 183, 68) : LIGHTGRAY, gui.selector);
                Rect(425, 175, 505, 295, area & 1 ? EGERGB(190, 183, 68) : LIGHTGRAY, gui.selector);
                outtextxy(453, 229, (char*)"手牌", gui.selector);
                if( (area & 1) && recipient->cardamount)
                {
                    PasteCard(425, 175, -2,gui.selector);
                    LineRect(425, 175, 505, 295, EGERGB(255, 215, 77), gui.selector);
                }

                char str[] = "武器\0防具\0-1马\0+1马\0";
                setcolor( area & 2 ? EGERGB(190, 183, 68) : LIGHTGRAY, gui.selector);
                for(int i = 0; i <= 3; i++)
                {
                    Rect(425 + 90 * i, 305, 505 + 90 * i, 425, area & 2 ? EGERGB(190, 183, 68) : LIGHTGRAY, gui.selector);
                    outtextxy(453 + 90 * i, 359, str + 5 * i, gui.selector);
                    if( (area & 2) && recipient->equips[i] != -1)
                    {
                        PasteCard(425 + 90 * i, 305, recipient->equips[i], gui.selector);
                        LineRect(425 + 90 * i, 305, 505 + 90 * i, 425, EGERGB(255, 215, 77), gui.selector);
                    }
                }

                setcolor( area & 4 ? EGERGB(190, 183, 68) : LIGHTGRAY, gui.selector);
                for(int i = 0; i <= 2; i++)
                {
                    Rect(515 + 90 * i, 175, 595 + 90 * i, 295, area & 4 ? EGERGB(190, 183, 68) : LIGHTGRAY, gui.selector);
                    outtextxy(543 + 90 * i, 229, (char*)"判定", gui.selector);
                    if( (area & 4) && recipient->judges[i][0] != -1)
                    {
                        PasteCard(515 + 90 * i, 175, recipient->judges[i][0], gui.selector);
                        LineRect(515 + 90 * i, 175, 595 + 90 * i, 295, EGERGB(255, 215, 77), gui.selector);
                    }
                }

                putimage_transparent(NULL, gui.selector, 0, 0, BLACK);

                int tothrow = -1;

                //点选弃置
                for(; is_run(); delay_fps(10))
                {

                    while (mousemsg()) msg = getmouse();
                    mousepos(&mouse_x, &mouse_y);

                    if( (area & 1) && msg.is_down() && mouse_x >= 425 && mouse_x <= 505 && mouse_y >= 175 && mouse_y <= 295)
                    {
                        tothrow = rand() % recipient->cardamount;
                        card_inf[recipient->card[tothrow]].owner = -1;
                        Putcard(recipient->card[tothrow]);
                        recipient->card[tothrow] = -1;
                        recipient->cardamount--;
                        IndexAlign(recipient->card, recipient->cardamount, 160);

                        break;

                    }
                    if( (area & 2) && msg.is_down() && mouse_x >= 425 && mouse_x <= 775 && mouse_y >= 305 && mouse_y <= 425)
                    {
                        tothrow = (mouse_x - 420) / 90;
                        card_inf[recipient->equips[tothrow]].owner = -1;
                        Putcard(recipient->equips[tothrow]);
                        recipient->equips[tothrow] = -1;

                        break;
                    }
                    if( (area & 4) && msg.is_down() && mouse_x >= 515 && mouse_x <= 775 && mouse_y >= 175 && mouse_y <= 295)
                    {
                        tothrow = (mouse_x - 510) / 90;
                        card_inf[recipient->judges[tothrow][0]].owner = -1;
                        Putcard(recipient->judges[tothrow][0]);
                        recipient->judges[tothrow][0] = -1;
                        recipient->judges[tothrow][1] = -1;

                        for(int j = 0; j <= 1; j++)
                        {
                            int temp[3];
                            for(int k = 0; k <= 2; k++) temp[k] = recipient->judges[k][j];
                            IndexAlign(temp, ArrayOccupied(temp, 3), 3);
                            for(int k = 0; k <= 2; k++) recipient->judges[k][j] = temp[k];
                        }

                        break;
                    }
                }

                //若无可弃牌则返回
                if(!( (area & 1 ? recipient->cardamount : 0) + (area & 2 ? ArrayOccupied(recipient->equips, 4) : 0) +
                        (area & 4 ? (recipient->judges[0][0] != -1) + (recipient->judges[1][0] != -1) + (recipient->judges[2][0] != -1) : 0) ))
                    return times;

                DrawGui();
            }
            return 0;
        }
    }
    //executor为电脑
    else  //TODO: AI when cancel == 1
    {
        delay_fps(3);
        //对自己弃牌
        if(recipient->id == executor->id)
        {
            //解算mode
            int suit = mode & 15;
            int type = (mode & 112) >> 4;
            int cancel = (mode & 128) >> 7;

            if(!cancel)
            {
                int accord = 0;

                if(area | 1) for(int i = 0; i <= recipient->cardamount - 1; i++)
                        if(suit & (1 << (int)card_inf[recipient->card[i]].suit) && (type & (1 << TypeIdentify(card_inf[recipient->card[i]].type)))) accord++;

                if(area | 2) for(int i = 0; i <= 3; i++)
                        if(recipient->equips[i] != -1 && suit & (1 << (int)card_inf[recipient->equips[i]].suit) && (type & (1 << TypeIdentify(card_inf[recipient->equips[i]].type)))) accord++;

                if(area | 4) for(int i = 0; i <= 2; i++)
                        if(recipient->judges[i][0] != -1 && suit & (1 << (int)card_inf[recipient->judges[i][0]].suit) && (type & (1 << TypeIdentify(card_inf[recipient->judges[i][0]].type)))) accord++;

                if(accord < amount) amount = accord;
            }

            for(int times = 0; times <= amount - 1; times++)
            {
                //使用优先级判断弃牌
                int state[12] = {3, 1, 0, 2, 6, 6, 7, 5, 4, 2, 3, 2};
                ThrowAi(recipient, state, area, suit, type);

                if(!( (area & 1 ? recipient->cardamount : 0) + (area & 2 ? ArrayOccupied(recipient->equips, 4) : 0) +
                        (area & 4 ? (recipient->judges[0][0] != -1) + (recipient->judges[1][0] != -1) + (recipient->judges[2][0] != -1) : 0) ))
                    return times;
            }
            return amount;
        }

        //对其他角色弃牌
        else
        {
            //根据阵营确定优先级
            int statetemp[2][12] = {{1, 1, 1, 1, 1, 1, 1, 2, 0, 0, 0, 0}, {1, 1, 1, 1, 1, 1, 1, 0, 2, 3, 4, 2}};
            int state[12];
            if(recipient->id + executor->id == 3) memcpy(state, statetemp[0], sizeof(state));
            else memcpy(state, statetemp[1], sizeof(state));

            for(int times = 1; times <= amount; times++)
            {
                ThrowAi(recipient, state, area);

                if(!( (area & 1 ? recipient->cardamount : 0) + (area & 2 ? ArrayOccupied(recipient->equips, 4) : 0) +
                        (area & 4 ? (recipient->judges[0][0] != -1) + (recipient->judges[1][0] != -1) + (recipient->judges[2][0] != -1) : 0) ))
                    return times;
            }
            return amount;
        }
    }
    return 0;//avoid of warning
}

//展示手牌
///amount=-1表示全部展示
void Showcard(player_t *executor, player_t *recipient, int amount)
{
    if(amount != -1)
    {
        /*
        int* selected = NULL;
        selected = (int*)calloc(amount, sizeof(int));
        */
        for(int i = 1; i <= amount; i++)
        {
            /* select cards */
        }
        for(int j = 0; j <= amount - 1; j++)
        {
            /* output thr information of recipient.card[selected[j]] */
        }
    }
}

//获得其他角色牌
///默认type=0表示executor获得recipient的牌,type=1表示executor交给recipient牌
///executor与recipient必不相同
int Getcard(player_t *executor, player_t *recipient, int amount, int area, int type)
{
    if(type == 0)
    {
        if(executor->controller == HUMAN)
        {
            for(int times = 1; times <= amount; times++)
            {
                delay_fps(3);

                DrawGui();
                cleardevice(gui.selector);

                //绘制区域
                setfillcolor(EGERGB(83, 30, 0), gui.selector);
                bar(415, 145, 785, 435, gui.selector);

                char topic[21] = "";
                strcpy(topic, Link( Link( (char*)"获得", general_inf[recipient->general].name), (char*)"的牌"));
                setcolor(EGERGB(249, 189, 34), gui.selector);
                setfont(20, 0, "隶书", gui.selector);
                outtextxy(600 - strlen(topic) * 5, 145, topic, gui.selector);

                setfont(12, 0, "仿宋", gui.selector);

                setcolor( area & 1 ? EGERGB(190, 183, 68) : LIGHTGRAY, gui.selector);
                Rect(425, 175, 505, 295, area & 1 ? EGERGB(190, 183, 68) : LIGHTGRAY, gui.selector);
                outtextxy(453, 229, (char*)"手牌", gui.selector);

                if( (area & 1) && recipient->cardamount)
                {
                    PasteCard(425, 175, -2, gui.selector);
                    LineRect(425, 175, 505, 295, EGERGB(255, 215, 77), gui.selector);
                }

                char str[] = "武器\0防具\0-1马\0+1马\0";
                setcolor( area & 2 ? EGERGB(190, 183, 68) : LIGHTGRAY, gui.selector);
                for(int i = 0; i <= 3; i++)
                {
                    Rect(425 + 90 * i, 305, 505 + 90 * i, 425, area & 2 ? EGERGB(190, 183, 68) : LIGHTGRAY, gui.selector);
                    outtextxy(453 + 90 * i, 359, str + 5 * i, gui.selector);
                    if( (area & 2) && recipient->equips[i] != -1)
                    {
                        PasteCard(425 + 90 * i, 305, recipient->equips[i], gui.selector);
                        LineRect(425 + 90 * i, 305, 505 + 90 * i, 425, EGERGB(255, 215, 77), gui.selector);
                    }
                }

                setcolor( area & 4 ? EGERGB(190, 183, 68) : LIGHTGRAY, gui.selector);
                for(int i = 0; i <= 2; i++)
                {
                    Rect(515 + 90 * i, 175, 595 + 90 * i, 295, area & 4 ? EGERGB(190, 183, 68) : LIGHTGRAY, gui.selector);
                    outtextxy(543 + 90 * i, 229, (char*)"判定", gui.selector);
                    if( (area & 4) && recipient->judges[i][0] != -1)
                    {
                        PasteCard(515 + 90 * i, 175, recipient->judges[i][0], gui.selector);
                        LineRect(515 + 90 * i, 175, 595 + 90 * i, 295, EGERGB(255, 215, 77), gui.selector);
                    }
                }

                putimage_transparent(NULL, gui.selector, 0, 0, BLACK);

                int tothrow = -1;

                for(; is_run(); delay_fps(10))
                {

                    while (mousemsg()) msg = getmouse();
                    mousepos(&mouse_x, &mouse_y);

                    if( (area & 1) && msg.is_down() && mouse_x >= 425 && mouse_x <= 505 && mouse_y >= 175 && mouse_y <= 295)
                    {
                        tothrow = rand() % recipient->cardamount;
                        executor->cardamount++;
                        executor->card[executor->cardamount - 1] = recipient->card[tothrow];
                        card_inf[recipient->card[tothrow]].owner = executor->id;
                        recipient->card[tothrow] = -1;
                        recipient->cardamount--;
                        IndexAlign(recipient->card, recipient->cardamount, 160);

                        break;

                    }
                    if( (area & 2) && msg.is_down() && mouse_x >= 425 && mouse_x <= 775 && mouse_y >= 305 && mouse_y <= 425)
                    {
                        tothrow = (mouse_x - 420) / 90;
                        executor->cardamount++;
                        executor->card[executor->cardamount - 1] = recipient->equips[tothrow];
                        card_inf[recipient->equips[tothrow]].owner = executor->id;
                        recipient->equips[tothrow] = -1;

                        break;
                    }
                    if( (area & 4) && msg.is_down() && mouse_x >= 515 && mouse_x <= 775 && mouse_y >= 175 && mouse_y <= 295)
                    {
                        tothrow = (mouse_x - 510) / 90;
                        executor->cardamount++;
                        executor->card[executor->cardamount - 1] = recipient->judges[tothrow][0];
                        card_inf[recipient->judges[tothrow][0]].owner = executor->id;
                        Putcard(recipient->judges[tothrow][0]);
                        recipient->judges[tothrow][0] = -1;
                        recipient->judges[tothrow][1] = -1;

                        for(int j = 0; j <= 1; j++)
                        {
                            int temp[3];
                            for(int k = 0; k <= 2; k++) temp[k] = recipient->judges[k][j];
                            IndexAlign(temp, ArrayOccupied(temp, 3), 3);
                            for(int k = 0; k <= 2; k++) recipient->judges[k][j] = temp[k];
                        }

                        break;
                    }
                }

                //若无可获得牌则返回
                if(!( (area & 1 ? recipient->cardamount : 0) + (area & 2 ? ArrayOccupied(recipient->equips, 4) : 0) +
                        (area & 4 ? (recipient->judges[0][0] != -1) + (recipient->judges[1][0] != -1) + (recipient->judges[2][0] != -1) : 0) ))
                    return times;

                DrawGui();
            }
            return 0;
        }

        else
        {
            delay_fps(3);
            //根据阵营确定优先级
            int statetemp[2][12] = {{1, 1, 1, 1, 1, 1, 1, 2, 0, 0, 0, 0}, {1, 1, 1, 1, 1, 1, 1, 0, 2, 3, 4, 2}};
            int state[12];
            if(recipient->id + executor->id == 3) memcpy(state, statetemp[0], sizeof(state));
            else memcpy(state, statetemp[1], sizeof(state));

            for(int times = 1; times <= amount; times++)
            {
                GetAi(executor, recipient, state, area);

                if(!( (area & 1 ? recipient->cardamount : 0) + (area & 2 ? ArrayOccupied(recipient->equips, 4) : 0) +
                        (area & 4 ? (recipient->judges[0][0] != -1) + (recipient->judges[1][0] != -1) + (recipient->judges[2][0] != -1) : 0) ))
                    return times;
            }
            return amount;
        }
    }
    //交给其他角色牌
    else
    {
        ;  //TODO
    }
    return 0;//avoid of warning
}

//判定
///返回值为卡牌id
int Judging(player_t *recipient)
{
    if(!game.nowpile) Shuffle();

    //将牌堆顶一张牌用于判定
    int res = game.card[0];
    card_inf[game.card[0]].owner = -1;
    game.card[0]= -1;
    game.nowpile--;
    Putcard(res);
    DrawGui();

    //整理牌堆
    IndexAlign(game.card, game.nowpile, 160);

    //显示判定结果
    cleardevice(gui.selector);
    setfillcolor(EGERGB(83, 30, 0), gui.selector);
    bar(0, 0, 240, 80, gui.selector);

    char topic[31] = "";
    strcpy(topic, Link(general_inf[recipient->general].name, (char*)"的判定结果") );
    setcolor(EGERGB(249, 189, 34), gui.selector);
    setfont(20, 0, "隶书", gui.selector);
    outtextxy(120 - strlen(topic) * 5, 0, topic, gui.selector);

    PIMAGE suitpic[2] = {newimage(), newimage()};  //花色图片需要放大,而使用放大贴图的时候不能同时透明,所以使用双图层
    getimage(suitpic[0], Link( Link( (char*)".\\Textures\\Suits\\", Myitoa( (int)card_inf[res].suit) ), (char*)".png"));
    getimage(suitpic[1], 0, 0, 1200, 600);
    setbkcolor(EGERGB(255, 255, 255), suitpic[1]);
    cleardevice(suitpic[1]);
    putimage_rotatezoom(suitpic[1], suitpic[0], 15, 30, 0, 0, 0, 2);
    putimage_transparent(gui.selector, suitpic[1], 0, 0, EGERGB(255, 255, 255));

    setfont(35, 0, "仿宋", gui.selector);
    outtextxy(45, 28, card_inf[res].num == 1 ? (char*)"A" :
                  card_inf[res].num == 11 ? (char*)"J" :
                  card_inf[res].num == 12 ? (char*)"Q" :
                  card_inf[res].num == 13 ? (char*)"K" :
                  Myitoa( (int)card_inf[res].num), gui.selector);

    setfont(25, 0, "仿宋", gui.selector);
    outtextxy(80, 35, card_inf[res].name, gui.selector);

    putimage_transparent(NULL, gui.selector, 0, 0, BLACK);
    delay_ms(0);  //奇怪的bug,不加这两句显示会有问题
    putimage_transparent(NULL, gui.selector, 0, 0, BLACK);
    delay_fps(0.6);

    return res;
}

//造成伤害
///executor=NULL表示伤害无来源(如闪电),type=LOSS表示失去体力
void Damage(player_t *executor, player_t *recipient, int amount, damage_e type)
{
    delay_fps(5);

    recipient->health -= amount;
    recipient->maxcard -= amount;  //手牌上限一般随体力变化
    DrawGui();

    if(recipient->health <= 0)
    {
        Neardeath(recipient);
    }

    //连锁结算
    if(recipient->chained && (type == FIRE || type == THUNDER) )
    {
        recipient->chained = 0;
        DrawGui();

        for(int i = 1; i <= 3; i++)
        {
            if(player[(recipient->id + i) % PLAYERS].chained) Damage(executor, &player[(recipient->id + i) % PLAYERS], amount, type);
            break;
        }
    }
}

//恢复
void Recover(player_t *recipient, int amount)
{
    if(recipient->maxhealth - recipient->health < amount) amount = recipient->maxhealth - recipient->health;
    recipient->health += amount;
    recipient->maxcard += amount;
}

//濒死结算
void Neardeath(player_t *recipient)
{
    /* skills here */
    for(int i = 0; i <= PLAYERS - 1; i++)
    {
        do
        {
            /*  ask recipient that id = (recipient.id + i) % PLAYERS for a peach */
            if(recipient->health > 0) return;
        }
        while(0/* used a peach */);
    }

    if(recipient->health < 0) Death(recipient);
}

//死亡结算
void Death(player_t *recipient)
{
    recipient->controller = DEAD;
    /* VictoryJudge(); */

    for(int i = 0; i <= recipient->cardamount - 1; i++)
    {
        card_inf[recipient->card[i]].owner = -1;
    }

    Throwcard(recipient, recipient, recipient->cardamount);
}

//胜利条件判定
void VictoryJudge(void)
{
    if(player[0].controller == DEAD && player[3].controller == DEAD)  //1号与4号均死亡
    {
        if(game.humanid != 0 && game.humanid != 3)  /* victory */;  //1号与4号均为电脑
        else /* failed */;
    }
    else if( player[1].controller == DEAD && player[2].controller == DEAD)
    {
        if(game.humanid != 1 && game.humanid != 2)  /* victory */;
        else /* failed */;
    }
}

//无懈响应
///start为第一次询问无懈时开始的角色id,card为锦囊id,用于提示信息
///返回值0=此牌仍有效,1=被无懈抵消
int AskWuxie(int start, int card)
{
    int res = 0;
    int ans = 0;

    do
    {
        ans = 0;

        for(int i = 0; i <= 3; i++)
        {
            if(player[start + i].controller == HUMAN)
            {
                /* use here */
            }
            else if(player[start + i].controller == AI)
            {
                /* use here */
            }

            if(ans)
            {
                (start += i) %= PLAYERS;  //下一次询问从此次使用者开始
                res++;
                break;
            }
        }
    }
    while(ans);

    return res % 2;
}
