#include "wavplay.h"

static int _dgSoundputVoc(long buf, int size);
static void _dgSoundRemakeVoice();

// 정영덕님의 DMA소스가 왓콤에 적응한거...-자료실에서 퍼왔습니다(올리신분께 감사를..)
/////////////////////////////////////////////////////////////////////////////////////
#define DMA_MASK 0x0A              // DMA 요구 MASK 어드레스
#define DMA_MODE 0x0B              // DMA 전송 모드 어드레스
#define DMA_FF_CLR 0x0C            // DMA 내부 (High/low Byte) Flip-Flop Clear 어드레스
#define CMD_DMA_MASK_SET 0x04      // DMA DREQ MASK SET 명령어
#define CMD_DMA_MASK_CLR 0x00      // DMA DREQ MASK CLEAR 명령어
#define CMD_DMA_SINGLE_WRITE 0x44  // 주변장치 -> Memory 간 전송 명령어
#define CMD_DMA_SINGLE_READ 0x48   // Memory -> 주변장치간 전송 명령어

static unsigned char _dgSoundDMAChannelPage[4] = {0x87, 0x83, 0x81, 0x82};

int _dgSoundsetDMA(unsigned long aptr, unsigned short length, char dmaChannel, char dir) {
    char page, low16Add, high16Add;
    unsigned short DMAPort, cmd;
    long avail;
    if (!length) return 1;
    if (dmaChannel > 3) return 1;
    page = (char)(aptr >> 16);
    low16Add = (char)(aptr & 0xff);
    high16Add = (char)((aptr >> 8) & 0xff);
    avail = (unsigned long)((~aptr & 0xffff) + 1);
    if (avail < length) return 2;
    if (dir)
        cmd = CMD_DMA_SINGLE_READ;
    else
        cmd = CMD_DMA_SINGLE_WRITE;
    cmd += dmaChannel;
    DMAPort = dmaChannel << 1;
    outp(_dgSoundDMAChannelPage[dmaChannel], page);  // 채널 Page Address Set
    outp(DMA_FF_CLR, 0);                             // DMA 내부 상/하위 선택 Flip-Flop Reset
    outp(DMAPort, low16Add);                         // DMA Base Low  8 Bit Address Set
    outp(DMAPort, high16Add);                        // DMA Base High 8 Bit Address Set
    DMAPort++;
    outp(DMAPort, (length & 0xff));                 // DMA Base Low  8 Bit Counter Set
    outp(DMAPort, (length >> 8));                   // DMA Base High 8 Bit Counter Set
    outp(DMA_MODE, cmd);                            // DMA Mode 를 Single Mode 로 세팅
    outp(DMA_MASK, CMD_DMA_MASK_CLR + dmaChannel);  // DMA Request Enable
    return 0;
}
int _dgSounddisableDMA(char dmaChannel) {
    if (dmaChannel > 3) return 1;
    outp(DMA_MASK, CMD_DMA_MASK_SET + dmaChannel);
    return 0;
}
int _dgSoundenableDMA(char dmaChannel) {
    if (dmaChannel > 3) return 1;
    outp(DMA_MASK, CMD_DMA_MASK_CLR + dmaChannel);
    return 0;
}
unsigned short _dgSoundreadDMACounter(char dmaChannel) {
    unsigned short low, high, DMAPort;
    if (dmaChannel > 3) return -1;
    outp(DMA_FF_CLR, 0);              // DMA 내부 상/하위 선택 Flip-Flop Reset
    DMAPort = (dmaChannel << 1) + 1;  // I/O 어드레스 계산
    low = inp(DMAPort);               // DMA 카운터 하위 8 비트
    high = inp(DMAPort);              // DMA 카운터 상위 8 비트
    return (low | (high << 8));
}
//	End of DMA Module
////////////////////////////////////////////////////////////////////////////////////////

//
//	멀티보이스 출력루틴.
//
//		최대 동시발음수 - _dgSound_MAX_VOICE 32
//
//
//		만든이: 위에꺼 만드신분...
//		뜯어고친이: 감자 @~..
//
////////////////////////////////////////////////////////////////////////////////////////

