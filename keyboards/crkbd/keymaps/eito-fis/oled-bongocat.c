// Copyright 2021 @filterpaper
// SPDX-License-Identifier: GPL-2.0+

/* Graphical bongocat animation, driven by key press timer or WPM.
   It has left and right aligned cats optimized for both OLEDs.
   This code uses run-length encoded frames that saves space by
   encoding frames into repeated or unique byte count.
   RLE code is modified from @vectorstorm's Bongocat:
   (https://github.com/vectorstorm/qmk_firmware/tree/bongo_rle/keyboards/crkbd/keymaps/vectorstorm)
   Inspired by @j-inc's bongocat animation code
   (keyboards/kyria/keymaps/j-inc)
   Cat images courtesy of @plandevida
   Usage guide
   1 Place this file next to keymap.c or in userspace.
   2 Add the following lines into rules.mk:
        OLED_ENABLE = yes
        SRC += oled-bongocat.c
   3 Left and right aligned Bongocat is default. To save space:
      * Add 'OPT_DEFS += -DLEFTCAT' into rules.mk
      * Or 'OPT_DEFS += -DRIGHTCAT' into rules.mk
   4 To animate with WPM, add 'WPM_ENABLE = yes' into rules.mk.
     Otherwise add the following 'process_record_user()' code block into
     keymap.c to trigger animation tap timer with key presses:
        bool process_record_user(uint16_t keycode, keyrecord_t *record) {
            if (record->event.pressed) {
                extern uint32_t oled_tap_timer;
                oled_tap_timer = timer_read32();
            }
            return true;
        }
   5 The 'oled_task_user()' calls 'render_mod_status()' from "oled-icons.c"
     for secondary OLED. Review that file for usage guide or replace
	 'render_mod_status()' with your own function.
 */

#include QMK_KEYBOARD_H

#define IDLE_FRAMES 5
#define TAP_FRAMES  2
#define FRAME_DURATION 200 // milliseconds
#define TAP_INTERVAL  FRAME_DURATION*2
#define PAWS_INTERVAL FRAME_DURATION*8

// Timer duration between key presses
uint32_t oled_tap_timer = 0;

// Run-length encoded animation frames
#ifndef LEFTCAT // Right aligned Bongocat
static unsigned char const idle0[] PROGMEM = {144,
	0x3a,0x00,0x01,0x80,0x02,0x40,0x04,0x20,0x05,0x10,0x02,0x08,0x03,0x04,0x84,0x08,
	0x30,0x40,0x80,0x2e,0x00,0x03,0x80,0x31,0x00,0x83,0x18,0x64,0x82,0x05,0x02,0x01,
	0x01,0x04,0x00,0x02,0x80,0x09,0x00,0x82,0x80,0x00,0x02,0x30,0x82,0x00,0xc0,0x02,
	0xc1,0x87,0xc2,0x04,0x08,0x10,0x20,0x40,0x80,0x03,0x00,0x04,0x80,0x04,0x40,0x04,
	0x20,0x04,0x10,0x05,0x08,0x05,0x04,0x04,0x02,0x04,0x01,0x33,0x00,0x84,0xc0,0x38,
	0x04,0x03,0x07,0x00,0x03,0x0c,0x88,0x0d,0x01,0x00,0x40,0xa0,0x21,0x22,0x12,0x03,
	0x11,0x01,0x09,0x04,0x08,0x02,0x04,0x02,0x08,0x05,0x10,0x82,0x11,0x0f,0x02,0x01,
	0x36,0x00,0x05,0x80,0x05,0x40,0x05,0x20,0x05,0x10,0x05,0x08,0x05,0x04,0x82,0x02,
	0x03,0x02,0x02,0x06,0x01,0x02,0x02,0x02,0x04,0x05,0x08,0x01,0x07,0x3d,0x00};
