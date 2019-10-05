/* Copyright 2015-2017 Jack Humbert
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include QMK_KEYBOARD_H
#include "muse.h"

enum preonic_layers {
  _QWERTY,
  _GAME,
  _PROG,
  _LOWER,
  _RAISE,
  _ADJUST,
  _ACCENT,
  _NAV,
  _GUIMGR,
};

enum preonic_keycodes {
  QWERTY = SAFE_RANGE,
  GAME,
  PROG,
  LOWER,
  RAISE,
  BACKLIT
};

#define CxESC  MT(MOD_LCTL, KC_ESC)  // left control when held or escape
#define CxENT  MT(MOD_RCTL, KC_ENT)  // right control when held or enter
#define SxENT  MT(MOD_RSFT, KC_ENT)  // right shift when held or enter
#define xSCLN  LT(_NAV, KC_SCLN)     // navigation layer when held or semicolon
#define xACC   MO(_ACCENT)   // accent layer when held or semicolon
#define xGUI   MO(_GUIMGR)
#define ALT_F4 LALT(KC_F4)

enum unicode_names {
	AA_u, AU_u, EA_u, IA_u,
	OA_u, OU_u, OJ_u,
	UA_u, UU_u, UJ_u,

	AA_l, AU_l, EA_l, IA_l,
	OA_l, OU_l, OJ_l,
	UA_l, UU_l, UJ_l,

	EUR, SHSS
};

#define accPair(x, v) \
	[x##_u] = v, \
	[x##_l] = v + 0x20

const uint32_t PROGMEM unicode_map[] = {
	accPair(AA, 0x00C1),
	accPair(AU, 0x00C4),
	accPair(EA, 0x00C9),
	accPair(IA, 0x00CD),
	accPair(OA, 0x00D3),
	accPair(OU, 0x00D6),
	[OJ_u] = 0x0150,
	[OJ_l] = 0x0151,
	accPair(UA, 0x00DA),
	accPair(UU, 0x00DC),
	[UJ_u] = 0x0170,
	[UJ_l] = 0x0171,

	[EUR] = 0x20AC, // €
	[SHSS] = 0x00DF, // ß
};

#define xc(x) XP(x##_l, x##_u)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/* Qwerty
 * ┌──────┬──────┬──────┬──────┬──────┬──────┬──────┬──────┬──────┬──────┬──────┬──────┐
 * │   `  │   1  │   2  │   3  │   4  │   5  │   6  │   7  │   8  │   9  │   0  │   \  │
 * ├──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┤
 * │ Tab  │   Q  │   W  │   E  │   R  │   T  │   Y  │   U  │   I  │   O  │   P  │ Bksp │
 * ├──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┤
 * │ Esc  │   A  │   S  │   D  │   F  │   G  │   H  │   J  │   K  │   L  │   ;  │   "  │
 * ├──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┤
 * │ Shift│   Z  │   X  │   C  │   V  │   B  │   N  │   M  │   ,  │   .  │   /  │Enter │
 * ├──────┼──────┼──────┼──────┼──────┼──────┴──────┼──────┼──────┼──────┼──────┼──────┤
 * │ Ctrl │ GUI  │ LAlt │ F20  │Lower │    Space    │Raise │ Left │ Down │  Up  │Right │
 * └──────┴──────┴──────┴──────┴──────┴─────────────┴──────┴──────┴──────┴──────┴──────┘
 */
[_QWERTY] = LAYOUT_preonic_1x2uC( \
  KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_BSLS, \
  KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSPC, \
  KC_ESC,  KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, \
  KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_ENT,  \
  KC_LCTL, KC_LGUI, KC_LALT, xACC,    LOWER,       KC_SPC,       RAISE,   KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT  \
),

/* Game
 * ┌──────┬──────┬──────┬──────┬──────┬──────┬──────┬──────┬──────┬──────┬──────┬──────┐
 * │   `  │   1  │   2  │   3  │   4  │   5  │   6  │   7  │   8  │   9  │   0  │   \  │
 * ├──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┤
 * │ Tab  │   Q  │   W  │   E  │   R  │   T  │   Y  │   U  │   I  │   O  │   P  │ Bksp │
 * ├──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┤
 * │ Ctrl │   A  │   S  │   D  │   F  │   G  │   H  │   J  │   K  │   L  │   ;  │   "  │
 * ├──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┤
 * │ Shift│   Z  │   X  │   C  │   V  │   B  │   N  │   M  │   ,  │   .  │   /  │Enter │
 * ├──────┼──────┼──────┼──────┼──────┼──────┴──────┼──────┼──────┼──────┼──────┼──────┤
 * │ F13  │ F14  │ LAlt │ F15  │Lower │    Space    │Raise │ Left │ Down │  Up  │Right │
 * └──────┴──────┴──────┴──────┴──────┴─────────────┴──────┴──────┴──────┴──────┴──────┘
 */
