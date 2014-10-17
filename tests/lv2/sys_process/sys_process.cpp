#include <stdio.h>

#include "../../../common/lv2.h"
#include "../../../common/output.h"

int main(void)
{
	// Start
	uint64_t ret;
	FILE* handler = fopen("/app_home/" OUTPUT_FILE, "wb");

	// sys_process_getpid
	ret = lv2_syscall_0(SYS_PROCESS_GETPID);
	fprintf(handler, "sys_process_getpid:\n");
	fprintf(handler, " - return: 0x%llx\n", ret);
	fprintf(handler, "\n");

	// sys_process_getppid
	ret = lv2_syscall_0(SYS_PROCESS_GETPPID);
	fprintf(handler, "sys_process_getppid:\n");
	fprintf(handler, " - return: 0x%llx\n", ret);
	fprintf(handler, "\n");

	// sys_process_get_ppu_guid
	ret = lv2_syscall_0(SYS_PROCESS_GET_PPU_GUID);
	fprintf(handler, "sys_process_get_ppu_guid:\n");
	fprintf(handler, " - return: 0x%llx\n", ret);
	fprintf(handler, "\n");

	// sys_process_get_sdk_version
	uint64_t sdk_version;
	uint64_t pid = lv2_syscall_0(SYS_PROCESS_GETPID);
	ret = lv2_syscall_2(SYS_PROCESS_GET_SDK_VERSION, pid, (uint64_t)&sdk_version);
	fprintf(handler, "sys_process_get_sdk_version:\n");
	fprintf(handler, " - return: 0x%llx\n", ret);
	fprintf(handler, " - sdk_version: 0x%llx\n", sdk_version);
	fprintf(handler, "\n");

	// sys_process_get_paramsfo
	char paramsfo[0x40] = {};
	ret = lv2_syscall_1(SYS_PROCESS_GET_PARAMSFO, (uint64_t)paramsfo);
	fprintf(handler, "sys_process_get_paramsfo:\n");
	fprintf(handler, " - return: 0x%llx\n", ret);
	fprintf(handler, " - paramsfo: ", sdk_version);
	for (size_t i = 0; i < sizeof(paramsfo); i++)
		fprintf(handler, "%02X ", (char)paramsfo[i]);
	fprintf(handler, "\n");
	fprintf(handler, "\n");

	// Exit
	fclose(handler);
	return 0;
}
