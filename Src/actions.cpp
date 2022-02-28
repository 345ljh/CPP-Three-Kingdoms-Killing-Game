/**
 * @file    actions.cpp
 * @brief   游戏过程中具体行为
 */

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
    for(int i = 0; i <= 160; i++)
    {
        if(card_inf[i].owner == -1) pilecards++;
    }

    //将洗过的牌接在牌堆后
    for(int i = game.nowpile; i <= pilecards - 1; i++)
    {
        pointer = rand() % 160;
        while(card_inf[pointer].owner != -1) ++pointer %= 160;
        game.card[i] = pointer;
        card_inf[pointer].owner = -2;
    }

    game.nowpile += pilecards;
}

//出牌阶段
void Playcard(player_t *executor)
{
    if(executor->controller == HUMAN)
    {
        for(; is_run(); delay_fps(10))
        {
            while (mousemsg()) msg = getmouse();
            mousepos(&mouse_x, &mouse_y);

            cleardevice(gui.selector);

            setcolor(WHITE, gui.selector);
            setfont(30, 0, "仿宋", gui.selector);
            outtextxy(350, 415, (char*)"请选择一张卡牌或点击\"取消\"结束", gui.selector);

            //取消键
            LineRect(960, 540, 1050, 565, EGERGB(255, 215, 77), gui.selector);

            //选择手牌
            if(msg.is_down() && mouse_x >= 150 && mouse_x <= 950 && mouse_y >= 450 && mouse_y <= 600)
            {
                cleardevice(gui.selector);
                DrawGui();

                int sel = (mouse_x - 150) / 100 + game.page * 8;
                if(sel < executor->cardamount)
                {
                    LineRect(160 + 100 * (sel % 8), 465, 240 + 100 * (sel % 8), 585, EGERGB(255, 57, 57), gui.selector);

                    //判断所选类型
                    //出牌阶段一般只能指定自己的牌
                    if(( (int)card_inf[executor->card[sel]].type >= 0x10 && (int)card_inf[executor->card[sel]].type < 0x90) ||  //装备
                       (card_inf[executor->card[sel]].type == TAO && executor->health < executor->maxhealth) ||  //桃
                       (card_inf[executor->card[sel]].type == JIU && executor->spirits == 0) ||  //酒
                       (card_inf[executor->card[sel]].type == WUZHONG) ||  //无中生有
                       (card_inf[executor->card[sel]].type == SHANDIAN && (type_e)executor->judges[0][1] != SHANDIAN && (type_e)executor->judges[1][1] != SHANDIAN && (type_e)executor->judges[2][1] != SHANDIAN) )  //闪电
                    {
                        for(; is_run(); delay_fps(10))
                        {
                            while (mousemsg()) msg = getmouse();
                            mousepos(&mouse_x, &mouse_y);

                            char str[121] = "";
                            if((int)card_inf[executor->card[sel]].type >= 0x10 && (int)card_inf[executor->card[sel]].type < 0x90) strcpy(str, Link((char*)"装备", card_inf[executor->card[sel]].name));
                            if(card_inf[executor->card[sel]].type == TAO) strcpy(str, "使用桃回复1点体力");
                            if(card_inf[executor->card[sel]].type == JIU) strcpy(str, "使用酒，本阶段下一张杀的伤害+1");
                            if(card_inf[executor->card[sel]].type == WUZHONG) strcpy(str, "使用无中生有");
                            if(card_inf[executor->card[sel]].type == SHANDIAN) strcpy(str, "使用闪电");
                            outtextxy(600 - 7.5 * strlen(str), 415, str, gui.selector);

                            LineRect(960, 510, 1050, 535, EGERGB(255, 215, 77), gui.selector);
                            LineRect(960, 540, 1050, 565, EGERGB(255, 215, 77), gui.selector);

                            putimage_transparent(NULL, gui.selector, 0, 0, BLACK);

                            if(msg.is_down() && mouse_x >= 960 && mouse_x <= 1050 && mouse_y >= 510 && mouse_y <= 535)
                            {
                                if( !( (int)card_inf[executor->card[sel]].type >= 0x10 && (int)card_inf[executor->card[sel]].type < 0x90) ) card_inf[executor->card[sel]].owner = -1;
                                int temp = executor->card[sel];
                                executor->card[sel] = -1;
                                executor->cardamount--;
                                IndexAlign(executor->card, executor->cardamount, 160);
                                Execard(executor, 1 << executor->id, temp);
                                goto Circ;
                            }
                            if(msg.is_down() && mouse_x >= 960 && mouse_x <= 1050 && mouse_y >= 540 && mouse_y <= 565) goto Circ;
                        }
                    }
                    //杀
                    if(card_inf[executor->card[sel]].type == SHA || card_inf[executor->card[sel]].type == HUOSHA || card_inf[executor->card[sel]].type == LEISHA)
                    {
                        for(; is_run(); delay_fps(10))
                        {
                            //判断次数限制
                            if(executor->slashlimit && executor->equips[0] != ZHUGE && executor->nowslash >= executor->maxslash) goto Circ;

                            //计算合法目标
                            int allowtar = 15 ^ (1 << executor->id);  //除去自己
                            for(int i = 1; i <= 3; i++)
                            {
                                //对位且两侧有间隔时基础距离为2,否则为1
                                int distance = (i == 2 && player[(executor->id + 1) % 4].controller != DEAD && player[(executor->id + 3) % 4].controller != DEAD) ? 2 : 1;
                                //+1与-1马的计算
                                distance += player[(executor->id + i) % 4].equips[3] != -1;
                                distance -= executor->equips[2] != -1;
                                //比较攻击范围
                                int range = executor->equips[0] != -1 ? (int)card_inf[executor->equips[0]].type >> 4 : 1;
                                if(distance > range) allowtar &= (15 ^ (1 << ((executor->id + i) % 4)));
                            }

                            int tar = SelectTarget(allowtar, executor->targets);
                            if(tar)
                            {
                                card_inf[executor->card[sel]].owner = -1;
                                int temp = executor->card[sel];
                                executor->card[sel] = -1;
                                executor->cardamount--;
                                IndexAlign(executor->card, executor->cardamount, 160);
                                Execard(executor, tar, temp);
                            }
                            goto Circ;
                        }
                    }
                    //决斗
                    if(card_inf[executor->card[sel]].type == JUEDOU)
                    {
                        for(; is_run(); delay_fps(10))
                        {
                            int tar = SelectTarget(15 ^ (1 << game.humanid), 1);
                            if(tar)
                            {
                                card_inf[executor->card[sel]].owner = -1;
                                int temp = executor->card[sel];
                                executor->card[sel] = -1;
                                executor->cardamount--;
                                IndexAlign(executor->card, executor->cardamount, 160);
                                Execard(executor, tar, temp);
                            }
                            goto Circ;
                        }
                    }
                    //过拆
                    if(card_inf[executor->card[sel]].type == GUOCHAI)
                    {
                        for(; is_run(); delay_fps(10))
                        {
                            //计算合法目标
                            int allowtar = 15 ^ (1 << executor->id);
                            for(int i = 1; i <= 3; i++)
                            {
                                //判断目标区域是否无牌,其中判定区只需检测下标=0是否为空即可
                                if(!player[(executor->id + i) % 4].cardamount && !ArrayOccupied(player[(executor->id + i) % 4].equips, 4) && !player[(executor->id + i) % 4].judges[0][0])
                                    allowtar &= (15 ^ (1 << ((executor->id + i) % 4)));
                            }

                            int tar = SelectTarget(allowtar, 1);
                            if(tar)
                            {
                                card_inf[executor->card[sel]].owner = -1;
                                int temp = executor->card[sel];
                                executor->card[sel] = -1;
                                executor->cardamount--;
                                IndexAlign(executor->card, executor->cardamount, 160);
                                Execard(executor, tar, temp);
                            }
                            goto Circ;
                        }
                    }
                    //顺牵
                    if(card_inf[executor->card[sel]].type == SHUNQIAN)
                    {
                        for(; is_run(); delay_fps(10))
                        {
                            //计算合法目标
                            int allowtar = 15 ^ (1 << executor->id);
                            for(int i = 1; i <= 3; i++)
                            {
                                //判断目标区域是否无牌,其中判定区只需检测下标=0是否为空即可
                                if(!player[(executor->id + i) % 4].cardamount && !ArrayOccupied(player[(executor->id + i) % 4].equips, 4) && !player[(executor->id + i) % 4].judges[0][0])
                                    allowtar &= (15 ^ (1 << ((executor->id + i) % 4)));

                                //距离计算
                                int distance = (i == 2 && player[(executor->id + 1) % 4].controller != DEAD && player[(executor->id + 3) % 4].controller) ? 2 : 1;
                                distance += player[(executor->id + i) % 4].equips[3] != -1;
                                distance -= executor->equips[2] != -1;
                                if(distance > 1) allowtar &= (15 ^ (1 << ((executor->id + i) % 4)));
                            }

                            int tar = SelectTarget(allowtar, 1);
                            if(tar)
                            {
                                card_inf[executor->card[sel]].owner = -1;
                                int temp = executor->card[sel];
                                executor->card[sel] = -1;
                                executor->cardamount--;
                                IndexAlign(executor->card, executor->cardamount, 160);
                                Execard(executor, tar, temp);
                            }
                            goto Circ;
                        }
                    }
                    //火攻
                    if(card_inf[executor->card[sel]].type == HUOGONG)
                    {
                        for(; is_run(); delay_fps(10))
                        {

                            //判断目标是否有手牌
                            int allowtar = 15;
                            for(int i = 0; i <= 3; i++) if(!player[i].cardamount) allowtar &= (15 ^ (1 << ((executor->id + i) % 4)));
                            int tar = SelectTarget(allowtar, 1);
                            if(tar)
                            {
                                card_inf[executor->card[sel]].owner = -1;
                                int temp = executor->card[sel];
                                executor->card[sel] = -1;
                                executor->cardamount--;
                                IndexAlign(executor->card, executor->cardamount, 160);
                                Execard(executor, tar, temp);
                            }
                            goto Circ;
                        }
                    }
                    //铁索
                    if(card_inf[executor->card[sel]].type == TIESUO)
                    {
                        for(; is_run(); delay_fps(10))
                        {

                            //判断目标是否有手牌
                            int allowtar = 15;
                            int tar = SelectTarget(allowtar, 2, 0x9A);

                            card_inf[executor->card[sel]].owner = -1;
                            int temp = executor->card[sel];
                            executor->card[sel] = -1;
                            executor->cardamount--;
                            IndexAlign(executor->card, executor->cardamount, 160);
                            Execard(executor, tar, temp);

                            goto Circ;
                        }
                    }
                    //群体锦囊,如万箭,南蛮,桃园,五谷
                    if(card_inf[executor->card[sel]].type == WANJIAN || card_inf[executor->card[sel]].type == NANMAN ||
                        card_inf[executor->card[sel]].type == TAOYUAN || card_inf[executor->card[sel]].type == WUGU)
                    {
                        for(; is_run(); delay_fps(10))
                        {
                            while (mousemsg()) msg = getmouse();
                            mousepos(&mouse_x, &mouse_y);

                            char str[121] = "";
                            if(card_inf[executor->card[sel]].type == WANJIAN) strcpy(str, "使用万箭齐发");
                            if(card_inf[executor->card[sel]].type == NANMAN) strcpy(str, "使用南蛮入侵");
                            if(card_inf[executor->card[sel]].type == TAOYUAN) strcpy(str, "使用桃园结义");
                            if(card_inf[executor->card[sel]].type == WUGU) strcpy(str, "使用五谷丰登");

                            outtextxy(600 - 7.5 * strlen(str), 415, str, gui.selector);

                            LineRect(960, 510, 1050, 535, EGERGB(255, 215, 77), gui.selector);
                            LineRect(960, 540, 1050, 565, EGERGB(255, 215, 77), gui.selector);

                            putimage_transparent(NULL, gui.selector, 0, 0, BLACK);

                            if(msg.is_down() && mouse_x >= 960 && mouse_x <= 1050 && mouse_y >= 510 && mouse_y <= 535)
                            {
                                int tar;
                                if(card_inf[executor->card[sel]].type == WANJIAN || card_inf[executor->card[sel]].type == NANMAN) tar = 15 ^ (1 << executor->id);
                                if(card_inf[executor->card[sel]].type == TAOYUAN || card_inf[executor->card[sel]].type == WUGU) tar = 15;

                                card_inf[executor->card[sel]].owner = -1;
                                int temp = executor->card[sel];
                                executor->card[sel] = -1;
                                executor->cardamount--;
                                IndexAlign(executor->card, executor->cardamount, 160);
                                Execard(executor, tar, temp);
                                goto Circ;
                            }
                            if(msg.is_down() && mouse_x >= 960 && mouse_x <= 1050 && mouse_y >= 540 && mouse_y <= 565) goto Circ;
                        }
                    }
                    //借刀
                    if(card_inf[executor->card[sel]].type == JIEDAO)
                    {
                        for(; is_run(); delay_fps(10))
                        {
                            //选择出杀目标
                            int allowtar1 = 15 ^ (1 << game.humanid);

                            //判断是否有武器
                            for(int i = 1; i <= 3; i++) if(player[i].equips[0] == -1) allowtar1 &= (15 ^ (1 << ((executor->id + i) % 4)));
                            int tar1 = SelectTarget(allowtar1, 1, 0x9B);  //TODO;

                            if(tar1)
                            {
                                //选择被杀目标
                                int allowtar2 = 15 ^ tar1;
                                for(int i = 1; i <= 3; i++)
                                {
                                    //对位且两侧有间隔时基础距离为2,否则为1
                                    int distance = (i == 2 && player[( (int)(log(tar1) / log(2)) + 1) % 4].controller != DEAD && player[( (int)(log(tar1) / log(2)) + 3) % 4].controller != DEAD) ? 2 : 1;
                                    //+1与-1马的计算
                                    distance += player[( (int)(log(tar1) / log(2)) + i) % 4].equips[3] != -1;
                                    distance -= player[ (int)(log(tar1) / log(2))].equips[2] != -1;
                                    //比较攻击范围
                                    int range = player[ (int)(log(tar1) / log(2))].equips[0] != -1 ? (int)card_inf[player[ (int)(log(tar1) / log(2))].equips[0]].type >> 4 : 1;
                                    if(distance > range) allowtar2 &= (15 ^ (1 << (( (int)(log(tar1) / log(2)) + i) % 4)));
                                }

                                int tar2 = SelectTarget(allowtar2, 1, 0x9C);
                                if(tar2)
                                {
                                    card_inf[executor->card[sel]].owner = -1;
                                    int temp = executor->card[sel];
                                    executor->card[sel] = -1;
                                    executor->cardamount--;
                                    IndexAlign(executor->card, executor->cardamount, 160);
                                    Execard(executor, (tar2 << 4) + tar1, temp);  //低4位为目标1,高4位为目标2
                                    break;
                                }
                            }

                            if(msg.is_down() && mouse_x >= 960 && mouse_x <= 1050 && mouse_y >= 540 && mouse_y <= 565) goto Circ;
                        }
                    }
                    //乐不思蜀
                    if(card_inf[executor->card[sel]].type == LE)
                    {
                        for(; is_run(); delay_fps(10))
                        {
                            //计算合法目标
                            int allowtar = 15 ^ (1 << executor->id);
                            for(int i = 1; i <= 3; i++)
                            {
                                //判定区已有乐不思蜀
                                if( (type_e)player[(executor->id + 1) % 4].judges[0][1] == LE || (type_e)player[(executor->id + 1) % 4].judges[1][1] == LE ||
                                   (type_e)player[(executor->id + 1) % 4].judges[2][1] == LE)
                                    allowtar &= (15 ^ (1 << ((executor->id + i) % 4)));
                            }

                            int tar = SelectTarget(allowtar, 1);
                            if(tar)
                            {
                                card_inf[executor->card[sel]].owner = -1;
                                int temp = executor->card[sel];
                                executor->card[sel] = -1;
                                executor->cardamount--;
                                IndexAlign(executor->card, executor->cardamount, 160);
                                Execard(executor, tar, temp);
                            }
                            goto Circ;
                        }
                    }
                    //兵粮寸断
                    if(card_inf[executor->card[sel]].type == BING)
                    {
                        for(; is_run(); delay_fps(10))
                        {
                            //计算合法目标
                            int allowtar = 15 ^ (1 << executor->id);
                            for(int i = 1; i <= 3; i++)
                            {
                                //距离计算
                                int distance = (i == 2 && player[(executor->id + 1) % 4].controller != DEAD && player[(executor->id + 3) % 4].controller) ? 2 : 1;
                                distance += player[(executor->id + i) % 4].equips[3] != -1;
                                distance -= executor->equips[2] != -1;
                                if(distance > 1) allowtar &= (15 ^ (1 << ((executor->id + i) % 4)));

                                //判定区已有兵粮寸断
                                if( (type_e)player[(executor->id + 1) % 4].judges[0][1] == BING || (type_e)player[(executor->id + 1) % 4].judges[1][1] == BING ||
                                        (type_e)player[(executor->id + 1) % 4].judges[2][1] == BING)
                                    allowtar &= (15 ^ (1 << ((executor->id + i) % 4)));
                            }

                            int tar = SelectTarget(allowtar, 1);
                            if(tar)
                            {
                                card_inf[executor->card[sel]].owner = -1;
                                int temp = executor->card[sel];
                                executor->card[sel] = -1;
                                executor->cardamount--;
                                IndexAlign(executor->card, executor->cardamount, 160);
                                Execard(executor, tar, temp);
                            }
                            goto Circ;
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

            //取消
            if(msg.is_down() && mouse_x >= 960 && mouse_x <= 1050 && mouse_y >= 540 && mouse_y <= 565)
            {
                return;
            }

            //使用goto而非break退出循环,防止选择牌后点"取消"会连续触发按钮导致出牌阶段结束,以及"空气杀"的bug
            if(0) Circ: delay_fps(2);

            DrawGui();
            putimage_transparent(NULL, gui.selector, 0, 0, BLACK);

        }
    }
    else
    {
        ;//TODO: AI
    }
    return;
}

//执行一张牌的效果
///此定义中未涉及"视为使用"或将多张当做一张使用
///type为执行牌的类型,若未转化牌则为默认值-1
void Execard(player_t *executor, int target, int id, int type)
{
    delay_fps(5);
    cleardevice(gui.selector);
    DrawGui();
    type = (type == -1) ? (int)card_inf[id].type : type;
    for(int i = 0; i <= 3; i++) if(player[i].controller == DEAD) target &= 15 ^ (1 << i);

    //装备牌
    if(type >= 0x10 && type < 0x90)
    {
        int index = type < 0x60 ? 0 : (type >> 4) - 5;

        //原有装备
        if(executor->equips[index] != -1)
        {
            card_inf[executor->equips[index]].owner = -1;
            Putcard(executor->equips[index]);
            if(index == 1 && (type_e)executor->equips[index] == BAIYIN) Recover(executor, 1);  //白银狮子效果
            executor->equips[index] = -1;
        }

        printf("%s装备", general_inf[executor->general].name);
        Printcard(id);
        printf("\n");

        executor->equips[index] = id;
        card_inf[id].owner = executor->id;
    }
    //杀
    if(type == SHA || type == HUOSHA || type == LEISHA)
    {
        //酒效果
        int basdamage = 1;
        if(executor->spirits == 1)
        {
            basdamage++;
            executor->spirits = 2;
        }
        //回合内计算次数
        if(game.active == executor->id) executor->nowslash++;

        printf("%s对", general_inf[executor->general].name);
        int printed = 0;
        for(int i = 1; i <= 3; i++)
            if(target & (1 << (executor->id + i) % 4))
            {
                if(printed++) printf(",");
                printf("%s", general_inf[player[(executor->id + i) % 4].general].name);
            }
        printf("使用");
        Printcard(id);
        printf("\n");
        Putcard(id);

        for(int i = 1; i <= 3; i++)
        {
            if(target & (1 << (executor->id + i) % 4) )
            {
                //仁王盾效果
                if( (type_e)player[(executor->id + i) % 4].equips[1] == RENWANG && (int)card_inf[id].suit >> 1 == 0)
                {
                    printf("%s发动了\"仁王盾\"\n", general_inf[player[(executor->id + i) % 4].general].name);
                    continue;
                }
                //藤甲效果
                if( (type_e)player[(executor->id + i) % 4].equips[1] == TENGJIA && card_inf[id].type == SHA)
                {
                    printf("%s发动了\"藤甲\"\n", general_inf[player[(executor->id + i) % 4].general].name);
                    continue;
                }
                //询问闪
                if(Askcard(&player[(executor->id + i) % 4], SHAN, 0)) continue;
                //造成伤害
                Damage(executor, &player[(executor->id + i) % 4], basdamage, (damage_e)card_inf[id].type, 1);
            }
        }
    }
    //非濒死使用桃
    if((type_e)type == TAO)
    {
        printf("%s使用", general_inf[executor->general].name);
        Printcard(id);
        printf("\n");

        Recover(executor, 1);
    }
    //非濒死使用酒
    if((type_e)type == JIU)
    {
        printf("%s使用", general_inf[executor->general].name);
        Printcard(id);
        printf("\n");

        executor->spirits = 1;
    }
    //普通锦囊
    if(type >= 0x90 && type < 0xA0)
    {
        //单体锦囊,如决斗,过河拆桥,顺手牵羊,火攻
        if((type_e)type == JUEDOU || (type_e)type == GUOCHAI || (type_e)type == SHUNQIAN || (type_e)type == HUOGONG)
        {
            cleardevice(gui.selector);
            DrawGui();

            printf("%s对", general_inf[executor->general].name);
            int printed = 0;
            for(int i = 1; i <= 3; i++)
                if(target & (1 << (executor->id + i) % 4))
                {
                    if(printed++) printf(",");
                    printf("%s", general_inf[player[(executor->id + i) % 4].general].name);
                }
            printf("使用");
            Printcard(id);
            printf("\n");
            Putcard(id);

            for(int i = 0; i <= 3; i++)
            {
                if( (target & (1 << (executor->id + i) % 4)) && !AskWuxie(executor->id, (executor->id + i) % 4) )
                {
                    if((type_e)type == JUEDOU)
                    {
                        int turn = -1;  //为偶目标出杀,为奇使用者出杀
                        do
                        {
                            turn++;
                        }
                        while(Askcard( (turn % 2 ? executor : &player[(executor->id + i) % 4]), SHA, 0x90));
                        if(turn % 2) Damage(&player[(executor->id + i) % 4], executor, 1, COMMON);
                        else Damage(executor, &player[(executor->id + i) % 4], 1, COMMON);
                    }
                    if((type_e)type == GUOCHAI) Throwcard(executor, &player[(executor->id + i) % 4], 1, 7);
                    if((type_e)type == SHUNQIAN) Getcard(executor, &player[(executor->id + i) % 4], 1, 7);
                    if((type_e)type == HUOGONG)
                    {
                        if(player[(executor->id + i) % 4].cardamount == 0) continue;
                        int suit = Showcard(&player[(executor->id + i) % 4], &player[(executor->id + i) % 4], 1, 0, 0x99);
                        if(Throwcard(executor, executor, 1, 1, 0b11110000 | (1 << suit), 0x99)) Damage(executor, &player[(executor->id + i) % 4], 1, FIRE, 1);
                    }
                }
            }
        }
        //铁索连环
        if((type_e)type == TIESUO)
        {
            if(target)
            {
                printf("%s对", general_inf[executor->general].name);
                int printed = 0;
                for(int i = 1; i <= 3; i++)
                    if(target & (1 << (executor->id + i) % 4))
                    {
                        if(printed++) printf(",");
                        printf("%s", general_inf[player[(executor->id + i) % 4].general].name);
                    }
                printf("使用");
                Printcard(id);
                printf("\n");
                Putcard(id);

                for(int i = 0; i <= 3; i++)
                {
                    if( (target & (1 << (executor->id + i) % 4)) && !AskWuxie(executor->id, (executor->id + i) % 4))
                    {
                        ++player[(executor->id + i) % 4].chained %= 2;
                        if(player[(executor->id + i) % 4].chained) printf("%s的武将牌横置\n", general_inf[player[(executor->id + i) % 4].general].name);
                        else printf("%s的武将牌重置\n", general_inf[player[(executor->id + i) % 4].general].name);
                    }
                }
            }
            else
            {
                printf("%s重铸", general_inf[executor->general].name);
                Printcard(id);
                printf("\n");
                Putcard(id);
                Drawcard(executor, 1);
            }
        }
        //借刀杀人
        if((type_e)type == JIEDAO)
        {
            int tar1 = (int)(log(target % 16) / log(2));
            int tar2 = (int)(log(target >> 4) / log(2));

            printf("%s对%s使用", general_inf[executor->general].name, general_inf[player[tar1].general].name);
            Printcard(id);
            printf(",杀的目标是%s\n", general_inf[player[tar2].general].name);
            Putcard(id);

            if(!AskWuxie(executor->id, tar1))
            {
                int ans = Askcard(&player[tar1], SHA, 0x9B | (tar2 << 8));
                if(ans != -1) Execard(&player[tar1], 1 << tar2, ans);
                else
                {
                    printf("%s获得%s的", general_inf[executor->general].name, general_inf[player[tar1].general].name);
                    Printcard(player[tar1].equips[0]);
                    printf("\n");

                    card_inf[player[tar1].equips[0]].owner = executor->id;
                    executor->card[executor->cardamount] = player[tar1].equips[0];
                    executor->cardamount++;
                    player[tar1].equips[0] = -1;
                }
            }
        }
        //群体锦囊
        if((type_e)type == WANJIAN || (type_e)type == NANMAN || (type_e)type == TAOYUAN || (type_e)type == WUGU)
        {
            printf("%s使用", general_inf[executor->general].name);
            Printcard(id);
            printf("\n");
            Putcard(id);

            if((type_e)type == WANJIAN)
            {
                for(int i = 1; i <= 3; i++)
                {
                    if(target & (1 << (executor->id + i) % 4))
                    {
                        //藤甲效果
                        if( (type_e)player[(executor->id + i) % 4].equips[1] == TENGJIA)
                        {
                            printf("%s发动了\"藤甲\"\n", general_inf[player[(executor->id + i) % 4].general].name);
                            continue;
                        }
                        if(!AskWuxie(executor->id, (executor->id + i) % 4) && !Askcard(&player[(executor->id + i) % 4], SHAN, 0x93) )
                            Damage(executor, &player[(executor->id + i) % 4], 1, COMMON, 1);
                    }
                }
            }
            if((type_e)type == NANMAN)
            {
                for(int i = 1; i <= 3; i++)
                {
                    if(target & (1 << (executor->id + i) % 4))
                    {
                        //藤甲效果
                        if( (type_e)player[(executor->id + i) % 4].equips[1] == TENGJIA)
                        {
                            printf("%s发动了\"藤甲\"\n", general_inf[player[(executor->id + i) % 4].general].name);
                            continue;
                        }
                        if(!AskWuxie(executor->id, (executor->id + i) % 4) && !Askcard(&player[(executor->id + i) % 4], SHA, 0x94) )
                            Damage(executor, &player[(executor->id + i) % 4], 1, COMMON, 1);
                    }
                }
            }
            if((type_e)type == TAOYUAN)
            {
                for(int i = 0; i <= 3; i++)
                {
                    if(player[(executor->id + i) % 4].health >= player[(executor->id + i) % 4].maxhealth) continue;
                    if(target & (1 << (executor->id + i) % 4) && !AskWuxie(executor->id, (executor->id + i) % 4)) Recover(&player[(executor->id + i) % 4], 1);
                }
            }
            if((type_e)type == WUGU)
            {

                int *totake = NULL;
                int live = 0;
                for(int i = 0; i <= 3; i++) live += player[i].controller != DEAD;
                totake = (int*)calloc(live, sizeof(int));

                //展示牌堆顶等同于现存人数的牌
                if(game.nowpile <= live) Shuffle();
                for(int i = 0; i <= 3; i++)
                {
                    totake[i] = game.card[i];
                    game.card[i] = -1;
                    IndexAlign(game.card, game.nowpile, 160);
                }

                //选牌界面
                PIMAGE wugu = newimage();
                getimage(wugu, 0, 0, 1200, 600);

                setfillcolor(EGERGB(83, 30, 0),wugu);
                bar(595 - 45 * live, 220, 605 + 45 * live, 380, wugu);

                setcolor(EGERGB(249, 189, 34), wugu);
                setfont(20, 0, "隶书", wugu);
                setbkmode(TRANSPARENT, wugu);
                outtextxy(560, 220, (char*)"五谷丰登", wugu);

                printf("五谷丰登展示%d张牌", live);
                for(int i = 0; i <= live - 1; i++)
                {
                    Pastecard(605 - 45 * live + 90 * i, 250, totake[i], wugu);
                    Printcard(totake[i]);
                }
                printf("\n");

                for(int i = 0; i <= 3; i++)
                {
                    DrawGui();
                    if(target & (1 << (executor->id + i) % 4) && !AskWuxie(executor->id, (executor->id + i) % 4))
                    {
                        putimage_transparent(NULL, wugu, 0, 0, BLACK);
                        int sel = -1;

                        //选牌
                        if(player[(executor->id + i) % 4].controller == HUMAN)
                        {
                            for(; is_run(); delay_fps(10))
                            {
                                while (mousemsg()) msg = getmouse();
                                mousepos(&mouse_x, &mouse_y);

                                if(msg.is_down() && mouse_x >= 595 - 45 * live && mouse_x <= 605 + 45 * live && mouse_y >= 250 && mouse_y <= 370)
                                {
                                    sel = (mouse_x - (595 - 45 * live)) / 90;
                                    if(totake[sel] != -1) break;
                                }
                            }
                        }
                        else
                        {
                            sel = WuguAi(&player[(executor->id + i) % 4], totake, live);
                        }
                        delay_fps(1);
                        //获得所选牌
                        printf("%s获得", general_inf[player[(executor->id + i) % 4].general].name);
                        Printcard(totake[sel]);
                        printf("\n");

                        player[(executor->id + i) % 4].card[player[(executor->id + i) % 4].cardamount] = totake[sel];
                        player[(executor->id + i) % 4].cardamount++;
                        card_inf[totake[sel]].owner = (executor->id + i) % 4;

                        setcolor(EGERGB(255, 215, 77), wugu);
                        line(605 - 45 * live + 90 * sel, 250, 685 - 45 * live + 90 * sel, 370, wugu);
                        line(605 - 45 * live + 90 * sel, 370, 685 - 45 * live + 90 * sel, 250, wugu);
                        totake[sel] = -1;

                        putimage_transparent(NULL, wugu, 0, 0, BLACK);
                        delay_ms(0);
                        putimage_transparent(NULL, wugu, 0, 0, BLACK);
                        delay_fps(0.5);

                        setcolor(EGERGB(255, 57, 57), wugu);
                        line(605 - 45 * live + 90 * sel, 250, 685 - 45 * live + 90 * sel, 370, wugu);
                        line(605 - 45 * live + 90 * sel, 370, 685 - 45 * live + 90 * sel, 250, wugu);
                    }
                }

                //将剩余牌弃置
                if(ArrayOccupied(totake, 4))
                {
                    printf("五谷丰登弃置");
                    for(int i = 0; i <= 3; i++)
                    {
                        if(totake[i] != -1)
                        {
                            card_inf[totake[i]].owner = -1;
                            Putcard(totake[i]);
                            Printcard(totake[i]);
                        }
                    }
                    printf("\n");
                }

                free(totake);
            }
        }
        //无中生有
        if((type_e)type == WUZHONG)
        {
            if(!AskWuxie(executor->id, executor->id % 4))
            {
                printf("%s使用", general_inf[executor->general].name);
                Printcard(id);
                printf("\n");

                Drawcard(executor, 2);
            }
        }
    }
    //乐不思蜀,兵粮寸断
    if((type_e)type == LE || (type_e)type == BING)
    {
        int tar = (int)(log(target % 16) / log(2));
        printf("%s对%s使用", general_inf[executor->general].name, general_inf[player[(int)(log(target % 16) / log(2))].general].name);
        Printcard(id);
        printf("\n");

        int judgearea[3];
        for(int i = 0; i <= 2; i++) judgearea[i] = player[tar].judges[i][0];
        player[tar].judges[ArrayOccupied(judgearea, 3)][0] = id;
        player[tar].judges[ArrayOccupied(judgearea, 3)][1] = (int)card_inf[id].type;
    }
    //闪电
    if((type_e)type == SHANDIAN)
    {
        printf("%s使用", general_inf[executor->general].name);
        Printcard(id);
        printf("\n");

        int judgearea[3];
        for(int i = 0; i <= 2; i++) judgearea[i] = executor->judges[i][0];
        executor->judges[ArrayOccupied(judgearea, 3)][0] = id;
        executor->judges[ArrayOccupied(judgearea, 3)][1] = (int)card_inf[id].type;
    }
    DrawGui();
}

//从牌堆顶摸牌
void Drawcard(player_t *recipient, int amount)
{
    if(game.nowpile <= amount) Shuffle();

    printf("%s从牌堆获得", general_inf[recipient->general].name);
    for(int i = 1; i <= amount; i++)
    {
        card_inf[game.card[0]].owner = recipient->id;
        recipient->card[recipient->cardamount] = game.card[0];
        recipient->cardamount++;

        if(recipient->controller == HUMAN) Printcard(game.card[0]);

        game.nowpile--;
        game.card[0] = -1;
        IndexAlign(game.card, game.nowpile, 160);
    }
    if(recipient->controller == AI) printf("%d张牌", amount);
    printf("\n");
}

//弃牌,其中executor为弃牌者,recipient为被弃牌者
///当executor与recipient均为玩家时返回值为弃置牌数,否则为0
///area表示可选其牌区域,0位为1表示手牌区,1位为装备区,2位为判定区,默认值为1(即只可弃置手牌)
///mode为8位整数,0~3位为可弃置的花色(对应suit_e).4~6位为可弃置的类型(低至高分别为基本,装备,锦囊),7位为cancel变量,默认为127(0b01111111)
///cancel=1时可点击取消,此时返回值为0,amount为最大弃牌数
///cancel=0时无法取消,且当区域内牌不足时则全弃
///mode当且仅当executor与recipient为同一角色时生效
/** add的值
 * 0: 默认
 * 0x99: 火攻使用者
 */

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
                char str[121] = "";
                if(add == 0) strcpy(str, Link( Link( Link( Link( (char*)"弃置", Myitoa(ArrayOccupied(tothrow, amount) )), (char*)"/"), MyitoaII(amount)), (char*)"张牌"));
                if(add == 0x99)
                {
                    char suitstr[11] = "";
                    if(suit == 1) strcpy(suitstr, "黑桃");
                    else if(suit == 2) strcpy(suitstr, "梅花");
                    else if(suit == 4) strcpy(suitstr, "红桃");
                    else if(suit == 8) strcpy(suitstr, "方片");
                    strcpy(str, Link( Link((char*)"弃置一张", suitstr), (char*)"牌，造成1点火焰伤害"));
                }
                setcolor(WHITE, gui.selector);
                setfont(30, 0, "仿宋", gui.selector);
                outtextxy(600 - 7.5 * strlen(str), 415, str, gui.selector);

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
                    int baiyin = 0;  //白银狮子效果
                    if(amount) printf("%s弃置%d张牌", general_inf[recipient->general].name, amount);
                    for(int i = 0; i <= amount - 1; i++)
                    {
                        if(tothrow[i] != -1)
                            switch(tothrow[i] >> 8)
                            {
                            case 0:
                            {
                                card_inf[recipient->card[tothrow[i]]].owner = -1;
                                Putcard(recipient->card[tothrow[i]]);
                                Printcard(recipient->card[tothrow[i]]);
                                recipient->card[tothrow[i]] = -1;
                                recipient->cardamount--;
                                break;
                            }
                            case 1:
                            {
                                card_inf[recipient->equips[tothrow[i] & 0xFF]].owner = -1;
                                Putcard(recipient->equips[tothrow[i] & 0xFF]);
                                Printcard(recipient->equips[tothrow[i] & 0xFF]);
                                if( (type_e)recipient->equips[tothrow[i] & 0xFF] == BAIYIN)  baiyin = 1;
                                recipient->equips[tothrow[i] & 0xFF] = -1;
                                break;
                            }
                            case 2:
                            {
                                card_inf[recipient->judges[tothrow[i] & 0xFF][0]].owner = -1;
                                Putcard(recipient->judges[tothrow[i] & 0xFF][0]);
                                Printcard(recipient->judges[tothrow[i] & 0xFF][0]);
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
                    printf("\n");
                    //白银狮子效果
                    if(baiyin) Recover(recipient, 1);
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

                char topic[121] = "";
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
                    Pastecard(425, 175, -2,gui.selector);
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
                        Pastecard(425 + 90 * i, 305, recipient->equips[i], gui.selector);
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
                        Pastecard(515 + 90 * i, 175, recipient->judges[i][0], gui.selector);
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

                        printf("%s弃置%s的", general_inf[executor->general].name, general_inf[recipient->general].name);
                        Printcard(recipient->card[tothrow]);
                        printf("\n");

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

                        printf("%s弃置%s的", general_inf[executor->general].name, general_inf[recipient->general].name);
                        Printcard(recipient->equips[tothrow]);
                        printf("\n");
                        if( (type_e)recipient->equips[tothrow] == BAIYIN)  Recover(recipient, 1);  //白银狮子效果
                        recipient->equips[tothrow] = -1;
                        break;
                    }
                    if( (area & 4) && msg.is_down() && mouse_x >= 515 && mouse_x <= 775 && mouse_y >= 175 && mouse_y <= 295)
                    {
                        tothrow = (mouse_x - 510) / 90;
                        card_inf[recipient->judges[tothrow][0]].owner = -1;
                        Putcard(recipient->judges[tothrow][0]);

                        printf("%s弃置%s的", general_inf[executor->general].name, general_inf[recipient->general].name);
                        Printcard(recipient->judges[tothrow][0]);
                        printf("\n");

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

            if(amount) printf("%s弃置%d张牌", general_inf[recipient->general].name, amount);

            int baiyin = 0;
            for(int times = 0; times <= amount - 1; times++)
            {
                //使用优先级判断弃牌
                int state[12] = {3, 1, 0, 2, 6, 6, 7, 5, 4, 2, 3, 2};
                baiyin = ThrowAi(recipient, state, area, suit, type);

                if(!( (area & 1 ? recipient->cardamount : 0) + (area & 2 ? ArrayOccupied(recipient->equips, 4) : 0) +
                        (area & 4 ? (recipient->judges[0][0] != -1) + (recipient->judges[1][0] != -1) + (recipient->judges[2][0] != -1) : 0) ))
                    return times;
            }
            printf("\n");
            if(baiyin) Recover(recipient, 1);
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

            printf("%s弃置%d张牌", general_inf[recipient->general].name, amount);

            int baiyin = 0;
            for(int times = 1; times <= amount; times++)
            {
                baiyin = ThrowAi(recipient, state, area);

                if(!( (area & 1 ? recipient->cardamount : 0) + (area & 2 ? ArrayOccupied(recipient->equips, 4) : 0) +
                        (area & 4 ? (recipient->judges[0][0] != -1) + (recipient->judges[1][0] != -1) + (recipient->judges[2][0] != -1) : 0) ))
                    return times;
            }
            printf("\n");
            if(baiyin) Recover(recipient, 1);
            return amount;
        }
    }
    return 0;//avoid of warning
}

/** add的值
 * 0: 默认
 * 0x99: 火攻使用者
 */
//展示自己手牌
int Showcard(player_t *executor, player_t *recipient, int amount, int cancel, int add)
{
    if(amount > recipient->cardamount) amount = recipient->cardamount;

    int *toshow = NULL;
    toshow = (int*)calloc(amount, sizeof(int));
    memset(toshow, 0xFF, amount * sizeof(int));

    if(recipient->controller == HUMAN)
    {
        for(; is_run(); delay_fps(10))
        {
            while (mousemsg()) msg = getmouse();
            mousepos(&mouse_x, &mouse_y);

            DrawGui();
            cleardevice(gui.selector);

            setcolor(WHITE, gui.selector);
            setfont(30, 0, "仿宋", gui.selector);
            outtextxy(520, 415, Link( Link( Link( Link( (char*)"选择", Myitoa(ArrayOccupied(toshow, amount) )), (char*)"/"), MyitoaII(amount)), (char*)"张牌展示"), gui.selector);

            //全部绘制为未选定状态
            for(int i = 0; i <= 7; i++)
                if(recipient->card[game.page * 8 + i] != -1) LineRect(160 + 100 * i, 465, 240 + 100 * i, 585, EGERGB(255, 215, 77), gui.selector);

            //绘制已选定状态
            for(int i = 0; i <= amount - 1; i++)
            {
                if(toshow[i] != -1 && recipient->card[toshow[i]] != -1 && toshow[i] >> 8 == 0 && game.page == toshow[i] / 8)
                    LineRect(160 + 100 * (toshow[i] % 8), 465, 240 + 100 * (toshow[i] % 8), 585, EGERGB(255, 57, 57), gui.selector);
            }

            //确定键
            if((cancel && ArrayOccupied(toshow, amount)) || ArrayOccupied(toshow, amount) == amount) LineRect(960, 510, 1050, 535, EGERGB(255, 215, 77), gui.selector);

            //取消键
            if(cancel) LineRect(960, 540, 1050, 565, EGERGB(255, 215, 77), gui.selector);

            //检测按键
            if(msg.is_down() && mouse_x >= 150 && mouse_x <= 950 && mouse_y >= 450 && mouse_y <= 600)
            {
                int sel = (mouse_x - 150) / 100;
                if(recipient->cardamount > game.page * 8 + sel)
                {
                    int found = 0;
                    for(int i = 0; i <= amount - 1; i++)
                    {
                        //若已选中则取消
                        if(toshow[i] == game.page * 8 + sel)
                        {
                            toshow[i] = -1;
                            found++;
                            break;
                        }
                    }

                    //若未选中则选定,将toshow中目前下标最小的-1改为该牌id
                    if(!found && ArrayOccupied(toshow, amount) <= amount)
                    {
                        for(int i = 0; i <= amount - 1; i++)
                        {
                            if(toshow[i] == -1)
                            {
                                toshow[i] = game.page * 8 + sel;
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

            //确定键展示
            if( (cancel || ArrayOccupied(toshow, amount) == amount) && msg.is_down() && mouse_x >= 960 && mouse_x <= 1050 && mouse_y >= 510 && mouse_y <= 535)
            {
                break;
            }
            putimage_transparent(NULL, gui.selector, 0, 0, BLACK);
        }
    }
    //电脑展示,随机展示amount张
    else
    {
        delay_fps(5);

        for(int i = 0; i <= amount - 1; i++)
        {
            int pointer = rand() % recipient->cardamount;
            int rep = 0;

            //检测重复
            do
            {
                rep = 0;
                for(int j = 0; j <= i - 1; j++)
                {
                    if(pointer == toshow[j])
                    {
                        rep = 1;
                        ++pointer %= recipient->cardamount;
                        break;
                    }
                }
            }while(rep);

            toshow[i] = pointer;
        }
    }

    //展示
    cleardevice(gui.selector);
    setfillcolor(EGERGB(83, 30, 0), gui.selector);
    bar( amount >= 3 ? (amount <= 8 ? 590 - 40 * amount : 270) : 470, 200, amount >= 3 ? (amount <= 8 ? 610 + 40 * amount : 930) : 730, 400, gui.selector);

    char topic[121] = "";
    strcpy(topic, Link( Link( Link(general_inf[recipient->general].name, (char*)"展示的"), Myitoa(amount) ), (char*)"张手牌") );
    setcolor(EGERGB(249, 189, 34), gui.selector);
    setfont(20, 0, "隶书", gui.selector);
    outtextxy(600 - strlen(topic) * 5, 200, topic, gui.selector);

    printf("%s展示%d张手牌", general_inf[recipient->general].name, amount);
    for(int i = 0; i <= amount - 1; i++)
    {
        Pastecard( (amount <= 8 ? 600 - 40 * amount + 80 * i : 270 + 240.0 * i / (amount - 1) ), 240, recipient->card[toshow[i]], gui.selector);
        Printcard(recipient->card[toshow[i]]);
    }
    printf("\n");

    putimage_transparent(NULL, gui.selector, 0, 0, BLACK);
    delay_ms(0);
    putimage_transparent(NULL, gui.selector, 0, 0, BLACK);
    delay_fps(0.6);
    if(add == 0) return amount;
    if(add == 0x99) return card_inf[recipient->card[toshow[0]]].suit;
    return 0;
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

                char topic[121] = "";
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
                    Pastecard(425, 175, -2, gui.selector);
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
                        Pastecard(425 + 90 * i, 305, recipient->equips[i], gui.selector);
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
                        Pastecard(515 + 90 * i, 175, recipient->judges[i][0], gui.selector);
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

                        printf("%s获得%s的", general_inf[executor->general].name, general_inf[recipient->general].name);
                        Printcard(recipient->card[tothrow]);
                        printf("\n");

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

                        printf("%s获得%s的", general_inf[executor->general].name, general_inf[recipient->general].name);
                        Printcard(recipient->equips[tothrow]);
                        printf("\n");

                        if( (type_e)recipient->equips[tothrow] == BAIYIN) Recover(recipient, 1);  //白银狮子效果
                        card_inf[recipient->equips[tothrow]].owner = executor->id;
                        recipient->equips[tothrow] = -1;

                        break;
                    }
                    if( (area & 4) && msg.is_down() && mouse_x >= 515 && mouse_x <= 775 && mouse_y >= 175 && mouse_y <= 295)
                    {
                        tothrow = (mouse_x - 510) / 90;
                        executor->cardamount++;
                        executor->card[executor->cardamount - 1] = recipient->judges[tothrow][0];

                        printf("%s获得%s的", general_inf[executor->general].name, general_inf[recipient->general].name);
                        Printcard(recipient->judges[tothrow][0]);
                        printf("\n");

                        card_inf[recipient->judges[tothrow][0]].owner = executor->id;
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

            printf("%s获得%s的", general_inf[executor->general].name, general_inf[recipient->general].name);

            int baiyin = 0;
            for(int times = 1; times <= amount; times++)
            {
                baiyin = GetAi(executor, recipient, state, area);

                //提前结束情形
                if(!( (area & 1 ? recipient->cardamount : 0) + (area & 2 ? ArrayOccupied(recipient->equips, 4) : 0) +
                        (area & 4 ? (recipient->judges[0][0] != -1) + (recipient->judges[1][0] != -1) + (recipient->judges[2][0] != -1) : 0) ))
                {
                    if(executor->controller != HUMAN && recipient->controller != HUMAN) printf("%d张牌", amount);
                    printf("\n");
                    return times;
                }
            }

            if(executor->controller != HUMAN && recipient->controller != HUMAN) printf("%d张牌", amount);
            printf("\n");
            if(baiyin) Recover(recipient, 1);  //白银狮子效果
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
    if(game.nowpile <= 1) Shuffle();

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

    char topic[121] = "";
    strcpy(topic, Link(general_inf[recipient->general].name, (char*)"的判定结果") );
    setcolor(EGERGB(249, 189, 34), gui.selector);
    setfont(20, 0, "隶书", gui.selector);
    outtextxy(120 - strlen(topic) * 5, 0, topic, gui.selector);

    printf("%s的判定结果为", general_inf[recipient->general].name);
    Printcard(res);
    printf("\n");

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
///linkstart=1表示为伤害传导的起点(默认值)
void Damage(player_t *executor, player_t *recipient, int amount, damage_e type, int linkstart)
{
    delay_fps(5);

    if(amount > 0)
    {
        //藤甲效果
        if( (type_e)recipient->equips[1] == TENGJIA && type == FIRE)
        {
            amount++;
            printf("%s发动了\"藤甲\"\n", general_inf[recipient->general].name);
        }
        //白银狮子效果
        if( (type_e)recipient->equips[1] == BAIYIN && type != LOSS && amount > 1)
        {
            amount = 1;
            printf("%s发动了\"白银狮子\"\n", general_inf[recipient->general].name);
        }

        recipient->health -= amount;
        recipient->maxcard -= amount;  //手牌上限一般随体力变化
        DrawGui();

        switch(type)
        {
        case COMMON:
            {
                if(executor) printf("%s受到%s造成的%d点伤害,体力值为%d\n", general_inf[recipient->general].name, general_inf[executor->general].name, amount, recipient->health);
                else printf("%s受到%d点伤害,体力值为%d\n", general_inf[recipient->general].name, amount, recipient->health);
                break;
            }
        case FIRE:
            {
                if(executor) printf("%s受到%s造成的%d点火焰伤害,体力值为%d\n", general_inf[recipient->general].name, general_inf[executor->general].name, amount, recipient->health);
                else printf("%s受到%d点火焰伤害,体力值为%d\n", general_inf[recipient->general].name, amount, recipient->health);
                break;
        }
        case THUNDER:
            {
                if(executor) printf("%s受到%s造成的%d点雷电伤害,体力值为%d\n", general_inf[recipient->general].name, general_inf[executor->general].name, amount, recipient->health);
                else printf("%s受到%d点雷电伤害,体力值为%d\n", general_inf[recipient->general].name, amount, recipient->health);
                break;
        }
        case LOSS:
            {
                printf("%s失去%d点体力,体力值为%d\n", general_inf[recipient->general].name, amount, recipient->health);
                break;
            }
        }

        if(recipient->health <= 0)
        {
            Neardeath(recipient);
        }

        //连锁结算
        if(linkstart && recipient->chained && (type == FIRE || type == THUNDER) )
        {
            recipient->chained = 0;
            printf("%s的武将牌重置", general_inf[recipient->general].name);
            DrawGui();

            for(int i = 1; i <= 3; i++)
                if(player[(recipient->id + i) % 4].chained)
                {
                    Damage(executor, &player[(recipient->id + i) % 4], amount, type, 0);
                    recipient->chained = 0;
                    printf("%s的武将牌重置", general_inf[player[(recipient->id + i) % 4].general].name);
                }
        }
    }
}

//恢复
void Recover(player_t *recipient, int amount)
{
    if(recipient->maxhealth - recipient->health < amount) amount = recipient->maxhealth - recipient->health;
    recipient->health += amount;
    recipient->maxcard += amount;
    printf("%s恢复%d点体力,体力值为%d\n", general_inf[recipient->general].name, amount, recipient->health);
}

/** add的值
 * 0: 默认
 * 0x9A: 铁索
 * 0x9B: 借刀杀人选择出杀者
 * 0x9C: 借刀杀人选择杀的目标
 */
//玩家手动选定目标
///allowed为可选目标,与返回值一样为0~15的整数,0~3位分别对应一至四号位
///maxtarget为最大目标数
int SelectTarget(int allowed, int maxtarget, int add)
{
    int sel = 0;

    //除去死亡目标
    for(int i = 0; i <= 3; i++) if(player[i].controller == DEAD) allowed &= 15 ^ (1 << i);

    for(; is_run(); delay_fps(10))
    {
        while (mousemsg()) msg = getmouse();
        mousepos(&mouse_x, &mouse_y);

        char str[121] = "";
        setcolor(WHITE, gui.selector);
        setfont(30, 0, "仿宋", gui.selector);
        if(add == 0) strcpy(str, Link( Link( (char*)"选择至多", Myitoa(maxtarget) ), (char*)"个目标"));
        if(add == 0x9A) strcpy(str, Link( Link( (char*)"选择至多", Myitoa(maxtarget) ), (char*)"个目标或不选以重铸"));
        if(add == 0x9B) strcpy(str, (char*)"选择一名有武器的角色");
        if(add == 0x9C) strcpy(str, (char*)"选择使用杀的目标");
        outtextxy(600 - 7.5 * strlen(str), 415, str, gui.selector);

        //绘制选定情况
        for(int i = 0; i <= 3; i++)
            if(allowed & (1 << i)) LineRect(pos[(4 - game.humanid + i) % 4 * 2], pos[(4 - game.humanid + i) % 4 * 2 + 1] - 20,  //将绝对位置(pos)和相对位置(i)进行转换
                                            pos[(4 - game.humanid + i) % 4 * 2] + 130, pos[(4 - game.humanid + i) % 4 * 2 + 1] + 170,
                                            sel & (1 << i) ? EGERGB(255, 57, 57) : EGERGB(255, 215, 77), gui.selector);

        LineRect(960, 540, 1050, 565, EGERGB(255, 215, 77), gui.selector);
        if(sel || add == 0x9A) LineRect(960, 510, 1050, 535, EGERGB(255, 215, 77), gui.selector);

        putimage_transparent(NULL, gui.selector, 0, 0, BLACK);

        //计算已选目标数
        int selected = 0;
        for(int i = 0; i <= 3; i++) if(sel & (1 << i)) selected++;

        //选择目标
        for(int i = 0; i <= 3; i++)
            if(msg.is_down() && (allowed & (1 << i)) && mouse_x >= pos[(4 - game.humanid + i) % 4 * 2] && mouse_x <= pos[(4 - game.humanid + i) % 4 * 2] + 130
                                                    && mouse_y >= pos[(4 - game.humanid + i) % 4 * 2 + 1] - 20 && mouse_y <= pos[(4 - game.humanid + i) % 4 * 2 + 1] + 170)
                    sel ^= ( (selected < maxtarget || (sel & (1 << i))) << i);

        //取消
        if(msg.is_down() && mouse_x >= 960 && mouse_x <= 1050 && mouse_y >= 540 && mouse_y <= 565)
        {
            return 0;
        }

        //确定
        if( (sel || add == 0x9A) && msg.is_down() && mouse_x >= 960 && mouse_x <= 1050 && mouse_y >= 510 && mouse_y <= 535)
        {
            if(add == 0x9B)
            {
                //清楚第一次选择目标的框,同时将选择目标标绿
                setfillcolor(BLACK, gui.selector);
                bar(0, 0, 1200, 450, gui.selector);
                DrawGui();
                int selindex = log(sel) / log(2);
                LineRect(pos[(4 - game.humanid + selindex) % 4 * 2], pos[(4 - game.humanid + selindex) % 4 * 2 + 1] - 20,
                                            pos[(4 - game.humanid + selindex) % 4 * 2] + 130, pos[(4 - game.humanid + selindex) % 4 * 2 + 1] + 170,
                                            GREEN, gui.selector);
            }
            return sel;
        }
    }
    return 0;
}

//濒死结算
void Neardeath(player_t *recipient)
{
    for(int i = 0; i <= 3; i++)
    {
        int use = 0;
        do
        {
            use = Askcard(&player[(recipient->id + i) % 4], TAO, 2 | (recipient->id << 8) );
            if(use) Recover(recipient, 1);
            if(recipient->health > 0) return;
        }while(use);
    }

    if(recipient->health <= 0) Death(recipient);
}

//死亡结算
void Death(player_t *recipient)
{
    //武将牌上显示阵亡
    PIMAGE temp[2] = {newimage(), newimage()};
    getimage(temp[0], (char*)".\\Textures\\States\\dead.png");
    getimage(temp[1], 0, 0, 1200, 600);
    setbkmode(TRANSPARENT, temp[0]);
    cleardevice(temp[1]);
    putimage_rotate(temp[1], temp[0], pos[(recipient->id + 4 - game.humanid) % 4 * 2] + 65, pos[(recipient->id + 4 - game.humanid) % 4 * 2 + 1] + 85, 0.5, 0.5, 3.1415 / 12);
    putimage_transparent(gui.general, temp[1], 0, 0, BLACK);
    delimage(temp[0]);
    delimage(temp[1]);

    //修改状态并输出
    recipient->controller = DEAD;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
    printf("%s死亡\n", general_inf[recipient->general].name);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
    VictoryJudge();

    //弃置所有牌
    for(int i = 0; i <= recipient->cardamount - 1; i++)
    {
        card_inf[recipient->card[i]].owner = -1;
        Putcard(recipient->card[i]);
    }

    for(int i = 0; i <= 3; i++)
    {
        if(recipient->equips[i] != -1)
        {
            card_inf[recipient->equips[i]].owner = -1;
            Putcard(recipient->equips[i]);
        }
    }

    for(int i = 0; i <= 2; i++)
    {
        if(recipient->judges[i][0] != -1)
        {
            card_inf[recipient->judges[i][0]].owner = -1;
            Putcard(recipient->judges[i][0]);
        }
    }

    //清除数据
    memset(recipient->card, 0xFF, sizeof(recipient->card));
    memset(recipient->equips, 0xFF, sizeof(recipient->equips));
    memset(recipient->judges, 0xFF, sizeof(recipient->judges));

    memset(recipient->temp, 0, sizeof(recipient->temp));
    memset(recipient->other, 0, sizeof(recipient->other));

    recipient->maxhealth = 0;
    recipient->health = 0;
    recipient->maxcard = 0;
    recipient->cardamount = 0;
    recipient->limit = 0;
    recipient->awaken = 0;

    if(player[3 - recipient->id].controller != DEAD) Drawcard(&player[3 - recipient->id], 1);
}

//胜利条件判定
void VictoryJudge(void)
{
    if(player[0].controller == DEAD && player[3].controller == DEAD)  //1号与4号均死亡
    {
        if(game.humanid != 0 && game.humanid != 3)  //1号与4号均为电脑
        {
            cleardevice(gui.selector);
            PasteImage((char*)".\\Textures\\victory.png", 0, 0, gui.selector, TRANSPARENT, BLACK);
            putimage_transparent(NULL, gui.selector, 0, 0, BLACK);
            getch();
        }
        else
        {
            cleardevice(gui.selector);
            PasteImage((char*)".\\Textures\\fail.png", 0, 0, gui.selector, TRANSPARENT, BLACK);
            putimage_transparent(NULL, gui.selector, 0, 0, BLACK);
            getch();
        }
    }
    else if( player[1].controller == DEAD && player[2].controller == DEAD)
    {
        if(game.humanid != 1 && game.humanid != 2)  //1号与4号均为电脑
        {
            cleardevice(gui.selector);
            PasteImage((char*)".\\Textures\\victory.png", 0, 0, gui.selector, TRANSPARENT, BLACK);
            putimage_transparent(NULL, gui.selector, 0, 0, BLACK);
            getch();
        }
        else
        {
            cleardevice(gui.selector);
            PasteImage((char*)".\\Textures\\fail.png", 0, 0, gui.selector, TRANSPARENT, BLACK);
            putimage_transparent(NULL, gui.selector, 0, 0, BLACK);
            getch();
        }
    }
}

//无懈响应
///start为第一次询问无懈时开始的角色id,add为锦囊牌的目标(0~3)
///返回值0=此牌仍有效,1=被无懈抵消
int AskWuxie(int start, int add)
{
    int res = 0;
    int ans = 0;
    do
    {
        ans = 0;
        for(int i = 0; i <= 3; i++)
        {
            ans = Askcard(&player[(start + i) % 4], WUXIE, res ? 0x9898 : 0x98 | add << 8);
            if(ans)
            {
                (start += i) %= 4;  //下一次询问从此次使用者开始
                res++;
                break;
            }
        }
    }
    while(ans);
    return res % 2;
}

//询问一张牌
///返回值为是否出type对应类型牌,add为触发原因
/** add的值
 * 0: 被杀指定(无论属性)
 * 低8位2: 濒死求桃(高8位为濒死角色)
 * 0x90: 被决斗指定
 * 0x93: 万箭齐发目标
 * 0x94: 南蛮入侵目标
 * 0x98: 无懈
 * 低8位0x9B: 借刀杀人出杀(此时高2位为杀的目标)
 */

int Askcard(player_t *recipient, type_e type, int add)
{
    delay_fps(3);
    //八卦阵效果
    if(type == SHAN && (type_e)recipient->equips[1] == BAGUA)
        if(Bagua(recipient)) return 1;

    if(recipient->controller == HUMAN)
    {
        int sel = -1;
        for(; is_run(); delay_fps(10))
        {
            while (mousemsg()) msg = getmouse();
            mousepos(&mouse_x, &mouse_y);

            DrawGui();
            cleardevice(gui.selector);

            //提示
            char str[121] = "";
            if(add == 0) strcpy(str, (char*)"成为杀的目标，请使用一张闪");
            if(add % 0x100 == 2) strcpy(str, Link(general_inf[player[add >> 8].general].name, (char*)"濒死,使用桃使其回复体力"));
            if(add == 0x90) strcpy(str, (char*)"决斗中，请打出一张杀");
            if(add == 0x93) strcpy(str, (char*)"成为万箭齐发的目标，请打出一张闪");
            if(add == 0x94) strcpy(str, (char*)"成为南蛮入侵的目标，请打出一张杀");
            if(add % 0x100 == 0x98)
            {
                if(add >> 8 == 0x98) strcpy(str, (char*)"是否使用无懈可击抵消无懈可击");
                else strcpy(str, Link( Link( (char*)"是否使用无懈可击抵消对", general_inf[player[add >> 8].general].name), (char*)"的效果"));
            }
            if(add % 0x100 == 0x9B) strcpy(str, Link( Link( (char*)"成为借刀杀人的目标，请对",  general_inf[player[add >> 8].general].name), (char*)"使用杀"));

            setcolor(WHITE, gui.selector);
            setfont(30, 0, "仿宋", gui.selector);
            outtextxy(600 - 7.5 * strlen(str), 415, str, gui.selector);

            //高亮
            for(int i = 0; i <= 7; i++)
            {
                if(type == SHA)
                {
                    if( (game.page * 8 + i < recipient->cardamount) &&
                       (card_inf[recipient->card[game.page * 8 + i]].type == SHA || card_inf[recipient->card[game.page * 8 + i]].type == HUOSHA ||
                       card_inf[recipient->card[game.page * 8 + i]].type == LEISHA) )
                        LineRect(160 + 100 * i, 465, 240 + 100 * i, 585, EGERGB(255, 215, 77), gui.selector);
                }
                else
                {
                    if((game.page * 8 + i < recipient->cardamount) &&
                       ( (card_inf[recipient->card[game.page * 8 + i]].type == type) ||   // 类型相同
                       (add % 0x100 == 2 && add >> 8 == recipient->id && card_inf[recipient->card[game.page * 8 + i]].type == JIU) ) )
                        LineRect(160 + 100 * i, 465, 240 + 100 * i, 585, EGERGB(255, 215, 77), gui.selector);
                }
            }

            if(sel != -1)
            {
                LineRect(160 + 100 * (sel % 8), 465, 240 + 100 * (sel % 8), 585, EGERGB(255, 57, 57), gui.selector);
                LineRect(960, 510, 1050, 535, EGERGB(255, 215, 77), gui.selector);
            }

            LineRect(960, 540, 1050, 565, EGERGB(255, 215, 77), gui.selector);
            putimage_transparent(NULL, gui.selector, 0, 0, BLACK);

            //检测按键
            if(msg.is_down() && mouse_x >= 150 && mouse_x <= 950 && mouse_y >= 450 && mouse_y <= 600)
            {
                int tosel = (mouse_x - 150) / 100;
                if(type == SHA)
                {
                    if( (game.page * 8 + tosel < recipient->cardamount) &&
                       (card_inf[recipient->card[game.page * 8 + tosel]].type == SHA || card_inf[recipient->card[game.page * 8 + tosel]].type == HUOSHA ||
                       card_inf[recipient->card[game.page * 8 + tosel]].type == LEISHA) )  sel = game.page * 8 + tosel;
                }
                else
                {
                    if((game.page * 8 + tosel < recipient->cardamount) &&
                       ( (card_inf[recipient->card[game.page * 8 + tosel]].type == type) ||   // 类型相同
                       (add % 0x100 == 2 && add >> 8 == recipient->id && card_inf[recipient->card[game.page * 8 + tosel]].type == JIU) ) ) //自己濒死使用酒
                        sel = game.page * 8 + tosel;
                }
            }

            if(sel != -1 && msg.is_down() && mouse_x >= 960 && mouse_x <= 1050 && mouse_y >= 510 && mouse_y <= 535)
            {
                if(add == 0 || add % 0x100 == 2 || add % 0x100 == 0x98) printf("%s使用", general_inf[recipient->general].name);
                if(add == 0x90 || add == 0x93 || add == 0x94) printf("%s打出", general_inf[recipient->general].name);
                if(add % 0x100 != 0x9B)
                {
                    Printcard(recipient->card[sel]);
                    printf("\n");
                    Putcard(recipient->card[sel]);
                }

                int temp = recipient->card[sel];
                card_inf[recipient->card[sel]].owner = -1;
                recipient->card[sel] = -1;
                recipient->cardamount--;
                IndexAlign(recipient->card, recipient->cardamount, 160);
                if(add % 0x100 == 0x9B) return temp;
                else return 1;
            }

            if(msg.is_down() && mouse_x >= 960 && mouse_x <= 1050 && mouse_y >= 540 && mouse_y <= 565)
            {
                DrawGui();
                if(add % 0x100 == 0x9B) return -1;
                else return 0;
            }

            //翻页
            if(msg.is_down() && mouse_x >= 960 && mouse_x <= 970 && mouse_y >= 575 && mouse_y <= 595)
            {
                if(game.page > 0) game.page--;
            }
            if(msg.is_down() && mouse_x >= 985 && mouse_x <= 1000 && mouse_y >= 575 && mouse_y <= 595)
            {
                if(recipient->cardamount > (game.page + 1) * 8) game.page++;
            }
        }
    }
    else
    {
        int sel = AnswerAi(recipient, type, (add % 0x100 == 2 && add >> 8 == recipient->id) ? 2 : 0);
        if(sel != -1)
        {

            if(add == 0 || add % 0x100 == 2 || add % 0x100 == 0x98) printf("%s使用", general_inf[recipient->general].name);
            if(add == 0x90 || add == 0x93 || add == 0x94) printf("%s打出", general_inf[recipient->general].name);
            if(add % 0x100 != 0x9B)
            {
                Printcard(recipient->card[sel]);
                printf("\n");
                Putcard(recipient->card[sel]);
            }

            int temp = recipient->card[sel];
            card_inf[recipient->card[sel]].owner = -1;
            recipient->card[sel] = -1;
            recipient->cardamount--;
            IndexAlign(recipient->card, recipient->cardamount, 160);
            if(add % 0x100 == 0x9B) return temp;
            else return 1;
        }
        else
        {
            if(add % 0x100 == 0x9B) return -1;
            else return 0;
        }
    }
    return 0;
}