[_GAME] = LAYOUT_preonic_1x2uC( \
  KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_BSLS, \
  KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSPC, \
  KC_LCTL, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, \
  KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_ENT,  \
  KC_F13,  KC_F14,  KC_LALT, KC_F15,  LOWER,       KC_SPC,       RAISE,   KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT  \
),

/* Prog
 * ┌──────┬──────┬──────┬──────┬──────┬──────┬──────┬──────┬──────┬──────┬──────┬──────┐
 * │   `  │   1  │   2  │   3  │   4  │   5  │   6  │   7  │   8  │   9  │   0  │  \   │
 * ├──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┤
 * │ Tab  │   Q  │   W  │   E  │   R  │   T  │   Y  │   U  │   I  │   O  │   P  │ Bksp │
 * ├──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┤
 * │C|Esc │   A  │   S  │   D  │   F  │   G  │   H  │   J  │   K  │   L  │Nav|; │  '   │
 * ├──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┤
 * │Shift │   Z  │   X  │   C  │   V  │   B  │   N  │   M  │   ,  │   .  │   /  │S|Entr│
 * ├──────┼──────┼──────┼──────┼──────┼──────┴──────┼──────┼──────┼──────┼──────┼──────┤
 * │ Mgr  │ GUI  │ LAlt │Accent│Lower │    Space    │Raise │Accent│ RAlt │ GUI  │ Mgr  │
 * └──────┴──────┴──────┴──────┴──────┴─────────────┴──────┴──────┴──────┴──────┴──────┘
 */
[_PROG] = LAYOUT_preonic_1x2uC( \
  KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_BSLS, \
  KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSPC, \
  CxESC,   KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    xSCLN,   KC_QUOT, \
  KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, SxENT,   \
  xGUI,    KC_LGUI, KC_LALT, xACC,    LOWER,       KC_SPC,       RAISE,   xACC,    KC_RALT, KC_RGUI, xGUI     \
),

/* Lower
 * ┌──────┬──────┬──────┬──────┬──────┬──────┬──────┬──────┬──────┬──────┬──────┬──────┐
 * │ Esc  │   !  │   @  │   #  │   $  │   %  │   ^  │   &  │   *  │   (  │   )  │      │
 * ├──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┤
 * │   ~  │  F9  │  F10 │  F11 │  F12 │      │   5  │   6  │   7  │   8  │   9  │ Del  │
 * ├──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┤
 * │ Esc  │  F5  │  F6  │  F7  │  F8  │      │   0  │   1  │   2  │   3  │   4  │  \   │
 * ├──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┤
 * │ Shift│  F1  │  F2  │  F3  │  F4  │      │   +  │   -  │   =  │      │   _  │Shift │
 * ├──────┼──────┼──────┼──────┼──────┼──────┴──────┼──────┼──────┼──────┼──────┼──────┤
 * │ Ctrl │      │      │      │      │    Bksp     │      │ Mute │ Vol- │ Vol+ │      │
 * └──────┴──────┴──────┴──────┴──────┴─────────────┴──────┴──────┴──────┴──────┴──────┘
 */
[_LOWER] = LAYOUT_preonic_1x2uC( \
  KC_ESC,  KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC, KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, _______, \
  KC_TILD, KC_F9,   KC_F10,  KC_F11,  KC_F12,  _______, KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_DEL,  \
  KC_ESC,  KC_F5,   KC_F6,   KC_F7,   KC_F8,   _______, KC_0,    KC_1,    KC_2,    KC_3,    KC_4,    KC_BSLS, \
  KC_LSFT, KC_F1,   KC_F2,   KC_F3,   KC_F4,   _______, KC_PLUS, KC_MINS, KC_EQL,  _______, KC_UNDS, KC_RSFT, \
  KC_LCTL, _______, _______, _______, _______,     KC_BSPC,      _______, KC_MUTE, KC_VOLD, KC_VOLU, _______  \
),

