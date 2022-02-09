#include"app.h"

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
void Rect(int left, int top, int right, int bottom, color_t color, PIMAGE img = NULL)
{
    setcolor(color, img);
    line(left, top, right, top, img);
    line(right, top, right, bottom, img);
    line(right, bottom, left, bottom, img);
    line(left, bottom, left, top, img);
}

//画空心三角形
void Tri(int x1, int y1, int x2, int y2, int x3, int y3, color_t color, PIMAGE img = NULL)
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

//计算数组中非-1元素个数
int ArrayOccupied(int* buf, int len)
{
    int res = 0;
    for(int i = 0; i <= len - 1; i++) if(buf[i] != -1) res++;
    return res;
}

//将数组中非-1的元素下标最小化,用于整理手牌
void IndexAlign(player_t* player)
{
    int *temp = NULL;
    temp = (int*)calloc(player->cardamount, sizeof(int));

    int index = 0;
    for(int i = 0; index <= player->cardamount - 1; i++)
        if(player->card[i] != -1) temp[index++] = player->card[i];

    memset(player->card, 0xFF, sizeof(player->card));

    for(int i = 0; i <= player->cardamount - 1; i++) player->card[i] = temp[i];

    free(temp);
}
