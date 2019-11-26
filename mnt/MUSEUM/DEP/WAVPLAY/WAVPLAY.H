#ifndef __WAVPLAY_H__
#define __WAVPLAY_H__

#include <conio.h>
#include <dos.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

////////////////////////////////////////////////////////////////////////
//
//	Wave file loader & Multivoic Player
//
//	모든 Wave파일을 8 bit/sample 로 바꾸어 읽어들이고, 다중출력해준다.
//
//	주의1:
//		동시에 여러소리를 낼경우 Sampling Rate는 마지막에 읽어들인
//		Wave로 설정된다. 정상적인 소리를 들으려면 모든 Wave파일의
//		Sampling Rate가 동일해야 할것이다.
//	주의2:
//		소프트웨어적으로 여러개의 음색을 8비트에다가 뭉쳐서 출력하게
//		되므로 너무많은 소리가 한꺼번에 날경우 깨질수 있다.
//		합성공식은..
//			출력음 = (a+b+c+d+....)
//			물론 최대,최소값처리 했음.( 0,255 )
//			-> 그냥 더하기보다 덜깨지는 방식은 없을꺼나...
//
//	읽을수 있는 Sampling Rate
//			- 44100, 22050, 11025, 8000 [Hz]
//	읽을수 있는 Bit/sample
//			- 8, 16
//
//				Coded by Gamza @~..
//
//			dgPIX entertainment
//
//
//	초기화함수 2개
//  ==========================================
//	void _dgSounduninitVoc(void);	<-하드웨어 초기화
//	int  _dgSoundInitVocMaxData( int num );	<-한번에 읽어들일수 있는 Wav갯수설정(포인터할당용)
//
//	Wave file 읽기 함수
//	========================================================
//	int  _dgSoundLoadVocDatas( char *wavfname, int num );
//			<- 파일명과 웨이브번호, 이미 그번호에 들어있는 웨이브가 있으면
//				지워버리고 새로지정한 웨이브가 로딩된다.
//				이함수로 할당한 번호를 가지고 웨이브의 출력,중지등을 하게된다.
//
//	Wave Play 함수
//	=========================================================
//	void dgVoicePlay( int num );
//	void dgVoiceStop( int num );
//			<- 같은 웨이브라도 여러번 출력할수 있다.
//
////////////////////////////////////////////////////////////////////////

extern "C" {

void _dgSounduninitVoc(void);
int _dgSoundInitVocMaxData(int num);
int _dgSoundLoadVocDatas(const char *wavfname, int num);
void dgVoicePlay(int num);
void dgVoiceStop(int num);
}

#endif
