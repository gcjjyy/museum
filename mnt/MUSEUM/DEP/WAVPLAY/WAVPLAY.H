#ifndef __WAVPLAY_H__
#define __WAVPLAY_H__

#include <conio.h>
#include <dos.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern "C" {
void _dgSounduninitVoc(void);
int _dgSoundInitVocMaxData(int num);
int _dgSoundLoadVocDatas(const char *wavfname, int num);
void dgVoicePlay(int num);
void dgVoiceStop(int num);
}

#endif
