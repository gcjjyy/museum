#include <conio.h>
#include <dos.h>
#include <gat_wc/gat_wc.h>
#include <museum/common.h>
#include <museum/director.h>
#include <museum/museum.h>
#include <museum/screen.h>
#include <stdio.h>
#include <time.h>
#include <wavplay/wavplay.h>

#define MAX_WAV_OUTPUT (32)

volatile char g_keys[128];
volatile char g_ekeys[128];

static void(_interrupt _far *g_dos_kbd_isr)(void);
static void _interrupt _far _museum_kbd_isr(void);

Museum *Museum::shared() {
    static Museum inst;
    return &inst;
}

void Museum::init() {
    // Init graphics
    Screen::shared()->setMode(0x13);

    // Init GAT
    gatInit();
    gatSetVol(100);

    // Init wavplay
    _dgSoundInitVocMaxData(MAX_WAV_OUTPUT);

    // Init keyboard
    g_dos_kbd_isr = _dos_getvect(0x09);
    _dos_setvect(0x09, _museum_kbd_isr);
}

void Museum::deinit() {
    // Deinit wavplay
    _dgSounduninitVoc();

    // Deinit GAT
    gatClose();

    // Deinit graphics
    Screen::shared()->setMode(0x03);

    // Deinit keyboard
    _dos_setvect(0x09, g_dos_kbd_isr);
}

void Museum::run() {
    float before;
    float now;

    before = this->getClock();
    m_running = TRUE;
    while (m_running) {
        now = this->getClock();
        Screen::shared()->clear();
        Director::shared()->drawScene(now - before);
        Screen::shared()->update();
        before = now;
    }
}

void Museum::quit() { m_running = FALSE; }

void Museum::loadGAT(int16_t num, const char *filename) {
    gatLoad(num, (unsigned char *)filename);
}

void Museum::unloadGAT(int16_t num) { gatFree(num); }

void Museum::playGAT(int16_t num) { gatPlay(num); }

void Museum::stopGAT() { gatStop(); }

void Museum::loadWAV(int16_t num, const char *filename) {
    _dgSoundLoadVocDatas(filename, num);
}

void Museum::playWAV(int16_t num) { dgVoicePlay(num); }

boolean_t Museum::getKeyState(Key key) {
    switch (key) {
        case KEY_ESC:
            return (g_keys[0x01]);
        case KEY_UP:
            return (g_ekeys[0x48]);
        case KEY_LEFT:
            return (g_ekeys[0x4b]);
        case KEY_RIGHT:
            return (g_ekeys[0x4d]);
        case KEY_DOWN:
            return (g_ekeys[0x50]);
        case KEY_ENTER:
            return (g_keys[0x1c]);
        case KEY_SPACE:
            return (g_keys[0x39]);
        case KEY_L_CTRL:
            return (g_keys[0x1d]);
        case KEY_R_CTRL:
            return FALSE;
        case KEY_L_ALT:
            return (g_keys[0x38]);
        case KEY_R_ALT:
            return (g_ekeys[0x38]);
        case KEY_L_SHIFT:
            return (g_keys[0x2a]);
        case KEY_R_SHIFT:
            return (g_keys[0x36]);
        default:
            return FALSE;
    }
}

float Museum::getClock() { return (float)clock() / CLOCKS_PER_SEC; }

Museum::Museum() {}

Museum::~Museum() {}

static void _interrupt _far _museum_kbd_isr(void) {
    static int extkey = 0;
    uint8_t scancode = (uint8_t)inp(0x60);  // read keyboard scan code

    if (scancode == 0xe0) {
        extkey = 1;  // use ekey array on next scan code
    } else {
        if (scancode & 0x80) {  // key released
            (extkey ? g_ekeys : g_keys)[scancode & 0x7f] = 0;
        } else {
            (extkey ? g_ekeys : g_keys)[scancode] = 1;
        }
        extkey = 0;
    }

    g_dos_kbd_isr();  // chain to previous keyboard ISR
}
