/**
 * @file    app.cpp
 * @brief   绘图、文字等常用的功能性函数
 */

#include"app.h"

//分段取值函数,用于牌的类别
///0=基本,1装备,2=锦囊
int TypeIdentify(type_e type)
{
    if((int)type < 0x10) return 0;
    else if((int)type >= 0x90) return 2;
    else return 1;
}

//求阶乘
///为防止数据溢出或程序崩溃,当n>12时仍返回12!的值
int Fact(int n)
{
    if(n > 12) return Fact(12);
    else if(n == 0) return 1;
    else return n * Fact(n - 1);
}

//求二项分布X~B(n, p)中P(X=m)的值
double Binomial(int n, double p, int m)
{
    return Fact(n) / Fact(m) / Fact(n - m) * pow(p, m) * pow(1 - p, n - m);
}

//将数值转换为字符串,字符串即返回值
///同一语句内不能多次调用,如 i = strcat(Myitoa(12), Myitoa(34) );会导致结果错误,故使用2个相同功能的函数
char* Myitoa(int num)
{
    static char p[11];
    memset(p, 0, sizeof(p));
    itoa(num, p, 10);
    return p;
}

char* MyitoaII(int num)
{
    static char p[11];
    memset(p, 0, sizeof(p));
    itoa(num, p, 10);
    return p;
}

//文字拼接,返回值为拼接后的字符串
///同一语句内多次调用Link须使用嵌套而非并列,如 Link(Link(Link(str1, str2), str3), str4)
///不能写作 Link( Link(str1, str2), Link(str3, str4) )
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

//画空心长方形
void Rect(int left, int top, int right, int bottom, color_t color, PIMAGE img)
{
    setcolor(color, img);
    line(left, top, right, top, img);
    line(right, top, right, bottom, img);
    line(right, bottom, left, bottom, img);
    line(left, bottom, left, top, img);
}

//画空心三角形
void Tri(int x1, int y1, int x2, int y2, int x3, int y3, color_t color, PIMAGE img)
{
    setcolor(color, img);
    line(x1, y1, x2, y2, img);
    line(x2, y2, x3, y3, img);
    line(x3, y3, x1, y1, img);
}

//画边框粗细为特定值的空心矩形
///默认边框宽度为2
///前4个变量均表示内框位置
void LineRect(int left, int top, int right, int bottom, color_t color, PIMAGE img, int wide)
{
    setfillcolor(color, img);
    bar(left - wide, top - wide, right + wide, top, img);
    bar(left - wide, bottom, right + wide, bottom + wide, img);
    bar(left - wide, top, left, bottom, img);
    bar(right, top, right + wide, bottom, img);
}

//画逐渐出现,一段时间后消失的直线,用于选定目标显示
///画线完成后程序延时,此后通过调用GuiDraw覆盖直线
void LineAppear(int startx, int starty, int endx, int endy, int appear_time, int disappear_time, color_t color, PIMAGE img)
{
    setcolor(color, img);
    for(double now = DELAY_TIME; now <= appear_time; now += DELAY_TIME)
    {
        cleardevice(img);
        line(startx, starty, startx + (endx - startx) * now / appear_time, starty + (endy - starty) * now / appear_time, img);
        putimage_transparent(NULL, img, 0, 0, BLACK);
        delay_ms(DELAY_TIME);
    }
    line(startx, starty, endx, endy, img);
    delay_fps(disappear_time);
}

///重载实现一次性画多条直线
void LineAppear(int startx, int starty, int* endx, int* endy, int line_amount, int appear_time, int disappear_time, color_t color, PIMAGE img)
{
    setcolor(color, img);
    for(double now = DELAY_TIME; now <= appear_time; now += DELAY_TIME)
    {
        cleardevice(img);
        for(int i = 0; i <= line_amount - 1; i++)
            line(startx, starty, startx + (endx[i] - startx) * now / appear_time, starty + (endy[i] - starty) * now / appear_time, img);
        putimage_transparent(NULL, img, 0, 0, BLACK);
        delay_ms(DELAY_TIME);
    }
    for(int i = 0; i <= line_amount - 1; i++)
        line(startx, starty, endx[i], endy[i], img);
    delay_ms(disappear_time);
}

//图像粘贴函数
///调用时将路径转为char*,否则报warning
///此函数调用时向mode传递的TRANSPARENT已被定义为1
void PasteImage(char* path, int x, int y, PIMAGE img, int mode, color_t color)
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

