#pragma once

typedef enum {
    PcvPowerDomain_Max77620_Sd0  = 0,
    PcvPowerDomain_Max77620_Sd1  = 1,
    PcvPowerDomain_Max77620_Sd2  = 2,
    PcvPowerDomain_Max77620_Sd3  = 3,
    PcvPowerDomain_Max77620_Ldo0 = 4,
    PcvPowerDomain_Max77620_Ldo1 = 5,
    PcvPowerDomain_Max77620_Ldo2 = 6,
    PcvPowerDomain_Max77620_Ldo3 = 7,
    PcvPowerDomain_Max77620_Ldo4 = 8,
    PcvPowerDomain_Max77620_Ldo5 = 9,
    PcvPowerDomain_Max77620_Ldo6 = 10,
    PcvPowerDomain_Max77620_Ldo7 = 11,
    PcvPowerDomain_Max77620_Ldo8 = 12,
    PcvPowerDomain_Max77621_Cpu  = 13,
    PcvPowerDomain_Max77621_Gpu  = 14,
    PcvPowerDomain_Max77812_Cpu  = 15,
    PcvPowerDomain_Max77812_Gpu  = 16,
    PcvPowerDomain_Max77812_Dram = 17,
} PowerDomain;

typedef enum {
    PcvPowerDomainId_Max77620_Sd0  = 0x3A000080,
    PcvPowerDomainId_Max77620_Sd1  = 0x3A000081,
    PcvPowerDomainId_Max77620_Sd2  = 0x3A000082,
    PcvPowerDomainId_Max77620_Sd3  = 0x3A000083,
    PcvPowerDomainId_Max77620_Ldo0 = 0x3A0000A0,
    PcvPowerDomainId_Max77620_Ldo1 = 0x3A0000A1,
    PcvPowerDomainId_Max77620_Ldo2 = 0x3A0000A2,
    PcvPowerDomainId_Max77620_Ldo3 = 0x3A0000A3,
    PcvPowerDomainId_Max77620_Ldo4 = 0x3A0000A4,
    PcvPowerDomainId_Max77620_Ldo5 = 0x3A0000A5,
    PcvPowerDomainId_Max77620_Ldo6 = 0x3A0000A6,
    PcvPowerDomainId_Max77620_Ldo7 = 0x3A0000A7,
    PcvPowerDomainId_Max77620_Ldo8 = 0x3A0000A8,
    PcvPowerDomainId_Max77621_Cpu  = 0x3A000003,
    PcvPowerDomainId_Max77621_Gpu  = 0x3A000004,
    PcvPowerDomainId_Max77812_Cpu  = 0x3A000003,
    PcvPowerDomainId_Max77812_Gpu  = 0x3A000004,
    PcvPowerDomainId_Max77812_Dram = 0x3A000005,
} PowerDomainId;