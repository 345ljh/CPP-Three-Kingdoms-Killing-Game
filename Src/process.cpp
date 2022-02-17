#include "process.h"

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
    }

    for(int i = 0; i <= 3; i++)
    {
        Takecard(&player[i], 4);
    }

    game.turn = 1;
    game.active = 0;
    game.period = 0;
    game.page = 0;

    DrawGui();

}

void GameRun(void)
{
    ///Remember to DrawGui after EVERY change!!!
    while(1)
    {
        if(player[game.active].controller != DEAD)
        {
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
                if(player[game.active].judges[i][1] != -1 && !AskWuxie(game.active, player[game.active].judges[i][1]))
                {
                    switch ((type_e)player[game.active].judges[i][1])
                    {
                    case LE:
                        {
                            if(card_inf[Judging(&player[game.active])].suit != HEART) skip |= 2;
                            break;
                        }
                    case BING:
                        {
                            if(card_inf[Judging(&player[game.active])].suit != CLUB) skip |= 1;
                            break;
                        }
                    case SHANDIAN:
                        {
                            if(card_inf[Judging(&player[game.active])].suit == SPADE &&
                               card_inf[Judging(&player[game.active])].num >= 2 && card_inf[Judging(&player[game.active])].num <= 9)
                            Damage(NULL, &player[game.active], 3, THUNDER);
                            break;
                        }
                    default:;
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
                Takecard(&player[game.active], player[game.active].takecard + (game.turn == 1 ? (game.active == 3) - !game.active : 0));
                DrawGui();
            }

            //出牌阶段
            delay_fps(4);
            game.period = 3;
            if(!(skip & 2))
            {
                /* use card */
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

        //轮数与当前回合角色更新
        (++game.active %= 4) ? : game.turn++;
    }
}
