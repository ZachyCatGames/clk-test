#include <cstdio>
#include <cstdlib>
#include <string.h>
#include <switch.h>
//#define CLK_TEST_DISPLAY_ALL_CLOCKS

extern "C" {
#include "rgltr.h"
}

/* CPU/GPU clock lists. */
constexpr static const u32 g_CpuClockList[] = { 
    204000000, 
    306000000, 
    408000000, 
    510000000, 
    612000000, 
    714000000, 
    816000000, 
    918000000, 
    1020000000, 
    1122000000, 
    1224000000, 
    1326000000, 
    1428000000, 
    1581000000, 
    1683000000, 
    1785000000,
};

constexpr static const u32 g_GpuClockList[] = {
    76800000, 
    153600000, 
    230400000, 
    307200000, 
    384000000, 
    460800000, 
    537600000, 
    614400000, 
    691200000, 
    768000000, 
    844800000, 
    921600000,
};

constexpr static const char* g_HardwareNames[] = {
    "Icosa",
    "Copper",
    "Hoag",
    "Iowa",
    "Calcio",
    "Aula",
};

void PrintMaxClocks(const bool isMariko);
void PrintAllClocks(const bool isMariko);

int main(int argc, char* argv[]) {
    Result result;
    u64 hardwareType;
    
    /* Initialize console. */
    consoleInit(NULL);
    
    /* Initialize pad. */
    PadState pad;
    padConfigureInput(1, HidNpadStyleSet_NpadStandard);
    padInitializeDefault(&pad);
    
    /* Initialize services. */
    result = clkrstInitialize();
    if(R_FAILED(result)) fatalThrow(result);
    
    result = rgltrInitialize();
    if(R_FAILED(result)) fatalThrow(result);
    
    result = splInitialize();
    if(R_FAILED(result)) fatalThrow(result);
    
    /* Get hardware type. */
    result = splGetConfig(SplConfigItem_HardwareType, &hardwareType);
    if(R_FAILED(result)) fatalThrow(result);
    const bool isMariko = hardwareType != 0 && hardwareType != 1;

    /* Print hardware info. */
    std::printf("Hardware Type: %ld (%s)\nSoc Type: %s\n\n", hardwareType, g_HardwareNames[hardwareType], isMariko ? "Mariko" : "Erista");

    /* Display clocks/voltages. */
#ifdef CLK_TEST_DISPLAY_ALL_CLOCKS
    PrintAllClocks(isMariko);
#else
    PrintMaxClocks(isMariko);
#endif
    
    while(appletMainLoop()) {
        padUpdate(&pad);
        u64 keyDown = padGetButtonsDown(&pad);
        
        if(keyDown & HidNpadButton_Plus) break;
        
        consoleUpdate(NULL);
    }
    
    /* Finalize services. */
    clkrstExit();
    rgltrExit();
    splExit();
    
    /* Finalize console. */
    consoleExit(NULL);
    return 0;
}

