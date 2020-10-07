#ifndef GAMMA_GAME_LIB_H
#define GAMMA_GAME_LIB_H

#include <stdbool.h>
#include <stdint.h>
#include "gamma_lib.h"

/** @brief Obsługuje grę.
*/
void game_lib_start_game();

/** @brief Sprawdza, czy linia jest startową linią.
 * @param[in] line   – linia do sprawdzenia
 * @return Wartość @p true jeśli tak, @p false w przeciwnym wypadku.
 */
bool game_lib_start_line(char *line);

/** @brief Tworzy nową strukturę z linii startowej.
 * @param[in] line   – linia z parametrami
 * @return Wartość @p gamma, jeśli udało się stworzyć, @p NULL jeśli nie.
 */
gamma_t *game_lib_create_gamma_from_line(char *line);

#endif //GAMMA_GAME_LIB_H
