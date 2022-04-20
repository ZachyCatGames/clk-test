#include <cstdio>
#include <cstdlib>
#include <string.h>
#include <thread>
#include <switch.h>
#include "info.h"

extern "C" {
#include "rgltr.h"
#include "pcv.h"
}

#define R_ABORT_UNLESS_SUCCESS(r)   { \
                                        Result res = r; \
                                        if(R_FAILED(res)) { \
                                            diagAbortWithResult(res); \
                                        } \
                                    }

void PrintMaxClocks(const bool isMariko);
void PrintAllClocks(const bool isMariko);

int main(int argc, char* argv[]) {
    u64 hardwareType;
    u64 hardwareState;
    u64 dramId;
    u32 hosVer;

    /* Initialize console. */
    consoleInit(NULL);

    /* Initialize pad. */
    PadState pad;
    padConfigureInput(1, HidNpadStyleSet_NpadStandard);
    padInitializeDefault(&pad);
    
    /* Initialize services. */
    if(hosversionAtLeast(8,0,0)) {
        R_ABORT_UNLESS_SUCCESS(clkrstInitialize());;
        R_ABORT_UNLESS_SUCCESS(rgltrInitialize());
    } else {
        R_ABORT_UNLESS_SUCCESS(pcvInitialize());
    }
    R_ABORT_UNLESS_SUCCESS(splInitialize());

    /* Get spl config items. */
    R_ABORT_UNLESS_SUCCESS(splGetConfig(SplConfigItem_HardwareType, &hardwareType));
    R_ABORT_UNLESS_SUCCESS(splGetConfig(SplConfigItem_IsRetail, &hardwareState));
    R_ABORT_UNLESS_SUCCESS(splGetConfig(SplConfigItem_DramId, &dramId));
    const bool isMariko = hardwareType != 0 && hardwareType != 1;

    /* Get HOS Version. */
    hosVer = hosversionGet();

    /* Print hardware info. */
    const auto& dramStrings = hardwareType == 1 ? g_DramIdNamesOld : g_DramIdNames;
    std::printf("System Version: %d.%d.%d\n", HOSVER_MAJOR(hosVer), HOSVER_MINOR(hosVer), HOSVER_MICRO(hosVer));
    std::printf("Hardware Type:  %ld (%s)\n", hardwareType, g_HardwareNames[hardwareType]);
    std::printf("Hardware State: %ld (%s)\n", hardwareState, g_HardwareStates[hardwareState]);
    std::printf("Dram Id:        %ld (%s)\n", dramId, dramStrings[dramId]);
    std::printf("Soc Type: %s\n\n", isMariko ? "Mariko" : "Erista");

    /* Print instructions .*/
    std::printf("Press B to print max clocks.\n"
                "Press A to print available clocks (upto 1785MHz/921MHz/1600MHz CPU/GPU/EMC)\n"
                "Press + to exit.\n");

    while(appletMainLoop()) {
        padUpdate(&pad);
        u64 keyDown = padGetButtonsDown(&pad);

        if(keyDown & HidNpadButton_B) {
            consoleClear();
            PrintMaxClocks(isMariko);
        }
        else if(keyDown & HidNpadButton_A) {
            consoleClear();
            PrintAllClocks(isMariko);
        }
        else if(keyDown & HidNpadButton_Plus)
            break;

        consoleUpdate(NULL);
    }

    /* Finalize services. */
    if(hosversionAtLeast(8,0,0)) {
        clkrstExit();
        rgltrExit();
    } else {
        pcvExit();
    }
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
    if(hosversionAtLeast(8,0,0)) {
        rgltrOpenSession(&rgltrSession, isMariko ? PcvPowerDomainId_Max77812_Cpu : PcvPowerDomainId_Max77621_Cpu);
        clkrstOpenSession(&clkrstSession, PcvModuleId_CpuBus, 3);
        clkrstGetClockRate(&clkrstSession, &curFreq);
        clkrstSetClockRate(&clkrstSession, 0x7FFFFFFF);
        svcSleepThread(10000000);
        rgltrGetVoltage(&rgltrSession, &cpuVolt);
        clkrstGetClockRate(&clkrstSession, &cpuFreq);
        clkrstSetClockRate(&clkrstSession, curFreq);
        clkrstCloseSession(&clkrstSession);
        rgltrCloseSession(&rgltrSession);

        rgltrOpenSession(&rgltrSession, isMariko ? PcvPowerDomainId_Max77812_Gpu : PcvPowerDomainId_Max77621_Gpu);
        clkrstOpenSession(&clkrstSession, PcvModuleId_GPU, 3);
        clkrstGetClockRate(&clkrstSession, &curFreq);
        clkrstSetClockRate(&clkrstSession, 0xFFFFFFFF);
        svcSleepThread(10000000);
        rgltrGetVoltage(&rgltrSession, &gpuVolt);
        clkrstGetClockRate(&clkrstSession, &gpuFreq);
        clkrstSetClockRate(&clkrstSession, curFreq);
        clkrstCloseSession(&clkrstSession);
        rgltrCloseSession(&rgltrSession);

        rgltrOpenSession(&rgltrSession, isMariko ? PcvPowerDomainId_Max77812_Dram : PcvPowerDomainId_Max77620_Sd1);
        clkrstOpenSession(&clkrstSession, PcvModuleId_EMC, 3);
        clkrstGetClockRate(&clkrstSession, &curFreq);
        clkrstSetClockRate(&clkrstSession, 0xFFFFFFFF);
        svcSleepThread(10000000);
        rgltrGetVoltage(&rgltrSession, &emcVolt);
        clkrstGetClockRate(&clkrstSession, &emcFreq);
        clkrstSetClockRate(&clkrstSession, curFreq);
        clkrstCloseSession(&clkrstSession);
        rgltrCloseSession(&rgltrSession);
    } else {
        pcvGetClockRate(PcvModule_CpuBus, &curFreq);
        pcvSetClockRate(PcvModule_CpuBus, 0xFFFFFFFF);
        svcSleepThread(10000000);
        pcvGetClockRate(PcvModule_CpuBus, &cpuFreq);
        pcvGetVoltageValue(isMariko ? PcvPowerDomain_Max77812_Cpu : PcvPowerDomain_Max77621_Cpu, &cpuVolt);
        pcvSetClockRate(PcvModule_CpuBus, curFreq);

        pcvGetClockRate(PcvModule_CpuBus, &curFreq);
        pcvSetClockRate(PcvModule_GPU, 0xFFFFFFFF);
        svcSleepThread(10000000);
        pcvGetClockRate(PcvModule_GPU, &gpuFreq);
        pcvGetVoltageValue(isMariko ? PcvPowerDomain_Max77812_Gpu : PcvPowerDomain_Max77621_Gpu, &gpuVolt);
        pcvSetClockRate(PcvModule_GPU, curFreq);

        pcvGetClockRate(PcvModule_CpuBus, &curFreq);
        pcvSetClockRate(PcvModule_EMC, 0xFFFFFFFF);
        svcSleepThread(10000000);
        pcvGetClockRate(PcvModule_EMC, &emcFreq);
        pcvGetVoltageValue(isMariko ? PcvPowerDomain_Max77812_Dram : PcvPowerDomain_Max77620_Sd1, &emcVolt);
        pcvSetClockRate(PcvModule_EMC, curFreq);
    }

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