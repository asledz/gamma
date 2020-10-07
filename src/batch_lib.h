#ifndef GAMMA_BATCH_LIB_H
#define GAMMA_BATCH_LIB_H

#include <stdint.h>
#include "game_lib.h"
#include "util.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include "gamma_lib.h"
#include "gamma.h"

/** @brief Obsługuje grę w trybie wsadowym.
* Obsługuje stworzoną grę w trybie wsadowym.
* Nic nie robi, jeśli wskaźnik ten ma wartość NULL.
* @param[in] gamma_ptr      -   wskaźnik na grę do obsłużenia.
*/
void batch_lib_play (gamma_t **gamma_ptr, uint64_t number_of_lines);

#endif //GAMMA_BATCH_LIB_H
