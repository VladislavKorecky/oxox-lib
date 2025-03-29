//
// Created by Vladislav Korecký on 10.12.2024.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "std_utils.h"

void println(const char *message, ...) {
    va_list args;
    va_start(args, message);
    vprintf(message, args);
    va_end(args);
    putchar('\n');
}

void throw_err(const char *functionName, const char *message, ...) {
    va_list args;
    va_start(args, message);
    printf("RuntimeError (%s): ", functionName);
    vprintf(message, args);
    va_end(args);
    putchar('\n');
    exit(EXIT_FAILURE);
}

void assert(const bool expression, const char *fail_message, ...) {
    if (expression) {
        return;
    }

    va_list args;
    va_start(args, fail_message);
    printf("Test failed: ");
    vprintf(fail_message, args);
    va_end(args);
    putchar('\n');
}
