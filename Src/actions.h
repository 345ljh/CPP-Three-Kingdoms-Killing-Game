#ifndef ACTIONS_H_INCLUDED
#define ACTIONS_H_INCLUDED

#include "items.h"

extern card_t card_inf[160];
extern int card[160];
extern int nowpile;

void PileInit(void);
void Shuffle(void);
#endif // ACTIONS_H_INCLUDED
