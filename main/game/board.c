//
// Created by Vladislav Korecký on 12.12.2024.
//

#include <stdlib.h>
#include "board.h"

#include <stdio.h>

#include "std_utils.h"

Board* board_create(const uint8_t board_size) {
    if (board_size == 0) {
        throw_err("board_create", "Board size cannot be 0.");
    }

    // allocate memory for the Board struct
    Board* board = malloc(sizeof(Board));
    if (board == NULL) {
        throw_err("board_create", "Couldn't allocate memory for a board.");
        return NULL;
    }

    board->board_size = board_size;
    board->player_one_board = bitset_create(board_size * board_size);
    board->player_two_board = bitset_create(board_size * board_size);

    return board;
}

Board* board_clone(const Board* original) {
    if (original == NULL) {
        throw_err("board_clone", "Can't clone a NULL board.");
        return NULL;
    }

    // allocate memory for the Board struct
    Board* board = malloc(sizeof(Board));
    if (board == NULL) {
        throw_err("board_clone", "Couldn't allocate memory for a board.");
        return NULL;
    }

    // clone the boards
    board->board_size = original->board_size;
    board->player_one_board = bitset_clone(original->player_one_board);
    board->player_two_board = bitset_clone(original->player_two_board);

    return board;
}

void board_free(Board* board) {
    if (board == NULL) {
        return;
    }

    // free the bitboards
    bitset_free(board->player_one_board);
    bitset_free(board->player_two_board);

    // prevent user-after-free errors
    board->player_one_board = NULL;
    board->player_two_board = NULL;

    // free the struct
    free(board);
}

PlayerMark board_get(const Board* board, const uint8_t x, const uint8_t y) {
    // calculate the index of the tile's bit
    const uint16_t index = y * board->board_size + x;

    // check that x and y are in-bounds
    if (index >= board->player_one_board->size) {
        throw_err("board_get", "Board coordinates are out-of-bounds.");
    }

    // check if player one is occupying the tile
    if (bitset_get(board->player_one_board, index)) {
        return X;
    }

    // check if player two is occupying the tile
    if (bitset_get(board->player_two_board, index)) {
        return O;
    }

    return EMPTY;
}

void board_set(const Board* board, const uint8_t x, const uint8_t y, const PlayerMark mark) {
    // calculate the index of the tile's bit
    const uint16_t index = y * board->board_size + x;

    // check that x and y are in-bounds
    if (index >= board->player_one_board->size) {
        throw_err("board_set", "Board coordinates are out-of-bounds.");
    }

    if (mark == X) {
        bitset_set(board->player_one_board, index);
        return;
    }

    if (mark == O) {
        bitset_set(board->player_two_board, index);
        return;
    }

    bitset_clear(board->player_one_board, index);
    bitset_clear(board->player_two_board, index);
}

bool board_coordinates_in_bounds(const Board* board, const short x, const short y) {
    return x >= 0 && y >= 0 && x < board->board_size && y < board->board_size;
}

void board_get_legal_moves(uint8_t move_buffer[][2], const Board* board) {
    // index where to put the next legal move
    uint16_t move_index = 0;

    for (uint8_t y = 0; y < board->board_size; y++) {
        for (uint8_t x = 0; x < board->board_size; x++) {
            if (board_get(board, x, y) == EMPTY) {
                move_buffer[move_index][0] = x;
                move_buffer[move_index][1] = y;
                move_index++;
            }
        }
    }
}

void board_shuffle_moves(uint8_t move_array[][2], const uint16_t length) {
    // iterate through the moves
    for (uint16_t i = 0; i < length - 1; i++) {
        // randomly generate an index between "i + 1" and the last index
        const uint16_t j = rand() % (length - (i + 1)) + i + 1;

        // create temporary variable to hold the move
        const uint8_t t_x = move_array[i][0];
        const uint8_t t_y = move_array[i][1];

        // swap the moves
        move_array[i][0] = move_array[j][0];
        move_array[i][1] = move_array[j][1];
        move_array[j][0] = t_x;
        move_array[j][1] = t_y;
    }
}

void board_to_string(const Board* board, char* buffer) {
    // loop over the lines
    for (uint8_t y = 0; y < board->board_size; y++) {
        // loop over the columns
        for (uint8_t x = 0; x < board->board_size; x++) {
            // get the mark at the position
            const PlayerMark mark = board_get(board, x, y);
            // using ternary operations decide which character to place
            buffer[y * board->board_size + x] = mark == X ? 'X' : (mark == O) ? 'O' : '_';
        }
    }

    // null-terminate the string at last position
    buffer[board->board_size * board->board_size] = '\0';
}

void board_from_string(const Board* board, const char* repr) {
    size_t repr_index = -1;
    char c;

    while ((c = repr[++repr_index]) != '\0') {
        // set "c" to the next character by incrementing the index and continue if it isn't null
        if (c == 'X') {
            bitset_set(board->player_one_board, repr_index);
        }

        if (c == 'O') {
            bitset_set(board->player_two_board, repr_index);
        }
    }
}

void board_print(const Board* board) {
    for (uint8_t y = 0; y < board->board_size; y++) {
        for (uint8_t x = 0; x < board->board_size; x++) {
            // get the mark at the position
            const PlayerMark mark = board_get(board, x, y);
            // using ternary operations decide which character to print
            printf("%c ", mark == X ? 'X' : (mark == O) ? 'O' : '_');
        }

        // add a new line to create rows
        printf("\n");
    }
}
