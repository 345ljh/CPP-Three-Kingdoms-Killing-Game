/**
 * @file    ai.cpp
 * @brief   AI对牌局的分析与判断
 * @note    所有函数以"Ai"结尾
 */


#include "ai.h"

//弃牌AI
///返回值用于白银狮子效果
int ThrowAi(player_t* recipient, int* state, int area, int suit, int type, int add)
{
    int maxstate = -1;
    int len = 1;
    int *tothrow = NULL;
    tothrow = (int*)malloc(sizeof(int));
    if(area & 1) for(int i = 0; i <= recipient->cardamount - 1; i++)
            if (suit & (1 << (int)card_inf[recipient->card[i]].suit) && (type & (1 << TypeIdentify(card_inf[recipient->card[i]].type))))
            {
                /*优先级变高,重置tothrow,此时只有1个元素*/
                if(StateCompareAi(state, recipient->card[i]) > maxstate)
                {
                    maxstate = StateCompareAi(state, recipient->card[i]);
                    int *ptemp = tothrow;
                    tothrow = (int*)realloc(ptemp, sizeof(int));
                    tothrow[0] = recipient->card[i];
                    len = 1;
                }
                /*优先级相等,增加1个元素*/
                else if(StateCompareAi(state, recipient->card[i]) == maxstate)
                {
                    int *ptemp = tothrow;
                    tothrow = (int*)realloc(ptemp, ++len * sizeof(int));
                    tothrow[len - 1] = recipient->card[i];
                }
            }
    if(area & 2) for(int i = (add == 0x30) ? 1 : (add == 0x50 ? 2 : 0); i <= 3; i++)
            if ((recipient->equips[i] != -1) && suit & (1 << (int)card_inf[recipient->equips[i]].suit) && (type & (1 << TypeIdentify(card_inf[recipient->equips[i]].type))))
            {
                if(StateCompareAi(state, recipient->equips[i] | 0x100) > maxstate)
                {
                    maxstate = StateCompareAi(state, recipient->equips[i] | 0x100);
                    int *ptemp = tothrow;
                    tothrow = (int*)realloc(ptemp, sizeof(int));
                    tothrow[0] = recipient->equips[i] | 0x100;
                    len = 1;
                }
                else if(StateCompareAi(state, recipient->equips[i] | 0x100) == maxstate)
                {
                    realloc(tothrow, ++len * sizeof(int));
                    tothrow[len - 1] = recipient->equips[i] | 0x100;
                }
            }
    if(area & 4) for(int i = 0; i <= 2; i++)
            if (suit & (1 << (int)card_inf[recipient->judges[i][0]].suit) && (type & (1 << TypeIdentify(card_inf[recipient->judges[i][0]].type))))
            {
                if(StateCompareAi(state, recipient->judges[i][0] | 0x200) > maxstate)
                {
                    maxstate = StateCompareAi(state, recipient->judges[i][0] | 0x200);
                    int *ptemp = tothrow;
                    tothrow = (int*)realloc(ptemp, sizeof(int));
                    tothrow[0] = recipient->judges[i][0] | 0x200;
                    len = 1;
                }
                else if(StateCompareAi(state, recipient->judges[i][0] | 0x200) == maxstate)
                {
                    realloc(tothrow, ++len * sizeof(int));
                    tothrow[len - 1] = recipient->judges[i][0] | 0x200;
                }
            }

    int sel = rand() % len;
    int baiyin = 0;  //白银狮子效果

    switch(sel >> 16)
    {
    case 0:
    {
        card_inf[recipient->card[sel]].owner = -1;
        Putcard(recipient->card[sel]);
        Printcard(recipient->card[sel]);
        recipient->card[sel] = -1;
        recipient->cardamount--;
        IndexAlign(recipient->card, recipient->cardamount, 160);
        break;
    }
    case 1:
    {
        card_inf[recipient->equips[sel & 0xFF]].owner = -1;
        Putcard(recipient->equips[sel & 0xFF]);
        Printcard(recipient->equips[sel & 0xFF]);
        if( (type_e)recipient->equips[sel & 0xFF] == BAIYIN)  baiyin = 1;
        recipient->equips[sel & 0xFF] = -1;
        break;
    }
    case 2:
    {
        card_inf[recipient->judges[sel & 0xFF][0]].owner = -1;
        Putcard(recipient->judges[sel & 0xFF][0]);
        Printcard(recipient->judges[sel & 0xFF][0]);
        recipient->judges[sel & 0xFF][0] = -1;
        recipient->judges[sel & 0xFF][1] = -1;
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
    free(tothrow);
    return baiyin;
}


//获得牌AI
int GetAi(player_t *executor, player_t* recipient, int* state, int area)
{
    int maxstate = -1;
    int len = 1;
    int *tothrow = NULL;
    tothrow = (int*)malloc(sizeof(int));

    if(area & 1) for(int i = 0; i <= recipient->cardamount - 1; i++)
    {
        /*优先级变高,重置tothrow,此时只有1个元素*/
        if(StateCompareAi(state, recipient->card[i]) > maxstate)
        {
            maxstate = StateCompareAi(state, recipient->card[i]);
            int *ptemp = tothrow;
            tothrow = (int*)realloc(ptemp, sizeof(int));
            tothrow[0] = recipient->card[i];
            len = 1;
        }
        /*优先级相等,增加1个元素*/
        else if(StateCompareAi(state, recipient->card[i]) == maxstate)
        {
            int *ptemp = tothrow;
            tothrow = (int*)realloc(ptemp, ++len * sizeof(int));
            tothrow[len - 1] = recipient->card[i];
        }
    }
    if(area & 2) for(int i = 0; i <= 3; i++)
    {
        if(StateCompareAi(state, recipient->equips[i] | 0x100) > maxstate)
        {
            maxstate = StateCompareAi(state, recipient->equips[i] | 0x100);
            int *ptemp = tothrow;
            tothrow = (int*)realloc(ptemp, sizeof(int));
            tothrow[0] = recipient->equips[i] | 0x100;
            len = 1;
        }
        else if(StateCompareAi(state, recipient->equips[i] | 0x100) == maxstate)
        {
            realloc(tothrow, ++len * sizeof(int));
            tothrow[len - 1] = recipient->equips[i] | 0x100;
        }
    }
    if(area & 4) for(int i = 0; i <= 2; i++)
    {
        if(StateCompareAi(state, recipient->judges[i][0] | 0x200) > maxstate)
        {
            maxstate = StateCompareAi(state, recipient->judges[i][0] | 0x200);
            int *ptemp = tothrow;
            tothrow = (int*)realloc(ptemp, sizeof(int));
            tothrow[0] = recipient->judges[i][0] | 0x200;
            len = 1;
        }
        else if(StateCompareAi(state, recipient->judges[i][0] | 0x200) == maxstate)
        {
            realloc(tothrow, ++len * sizeof(int));
            tothrow[len - 1] = recipient->judges[i][0] | 0x200;
        }
    }

    int sel = rand() % len;
    int baiyin = 0;

    switch(sel >> 16)
    {
    case 0:
    {
        executor->cardamount++;
        executor->card[executor->cardamount - 1] = recipient->card[sel];
        card_inf[recipient->card[sel]].owner = executor->id;

        if(executor->controller == HUMAN || recipient->controller == HUMAN) Printcard(recipient->card[sel]);

        recipient->card[sel] = -1;
        recipient->cardamount--;
        IndexAlign(recipient->card, recipient->cardamount, 160);
        break;
    }
    case 1:
    {
        executor->cardamount++;
        executor->card[executor->cardamount - 1] = recipient->equips[sel & 0xFF];
        card_inf[recipient->equips[sel & 0xFF]].owner = executor->id;
        if(executor->controller == HUMAN || recipient->controller == HUMAN) Printcard(recipient->equips[sel & 0xFF]);
        if( (type_e)recipient->equips[sel & 0xFF] == BAIYIN)  baiyin = 1;
        recipient->equips[sel & 0xFF] = -1;
        break;
    }
    case 2:
    {
        executor->cardamount++;
        executor->card[executor->cardamount - 1] = recipient->judges[sel & 0xFF][0];
        card_inf[recipient->judges[sel & 0xFF][0]].owner = executor->id;

        if(executor->controller == HUMAN || recipient->controller == HUMAN) Printcard(recipient->judges[sel & 0xFF][0]);

        recipient->judges[sel & 0xFF][0] = -1;
        recipient->judges[sel & 0xFF][1] = -1;
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
    free(tothrow);
    return baiyin;
}

//弃牌权重比对,返回值较大者优先弃置
///state中元素按顺序代表:[手牌区]杀,闪,桃/酒,无懈,其他普通锦囊,延时锦囊,装备,[判定区],[装备区]武器,防具,-1,+1
///id低16位为card_inf对应的id,高8位表示区域
int StateCompareAi(int state[13], int id)
{
    int area = id >> 16;
    id &= 0xFF;
    return
    ( (int)card_inf[id].type <= 2 && !area) ? state[0] :
    (card_inf[id].type == SHAN && !area)? state[1] :
    ((card_inf[id].type == TAO || card_inf[id].type == JIU) && !area) ? state[2] :
    (card_inf[id].type == WUXIE && !area) ? state[3] :
    ( ((int)card_inf[id].type & 0xF0) == 0x90 && !area) ? state[4] :
    ( ((int)card_inf[id].type & 0xF0) == 0xA0 && !area) ? state[5] :
    !area ? state[6] :
    area == 2 ? state[7] :
    ((int)card_inf[id].type >= 0x10 && (int)card_inf[id].type < 0x60) ? state[8] :
    ( ((int)card_inf[id].type & 0xF0) == 0x60) ? state[9] :
    ( ((int)card_inf[id].type & 0xF0) == 0x70) ? state[10] : state[11];
}

//出牌响应AI
///返回值为手牌中的位置,若没有符合条件的牌则返回-1
///若type为SHA则3种类型的杀均可响应
int AnswerAi(player_t *recipient, type_e type, int add)
{
        int *buf = NULL;
        int amount = 0;
        buf = (int*)calloc(0, sizeof(int));

        //判断手牌中是否有对应类型,若有则随机出一张
        for(int i = 0; i <= recipient->cardamount - 1; i++)
        {
            if( (type == SHA && (card_inf[recipient->card[i]].type == SHA || card_inf[recipient->card[i]].type == HUOSHA || card_inf[recipient->card[i]].type == LEISHA)) ||
                card_inf[recipient->card[i]].type == type ||
                (add == 2 && card_inf[recipient->card[i]].type == JIU) )
               {
                   int *ptemp = buf;
                   buf = (int*)realloc(ptemp, ++amount * sizeof(int));
                   buf[amount - 1] = i;
               }
        }

        if(amount) return buf[rand() % amount];
        else return -1;
}

//五谷AI
int WuguAi(player_t* recipient, int* buf, int len)
{
    int *state = NULL;
    state = (int*)calloc(len, sizeof(int));

    //分别计算剩余牌的权重
    for(int i = 0; i <= len - 1; i++)
    {
        if(buf[i] == -1)
        {
            state[i] = 100;
            continue;
        }
        if(card_inf[buf[i]].type == SHA || card_inf[buf[i]].type == HUOSHA || card_inf[buf[i]].type == LEISHA)
        {
            if(game.active == recipient->id) state[i] = 3;
            else state[i] = 2;
        }
        if(card_inf[buf[i]].type == SHAN)
        {
            if(game.active == recipient->id) state[i] = 2;
            else state[i] = 1;
        }
        if(card_inf[buf[i]].type == TAO)
        {
            state[i] = 0;
        }
        if(card_inf[buf[i]].type == JIU)
        {
            if(recipient->health == 1) state[i] = 0;
            else state[i] = 1;
        }
        if(card_inf[buf[i]].type == JUEDOU)
        {
            if(game.active == recipient->id) state[i] = 3;
            else state[i] = 3;
        }
        if(card_inf[buf[i]].type == GUOCHAI)
        {
            if(game.active == recipient->id) state[i] = 2;
            else state[i] = 1;
        }
        if(card_inf[buf[i]].type == SHUNQIAN)
        {
            if(game.active == recipient->id) state[i] = 2;
            else state[i] = 1;
        }
        if(card_inf[buf[i]].type == WANJIAN)
        {
            if(game.active == recipient->id &&
               (player[3 - recipient->id].health > 1 || player[(recipient->id + 2) % 4].health == 1 || player[(recipient->id + 2) % 4].health == 1)) state[i] = 2;
            else state[i] = 3;
        }
        if(card_inf[buf[i]].type == NANMAN)
        {
            if(game.active == recipient->id &&
               (player[3 - recipient->id].health > 1 || player[(recipient->id + 2) % 4].health == 1 || player[(recipient->id + 2) % 4].health == 1)) state[i] = 2;
            else state[i] = 3;
        }
        if(card_inf[buf[i]].type == TAOYUAN)
        {
            if(game.active == recipient->id &&
               (recipient->health < recipient->maxhealth && player[3 - recipient->id].health < player[3 - recipient->id].maxhealth)) state[i] = 2;
            else state[i] = 4;
        }
        if(card_inf[buf[i]].type == WUZHONG)
        {
            if(game.active == recipient->id) state[i] = 0;
            else state[i] = 1;
        }
        if(card_inf[buf[i]].type == WUGU)
        {
            if(game.active == recipient->id) state[i] = 4;
            else state[i] = 5;
        }
        if(card_inf[buf[i]].type == WUXIE)
        {
            if(game.active == recipient->id) state[i] = 1;
            else state[i] = 2;
        }
        if(card_inf[buf[i]].type == HUOGONG)
        {
            if(game.active == recipient->id && recipient->cardamount >= 3) state[i] = 3;
            else state[i] = 3;
        }
        if(card_inf[buf[i]].type == TIESUO)
        {
            state[i] = 4;
        }
        if(card_inf[buf[i]].type == JIEDAO)
        {
            if(game.active == recipient->id && (player[(recipient->id + 2) % 4].equips[0] != -1 || player[(recipient->id + 3) % 4].equips[0] != -1)) state[i] = 2;
            else state[i] = 4;
        }
        if(card_inf[buf[i]].type == LE)
        {
            if(game.active == recipient->id) state[i] = 1;
            else state[i] = 2;
        }
        if(card_inf[buf[i]].type == BING)
        {
            if(game.active == recipient->id) state[i] = 1;
            else state[i] = 2;
        }
        if(card_inf[buf[i]].type == SHANDIAN)
        {
            if(game.active == recipient->id) state[i] = 5;
            else state[i] = 5;
        }
        if( (int)card_inf[buf[i]].type >= 0x10 && (int)card_inf[buf[i]].type <= 0x60)
        {
            if(game.active == recipient->id) state[i] = 2;
            else state[i] = 4;
        }
        if( (int)card_inf[buf[i]].type >= 0x60 && (int)card_inf[buf[i]].type <= 0x70)
        {
            if(game.active == recipient->id) state[i] = 1;
            else state[i] = 2;
        }
        if( (int)card_inf[buf[i]].type >= 0x70 && (int)card_inf[buf[i]].type <= 0x90)
        {
            if(game.active == recipient->id) state[i] = 1;
            else state[i] = 3;
        }
    }

    int res = 0;
    for(int i = 1; i <= len - 1; i++) if(state[i] < state[res]) res = i;
    return res;
}
