#include "gui.h"

mouse_msg msg;
int mouse_x, mouse_y;//���ڻ�ȡ�������
gui_t gui = {newimage(), newimage(), newimage(), newimage(), newimage(), newimage()};

int pos[8] = {1070, 430, 1040, 150, 520, 20, 30, 150};  //�������ʱ��,�佫λ��

//����ֵת��Ϊ�ַ���,�ַ���������ֵ
///ͬһ����ڲ��ܶ�ε���,�� i = strcat(Myitoa(12), Myitoa(34) );�ᵼ�½������
char* Myitoa(int num)
{
    static char p[11];
    memset(p, 0, sizeof(p));
    itoa(num, p, 10);
    return p;
}

//����ƴ��,����ֵΪƴ�Ӻ���ַ���
///ͬһ����ڶ�ε���Link��ʹ��Ƕ�׶��ǲ���,�� Link(Link(Link(str1, str2), str3), str4)
///����д�� Link( Link(str1, str2), Link(str3, str4) )
char* Link(char* str1, char* str2)
{
    static char p[101];
    memset(p, 0, sizeof(p));
    strcpy(p, str1);
    strcat(p, str2);

    static char pcopy[101];
    memset(pcopy, 0, sizeof(p));
    strcpy(pcopy, p);
    return pcopy;
}

//�����ĳ�����
void Rect(int left, int top, int right, int bottom, color_t color, PIMAGE img = NULL)
{
    setcolor(color, img);
    line(left, top, right, top, img);
    line(right, top, right, bottom, img);
    line(right, bottom, left, bottom, img);
    line(left, bottom, left, top, img);
}

//������������
void Tri(int x1, int y1, int x2, int y2, int x3, int y3, color_t color, PIMAGE img = NULL)
{
    setcolor(color, img);
    line(x1, y1, x2, y2, img);
    line(x2, y2, x3, y3, img);
    line(x3, y3, x1, y1, img);
}

//ͼ��ճ������
///����ʱ��·��תΪchar*,����warning
///�˺�������ʱ��mode���ݵ�TRANSPARENT�ѱ�����Ϊ1
void PasteImage(char* path, int x, int y, PIMAGE img = NULL, int mode = 0, color_t color = BLACK)
{
    PIMAGE temp = newimage();
    getimage(temp, path);
    switch(mode)
    {
    case 0:
    {
        putimage_alphablend(img, temp, x, y, 255);
        break;
    }
    case 1:
    {
        putimage_transparent(img, temp, x, y, color);
        break;
    }
    }
    delimage(temp);
}

