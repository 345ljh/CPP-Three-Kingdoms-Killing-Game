#include "process.h"

void GameStart(void)
{
    //初始化
    for(int i = 0; i <= 3; i++)
    {
        player[i].maxhealth = general_inf[player[i].general].maxhealth;
        player[i].health = player[i].maxhealth;
        player[i].maxcard = player[i].health;
        player[i].cardamount = 0;
        player[i].nowslash = 0;
        player[i].maxslash = 0;
        player[i].slashlimit = 1;
        player[i].spirits = 0;
        player[i].chained = 0;
        player[i].turned = 0;
        player[i].limitused = general_inf[player[i].general].limit;
        player[i].awaken = 0;
        player[i].range = 1;

        memset(player[i].card, 0, sizeof(player[i].card));
        memset(player[i].equips, 0, sizeof(player[i].equips));
        memset(player[i].judges, 0, sizeof(player[i].judges));
    }

    for(int i = 0; i <= 3; i++)
    {
        Takecard(player[i], 4);
    }
}
