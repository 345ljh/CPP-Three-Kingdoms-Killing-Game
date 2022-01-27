#include "graphics.h"
#include "items.h"
#include "gui.h"
#include "actions.h"

#include <time.h>

int main()
{
    srand(time(NULL));
    ProgramStart();

    PileInit();
    GameGuiInit();
    //getch();
    GeneralSelect();
    getch();
    return 0;
}
