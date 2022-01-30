#include "graphics.h"
#include "items.h"
#include "gui.h"
#include "actions.h"
#include "process.h"

#include <time.h>

int main()
{
    srand(time(NULL));
    ProgramStart();

    PileInit();
    GameGuiInit();
    GeneralSelect();
    GameStart();
    getch();
    return 0;
}
