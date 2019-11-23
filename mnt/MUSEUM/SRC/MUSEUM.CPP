#include <conio.h>
#include <dos.h>
#include <gat_wc/gat_wc.h>
#include <museum/common.h>
#include <museum/director.h>
#include <museum/museum.h>
#include <museum/screen.h>
#include <stdio.h>

#define GAT_MUSIC_NO (0)

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

    // Init sound
    gatInit();
    gatSetVol(100);

    // Init keyboard
    g_dos_kbd_isr = _dos_getvect(0x09);
    _dos_setvect(0x09, _museum_kbd_isr);
}

void Museum::deinit() {
    // Deinit sound
    gatClose();

    // Deinit graphics
    Screen::shared()->setMode(0x03);

    // Deinit keyboard
    _dos_setvect(0x09, g_dos_kbd_isr);
}

void Museum::run() {
    m_running = TRUE;
    while (m_running) {
        Director::shared()->drawScene();
        Screen::shared()->update();
    }
}

void Museum::quit() {
    m_running = FALSE;
}

void Museum::playMusic(const char *filename) {
    if (gatLoad(GAT_MUSIC_NO, (unsigned char *)filename)) {
        gatPlay(GAT_MUSIC_NO);
    }
}

void Museum::stopMusic() {
    gatStop();
    gatFree(GAT_MUSIC_NO);
}

boolean_t Museum::getKeyState(key_t key) {
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

Museum::Museum() {
}

Museum::~Museum() {
}

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
