/*
Copyright 2019 @foostan
Copyright 2020 Drashna Jaelre <@drashna>
Copyright 2021 Elliot Powell @e11i0t23

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "r2g.h"

#ifdef RGB_MATRIX_ENABLE

  // Logical Layout
  // Columns
  // Left
  // 0  1  2  3  4  5
  //                   ROWS
  // 25 24 19 18 11 10   0
  //    03    02    01
  // 26 23 20 17 12 09   1
  //    04    05    06
  // 27 22 21 16 13 08   2
  //
  //          15 14 07   3
  //
  // Right
  // 0  1  2  3  4  5
  //                    ROWS
  // 25 24 19 18 11 10   4
  //    03    02    01
  // 26 23 20 17 12 09   5
  //    04    05    06
  // 27 22 21 16 13 08   6
  //
  //          15 14 07   7
  //
  // Physical Layout
  // Columns
  // 0  1  2  3  4  5  6  7  8  9  10 11 12 13
  //                                           ROWS
  // 25 24 19 18 11 10       10 11 18 19 24 25  0
  //    03    02    01       01    02    03
  // 26 23 20 17 12 09       09 12 17 20 23 26  1
  //    04                               04
  // 27 22 21 16 13 08       08 13 16 21 22 27  2
  //          05    06       06    05
  //           15 14 07     07 14 15              3

led_config_t g_led_config = { {
    {  24,  23,  18,  17,  10,   9 },
    {  25,  22,  19,  16,  11,   8 },
    {  26,  21,  20,  15,  12,   7 },
    { NO_LED, NO_LED, NO_LED,  14,  13,   6 },
    {  51,  50,  45,  44,  37,  36 },
    {  52,  49,  46,  43,  38,  35 },
    {  53,  48,  47,  42,  39,  34 },
    { NO_LED, NO_LED, NO_LED,  41,  40,  33 }
}, {
    {  85,  16 }, {  50,  13 }, {  16,  20 }, {  16,  38 }, {  50,  48 }, {  85,  52 }, {  95,  63 },
    {  85,  39 }, {  85,  21 }, {  85,   4 }, {  68,   2 }, {  68,  19 }, {  68,  37 }, {  80,  58 },
    {  60,  55 }, {  50,  35 }, {  50,  13 }, {  50,   0 }, {  33,   3 }, {  33,  20 }, {  33,  37 },
    {  16,  42 }, {  16,  24 }, {  16,   7 }, {   0,   7 }, {   0,  24 }, {   0,  41 }, { 139,  16 },
    { 174,  13 }, { 208,  20 }, { 208,  38 }, { 174,  48 }, { 139,  52 }, { 129,  63 }, { 139,  39 },
    { 139,  21 }, { 139,   4 }, { 156,   2 }, { 156,  19 }, { 156,  37 }, { 144,  58 }, { 164,  55 },
    { 174,  35 }, { 174,  13 }, { 174,   0 }, { 191,   3 }, { 191,  20 }, { 191,  37 }, { 208,  42 },
    { 208,  24 }, { 208,   7 }, { 224,   7 }, { 224,  24 }, { 224,  41 }
}, {
    2, 2, 2, 2, 2, 2, 1,
    4, 4, 4, 4, 4, 4, 1,
    1, 4, 4, 4, 4, 4, 4,
    4, 4, 4, 1, 1, 1, 2,
    2, 2, 2, 2, 2, 1, 4,
    4, 4, 4, 4, 4, 1, 1,
    4, 4, 4, 4, 4, 4, 4,
    4, 4, 1, 1, 1
} };

#endif

#ifdef OLED_ENABLE

/* oled_rotation_t oled_init_kb(oled_rotation_t rotation) { */
/*   if (!is_keyboard_master()) { */
/*     return OLED_ROTATION_180;  // flips the display 180 degrees if offhand */
/*   } */
/*   return rotation; */
/* } */

/* enum Layers{ */
/*     L_BASE, L_LOWER, L_RAISE, L_ADJUST */
/* }; */