static unsigned char const idle1[] PROGMEM = {147,
	0x3a,0x00,0x01,0x80,0x02,0x40,0x04,0x20,0x04,0x10,0x02,0x08,0x01,0x04,0x03,0x02,
	0x84,0x04,0x38,0x40,0x80,0x2e,0x00,0x03,0x80,0x31,0x00,0x82,0x1c,0xe2,0x02,0x01,
	0x04,0x02,0x01,0x01,0x04,0x00,0x02,0x80,0x09,0x00,0x82,0x80,0x00,0x02,0x30,0x82,
	0x00,0xc0,0x02,0xc1,0x87,0xc2,0x04,0x08,0x10,0x20,0x40,0x80,0x03,0x00,0x04,0x80,
	0x04,0x40,0x04,0x20,0x04,0x10,0x05,0x08,0x05,0x04,0x04,0x02,0x04,0x01,0x33,0x00,
	0x84,0x80,0x70,0x0c,0x03,0x07,0x00,0x03,0x0c,0x88,0x0d,0x01,0x00,0x40,0xa0,0x21,
	0x22,0x12,0x03,0x11,0x01,0x09,0x04,0x08,0x02,0x04,0x02,0x08,0x05,0x10,0x82,0x11,
	0x0f,0x02,0x01,0x36,0x00,0x05,0x80,0x05,0x40,0x05,0x20,0x05,0x10,0x05,0x08,0x05,
	0x04,0x82,0x02,0x03,0x02,0x02,0x06,0x01,0x02,0x02,0x02,0x04,0x05,0x08,0x01,0x07,
	0x3d,0x00};
static unsigned char const idle2[] PROGMEM = {154,
	0x33,0x00,0x02,0x80,0x05,0x00,0x01,0x80,0x02,0x40,0x04,0x20,0x04,0x10,0x83,0x08,
	0x04,0x02,0x02,0x01,0x85,0x02,0x0c,0x30,0x40,0x80,0x2e,0x00,0x03,0x80,0x31,0x00,
	0x82,0x1e,0xe1,0x02,0x00,0x02,0x01,0x02,0x02,0x01,0x01,0x04,0x00,0x02,0x80,0x09,
	0x00,0x82,0x80,0x00,0x02,0x30,0x82,0x00,0xc0,0x02,0xc1,0x87,0xc2,0x04,0x08,0x10,
	0x20,0x40,0x80,0x03,0x00,0x04,0x80,0x04,0x40,0x04,0x20,0x04,0x10,0x05,0x08,0x05,
	0x04,0x04,0x02,0x04,0x01,0x33,0x00,0x84,0x80,0x70,0x0c,0x03,0x07,0x00,0x03,0x0c,
	0x88,0x0d,0x01,0x00,0x40,0xa0,0x21,0x22,0x12,0x03,0x11,0x01,0x09,0x04,0x08,0x02,
	0x04,0x02,0x08,0x05,0x10,0x82,0x11,0x0f,0x02,0x01,0x36,0x00,0x05,0x80,0x05,0x40,
	0x05,0x20,0x05,0x10,0x05,0x08,0x05,0x04,0x82,0x02,0x03,0x02,0x02,0x06,0x01,0x02,
	0x02,0x02,0x04,0x05,0x08,0x01,0x07,0x3d,0x00};
static unsigned char const idle3[] PROGMEM = {140,
	0x3b,0x00,0x02,0x80,0x04,0x40,0x04,0x20,0x83,0x10,0x08,0x04,0x02,0x02,0x84,0x04,
	0x18,0x60,0x80,0x2f,0x00,0x03,0x80,0x31,0x00,0x82,0x3c,0xc2,0x02,0x01,0x02,0x02,
	0x02,0x04,0x82,0x02,0x01,0x10,0x00,0x02,0x60,0x82,0x00,0x81,0x02,0x82,0x86,0x84,
	0x08,0x10,0x20,0x40,0x80,0x04,0x00,0x04,0x80,0x04,0x40,0x04,0x20,0x04,0x10,0x05,
	0x08,0x05,0x04,0x04,0x02,0x04,0x01,0x33,0x00,0x84,0x80,0x70,0x19,0x06,0x07,0x00,
	0x03,0x18,0x88,0x1b,0x03,0x00,0x40,0xa0,0x22,0x24,0x14,0x03,0x12,0x01,0x0b,0x04,
	0x08,0x02,0x05,0x02,0x09,0x05,0x10,0x82,0x11,0x0f,0x02,0x01,0x36,0x00,0x05,0x80,
	0x05,0x40,0x05,0x20,0x05,0x10,0x05,0x08,0x05,0x04,0x82,0x02,0x03,0x02,0x02,0x06,
	0x01,0x02,0x02,0x02,0x04,0x05,0x08,0x01,0x07,0x3d,0x00};
static unsigned char const *idle[IDLE_FRAMES] = {
	idle0, idle0, idle1, idle2, idle3 };