#define DMA_8BIT_DAC 0x14     // DMA를 이용한 8 비트 DAC
#define HALT_DMA 0xd0         // DMA 전송 중지
#define FREQ_DIVISER 0x40     // 샘플링 주파수 설정
#define SBL_SPEAKER_ON 0xD1   // 사블 스피커를 켠다.
#define SBL_SPEAKER_OFF 0xD3  // 사블 스피커를 끈다.
#define SBL_RESET 0x6         // 사블 리셋
#define SBL_RESET_CMD 0x1
#define SBL_DATA_AVAIL 0x0e  // 사블 데이타 분석중
#define SBL_READ_DATA 0xa    // 사블 데이타 읽기
#define SBL_READ_STATUS 0xc  // 사블 상태 읽기
#define SBL_WRITE_DATA 0xc   // 사블 데이타 쓰기
#define MIXER_ADDRESS 0x4
#define MIXER_DATA 0x5

#define REAL_SEG(P) ((((unsigned)(P)) >> 4) & 0xffff)
#define REAL_OFF(P) (((unsigned)(P)) & 0xf)
#define MAKE_PTR(S, O) (((S)&0xffff) << 4) + ((O)&0xffff)
// 사블 DAC에 명령어를 보내는 매크로 루틴
// 이 부분은 어셈으로 짜야 되지만 WC의 최적화 기능을 믿음. (^^;)
#define _dgSoundsbCommand(S)                               \
    {                                                      \
        while (inp(_dgSoundsbIO + SBL_READ_STATUS) & 0x80) \
            ;                                              \
        outp(_dgSoundsbIO + SBL_WRITE_DATA, (S));          \
    }

// 사블 스피커 매크로
// S가 1이면 스피커를 키고, 0이면 끈다.
#define _dgSoundsbSpeaker(S) _dgSoundsbCommand((S) ? SBL_SPEAKER_ON : SBL_SPEAKER_OFF)

volatile int _dgSoundvoiceBusy;  // 보이스 출력중일때 1

static int _dgSoundsbIO;   // 사블 IO 포트 번호
static int _dgSoundsbIRQ;  // 사블 IRQ 번호
static int _dgSoundsbDMA;  // 사블 DMA 채널 번호

#define _dgSound_MAX_VOICE 32
static char *_dgSoundvoice[_dgSound_MAX_VOICE];
static long _dgSoundrealSize[_dgSound_MAX_VOICE];
static long _dgSoundvoicenumber[_dgSound_MAX_VOICE];
static long _dgSoundrealDMASize = 0;

static unsigned short _dgSoundrealVocSel;  // 리얼 모드에 할당한 버퍼의 셀렉터
static char *_dgSoundrealVocBuf = NULL;    // 리얼 모드에 할당할 보이스 버퍼
static long _dgSoundvBuf[2];               // 보이스를 나눌때의 시작주소
static int _dgSoundvSize[2];               //         "         부분 크기
static char _dgSoundvFlag;                 //         "         배열 번호

static void(__interrupt __far *_dgSoundoldSbInt)(void);

static void _dgSoundcheckDivide(long physical, long l);
static void _dgSoundintSubFunc();

// 스테레오 출력 모드 설정
void SB_MixerStereo(void) {
    outp(_dgSoundsbIO + MIXER_ADDRESS, 0xe);
    outp(_dgSoundsbIO + MIXER_DATA, inp(_dgSoundsbIO + MIXER_DATA) | 2);
}
// 모노 출력모드 설정
void SB_MixerMono(void) {
    outp(_dgSoundsbIO + MIXER_ADDRESS, 0xe);
    outp(_dgSoundsbIO + MIXER_DATA, inp(_dgSoundsbIO + MIXER_DATA) & 0xfd);
}

////////////////////////////////////////////////////////////////////////
//
// 보이스 출력이 끝날때 호출되는 인터럽트 (사블 인터럽트)
//
static void __interrupt __far _dgSoundnewSbInt(void)

