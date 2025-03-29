//
// Created by Vladislav Korecký on 09.12.2024.
//

#ifndef BITSET_H
#define BITSET_H

#include <stdint.h>
// this import IS NOT unused, it is the source of the size_t data type
// ReSharper disable once CppUnusedIncludeDirective
#include <stddef.h>

typedef struct
{
    uint8_t* bits; // pointer to array of bytes
    size_t size; // number of bits in the bitset
} BitSet;

/**
 * Allocate a new bitset with all bits set to 0.
 * @param size Number of bits in the bitset.
 * @return Pointer to the bitset.
 */
BitSet* bitset_create(size_t size);

/**
 * Create a copy of a bitset.
 * @param original Bitset to clone the data from.
 * @return A new bitset with a copy of the original's data.
 */
BitSet* bitset_clone(const BitSet* original);

/**
 * Free the memory allocated for the bitset.
 * @param bitset Pointer to the memory.
 */
void bitset_free(BitSet* bitset);

/**
 * Sets a specific bit to 1.
 * @param bitset Bitset to modify.
 * @param index Index of the target bit.
 */
void bitset_set(const BitSet* bitset, size_t index);

/**
 * Set a specific bit to 0.
 * @param bitset Bitset to modify.
 * @param index Index of the target bit.
 */
void bitset_clear(const BitSet* bitset, size_t index);

/**
 * Toggles a specific bit (inverts its value).
 * @param bitset Bitset to modify.
 * @param index Index of the target bit.
 */
void bitset_flip(const BitSet* bitset, size_t index);

/**
 * Get the value of a specific bit.
 * @param bitset Bitset to modify.
 * @param index Index of the target bit.
 * @return True if the bit is 1, false if it is 0.
 */
bool bitset_get(const BitSet* bitset, size_t index);

/**
 * Return a string representation of the bitset in binary form.
 * @param bitset The bitset to represent as a string.
 * @param buffer A pre-allocated buffer to hold the string representation.
 *               The buffer must be large enough to store the binary string
 *               (bitset size + 1 for null terminator).
 */
void bitset_to_string(const BitSet* bitset, char* buffer);

/**
 * Create a bitset from a string representation of it.
 * @param bitset A pre-allocated bitset to fill.
 * @param repr Continuous array of 1s and 0s of the same (or smaller) length as the bitset. E.g. 01110100 - for an 8-bit bitset
 */
void bitset_from_string(const BitSet* bitset, const char* repr);

#endif //BITSET_H
