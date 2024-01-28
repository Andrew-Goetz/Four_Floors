#include <stdint.h>
#include <stdio.h>

#include <stdlib.h>
#include <unistd.h>

#include "constants.h"
#include "defs.h"

// 4 least-significant nibbles will be reserved for future use
static const uint64_t MAGIC_NUM = 0x416E647265770000;

// 8 byte magic number 
// 4 byte character name
// bits: 2(class) + (8*6)(stats) + item_slot(3) + potion_slot(3) + spells(8) == 64 == 8 bytes character stats
// 1 byte for level progress
// 3 bytes reserved for now
//
// Total size == 24, 3 uint64_t's or 24 uint8_t's
static const size_t SAVE_SIZE_BYTES = sizeof(uint8_t) * 24;
static const char *SAVEFILE = "four_floors_save.bin";

#pragma pack(1)
struct SaveFileBits {
    uint64_t magic_num:64;
    uint32_t character_name:32;
    uint8_t class:2;
    uint8_t total_health:8;
    uint8_t total_mana:8;
    int8_t health:8;
    uint8_t mana:8;
    uint8_t attack:8;
    uint8_t defense:8;
    uint8_t item_slot:3;
    uint8_t potion_slot:3;
    uint8_t known_spells:8;
    uint8_t level_progress:8;
    uint64_t reserved:24;
};
#pragma pack()

_Static_assert(sizeof(struct SaveFileBits) == 24, "Invalid bit field packing");

/* Returns true if file exists */
static bool save_check(void) {
    return !access(SAVEFILE, F_OK);
}

static void save_die(const char *str) {
    fprintf(stderr, "Your savefile (%s) is corrupted! Please delete it and restart the game!!!\nReason: %s\n",
            SAVEFILE, str);
}

int save_load(Character *c, GameProgress *p) {
    if (!c || !p) {
        fprintf(stderr, "FATAL ERROR: NULL pointer! %s:%d\n", __func__, __LINE__);
        exit(EXIT_FAILURE);
    }

    struct SaveFileBits save_bits = {0};

    return 0;
}

/* Writes current character information and level progress to savefile */
int save_write(Character *c, GameProgress p) {
    if (p < 0) {
        fprintf(stderr, "Error: invalid GameProgress value of %d, defaulting to initial state\n", p);
        p = PROGRESS_0;
    }

    if (!c) {
        fprintf(stderr, "FATAL ERROR: character is NULL!\n");
        exit(EXIT_FAILURE);
    }

    FILE *fd = fopen(SAVEFILE, "w");
    if (!fd) {
        perror("fopen():");
        return -1;
    }

    struct SaveFileBits save_bits = {0};

    //fread();

    return 0;
}
