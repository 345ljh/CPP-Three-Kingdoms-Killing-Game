#ifndef ITEMS_H_INCLUDED
#define ITEMS_H_INCLUDED
/********************************************
*            consts&variables               *
********************************************/

/********************************************
*                  enums                    *
********************************************/

typedef enum
{
    SLASH = 0x01, DODGE, PEACH, SPIRITS, FIRESLASH, THUNDERSLASH,
    //基本牌:杀,闪,桃,酒,火杀,雷杀
    ZHUGE = 0x10,
    QINGGANG = 0x20, CIXIONG, GUDING, HANBING,
    GUANSHI = 0x30, QINGLONG, ZHANGBA,
    FANGTIAN = 0x40, ZHUQUE,
    QILIN = 0x50,
    //武器,十六进制下第2位表示范围
    RENWANG = 0x60, BAGUA, TENGJIA,
    //防具
    CHITU = 0x70, DAWAN, ZIXIN,
    //-1马
    JUEYING = 0x80, FEIDIAN, DILU, HUALIU,
    //+1马
    DUEL = 0x90, SABOTAGE, THEFT, ARROWRAIN, SAVAGE, FRATERNITY, OUTOFNONE, HARVEST,
    DEFENSE, FIREATTACK, CHAIN, BORROWKNIFE,
    //基本锦囊:决斗,过拆,顺牵,万箭,南蛮,桃园,无中,五谷,无懈,火攻,铁锁,借刀
    DROWN = 0xA0, SUPPLYSHORT, LIGHTNING,
    //延时锦囊:乐,兵,闪电
}type_e; //牌类型

typedef enum
{
    SPADE = 0x00, CLUB, HEART = 0x10, DIAMOND, NONE = 0x20
}suit_e; //花色

typedef enum
{
    WEI, SHU, WU, QUN
} nation_e; //势力


/********************************************
*                 structs                   *
********************************************/

typedef struct
{
    char name[11];  //武将名
    int id;  //对应id
    char skill[21];  //技能名,多个技能用空格分开
    int maxhealth;  //体力上限
    nation_e nation;//势力
    int limit;  //限定技是否存在
    int awake;  //觉醒技是否存在
} general_t;//武将信息

typedef struct
{
    int num;  //点数
    suit_e suit; //花色
    type_e type; //类型
    char name[17];  //牌名
    int owner;  //当前所有者,0表示牌堆,-1表示弃牌堆

} card_t;
#endif // ITEMS_H_INCLUDED