static unsigned char const paws[] PROGMEM = {150,
	0x33,0x00,0x02,0x80,0x05,0x00,0x01,0x80,0x02,0x40,0x04,0x20,0x04,0x10,0x83,0x08,
	0x04,0x02,0x02,0x01,0x85,0x02,0x0c,0x30,0x40,0x80,0x2e,0x00,0x03,0x80,0x31,0x00,
	0x82,0x1e,0xe1,0x02,0x00,0x02,0x01,0x02,0x02,0x01,0x81,0x02,0x80,0x02,0x00,0x02,
	0x80,0x09,0x00,0x82,0x80,0x00,0x02,0x30,0x02,0x00,0x8a,0x01,0xe1,0x1a,0x06,0x09,
	0x31,0x35,0x01,0x8a,0x7c,0x02,0x00,0x04,0x80,0x04,0x40,0x04,0x20,0x04,0x10,0x05,
	0x08,0x05,0x04,0x04,0x02,0x04,0x01,0x33,0x00,0x84,0x80,0x70,0x0c,0x03,0x02,0x00,
	0x88,0x18,0x06,0x05,0x98,0x99,0x84,0xc3,0x7c,0x02,0x41,0x02,0x40,0x84,0x20,0x21,
	0x22,0x12,0x03,0x11,0x01,0x09,0x04,0x08,0x06,0x04,0x03,0x02,0x04,0x01,0x36,0x00,
	0x05,0x80,0x05,0x40,0x05,0x20,0x05,0x10,0x05,0x08,0x05,0x04,0x82,0x02,0x03,0x02,
	0x02,0x06,0x01,0x47,0x00};

static unsigned char const tap0[] PROGMEM = {180,
	0x33,0x00,0x02,0x80,0x05,0x00,0x01,0x80,0x02,0x40,0x04,0x20,0x04,0x10,0x83,0x08,
	0x04,0x02,0x02,0x01,0x85,0x02,0x0c,0x30,0x40,0x80,0x07,0x00,0x04,0xf8,0x05,0x00,
	0x02,0x80,0x1c,0x00,0x03,0x80,0x31,0x00,0x82,0x1e,0xe1,0x02,0x00,0x02,0x01,0x02,
	0x02,0x01,0x81,0x02,0x80,0x02,0x00,0x02,0x80,0x09,0x00,0x82,0x80,0x00,0x02,0x30,
	0x02,0x00,0x02,0x01,0x8e,0x02,0x04,0x08,0x10,0x20,0x43,0x87,0x07,0x01,0x00,0xb8,
	0xbc,0xbe,0x9f,0x02,0x5f,0x82,0x4f,0x4c,0x04,0x20,0x04,0x10,0x05,0x08,0x05,0x04,
	0x04,0x02,0x04,0x01,0x33,0x00,0x84,0x80,0x70,0x0c,0x03,0x02,0x00,0x88,0x18,0x06,
	0x05,0x98,0x99,0x84,0x43,0x7c,0x02,0x41,0x02,0x40,0x84,0x20,0x21,0x22,0x12,0x03,
	0x11,0x01,0x09,0x04,0x08,0x02,0x04,0x02,0x08,0x05,0x10,0x85,0x11,0x0f,0x01,0x3d,
	0x7c,0x05,0xfc,0x82,0x3c,0x0c,0x2e,0x00,0x05,0x80,0x05,0x40,0x05,0x20,0x05,0x10,
	0x05,0x08,0x05,0x04,0x82,0x02,0x03,0x02,0x02,0x06,0x01,0x24,0x00,0x01,0x01,0x02,
	0x03,0x20,0x00};
