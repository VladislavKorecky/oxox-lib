//
// Created by Vladislav Korecký on 10.12.2024.
//

#ifndef STD_UTILS_H
#define STD_UTILS_H


/**
 * Print a message with a new line at the end.
 * @param message A formatted string to display.
 * @param ... A variable number of arguments that match the format specifiers in the message.
 */
void println(const char* message, ...);

/**
* Print an error message and exit the program with a failure code.
* @param functionName Name of the function where the error was raised.
* @param message An error message to print. Can include format specifiers (similar to printf).
* @param ... A variable number of arguments that match the format specifiers in the message.
*/
void throw_err(const char* functionName, const char* message, ...);

/**
 * Check that an expression is true. Print a failure message if not.
 * @param expression Condition to evaluate.
 * @param fail_message Message to print when the expression is falsy.
 * @param ... A variable number of arguments that match the format specifiers in the fail message.
 */
void assert(bool expression, const char* fail_message, ...);

#endif //STD_UTILS_H
