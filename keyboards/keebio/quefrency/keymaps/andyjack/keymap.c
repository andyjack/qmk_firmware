/*
Copyright 2020 Andy Jack

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
#include QMK_KEYBOARD_H

extern keymap_config_t keymap_config;

enum layers { _QWERTY, _L1, _L2 };

enum custom_keycodes { AJ_FN1 = SAFE_RANGE, AJ_FN2 };

#define CTL_ESC LCTL_T(KC_ESC)

/* these key combos are mapped to shell commands in my .keylaunchrc */
#define MUT_IN LALT(KC_F10)
#define MUT_OUT LALT(KC_F11)
#define MUT_MIC LALT(KC_F12)
#define SCN_LCK LCTL(KC_SLCK)

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_QWERTY] = LAYOUT_65_with_macro(
    BL_TOGG, KC_ESC,  KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,             KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC, KC_BSPC, KC_DEL,  \
    KC_F1,   KC_F2,   KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,             KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS, KC_INS, \
    KC_F3,   KC_F4,   CTL_ESC, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,             KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, KC_ENT,  KC_HOME, \
    KC_F5,   KC_F6,   KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,             KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT, KC_UP,   KC_END,  \
    KC_F7,   KC_F8,   KC_LCTL, KC_LGUI, KC_LALT, AJ_FN1,  KC_SPC,                    _______, KC_SPC,  KC_RALT, AJ_FN2,  KC_RCTL, KC_LEFT, KC_DOWN, KC_RGHT
  ),

  [_L1] = LAYOUT_65_with_macro(
    _______, _______, _______, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,            KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_DEL,  KC_DEL,  _______, \
    MUT_IN,  _______, _______, _______, _______, KC_UP,   _______, _______,          _______, _______, _______, _______, _______, BL_TOGG, BL_STEP, BL_BRTG, _______, \
    MUT_OUT, _______, _______, _______, KC_LEFT, KC_DOWN, KC_RGHT, _______,          _______, _______, _______, _______, _______, _______, _______, KC_PGUP, \
    MUT_MIC, _______, _______, _______, _______, _______, KC_BTN3, _______,          _______, _______, _______, _______, _______, _______, _______, KC_PGDN, \
    SCN_LCK, _______, _______, _______, _______, _______, KC_DEL,                    _______, KC_BSPC, _______, _______, _______, KC_MPRV, KC_MPLY, KC_MNXT
  ),

  [_L2] = LAYOUT_65_with_macro(
    _______, _______, _______, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,            KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  _______, _______, _______, \
    _______, _______, _______, _______, _______, KC_UP,   _______, _______,          _______, _______, _______, _______, _______, _______, _______, _______, _______, \
    _______, _______, _______, _______, KC_LEFT, KC_DOWN, KC_RGHT, _______,          _______, _______, _______, _______, _______, _______, _______, _______, \
    _______, _______, _______, _______, _______, _______, _______, _______,          _______, _______, _______, _______, _______, _______, _______, _______, \
    _______, _______, _______, _______, _______, _______, _______,                   _______, _______, _______, _______, _______, _______, _______, _______
  )
};
// clang-format on

void encoder_update_user(uint8_t index, bool clockwise) {
    if (index == 0) {  // left encoder
        if (clockwise) {
            backlight_increase();
        } else {
            backlight_decrease();
        }
    } else if (index == 1) {  // right encoder
        if (clockwise) {
            tap_code(KC_PGDN);
        } else {
            tap_code(KC_PGUP);
        }
    }
}

static bool aj_fn1_down = false;
static bool aj_fn2_down = false;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case AJ_FN1:
            if (record->event.pressed) {
                if (aj_fn2_down) {
                    layer_on(_L2);
                } else {
                    layer_on(_L1);
                }
                aj_fn1_down = true;
            } else {
                layer_off(_L1);
                layer_off(_L2);
                aj_fn1_down = false;
            }
            return false;
            break;
        case AJ_FN2:
            if (record->event.pressed) {
                if (aj_fn1_down) {
                    layer_on(_L2);
                } else {
                    layer_on(_L1);
                }
                aj_fn2_down = true;
            } else {
                layer_off(_L1);
                layer_off(_L2);
                aj_fn2_down = false;
            }
            return false;
            break;
    }
    return true;
}
