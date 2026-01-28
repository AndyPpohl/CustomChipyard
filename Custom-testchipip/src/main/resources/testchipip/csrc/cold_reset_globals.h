#pragma once
#include "VTestDriver.h"

extern VTestDriver* g_tsi_top;

#ifdef __cplusplus
extern "C" {
#endif

extern int cold_reset_requested;

void request_cold_reset(void);
int consume_cold_reset_request(void);

#ifdef __cplusplus
}
#endif

