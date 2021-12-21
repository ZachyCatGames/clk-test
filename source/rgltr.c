#define NX_SERVICE_ASSUME_NON_DOMAIN
//#include "service_guard.h"
//#include "services/rgltr.h"
//#include "runtime/hosversion.h"
#include <switch.h>
#include "rgltr.h"

static Service g_rgltrSrv;

//NX_GENERATE_SERVICE_GUARD(rgltr);

Result rgltrInitialize(void) {
    if(hosversionBefore(8,0,0))
        return MAKERESULT(Module_Libnx, LibnxError_IncompatSysVer);

    return smGetService(&g_rgltrSrv, "rgltr");
}

void rgltrExit(void) {
    serviceClose(&g_rgltrSrv);
}

Service* rgltrGetServiceSession(void) {
    return &g_rgltrSrv;
}

Result rgltrOpenSession(RgltrSession* session_out, PowerDomainId module_id) {
    const u32 in = module_id;
    return serviceDispatchIn(&g_rgltrSrv, 0, in,
        .out_num_objects = 1,
        .out_objects = &session_out->s,
    );
}

Result rgltrGetPowerModuleNumLimit(u32 *out) {
    return serviceDispatchOut(&g_rgltrSrv, 3, *out);
}

void rgltrCloseSession(RgltrSession* session) {
    serviceClose(&session->s);
}

Result rgltrGetVoltageEnabled(RgltrSession* session, u32 *out) {
    return serviceDispatchOut(&session->s, 2, *out);
}

Result rgltrGetVoltage(RgltrSession* session, u32 *out_volt) {
    return serviceDispatchOut(&session->s, 4, *out_volt);
}