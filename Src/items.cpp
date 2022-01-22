#include "items.h"

class player
{
public:
    PlayerInit(general_t general, int health, int maxhealth, int maxcard)
    {
        maxhealth = general.maxhealth;
        health = maxhealth;
        maxcard = maxhealth;
        n
    }
private:

    int number;  //位次
    general_t general;  //武将信息

    int health;  //当前体力
    int maxhealth; //体力上限
    int maxcard;  //当前手牌上限
    int nowslash = 0;  //当前回合已使用杀次数
    int maxslash = 0;  //每回合使用杀的最大次数
    int spirits = 0;  //回合内使用酒的状态:0=未使用,1=已使用且存在伤害+1效果,2=已使用

    int card[100]; //当前拥有手牌,储存id=0代表为空
    int equips[4];  //装备区,分别为武器,防具,+1,-1
    int judges[3]; //判定区,判定阶段由后向前结算
    int range; //当前攻击距离

    int limitused;  //存在未使用的限定技=1,否则为0
    int awaken;  //存在觉醒技且已发动=1,否则为0
};

card_t pile[160] =
{
    {1, SPADE, DUEL, "决斗", 0}, {1, SPADE, LIGHTNING, "闪电", 0}, {1, SPADE, GUDING, "古锭刀", 0},
    {2, SPADE, DUEL, "雌雄双股剑", 0}, {2, SPADE, DUEL, "八卦阵", 0}, {2, SPADE, DUEL, "藤甲", 0}, {2, SPADE, DUEL, "寒冰剑", 0},
    {3, SPADE, DUEL, "过河拆桥", 0}, {3, SPADE, DUEL, "顺手牵羊", 0}, {3, SPADE, DUEL, "酒", 0},
    {4, SPADE, DUEL, "过河拆桥", 0}, {4, SPADE, DUEL, "顺手牵羊", 0}, {4, SPADE, DUEL, "雷杀", 0},
    {5, SPADE, DUEL, "青龙偃月刀", 0}, {5, SPADE, DUEL, "绝影", 0}, {5, SPADE, DUEL, "雷杀", 0},
    {6, SPADE, DUEL, "乐不思蜀", 0}, {6, SPADE, DUEL, "青釭剑", 0}, {6, SPADE, DUEL, "雷杀", 0},
    {7, SPADE, DUEL, "杀", 0}, {7, SPADE, DUEL, "南蛮入侵", 0}, {7, SPADE, DUEL, "雷杀", 0},
    {8, SPADE, DUEL, "杀", 0}, {8, SPADE, DUEL, "杀", 0}, {8, SPADE, DUEL, "雷杀", 0},
    {9, SPADE, DUEL, "杀", 0}, {9, SPADE, DUEL, "杀", 0}, {9, SPADE, DUEL, "酒", 0},
    {10, SPADE, DUEL, "杀", 0}, {10, SPADE, DUEL, "杀", 0}, {10, SPADE, DUEL, "决斗", 0},
    {11, SPADE, DUEL, "顺手牵羊", 0}, {11, SPADE, DUEL, "无懈可击", 0}, {11, SPADE, DUEL, "铁索连环", 0},
    {12, SPADE, DUEL, "过河拆桥", 0}, {12, SPADE, DUEL, "丈八蛇矛", 0}, {12, SPADE, DUEL, "铁索连环", 0},
    {13, SPADE, DUEL, "南蛮入侵", 0}, {13, SPADE, DUEL, "大宛", 0}, {13, SPADE, DUEL, "无懈可击", 0},

    {1, SPADE, DUEL, "决斗", 0}, {1, SPADE, DUEL, "决斗", 0}, {1, SPADE, DUEL, "决斗", 0},
    {1, SPADE, DUEL, "决斗", 0}, {1, SPADE, DUEL, "决斗", 0}, {1, SPADE, DUEL, "决斗", 0},
    {1, SPADE, DUEL, "决斗", 0}, {1, SPADE, DUEL, "决斗", 0}, {1, SPADE, DUEL, "决斗", 0},
    {1, SPADE, DUEL, "决斗", 0}, {1, SPADE, DUEL, "决斗", 0}, {1, SPADE, DUEL, "决斗", 0},
    {1, SPADE, DUEL, "决斗", 0}, {1, SPADE, DUEL, "决斗", 0}, {1, SPADE, DUEL, "决斗", 0},
    {1, SPADE, DUEL, "决斗", 0}, {1, SPADE, DUEL, "决斗", 0}, {1, SPADE, DUEL, "决斗", 0},
    {1, SPADE, DUEL, "决斗", 0}, {1, SPADE, DUEL, "决斗", 0}, {1, SPADE, DUEL, "决斗", 0},
    {1, SPADE, DUEL, "决斗", 0}, {1, SPADE, DUEL, "决斗", 0}, {1, SPADE, DUEL, "决斗", 0},
    {1, SPADE, DUEL, "决斗", 0}, {1, SPADE, DUEL, "决斗", 0}, {1, SPADE, DUEL, "决斗", 0},
    {1, SPADE, DUEL, "决斗", 0}, {1, SPADE, DUEL, "决斗", 0}, {1, SPADE, DUEL, "决斗", 0},
    {1, SPADE, DUEL, "决斗", 0}, {1, SPADE, DUEL, "决斗", 0}, {1, SPADE, DUEL, "决斗", 0},
    {1, SPADE, DUEL, "决斗", 0}, {1, SPADE, DUEL, "决斗", 0}, {1, SPADE, DUEL, "决斗", 0},
    {1, SPADE, DUEL, "决斗", 0}, {1, SPADE, DUEL, "决斗", 0}, {1, SPADE, DUEL, "决斗", 0},

    {1, SPADE, DUEL, "决斗", 0}, {1, SPADE, DUEL, "决斗", 0}, {1, SPADE, DUEL, "决斗", 0},
    {1, SPADE, DUEL, "决斗", 0}, {1, SPADE, DUEL, "决斗", 0}, {1, SPADE, DUEL, "决斗", 0},
    {1, SPADE, DUEL, "决斗", 0}, {1, SPADE, DUEL, "决斗", 0}, {1, SPADE, DUEL, "决斗", 0},
    {1, SPADE, DUEL, "决斗", 0}, {1, SPADE, DUEL, "决斗", 0}, {1, SPADE, DUEL, "决斗", 0},
    {1, SPADE, DUEL, "决斗", 0}, {1, SPADE, DUEL, "决斗", 0}, {1, SPADE, DUEL, "决斗", 0},
    {1, SPADE, DUEL, "决斗", 0}, {1, SPADE, DUEL, "决斗", 0}, {1, SPADE, DUEL, "决斗", 0},
    {1, SPADE, DUEL, "决斗", 0}, {1, SPADE, DUEL, "决斗", 0}, {1, SPADE, DUEL, "决斗", 0},
    {1, SPADE, DUEL, "决斗", 0}, {1, SPADE, DUEL, "决斗", 0}, {1, SPADE, DUEL, "决斗", 0},
    {1, SPADE, DUEL, "决斗", 0}, {1, SPADE, DUEL, "决斗", 0}, {1, SPADE, DUEL, "决斗", 0},
    {1, SPADE, DUEL, "决斗", 0}, {1, SPADE, DUEL, "决斗", 0}, {1, SPADE, DUEL, "决斗", 0},
    {1, SPADE, DUEL, "决斗", 0}, {1, SPADE, DUEL, "决斗", 0}, {1, SPADE, DUEL, "决斗", 0},
    {1, SPADE, DUEL, "决斗", 0}, {1, SPADE, DUEL, "决斗", 0}, {1, SPADE, DUEL, "决斗", 0},
    {1, SPADE, DUEL, "决斗", 0}, {1, SPADE, DUEL, "决斗", 0}, {1, SPADE, DUEL, "决斗", 0},

    {1, SPADE, DUEL, "决斗", 0}, {1, SPADE, DUEL, "决斗", 0}, {1, SPADE, DUEL, "决斗", 0},
    {1, SPADE, DUEL, "决斗", 0}, {1, SPADE, DUEL, "决斗", 0}, {1, SPADE, DUEL, "决斗", 0},
    {1, SPADE, DUEL, "决斗", 0}, {1, SPADE, DUEL, "决斗", 0}, {1, SPADE, DUEL, "决斗", 0},
    {1, SPADE, DUEL, "决斗", 0}, {1, SPADE, DUEL, "决斗", 0}, {1, SPADE, DUEL, "决斗", 0},
    {1, SPADE, DUEL, "决斗", 0}, {1, SPADE, DUEL, "决斗", 0}, {1, SPADE, DUEL, "决斗", 0},
    {1, SPADE, DUEL, "决斗", 0}, {1, SPADE, DUEL, "决斗", 0}, {1, SPADE, DUEL, "决斗", 0},
    {1, SPADE, DUEL, "决斗", 0}, {1, SPADE, DUEL, "决斗", 0}, {1, SPADE, DUEL, "决斗", 0},
    {1, SPADE, DUEL, "决斗", 0}, {1, SPADE, DUEL, "决斗", 0}, {1, SPADE, DUEL, "决斗", 0},
    {1, SPADE, DUEL, "决斗", 0}, {1, SPADE, DUEL, "决斗", 0}, {1, SPADE, DUEL, "决斗", 0},
    {1, SPADE, DUEL, "决斗", 0}, {1, SPADE, DUEL, "决斗", 0}, {1, SPADE, DUEL, "决斗", 0},
    {1, SPADE, DUEL, "决斗", 0}, {1, SPADE, DUEL, "决斗", 0}, {1, SPADE, DUEL, "决斗", 0},
    {1, SPADE, DUEL, "决斗", 0}, {1, SPADE, DUEL, "决斗", 0}, {1, SPADE, DUEL, "决斗", 0},
    {1, SPADE, DUEL, "决斗", 0}, {1, SPADE, DUEL, "决斗", 0}, {1, SPADE, DUEL, "决斗", 0},

};
