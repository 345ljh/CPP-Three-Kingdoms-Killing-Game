#include "process.h"

void GameStart(void)
{
    //初始化
    for(int i = 0; i <= 3; i++)
    {
        player[i].id = i;

        player[i].maxhealth = general_inf[player[i].general].maxhealth;
        player[i].health = player[i].maxhealth;
        player[i].maxcard = player[i].health;
        player[i].cardamount = 0;

        player[i].nowslash = 0;
        player[i].maxslash = 1;
        player[i].slashlimit = 1;
        player[i].spirits = 1;
        player[i].targets = 1;

        player[i].chained = 0;
        player[i].turned = 0;

        player[i].activeskill = strcmp(general_inf[player[i].general].activeskill, "");
        player[i].limit = 0;
        player[i].awaken = 0;

        memset(player[i].card, 0xFF, sizeof(player[i].card));
        memset(player[i].equips, 0xFF, sizeof(player[i].equips));
        memset(player[i].judges, 0xFF, sizeof(player[i].judges));
        memset(player[i].outcard, 0xFF, sizeof(player[i].outcard));

        memset(player[i].temp, 0, sizeof(player[i].temp));
        memset(player[i].other, 0, sizeof(player[i].other));
    }

    for(int i = 0; i <= 3; i++)
    {
        Takecard(&player[i], 4);
    }

    game.turn = 0;
    game.page = 0;
    DrawGui();
}
