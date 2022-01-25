#include "gui.h"
#include "graphics.h"

#include <time.h>

mouse_msg msg;
int mouse_x, mouse_y;//用于获取鼠标坐标
gui_t gui = {newimage(), newimage(), newimage(), newimage(), newimage()};

void ProgramStart(void)
{
    srand(time(NULL));

    initgraph(1200, 600);

    PIMAGE background = newimage();
    getimage(background, ".\\Textures\\origin.png");
    putimage(0, 0, background);
    delimage(background);

    setcolor(EGERGB(236, 95, 0));
    setbkmode(TRANSPARENT);
    setfont(120, 0, "隶书");
    outtextxy(420, 100, "三国杀");

    setfillcolor(EGERGB(136, 0, 21));
    bar(500, 350, 700, 400);

    setcolor(EGERGB(234, 234, 255));
    setbkmode(TRANSPARENT);
    setfont(40, 0, "隶书");
    outtextxy(520, 355, "开始游戏");

    for(; is_run(); delay_fps(60))
    {
        msg = getmouse();

        mousepos(&mouse_x, &mouse_y);
        if(msg.is_down() && mouse_x >= 500 && mouse_x <= 700 && mouse_y >= 350 && mouse_y <= 400)
        {
            return;
        }
    }
}

void GameGuiInit(void)
{
    setbkcolor(BLACK);
    cleardevice();

    getimage(gui.background, ".\\Textures\\background.png");
    PIMAGE temp = newimage();
    getimage(temp, 0, 0, 1200, 600);

    setfillcolor(EGERGB(190, 190, 126), temp);
    bar(0, 450, 1200, 600, temp);
    putimage_alphatransparent(gui.background, temp, 0, 0, BLACK, 100);
    putimage(0, 0, gui.background);
}
