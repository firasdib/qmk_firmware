/* Copyright 2021 Glorious, LLC <salman@pcgamingrace.com>
   Copyright 2021 Jonavin
   Copyright 2022 RustyBrakes (ISO conversion)

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
#include "rgb_matrix_map.h"
#include "jonavin.h"

#define RGB_SIDE_COLOR 0xA9, 0x57, 0x00

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

//      ESC      F1       F2       F3       F4       F5       F6       F7       F8       F9       F10      F11      F12	     Ins           Rotary(Mute)
//      ~        1        2        3        4        5        6        7        8        9        0         -       (=)	     BackSpc           Del
//      Tab      Q        W        E        R        T        Y        U        I        O        P        [        ]                          PgUp
//      Caps     A        S        D        F        G        H        J        K        L        ;        "        #        Enter             PgDn
//      Sh_L     \        Z        X        C        V        B        N        M        ,        .        ?                 Sh_R     Up       End
//      Ct_L     Win_L    Alt_L                               SPACE                               Alt_R    FN       Ct_R     Left     Down     Right

    [0] = LAYOUT(
        KC_ESC,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_PSCR,          KC_MUTE,
        KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC,          KC_DEL,
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC,                   KC_PGDN,
        KC_CAPS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, KC_NUHS, KC_ENT,           KC_HOME,
        KC_LSFT, KC_NUBS, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,          KC_RSFT, KC_UP,   KC_END,
        KC_LCTL, KC_LGUI, KC_LALT,                            KC_SPC,                             KC_RALT, MO(_FN1),KC_RCTL, KC_LEFT, KC_DOWN, KC_RGHT
    ),

    [1] = LAYOUT(
        _______, KC_MYCM, KC_WHOM, KC_CALC, KC_MSEL, KC_MPRV, KC_MNXT, KC_MPLY, KC_MSTP, KC_MUTE, KC_VOLD, KC_VOLU, _______, KC_CALC,          _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, RESET,            RGB_TOG,
        _______, _______, _______, _______, _______, _______, _______, _______, KC_SLCK, _______, _______, _______, _______,                   _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,
        _______, _______, _______, _______, _______, _______, _______, NK_TOGG, _______, _______, _______, _______,          _______, RGB_MOD, _______,
        _______, KC_WINLCK, _______,                          _______,                            _______, _______, _______, RGB_SPD, RGB_RMOD, RGB_SPI
    ),

};

bool encoder_update_user(uint8_t index, bool clockwise) {
    uint8_t mods_state = get_mods();
    if (mods_state & MOD_BIT(KC_LCTL)) {
        encoder_action_volume(clockwise);
    } else {
        encoder_action_navpage(!clockwise);
    }
    return true;
}

// Capslock, Scroll lock and Numlock  indicator on Left side lights.
void rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    if (get_rgb_nightmode()) rgb_matrix_set_color_all(RGB_OFF);

    if (IS_HOST_LED_ON(USB_LED_SCROLL_LOCK)) {
        rgb_matrix_set_color(LED_L1, RGB_GREEN);
        rgb_matrix_set_color(LED_L2, RGB_GREEN);
    }

    if (IS_HOST_LED_ON(USB_LED_CAPS_LOCK)) {
        rgb_matrix_set_color(LED_CAPS, RGB_RED);
    }

    if (keymap_config.no_gui) {
        rgb_matrix_set_color(LED_LWIN, RGB_RED);  //light up Win key when disabled
    }
}


void keyboard_post_init_keymap(void) {
    // keyboard_post_init_user() moved to userspace
//    rgb_matrix_mode(RGB_MATRIX_SOLID_COLOR);
//    rgb_matrix_set_color_all(RGB_NAUTILUS); // Default startup colour
    activate_rgb_nightmode(false);  // Set to true if you want to startup in nightmode, otherwise use Fn + Z to toggle

    for (uint8_t i = 0; i < ARRAYSIZE(LED_SIDE_LEFT); i++) {
        rgb_matrix_set_color(LED_SIDE_LEFT[i], RGB_SIDE_COLOR);
        rgb_matrix_set_color(LED_SIDE_RIGHT[i], RGB_SIDE_COLOR);
    }
}