static unsigned char const tap1[] PROGMEM = {153,
	0x33,0x00,0x02,0x80,0x05,0x00,0x01,0x80,0x02,0x40,0x04,0x20,0x04,0x10,0x83,0x08,
	0x04,0x02,0x02,0x01,0x85,0x02,0x0c,0x30,0x40,0x80,0x2e,0x00,0x03,0x80,0x31,0x00,
	0x82,0x1e,0xe1,0x02,0x00,0x02,0x01,0x02,0x02,0x01,0x01,0x04,0x00,0x02,0x80,0x09,
	0x00,0x82,0x80,0x00,0x02,0x30,0x02,0x00,0x8a,0x01,0xe1,0x1a,0x06,0x09,0x31,0x35,
	0x01,0x8a,0x7c,0x02,0x00,0x04,0x80,0x04,0x40,0x04,0x20,0x04,0x10,0x05,0x08,0x05,
	0x04,0x04,0x02,0x04,0x01,0x33,0x00,0x84,0x80,0x70,0x0c,0x03,0x0a,0x00,0x02,0x01,
	0x86,0x00,0x40,0xa0,0x21,0x22,0x12,0x03,0x11,0x01,0x09,0x04,0x08,0x06,0x04,0x03,
	0x02,0x04,0x01,0x36,0x00,0x05,0x80,0x05,0x40,0x05,0x20,0x05,0x10,0x05,0x08,0x05,
	0x04,0x82,0x02,0x03,0x02,0x7a,0x04,0x79,0x82,0x39,0x31,0x02,0x02,0x02,0x04,0x03,
	0x08,0x02,0x88,0x82,0x87,0x80,0x3c,0x00};
static unsigned char const *tap[TAP_FRAMES] = {
	tap0, tap1 };
#endif // #ifndef LEFTCAT

#ifndef RIGHTCAT // Left aligned Bongocat
static unsigned char const left_idle0[] PROGMEM = {142,
	0x03,0x80,0x2e,0x00,0x84,0x80,0x40,0x30,0x08,0x03,0x04,0x02,0x08,0x05,0x10,0x04,
	0x20,0x02,0x40,0x01,0x80,0x3d,0x00,0x04,0x01,0x04,0x02,0x05,0x04,0x05,0x08,0x04,
	0x10,0x04,0x20,0x04,0x40,0x04,0x80,0x03,0x00,0x87,0x80,0x40,0x20,0x10,0x08,0x04,
	0xc2,0x02,0xc1,0x82,0xc0,0x00,0x02,0x30,0x82,0x00,0x80,0x09,0x00,0x02,0x80,0x04,
	0x00,0x01,0x01,0x05,0x02,0x83,0x82,0x64,0x18,0x56,0x00,0x02,0x01,0x82,0x0f,0x11,
	0x05,0x10,0x02,0x08,0x02,0x04,0x04,0x08,0x01,0x09,0x03,0x11,0x88,0x12,0x22,0x21,
	0xa0,0x40,0x00,0x01,0x0d,0x03,0x0c,0x07,0x00,0x84,0x03,0x04,0x38,0xc0,0x6d,0x00,
	0x01,0x07,0x05,0x08,0x02,0x04,0x02,0x02,0x06,0x01,0x02,0x02,0x82,0x03,0x02,0x05,
	0x04,0x05,0x08,0x05,0x10,0x05,0x20,0x05,0x40,0x05,0x80,0x11,0x00};
static unsigned char const left_idle1[] PROGMEM = {145,
	0x03,0x80,0x2e,0x00,0x84,0x80,0x40,0x38,0x04,0x03,0x02,0x01,0x04,0x02,0x08,0x04,
	0x10,0x04,0x20,0x02,0x40,0x01,0x80,0x3d,0x00,0x04,0x01,0x04,0x02,0x05,0x04,0x05,
	0x08,0x04,0x10,0x04,0x20,0x04,0x40,0x04,0x80,0x03,0x00,0x87,0x80,0x40,0x20,0x10,
	0x08,0x04,0xc2,0x02,0xc1,0x82,0xc0,0x00,0x02,0x30,0x82,0x00,0x80,0x09,0x00,0x02,
	0x80,0x04,0x00,0x01,0x01,0x04,0x02,0x02,0x01,0x82,0xe2,0x1c,0x56,0x00,0x02,0x01,
	0x82,0x0f,0x11,0x05,0x10,0x02,0x08,0x02,0x04,0x04,0x08,0x01,0x09,0x03,0x11,0x88,
	0x12,0x22,0x21,0xa0,0x40,0x00,0x01,0x0d,0x03,0x0c,0x07,0x00,0x84,0x03,0x0c,0x70,
	0x80,0x6d,0x00,0x01,0x07,0x05,0x08,0x02,0x04,0x02,0x02,0x06,0x01,0x02,0x02,0x82,
	0x03,0x02,0x05,0x04,0x05,0x08,0x05,0x10,0x05,0x20,0x05,0x40,0x05,0x80,0x11,0x00};
