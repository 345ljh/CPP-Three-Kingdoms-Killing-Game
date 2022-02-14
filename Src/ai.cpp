#include "ai.h"

//弃牌AI
void ThrowAi(player_t* player, int* state, int area, int suit, int type)
{
    int maxstate = -1;
    int len = 1;
    int *tothrow = NULL;
    tothrow = (int*)malloc(sizeof(int));
    if(area & 1) for(int i = 0; i <= player->cardamount - 1; i++)
            if (suit & (1 << (int)card_inf[player->card[i]].suit) && (type & (1 << TypeIdentify(card_inf[player->card[i]].type))))
            {
                /*优先级变高,重置tothrow,此时只有1个元素*/
                if(StateCompareAi(state, player->card[i]) > maxstate)
                {
                    maxstate = StateCompareAi(state, player->card[i]);
                    int *ptemp = tothrow;
                    tothrow = (int*)realloc(ptemp, sizeof(int));
                    tothrow[0] = player->card[i];
                    len = 1;
                }
                /*优先级相等,增加1个元素*/
                else if(StateCompareAi(state, player->card[i]) == maxstate)
                {
                    int *ptemp = tothrow;
                    tothrow = (int*)realloc(ptemp, ++len * sizeof(int));
                    tothrow[len - 1] = player->card[i];
                }
            }
    if(area & 2) for(int i = 0; i <= 3; i++)
            if ((player->equips[i] != -1) && suit & (1 << (int)card_inf[player->equips[i]].suit) && (type & (1 << TypeIdentify(card_inf[player->equips[i]].type))))
            {
                if(StateCompareAi(state, player->equips[i] | 0x100) > maxstate)
                {
                    maxstate = StateCompareAi(state, player->equips[i] | 0x100);
                    int *ptemp = tothrow;
                    tothrow = (int*)realloc(ptemp, sizeof(int));
                    tothrow[0] = player->equips[i] | 0x100;
                    len = 1;
                }
                else if(StateCompareAi(state, player->equips[i] | 0x100) == maxstate)
                {
                    realloc(tothrow, ++len * sizeof(int));
                    tothrow[len - 1] = player->equips[i] | 0x100;
                }
            }
    if(area & 4) for(int i = 0; i <= 2; i++)
            if (suit & (1 << (int)card_inf[player->judges[i][0]].suit) && (type & (1 << TypeIdentify(card_inf[player->judges[i][0]].type))))
            {
                if(StateCompareAi(state, player->judges[i][0] | 0x200) > maxstate)
                {
                    maxstate = StateCompareAi(state, player->judges[i][0] | 0x200);
                    int *ptemp = tothrow;
                    tothrow = (int*)realloc(ptemp, sizeof(int));
                    tothrow[0] = player->judges[i][0] | 0x200;
                    len = 1;
                }
                else if(StateCompareAi(state, player->judges[i][0] | 0x200) == maxstate)
                {
                    realloc(tothrow, ++len * sizeof(int));
                    tothrow[len - 1] = player->judges[i][0] | 0x200;
                }
            }
    int sel = rand() % len;
    switch(sel >> 16)
    {
    case 0:
    {
        card_inf[player->card[sel]].owner = -1;
        Putcard(player->card[sel]);
        player->card[sel] = -1;
        player->cardamount--;
        IndexAlign(player->card, player->cardamount, 160);
        break;
    }
    case 1:
    {
        card_inf[player->equips[sel & 0xFF]].owner = -1;
        Putcard(player->equips[sel & 0xFF]);
        player->equips[sel & 0xFF] = -1;
        break;
    }
    case 2:
    {
        card_inf[player->judges[sel & 0xFF][0]].owner = -1;
        Putcard(player->judges[sel & 0xFF][0]);
        player->judges[sel & 0xFF][0] = -1;
        player->judges[sel & 0xFF][1] = -1;
        for(int j = 0; j <= 1; j++)
        {
            int temp[3];
            for(int k = 0; k <= 2; k++) temp[k] = player->judges[k][j];
            IndexAlign(temp, ArrayOccupied(temp, 3), 3);
            for(int k = 0; k <= 2; k++) player->judges[k][j] = temp[k];
        }
        break;
    }
    }
    free(tothrow);
}