{
    int i;
    _disable();
    if (_dgSoundreadDMACounter(_dgSoundsbDMA) != 0xffff) {
        _enable();
        return;
    }
    inp(_dgSoundsbIO + 0xe);  // acknowledge interrupt

    if (_dgSoundvFlag) {
        _dgSoundvFlag--;
        for (i = 0; i < _dgSound_MAX_VOICE; i++) {
            if (_dgSoundrealSize[i] > 0) {
                _dgSoundrealSize[i] -= _dgSoundrealDMASize;
                _dgSoundvoice[i] += _dgSoundrealDMASize;
            }
        }
        _dgSoundputVoc(_dgSoundvBuf[_dgSoundvFlag], _dgSoundvSize[_dgSoundvFlag]);
    } else {
        _dgSoundRemakeVoice();
    }

    outp(0x20, 0x20);
    _enable();
}

// 리얼 모드 영역에 지정한 크기의 메모리를 할당한다.
static char *_dgSounddosMalloc(unsigned size, unsigned short *selector) {
    union REGS r;

    r.w.ax = 0x0100;            // DPMI allocate DOS memory
    r.w.bx = (size + 15) >> 4;  // Number of paragraphs requested
    int386(0x31, &r, &r);

    if (r.x.cflag) return NULL;  // Failed

    *selector = r.w.dx;            // 할당한 메모리의 셀렉터
    return (char *)(r.w.ax << 4);  // 리얼모드 세그먼트를 linear 번지로 바꾼다
}

// 리얼 모드 영역에 할당한 메모리를 해제한다.
static int _dgSounddosFree(char *ptr, unsigned short selector) {
    union REGS r;

    r.w.ax = 0x0101;    // DPMI free DOS memory
    r.w.dx = selector;  // Selector
    int386(0x31, &r, &r);

    if (r.x.cflag) return 1;  // Failed

    return 0;
}

////////////////////////////////////////////////////////////////////////
//
// 사블을 리셋한다.
// 리턴 : 0 - 성공
//        1 - 사블이 발견되지 않았다.
//
static int _dgSoundsbReset(int ioAddr) {
    volatile int i, j, k;

    for (i = 0; i < 512; i++) {
        // 리셋 과정 : 리셋 코드(1)를 보낸다, (기다린다), 0을 보낸다.
        outp(ioAddr + SBL_RESET, SBL_RESET_CMD);                // 사블에 리셋 코드를 보낸다.
        for (j = 0; j < 100; j++) k = inp(ioAddr + SBL_RESET);  // 기다린다.
        outp(ioAddr + SBL_RESET, 0);                            // 0을 보낸다.

        for (j = 0; j < 512; j++)
            if (inp(ioAddr + SBL_DATA_AVAIL) & 0x80) break;

        // 사블 인식값 AAh가 읽히는지 확인 한다.
        if (inp(ioAddr + SBL_READ_DATA) == 0xAA) return 0;  // 사블이 있다면 0을 리턴.
    }

    return 1;  // 실패, 사블이 발견되지 않았음.
}
////////////////////////////////////////////////////////////////////////
//
// 사블 샘플링 주파수를 설정한다.
// 입력 : rate - 사블 주파수 (8000면 8KHz로 주파수를 설정한다.)
//
void _dgSoundsetSampleRate(unsigned rate) {
    _dgSoundsbCommand(FREQ_DIVISER);                  // 사블 주파수 분주값 설정
    _dgSoundsbCommand((char)(256 - 1000000 / rate));  // 사블 주파수 설정
}

////////////////////////////////////////////////////////////////////////
//
// _dgSoundrealVocBuf의 위치와 인수인 size를 참조하여 데이타가 두 부분으로
// 나눠지는지 검사하고, 변수를 세팅한다.
// 입력 : size - 현재 출력할 보이스의 길이
//
static void _dgSoundcheckDivide(long physical, long l) {
    long page, add16;
    page = (physical & 0xFF0000L); /* DMA 堅래빨값 계フ */
    add16 = (physical & 0xFFFFL);  /* 	州� 16㎕揀 닯드레스 */
    _dgSoundvFlag = 0;
    _dgSoundvBuf[0] = physical;
    _dgSoundvSize[0] = l;
    if (add16 + l > 0x10000L) /* 堅래빨 덧ㅱ槐로가 랭닯나는가 ? */
    {
        _dgSoundvBuf[0] = page + 0x10000L; /* 멨�렀� 2묀각뗄로 나눈다. */
        _dgSoundvSize[0] = ((physical + l) & 0xFFFF);
        _dgSoundvSize[1] = l - _dgSoundvSize[0];
        _dgSoundvBuf[1] = physical;
        _dgSoundvFlag = 1;
    }
}

