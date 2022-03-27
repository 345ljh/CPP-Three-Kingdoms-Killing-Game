#ifndef DRAW_H_INCLUDED
#define DRAW_H_INCLUDED

#include "graphics.h"
#include "items.h"
#include "app.h"

#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <windows.h>
#include <time.h>

void ProgramStartGui(void);
void GameGuiInit(void);
void DrawGui(void);

extern const int pos[8];

#endif // DRAW_H_INCLUDED