static unsigned char const left_idle2[] PROGMEM = {152,
	0x03,0x80,0x2e,0x00,0x85,0x80,0x40,0x30,0x0c,0x02,0x02,0x01,0x83,0x02,0x04,0x08,
	0x04,0x10,0x04,0x20,0x02,0x40,0x01,0x80,0x05,0x00,0x02,0x80,0x36,0x00,0x04,0x01,
	0x04,0x02,0x05,0x04,0x05,0x08,0x04,0x10,0x04,0x20,0x04,0x40,0x04,0x80,0x03,0x00,
	0x87,0x80,0x40,0x20,0x10,0x08,0x04,0xc2,0x02,0xc1,0x82,0xc0,0x00,0x02,0x30,0x82,
	0x00,0x80,0x09,0x00,0x02,0x80,0x04,0x00,0x01,0x01,0x02,0x02,0x02,0x01,0x02,0x00,
	0x82,0xe1,0x1e,0x56,0x00,0x02,0x01,0x82,0x0f,0x11,0x05,0x10,0x02,0x08,0x02,0x04,
	0x04,0x08,0x01,0x09,0x03,0x11,0x88,0x12,0x22,0x21,0xa0,0x40,0x00,0x01,0x0d,0x03,
	0x0c,0x07,0x00,0x84,0x03,0x0c,0x70,0x80,0x6d,0x00,0x01,0x07,0x05,0x08,0x02,0x04,
	0x02,0x02,0x06,0x01,0x02,0x02,0x82,0x03,0x02,0x05,0x04,0x05,0x08,0x05,0x10,0x05,
	0x20,0x05,0x40,0x05,0x80,0x11,0x00};
static unsigned char const left_idle3[] PROGMEM = {138,
	0x03,0x80,0x2f,0x00,0x84,0x80,0x60,0x18,0x04,0x02,0x02,0x83,0x04,0x08,0x10,0x04,
	0x20,0x04,0x40,0x02,0x80,0x3e,0x00,0x04,0x01,0x04,0x02,0x05,0x04,0x05,0x08,0x04,
	0x10,0x04,0x20,0x04,0x40,0x04,0x80,0x04,0x00,0x86,0x80,0x40,0x20,0x10,0x08,0x84,
	0x02,0x82,0x82,0x81,0x00,0x02,0x60,0x10,0x00,0x82,0x01,0x02,0x02,0x04,0x02,0x02,
	0x02,0x01,0x82,0xc2,0x3c,0x56,0x00,0x02,0x01,0x82,0x0f,0x11,0x05,0x10,0x02,0x09,
	0x02,0x05,0x04,0x08,0x01,0x0b,0x03,0x12,0x88,0x14,0x24,0x22,0xa0,0x40,0x00,0x03,
	0x1b,0x03,0x18,0x07,0x00,0x84,0x06,0x19,0x70,0x80,0x6d,0x00,0x01,0x07,0x05,0x08,
	0x02,0x04,0x02,0x02,0x06,0x01,0x02,0x02,0x82,0x03,0x02,0x05,0x04,0x05,0x08,0x05,
	0x10,0x05,0x20,0x05,0x40,0x05,0x80,0x11,0x00};
static unsigned char const *left_idle[IDLE_FRAMES] = {
	left_idle0, left_idle0, left_idle1, left_idle2, left_idle3 };

static unsigned char const left_paws[] PROGMEM = {148,
	0x03,0x80,0x2e,0x00,0x85,0x80,0x40,0x30,0x0c,0x02,0x02,0x01,0x83,0x02,0x04,0x08,
	0x04,0x10,0x04,0x20,0x02,0x40,0x01,0x80,0x05,0x00,0x02,0x80,0x36,0x00,0x04,0x01,
	0x04,0x02,0x05,0x04,0x05,0x08,0x04,0x10,0x04,0x20,0x04,0x40,0x04,0x80,0x02,0x00,
	0x8a,0x7c,0x8a,0x01,0x35,0x31,0x09,0x06,0x1a,0xe1,0x01,0x02,0x00,0x02,0x30,0x82,
	0x00,0x80,0x09,0x00,0x02,0x80,0x02,0x00,0x02,0x80,0x01,0x81,0x02,0x02,0x02,0x01,
	0x02,0x00,0x82,0xe1,0x1e,0x56,0x00,0x04,0x01,0x03,0x02,0x06,0x04,0x04,0x08,0x01,
	0x09,0x03,0x11,0x84,0x12,0x22,0x21,0x20,0x02,0x40,0x02,0x41,0x88,0x7c,0xc3,0x84,
	0x99,0x98,0x05,0x06,0x18,0x02,0x00,0x84,0x03,0x0c,0x70,0x80,0x77,0x00,0x06,0x01,
	0x02,0x02,0x82,0x03,0x02,0x05,0x04,0x05,0x08,0x05,0x10,0x05,0x20,0x05,0x40,0x05,
	0x80,0x11,0x00};

