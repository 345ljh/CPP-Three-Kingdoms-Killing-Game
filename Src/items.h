#ifndef ITEMS_H_INCLUDED
#define ITEMS_H_INCLUDED

#include <stdio.h>
#include "graphics.h"
#include "music.h"

#define GENERALS 4 //当前已加入武将数量

typedef enum
{
    //基本牌:杀,火杀,雷杀,闪,桃,酒
    SHA = 0x00, HUOSHA, LEISHA, SHAN, TAO, JIU,
    //武器,十六进制下第2位表示范围
    ZHUGE = 0x10,
    QINGGANG = 0x20, CIXIONG, GUDING, HANBING,
    GUANSHI = 0x30, QINGLONG, ZHANGBA,
    FANGTIAN = 0x40, ZHUQUE,
    QILIN = 0x50,
    //防具
    RENWANG = 0x60, BAGUA, TENGJIA, BAIYIN,
    //-1马
    CHITU = 0x70, DAWAN, ZIXIN,
    //+1马
    JUEYING = 0x80, FEIDIAN, DILU, HUALIU,
    //普通锦囊:决斗,过拆,顺牵,万箭,南蛮,桃园,无中,五谷,无懈,火攻,铁锁,借刀
    JUEDOU = 0x90, GUOCHAI, SHUNQIAN, WANJIAN, NANMAN, TAOYUAN, WUZHONG, WUGU,
    WUXIE, HUOGONG, TIESUO, JIEDAO,
    //延时锦囊:乐,兵,闪电
    LE = 0xA0, BING, SHANDIAN,
}type_e; //牌类型
///本枚举均为牌名的拼音

typedef enum
{
    SPADE = 0x00, CLUB, HEART, DIAMOND, NONE
}suit_e; //花色

typedef enum
{
    WEI, SHU, WU, QUN, SHEN
}nation_e; //势力

typedef enum
{
    HUMAN, AI, DEAD
}controller_e; //控制者


typedef enum
{
    COMMON, FIRE, THUNDER, LOSS
}damage_e; //伤害属性

typedef enum
{
    MALE, FEMALE
}gender_e; //性别

typedef struct
{
    char name[31];  //武将名
    int id;  //对应id
    gender_e gender;  //性别
    char skill[21];  //技能名,多个技能用空格分开
    char activeskill[21]; //主动技名
    int maxhealth;  //体力上限
    nation_e nation;//势力
    int limit;  //是否拥有限定技,0=无,1=有
    int selected;  //是否已被选中(用于选将防止重复),0=未选,1=已选,2=待玩家选择
} general_t;//武将信息

typedef struct
{
    int num;  //点数
    suit_e suit; //花色
    type_e type; //类型
    char name[17];  //牌名
    int owner;  //当前所有者,-2=牌堆,-1=弃牌堆
} card_t;

typedef struct
{
    int id;  //位次
    int general;  //武将id
    controller_e controller;  //玩家或AI,以及存活情况

    int health;  //当前体力
    int cardamount;  //当前手牌数
    int maxhealth; //体力上限
    int maxcard;  //当前手牌上限

    int takecard;  //摸牌阶段摸牌数
    int nowslash;  //当前回合已使用杀次数
    int maxslash;  //每回合使用杀的最大次数
    int slashlimit;  //是否限制杀的使用次数
    int targets; //杀可指定目标数
    int spirits;  //回合内使用酒的状态:0=未使用,1=已使用且存在伤害+1效果,2=已使用

    int card[160]; //当前拥有手牌,储存id=-1代表为空
    int equips[4];  //装备区,分别为武器,防具,-1,+1
    int judges[3][2]; //判定区,判定阶段由后向前结算,每行下标为顺序,列下标0为牌id,1为类别,用于处理[国色]等技能的转化

    int chained; //是否横置
    int turned;//是否翻面

    int activeskill; //1=拥有主动触发或转换类等技能(需要另外页面释放),否则为0
    int limit;  //1=存在未使用的限定技=1,否则为0
    int awaken;  //1=存在觉醒技且已发动,否则为0

    int temp[4];  //临时用变量,用于自身技能相关计算
    int other[8];  //受到其他角色技能产生影响使用的临时变量,下标代表位次,低8位为值,高8位代表来源武将
    int *skillcard;  //因为技能效果而置于武将牌上的牌,初始化时拥有1个元素-1

}player_t;

typedef struct
{
    int humanid;  //玩家id
    int card[160];  //牌堆,其中储存0~159,分别对应card_inf中的排列
    ///一般情况下摸牌时将card[0]写入用户结构体，之后将所有牌前移1下标，并在末尾写入-1表示无牌
    int nowpile;  //当前牌堆剩余量
    int turn;  //当前轮数
    int active;  //当前行动者
    int period;  //当前阶段: 0=准备, 1=判定, 2=摸牌, 3=出牌, 4=弃牌, 5=结束
    int page; //玩家手牌界面的页码数,处理手牌>8时的显示问题
}game_t;

typedef struct
{
    PIMAGE background;  //背景图层
    PIMAGE frame;  //各类信息与边界、按钮图层
    PIMAGE general;  //武将与势力图层
    PIMAGE playercard;  //各区域卡牌图层
    PIMAGE selector;  //选牌与选定目标图层
    PIMAGE tips;  //提示文字图层
    PIMAGE throwcard;  //弃牌堆绘制图层
    PIMAGE arrow;  //指向目标图层
}gui_t;

extern game_t game;
extern general_t general_inf[GENERALS];
extern card_t card_inf[160];
extern player_t player[4];
extern gui_t gui;
extern mouse_msg msg;
extern int mouse_x, mouse_y;

#endif // ITEMS_H_INCLUDED
