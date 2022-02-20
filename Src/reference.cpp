/**
 * 声明：本文件中所有代码均不进行编译，仅用于重复内容的储存
 */

#if(0)
/*----------------------常用颜色----------------------*/
可选内容边框  EGERGB(255, 215, 77)
已选内容边框  EGERGB(255, 215, 77)

/*----------------------常用按键----------------------*/
//确定键
if(msg.is_down() && mouse_x >= 960 && mouse_x <= 1050 && mouse_y >= 510 && mouse_y <= 535)
{

}

//取消键
if(msg.is_down() && mouse_x >= 960 && mouse_x <= 1050 && mouse_y >= 540 && mouse_y <= 565)
{

}

//翻页
if(msg.is_down() && mouse_x >= 960 && mouse_x <= 970 && mouse_y >= 575 && mouse_y <= 595)
{
    if(game.page > 0) game.page--;
}
if(msg.is_down() && mouse_x >= 985 && mouse_x <= 1000 && mouse_y >= 575 && mouse_y <= 595)
{
    if(recipient->cardamount > (game.page + 1) * 8) game.page++;
}

/*----------------------贴图相关----------------------*/
//确定与取消键
LineRect(960, 510, 1050, 535, EGERGB(255, 215, 77), gui.selector);
LineRect(960, 540, 1050, 565, EGERGB(255, 215, 77), gui.selector);

//覆盖选择框图层
putimage_transparent(NULL, gui.selector, 0, 0, BLACK);

//在玩家手牌上方居中输出文字,以出闪为例
char str[121] = "";
strcpy(str, (char*)"成为杀的目标，请使用一张闪");
setcolor(WHITE, gui.selector);
setfont(30, 0, "仿宋", gui.selector);
outtextxy(600 - 7.5 * strlen(str), 415, str, gui.selector);

#endif
