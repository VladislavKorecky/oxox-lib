//
// Created by Vladislav Korecký on 09.12.2024.
//

#include <stdint.h>
#include <stdlib.h>
#include "utils/functions/std_utils.h"
#include "bitset.h"

BitSet* bitset_create(const size_t size) {
    if (size == 0) {
        throw_err("bitset_create", "Size cannot be 0.");
    }

    // allocate memory for the struct
    BitSet* bitset = malloc(sizeof(BitSet));

    if (bitset == NULL) {
        throw_err("bitset_create", "Failed to allocate memory for a BitSet.");
        return NULL;
    }

    bitset->size = size;

    // allocate memory for the byte array
    const size_t byte_count = (size + 7) / 8; // number of bytes needed
    bitset->bits = (uint8_t*)calloc(byte_count, sizeof(uint8_t));

    if (bitset->bits == NULL) {
        throw_err("bitset_create", "Failed to allocate memory for BitSet data.");
        free(bitset);
        bitset = NULL;
    }

    return bitset;
}

BitSet* bitset_clone(const BitSet* original) {
    if (original == NULL) {
        throw_err("bitset_clone", "Can't clone a NULL BitSet.");
        return NULL;
    }

    BitSet* bitset = bitset_create(original->size);
    const size_t byte_count = (bitset->size + 7) / 8;

    for (size_t i = 0; i < byte_count; i++) {
        bitset->bits[i] = original->bits[i];
    }

    return bitset;
}

void bitset_free(BitSet* bitset) {
    if (bitset == NULL) {
        return;
    }

    free(bitset->bits);
    bitset->bits = NULL;
    free(bitset);
}

void bitset_set(const BitSet* bitset, const size_t index) {
    if (index >= bitset->size) {
        throw_err("bitset_set", "Index out of bounds.");
    }

    bitset->bits[index / 8] |= (1 << (index % 8));
}

void bitset_clear(const BitSet* bitset, const size_t index) {
    if (index >= bitset->size) {
        throw_err("bitset_clear", "Index out of bounds.");
    }

    bitset->bits[index / 8] &= ~(1 << (index % 8));
}

void bitset_flip(const BitSet* bitset, const size_t index) {
    if (index >= bitset->size) {
        throw_err("bitset_flip", "Index out of bounds.");
    }

    bitset->bits[index / 8] ^= (1 << (index % 8));
}

bool bitset_get(const BitSet* bitset, const size_t index) {
    if (index >= bitset->size) {
        throw_err("bitset_get", "Index out of bounds.");
    }

    return (bitset->bits[index / 8] & (1 << (index % 8))) != 0;
}

void bitset_to_string(const BitSet* bitset, char* buffer) {
    size_t buffer_index = 0;

    for (size_t i = 0; i < bitset->size; i++) {
        buffer[buffer_index++] = bitset_get(bitset, i) ? '1' : '0';
    }

    buffer[buffer_index] = '\0'; // null-terminate the string
}

void bitset_from_string(const BitSet* bitset, const char* repr) {
    size_t repr_index = -1;
    char c;

    while ((c = repr[++repr_index]) != '\0') {
        // set "c" to the next character by incrementing the index and continue if it isn't null
        if (c == '1') {
            bitset_set(bitset, repr_index);
            continue;
        }

        bitset_clear(bitset, repr_index);
    }
}
