#ifndef SKILL_H_INCLUDED
#define SKILL_H_INCLUDED

#include "actions.h"
#include "items.h"

int Bagua(player_t *recipient);
void Cixiong(player_t* executor, player_t* recipient);
int Hanbing(player_t* executor, player_t* recipient);
void Qinglong(player_t* executor, player_t* recipient);
int Guanshi(player_t* executor);
void Zhangba(player_t* executor);
int Zhuque(player_t* executor);
void Qilin(player_t* executor, player_t* recipient);

void Tannang(player_t* recipient, int id);
#endif // SKILL_H_INCLUDED
