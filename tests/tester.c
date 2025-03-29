//
// Created by Vladislav Korecký on 11.12.2024.
//

#include <stdio.h>

#include "utils/data_structures/test_bitset.h"
#include "game/test_board.h"
#include "game/test_game.h"

int main(void) {
    // test all bitset methods
    test_bitset_init();
    test_bitset_clone();
    test_bitset_set();
    test_bitset_clear();
    test_bitset_flip();
    test_bitset_get();
    test_bitset_to_string();

    // test all board methods
    test_board_init();
    test_board_clone();
    test_board_get();
    test_board_set();
    test_board_coordinates_in_bounds();
    test_board_get_legal_moves();
    test_board_shuffle_moves();
    test_board_to_string();
    test_board_from_string();

    // test all game methods
    test_game_clone();
    test_game_move();
    test_game_un_move();
    test_game_is_tie();
    test_game_is_win();
    test_game_full_suite();
    test_game_random_play();
    test_game_rollout();

    printf("All tests passed.\n");

    return 0;
}
