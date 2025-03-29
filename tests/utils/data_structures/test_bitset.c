//
// Created by Vladislav Korecký on 11.12.2024.
//

#include <stddef.h>
#include <string.h>

#include "std_utils.h"
#include "bitset.h"

void test_bitset_init(void) {
    // 1-bit bitset
    BitSet* bitset1 = bitset_create(1);
    assert(bitset1->bits[0] == 0, "Bitset of length 1 didn't initialize correctly.");
    bitset_free(bitset1);
    bitset1 = NULL;

    // 8-bit bitset
    BitSet* bitset8 = bitset_create(8);
    assert(bitset8->bits[0] == 0, "Bitset of length 8 didn't initialize correctly.");
    bitset_free(bitset8);
    bitset8 = NULL;

    // 22-bit bitset
    BitSet* bitset22 = bitset_create(22);
    assert(bitset22->bits[0] == 0, "First byte of the 22-bit bitset didn't initialize correctly.");
    assert(bitset22->bits[1] == 0, "Second byte of the 22-bit bitset didn't initialize correctly.");
    assert(bitset22->bits[2] == 0, "Third byte of the 22-bit bitset didn't initialize correctly.");
    bitset_free(bitset22);
    bitset22 = NULL;
}

void test_bitset_clone(void) {
    // 8-bit bitset
    uint8_t buffer8[] = {0b00101011};
    const BitSet bitset8 = {buffer8, 8};
    BitSet* bitset8_clone = bitset_clone(&bitset8);

    assert(bitset8.size == bitset8_clone->size, "Cloned 8-bit bitset doesn't have the original's size.");
    assert(bitset8.bits[0] == bitset8_clone->bits[0], "Cloned 8-bit bitset doesn't have the same data.");

    bitset_free(bitset8_clone);
    bitset8_clone = NULL;

    // 22-bit bitset
    uint8_t buffer22[] = {0b01101000, 0b01101111, 0b01011010};
    const BitSet bitset22 = {buffer22, 22};
    BitSet* bitset22_clone = bitset_clone(&bitset22);

    assert(bitset22.size == bitset22_clone->size, "Cloned 22-bit bitset doesn't have the original's size.");
    assert(bitset22.bits[0] == bitset22_clone->bits[0],
           "First byte of the 22-bit cloned bitset doesn't match original.");
    assert(bitset22.bits[1] == bitset22_clone->bits[1],
           "Second byte of the 22-bit cloned bitset doesn't match original.");
    assert(bitset22.bits[2] == bitset22_clone->bits[2],
           "Third byte of the 22-bit cloned bitset doesn't match original.");

    bitset_free(bitset22_clone);
    bitset22_clone = NULL;
}

void test_bitset_set(void) {
    // 8-bit bitset
    BitSet* bitset8 = bitset_create(8);

    bitset_set(bitset8, 0);
    assert(bitset8->bits[0] == 0b00000001, "First bit of the 8-bit bitset wasn't set correctly.");

    bitset_set(bitset8, 7);
    assert(bitset8->bits[0] == 0b10000001, "Last bit of the 8-bit bitset wasn't set correctly.");

    bitset_set(bitset8, 2);
    assert(bitset8->bits[0] == 0b10000101, "Third bit of the 8-bit bitset wasn't set correctly.");

    bitset_set(bitset8, 2);
    assert(bitset8->bits[0] == 0b10000101, "A change occurred after setting an already set bit of the 8-bit bitset.");

    bitset_free(bitset8);
    bitset8 = NULL;

    // 22-bit bitset
    BitSet* bitset22 = bitset_create(22);

    bitset_set(bitset22, 11);
    assert(bitset22->bits[1] == 0b00001000, "12th bit of the 22-bit bitset wasn't set correctly.");

    bitset_set(bitset22, 16);
    assert(bitset22->bits[2] == 0b00000001, "17th bit of the 22-bit bitset wasn't set correctly.");

    bitset_free(bitset22);
    bitset22 = NULL;
}

void test_bitset_clear(void) {
    // 8-bit bitset
    uint8_t buffer8[] = {0b01000101};
    const BitSet bitset8 = {buffer8, 8};

    bitset_clear(&bitset8, 0);
    assert(bitset8.bits[0] == 0b01000100, "First bit of the 8-bit bitset wasn't cleared correctly.");

    bitset_clear(&bitset8, 6);
    assert(bitset8.bits[0] == 0b00000100, "7th bit of the 8-bit bitset wasn't cleared correctly.");

    bitset_clear(&bitset8, 6);
    assert(bitset8.bits[0] == 0b00000100,
           "A change occurred after clearing an already cleared bit of the 8-bit bitset.");

    // 22-bit bitset
    uint8_t buffer22[] = {0, 0b00000010, 0b00000101};
    const BitSet bitset22 = {buffer22, 22};

    bitset_clear(&bitset22, 9);
    assert(bitset22.bits[1] == 0, "10th bit of the 22-bit bitset wasn't cleared correctly.");

    bitset_clear(&bitset22, 18);
    assert(bitset22.bits[2] == 0b00000001, "19th bit of the 22-bit bitset wasn't cleared correctly.");
}