/* Raise
 * ┌──────┬──────┬──────┬──────┬──────┬──────┬──────┬──────┬──────┬──────┬──────┬──────┐
 * │ Esc  │   !  │   @  │   #  │   $  │   %  │   ^  │   &  │   *  │   (  │   )  │      │
 * ├──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┤
 * │   `  │  F9  │  F10 │  F11 │  F12 │      │   !  │   (  │   )  │   {  │   }  │ Del  │
 * ├──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┤
 * │ Ctrl │  F5  │  F6  │  F7  │  F8  │      │   #  │   [  │   ]  │   -  │   =  │  |   │
 * ├──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┤
 * │ Shift│  F1  │  F2  │  F3  │  F4  │      │   *  │   _  │   +  │      │   $  │      │
 * ├──────┼──────┼──────┼──────┼──────┼──────┴──────┼──────┼──────┼──────┼──────┼──────┤
 * │      │      │      │      │      │     Del     │      │ Mute │ Vol- │ Vol+ │      │
 * └──────┴──────┴──────┴──────┴──────┴─────────────┴──────┴──────┴──────┴──────┴──────┘
 */
[_RAISE] = LAYOUT_preonic_1x2uC( \
  KC_ESC,  KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC, KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, _______, \
  KC_GRV,  KC_F9,   KC_F10,  KC_F11,  KC_F12,  _______, KC_EXLM, KC_LPRN, KC_RPRN, KC_LCBR, KC_RCBR, KC_DEL,  \
  KC_LCTL, KC_F5,   KC_F6,   KC_F7,   KC_F8,   _______, KC_HASH, KC_LBRC, KC_RBRC, KC_MINS, KC_EQL,  KC_PIPE, \
  KC_LSFT, KC_F1,   KC_F2,   KC_F3,   KC_F4,   _______, KC_ASTR, KC_UNDS, KC_PLUS, XXXXXXX, KC_DLR,  _______, \
  _______, _______, _______, _______, _______,     KC_DEL,       _______, KC_MUTE, KC_VOLD, KC_VOLU, _______  \
),

/* Adjust (Lower + Raise)
 * ┌──────┬──────┬──────┬──────┬──────┬──────┬──────┬──────┬──────┬──────┬──────┬──────┐
 * │  F1  │  F2  │  F3  │  F4  │  F5  │  F6  │  F7  │  F8  │  F9  │  F10 │  F11 │  F12 │
 * ├──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┤
 * │      │ Reset│ Debug│      │      │      │      │ Linux│ OSX  │ Win  │      │  Del │
 * ├──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┤
 * │      │      │      │AudOn │AudOff│AGnorm│AGswap│Qwerty│Game  │Prog  │      │      │
 * ├──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┤
 * │      │Voice-│Voice+│MusOn │MusOff│MidiOn│MidOff│      │      │      │      │      │
 * ├──────┼──────┼──────┼──────┼──────┼──────┴──────┼──────┼──────┼──────┼──────┼──────┤
 * │      │      │      │      │      │             │      │      │      │      │      │
 * └──────┴──────┴──────┴──────┴──────┴─────────────┴──────┴──────┴──────┴──────┴──────┘
 */
[_ADJUST] = LAYOUT_preonic_1x2uC( \
  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  \
  _______, RESET,   DEBUG,   TERM_ON, TERM_OFF,_______, _______, UC_M_LN, UC_M_OS, UC_M_WC, _______, KC_DEL,  \
  _______, _______, MU_MOD,  AU_ON,   AU_OFF,  AG_NORM, AG_SWAP, QWERTY,  GAME,    PROG,    _______, _______, \
  _______, MUV_DE,  MUV_IN,  MU_ON,   MU_OFF,  MI_ON,   MI_OFF,  _______, _______, _______, _______, _______, \
  _______, _______, _______, _______, _______,     _______,      _______, _______, _______, _______, _______  \
),

