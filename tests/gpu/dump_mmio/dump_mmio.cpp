/**
 * (c) 2015 Alexandro Sanchez Bach. All rights reserved.
 * Released under GPL v2 license. Read LICENSE for more details.
 */

#include <stdio.h>
#include <stdlib.h>
#include <cell/gcm.h>

#include "../../../common/lv1.h"
#include "../../../common/lv2.h"
#include "../../../common/output.h"

// Shorthands
#define GB  *1024*1024*1024
#define MB       *1024*1024
#define KB            *1024

// MMIO dump size:
// Although 32 MB are mapped, only the first 8 MB are relevant.
// Furthermore, dumping past the 8 MB will cause a LV1 panic
// as the write-only PFIFO command region starts at 0x800000
#define MMIO_DUMP_SIZE  8 MB


int main() {
    FILE* handler1 = fopen("/app_home/mmio-32MB-A.bin", "wb");
    FILE* handler2 = fopen("/app_home/mmio-32MB-B.bin", "wb");

    // Dumping MMIO
    for (size_t i = 0; i < MMIO_DUMP_SIZE; i += 8) {
        uint64_t result = lv1_read64(0x28000000000ULL + i);
        fwrite(&result, sizeof(result), 1, handler1);
    }

    // Initialize GCM
    const uint32_t cmdSize = 0x10000;
    const uint32_t ioSize = 1*1024*1024;
    const void *ioAddress = memalign(1*1024*1024, ioSize);
    cellGcmInit(cmdSize, ioSize, ioAddress);

    // Dumping MMIO
    for (size_t i = 0; i < MMIO_DUMP_SIZE; i += 8) {
        uint64_t result = lv1_read64(0x28000000000ULL + i);
        fwrite(&result, sizeof(result), 1, handler2);
    }

    printf("Done!\n");
    return 0;
}