//获得牌AI
void GetAi(player_t *executor, player_t* player, int* state, int area)
{
    int maxstate = -1;
    int len = 1;
    int *tothrow = NULL;
    tothrow = (int*)malloc(sizeof(int));
    if(area & 1) for(int i = 0; i <= player->cardamount - 1; i++)
    {
        /*优先级变高,重置tothrow,此时只有1个元素*/
        if(StateCompareAi(state, player->card[i]) > maxstate)
        {
            maxstate = StateCompareAi(state, player->card[i]);
            int *ptemp = tothrow;
            tothrow = (int*)realloc(ptemp, sizeof(int));
            tothrow[0] = player->card[i];
            len = 1;
        }
        /*优先级相等,增加1个元素*/
        else if(StateCompareAi(state, player->card[i]) == maxstate)
        {
            int *ptemp = tothrow;
            tothrow = (int*)realloc(ptemp, ++len * sizeof(int));
            tothrow[len - 1] = player->card[i];
        }
    }
    if(area & 2) for(int i = 0; i <= 3; i++)
    {
        if(StateCompareAi(state, player->equips[i] | 0x100) > maxstate)
        {
            maxstate = StateCompareAi(state, player->equips[i] | 0x100);
            int *ptemp = tothrow;
            tothrow = (int*)realloc(ptemp, sizeof(int));
            tothrow[0] = player->equips[i] | 0x100;
            len = 1;
        }
        else if(StateCompareAi(state, player->equips[i] | 0x100) == maxstate)
        {
            realloc(tothrow, ++len * sizeof(int));
            tothrow[len - 1] = player->equips[i] | 0x100;
        }
    }
    if(area & 4) for(int i = 0; i <= 2; i++)
    {
        if(StateCompareAi(state, player->judges[i][0] | 0x200) > maxstate)
        {
            maxstate = StateCompareAi(state, player->judges[i][0] | 0x200);
            int *ptemp = tothrow;
            tothrow = (int*)realloc(ptemp, sizeof(int));
            tothrow[0] = player->judges[i][0] | 0x200;
            len = 1;
        }
        else if(StateCompareAi(state, player->judges[i][0] | 0x200) == maxstate)
        {
            realloc(tothrow, ++len * sizeof(int));
            tothrow[len - 1] = player->judges[i][0] | 0x200;
        }
    }
    int sel = rand() % len;
    switch(sel >> 16)
    {
    case 0:
    {
        executor->cardamount++;
        executor->card[executor->cardamount - 1] = player->card[sel];
        card_inf[player->card[sel]].owner = executor->id;
        player->card[sel] = -1;
        player->cardamount--;
        IndexAlign(player->card, player->cardamount, 160);
        break;
    }
    case 1:
    {
        executor->cardamount++;
        executor->card[executor->cardamount - 1] = player->equips[sel & 0xFF];
        card_inf[player->equips[sel & 0xFF]].owner = executor->id;
        player->equips[sel & 0xFF] = -1;
        break;
    }
    case 2:
    {
        executor->cardamount++;
        executor->card[executor->cardamount - 1] = player->judges[sel & 0xFF][0];
        card_inf[player->judges[sel & 0xFF][0]].owner = executor->id;
        player->judges[sel & 0xFF][0] = -1;
        player->judges[sel & 0xFF][1] = -1;
        for(int j = 0; j <= 1; j++)
        {
            int temp[3];
            for(int k = 0; k <= 2; k++) temp[k] = player->judges[k][j];
            IndexAlign(temp, ArrayOccupied(temp, 3), 3);
            for(int k = 0; k <= 2; k++) player->judges[k][j] = temp[k];
        }
        break;
    }
    }
    free(tothrow);
}

//弃牌权重比对,返回值叫大者优先弃置
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