static unsigned char const left_tap0[] PROGMEM = {178,
	0x03,0x80,0x1c,0x00,0x02,0x80,0x05,0x00,0x04,0xf8,0x07,0x00,0x85,0x80,0x40,0x30,
	0x0c,0x02,0x02,0x01,0x83,0x02,0x04,0x08,0x04,0x10,0x04,0x20,0x02,0x40,0x01,0x80,
	0x05,0x00,0x02,0x80,0x36,0x00,0x04,0x01,0x04,0x02,0x05,0x04,0x05,0x08,0x04,0x10,
	0x04,0x20,0x82,0x4c,0x4f,0x02,0x5f,0x8e,0x9f,0xbe,0xbc,0xb8,0x00,0x01,0x07,0x87,
	0x43,0x20,0x10,0x08,0x04,0x02,0x02,0x01,0x02,0x00,0x02,0x30,0x82,0x00,0x80,0x09,
	0x00,0x02,0x80,0x02,0x00,0x02,0x80,0x01,0x81,0x02,0x02,0x02,0x01,0x02,0x00,0x82,
	0xe1,0x1e,0x4e,0x00,0x82,0x0c,0x3c,0x05,0xfc,0x85,0x7c,0x3d,0x01,0x0f,0x11,0x05,
	0x10,0x02,0x08,0x02,0x04,0x04,0x08,0x01,0x09,0x03,0x11,0x84,0x12,0x22,0x21,0x20,
	0x02,0x40,0x02,0x41,0x88,0x7c,0x43,0x84,0x99,0x98,0x05,0x06,0x18,0x02,0x00,0x84,
	0x03,0x0c,0x70,0x80,0x50,0x00,0x02,0x03,0x01,0x01,0x24,0x00,0x06,0x01,0x02,0x02,
	0x82,0x03,0x02,0x05,0x04,0x05,0x08,0x05,0x10,0x05,0x20,0x05,0x40,0x05,0x80,0x11,
	0x00};
static unsigned char const left_tap1[] PROGMEM = {151,
	0x03,0x80,0x2e,0x00,0x85,0x80,0x40,0x30,0x0c,0x02,0x02,0x01,0x83,0x02,0x04,0x08,
	0x04,0x10,0x04,0x20,0x02,0x40,0x01,0x80,0x05,0x00,0x02,0x80,0x36,0x00,0x04,0x01,
	0x04,0x02,0x05,0x04,0x05,0x08,0x04,0x10,0x04,0x20,0x04,0x40,0x04,0x80,0x02,0x00,
	0x8a,0x7c,0x8a,0x01,0x35,0x31,0x09,0x06,0x1a,0xe1,0x01,0x02,0x00,0x02,0x30,0x82,
	0x00,0x80,0x09,0x00,0x02,0x80,0x04,0x00,0x01,0x01,0x02,0x02,0x02,0x01,0x02,0x00,
	0x82,0xe1,0x1e,0x56,0x00,0x04,0x01,0x03,0x02,0x06,0x04,0x04,0x08,0x01,0x09,0x03,
	0x11,0x86,0x12,0x22,0x21,0xa0,0x40,0x00,0x02,0x01,0x0a,0x00,0x84,0x03,0x0c,0x70,
	0x80,0x6c,0x00,0x82,0x80,0x87,0x02,0x88,0x03,0x08,0x02,0x04,0x02,0x02,0x82,0x31,
	0x39,0x04,0x79,0x02,0x7a,0x82,0x03,0x02,0x05,0x04,0x05,0x08,0x05,0x10,0x05,0x20,
	0x05,0x40,0x05,0x80,0x11,0x00};
static unsigned char const *left_tap[TAP_FRAMES] = {
	left_tap0, left_tap1 };
#endif // #ifndef RIGHTCAT

//----------------------------------------------------------
// Sync
#include "print.h"
#include <string.h>
#include <transactions.h>

typedef struct {
  uint32_t oled_time;
  uint32_t anim_time;
} Counters;