/* Accent
 * ┌──────┬──────┬──────┬──────┬──────┬──────┬──────┬──────┬──────┬──────┬──────┬──────┐
 * │      │      │      │      │      │      │      │      │      │      │      │      │
 * ├──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┤
 * │      │      │   €  │   É  │      │      │      │   Ú  │   Í  │   Ó  │      │      │
 * ├──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┤
 * │      │   Á  │   ß  │      │      │      │   Ű  │   Ü  │   Ő  │   Ö  │      │      │
 * ├──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┤
 * │ Shift│   Ä  │      │      │      │      │      │      │      │      │      │Shift │
 * ├──────┼──────┼──────┼──────┼──────┼──────┴──────┼──────┼──────┼──────┼──────┼──────┤
 * │      │      │      │      │      │     F20     │      │      │      │      │      │
 * └──────┴──────┴──────┴──────┴──────┴─────────────┴──────┴──────┴──────┴──────┴──────┘
 */
[_ACCENT] = LAYOUT_preonic_1x2uC( \
  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, \
  _______, _______, X(EUR),  xc(EA),  _______, _______, _______, xc(UA),  xc(IA),  xc(OA),  _______, _______, \
  _______, xc(AA),  X(SHSS), _______, _______, _______, xc(UJ),  xc(UU),  xc(OJ),  xc(OU),  _______, _______, \
  KC_LSFT, xc(AU),  _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_RSFT, \
  _______, _______, _______, _______, _______,     KC_F20,       _______, _______, _______, _______, _______  \
),

/* Navigation
 * ┌──────┬──────┬──────┬──────┬──────┬──────┬──────┬──────┬──────┬──────┬──────┬──────┐
 * │      │      │      │      │      │      │      │      │      │      │      │      │
 * ├──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┤
 * │      │      │      │      │      │      │      │ Home │  ↑   │ End  │      │      │
 * ├──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┤
 * │ Ctrl │      │      │      │      │      │ PgUp │  ←   │  ↓   │  →   │      │      │
 * ├──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┤
 * │ Shift│      │      │      │      │      │ PgDn │      │      │      │      │      │
 * ├──────┼──────┼──────┼──────┼──────┼──────┴──────┼──────┼──────┼──────┼──────┼──────┤
 * │      │      │      │      │      │             │      │      │      │      │      │
 * └──────┴──────┴──────┴──────┴──────┴─────────────┴──────┴──────┴──────┴──────┴──────┘
 */
[_NAV] = LAYOUT_preonic_1x2uC( \
  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, \
  _______, _______, _______, _______, _______, _______, _______, KC_HOME, KC_UP,   KC_END,  _______, _______, \
  KC_LCTL, _______, _______, _______, _______, _______, KC_PGUP, KC_LEFT, KC_DOWN, KC_RGHT, xSCLN,   _______, \
  KC_LSFT, _______, _______, _______, _______, _______, KC_PGDN, _______, _______, _______, _______, _______, \
  _______, _______, _______, _______, _______,     _______,      _______, _______, _______, _______, _______  \
),

/* GUI/Window management
 * ┌──────┬──────┬──────┬──────┬──────┬──────┬──────┬──────┬──────┬──────┬──────┬──────┐
 * │Alt F4│      │      │      │      │      │      │      │      │      │      │      │
 * ├──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┤
 * │      │ LClk │ Ms ↑ │ RClk │Ms WU │ Acc0 │      │      │      │      │      │      │
 * ├──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┤
 * │ Ctrl │ Ms ← │ Ms ↓ │ Ms → │Ms WD │ Acc1 │      │      │      │      │      │      │
 * ├──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┤
 * │ Shift│      │      │      │      │ Acc2 │      │      │      │      │      │      │
 * ├──────┼──────┼──────┼──────┼──────┼──────┴──────┼──────┼──────┼──────┼──────┼──────┤
 * │      │      │      │      │      │             │      │      │      │      │      │
 * └──────┴──────┴──────┴──────┴──────┴─────────────┴──────┴──────┴──────┴──────┴──────┘
 */
[_GUIMGR] = LAYOUT_preonic_1x2uC( \
  ALT_F4,  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, \
  _______, KC_BTN1, KC_MS_U, KC_BTN2, KC_WH_U, KC_ACL0, _______, _______, _______, _______, _______, _______, \
  KC_LCTL, KC_MS_L, KC_MS_D, KC_MS_R, KC_WH_D, KC_ACL1, _______, _______, _______, _______, _______, KC_RCTL, \
  KC_LSFT, _______, _______, _______, _______, KC_ACL2, _______, _______, _______, _______, _______, KC_RSFT, \
  _______, _______, _______, _______, _______,     _______,      _______, _______, _______, _______, _______  \
),


};