/* void oled_render_layer_state_r2g(void) { */
/*     oled_write_P(PSTR("Layer: "), false); */
/*     switch (get_highest_layer(layer_state)) { */
/*         case L_BASE: */
/*             oled_write_ln_P(PSTR("Default"), false); */
/*             break; */
/*         case L_LOWER: */
/*             oled_write_ln_P(PSTR("Lower"), false); */
/*             break; */
/*         case L_RAISE: */
/*             oled_write_ln_P(PSTR("Raise"), false); */
/*             break; */
/*         case L_ADJUST: */
/*             oled_write_ln_P(PSTR("Adjust"), false); */
/*             break; */
/*     } */
/* } */

/* //char keylog_str_r2g[24] = {}; */

/* const char code_to_name_r2g[60] = { */
/*     ' ', ' ', ' ', ' ', 'a', 'b', 'c', 'd', 'e', 'f', */
/*     'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', */
/*     'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', */
/*     '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', */
/*     'R', 'E', 'B', 'T', '_', '-', '=', '[', ']', '\\', */
/*     '#', ';', '\'', '`', ',', '.', '/', ' ', ' ', ' '}; */

/* char key_name_r2g = ' '; */
/* uint16_t last_keycode_r2g; */
/* uint8_t last_row_r2g; */
/* uint8_t last_col_r2g; */

/* void set_keylog_r2g(uint16_t keycode, keyrecord_t *record) { */
/*     key_name_r2g = ' '; */
/*     last_keycode_r2g = keycode; */
/*     if ((keycode >= QK_MOD_TAP && keycode <= QK_MOD_TAP_MAX) || */
/*         (keycode >= QK_LAYER_TAP && keycode <= QK_LAYER_TAP_MAX)) { last_keycode_r2g = keycode & 0xFF; } */
/*     if (keycode < 60) { */
/*       key_name_r2g = code_to_name_r2g[keycode]; */
/*     } */
/*     last_row_r2g = record->event.key.row; */
/*     last_col_r2g = record->event.key.col; */
/* } */

/* const char *depad_str(const char *depad_str, char depad_char) { */
/*     while (*depad_str == depad_char) ++depad_str; */
/*     return depad_str; */
/* } */

/* void oled_render_keylog_r2g(void) { */
/*     //oled_write(keylog_str_r2g, false); */
/*     const char *last_row_r2g_str = get_u8_str(last_row_r2g, ' '); */
/*     oled_write(depad_str(last_row_r2g_str, ' '), false); */
/*     oled_write_P(PSTR("x"), false); */
/*     const char *last_col_r2g_str = get_u8_str(last_col_r2g, ' '); */
/*     oled_write(depad_str(last_col_r2g_str, ' '), false); */
/*     oled_write_P(PSTR(", k"), false); */
/*     const char *last_keycode_r2g_str = get_u16_str(last_keycode_r2g, ' '); */
/*     oled_write(depad_str(last_keycode_r2g_str, ' '), false); */
/*     oled_write_P(PSTR(":"), false); */
/*     oled_write_char(key_name_r2g, false); */
/* } */

/* void render_bootmagic_status_r2g(bool status) { */
/*     /1* Show Ctrl-Gui Swap options *1/ */
/*     static const char PROGMEM logo[][2][3] = { */
/*         {{0x97, 0x98, 0}, {0xb7, 0xb8, 0}}, */
/*         {{0x95, 0x96, 0}, {0xb5, 0xb6, 0}}, */
/*     }; */
/*     if (status) { */
/*         oled_write_ln_P(logo[0][0], false); */
/*         oled_write_ln_P(logo[0][1], false); */
/*     } else { */
/*         oled_write_ln_P(logo[1][0], false); */
/*         oled_write_ln_P(logo[1][1], false); */
/*     } */
/* } */

