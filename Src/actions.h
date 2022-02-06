#ifndef ACTIONS_H_INCLUDED
#define ACTIONS_H_INCLUDED

#include "items.h"
#include "gui.h"

#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <graphics.h>

void PileInit(void);
void Shuffle(void);

void Takecard(player_t *player, int amount);
void Throwcard(player_t *executor, player_t *player, int amount, int area = 1);
void Showcard(player_t *executor, player_t *player, int amount);
void Getcard(player_t *executor, player_t *player, int amount, int type);
int Judging(player_t *player);
void Damage(player_t *executor, player_t *player, int amount, damage_e type);
void Recover(player_t *player, int amount);

void Neardeath(player_t *player);
void Death(player_t *player);
void VictoryJudge(void);

int AskWuxie(int start, int card);
#endif // ACTIONS_H_INCLUDED
