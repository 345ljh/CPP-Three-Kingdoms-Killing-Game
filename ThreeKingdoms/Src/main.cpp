#include "graphics.h"
#include "items.h"
#include "gui.h"
#include "actions.h"

#include <time.h>

game_t game;

int main()
{
    srand(time(NULL));
    ProgramStart();

    PileInit();
    GameGuiInit();
    //GeneralSelect();
    getch();
    return 0;
}
