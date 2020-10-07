//
// Created by Anita on 20/08/2020.
//

#ifndef GAMMA_INTERACTIVE_LIB_H
#define GAMMA_INTERACTIVE_LIB_H


#include "game_lib.h"
#include "util.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include "gamma_lib.h"
#include <stdint.h>

#include "gamma.h"


/**
 * Enumerator przycisków.
 */
enum key_codes {
	KEY_NULL = 0, /**<  NULL  */
	KEY_CTRL_D = 0x04, /**<  ctrl + D */
	KEY_ESC = 0x1b, /**< ESC */
	KEY_SPACE = 32, /**< Spacja*/

	KEY_UP = 1000, /**< Strzałka w górę */
	KEY_DOWN,           /**< Strzałka w dół */
	KEY_RIGHT,          /**< Strzałka w prawo */
	KEY_LEFT,           /**< Strzałka w lewo */
};

/** @brief Obsługuje grę w trybie interaktywnym.
* Obsługuje stworzoną grę w trybie interaktywnym.
* Nic nie robi, jeśli wskaźnik ten ma wartość NULL.
* @param[in] gamma_ptr      -   wskaźnik na grę do obsłużenia.
*/
void interactive_lib_play (gamma_t **gamma_ptr, uint64_t number_of_lines);


#endif //GAMMA_INTERACTIVE_LIB_H
