#ifndef GAMMA_DRAWING_LIB_H
#define GAMMA_DRAWING_LIB_H
#include <stdint.h>
#include "util.h"
#include "gamma_lib.h"

/** @brief Zwraca narysowaną planszę dla liczby graczy mniejszą niż 10.
 * @param[in] gamma   – struktura do narysowania.
 * @return Wartość @p gamma jeśli się udało narysować,
 * @p false w przeciwnym wypadku.
 */
char *drawing_lib_plain_board_small(gamma_t *gamma);

/** @brief Zwraca narysowaną planszę dla liczby graczy większą niż 10.
 * @param[in] gamma   – struktura do narysowania.
 * @return Wartość @p gamma jeśli się udało narysować,
 * @p false w przeciwnym wypadku.
 */
char *drawing_lib_plain_board_big(gamma_t *gamma);

/** @brief Zwraca narysowaną planszę dla liczby graczy mniejszą niż 10.
 * Zwraca narysowaną planszę dla liczby graczy mniejsząma niż 10, z kursoerem i
 * kolorami.
 * @param[in] gamma   – struktura do narysowania.
 * @param[in] x   – xowe kursora
 * @param[in] y   – ykowe kursora
 * @param[in] player   – aktualny gracz
 * @return Wartość @p gamma jeśli się udało narysować,
 * @p false w przeciwnym wypadku.
 */
char *drawing_lib_color_board_small(gamma_t *gamma, uint32_t x, uint32_t y, uint32_t player) ;

/** @brief Zwraca narysowaną planszę dla liczby graczy większą niż 10.
 * Zwraca narysowaną planszę dla liczby graczy większą niż 10, z kursoerem i
 * kolorami.
 * @param[in] gamma   – struktura do narysowania.
 * @param[in] x   – xowe kursora
 * @param[in] y   – ykowe kursora
 * @param[in] player   – aktualny gracz
 * @return Wartość @p gamma jeśli się udało narysować,
 * @p false w przeciwnym wypadku.
 */
char *drawing_lib_color_board_big(gamma_t *gamma, uint32_t x, uint32_t y, uint32_t player);

/** @brief Wypisuje końcowe statystyki.
 * @param[in] gamma   – gra do wyświetlenia statystyk.s
 */
void drawing_lib_ending_screen (gamma_t *gamma);

#endif //GAMMA_DRAWING_LIB_H

