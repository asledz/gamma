#ifndef GAMMA_UTIL_H
#define GAMMA_UTIL_H

#include <stdint.h>

#include <stdbool.h>
#include <stdint.h>

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdio.h>

/** @brief Zwraca długość liczby.
 * Zwraca ilość znaków w liczbie.
 * @param[in] number       – licznba
 * @return Ilość znaków w liczbie.
 */
uint32_t util_size_of_number (uint32_t number);

/** @brief Zwiększa długość tablicy charów.
 * Sprawdza, czy może zwiększyć tablicę charów do wskazanego rozmiaru.
 * @param[in] array       	– tablica do zwiększenia.
 * @param[in] size  		– nowy rozmiar
 * @return Wskaźnik na tablicę, jeśli realokacja pamięci się powiodła,
 * @p NULL w przeciwnym przypadku.
 */
char *util_extend (char **array, size_t size);

/** @brief Sprawdza, czy znak jest białym znakiem.
 * Sprawdza, czy znak @p c jest białym znakiem.
 * @param[in] c       – znak do sprawdzenia.
 * @return Wartość @p true, jeśli tak,
 * a @p false w przeciwnym przypadku.
 */
bool util_is_whitespace (char c);

/** @brief Sprawdza, czy linia składa się ze znaków dopuszczalnych.
 * Sprawdza, czy linia @p line składa się z dopuszczalnych znaków w batch mode.
 * @param[in] line       – linia do sprawdzenia
 * @return Wartość @p true, jeśli tak,
 * a @p false w przeciwnym przypadku.
 */
bool util_valid_characters (char *line);

/** @brief Zwraca ilość spójnych sekwencji białych znaków.
 * Zwraca ilość spójnych sekwencji białych znaków w linii.
 * @param[in] line       – linia do sprawdzenia
 * @return Ilość spójnych sekwencji białych znaków.
 */
uint32_t util_number_of_breaks(char *line);


/** @brief Zwraca odpowiedni argument w linii w batch mode.
 * Zwraca odpowiedni argument liczbowy w linii w batch mode.
 * @param[in] line       – linia do sprawdzenia
 * @param[in] index 	 - argument do zwrócenia.
 * @return Argument liczbowy batch mode.
 */
uint64_t util_read_argument(char *line, uint32_t index);

#endif //GAMMA_UTIL_H
