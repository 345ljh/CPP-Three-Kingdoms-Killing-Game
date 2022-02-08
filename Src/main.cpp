#include "graphics.h"
#include "items.h"
#include "gui.h"
#include "actions.h"
#include "process.h"

#include <time.h>

int main()
{
    //用于变量监视
    fp = fopen(".\\watch.txt", "w");

    srand(time(NULL));
    ProgramStart();

    PileInit();
    GameGuiInit();
    GeneralSelect();

    GameStart();
    GameRun();
    getch();
    return 0;
}
