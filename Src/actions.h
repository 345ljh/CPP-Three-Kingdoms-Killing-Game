#ifndef ACTIONS_H_INCLUDED
#define ACTIONS_H_INCLUDED

#include "items.h"
#include "gui.h"
#include "app.h"
#include "ai.h"
#include "skill.h"

#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <graphics.h>
#include <math.h>

void PileInit(void);
void Shuffle(void);

void Playcard(player_t *executor);
void Execard(player_t *executor, int target, int id, int type = -1);
void Drawcard(player_t *player, int amount);
int Throwcard(player_t *executor, player_t *player, int amount, int area = 1, int mode = 127, int add = 0);
int Showcard(player_t *executor, player_t *player, int amount, int cancel = 0, int add = 0);
int Getcard(player_t *executor, player_t *player, int amount, int area = 1, int type = 0);
int Judging(player_t *player);
void Damage(player_t *executor, player_t *player, int amount, damage_e type, int linkstart = 1);
void Recover(player_t *player, int amount);
int SelectTarget(int allowed, int maxtarget, int add = 0);
int AskWuxie(int start, int add);
int Askcard(player_t *recipient, type_e type, int add);

void Neardeath(player_t *player);
void Death(player_t *player);
void VictoryJudge(void);

#endif // ACTIONS_H_INCLUDED
