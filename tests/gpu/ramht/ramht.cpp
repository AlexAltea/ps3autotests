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

// Writing on FIFO buffer
#define METHOD(offset, count) \
    (((count) << 18) | (offset))
#define PUT \
    *(uint32_t*)(LPAR_DMA_CONTROL_ADDR + 0x40)
#define COMMAND \
    ((uint32_t*)((uint32_t)ioAddress + PUT))

// TODO: Do the same without hardcoding the addresses
#define DEVICE_ADDR            0x40000000
#define DEVICE_SIZE            0x1000
#define LPAR_DMA_CONTROL_ADDR  0x40100000
#define LPAR_DMA_CONTROL_SIZE  0x1000
#define LPAR_DRIVER_INFO_ADDR  0x40200000
#define LPAR_DRIVER_INFO_SIZE  0x4000
#define LPAR_REPORTS_ADDR      0x40300000
#define LPAR_REPORTS_SIZE      0x10000

// NV40_CHANNEL_DMA (NV406E)
#define NV406E_SET_REFERENCE                  0x0050
#define NV406E_SET_CONTEXT_DMA_SEMAPHORE      0x0060
#define NV406E_SEMAPHORE_OFFSET               0x0064
#define NV406E_SEMAPHORE_ACQUIRE              0x0068
#define NV406E_SEMAPHORE_RELEASE              0x006C

// Write a 32-bit value to the specified DMA handle:offset
#define PFIFO_WRITE(handle, offset, value) {                   \
    COMMAND[0] = METHOD(NV406E_SET_CONTEXT_DMA_SEMAPHORE, 1);  \
    COMMAND[1] = handle;                                       \
    COMMAND[2] = METHOD(NV406E_SEMAPHORE_OFFSET, 1);           \
    COMMAND[3] = offset;                                       \
    COMMAND[4] = METHOD(NV406E_SEMAPHORE_RELEASE, 1);          \
    COMMAND[5] = value;                                        \
    PUT += 6 * 4;                                              \
    sys_timer_usleep(10000);                                   \
}

int main() {
    FILE* handler = fopen("/app_home/" OUTPUT_FILE, "wb");

    // Initialize GCM
    const uint32_t cmdSize = 0x10000;
    const uint32_t ioSize = 1*1024*1024;
    const void *ioAddress = memalign(1*1024*1024, ioSize);
    cellGcmInit(cmdSize, ioSize, ioAddress);

    PFIFO_WRITE(0x66616661, 0x0, 0);
    fprintf(handler, "LPAR_REPORTS_ADDR:0x0 => %d\n", *(uint32_t*)(LPAR_REPORTS_ADDR + 0x0));
    PFIFO_WRITE(0x66616661, 0x0, 1);
    fprintf(handler, "LPAR_REPORTS_ADDR:0x0 => %d\n", *(uint32_t*)(LPAR_REPORTS_ADDR + 0x0));

    printf("RAMHT Before:\n");
    for (size_t i = 0; i < 0x8000; i += 8) {
        uint64_t result = lv1_read64(0x28002010000ULL + i);
        uint32_t handle = (result >> 32);
        uint32_t offset = (result);
        if (handle) {
            printf("Handle: 0x%08X; Offset: 0x%08X\n", handle, offset);
            if (handle == 0xFEED0001) {
                // NOTE: This line rebinds the DMA object handle 0xFEED0001 to the object
                // pointed by the handle 0x66616661 by just copying its value,
                // which is assumed to be constant and equal to:
                // 0x00904021 == 0x00800000 (chid=1) | 0x04021 (ramin=0x40210) | 0x00100000 (unknown)
                lv1_write64(0x28002010000ULL + i, 0xFEED000100904021ULL);
            } else {
                lv1_write64(0x28002010000ULL + i, 0ULL);
            }
        }
    }

    printf("\nRAMHT After:\n");
    for (size_t i = 0; i < 0x8000; i += 8) {
        uint64_t result = lv1_read64(0x28002010000ULL + i);
        uint32_t handle = (result >> 32);
        uint32_t offset = (result);
        if (handle) {
            printf("Handle: 0x%08X; Offset: 0x%08X\n", handle, offset);
        }
    }

    PFIFO_WRITE(0xFEED0001, 0x0, 2);
    fprintf(handler, "LPAR_REPORTS_ADDR:0x0 => %d\n", *(uint32_t*)(LPAR_REPORTS_ADDR + 0x0));

    printf("Done!\n");
    return 0;
}