////////////////////////////////////////////////////////////////////////
//
// 지정한 버퍼와 크기만큼의 보이스 데이타를 DMA로 출력한다.
// 입력 : buf - 보이스 데이타가 저장된 주소
//              이 버퍼의 주소는 리얼 모드 영역에 위치해야 한다.
//        size - 보이스 데이타의 크기
//               DMA로 출력할 크기 이므로 64k를 넘지 않아야 한다.
// 리턴 : 0 - 성공
//        6 - DMA채널이 지정한 범위를 넘었다. (채널 = 0 ~ 3)
//            또는 _dgSoundsetDMA에서 데이타의 크기가 0이다.
//        7 - _dgSoundsetDMA에서 데이타의 크기가 커서 DMA로 모두 출력할 수 없다.
//
static int _dgSoundputVoc(long buf, int size) {
    _dgSoundrealDMASize = size;

    if (_dgSounddisableDMA(_dgSoundsbDMA)) return 6;
    switch (_dgSoundsetDMA((unsigned long)buf, size, _dgSoundsbDMA, 1)) {
        case 1:
            return 6;
        case 2:
            return 7;
        default:
            break;
    }
    _dgSoundsbCommand(DMA_8BIT_DAC);  // DMA로 출력할 보이스 크기를 세팅한다.
    _dgSoundsbCommand(size & 0xff);
    _dgSoundsbCommand((size >> 8) & 0xff);
    _dgSoundvoiceBusy = 1;

    return 0;
}

////////////////////////////////////////////////////////////////////////
//
// 보이스 버퍼에 있는 보이스를 사블로 출력한다.
// 입력 : voiceNumber - 보이스 번호
// 리턴 : 0 - 성공
//        3 - 보이스 번호가 최대 보이스 갯수를 넘었음
//        5 - 로드되지 않은 보이스 번호임
//        6 - DMA채널이 지정한 범위를 넘었다. (채널 = 0 ~ 3)
//            또는 _dgSoundsetDMA에서 데이타의 크기가 0이다.
//        7 - _dgSoundsetDMA에서 데이타의 크기가 커서 DMA로 모두 출력할 수 없다.
// 참조 : readyVoc, _dgSoundputVoc
//

static void _dgSoundRemakeVoice() {
    int i, j, data;
    long size;
    long playlong;
    char *playptr;
    char *vocplayptr;

    playptr = _dgSoundrealVocBuf;
    playlong = ((long)_dgSoundrealVocBuf);

    if (_dgSoundrealDMASize > 0) {
        for (i = 0; i < _dgSound_MAX_VOICE; i++) {
            if (_dgSoundrealSize[i] > 0) {
                _dgSoundrealSize[i] -= _dgSoundrealDMASize;
                _dgSoundvoice[i] += _dgSoundrealDMASize;
            }
        }
    }
    memset(playptr, 128, 65536);
    size = 0;
    for (i = 0; i < _dgSound_MAX_VOICE; i++) {
        if (_dgSoundrealSize[i] > 0) {
            vocplayptr = _dgSoundvoice[i];
            for (j = 0; j < _dgSoundrealSize[i] && j < 65535; j++) {
                data = ((long)vocplayptr[j]);
                data = ((long)playptr[j] + data - 128);
                if (data < 0)
                    data = 0;
                else if (data > 255)
                    data = 255;
                playptr[j] = data;
            }
            if (size < j) size = j;
        }
    }
    if (size <= 0) {
        _dgSoundvoiceBusy = 0;
    } else {
        _dgSoundcheckDivide(playlong, size);
        _dgSoundputVoc(_dgSoundvBuf[_dgSoundvFlag], _dgSoundvSize[_dgSoundvFlag]);
    }
}

