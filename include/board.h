//
// Created by Vladislav Korecký on 12.12.2024.
//

#ifndef BOARD_H
#define BOARD_H

#include "bitset.h"

typedef struct
{
    BitSet* player_one_board;
    BitSet* player_two_board;
    uint8_t board_size;
} Board;

typedef enum
{
    EMPTY = 0,
    X = 1,
    O = 2,
} PlayerMark;

/**
 * Allocate a new board with all tiles empty.
 * @param board_size Number of tiles along one axis. (e.g. 8 -> 8x8 board)
 * @return Pointer to the board.
 */
Board* board_create(uint8_t board_size);

/**
 * Deep copy a board.
 * @param original Board to clone the data from.
 * @return A newly allocated board.
 */
Board* board_clone(const Board* original);

/**
 * Free the allocated memory for the board.
 * @param board Pointer to the board.
 */
void board_free(Board* board);

/**
 * Return which player occupies a tile or if it's empty.
 * @param board Pointer to the board.
 * @param x X coordinate of the tile.
 * @param y Y coordinate of the tile.
 * @return What mark is on the tile.
 */
PlayerMark board_get(const Board* board, uint8_t x, uint8_t y);

/**
 * Assign a tile to a player or clear it.
 * @param board Board to change the tile in.
 * @param x X coordinate of the tile.
 * @param y Y coordinate of the tile.
 * @param mark How the tile should be marked.
 */
void board_set(const Board* board, uint8_t x, uint8_t y, PlayerMark mark);

/**
 * Check if coordinates are in bounds of the board.
 * @param board Board determining the bounds.
 * @param x X coordinate.
 * @param y Y coordinate.
 * @return True if the coordinates are in bounds, false otherwise.
 */
bool board_coordinates_in_bounds(const Board* board, short x, short y);

/**
 * Register all legal moves in a board position.
 * @param move_buffer A 2D array (num_legal_moves x 2 for x and y coordinates) that will be filled with legal moves. Must have large enough capacity.
 * @param board Board to search for legal moves in.
 */
void board_get_legal_moves(uint8_t move_buffer[][2], const Board* board);

/**
 * Shuffle an array of moves by randomly swapping elements (in-place operation).
 * @param move_array 2D array (length x 2 for x and y coordinates) with the moves to shuffle. It holds the shuffled data after the function finishes (will be modified).
 * @param length Number of moves in the array.
 */
void board_shuffle_moves(uint8_t move_array[][2], uint16_t length);

/**
 * Return a string representation of the board state.
 * @param board Board to read from.
 * @param buffer A pre-allocated buffer to hold the string representation. The buffer must be large enough to store the
 *               tiles (board_size^2 + 1 for null character).
 */
void board_to_string(const Board* board, char* buffer);

/**
 * Configure a board from its string representation.
 * @param board Board to set up.
 * @param repr A string representing the board state. Format: Flattened no-space series of characters (X = player 1, O = player 2, any = empty), e.g. XO_X__O__ for a 3x3 board
 */
void board_from_string(const Board* board, const char* repr);

/**
 * Print a human-readable representation of the board state.
 * @param board Board to display.
 */
void board_print(const Board* board);

#endif //BOARD_H