/* void oled_render_logo_r2g(void) { */
/*     static const char PROGMEM mb_logo[] = { */
/* 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, */
/* 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, */
/* 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x0f, 0x0f, 0x0f, 0x0f, */
/* 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, */
/* 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, */
/* 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, */
/* 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, */
/* 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, */
/* 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x83, 0x83, 0x83, 0x83, 0x83, 0xff, 0xff, */
/* 0x83, 0x83, 0x83, 0x83, 0xff, 0xff, 0x83, 0x83, 0x83, 0x83, 0x83, 0xff, 0xff, 0x83, 0x83, 0x83, */
/* 0x83, 0xff, 0xff, 0x83, 0x83, 0x83, 0x83, 0x83, 0xff, 0xff, 0xff, 0xff, 0x82, 0x82, 0x82, 0x82, */
/* 0x82, 0xff, 0xff, 0x83, 0x83, 0x83, 0x83, 0xff, 0xff, 0x83, 0x83, 0x83, 0x83, 0x83, 0xff, 0xff, */
/* 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0xfc, 0xfc, 0xfc, */
/* 0x9c, 0x9c, 0xfc, 0xfc, 0xf8, 0xf8, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x78, 0x7c, 0x3c, */
/* 0x9c, 0xfc, 0xfc, 0xf8, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0xf0, 0xf8, 0xf8, 0x3c, 0x3c, */
/* 0x9c, 0xbc, 0xfc, 0xb8, 0xb8, 0xa0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, */
/* 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x41, 0x41, 0x41, 0x41, 0x41, 0xff, 0xff, */
/* 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x41, 0x41, 0x41, 0x41, 0x41, 0xff, 0xff, 0xff, 0xff, 0xff, */
/* 0xff, 0xff, 0xff, 0x41, 0x41, 0x41, 0x41, 0x41, 0xff, 0xff, 0xff, 0xff, 0x41, 0x41, 0x41, 0x41, */
/* 0x41, 0xff, 0xff, 0x7f, 0x7f, 0x7f, 0x7f, 0xff, 0xff, 0x41, 0x41, 0x41, 0x41, 0x41, 0xff, 0xff, */
/* 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x3f, 0x3f, 0x3f, */
/* 0x03, 0x07, 0x0f, 0x3f, 0x3f, 0x3e, 0x38, 0x20, 0x00, 0x00, 0x00, 0x00, 0x38, 0x3c, 0x3e, 0x3f, */
/* 0x3f, 0x3f, 0x3b, 0x39, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x0f, 0x1f, 0x3f, 0x3c, 0x38, */
/* 0x3b, 0x3b, 0x3f, 0x1f, 0x1f, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, */
/* 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xff, 0xff, */
/* 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xff, 0xff, 0xff, 0xff, 0xff, */
/* 0xff, 0xff, 0xff, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xff, 0xff, 0xff, 0xff, 0xf0, 0xf0, 0xf0, 0xf0, */
/* 0xf0, 0xff, 0xff, 0xf0, 0xf0, 0xf0, 0xf0, 0xff, 0xff, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xff, 0xff, */
/* 0xff, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, */
/* 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, */
/* 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, */
/* 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 */
/* }; */
/*     oled_write_raw_P(mb_logo, sizeof(mb_logo)); */
/*     //oled_set_cursor(oled_max_chars()/2,oled_max_lines()/2); */
/*     //oled_write_P(PSTR("R2G"), false); */
/* } */

bool oled_task_kb(void) {
    if (!oled_task_user()) { return false; }
    /* if (is_keyboard_master()) { */
    /*     oled_render_layer_state_r2g(); */
    /*     oled_render_keylog_r2g(); */
    /* } else { */
    /*     oled_render_logo_r2g(); */
    /* } */
    return false;
}

bool process_record_kb(uint16_t keycode, keyrecord_t *record) {
  /* if (record->event.pressed) { */
  /*   set_keylog_r2g(keycode, record); */
  /* } */
  return process_record_user(keycode, record);
}
#endif // OLED_ENABLE