void PrintMaxClocks(const bool isMariko) {
	ClkrstSession clkrstSession;
    RgltrSession rgltrSession;
    u32 curFreq;
    u32 cpuFreq;
    u32 cpuVolt;
    u32 gpuFreq;
    u32 gpuVolt;
    u32 emcFreq;
    u32 emcVolt;

    /* Get max clocks and voltages at those clocks for CPU, GPU, and RAM. */
    rgltrOpenSession(&rgltrSession, isMariko ? PcvPowerDomainId_Max77812_Cpu : PcvPowerDomainId_Max77621_Cpu);
	clkrstOpenSession(&clkrstSession, PcvModuleId_CpuBus, 3);
	clkrstGetClockRate(&clkrstSession, &curFreq);
	clkrstSetClockRate(&clkrstSession, 4000000000);
    svcSleepThread(10000000);
    rgltrGetVoltage(&rgltrSession, &cpuVolt);
	clkrstGetClockRate(&clkrstSession, &cpuFreq);
	clkrstSetClockRate(&clkrstSession, curFreq);
	clkrstCloseSession(&clkrstSession);
    rgltrCloseSession(&rgltrSession);

    rgltrOpenSession(&rgltrSession, isMariko ? PcvPowerDomainId_Max77812_Gpu : PcvPowerDomainId_Max77621_Gpu);
	clkrstOpenSession(&clkrstSession, PcvModuleId_GPU, 3);
	clkrstGetClockRate(&clkrstSession, &curFreq);
	clkrstSetClockRate(&clkrstSession, 4000000000);
    svcSleepThread(10000000);
    rgltrGetVoltage(&rgltrSession, &gpuVolt);
	clkrstGetClockRate(&clkrstSession, &gpuFreq);
	clkrstSetClockRate(&clkrstSession, curFreq);
	clkrstCloseSession(&clkrstSession);
    rgltrCloseSession(&rgltrSession);

    rgltrOpenSession(&rgltrSession, isMariko ? PcvPowerDomainId_Max77812_Dram : PcvPowerDomainId_Max77620_Sd1);
	clkrstOpenSession(&clkrstSession, PcvModuleId_EMC, 3);
	clkrstGetClockRate(&clkrstSession, &curFreq);
	clkrstSetClockRate(&clkrstSession, 4000000000);
    svcSleepThread(10000000);
    rgltrGetVoltage(&rgltrSession, &emcVolt);
	clkrstGetClockRate(&clkrstSession, &emcFreq);
	clkrstSetClockRate(&clkrstSession, curFreq);
	clkrstCloseSession(&clkrstSession);
    rgltrCloseSession(&rgltrSession);

    /* Print the maximum clocks and voltages obtained earlier */
	printf("Maximum Usable Clocks:\n");
	printf("CPU Clock: %6.1fMHz\n", (double)cpuFreq / 1000000);
    printf("GPU Clock: %6.1fMHz\n", (double)gpuFreq / 1000000);
	printf("EMC Clock: %6.1fMHz\n", (double)emcFreq / 1000000);

    printf("\nVoltage:\n");
    printf("CPU Voltage: %6.1fmV\n", (double)cpuVolt / 1000);
    printf("GPU Voltage: %6.1fmV\n", (double)gpuVolt / 1000);
    printf("EMC Voltage: %6.1fmV\n", (double)emcVolt / 1000);
}

void PrintAllClocks(const bool isMariko) {
	ClkrstSession clkrstSession;
    RgltrSession rgltrSession;
    u32 curFreq;
    u32 freq;
    u32 volt;

    /* Attempt to set CPU/GPU to all clocks in the lists above and get voltages. */
    rgltrOpenSession(&rgltrSession, isMariko ? PcvPowerDomainId_Max77812_Cpu : PcvPowerDomainId_Max77621_Cpu);
	clkrstOpenSession(&clkrstSession, PcvModuleId_CpuBus, 3);
    clkrstGetClockRate(&clkrstSession, &curFreq);
    printf("CPU: \n");
    for(unsigned int i = 0; i < sizeof(g_CpuClockList)/sizeof(u32); i++) {
        clkrstSetClockRate(&clkrstSession, g_CpuClockList[i]);
        svcSleepThread(10000000);
        clkrstGetClockRate(&clkrstSession, &freq);
        if(freq != g_CpuClockList[i]) break;
        rgltrGetVoltage(&rgltrSession, &volt);
        printf("%6.1f: %6.1fmV\n", static_cast<double>(freq) / 1000000, static_cast<double>(volt) / 1000);
    }

    clkrstSetClockRate(&clkrstSession, curFreq);
    clkrstCloseSession(&clkrstSession);
    rgltrCloseSession(&rgltrSession);

    rgltrOpenSession(&rgltrSession, isMariko ? PcvPowerDomainId_Max77812_Gpu : PcvPowerDomainId_Max77621_Gpu);
	clkrstOpenSession(&clkrstSession, PcvModuleId_GPU, 3);
    clkrstGetClockRate(&clkrstSession, &curFreq);
    printf("\nGPU: \n");
    for(unsigned int i = 0; i < sizeof(g_GpuClockList)/sizeof(u32); i++) {
        clkrstSetClockRate(&clkrstSession, g_GpuClockList[i]);
        svcSleepThread(10000000);
        clkrstGetClockRate(&clkrstSession, &freq);
        rgltrGetVoltage(&rgltrSession, &volt);
        if(freq != g_GpuClockList[i]) break;
        printf("%6.1f: %6.1fmV\n", static_cast<double>(freq) / 1000000, static_cast<double>(volt) / 1000);
    }

    clkrstSetClockRate(&clkrstSession, curFreq);
    clkrstCloseSession(&clkrstSession);
    rgltrCloseSession(&rgltrSession);
}