int _dgSoundMakeVoice(int num) {
    int i;
    long passSize;

    if (_dgSoundvoiceBusy)  // 멀티 보이스 합성
    {
        _dgSoundsbCommand(HALT_DMA);  // 일단 보이스 출력을 중지한다.
        passSize = _dgSoundreadDMACounter(_dgSoundsbDMA);
        if (passSize != 0xffff) {
            passSize = _dgSoundrealDMASize - passSize;

            for (i = 0; i < _dgSound_MAX_VOICE; i++) {
                if (_dgSoundrealSize[i] > 0 && num != i) {
                    _dgSoundrealSize[i] -= passSize;
                    _dgSoundvoice[i] += passSize;
                }
            }
            _dgSoundrealDMASize = 0;
        }
    } else
        _dgSoundrealDMASize = 0;
    _dgSoundRemakeVoice();
    return 0;
}

int _dgSoundplayVoc(char *voc_data, long voc_size) {
    int i;
    for (i = 0; i < _dgSound_MAX_VOICE; i++)
        if (_dgSoundrealSize[i] <= 0) break;
    if (i >= _dgSound_MAX_VOICE) return -1;
    _dgSoundvoice[i] = voc_data;
    _dgSoundrealSize[i] = voc_size;

    _dgSoundMakeVoice(i);
    return i;
}

void dgVoiceStop(int num) {
    int i;
    if (_dgSoundvoiceBusy) {
        if (num < 0) {
            _dgSoundsbCommand(HALT_DMA);
            _dgSoundvoiceBusy = 0;
            for (i = 0; i < _dgSound_MAX_VOICE; i++) _dgSoundrealSize[i] = 0;
        } else {
            _dgSoundrealSize[num] = 0;

            for (i = 0; i < _dgSound_MAX_VOICE; i++) {
                if (_dgSoundvoicenumber[i] == num) _dgSoundrealSize[i] = 0;
            }
            _dgSoundMakeVoice(-1);
        }
    }
}

////////////////////////////////////////////////////////////////////////
//                                                                    //
//                      Sound Blaster auto detecting                  //
//               Original programming : 정영덕(Hitel;wd40)            //
//                                                                    //
////////////////////////////////////////////////////////////////////////
static void __interrupt __far _dgSoundint2(void) {
    _disable();
    inp(_dgSoundsbIO + 0xe);
    outp(0x20, 0x20);
    _dgSoundsbIRQ = 2;
    _enable();
}

static void __interrupt __far _dgSoundint3(void) {
    _disable();
    inp(_dgSoundsbIO + 0xe);
    outp(0x20, 0x20);
    _dgSoundsbIRQ = 3;
    _enable();
}

static void __interrupt __far _dgSoundint5(void) {
    _disable();
    inp(_dgSoundsbIO + 0xe);
    outp(0x20, 0x20);
    _dgSoundsbIRQ = 5;
    _enable();
}

static void __interrupt __far _dgSoundint7(void) {
    _disable();
    inp(_dgSoundsbIO + 0xe);
    outp(0x20, 0x20);
    _dgSoundsbIRQ = 7;
    _enable();
}

////////////////////////////////////////////////////////////////////////
//
//   밀리초 단위로 시간 지연을 한다. (AT 이상에서만 가능)
//
//   입력 : mSec - 밀리초 (1000mSec -> 1초)
//
static void _dgSoundwaitmSec(int mSec) {
    long tick;

    tick = (32768 * (long)mSec) / 1000;  // 초단위를 펄스 단위로 바꾼다.
    //
    // |-| |-| |-| |-| |-| |-| |-| |-| |   <- 61h 번지 비트 4
    // | |_| |_| |_| |_| |_| |_| |_| |_|
    //          한주기 32Khz
    //
    for (; tick > 0; tick--) {
        while (inp(0x61) & 0x10)
            ;
        while (!(inp(0x61) & 0x10))
            ;
    }
}