Counters *counters;

void slave_counter_sync(uint8_t initiator2target_buffer_size, const void* initiator2target_buffer, uint8_t target2initiator_buffer_size, void* target2initiator_buffer) {
    const Counters* recv = (const Counters*)initiator2target_buffer;
    counters->oled_time = recv->oled_time;
    counters->anim_time = recv->anim_time;
}

void keyboard_post_init_user(void) {
    debug_enable = true;
    counters = malloc(sizeof(Counters));
    counters->oled_time = 0;
    counters->anim_time = 0;
    transaction_register_rpc(RPC_ID_COUNTERS, slave_counter_sync);
}

void user_state_sync(void) {
    if (is_keyboard_master()) {
        static uint32_t last_sync = 0;
        if (timer_elapsed32(last_sync) > 100) {
            if (transaction_rpc_send(RPC_ID_COUNTERS, sizeof(counters), counters)) {
              last_sync = timer_read32();
              dprint("Performed RPC call!\n");
            } else  {
              dprint("Failed to perform RPC call!\n");
            }
        }
    }
}

void housekeeping_task_user(void) {
    // Data sync from master to slave
    user_state_sync();
}


// RLE decoding loop that reads count from frame index
// If count >= 0x80, next (count - 128) bytes are unique
// If count < 0x80, next byte is repeated by count
static void decode_frame(unsigned char const *frame) {
	uint16_t cursor = 0;
	uint8_t size    = pgm_read_byte(frame);
	uint8_t i       = 1;

	oled_set_cursor(0,0);
	while (i < size) {
		uint8_t count = pgm_read_byte(frame + i); i++;
		if (count & 0x80) {
			// Next count-128 bytes are unique
			count &= ~(0x80);
  		for (uint8_t uniqs = 0; uniqs < count; ++uniqs) {
				uint8_t byte = pgm_read_byte(frame + i); i++;
				oled_write_raw_byte(byte, cursor++);
			}
		} else {
			// Next byte is repeated by count
			uint8_t byte = pgm_read_byte(frame + i); i++;
			for (uint8_t reps = 0; reps < count; ++reps) {
				oled_write_raw_byte(byte, cursor++);
			}
		}
	}
}


static void render_cat_idle(void) {
	static uint8_t index = 0;
	index = index < IDLE_FRAMES - 1 ? index + 1 : 0;

#if defined(LEFTCAT)
	decode_frame(left_idle[index]);
#elif defined(RIGHTCAT)
	decode_frame(idle[index]);
#else
	decode_frame(is_keyboard_left() ? left_idle[index] : idle[index]);
#endif
}


static void render_cat_paws(void) {
#if defined(LEFTCAT)
	decode_frame(left_paws);
#elif defined(RIGHTCAT)
	decode_frame(paws);
#else
	decode_frame(is_keyboard_left() ? left_paws : paws);
#endif
}


static void render_cat_tap(void) {
	static uint8_t index = 0;
	index = (index + 1) & 1;

#if defined(LEFTCAT)
	decode_frame(left_tap[index]);
#elif defined(RIGHTCAT)
	decode_frame(tap[index]);
#else
	decode_frame(is_keyboard_left() ? left_tap[index] : tap[index]);
#endif
}


static void render_bongocat(void) {
	// Timer duration between animation frames
	static uint16_t anim_timer = 0;

  if (is_keyboard_master()) {
    counters->oled_time = timer_elapsed32(oled_tap_timer);
    counters->anim_time = timer_elapsed32(anim_timer);
  }

	void animate_cat(void) {
		if (counters->oled_time < TAP_INTERVAL) {
			render_cat_tap();
		} else if (counters->oled_time < PAWS_INTERVAL) {
			render_cat_paws();
		} else {
			render_cat_idle();
		}
	}

	if (counters->oled_time > OLED_TIMEOUT) {
		oled_off();
	} else if (counters->anim_time > FRAME_DURATION) {
		anim_timer = timer_read();
		animate_cat();
	}
}


// Init and rendering calls
oled_rotation_t oled_init_user(oled_rotation_t const rotation) {
#if defined(LEFTCAT)
  return rotation;
#elif defined(RIGHTCAT)
  return OLED_ROTATION_180;
#else
  return is_keyboard_left() ? rotation : OLED_ROTATION_180;
#endif
}

bool oled_task_user(void) {
  render_bongocat();
	return false;
}
