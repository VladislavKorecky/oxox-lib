//
// Created by Vladislav Korecký on 13.12.2024.
//

#include "test_board.h"

#include <stdlib.h>
#include <string.h>

#include "board.h"
#include "utils/functions/std_utils.h"

void test_board_init(void) {
    // 2x2 board
    Board* board2 = board_create(2);
    assert(board2->player_one_board->size == 4, "Player one bitboard size is not 4.");
    assert(board2->player_two_board->size == 4, "Player two bitboard size is not 4.");
    assert(board2->player_one_board->bits[0] == 0, "Player one bitboard of size 2x2 wasn't initialized correctly.");
    assert(board2->player_two_board->bits[0] == 0, "Player two bitboard of size 2x2 wasn't initialized correctly.");
    board_free(board2);
    board2 = NULL;

    // 4x4 board
    Board* board4 = board_create(4);
    assert(board4->player_one_board->size == 16, "Player one bitboard size is not 16.");
    assert(board4->player_two_board->size == 16, "Player two bitboard size is not 16.");
    assert(board4->player_one_board->bits[1] == 0, "Player one bitboard of size 4x4 was not initialized correctly.");
    assert(board4->player_two_board->bits[1] == 0, "Player two bitboard of size 4x4 was not initialized correctly.");
    board_free(board4);
    board4 = NULL;
}

void test_board_clone(void) {
    const char* player_one_repr = "0110000000000000010000000";
    const char* player_two_repr = "1000000000001000000000100";

    Board* board5 = board_create(5);
    bitset_from_string(board5->player_one_board, player_one_repr);
    bitset_from_string(board5->player_two_board, player_two_repr);
    Board* board5_clone = board_clone(board5);

    // get the bitset string representations of the clone to easily compare it with the original
    char player_one_buffer[26];
    char player_two_buffer[26];
    bitset_to_string(board5_clone->player_one_board, player_one_buffer);
    bitset_to_string(board5_clone->player_two_board, player_two_buffer);
    // compare the board sizes
    assert(board5->board_size == board5_clone->board_size, "Board sizes cloned incorrectly on a 5x5 board.");
    // compare the input and output string representations
    assert(strcmp(player_one_repr, player_one_buffer) == 0, "Xs recorded incorrectly when cloning a 5x5 board.");
    assert(strcmp(player_two_repr, player_two_buffer) == 0, "Os recorded incorrectly when cloning a 5x5 board.");


    board_free(board5);
    board_free(board5_clone);
    board5 = NULL;
    board5_clone = NULL;
}

void test_board_get(void) {
    Board* board3 = board_create(3);
    bitset_from_string(board3->player_one_board, "100010001");
    bitset_from_string(board3->player_two_board, "001001000");
    assert(board_get(board3, 2, 2) == X, "Incorrect player returned on tile 2,2 in a 3x3 board.");
    assert(board_get(board3, 1, 0) == EMPTY, "Incorrect player returned on tile 1,0 in a 3x3 board.");
    assert(board_get(board3, 2, 1) == O, "Incorrect player returned on tile 2,1 in a 3x3 board.");
    board_free(board3);
    board3 = NULL;
}

void test_board_set(void) {
    Board* board5 = board_create(5);
    board_set(board5, 4, 4, X);
    assert(board5->player_one_board->bits[3] == 0b00000001, "Tile set incorrectly at 4,4 in a 5x5 board.");
    board_set(board5, 4, 4, EMPTY);
    assert(board5->player_one_board->bits[3] == 0, "Tile cleared incorrectly at 4,4 in a 5x5 board.");
    board_set(board5, 0, 0, O);
    assert(board5->player_two_board->bits[0] == 0b00000001, "Tile set incorrectly at 0,0 in a 5x5 board.");
    board_free(board5);
    board5 = NULL;
}

void test_board_coordinates_in_bounds(void) {
    Board* board12 = board_create(12);
    assert(board_coordinates_in_bounds(board12, -1, 0) == false,
           "Out-of-bounds coordinates -1,0 were evaluated as valid.");
    assert(board_coordinates_in_bounds(board12, -10, -3) == false,
           "Out-of-bounds coordinates -10,-3 were evaluated as valid.");
    assert(board_coordinates_in_bounds(board12, 11, 12) == false,
           "Out-of-bounds coordinates 12,12 were evaluated as valid.");
    assert(board_coordinates_in_bounds(board12, 15, 20) == false,
           "Out-of-bounds coordinates 15,20 were evaluated as valid.");
    assert(board_coordinates_in_bounds(board12, 5, 7), "In-bounds coordinates 5,7 we evaluated as invalid.");
    assert(board_coordinates_in_bounds(board12, 0, 0), "In-bounds coordinates 0,0 we evaluated as invalid.");
    assert(board_coordinates_in_bounds(board12, 11, 11), "In-bounds coordinates 11,11 we evaluated as invalid.");
    board_free(board12);
    board12 = NULL;
}