float tone_qwerty[][2]  = SONG(QWERTY_SOUND);
float tone_game[][2]    = {
  {NOTE_E6, 10}   ,{NOTE_E6, 10}  ,{NOTE_REST, 10} ,{NOTE_E6, 10}   ,
  {NOTE_REST, 10} ,{NOTE_C6, 10}  ,{NOTE_E6, 10}   ,{NOTE_REST, 10} ,
  {NOTE_G6, 10}   ,{NOTE_REST, 30},
  {NOTE_G5, 10}   ,{NOTE_REST, 30}
};
float tone_prog[][2]  = SONG(PREONIC_SOUND);


bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
        case QWERTY:
          if (record->event.pressed) {
            #ifdef AUDIO_ENABLE
            PLAY_SONG(tone_qwerty);
            #endif
            set_single_persistent_default_layer(_QWERTY);
          }
          return false;
          break;
        case GAME:
          if (record->event.pressed) {
            #ifdef AUDIO_ENABLE
            PLAY_SONG(tone_game);
            #endif
            set_single_persistent_default_layer(_GAME);
          }
          return false;
          break;
        case PROG:
          if (record->event.pressed) {
            #ifdef AUDIO_ENABLE
            PLAY_SONG(tone_prog);
            #endif
            set_single_persistent_default_layer(_PROG);
          }
          return false;
          break;
        case LOWER:
          if (record->event.pressed) {
            layer_on(_LOWER);
            update_tri_layer(_LOWER, _RAISE, _ADJUST);
          } else {
            layer_off(_LOWER);
            update_tri_layer(_LOWER, _RAISE, _ADJUST);
          }
          return false;
          break;
        case RAISE:
          if (record->event.pressed) {
            layer_on(_RAISE);
            update_tri_layer(_LOWER, _RAISE, _ADJUST);
          } else {
            layer_off(_RAISE);
            update_tri_layer(_LOWER, _RAISE, _ADJUST);
          }
          return false;
          break;
        case BACKLIT:
          if (record->event.pressed) {
            register_code(KC_RSFT);
            #ifdef BACKLIGHT_ENABLE
              backlight_step();
            #endif
            #ifdef __AVR__
            PORTE &= ~(1<<6);
            #endif
          } else {
            unregister_code(KC_RSFT);
            #ifdef __AVR__
            PORTE |= (1<<6);
            #endif
          }
          return false;
          break;
      }
    return true;
};

bool muse_mode = false;
uint8_t last_muse_note = 0;
uint16_t muse_counter = 0;
uint8_t muse_offset = 70;
uint16_t muse_tempo = 50;

void encoder_update_user(uint8_t index, bool clockwise) {
  if (muse_mode) {
    if (IS_LAYER_ON(_RAISE)) {
      if (clockwise) {
        muse_offset++;
      } else {
        muse_offset--;
      }
    } else {
      if (clockwise) {
        muse_tempo+=1;
      } else {
        muse_tempo-=1;
      }
    }
  } else {
    if (clockwise) {
      register_code(KC_PGDN);
      unregister_code(KC_PGDN);
    } else {
      register_code(KC_PGUP);
      unregister_code(KC_PGUP);
    }
  }
}

void dip_switch_update_user(uint8_t index, bool active) {
    switch (index) {
        case 0:
            if (active) {
                layer_on(_ADJUST);
            } else {
                layer_off(_ADJUST);
            }
            break;
        case 1:
            if (active) {
                muse_mode = true;
            } else {
                muse_mode = false;
            }
    }
}


void matrix_scan_user(void) {
#ifdef AUDIO_ENABLE
    if (muse_mode) {
        if (muse_counter == 0) {
            uint8_t muse_note = muse_offset + SCALE[muse_clock_pulse()];
            if (muse_note != last_muse_note) {
                stop_note(compute_freq_for_midi_note(last_muse_note));
                play_note(compute_freq_for_midi_note(muse_note), 0xF);
                last_muse_note = muse_note;
            }
        }
        muse_counter = (muse_counter + 1) % muse_tempo;
    } else {
        if (muse_counter) {
            stop_all_notes();
            muse_counter = 0;
        }
    }
#endif
}

bool music_mask_user(uint16_t keycode) {
  switch (keycode) {
    case RAISE:
    case LOWER:
      return false;
    default:
      return true;
  }
}

// vim: expandtab tabstop=2 shiftwidth=2
