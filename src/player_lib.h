#ifndef player_lib_h
#define player_lib_h

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include <stdlib.h>


/**
 * Struktura przechowująca gracza.
 * Przechowuje informacje takie jak - czy został wykonany złoty ruch, ile
 * zajętych obszarów, ile zajętych pól.
 */
typedef struct player {
	bool golden_move; ///< złoty ruch
	uint32_t areas; ///< ilość obszarów
	uint64_t fields; ///< ilość pól
	uint32_t dfs; ///< zmienna pomnocnicza do DFSa
} player_t;


/** @brief Zwraca strukturę nowego gracza.
 * @return Nowy, pusty gracz.
 */
player_t create_new_player();

#endif /* player_lib_h */
