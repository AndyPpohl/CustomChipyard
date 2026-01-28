#include "cold_reset_globals.h"
#include "cold_reset_globals.h"

VTestDriver* g_tsi_top = nullptr;

extern "C" void sim_init_cb(void* top) {
    g_tsi_top = static_cast<VTestDriver*>(top);
}

int cold_reset_requested = 0;

void request_cold_reset(void) {
    cold_reset_requested = 1;
}

int consume_cold_reset_request(void) {
    if (cold_reset_requested) {
        cold_reset_requested = 0;
        return 1;
    }
    return 0;
}
