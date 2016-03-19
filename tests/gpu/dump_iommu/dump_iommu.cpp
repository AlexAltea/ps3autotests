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

// IOMMU dump size:
// Dumping from 0x918 onwards will cause a LV1 panic.   
#define IOMMU_DUMP_SIZE   0x918


int main() {
    FILE* handler1 = fopen("/app_home/iommu-4KB-A.bin", "wb");
    FILE* handler2 = fopen("/app_home/iommu-4KB-B.bin", "wb");

    // Dumping IOMMU
    for (size_t i = 0; i < IOMMU_DUMP_SIZE; i += 8) {
        uint64_t result = lv1_read64(0x20000510000ULL + i);
        fwrite(&result, sizeof(result), 1, handler1);
    }

    // Initialize GCM
    const uint32_t cmdSize = 0x10000;
    const uint32_t ioSize = 1*1024*1024;
    const void *ioAddress = memalign(1*1024*1024, ioSize);
    cellGcmInit(cmdSize, ioSize, ioAddress);

    // Dumping IOMMU
    for (size_t i = 0; i < IOMMU_DUMP_SIZE; i += 8) {
        uint64_t result = lv1_read64(0x20000510000ULL + i);
        fwrite(&result, sizeof(result), 1, handler2);
    }

    printf("Done!\n");
    return 0;
}
