//
// Created by Vladislav Korecký on 14.12.2024.
//

#include "game.h"

#include <stdio.h>
#include <stdlib.h>

#include "std_utils.h"

const uint8_t NUM_WIN_OFFSETS = 12;
// index offsets from the last move that determine if the move is a win
const char WIN_OFFSETS[NUM_WIN_OFFSETS][3][2] = {
    // wins going outward (away from the last move)
    {{0, 0}, {0, 1}, {0, 2}},
    {{0, 0}, {1, 1}, {2, 2}},
    {{0, 0}, {1, 0}, {2, 0}},
    {{0, 0}, {1, -1}, {2, -2}},
    {{0, 0}, {0, -1}, {0, -2}},
    {{0, 0}, {-1, -1}, {-2, -2}},
    {{0, 0}, {-1, 0}, {-2, 0}},
    {{0, 0}, {-1, 1}, {-2, 2}},

    // wins that are around (last move is the center)
    {{-1, 0}, {0, 0}, {1, 0}},
    {{0, -1}, {0, 0}, {0, 1}},
    {{-1, 1}, {0, 0}, {1, -1}},
    {{-1, -1}, {0, 0}, {1, 1}}
};

Game* game_create(const uint8_t board_size) {
    Game* game = malloc(sizeof(Game));

    if (game == NULL) {
        throw_err("game_create", "Couldn't allocate memory for a new game.");
        return NULL;
    }

    game->board = board_create(board_size);
    game->turns_taken = 0;

    // this is potentially dangerous because instead of having invalid values, (0,0) coordinates will work
    // in function, potentially producing unexpected behaviour without errors, as a trade-off we're decreasing memory
    // usage because the char is unsigned
    game->last_x = 0;
    game->last_y = 0;
    game->current_player = X;
    return game;
}

Game* game_clone(const Game* original) {
    if (original == NULL) {
        throw_err("game_clone", "Can't clone a NULL game.");
        return NULL;
    }

    Game* game = malloc(sizeof(Game));

    if (game == NULL) {
        throw_err("game_clone", "Couldn't allocate memory for a cloned game.");
        return NULL;
    }

    game->board = board_clone(original->board);
    game->turns_taken = original->turns_taken;
    game->last_x = original->last_x;
    game->last_y = original->last_y;
    game->current_player = original->current_player;

    return game;
}

void game_free(Game* game) {
    if (game == NULL) {
        return;
    }

    board_free(game->board);
    game->board = NULL;
    free(game);
}

void game_move(Game* game, const uint8_t x, const uint8_t y) {
    if (game == NULL) {
        throw_err("game_move", "Game cannot be NULL.");
        return;
    }

    const PlayerMark existing = board_get(game->board, x, y);

    if (existing != EMPTY) {
        throw_err("game_move", "This move is illegal. The tile is already occupied.");
    }

    board_set(game->board, x, y, game->current_player);
    game->turns_taken++;
    game->last_x = x;
    game->last_y = y;
    game->current_player = game->current_player == X ? O : X;
}

void game_un_move(Game* game, const uint8_t x, const uint8_t y) {
    const PlayerMark existing = board_get(game->board, x, y);

    if (existing == EMPTY) {
        throw_err("game_un_move", "Can't un-move an empty tile.");
    }

    board_set(game->board, x, y, EMPTY);
    game->turns_taken -= 1;

    // this is potentially dangerous because instead of having invalid values, (0,0) coordinates will work
    // in function, potentially producing unexpected behaviour without errors, as a trade-off we're decreasing memory
    // usage because the char is unsigned
    game->last_x = 0;
    game->last_y = 0;

    game->current_player = game->current_player == X ? O : X;
}

bool game_is_tie(const Game* game) {
    return game->turns_taken == game->board->board_size * game->board->board_size;
}

bool game_is_win(const Game* game) {
    if (game == NULL) {
        throw_err("game_is_win", "Game cannot be NULL.");
        return false;
    }

    // try every offset from the last move
    for (uint8_t i = 0; i < NUM_WIN_OFFSETS; i++) {
        // assume this offset is winning till proven otherwise
        bool winning = true;

        // go through the 3 position offsets
        // because the tiles in the patterns (OXO and XOX) are switching, we can compare each mark with the previous one
        PlayerMark previous_mark = EMPTY;
        for (uint8_t j = 0; j < 3; j++) {
            // extract the x and y components of the position
            const short x = game->last_x + WIN_OFFSETS[i][j][0];
            const short y = game->last_y + WIN_OFFSETS[i][j][1];

            // check that we haven't gotten outside the board
            if (!board_coordinates_in_bounds(game->board, x, y)) {
                winning = false;
                break;
            }

            const PlayerMark mark = board_get(game->board, x, y);

            if (mark == EMPTY) {
                winning = false;
                break;
            }

            // check if the pattern was broken
            if (mark == previous_mark) {
                winning = false;
                break;
            }

            previous_mark = mark;
        }

        // end the search if we already got a winning pattern
        if (winning) {
            return true;
        }
    }

    return false;
}

float game_random_play(Game* game) {
    if (game == NULL) {
        throw_err("game_random_play", "Game cannot be NULL.");
        return 0.0f;
    }

    const PlayerMark starting_player = game->current_player;
    const uint16_t num_of_tiles = game->board->board_size * game->board->board_size;

    // gather all the moves that are legal in the starting position
    const uint16_t num_legal_moves = num_of_tiles - game->turns_taken;
    uint8_t legal_moves[num_legal_moves][2];
    board_get_legal_moves(legal_moves, game->board);

    // shuffle the moves to get a random play
    board_shuffle_moves(legal_moves, num_legal_moves);

    // play out the randomized moves one by one until a win or a draw is reached
    for (uint16_t i = 0; i < num_legal_moves; i++) {
        if (game_is_win(game)) {
            // who's turn it is now lost during the last turn, so the current player is the loser
            return game->current_player == starting_player ? -1 : 1;
        }

        // OPTIMIZATION: we don't have to check for a draw, because we know there are still legal moves to play

        // make the move
        game_move(game, legal_moves[i][0], legal_moves[i][1]);
    }

    // the game ends in a draw if all the moves are depleted
    return 0;
}

float game_rollout(const Game* position, const unsigned int num_iterations) {
    float score_sum = 0;

    for (int i = 0; i < num_iterations; i++) {
        Game* game = game_clone(position);
        score_sum += game_random_play(game);
    }

    return score_sum / (float)num_iterations;
}
