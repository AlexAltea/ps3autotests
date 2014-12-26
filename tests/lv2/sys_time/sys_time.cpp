#include <stdio.h>

#include "../../../common/lv2.h"
#include "../../../common/output.h"

// SysCalls
uint64_t lv2_sys_time_get_timezone(uint64_t *timezone, uint64_t *summertime)
{
    return lv2_syscall_2(SYS_TIME_GET_TIMEZONE, (uint64_t)timezone, (uint64_t)summertime);
}

uint64_t lv2_sys_time_get_current_time(sys_time_sec_t* sec, sys_time_nsec_t* nsec)
{
    return lv2_syscall_2(SYS_TIME_GET_CURRENT_TIME, (uint64_t)sec, (uint64_t)nsec);
}

uint64_t lv2_sys_time_get_timebase_frequency()
{
    return lv2_syscall_0(SYS_TIME_GET_TIMEBASE_FREQUENCY);
}

int main(void)
{
    // Start
    uint64_t ret;
    FILE* handler = fopen("/app_home/" OUTPUT_FILE, "wb");

    // sys_time_get_timebase_frequency
    ret = lv2_sys_time_get_timebase_frequency();
    fprintf(handler, "sys_time_get_timebase_frequency:\n");
    fprintf(handler, " - return: 0x%llx (%lld)\n", ret, ret);
    fprintf(handler, "\n");
    
    // sys_time_get_current_time
    sys_time_nsec_t nsec0 = 0, nsec = 0;
    sys_time_sec_t sec0 = 0, sec = 0;
    ret = lv2_sys_time_get_current_time(&sec0, &nsec0);
    ret = lv2_sys_time_get_current_time(&sec, &nsec);
    fprintf(handler, "sys_time_get_current_time:\n");
    fprintf(handler, " - return: 0x%llx\n", ret);
    fprintf(handler, " - sec diff: 0x%llx\n", sec - sec0);
    fprintf(handler, " - nsec diff: 0x%llx\n", nsec - nsec0);
    fprintf(handler, "\n");

    // lv2_sys_time_get_timezone
    uint64_t timezone = 0;
    uint64_t summertime = 0;
    ret = lv2_sys_time_get_timezone(&timezone, &summertime);
    fprintf(handler, "lv2_sys_time_get_timezone:\n");
    fprintf(handler, " - return: 0x%llx\n", ret);
    fprintf(handler, " - timezone: 0x%llx\n", timezone);
    fprintf(handler, " - summertime: 0x%llx\n", summertime);
    fprintf(handler, "\n");

    // Exit
    fclose(handler);
    return 0;
}