////////////////////////////////////////////////////////////////////////
//
// 사블 IO 번지, IRQ 번호, DMA 채널을 자동으로 검사하여 찾아준다.
// 리턴 : 0 - 성공적으로 찾았을때
//            ioAddr, irqNum, dmaCh에 값이 들어간다.
//        1 - 사블을 찾지 못했다.
//
int _dgSoundDetect() {
    int i, found;
    int old8259mask;
    void(interrupt * oldInt2)(void);  // 인터럽트 2,3,5,7 보관용
    void(interrupt * oldInt3)(void);
    void(interrupt * oldInt5)(void);
    void(interrupt * oldInt7)(void);

    _dgSoundsbIO = _dgSoundsbIRQ = _dgSoundsbDMA = -1;

    // 210h 에서 280h 까지 사블을 리셋시키면서 사블 IO 번지를 찾는다.
    found = 0;
    for (i = 0x210; i <= 0x280; i += 0x10) {
        if (!_dgSoundsbReset(i)) {
            found = 1;
            _dgSoundsbIO = i;
            break;
        }
    }
    if (!found) return 0;

    _disable();
    oldInt2 = _dos_getvect(0x8 + 2);
    _dos_setvect(0x8 + 2, _dgSoundint2);
    oldInt3 = _dos_getvect(0x8 + 3);
    _dos_setvect(0x8 + 3, _dgSoundint3);
    oldInt5 = _dos_getvect(0x8 + 5);
    _dos_setvect(0x8 + 5, _dgSoundint5);
    oldInt7 = _dos_getvect(0x8 + 7);
    _dos_setvect(0x8 + 7, _dgSoundint7);
    _enable();

    // 사블이 사용하는 IRQ 2,3,5,7의 인터럽트를 가능하게 한다.
    old8259mask = inp(0x21);
    outp(0x21, old8259mask & 0x53);  // 01010011 (Enable 2,3,5,7 interrupts on PIC)

    // 인터럽트를 찾기 위해서 더미 인터럽트를 발생시키는 명령을 보낸다.
    _dgSoundsbCommand(0xf2);  // interrupt request
    _dgSoundwaitmSec(400);    // 인터럽트가 발생할때까지 400 밀리초 기다린다.

    _dgSoundsbSpeaker(0);
    _dgSoundsetSampleRate(8000);

    // DMA 채널을 찾기위해서 사블이 사용하는 DMA 채널 1, 3를 이용 음성 발생
    _dgSoundsetDMA(0x30000L, 100, 1, 1);
    _dgSoundsetDMA(0x30000L, 100, 3, 1);
    _dgSoundsbCommand(DMA_8BIT_DAC);
    _dgSoundsbCommand(2);
    _dgSoundsbCommand(0);
    _dgSoundwaitmSec(400);

    _dgSoundsbDMA = 1;
    // 설치된 DMA 채널의 카운터값은 3 감소한다.
    if (_dgSoundreadDMACounter(3) == 97) {
        _dgSoundsbDMA = 3;
        //		printf("_dgSoundsbDMA = 3;\n");
    }
    if (_dgSoundreadDMACounter(1) == 97) {
        _dgSoundsbDMA = 1;
        //		printf("_dgSoundsbDMA = 1;\n");
    }

    outp(0x21, old8259mask);
    _disable();
    _dos_setvect(0x8 + 2, oldInt2);
    _dos_setvect(0x8 + 3, oldInt3);
    _dos_setvect(0x8 + 5, oldInt5);
    _dos_setvect(0x8 + 7, oldInt7);
    _enable();

    if (_dgSoundsbIRQ == -1 || _dgSoundsbDMA == -1) return 0;  // 사블을 찾지 못했다.

    _dgSoundvoiceBusy = 0;
    _dgSoundrealVocBuf = _dgSounddosMalloc(65536 * 2, &_dgSoundrealVocSel);
    if (_dgSoundrealVocBuf == NULL) return 0;

    _disable();
    _dgSoundoldSbInt = _dos_getvect(0x8 + _dgSoundsbIRQ);
    _dos_setvect(0x8 + _dgSoundsbIRQ, _dgSoundnewSbInt);  // 사블 인터럽트 루틴 설정
    outp(0x21, inp(0x21) & (~(1 << _dgSoundsbIRQ)));      // Enable interrupts on PIC
    _enable();

    _dgSoundsbSpeaker(1);
    _dgSoundsetSampleRate(8000);  // 샘플링 주파수 초기 설정

    for (i = 0; i < _dgSound_MAX_VOICE; i++) {
        _dgSoundrealSize[i] = 0;
    }

    return 1;
}

