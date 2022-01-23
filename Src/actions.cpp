#include "actions.h"

#include <string.h>
#include <stdlib.h>
#include <malloc.h>

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
        card[i] = pointer;
        temp[pointer] = 1;
    }

    nowpile = 160;
}

//洗牌
void Shuffle(void)
{
    int pilecards = 0;
    int pointer;
    int* temp = NULL;
    temp = (int*)calloc(pilecards, sizeof(int));
    memset(temp, 0, sizeof(int) * pilecards);

    //统计弃牌堆内牌的数量
    for(int i = 0; i <= 150; i++)
    {
        if(card_inf[i].owner == -1) pilecards++;
    }

    //此部分与Pileinit类似
    for(int i = 0; i <= pilecards - 1; i++)
    {
        pointer = rand() % 160;
        while(temp[pointer]) ++pointer %= pilecards;
        card[i] = pointer;
        temp[pointer] = 1;

    }

    nowpile = pilecards;
    free(temp);
}

//摸牌
void Takecard(player_t player, int amount)
{
    for(int i = 1; i <= amount; i++)
    {
        if(card_inf[0].owner == -1) Shuffle();
        card_inf[card[0]].owner = player.id;
        player.card[player.cardamount] = card[0];
        player.cardamount++;
        nowpile--;

        for(int j = 0; j <= nowpile - 1; j++)
        {
            if(j != 159)card[j] = card[j + 1];
            else card[159] = -1;
        }
    }
}
