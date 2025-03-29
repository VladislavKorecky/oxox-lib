//
// Created by Vladislav Korecký on 13.12.2024.
//

#ifndef TEST_BOARD_H
#define TEST_BOARD_H

void test_board_init(void);

void test_board_clone(void);

void test_board_get(void);

void test_board_set(void);

void test_board_coordinates_in_bounds(void);

void test_board_get_legal_moves(void);

void test_board_shuffle_moves(void);

void test_board_to_string(void);

void test_board_from_string(void);

#endif //TEST_BOARD_H
