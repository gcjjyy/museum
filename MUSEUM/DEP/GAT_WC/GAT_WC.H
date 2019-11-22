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

/********************************************************
 * �A�� �a�� ͡�q
 */
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include <i86.h>
#include <mem.h>
#include <dos.h>

/********************************************************
 * �w�� ����
 */
#ifndef TRUE
 #define TRUE              1
 #define FALSE             0
#endif
#ifndef STOPPED               // �e�� �wȁ
 #define STOPPED           0
 #define PLAYING           1
#endif

/********************************************************
 * �q�� ���
 */

extern "C" {

void gatInit(void);
void gatClose(void);

int gatLoad(int no, unsigned char *fname);
int gatFree(int no);

int  gatPlay(int no);
void gatStop(void);

void gatSetVol(int vol);

unsigned char gatGetStatus(void);

}

#endif