//��ʼ����
void ProgramStart(void)
{
    initgraph(1200, 600);

    PasteImage((char*)".\\Textures\\origin.png", 0, 0);

    setcolor(EGERGB(236, 95, 0));
    setbkmode(TRANSPARENT);
    setfont(120, 0, "����");
    outtextxy(420, 100, "����ɱ");

    setfillcolor(EGERGB(136, 0, 21));
    bar(500, 350, 700, 400);

    setcolor(EGERGB(234, 234, 255));
    setbkmode(TRANSPARENT);
    setfont(40, 0, "����");
    outtextxy(520, 355, "��ʼ��Ϸ");

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

//��ʼ��Ϸʱ�������
void GameGuiInit(void)
{
    setbkcolor(BLACK);
    cleardevice();
    //ͼ���ʼ��
    getimage(gui.background, 0, 0, 1200, 600);
    getimage(gui.frame, 0, 0, 1200, 600);
    getimage(gui.general, 0, 0, 1200, 600);
    getimage(gui.playercard, 0, 0, 1200, 600);
    getimage(gui.selector, 0, 0, 1200, 600);
    getimage(gui.tips, 0, 0, 1200, 600);

    PasteImage((char*)".\\Textures\\background.png", 0, 0, gui.background);

    //�������
    setfillcolor(EGERGB(71, 98, 32), gui.frame);
    bar(0, 450, 1070, 600, gui.frame);

    //���Ʊ߽�
    setcolor(YELLOW, gui.frame);
    line(150, 450, 150, 600, gui.frame);
    for(int i = 1; i <= 3; i++)  line(0, 450 + 37.5 * i, 150, 450 + 37.5 * i, gui.frame);
    for(int i = 1; i <= 8; i++)  line(150 + 100 * i, 450, 150 + 100 * i, 600, gui.frame);

    //���ư�ť
    ///ʹ��������л���ť�������
    Rect(960, 510, 1050, 535, YELLOW, gui.frame);
    Rect(960, 540, 1050, 565, YELLOW, gui.frame);
    setbkmode(TRANSPARENT, gui.frame);
    setfont(25, 0, "����", gui.frame);
    outtextxy(980, 510, "ȷ��", gui.frame);
    outtextxy(980, 540, "ȡ��", gui.frame);
    setfillcolor(LIGHTGRAY, gui.frame);
    floodfill(981, 511, YELLOW, gui.frame);
    floodfill(981, 541, YELLOW, gui.frame);
    Tri(960, 585, 975, 575, 975, 595, EGERGB(44, 182, 14), gui.frame);
    Tri(1000, 585, 985, 575, 985, 595, EGERGB(44, 182, 14), gui.frame);
    setfillcolor(EGERGB(181, 230, 29), gui.frame);
    floodfill(970, 585, EGERGB(44, 182, 14), gui.frame);
    floodfill(990, 585, EGERGB(44, 182, 14), gui.frame);

    //�����ƾ���Ϣ
    setcolor(EGERGB(255, 127, 39), gui.frame);
    setfont(40, 0, "����", gui.frame);
    outtextxy(1000, 20, "��     ��", gui.frame);
    setcolor(WHITE, gui.frame);
    setfont(20, 0, "����", gui.frame);
    outtextxy(1000, 70, "�ƶ�ʣ��:", gui.frame);

    //����������ɫ
    for(int i = 0; i <= 3; i++)
    {
        Rect(pos[2 * i], pos[2 * i + 1], pos[2 * i] + 130 ,pos[2 * i + 1] + 170, YELLOW, gui.frame);
    }


    //��Ϣ��ʾͼ��
    PasteImage((char*)".\\Textures\\condition.png", 950, 450, gui.frame, TRANSPARENT, BLACK);
    PasteImage((char*)".\\Textures\\condition.png", 30, 340, gui.frame, TRANSPARENT, BLACK);
    PasteImage((char*)".\\Textures\\condition.png", 1040, 340, gui.frame, TRANSPARENT, BLACK);
    PasteImage((char*)".\\Textures\\condition.png", 650, 130, gui.frame, TRANSPARENT, BLACK);

    //ȷ��λ��
    for(int i = 0; i <= PLAYERS - 1; i++)
    {
        player[i].controller = AI;
    }

    game.humanid = rand() % PLAYERS;
    player[game.humanid].controller = HUMAN;

    char str[] = "һ\0��\0��\0��\0";
    setfillcolor(EGERGB(238, 89, 0), gui.frame);
    setcolor(RED, gui.frame);
    fillellipse(1050, 585, 15, 15, gui.frame);
    setcolor(EGERGB(253, 228, 140), gui.frame);
    setfont(20, 0, "����", gui.frame);
    outtextxy(1040, 575, str + game.humanid * 3, gui.frame);
    setcolor(YELLOW, gui.frame);
    setfont(20, 0, "����", gui.frame);

    for(int i = 1; i <= 3; i++)
    {
        outtextxy(pos[2 * i] + 40, pos[2 * i + 1] + 170, Link(str + ((game.humanid + i) % PLAYERS) * 3, (char*)"��λ"), gui.frame);
    }
    //���ͼ��
    putimage(0, 0, gui.background);
    putimage_alphatransparent(NULL, gui.frame, 0, 0, BLACK, 150);

    return;
}

//ѡ��
void GeneralSelect(void)
{
    int forselect[4];
    int pointer;

    //ȷ����ҿ�ѡ��4��
    for(int i = 0; i <= 3; i++)
    {
        pointer = rand() % GENERALS;
        while(general_inf[pointer].selected) ++pointer %= GENERALS;
        general_inf[pointer].selected = 2;
        forselect[i] = pointer;
    }

    //ѡ����
    setcolor(EGERGB(102, 0, 15), gui.selector);
    setfillcolor(EGERGB(83, 30, 0), gui.selector);
    bar(300, 200, 900, 450, gui.selector);
    Rect(300, 200, 900, 450, YELLOW, gui.selector);

    setbkmode(TRANSPARENT, gui.selector);
    setcolor(EGERGB(249, 189, 34), gui.selector);
    setfont(40, 0, "����", gui.selector);
    outtextxy(520, 200, (char*)"ѡ���佫", gui.selector);

    //�佫����ͼƬ
    for(int i = 0; i <= 3; i++)
    {
        PasteImage(Link( (char*)".\\Textures\\Generals\\", Link( (char*)Myitoa(forselect[i]), (char*)".png")), 310 + 150 * i, 240, gui.selector);
        setfont(20, 0, "����", gui.selector);
        switch(general_inf[forselect[i]].nation)
        {
        case WEI:
        {
            setcolor(EGERGB(62, 117, 181), gui.selector);
            break;
        }
        case SHU:
        {
            setcolor(EGERGB(226, 54, 22), gui.selector);
            break;
        }
        case WU:
        {
            setcolor(EGERGB(124, 170, 125), gui.selector);
            break;
        }
        case QUN:
        {
            setcolor(EGERGB(141, 139, 95), gui.selector);
            break;
        }
        case SHEN:
        {
            setcolor(EGERGB(240, 248, 148), gui.selector);
            break;
        }
    }
        outtextxy(310 + 150 * i, 410, general_inf[forselect[i]].name, gui.selector);
    }

    putimage_transparent(NULL, gui.selector, 0, 0, BLACK);

    //��Ӧ���ѡ��
    for(; is_run(); delay_fps(60))
    {
        msg = getmouse();
        mousepos(&mouse_x, &mouse_y);

        if(msg.is_down() && mouse_x >= 310 && mouse_x <= 440 && mouse_y >= 240 && mouse_y <= 410) player[game.humanid].general = forselect[0];
        else if(msg.is_down() && mouse_x >= 460 && mouse_x <= 590 && mouse_y >= 240 && mouse_y <= 410) player[game.humanid].general = forselect[1];
        else if(msg.is_down() && mouse_x >= 610 && mouse_x <= 740 && mouse_y >= 240 && mouse_y <= 410) player[game.humanid].general = forselect[2];
        else if(msg.is_down() && mouse_x >= 760 && mouse_x <= 890 && mouse_y >= 240 && mouse_y <= 410) player[game.humanid].general = forselect[3];
        else continue;
        break;
    }
    for(int i = 0; i <= 3; i++) general_inf[forselect[i]].selected = (forselect[i] == player[game.humanid].general ? 1 : 0);

    //����ѡ��
    for(int i = 1; i <= 3; i++)
    {
        pointer = rand() % GENERALS;
        while(general_inf[pointer].selected) ++pointer %= GENERALS;
        general_inf[pointer].selected = 1;
        player[(game.humanid + i) % PLAYERS].general = pointer;
    }
    //������ѡ�佫
    for(int i = 0; i <= 3; i++)
    {
        PasteImage(Link( (char*)".\\Textures\\Generals\\",  \
            Link( (char*)Myitoa(player[(game.humanid + i) % PLAYERS].general), (char*)".png")), pos[2 * i], pos[2 * i + 1], gui.general);
    }

    //�����佫��
    char str[31];
    for(int i = 0; i <= 3; i++)
    {
        switch(general_inf[player[(game.humanid + i) % PLAYERS].general].nation)
        {
        case WEI:
        {
            setfillcolor(EGERGB(62, 117, 181), gui.general);
            strcpy(str, ".\\Textures\\Nations\\wei.png");
            break;
        }
        case SHU:
        {
            setfillcolor(EGERGB(226, 54, 22), gui.general);
            strcpy(str, ".\\Textures\\Nations\\shu.png");
            break;
        }
        case WU:
        {
            setfillcolor(EGERGB(124, 170, 125), gui.general);
            strcpy(str, ".\\Textures\\Nations\\wu.png");
            break;
        }
        case QUN:
        {
            setfillcolor(EGERGB(141, 139, 95), gui.general);
            strcpy(str, ".\\Textures\\Nations\\qun.png");
            break;
        }
        case SHEN:
        {
            setfillcolor(EGERGB(204, 193, 0), gui.general);
            strcpy(str, ".\\Textures\\Nations\\shen.png");
            break;
        }
        }
        bar(pos[2 * i], pos[2 * i + 1] - 20, pos[2 * i] + 130, pos[2 * i + 1], gui.general);
        PasteImage((char*)str, pos[2 * i], pos[2 * i + 1] - 20, gui.general, TRANSPARENT, BLACK);
        setcolor( general_inf[player[(game.humanid + i) % PLAYERS].general].nation == SHEN ? BLACK : WHITE, gui.general);
        setfont(20, 0, "����", gui.general);
        setbkmode(TRANSPARENT, gui.general);
        outtextxy(pos[2 * i] + 30, pos[2 * i + 1] - 20, general_inf[player[(game.humanid + i) % PLAYERS].general].name, gui.general);
    }

    //�ػ����
    setbkcolor(BLACK, gui.selector);
    cleardevice(gui.selector);
    putimage_transparent(NULL, gui.general, 0, 0, BLACK);
    putimage(0, 0, gui.background);
    putimage_alphatransparent(NULL, gui.frame, 0, 0, BLACK, 150);
    putimage_transparent(NULL, gui.general, 0, 0, BLACK);
}