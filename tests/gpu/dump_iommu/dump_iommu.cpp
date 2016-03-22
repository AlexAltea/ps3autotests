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
#define IOMMU_PART1_ADDR  0x20000510000ULL
#define IOMMU_PART1_SIZE  0x918
#define IOMMU_PART2_ADDR  0x20000511C00ULL
#define IOMMU_PART2_SIZE  0x68


int main() {
    FILE* handler1A = fopen("/app_home/iommu-part1-A.bin", "wb");
    FILE* handler2A = fopen("/app_home/iommu-part2-A.bin", "wb");
    FILE* handler1B = fopen("/app_home/iommu-part1-B.bin", "wb");
    FILE* handler2B = fopen("/app_home/iommu-part2-B.bin", "wb");

    // Dumping IOMMU
    for (size_t i = 0; i < IOMMU_PART1_SIZE; i += 8) {
        uint64_t result = lv1_read64(IOMMU_PART1_ADDR + i);
        fwrite(&result, sizeof(result), 1, handler1A);
    }
    for (size_t i = 0; i < IOMMU_PART2_SIZE; i += 8) {
        uint64_t result = lv1_read64(IOMMU_PART2_ADDR + i);
        fwrite(&result, sizeof(result), 1, handler2A);
    }

    // Initialize GCM
    const uint32_t cmdSize = 0x10000;
    const uint32_t ioSize = 1*1024*1024;
    const void *ioAddress = memalign(1*1024*1024, ioSize);
    cellGcmInit(cmdSize, ioSize, ioAddress);

    // Dumping IOMMU
    for (size_t i = 0; i < IOMMU_PART1_SIZE; i += 8) {
        uint64_t result = lv1_read64(IOMMU_PART1_ADDR + i);
        fwrite(&result, sizeof(result), 1, handler1B);
    }
    for (size_t i = 0; i < IOMMU_PART2_SIZE; i += 8) {
        uint64_t result = lv1_read64(IOMMU_PART2_ADDR + i);
        fwrite(&result, sizeof(result), 1, handler2B);
    }

    printf("Done!\n");
    return 0;
}
