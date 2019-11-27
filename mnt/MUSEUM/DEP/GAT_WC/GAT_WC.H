/******************************************************************
 * GAT_WC.H
 *
 * GAT Library
 *
 * Programmed in Watcom C/C++ 10.0a
 * Programmed by .PRoGRaMmeR.
 *                .1997-1998.
 */

#ifndef __prgrmr_LIB_Music_AdLib_GAT__
#define __prgrmr_LIB_Music_AdLib_GAT__

#include <dos.h>
#include <i86.h>
#include <malloc.h>
#include <mem.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif
#ifndef STOPPED
#define STOPPED 0
#define PLAYING 1
#endif

extern "C" {
void gatInit(void);
void gatClose(void);

int gatLoad(int no, unsigned char *fname);
int gatFree(int no);

int gatPlay(int no);
void gatStop(void);

void gatSetVol(int vol);

unsigned char gatGetStatus(void);
}

#endif