void test_board_get_legal_moves(void) {
    const char* player_one_repr = "001100100";
    const char* player_two_repr = "000011000";

    Board* board3 = board_create(3);
    bitset_from_string(board3->player_one_board, player_one_repr);
    bitset_from_string(board3->player_two_board, player_two_repr);

    // get the legal moves
    uint8_t move_array[4][2];
    board_get_legal_moves(move_array, board3);
    const uint8_t correct_move_array[4][2] = {
        {0, 0},
        {1, 0},
        {1, 2},
        {2, 2}
    };

    // compare the given and correct 2d arrays using memcmp provided with arrays and their sizes
    const int diff = memcmp(move_array, correct_move_array, 4 * 2 * sizeof(uint8_t));
    assert(diff == 0, "Function provided incorrect legal moves for a 3x3 board position.");

    board_free(board3);
    board3 = NULL;
}

void test_board_shuffle_moves(void) {
    // set the random seed for deterministic results
    srand(1);

    const uint16_t num_moves = 5;
    const uint8_t move_array[num_moves][2] = {
        {2, 3},
        {1, 1},
        {9, 11},
        {6, 3},
        {8, 7}
    };
    uint8_t shuffled_move_array[num_moves][2];
    // clone the original array so it can be shuffled without data being overwritten
    memcpy(shuffled_move_array, move_array, sizeof(uint8_t) * 5 * 2);
    // shuffle the array
    board_shuffle_moves(shuffled_move_array, num_moves);

    // iterate through every pair of the original and shuffled moves, and check that that all of them are present
    for (uint16_t i = 0; i < num_moves; i++) {
        bool present = false;

        for (uint16_t j = 0; j < num_moves; j++) {
            // check that moves are equal
            if (move_array[i][0] == shuffled_move_array[j][0] && move_array[i][1] == shuffled_move_array[j][1]) {
                present = true;
            }
        }

        assert(present, "One of the moves wasn't included in the shuffled array.");
    }
}

void test_board_to_string(void) {
    // 2x2 board
    Board* board2 = board_create(2);
    bitset_from_string(board2->player_one_board, "0011");
    bitset_from_string(board2->player_two_board, "1100");
    char buffer2[5];
    board_to_string(board2, buffer2);
    assert(strcmp(buffer2, "OOXX") == 0, "String representation of a 2x2 board is incorrect.");
    board_free(board2);
    board2 = NULL;

    // 3x3 board
    Board* board3 = board_create(3);
    bitset_from_string(board3->player_one_board, "100100010");
    bitset_from_string(board3->player_two_board, "000010100");
    char buffer3[10];
    board_to_string(board3, buffer3);
    assert(strcmp(buffer3, "X__XO_OX_") == 0, "String representation of a 3x3 board is incorrect.");
    board_free(board3);
    board3 = NULL;
}

void test_board_from_string(void) {
    // 2x2 board
    Board* board2 = board_create(2);
    board_from_string(board2, "X__O");
    assert(board2->player_one_board->bits[0] == 0b00000001,
           "Xs recorded incorrectly while creating a 2x2 board from a string.");
    assert(board2->player_two_board->bits[0] == 0b00001000,
           "Os recorded incorrectly while creating a 2x2 board from a string.");
    board_free(board2);
    board2 = NULL;

    // 3x3 board
    Board* board3 = board_create(3);
    board_from_string(board3, "XO_X__O__");
    assert(board3->player_one_board->bits[0] == 0b00001001,
           "Xs recorded incorrectly in the first byte while creating a 3x3 board from a string.");
    assert(board3->player_one_board->bits[1] == 0,
           "Xs recorded incorrectly in the second byte while creating a 3x3 board from a string.");
    assert(board3->player_two_board->bits[0] == 0b01000010,
           "Os recorded incorrectly in the first byte while creating a 3x3 board from a string.");
    assert(board3->player_two_board->bits[1] == 0,
           "Os recorded incorrectly in the second byte while creating a 3x3 board from a string.");
    board_free(board3);
    board3 = NULL;
}
