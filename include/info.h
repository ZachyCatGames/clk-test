#include <switch.h>

/* CPU/GPU clock lists. */
static constexpr u32 g_CpuClockList[] = {
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

static constexpr u32 g_GpuClockList[] = {
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

/* Various strings. */
static constexpr const char* g_HardwareNames[] = {
    "Icosa",
    "Copper",
    "Hoag",
    "Iowa",
    "Calcio",
    "Aula",
};

static constexpr const char* g_HardwareStates[] = {
    "Development",
    "Retail",
    "Invalid",
};

static constexpr const char* g_DramIdNames[] = {
    "EristaIcosaSamsung4gb",
    "EristaIcosaHynix4gb",
    "EristaIcosaMicron4gb",
    "MarikoIowaHynix1y4gb",
    "EristaIcosaSamsung6gb",
    "MarikoHoagHynix1y4gb",
    "MarikoAulaHynix1y4gb",
    "MarikoIowax1x2Samsung4gb",
    "MarikoIowaSamsung4gb",
    "MarikoIowaSamsung8gb",
    "MarikoIowaHynix4gb",
    "MarikoIowaMicron4gb",
    "MarikoHoagSamsung4gb",
    "MarikoHoagSamsung8gb",
    "MarikoHoagHynix4gb",
    "MarikoHoagMicron4gb",
    "MarikoIowaSamsung4gbY",
    "MarikoIowaSamsung1y4gbX",
    "MarikoIowaSamsung1y8gbX",
    "MarikoHoagSamsung1y4gbX",
    "MarikoIowaSamsung1y4gbY",
    "MarikoIowaSamsung1y8gbY",
    "MarikoAulaSamsung1y4gb",
    "MarikoHoagSamsung1y8gbX",
    "MarikoAulaSamsung1y4gbX",
    "MarikoIowaMicron1y4gb",
    "MarikoHoagMicron1y4gb",
    "MarikoAulaMicron1y4gb",
    "MarikoAulaSamsung1y8gbX",
};

static constexpr const char* g_DramIdNamesOld[] = {
    "EristaIcosaSamsung4gb",
    "EristaIcosaHynix4gb",
    "EristaIcosaMicron4gb",
    "EristaCopperSamsung4gb",
    "EristaIcosaSamsung6gb",
    "EristaCopperHynix4gb",
    "EristaCopperMicron4gb",
};