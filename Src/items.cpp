#include "items.h"

class player
{
public:
    //玩家初始化
    void PlayerInit(general_t general, int health, int maxhealth, int maxcard, int limitused)
    {
        maxhealth = general.maxhealth;
        health = maxhealth;
        maxcard = maxhealth;
        limitused = general.limit;
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
    {1, SPADE, JUEDOU, "决斗", 0}, {1, SPADE, SHANDIAN, "闪电", 0}, {1, SPADE, GUDING, "古锭刀", 0},
    {2, SPADE, CIXIONG, "雌雄双股剑", 0}, {2, SPADE, BAGUA, "八卦阵", 0}, {2, SPADE, TENGJIA, "藤甲", 0}, {2, SPADE, HANBING, "寒冰剑", 0},
    {3, SPADE, GUOCHAI, "过河拆桥", 0}, {3, SPADE, SHUNQIAN, "顺手牵羊", 0}, {3, SPADE, JIU, "酒", 0},
    {4, SPADE, GUOCHAI, "过河拆桥", 0}, {4, SPADE, SHUNQIAN, "顺手牵羊", 0}, {4, SPADE, LEISHA, "雷杀", 0},
    {5, SPADE, QINGLONG, "青龙偃月刀", 0}, {5, SPADE, JUEYING, "绝影", 0}, {5, SPADE, LEISHA, "雷杀", 0},
    {6, SPADE, LE, "乐不思蜀", 0}, {6, SPADE, QINGGANG, "青釭剑", 0}, {6, SPADE, LEISHA, "雷杀", 0},
    {7, SPADE, SHA, "杀", 0}, {7, SPADE, NANMAN, "南蛮入侵", 0}, {7, SPADE, LEISHA, "雷杀", 0},
    {8, SPADE, SHA, "杀", 0}, {8, SPADE, SHA, "杀", 0}, {8, SPADE, LEISHA, "雷杀", 0},
    {9, SPADE, SHA, "杀", 0}, {9, SPADE, SHA, "杀", 0}, {9, SPADE, JIU, "酒", 0},
    {10, SPADE, SHA, "杀", 0}, {10, SPADE, SHA, "杀", 0}, {10, SPADE, JUEDOU, "决斗", 0},
    {11, SPADE, SHUNQIAN, "顺手牵羊", 0}, {11, SPADE, WUXIE, "无懈可击", 0}, {11, SPADE, TIESUO, "铁索连环", 0},
    {12, SPADE, GUOCHAI, "过河拆桥", 0}, {12, SPADE, ZHANGBA, "丈八蛇矛", 0}, {12, SPADE, TIESUO, "铁索连环", 0},
    {13, SPADE, NANMAN, "南蛮入侵", 0}, {13, SPADE, DAWAN, "大宛", 0}, {13, SPADE, WUXIE, "无懈可击", 0},

    {1, HEART, TAOYUAN, "桃园结义", 0}, {1, HEART, WANJIAN, "万箭齐发", 0}, {1, HEART, WUXIE, "无懈可击", 0},
    {2, HEART, SHAN, "闪", 0}, {2, HEART, SHAN, "闪", 0}, {2, HEART,HUOGONG, "火攻", 0},
    {3, HEART, TAO, "桃", 0}, {3, HEART, WUGU, "五谷丰登", 0}, {3, HEART, HUOGONG, "火攻", 0},
    {4, HEART, TAO, "桃", 0}, {4, HEART, WUGU, "五谷丰登", 0}, {4, HEART, HUOSHA, "火杀", 0},
    {5, HEART, QILIN, "麒麟弓", 0}, {5, HEART, CHITU, "赤兔", 0}, {5, HEART, TAO, "桃", 0},
    {6, HEART, TAO, "桃", 0}, {6, HEART, LE, "乐不思蜀", 0}, {6, HEART, TAO, "桃", 0},
    {7, HEART, TAO, "桃", 0}, {7, HEART, WUZHONG, "无中生有", 0}, {7, HEART, HUOSHA, "火杀", 0},
    {8, HEART, TAO, "桃", 0}, {8, HEART, WUZHONG, "无中生有", 0}, {8, HEART, SHAN, "闪", 0},
    {9, HEART, TAO, "桃", 0}, {9, HEART, WUZHONG, "无中生有", 0}, {9, HEART, SHAN, "闪", 0},
    {10, HEART, SHA, "杀", 0}, {10, HEART, SHA, "杀", 0}, {10, HEART, HUOSHA, "火杀", 0},
    {11, HEART, SHA, "杀", 0}, {11, HEART, WUZHONG, "无中生有", 0}, {11, HEART, SHAN, "闪", 0},
    {12, HEART, TAO, "桃", 0}, {12, HEART, GUOCHAI, "过河拆桥", 0}, {12, HEART, SHAN, "闪", 0}, {12, HEART, SHANDIAN, "闪电", 0},
    {13, HEART, SHAN, "闪", 0}, {13, HEART, FEIDIAN, "爪黄飞电", 0}, {13, HEART, WUXIE, "无懈可击", 0},

    {1, CLUB, JUEDOU, "决斗", 0}, {1, CLUB, ZHUGE, "诸葛连弩", 0}, {1, CLUB, BAIYIN, "白银狮子", 0},
    {2, CLUB, SHA, "杀", 0}, {2, CLUB, BAGUA, "八卦阵", 0}, {2, CLUB, TENGJIA, "藤甲", 0}, {2, CLUB, RENWANG, "仁王盾", 0},
    {3, CLUB, SHA, "杀", 0}, {3, CLUB, GUOCHAI, "过河拆桥", 0}, {3, CLUB, JIU, "酒", 0},
    {4, CLUB, SHA, "杀", 0}, {4, CLUB, GUOCHAI, "过河拆桥", 0}, {4, CLUB, BING, "兵粮寸断", 0},
    {5, CLUB, SHA, "杀", 0}, {5, CLUB, DILU, "的卢", 0}, {5, CLUB, LEISHA, "雷杀", 0},
    {6, CLUB, SHA, "杀", 0}, {6, CLUB, LE, "乐不思蜀", 0}, {6, CLUB, LEISHA, "雷杀", 0},
    {7, CLUB, SHA, "杀", 0}, {7, CLUB, NANMAN, "南蛮入侵", 0}, {7, CLUB, LEISHA, "雷杀", 0},
    {8, CLUB, SHA, "杀", 0}, {8, CLUB, SHA, "杀", 0}, {8, CLUB, LEISHA, "雷杀", 0},
    {9, CLUB, SHA, "杀", 0}, {9, CLUB, SHA, "杀", 0}, {9, CLUB, JIU, "酒", 0},
    {10, CLUB, SHA, "杀", 0}, {10, CLUB, SHA, "杀", 0}, {10, CLUB, TIESUO, "铁索连环", 0},
    {11, CLUB, SHA, "杀", 0}, {11, CLUB, SHA, "杀", 0}, {11, CLUB, TIESUO, "铁索连环", 0},
    {12, CLUB, JIEDAO, "借刀杀人", 0}, {12, CLUB, WUXIE, "无懈可击", 0}, {12, CLUB, TIESUO, "铁索连环", 0},
    {13, CLUB, JIEDAO, "借刀杀人", 0}, {13, CLUB, WUXIE, "无懈可击", 0}, {13, CLUB, TIESUO, "铁索连环", 0},

    {1, DIAMOND, ZHUGE, "诸葛连弩", 0}, {1, DIAMOND, JUEDOU, "决斗", 0}, {1, DIAMOND, ZHUQUE, "朱雀羽扇", 0},
    {2, DIAMOND, SHAN, "闪", 0}, {2, DIAMOND, SHAN, "闪", 0}, {2, DIAMOND, TAO, "桃", 0},
    {3, DIAMOND, SHAN, "闪", 0}, {3, DIAMOND, SHUNQIAN, "顺手牵羊", 0}, {3, DIAMOND, TAO, "桃", 0},
    {4, DIAMOND, SHAN, "闪", 0}, {4, DIAMOND, SHUNQIAN, "顺手牵羊", 0}, {4, DIAMOND, HUOSHA, "火杀", 0},
    {5, DIAMOND, SHAN, "闪", 0}, {5, DIAMOND, GUANSHI, "贯石斧", 0}, {5, DIAMOND, HUOSHA, "火杀", 0},
    {6, DIAMOND, SHAN, "闪", 0}, {6, DIAMOND, SHAN, "闪", 0}, {6, DIAMOND, SHA, "杀", 0},
    {7, DIAMOND, SHAN, "闪", 0}, {7, DIAMOND, SHAN, "闪", 0}, {7, DIAMOND, SHA, "杀", 0},
    {8, DIAMOND, SHAN, "闪", 0}, {8, DIAMOND, SHAN, "闪", 0}, {8, DIAMOND, SHA, "杀", 0},
    {9, DIAMOND, SHAN, "闪", 0}, {9, DIAMOND, SHA, "杀", 0}, {9, DIAMOND, JIU, "酒", 0},
    {10, DIAMOND, SHAN, "闪", 0}, {10, DIAMOND, SHAN, "闪", 0}, {10, DIAMOND, SHA, "杀", 0},
    {11, DIAMOND, SHAN, "闪", 0}, {11, DIAMOND, SHAN, "闪", 0}, {11, DIAMOND, SHAN, "闪", 0},
    {12, DIAMOND, TAO, "桃", 0}, {12, DIAMOND, FANGTIAN, "方天画戟", 0}, {12, DIAMOND, HUOGONG, "火攻", 0}, {12, DIAMOND, WUXIE, "无懈可击", 0},
    {13, DIAMOND, SHA, "杀", 0}, {13, DIAMOND, ZIXIN, "紫骍", 0}, {13, DIAMOND, HUALIU, "骅骝", 0},
};
