//
// Created by Vladislav Korecký on 14.12.2024.
//

#ifndef GAME_H
#define GAME_H

#include "board.h"

typedef struct
{
    Board* board;
    uint16_t turns_taken; // number of turns that already occurred
    uint8_t last_x; // X coordinate of the last move
    uint8_t last_y; // Y coordinate of the last move
    PlayerMark current_player;
} Game;

/**
 * Allocate memory for a new game with all tiles empty.
 * @param board_size Size of the board along one axis (e.g. 8 -> 8x8 board).
 * @return Pointer to the created board.
 */
Game* game_create(uint8_t board_size);

/**
 * Deep copy a game.
 * @param original Game to copy the data from (it won't be modified in the process).
 * @return Pointer to a new allocated game.
 */
Game* game_clone(const Game* original);

/**
 * Free the allocated memory for a game.
 * @param game Pointer to the game to free.
 */
void game_free(Game* game);

/**
 * Make a move on the board. This includes rejecting illegal moves, occupying the tile, recording the move and
 * switching the active player.
 * @param game Game where the move takes place.
 * @param x X coordinate of the tile to play at.
 * @param y Y coordinate of the tile to play at.
 */
void game_move(Game* game, uint8_t x, uint8_t y);

/**
 * Revert a move on the board. This includes clearing the tile, decrease the move count and switching the active player.
 * A limitation is that the game won't remember the previous last made move.
 * @param game Game where move was previously made.
 * @param x X coordinate of the tile to clear.
 * @param y Y coordinate of the tile to clear.
 */
void game_un_move(Game* game, uint8_t x, uint8_t y);

/**
 * Check if the current position is a tie. The result purely depends on the turnsTaken property, and won't work when set incorrectly.
 * @param game Game position to evaluate.
 * @return True if the game is a tie, false otherwise.
 */
bool game_is_tie(const Game* game);

/**
 * Check if the current position is winning for one of the players. The function only works when the last recorded move
 * is part of the XOX or OXO sequence.
 * @param game Game position to evaluate.
 * @return True if the position is winning, false otherwise.
 */
bool game_is_win(const Game* game);

/**
 * Play randomly until the game ends and return the value of the position from the perspective of the starting player.
 * @param game Game position to play from. The game instance will be modified.
 * @return 1 if the starting player won, -1 if he lost, 0 for draw.
 */
float game_random_play(Game* game);

/**
 * Estimate the value of this game position by performing "n" number of random plays and averaging the game results.
 * @param position Starting position for all the simulations.
 * @param num_iterations Number of simulations to perform from the starting position.
 * @return Average game result score from the simulations.
 */
float game_rollout(const Game* position, unsigned int num_iterations);

#endif //GAME_H