////////////////////////////////////////////////////////////////////////
//
// 사블 하드웨어 인터럽트를 원 상태로 복귀시킨다.
// 프로그램을 끝내기 전에 호출한다.
//
void _dgSounduninit(void) {
    _dgSounddosFree(_dgSoundrealVocBuf, _dgSoundrealVocSel);

    dgVoiceStop(-1);
    _dgSoundsbSpeaker(0);

    _disable();
    //_dos_setvect(0x8 + _dgSoundsbIRQ, _dgSoundoldSbInt);
    if (_dgSoundsbIRQ != 2) outp(0x21, inp(0x21) | (1 << _dgSoundsbIRQ));
    _enable();
    _dgSoundvoiceBusy = 0;
}

///////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
//
//	Wave file loader & Multivoic Player
//
//	메모리를 할당받고 Wave 파일을 로드한다.
//	모든 Wave파일을 8 bit/sample 로 바꾸어 읽어들인다.
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
//			물론 최대,최소값처리는 했음.( 0,255 )
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
////////////////////////////////////////////////////////////////////////
typedef struct {
    char chunkID[4];
    long chunkSize;
} _dgSoundWavChunk;

typedef struct {
    short wFormatTag;
    unsigned short wChannels;
    unsigned long dwSamplesPerSec;
    unsigned long dwAvgBytesPerSec;
    unsigned short wBlockAlign;
    unsigned short wBitsPerSample;
} _dgSoundFormatChunk;

typedef struct {
    unsigned short Channels;
    unsigned long sps;   // Samples per Sec
    unsigned short bps;  // Bits per Sample
    unsigned long datasize;
    unsigned char data[1];
} _dgSoundWav;

_dgSoundWav *_dgSoundloadVoc(const char *filename) {
    FILE *fp;
    long fSize;
    long i;
    long l;
    short int a, b;
    _dgSoundWav *result;

    _dgSoundWavChunk chunk;
    _dgSoundFormatChunk format;
    char checkformat = 0;
    char checkdata = 0;

    fp = fopen(filename, "rb");
    if (fp == NULL) return NULL;

    fread(&chunk, sizeof(_dgSoundWavChunk), 1, fp);
    fseek(fp, 4, SEEK_CUR);

    while (fread(&chunk, sizeof(_dgSoundWavChunk), 1, fp) > 0) {
        if (chunk.chunkID[0] == 'f' &&
            chunk.chunkID[1] == 'm' &&
            chunk.chunkID[2] == 't' &&
            checkformat == 0) {
            fread(&format, sizeof(_dgSoundFormatChunk), 1, fp);
            fseek(fp, chunk.chunkSize - sizeof(_dgSoundFormatChunk), SEEK_CUR);
            checkformat = 1;
        } else if (chunk.chunkID[0] == 'd' &&
                   chunk.chunkID[1] == 'a' &&
                   chunk.chunkID[2] == 't' &&
                   chunk.chunkID[3] == 'a' &&
                   checkdata == 0) {
            if (format.wChannels == 1) {
                if (format.wBitsPerSample == 8) {
                    fSize = chunk.chunkSize;
                    result = (_dgSoundWav *)malloc(fSize + sizeof(_dgSoundWav) - 1);
                    if (result == NULL) {
                        printf("Memory allocation error!!\n");
                        fclose(fp);
                        return NULL;
                    }
                    result->datasize = fSize;
                    fread(result->data, fSize, 1, fp);
                } else if (format.wBitsPerSample == 16) {
                    fSize = chunk.chunkSize / 2;
                    result = (_dgSoundWav *)malloc(fSize + sizeof(_dgSoundWav) - 1);
                    if (result == NULL) {
                        printf("Memory allocation error!!\n");
                        fclose(fp);
                        return NULL;
                    }
                    result->datasize = fSize;

                    for (i = 0; i < fSize; i++) {
                        fread((void *)&(a), 2, 1, fp);
                        l = a;
                        l = (l >> 8) + 128;
                        if (l < 0)
                            l = 0;
                        else if (l > 255)
                            l = 255;
                        result->data[i] = (unsigned char)l;
                    }
                }
                checkdata++;
            } else if (format.wChannels == 2) {
                if (format.wBitsPerSample == 8) {
                    fSize = chunk.chunkSize / 2;
                    result = (_dgSoundWav *)malloc(fSize + sizeof(_dgSoundWav) - 1);
                    if (result == NULL) {
                        printf("Memory allocation error!!\n");
                        fclose(fp);
                        return NULL;
                    }
                    result->datasize = fSize;

                    for (i = 0; i < fSize; i++) {
                        fread((void *)&(a), 1, 1, fp);
                        fread((void *)&(b), 1, 1, fp);
                        l = (a + b) / 2;
                        if (l < 0)
                            l = 0;
                        else if (l > 255)
                            l = 255;
                        result->data[i] = (unsigned char)l;
                    }
                } else if (format.wBitsPerSample == 16) {
                    fSize = chunk.chunkSize / 4;
                    result = (_dgSoundWav *)malloc(fSize + sizeof(_dgSoundWav) - 1);
                    if (result == NULL) {
                        printf("Memory allocation error!!\n");
                        fclose(fp);
                        return NULL;
                    }
                    result->datasize = fSize;

                    for (i = 0; i < fSize; i++) {
                        fread((void *)&(a), 2, 1, fp);
                        fread((void *)&(b), 2, 1, fp);
                        l = (a + b) / 2;
                        l = (l >> 8) + 128;
                        if (l < 0)
                            l = 0;
                        else if (l > 255)
                            l = 255;
                        result->data[i] = (unsigned char)l;
                    }
                }
                checkdata++;
            } else {
                fclose(fp);
                return NULL;
            }
        } else
            fseek(fp, chunk.chunkSize, SEEK_CUR);
        if (checkdata && checkformat) break;
    }
    fclose(fp);

    if (checkdata == 0) return NULL;
    if (checkformat == 0) {
        free(result);
        return NULL;
    }

    result->Channels = format.wChannels;
    result->sps = format.dwSamplesPerSec;
    result->bps = format.wBitsPerSample;

    return result;
}

