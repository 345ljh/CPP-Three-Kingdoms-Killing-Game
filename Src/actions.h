#ifndef ACTIONS_H_INCLUDED
#define ACTIONS_H_INCLUDED

#include "items.h"
#include "gui.h"
#include "app.h"
#include "ai.h"

#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <graphics.h>

void PileInit(void);
void Shuffle(void);

void Takecard(player_t *player, int amount);
int Throwcard(player_t *executor, player_t *player, int amount, int area = 1, int mode = 127, int add = 0);
int Showcard(player_t *executor, player_t *player, int amount, int cancel = 0, int add = 0);
int Getcard(player_t *executor, player_t *player, int amount, int area = 1, int type = 0);
int Judging(player_t *player);
void Damage(player_t *executor, player_t *player, int amount, damage_e type, int linkstart = 1);
void Recover(player_t *player, int amount);
int SelectTarget(int allowed, int maxtarget, int add = 0);

void Neardeath(player_t *player);
void Death(player_t *player);
void VictoryJudge(void);

int AskWuxie(int start, int card);
#endif // ACTIONS_H_INCLUDED