//卡牌粘贴函数
///id=-2表示卡牌背面
void Pastecard(int x, int y, int id, PIMAGE img)
{
    if(id >= 0)
    {
        PasteImage( Link( Link( (char*)".\\Textures\\Cards\\", Myitoa( (int)card_inf[id].type) ), (char*)".png"),
                    x, y, img, TRANSPARENT, BLACK);
        PasteImage( Link( Link( (char*)".\\Textures\\Suits\\", Myitoa( (int)card_inf[id].suit) ), (char*)".png"),
                    x + 1, y + 1, img, TRANSPARENT, EGERGB(255, 255, 255));

        setcolor((int)card_inf[id].suit & 2 ? EGERGB(255, 0, 0) : EGERGB(11, 11, 11), img);
        setfont(15, 0, "Lucida Handwriting", img);
        outtextxy(x + 69, y + 1,
                  card_inf[id].num == 1 ? (char*)"A" :
                  card_inf[id].num == 10 ? (char*)"⒑" :
                  card_inf[id].num == 11 ? (char*)"J" :
                  card_inf[id].num == 12 ? (char*)"Q" :
                  card_inf[id].num == 13 ? (char*)"K" :
                  Myitoa( (int)card_inf[id].num), img);
    }
    else if(id == -2) PasteImage((char*)".\\Textures\\Cards\\back.png", x, y, img, TRANSPARENT, BLACK);
}

//将牌输出到屏幕上,实现弃牌堆效果,图层默认gui.throwcard
void Putcard(int id, PIMAGE img)
{
    PIMAGE temp = newimage();
    getimage(temp,(char*)".\\Textures\\Cards\\back.png");
    setbkmode(TRANSPARENT, temp);

    Pastecard(0, 0, id, temp);
    Rect(0, 0, 80, 120, EGERGB(1, 1, 1), temp);
    putimage_rotate(img, temp, rand() % 660 + 270, rand() % 220 + 210, 0.5, 0.5, rand() / 32767.0 * 6.2832);
    delimage(temp);
}

//输出卡牌信息
void Printcard(int id)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);

    printf("【");
    printf("%s ", card_inf[id].name);

    switch(card_inf[id].suit)
    {
    case SPADE:
        {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
            printf("黑桃");
            break;
        }
    case CLUB:
        {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
            printf("梅花");
            break;
        }
    case HEART:
        {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
            printf("红桃");
            break;
        }
    case DIAMOND:
        {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
            printf("方片");
            break;
        }
    default:;
    }

    switch(card_inf[id].num)
    {
    case 1:
        {
            printf("A");
            break;
        }
    case 11:
        {
            printf("J");
            break;
        }
    case 12:
        {
            printf("Q");
            break;
        }
    case 13:
        {
            printf("K");
            break;
        }
    default: printf("%d", card_inf[id].num);
    }

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
    printf("】");
}

//LineAppear的封装,输入[0, 3]的使用者与4位二进制目标,图层gui.arrow
void ShowTarget(int executor_id, int target)
{
    int amount = 0;
    for(int i = 0; i <= 3; i++) if( (target & (1 << i)) && (i != executor_id) )  amount++;

    int* endx = NULL;
    int* endy = NULL;
    endx = (int*)calloc(amount, sizeof(int));
    endy = (int*)calloc(amount, sizeof(int));

    int index = 0;
    for(int i = 0; i <= 3; i++)
    {
        if(( (target & (1 << i)) && (i != executor_id) ))
        {
            endx[index] = pos[( (i + 4 - game.humanid) % 4) * 2] + 65;
            endy[index] = pos[( (i + 4 - game.humanid) % 4) * 2 + 1] + 85;
            index++;
        }

    }
//for(int i = 0; i <= amount - 1; i++) printf("line%d:  x=%d  y=%d\n", i, endx[i], endy[i]);
    LineAppear(pos[2 * (executor_id + 4 - game.humanid)] + 65, pos[2 * (executor_id + 4 - game.humanid) + 1] + 85,
               endx, endy, amount, 400, 2000, RED, gui.arrow);

    free(endx);
    free(endy);
}

//计算数组中非-1元素个数
int ArrayOccupied(int* buf, int len)
{
    int res = 0;
    for(int i = 0; i <= len - 1; i++) if(buf[i] != -1) res++;
    return res;
}

//将数组中非-1的元素下标最小化
///len为处理后数组有效部分元素个数,blen为buf的元素个数
void IndexAlign(int* buf, int len, int blen)
{
    int *temp = NULL;
    temp = (int*)calloc(len, sizeof(int));

    int index = 0;
    for(int i = 0; index <= len - 1; i++)
        if(buf[i] != -1) temp[index++] = buf[i];

    memset(buf, 0xFF, blen * sizeof(int));

    for(int i = 0; i <= len - 1; i++) buf[i] = temp[i];

    free(temp);
}
