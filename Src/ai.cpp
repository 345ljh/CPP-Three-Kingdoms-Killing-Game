#include "ai.h"

//弃牌权重比对,返回值叫大者优先弃置
///state中元素按顺序代表:[手牌区]杀,闪,桃/酒,无懈,其他普通锦囊,延时锦囊,装备,[判定区],[装备区]武器,防具,-1,+1
///id低16位为card_inf对应的id,高8位表示区域
int StateCompareAi(int state[13], int id)
{
    int area = id >> 16;
    id &= 0xFF;
    return
    ( (int)card_inf[id].type <= 2 && !area) ? state[0] :
    (card_inf[id].type == SHAN && !area)? state[1] :
    ((card_inf[id].type == TAO || card_inf[id].type == JIU) && !area) ? state[2] :
    (card_inf[id].type == WUXIE && !area) ? state[3] :
    ( ((int)card_inf[id].type & 0xF0) == 0x90 && !area) ? state[4] :
    ( ((int)card_inf[id].type & 0xF0) == 0xA0 && !area) ? state[5] :
    !area ? state[6] :
    area == 2 ? state[7] :
    ((int)card_inf[id].type >= 0x10 && (int)card_inf[id].type < 0x60) ? state[8] :
    ( ((int)card_inf[id].type & 0xF0) == 0x60) ? state[9] :
    ( ((int)card_inf[id].type & 0xF0) == 0x70) ? state[10] : state[11];
}
