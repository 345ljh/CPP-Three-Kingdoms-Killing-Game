/**
 * @file    music.cpp
 * @brief   音乐播放
 */

#include "music.h"

MUSIC bgm;
MUSIC musicplayer;

//播放背景音乐
///程序在DrawGui中检测音乐是否播放完毕以实现循环
void BackgroundMusic(void)
{
    if (!bgm.IsOpen())
    {
        bgm.OpenFile(".\\Music\\bgm.mp3");
        bgm.SetVolume(0.4);
        bgm.Play(0);
        musicplayer.OpenFile(".\\Music\\bgm.mp3");
    }
    else
    {
        if(bgm.GetPlayStatus() == MUSIC_MODE_STOP) bgm.Play(0);
    }
}

//播放卡牌、状态等音效
///所有音效共用一个MUSIC类,每次调用前关闭先前的文件后打开新文件
void PlayMusic(char* path)
{
    musicplayer.Close();
    musicplayer.OpenFile(path);
    if(!musicplayer.IsOpen()) printf("%s\n", path);
    musicplayer.Play(0);
}
