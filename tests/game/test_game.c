//
// Created by Vladislav Korecký on 14.12.2024.
//

#include "test_game.h"

#include <stdlib.h>
#include <string.h>

#include "game.h"
#include "utils/functions/std_utils.h"

void test_game_clone(void) {
    const char* repr = "__X___O_______X_O__X_____";

    Game* game = game_create(5);
    board_from_string(game->board, repr);
    game->current_player = O;
    game->last_x = 2;
    game->last_y = 0;
    game->turns_taken = 5;
    Game* clone = game_clone(game);

    char buffer[26];
    board_to_string(clone->board, buffer);
    assert(strcmp(repr, buffer) == 0, "Board representation is incorrect after cloning a game.");
    assert(game->current_player == clone->current_player, "Current player is incorrect after cloning a game.");
    assert(game->last_x == clone->last_x, "X coordinate of the last move is incorrect after cloning a game.");
    assert(game->last_y == clone->last_y, "Y coordinate of the last move is incorrect after cloning a game.");
    assert(game->turns_taken == clone->turns_taken, "Number of turns taken is incorrect after cloning a game.");

    game_free(game);
    game = NULL;
    game_free(clone);
    clone = NULL;
}

void test_game_move(void) {
    Game* game = game_create(6);

    game_move(game, 2, 0);
    game_move(game, 5, 5);
    game_move(game, 3, 2);

    char repr[37];
    board_to_string(game->board, repr);
    assert(strcmp(repr, "__X____________X___________________O") == 0, "Game moves were not recorded correctly.");
    assert(game->current_player == O, "Current player is incorrect after making moves.");
    assert(game->turns_taken == 3, "Number of turns is not 3.");

    game_free(game);
    game = NULL;
}

void test_game_un_move(void) {
    Game* game = game_create(6);
    board_from_string(game->board, "X_X__________O______________________");
    game->turns_taken = 3;
    game->current_player = O;

    game_un_move(game, 2, 0);
    game_un_move(game, 1, 2);
    game_un_move(game, 0, 0);

    assert(game->board->player_one_board->bits[0] == 0, "X un-moves were recorded incorrectly.");
    assert(game->board->player_two_board->bits[0] == 0, "O un-moves were recorded incorrectly.");
    assert(game->current_player == X, "Current player is incorrect after making un-moves.");
    assert(game->turns_taken == 0, "Number of turns is not 0.");

    game_free(game);
    game = NULL;
}

void test_game_is_tie(void) {
    // 3x3 board
    Game* game3 = game_create(3);
    game3->turns_taken = 9;
    assert(game_is_tie(game3), "3x3 board was incorrectly evaluated as not a tie.");
    game3->turns_taken = 8;
    assert(game_is_tie(game3) == false, "3x3 board was incorrectly evaluated as a tie.");
    game_free(game3);
    game3 = NULL;

    // 12x12 board
    Game* game12 = game_create(12);
    assert(game_is_tie(game12) == false, "12x12 board incorrectly evaluated as a tie.");
    game12->turns_taken = 144;
    assert(game_is_tie(game12), "12x12 board was incorrectly evaluated as not a tie.");
    game_free(game12);
    game12 = NULL;
}

void test_game_is_win(void) {
    // 3x3 board
    Game* game3 = game_create(3);
    board_from_string(game3->board, "OX_XOX__O");

    game3->last_x = 0;
    game3->last_y = 1;
    assert(game_is_win(game3), "First win pattern on a 3x3 board not detected.");
    game3->last_x = 1;
    game3->last_y = 1;
    assert(game_is_win(game3), "Second win pattern on a 3x3 board not detected.");
    game3->last_x = 2;
    game3->last_y = 2;
    assert(!game_is_win(game3), "False-positive win detected on a 3x3 board.");

    game_free(game3);
    game3 = NULL;

    // 4x4 board
    Game* game4 = game_create(4);
    board_from_string(game4->board, "_____O_O__XX_X_O");

    game4->last_x = 3;
    game4->last_y = 3;
    assert(game_is_win(game4), "First win pattern on a 4x4 board not detected.");
    game4->last_x = 2;
    game4->last_y = 2;
    assert(game_is_win(game4), "Second win pattern on a 4x4 board not detected.");
    game4->last_x = 1;
    game4->last_y = 3;
    assert(!game_is_win(game4), "False-positive win detected on a 4x4 board.");

    game_free(game4);
    game4 = NULL;
}

void test_game_full_suite(void) {
    Game* game = game_create(6);

    game_move(game, 4, 1);
    game_move(game, 1, 3);
    game_un_move(game, 1, 3);

    assert(!game_is_tie(game), "Game was incorrectly evaluated as a tie in a full test suite.");

    game_move(game, 4, 0);
    game_move(game, 4, 2);
    game_move(game, 5, 1);
    game_move(game, 3, 1);

    assert(!game_is_win(game), "Game was incorrectly evaluated as a win in a full test suite.");

    game_move(game, 3, 3);

    assert(game_is_win(game), "Win wasn't detected in a full test suite.");
    char repr[37];
    board_to_string(game->board, repr);
    assert(strcmp(repr, "____O____XXO____X____O______________") == 0,
           "Board representation doesn't match moves in a full test suite.");

    game_free(game);
    game = NULL;
}

void test_game_random_play(void) {
    // set up and arbitrary position
    Game* game = game_create(6);
    board_from_string(game->board, "_________X___________O____________X_");
    game->current_player = O;
    game->turns_taken = 3;
    game->last_x = 4;
    game->last_y = 4;

    // set the random seed for deterministic behaviour
    srand(1);

    Game* cloned_game = game_clone(game);
    const float value = game_random_play(cloned_game);

    if (value == 0) {
        assert(game_is_tie(cloned_game), "Expected random play to result in a tie.");
    }
    else {
        assert(game_is_win(cloned_game), "Expected random play to result in a win.");
    }

    // change the seed and assert that the resulting end position is different (testing for randomness)
    srand(2);
    Game* different_game = game_clone(game);
    game_random_play(different_game);
    char repr[37];
    char different_repr[37];
    board_to_string(cloned_game->board, repr);
    board_to_string(different_game->board, different_repr);
    assert(strcmp(repr, different_repr) != 0, "Random play produced the same result for different seeds.");

    // clean-up
    game_free(game);
    game_free(cloned_game);
    game_free(different_game);
    game = NULL;
    cloned_game = NULL;
    different_game = NULL;
}

void test_game_rollout(void) {
    // because the process is random, the tests are pretty much just confirming the function runs without errors
    srand(1);
    Game* game = game_create(12);
    const float result = game_rollout(game, 100);
    assert(-1 <= result && result <= 1, "Game position value from rollout is out of bounds.");
    game_free(game);
    game = NULL;
}
