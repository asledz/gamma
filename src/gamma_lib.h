#ifndef GAMMA_GAMMA_LIB_H
#define GAMMA_GAMMA_LIB_H

#include "player_lib.h"
#include <stdint.h>


/**
 * Struktura przechowująca planszę do gry.
 * Przechowuje informacje o stanie gry, stanie każdego gracza, oraz informacje o
 * założeniach gry.
 */
typedef struct gamma {
	uint32_t **board; ///< tablica planszy, na [x][y] przechowuje 0, jeśli pole niezajęte, id gracza jeśli zajęte
	player_t *players; ///< tablica graczy

	uint32_t **visited; ///< tablica pomocnicza do dfs

	uint32_t width; ///< szerokość planszy
	uint32_t height; ///< długość planszy
	uint32_t players_number; ///<ilość graczy
	uint32_t areas_number; ///<ilość pól możliwych
} gamma_t;

/** @brief Tworzy pustą strukturę przechowującą stan gry.
 * Alokuje pamięć na nową strukturę przechowującą stan gry.
 * Inicjuje tę strukturę tak, aby reprezentowała początkowy stan gry.
 * Nie sprawdza parametrów wejściowych.
 * @param[in] width   – szerokość planszy, liczba dodatnia,
 * @param[in] height  – wysokość planszy, liczba dodatnia,
 * @param[in] players – liczba graczy, liczba dodatnia,
 * @param[in] areas   – maksymalna liczba obszarów,
 *                      jakie może zająć jeden gracz.
 * @return Wskaźnik na utworzoną strukturę lub NULL, gdy nie udało się
 * zaalokować pamięci.
 */
gamma_t *gamma_lib_create_new_gamma (uint32_t width, uint32_t height,
		uint32_t players, uint32_t areas);

void gamma_lib_delete_gamma(gamma_t *g);

/** @brief Wykonuje ruch.
 * Wykonuje podany ruch - sprawdza, czy ruch jest poprawy, czyli czy wynikowa
 * plasza spełnia założenia gry. Jeśli nie jest poprawna, ustawia na dane
 * miejsce poprzednie i zwraca false. Jeśli poprawna, aktualizuje również ilość zajętych
 * obszarów każdego gracza i zwraca true.
 * @param[in,out] g   – wskaźnik na strukturę przechowującą stan gry,
 * @param[in] player  – numer gracza, liczba dodatnia niewiększa od wartości
 *                      @p players z funkcji @ref gamma_new,
 * @param[in] x       – numer kolumny, liczba nieujemna mniejsza od wartości
 *                      @p width z funkcji @ref gamma_new,
 * @param[in] y       – numer wiersza, liczba nieujemna mniejsza od wartości
 *                      @p height z funkcji @ref gamma_new.
 * @return Wartość @p true, jeśli ruch został wykonany, a @p false,
 * gdy ruch jest nielegalny lub któryś z parametrów jest niepoprawny.
 */
bool gamma_lib_make_move(gamma_t *g, uint32_t player, uint32_t x, uint32_t y);

/** @brief Przelicza pola gracza.
 * Podaje liczbę pól zajętych przez gracza @p player. Nie sprawdza poprawności
 * parametrów.
 * @param[in] g       – wskaźnik na strukturę przechowującą stan gry,
 * @param[in] player  – numer gracza, liczba dodatnia niewiększa od wartości
 *                      @p players z funkcji @ref gamma_new.
 * @return Liczba pól zajętych przez gracza.
 */
uint64_t gamma_lib_count_fields (gamma_t *g, uint32_t player);

/** @brief Podaje liczbę pól, jakie jeszcze gracz może zająć.
 * Podaje liczbę wolnych pól, na których w danym stanie gry gracz @p player może
 * postawić swój pionek w następnym normalnym ruchu. Nie sprawdza poprawności
 * parametrów.
 * @param[in] g       – wskaźnik na strukturę przechowującą stan gry,
 * @param[in] player  – numer gracza, liczba dodatnia niewiększa od wartości
 *                      @p players z funkcji @ref gamma_new.
 * @return Liczba pól, jakie jeszcze może zająć gracz.
 */
uint64_t gamma_lib_count_free_fields(gamma_t *g, uint32_t player);

/** @brief Sprawdza, czy gracz może wykonać złoty ruch.
 * Sprawdza, czy gracz @p player jeszcze nie wykonał w tej rozgrywce złotego
 * ruchu i jest przynajmniej jedno pole zajęte przez innego gracza. Nie sprawdza
 * poprawnosci parametrów.
 * @param[in] g       – wskaźnik na strukturę przechowującą stan gry,
 * @param[in] player  – numer gracza, liczba dodatnia niewiększa od wartości
 *                      @p players z funkcji @ref gamma_new.
 * @return Wartość @p true, jeśli gracz jeszcze nie wykonał w tej rozgrywce
 * złotego ruchu i jest przynajmniej jedno pole zajęte przez innego gracza,
 * a @p false w przeciwnym przypadku.
 */
bool gamma_lib_can_golden_move_possible (gamma_t *g, uint32_t player);

/** @brief Sprawdza, czy gracz może wykonać ruch.
 * Sprawdza czy gra się kończy, czyli według treści zadania - czy każdy gracz
 * nie może wykonać regularnego ruchu i złotego ruchu.
 * @param[in] gamma_ptr       – wskaźnik na strukturę przechowującą stan gry,
 * @param[in] player              – numer gracza.
 * @return Wartość @p true, jeśli gracz może wykonać ruch, @p false w
 * przeciwnym przypadku.
 */
bool gamma_lib_is_valid_player (gamma_t **gamma_ptr, uint32_t player);

/** @brief Sprawdza, czy gra według wymagań się kończy.
 * Sprawdza czy gra się kończy, czyli według treści zadania - czy każdy gracz
 * nie może wykonać regularnego ruchu i złotego ruchu.
 * @param[in] gamma_ptr       – wskaźnik na strukturę przechowującą stan gry,
 * @return Wartość @p true, jeśli gra się kończy, @p false w przeciwnym
 * przypadku.
 */
bool gamma_lib_finish_game (gamma_t **gamma_ptr);


#endif //GAMMA_GAMMA_LIB_H
