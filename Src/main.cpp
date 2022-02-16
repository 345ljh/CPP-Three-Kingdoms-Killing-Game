#include "graphics.h"
#include "items.h"
#include "gui.h"
#include "actions.h"
#include "process.h"

int main()
{
    ProgramStart();

    PileInit();
    GameGuiInit();
    GeneralSelect();

    GameStart();
    GameRun();
    getch();
    return 0;
}