void test_bitset_flip(void) {
    // 8-bit bitset
    uint8_t buffer8[] = {0b00101011};
    const BitSet bitset8 = {buffer8, 8};

    bitset_flip(&bitset8, 0);
    assert(bitset8.bits[0] == 0b00101010, "First bit of the 8-bit bitset wasn't flipped correctly.");

    bitset_flip(&bitset8, 5);
    assert(bitset8.bits[0] == 0b00001010, "6th bit of the 8-bit bitset wasn't flipped correctly.");

    bitset_flip(&bitset8, 5);
    assert(bitset8.bits[0] == 0b00101010, "Couldn't flip bit back to original state in the 8-bit bitset.");

    // 22-bit bitset
    uint8_t buffer22[] = {0b00000101, 0b00100100, 0b00001100};
    const BitSet bitset22 = {buffer22, 22};

    bitset_flip(&bitset22, 13);
    assert(bitset22.bits[1] == 0b00000100, "14th bit of the 22-bit bitset wasn't flipped correctly.");

    bitset_flip(&bitset22, 19);
    assert(bitset22.bits[2] == 0b00000100, "20th bit of the 22-bit bitset wasn't flipped correctly.");
}

void test_bitset_get(void) {
    // 8-bit bitset
    uint8_t buffer8[] = {0b01101100};
    const BitSet bitset8 = {buffer8, 8};

    assert(!bitset_get(&bitset8, 1), "Incorrect value returned at index 1 of an 8-bit bitset.");
    assert(bitset_get(&bitset8, 2), "Incorrect value returned at index 2 of an 8-bit bitset.");

    // 22-bit bitset
    uint8_t buffer22[] = {0, 0b1110001, 0b0001110};
    const BitSet bitset22 = {buffer22, 22};

    assert(!bitset_get(&bitset22, 9), "Incorrect value returned at index 9 of a 22-bit bitset.");
    assert(bitset_get(&bitset22, 17), "Incorrect value returned at index 17 of a 22-bit bitset.");
}

void test_bitset_to_string(void) {
    // 1-bit bitset
    uint8_t buffer1[] = {0b00000001};
    const BitSet bitset1 = {buffer1, 1};
    char repr1[2];
    bitset_to_string(&bitset1, repr1);
    assert(strcmp(repr1, "1") == 0, "String representation of a 1-bit bitset is incorrect.");

    // 8-bit bitset
    uint8_t buffer8[] = {0b10100100};
    const BitSet bitset8 = {buffer8, 8};
    char repr8[9];
    bitset_to_string(&bitset8, repr8);
    assert(strcmp(repr8, "00100101") == 0, "String representation of an 8-bit bitset is incorrect.");

    // 22-bit bitset
    uint8_t buffer22[] = {0b10100100, 0b10111000, 0b00111111};
    // note: the last one's significant digits are only the last 6 (because the length is 22 and not 24)
    const BitSet bitset22 = {buffer22, 22};
    char repr22[23];
    bitset_to_string(&bitset22, repr22);
    assert(strcmp(repr22, "0010010100011101111111") == 0, "String representation of a 22-bit bitset is incorrect.");
}

void test_bitset_from_string(void) {
    // 1-bit bitset
    BitSet* bitset1 = bitset_create(1);
    const char* repr1 = "1";
    bitset_from_string(bitset1, repr1);
    assert(bitset1->bits[0] == 0b00000001, "The 1-bit bitset didn't load correctly from the string.");
    bitset_free(bitset1);
    bitset1 = NULL;

    // 8-bit bitset
    BitSet* bitset8 = bitset_create(8);
    const char* repr8 = "01101001";
    bitset_from_string(bitset8, repr8);
    assert(bitset8->bits[0] == 0b10010110, "The 8-bit bitset didn't load correctly from the string.");
    bitset_free(bitset8);
    bitset8 = NULL;

    // 22-bit bitset
    BitSet* bitset22 = bitset_create(22);
    const char* repr22 = "0110110011101000101110";
    bitset_from_string(bitset22, repr22);
    assert(bitset22->bits[0] == 0b00110110, "First byte of the 22-bit bitset didn't load correctly from the string.");
    assert(bitset22->bits[1] == 0b00010111, "Second byte of the 22-bit bitset didn't load correctly from the string.");
    assert(bitset22->bits[2] == 0b00011101, "Third byte of the 22-bit bitset didn't load correctly from the string.");
    bitset_free(bitset22);
    bitset22 = NULL;
}
