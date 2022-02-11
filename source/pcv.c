#include <pcv.h>

Result pcvGetVoltageValue(PowerDomain module, u32* volt_out) {
    if(hosversionAtLeast(8,0,0))
        return MAKERESULT(Module_Libnx, LibnxError_IncompatSysVer);

    Service* service = pcvGetServiceSession();
    
    const u32 in = module;
    return serviceDispatchInOut(service, 12, in, *volt_out);
}