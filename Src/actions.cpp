#include "actions.h"

///本文件中game均作为全局变量出现

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
void Takecard(player_t *player, int amount)
{
    for(int i = 1; i <= amount; i++)
    {
        if(!game.nowpile) Shuffle();
        card_inf[game.card[0]].owner = player->id;
        player->card[player->cardamount] = game.card[0];
        player->cardamount++;
        game.nowpile--;

        for(int j = 0; j <= game.nowpile - 1; j++)
        {
            game.card[j] = game.card[j + 1];
        }
        game.card[game.nowpile] = -1;
    }

        /* skills here */
}

//弃牌,其中executor为弃牌者,player为被弃牌者
///area表示可选其牌区域,0位为1表示手牌区,1位为装备区,2位为判定区,默认值为1(即只可弃置手牌)
///此函数下弃手牌不能对所弃牌的种类进行限制,如火攻时的相同花色
void Throwcard(player_t *executor, player_t *player, int amount, int area)
{
    //判断对应区域有牌
    area |= 1 & (player->card[0] != -1);
    area |= 2 & (player->equips[0] != -1 || player->equips[1] != -1 || player->equips[2] != -1 || player->equips[3] != -1);
    area |= 4 & (player->judges[0][0] != -1 || player->judges[1][0] != -1 || player->judges[2][0] != -1);

    if(executor->controller == HUMAN &&player->controller == HUMAN)
    {
        int *tothrow = NULL;
        tothrow = (int*)calloc(amount, sizeof(int));   //储存所弃牌在区域中的位置id
        memset(tothrow, 0xFF, amount * sizeof(int));
        DrawGui();

        //已知玩家弃置自己的牌时不会涉及判定区
        for(; is_run(); delay_fps(100))
        {
            while (mousemsg()) msg = getmouse();

            //全部绘制为未选定状态
            if(ArrayOccupied(tothrow, amount) < amount)
            {
                if(area & 1) for(int i = 0; i <= 7; i++)
                    if(player->card[game.page * 8 + i] != -1) LineRect(160 + 100 * i, 465, 240 + 100 * i, 585, EGERGB(255, 215, 77), gui.selector);
                if(area & 2) for(int i = 0; i <= 3; i++)
                    if(player->equips[i] != -1) LineRect(5, 453.75 + 37.5 * i, 145, 483.75 + 37.5 * i, EGERGB(255, 215, 77), gui.selector);
            }

            //绘制已选定状态,tothrow低8位为选定牌在card/equips/judges的下标,高8位代表选定区域,0=手牌,1=装备区,2=判定区
            for(int i = 0; i <= amount - 1; i++)
            {
                if(tothrow[i] >> 8 == 0 && game.page == tothrow[i] / 8 && player->card[tothrow[i]] != -1)
                    LineRect(160 + 100 * tothrow[i] % 8, 465, 240 + 100 * tothrow[i] % 8, 585, EGERGB(255, 215, 77), gui.selector);
                if(tothrow[i] >> 8 == 1 && player->equips[tothrow[i] % 0x10] != -1)
                    LineRect(5, 453.75 + 37.5 * (tothrow[i] % 0x10), 145, 483.75 + 37.5 * (tothrow[i] % 0x10), EGERGB(255, 215, 77), gui.selector);
            }

            //检测按键
            for(int i = 0; i <= 7; i++)
            {
                //手牌区
                if(mouse_x >= 150 + 100 * i && mouse_x <= 250 + 100 * i && mouse_y >= 450 && mouse_y <= 600)
                {
                    if(player->cardamount > game.page * 8 + i)  //确定对应位置有手牌
                    {
                        int found = 0;
                        for(int j = 0; j <= amount - 1; j++)
                        {
                            //若已选中则取消
                            if(tothrow[j] == game.page * 8 + i)
                            {
                                tothrow[j] = -1;
                                found++;
                                break;
                            }
                        }

                        //若未选中则选定,将tothrow中目前下标最小的-1改为该牌id
                        if(ArrayOccupied(tothrow, amount) != amount)
                        {
                            for(int j = 0; j <= amount - 1; j++)
                            {
                                if(tothrow[j] == -1)
                                {
                                    tothrow[j] = game.page + i;
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

//展示手牌
///amount=-1表示全部展示
void Showcard(player_t *executor, player_t *player, int amount)
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
            /* output thr information of player.card[selected[j]] */
        }
    }
}

//获得其他角色牌
///默认type=0表示executor获得player的牌,type=1表示executor交给player牌
//TODO: amount=-1 stands for getting or giving all cards
void Getcard(player_t *executor, player_t *player, int amount, int type = 0)
{
    if(type)
    {
        /*
        int* selected = NULL;
        selected = (int*)calloc(amount, sizeof(int));
        */
        for(int i = 1; i <= amount; i++)
        {
            /*
            card_inf[executor.card[selected[j]]] = player.id;
            player.card[player.cardamount] = executor.card[selected[j]];
            executor.card[selected[j]] = -1;
            */
            executor->cardamount--;
            player->cardamount++;
        }
        /* skills here */
    }
    else
    {
        /*
        int* selected = NULL;
        selected = (int*)calloc(amount, sizeof(int));
        */
        for(int i = 1; i <= amount; i++)
        {
            /*
            card_inf[player.card[selected[j]]] = executor.id;
            executor.card[executor.cardamount] = player.card[selected[j]];
            player.card[selected[j]] = -1;
            */
            player->cardamount--;
            executor->cardamount++;
        }
        /* skills here */
    }
}

//判定
///返回值为卡牌id
int Judging(player_t *player)
{
    if(!game.nowpile) Shuffle();

    int res = game.card[0];
    card_inf[game.card[0]].owner = -1;
    game.nowpile--;

    for(int j = 0; j <= game.nowpile - 1; j++)
    {
        game.card[j] = game.card[j + 1];
    }
    game.card[game.nowpile] = -1;

    return res;
}
//造成伤害
//TODO: Add the card that causes the damage for the skill [奸雄]
///executor=NULL表示伤害无来源(如闪电),type=LOSS表示失去体力
void Damage(player_t *executor, player_t *player, int amount, damage_e type)
{
    /* skills here */
    player->health -= amount;
    if(player->health <= 0)
    {
        Neardeath(player);
    }
    /* skills here */
    /* chain here */
}

//恢复
void Recover(player_t *player, int amount)
{
    if(player->maxhealth - player->health < amount) amount = player->maxhealth - player->health;
    player->health += amount;
    /* skills here */
}

//濒死结算
void Neardeath(player_t *player)
{
    /* skills here */
    for(int i = 0; i <= PLAYERS - 1; i++)
    {
        do
        {
            /*  ask player that id = (player.id + i) % PLAYERS for a peach */
            if(player->health > 0) return;
        }while(0/* used a peach */);
    }

    if(player->health < 0) Death(player);
}

//死亡结算
void Death(player_t *player)
{
    player->controller = DEAD;
    /* VictoryJudge(); */

    for(int i = 0; i <= player->cardamount - 1; i++)
    {
        card_inf[player->card[i]].owner = -1;
    }

    Throwcard(player, player, player->cardamount);
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
    }while(ans);

    return res % 2;
}
