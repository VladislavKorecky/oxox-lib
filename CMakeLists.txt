cmake_minimum_required(VERSION 3.30)
project(oxox_lib C)

set(CMAKE_C_STANDARD 23)

# list of all utility code
set(ALL_UTIL_FILES main/utils/data_structures/bitset.c include/bitset.h main/utils/functions/std_utils.c main/utils/functions/std_utils.h)

# list of all OXOX game files
set(GAME_FILES main/game/board.c include/board.h main/game/game.c include/game.h)

# executable for full unit testing
add_executable(full_tests
        ${ALL_UTIL_FILES}
        ${GAME_FILES}
        tests/game/test_board.c
        tests/game/test_board.h
        tests/game/test_game.c
        tests/game/test_game.h
        tests/utils/data_structures/test_bitset.c
        tests/utils/data_structures/test_bitset.h
        tests/tester.c
)
target_include_directories(full_tests PRIVATE main)
target_include_directories(full_tests PRIVATE include)

# build the library
add_library(oxox_lib STATIC
        ${ALL_UTIL_FILES}
        ${GAME_FILES}
)
target_include_directories(oxox_lib PRIVATE main)
target_include_directories(oxox_lib PUBLIC include)
target_compile_options(oxox_lib PRIVATE
        $<$<CONFIG:Debug>:-g -O0>
        $<$<CONFIG:Release>:-O2>
)
