/**
 * (c) 2015 Alexandro Sanchez Bach. All rights reserved.
 * Released under GPL v2 license. Read LICENSE for more details.
 */

#pragma once

#include "lv2.h"

#define LV2_LV1_PEEK  8ULL
#define LV2_LV1_POKE  9ULL

static inline uint64_t lv1_read64(uint64_t addr) {
    return lv2_syscall_1(LV2_LV1_PEEK, addr);
}
static inline uint64_t lv1_write64(uint64_t addr, uint64_t value) {
    return lv2_syscall_2(LV2_LV1_POKE, addr, value);
}
