
/* Copyright 2021 Jonavin Eng @Jonavin

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
#include "firas.h"

static bool rgb_nightmode = false;

// Turn on/off NUM LOCK if current state is different
void activate_rgb_nightmode (bool turn_on) {
    if (rgb_nightmode != turn_on) {
        rgb_nightmode = !rgb_nightmode;
    }
}

bool get_rgb_nightmode(void) {
    return rgb_nightmode;
}


static uint16_t timeout_timer = 0;
static uint16_t timeout_counter = 0;  //in minute intervals
static uint16_t timeout_threshold = TIMEOUT_THRESHOLD_DEFAULT;

uint16_t get_timeout_threshold(void) {
    return timeout_threshold;
}

void timeout_reset_timer(void) {
    timeout_timer = timer_read();
    timeout_counter = 0;
};

void timeout_update_threshold(bool increase) {
    if (increase && timeout_threshold < TIMEOUT_THRESHOLD_MAX) timeout_threshold++;
    if (!increase && timeout_threshold > 0) timeout_threshold--;
};

void timeout_tick_timer(void) {
    if (timeout_threshold > 0) {
        if (timer_elapsed(timeout_timer) >= 60000) { // 1 minute tick
            timeout_counter++;
            timeout_timer = timer_read();
        }
        #ifdef RGB_MATRIX_ENABLE
            if (timeout_threshold > 0 && timeout_counter >= timeout_threshold) {
                rgb_matrix_disable_noeeprom();
            }
        #endif
    } // timeout_threshold = 0 will disable timeout
}


// PROCESS KEY CODES
__attribute__ ((weak))  bool process_record_keymap(uint16_t keycode, keyrecord_t *record) { return true; }

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (!process_record_keymap(keycode, record)) { return false; }

    switch (keycode) {
    case KC_WINLCK:
        if (record->event.pressed) {
            keymap_config.no_gui = !keymap_config.no_gui; //toggle status
        } else  unregister_code16(keycode);
        break;
    case RGB_NITE:
        if(record->event.pressed) {
            rgb_nightmode = !rgb_nightmode;
        } else  unregister_code16(keycode);
        break;
    default:
        if (record->event.pressed) {
            timeout_reset_timer();  //reset activity timer
        }
        break;
    }

    return true;
};


// INITIAL STARTUP

__attribute__ ((weak)) void keyboard_post_init_keymap(void) {}

void keyboard_post_init_user(void) {
    keyboard_post_init_keymap();

    timeout_timer = timer_read(); // set inital time for ide timeout
}





void encoder_action_volume(bool clockwise) {
    if (clockwise)
        tap_code(KC_VOLU);
    else
        tap_code(KC_VOLD);
}

void encoder_action_mediatrack(bool clockwise) {
    if (clockwise)
        tap_code(KC_MEDIA_NEXT_TRACK);
    else
        tap_code(KC_MEDIA_PREV_TRACK);
}

void encoder_action_navword(bool clockwise) {
    if (clockwise)
        tap_code16(LCTL(KC_RGHT));
    else
        tap_code16(LCTL(KC_LEFT));
}

void encoder_action_navpage(bool clockwise) {
    if (clockwise)
        tap_code16(KC_PGUP);
    else
        tap_code16(KC_PGDN);
}