//////////////////////////////////////////////////////////////
_dgSoundWav **_dgSoundWavDatas = NULL;
long _dgSoundMaxWavData = 0;
long _dgSoundInitOK = 0;

int _dgSoundInitVocMaxData(int num) {
    int i;
    if (num <= 0) return 0;
    if (_dgSoundWavDatas != NULL) return 0;
    _dgSoundWavDatas = (_dgSoundWav **)malloc(num * 4);
    if (_dgSoundWavDatas == NULL) return 0;
    if (_dgSoundDetect() == 0) {
        free(_dgSoundWavDatas);
        return 0;
    }
    _dgSoundMaxWavData = num;
    for (i = 0; i < _dgSoundMaxWavData; i++)
        _dgSoundWavDatas[i] = NULL;
    _dgSoundInitOK = 1;
    return 1;
}

void _dgSounduninitVoc(void) {
    int i;
    if (_dgSoundInitOK == 0) return;
    _dgSounduninit();
    if (_dgSoundWavDatas != NULL) {
        for (i = 0; i < _dgSoundMaxWavData; i++) {
            if (_dgSoundWavDatas[i] != NULL)
                free(_dgSoundWavDatas[i]);
        }
        free(_dgSoundWavDatas);
    }
    _dgSoundInitOK = 0;
}

int _dgSoundLoadVocDatas(const char *wavfname, int num) {
    if (_dgSoundInitOK == 0) return 0;
    _dgSoundWavDatas[num] = _dgSoundloadVoc(wavfname);
    if (_dgSoundWavDatas[num] == NULL) {
        printf("LoadWav Failed.. %s\n", wavfname);
        return 0;
    }

    return 1;
}

void dgVoicePlay(int num) {
    int i;
    if (_dgSoundInitOK == 0) return;
    if (_dgSoundWavDatas[num] == NULL) return;
    _dgSoundsetSampleRate(_dgSoundWavDatas[num]->sps);
    i = _dgSoundplayVoc((char *)_dgSoundWavDatas[num]->data, _dgSoundWavDatas[num]->datasize);
    _dgSoundvoicenumber[i] = num;
}
//////////////////////////////////////////////////////////////
