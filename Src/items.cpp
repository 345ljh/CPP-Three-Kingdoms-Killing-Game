#include "items.h"

card_t card_inf[160] =
{
    {1, SPADE, JUEDOU, "决斗", -2}, {1, SPADE, SHANDIAN, "闪电", -2}, {1, SPADE, GUDING, "古锭刀", -2},
    {2, SPADE, CIXIONG, "雌雄双股剑", -2}, {2, SPADE, BAGUA, "八卦阵", -2}, {2, SPADE, TENGJIA, "藤甲", -2}, {2, SPADE, HANBING, "寒冰剑", -2},
    {3, SPADE, GUOCHAI, "过河拆桥", -2}, {3, SPADE, SHUNQIAN, "顺手牵羊", -2}, {3, SPADE, JIU, "酒", -2},
    {4, SPADE, GUOCHAI, "过河拆桥", -2}, {4, SPADE, SHUNQIAN, "顺手牵羊", -2}, {4, SPADE, LEISHA, "雷杀", -2},
    {5, SPADE, QINGLONG, "青龙偃月刀", -2}, {5, SPADE, JUEYING, "绝影", -2}, {5, SPADE, LEISHA, "雷杀", -2},
    {6, SPADE, LE, "乐不思蜀", -2}, {6, SPADE, QINGGANG, "青釭剑", -2}, {6, SPADE, LEISHA, "雷杀", -2},
    {7, SPADE, SHA, "杀", -2}, {7, SPADE, NANMAN, "南蛮入侵", -2}, {7, SPADE, LEISHA, "雷杀", -2},
    {8, SPADE, SHA, "杀", -2}, {8, SPADE, SHA, "杀", -2}, {8, SPADE, LEISHA, "雷杀", -2},
    {9, SPADE, SHA, "杀", -2}, {9, SPADE, SHA, "杀", -2}, {9, SPADE, JIU, "酒", -2},
    {10, SPADE, SHA, "杀", -2}, {10, SPADE, SHA, "杀", -2}, {10, SPADE, JUEDOU, "决斗", -2},
    {11, SPADE, SHUNQIAN, "顺手牵羊", -2}, {11, SPADE, WUXIE, "无懈可击", -2}, {11, SPADE, TIESUO, "铁索连环", -2},
    {12, SPADE, GUOCHAI, "过河拆桥", -2}, {12, SPADE, ZHANGBA, "丈八蛇矛", -2}, {12, SPADE, TIESUO, "铁索连环", -2},
    {13, SPADE, NANMAN, "南蛮入侵", -2}, {13, SPADE, DAWAN, "大宛", -2}, {13, SPADE, WUXIE, "无懈可击", -2},

    {1, HEART, TAOYUAN, "桃园结义", -2}, {1, HEART, WANJIAN, "万箭齐发", -2}, {1, HEART, WUXIE, "无懈可击", -2},
    {2, HEART, SHAN, "闪", -2}, {2, HEART, SHAN, "闪", -2}, {2, HEART,HUOGONG, "火攻", -2},
    {3, HEART, TAO, "桃", -2}, {3, HEART, WUGU, "五谷丰登", -2}, {3, HEART, HUOGONG, "火攻", -2},
    {4, HEART, TAO, "桃", -2}, {4, HEART, WUGU, "五谷丰登", -2}, {4, HEART, HUOSHA, "火杀", -2},
    {5, HEART, QILIN, "麒麟弓", -2}, {5, HEART, CHITU, "赤兔", -2}, {5, HEART, TAO, "桃", -2},
    {6, HEART, TAO, "桃", -2}, {6, HEART, LE, "乐不思蜀", -2}, {6, HEART, TAO, "桃", -2},
    {7, HEART, TAO, "桃", -2}, {7, HEART, WUZHONG, "无中生有", -2}, {7, HEART, HUOSHA, "火杀", -2},
    {8, HEART, TAO, "桃", -2}, {8, HEART, WUZHONG, "无中生有", -2}, {8, HEART, SHAN, "闪", -2},
    {9, HEART, TAO, "桃", -2}, {9, HEART, WUZHONG, "无中生有", -2}, {9, HEART, SHAN, "闪", -2},
    {10, HEART, SHA, "杀", -2}, {10, HEART, SHA, "杀", -2}, {10, HEART, HUOSHA, "火杀", -2},
    {11, HEART, SHA, "杀", -2}, {11, HEART, WUZHONG, "无中生有", -2}, {11, HEART, SHAN, "闪", -2},
    {12, HEART, TAO, "桃", -2}, {12, HEART, GUOCHAI, "过河拆桥", -2}, {12, HEART, SHAN, "闪", -2}, {12, HEART, SHANDIAN, "闪电", -2},
    {13, HEART, SHAN, "闪", -2}, {13, HEART, FEIDIAN, "爪黄飞电", -2}, {13, HEART, WUXIE, "无懈可击", -2},

    {1, CLUB, JUEDOU, "决斗", -2}, {1, CLUB, ZHUGE, "诸葛连弩", -2}, {1, CLUB, BAIYIN, "白银狮子", -2},
    {2, CLUB, SHA, "杀", -2}, {2, CLUB, BAGUA, "八卦阵", -2}, {2, CLUB, TENGJIA, "藤甲", -2}, {2, CLUB, RENWANG, "仁王盾", -2},
    {3, CLUB, SHA, "杀", -2}, {3, CLUB, GUOCHAI, "过河拆桥", -2}, {3, CLUB, JIU, "酒", -2},
    {4, CLUB, SHA, "杀", -2}, {4, CLUB, GUOCHAI, "过河拆桥", -2}, {4, CLUB, BING, "兵粮寸断", -2},
    {5, CLUB, SHA, "杀", -2}, {5, CLUB, DILU, "的卢", -2}, {5, CLUB, LEISHA, "雷杀", -2},
    {6, CLUB, SHA, "杀", -2}, {6, CLUB, LE, "乐不思蜀", -2}, {6, CLUB, LEISHA, "雷杀", -2},
    {7, CLUB, SHA, "杀", -2}, {7, CLUB, NANMAN, "南蛮入侵", -2}, {7, CLUB, LEISHA, "雷杀", -2},
    {8, CLUB, SHA, "杀", -2}, {8, CLUB, SHA, "杀", -2}, {8, CLUB, LEISHA, "雷杀", -2},
    {9, CLUB, SHA, "杀", -2}, {9, CLUB, SHA, "杀", -2}, {9, CLUB, JIU, "酒", -2},
    {10, CLUB, SHA, "杀", -2}, {10, CLUB, SHA, "杀", -2}, {10, CLUB, TIESUO, "铁索连环", -2},
    {11, CLUB, SHA, "杀", -2}, {11, CLUB, SHA, "杀", -2}, {11, CLUB, TIESUO, "铁索连环", -2},
    {12, CLUB, JIEDAO, "借刀杀人", -2}, {12, CLUB, WUXIE, "无懈可击", -2}, {12, CLUB, TIESUO, "铁索连环", -2},
    {13, CLUB, JIEDAO, "借刀杀人", -2}, {13, CLUB, WUXIE, "无懈可击", -2}, {13, CLUB, TIESUO, "铁索连环", -2},

    {1, DIAMOND, ZHUGE, "诸葛连弩", -2}, {1, DIAMOND, JUEDOU, "决斗", -2}, {1, DIAMOND, ZHUQUE, "朱雀羽扇", -2},
    {2, DIAMOND, SHAN, "闪", -2}, {2, DIAMOND, SHAN, "闪", -2}, {2, DIAMOND, TAO, "桃", -2},
    {3, DIAMOND, SHAN, "闪", -2}, {3, DIAMOND, SHUNQIAN, "顺手牵羊", -2}, {3, DIAMOND, TAO, "桃", -2},
    {4, DIAMOND, SHAN, "闪", -2}, {4, DIAMOND, SHUNQIAN, "顺手牵羊", -2}, {4, DIAMOND, HUOSHA, "火杀", -2},
    {5, DIAMOND, SHAN, "闪", -2}, {5, DIAMOND, GUANSHI, "贯石斧", -2}, {5, DIAMOND, HUOSHA, "火杀", -2},
    {6, DIAMOND, SHAN, "闪", -2}, {6, DIAMOND, SHAN, "闪", -2}, {6, DIAMOND, SHA, "杀", -2},
    {7, DIAMOND, SHAN, "闪", -2}, {7, DIAMOND, SHAN, "闪", -2}, {7, DIAMOND, SHA, "杀", -2},
    {8, DIAMOND, SHAN, "闪", -2}, {8, DIAMOND, SHAN, "闪", -2}, {8, DIAMOND, SHA, "杀", -2},
    {9, DIAMOND, SHAN, "闪", -2}, {9, DIAMOND, SHA, "杀", -2}, {9, DIAMOND, JIU, "酒", -2},
    {10, DIAMOND, SHAN, "闪", -2}, {10, DIAMOND, SHAN, "闪", -2}, {10, DIAMOND, SHA, "杀", -2},
    {11, DIAMOND, SHAN, "闪", -2}, {11, DIAMOND, SHAN, "闪", -2}, {11, DIAMOND, SHAN, "闪", -2},
    {12, DIAMOND, TAO, "桃", -2}, {12, DIAMOND, FANGTIAN, "方天画戟", -2}, {12, DIAMOND, HUOGONG, "火攻", -2}, {12, DIAMOND, WUXIE, "无懈可击", -2},
    {13, DIAMOND, SHA, "杀", -2}, {13, DIAMOND, ZIXIN, "紫骍", -2}, {13, DIAMOND, HUALIU, "骅骝", -2},
};

//武将信息
general_t general_inf[GENERALS] =
{
    {"魏国士兵", 0, MALE, "", "", 4, WEI, 0, 0},
    {"蜀国女兵", 1, FEMALE, "", "", 4, SHU, 0, 0},
    {"吴国弓兵", 2, MALE, "", "", 4, WU, 0, 0},
    {"黄巾军", 3, MALE, "", "", 4, QUN, 0, 0},
    {"金虎", 4, MALE, "", "贺岁", 8, SHEN, 0, 0},
};

game_t game;
player_t player[PLAYERS];

FILE *fp = NULL; //监视变量
