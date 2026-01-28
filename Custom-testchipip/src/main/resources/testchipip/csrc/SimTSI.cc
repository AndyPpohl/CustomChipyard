#include <vpi_user.h>
#include <svdpi.h>
#include <map>
#include <string>
#include "testchip_tsi.h"
#include "VTestDriver.h"
#include "VTestDriver___024root.h"
#include "cold_reset_globals.h"

extern "C" void simdram_reload_program();
//extern "C" void cold_reset_scrub_csrs();

std::map<int, testchip_tsi_t*> tsis;

// Remove VCS simv option from argv if it match pattern -X???=
void remove_vcs_simv_opt(int & argc, char **& argv){
    int idx = 0;
    while(idx < argc){
        std::string str = std::string(argv[idx]);
        if(str.length() > 1 && str[0] == '-' && str[1] != '-' && str.find('=') != std::string::npos){
            // Found -????=???? as VCS simv option
            for(int i = idx; i < argc - 1; i++){
                // Remove the current option
                argv[i] = argv[i + 1];
            }
            argc--;
        }else{
            idx++;
        }
    }
}

// ---------------------------------------------
// Store cleaned argc/argv once for all TSI resets
// ---------------------------------------------
static bool args_initialized = false;
static int saved_argc;
static std::vector<char*> saved_argv;

void init_tsi_args() {
    if (args_initialized) return;

    s_vpi_vlog_info info;
    if (!vpi_get_vlog_info(&info))
        abort();

    // Copy argv so we don't mutate the simulator's own array
    saved_argc = info.argc;
    saved_argv.assign(info.argv, info.argv + info.argc);

    char** argv_copy = saved_argv.data();
    remove_vcs_simv_opt(saved_argc, argv_copy);

    args_initialized = true;
}

//extern VTestDriver* topp;

extern "C" void cold_reset_scrub_csrs() {
    printf("COLD RESET CALLED");

    //if(!g_tsi_top) {
    //    printf("[TSI] g_tsi_top is null, skipping scrub\n");
    //    return;
    //}

    auto* root = g_tsi_top->rootp;

    //if(!root) {
    //    printf("[TSI] rootp is null, skipping scrub\n");
    //    return;
    //}
    root->TestDriver__DOT__testHarness__DOT__chiptop0__DOT__system__DOT__tile_prci_domain__DOT__element_reset_domain_rockettile__DOT__core__DOT__csr__DOT__reg_mie = 0;
    
    root->TestDriver__DOT__testHarness__DOT__chiptop0__DOT__system__DOT__tile_prci_domain__DOT__element_reset_domain_rockettile__DOT__core__DOT__csr__DOT__reg_mideleg = 0;

    root->TestDriver__DOT__testHarness__DOT__chiptop0__DOT__system__DOT__tile_prci_domain__DOT__element_reset_domain_rockettile__DOT__core__DOT__csr__DOT__reg_mideleg = 0;

    root->TestDriver__DOT__testHarness__DOT__chiptop0__DOT__system__DOT__tile_prci_domain__DOT__element_reset_domain_rockettile__DOT__core__DOT__csr__DOT__reg_medeleg = 0;

    root->TestDriver__DOT__testHarness__DOT__chiptop0__DOT__system__DOT__tile_prci_domain__DOT__element_reset_domain_rockettile__DOT__core__DOT__csr__DOT__reg_mepc = 0; 
    
    root->TestDriver__DOT__testHarness__DOT__chiptop0__DOT__system__DOT__tile_prci_domain__DOT__element_reset_domain_rockettile__DOT__core__DOT__csr__DOT__reg_mcause = 0; 
    root->TestDriver__DOT__testHarness__DOT__chiptop0__DOT__system__DOT__tile_prci_domain__DOT__element_reset_domain_rockettile__DOT__core__DOT__csr__DOT__reg_mtval = 0; 
    root->TestDriver__DOT__testHarness__DOT__chiptop0__DOT__system__DOT__tile_prci_domain__DOT__element_reset_domain_rockettile__DOT__core__DOT__csr__DOT__reg_mscratch = 0; 
    root->TestDriver__DOT__testHarness__DOT__chiptop0__DOT__system__DOT__tile_prci_domain__DOT__element_reset_domain_rockettile__DOT__core__DOT__csr__DOT__reg_sepc = 0;

    root->TestDriver__DOT__testHarness__DOT__chiptop0__DOT__system__DOT__tile_prci_domain__DOT__element_reset_domain_rockettile__DOT__core__DOT__csr__DOT__reg_scause = 0; 
    root->TestDriver__DOT__testHarness__DOT__chiptop0__DOT__system__DOT__tile_prci_domain__DOT__element_reset_domain_rockettile__DOT__core__DOT__csr__DOT__reg_stval = 0; 
    root->TestDriver__DOT__testHarness__DOT__chiptop0__DOT__system__DOT__tile_prci_domain__DOT__element_reset_domain_rockettile__DOT__core__DOT__csr__DOT__reg_sscratch = 0; 
    root->TestDriver__DOT__testHarness__DOT__chiptop0__DOT__system__DOT__tile_prci_domain__DOT__element_reset_domain_rockettile__DOT__core__DOT__csr__DOT__reg_stvec = 0; 
    root->TestDriver__DOT__testHarness__DOT__chiptop0__DOT__system__DOT__tile_prci_domain__DOT__element_reset_domain_rockettile__DOT__core__DOT__csr__DOT__reg_dscratch0 = 0; 
    root->TestDriver__DOT__testHarness__DOT__chiptop0__DOT__system__DOT__tile_prci_domain__DOT__element_reset_domain_rockettile__DOT__core__DOT__csr__DOT__reg_bp_0_address = 0;

    root->TestDriver__DOT__testHarness__DOT__chiptop0__DOT__system__DOT__tile_prci_domain__DOT__element_reset_domain_rockettile__DOT__core__DOT__csr__DOT__reg_vsepc = 0; 
    root->TestDriver__DOT__testHarness__DOT__chiptop0__DOT__system__DOT__tile_prci_domain__DOT__element_reset_domain_rockettile__DOT__core__DOT__csr__DOT__reg_satp_ppn = 0;
}

static unsigned char prev_reset = 1;
static int reset_count = 0;
static long long tick = 0;

extern "C" int tsi_tick(
    int chip_id,
    unsigned char out_valid,
    unsigned char *out_ready,
    int out_bits,
    unsigned char *in_valid,
    unsigned char in_ready,
    int *in_bits,
    unsigned char reset,
    unsigned char cold_reset
) {
    //printf("[TSI] tsi_tick entered: reset=%d cold_reset=%d\n", reset, cold_reset);

    init_tsi_args();
    
    //if(consume_cold_reset_request()) {
    if(cold_reset && !prev_reset) {
        printf("[TSI] COLD RESET REQUESTED\n");

        simdram_reload_program();
        printf("[TSI] simdram_reload ran\n");
        cold_reset_scrub_csrs();
        printf("[TSI] cold_reset_scrub ran\n");

        auto it = tsis.find(chip_id);
        if(it != tsis.end()) {
            delete it->second;
            tsis.erase(it);
        }
        tsis[chip_id] = new testchip_tsi_t(saved_argc, saved_argv.data(), true);
        printf("[TSI] TSI reinitialized\n");

        *in_valid  = 0;
        *in_bits   = 0;
        *out_ready = 0;
        
        //return 0;
    }

    //if(cold_reset != prev_reset) {
    //    printf("[TSI] reset change: tick=%lld prev=%d now=%d\n", tick, prev_reset, cold_reset);
    //}

    prev_reset = cold_reset;
    tick++;

    /////////////
    //if(cold_reset) {
    //    *in_valid  = 0;
    //    *in_bits   = 0;
    //    *out_ready = 0;
    //    return 0;
    //}
    //////////////

    auto it = tsis.find(chip_id);
    if (it == tsis.end()) {
        tsis[chip_id] = new testchip_tsi_t(saved_argc, saved_argv.data(), true);
        it = tsis.find(chip_id);
    }

    testchip_tsi_t* tsi = it->second;

    tsi->tick(out_valid, out_bits, in_ready);
    tsi->switch_to_host();

    *in_valid  = tsi->in_valid();
    *in_bits   = tsi->in_bits();
    *out_ready = tsi->out_ready();

    //prev_reset = reset;

    return tsi->done() ? ((tsi->exit_code() << 1) | 1) : 0;
}


/*
extern "C" int tsi_tick(
    int chip_id,
    unsigned char out_valid,
    unsigned char *out_ready,
    int out_bits,
    unsigned char *in_valid,
    unsigned char in_ready,
    int *in_bits,
    unsigned char reset
) {

    static unsigned char prev_reset = 1;
    static int reset_count = 0;
    static long long tick = 0;

    init_tsi_args();
    
    if (reset && !prev_reset) {
        reset_count ++;
        printf("[TSI] RESET seen in tsi_tick\n");

        if(reset_count >= 1) {
            //auto it = tsis.find(chip_id);
            //if(it == tsis.end()) {
            //    tsis[chip_id] = new testchip_tsi_t(saved_argc, saved_argv.data(), true);
            //}
            simdram_reload_program();
            cold_reset_scrub_csrs();
        }
   
        auto it = tsis.find(chip_id);
        if (it != tsis.end()) {
            delete it->second;
            tsis.erase(it);
        }

        tsis[chip_id] = new testchip_tsi_t(saved_argc, saved_argv.data(), true);

        *in_valid  = 0;
        *in_bits   = 0;
        *out_ready = 0;
        
        prev_reset = reset;
        return 0;
    }
    

    if(reset != prev_reset){
        printf("[TSI] reset change: tick=%lld prev=%d now=%d\n", tick, prev_reset, reset);
    }

    prev_reset = reset;
    tick++;

    /////////////
    if(reset) {
        *in_valid  = 0;
        *in_bits   = 0;
        *out_ready = 0;
        return 0;
    }
    //////////////

    auto it = tsis.find(chip_id);
    if (it == tsis.end()) {
        tsis[chip_id] = new testchip_tsi_t(saved_argc, saved_argv.data(), true);
        it = tsis.find(chip_id);
    }

    testchip_tsi_t* tsi = it->second;

    tsi->tick(out_valid, out_bits, in_ready);
    tsi->switch_to_host();

    *in_valid  = tsi->in_valid();
    *in_bits   = tsi->in_bits();
    *out_ready = tsi->out_ready();

    //prev_reset = reset;

    return tsi->done() ? ((tsi->exit_code() << 1) | 1) : 0;
}
*/
   
/*
extern "C" int tsi_tick(
                        int chip_id,
                        unsigned char out_valid,
                        unsigned char *out_ready,
                        int out_bits,

                        unsigned char *in_valid,
                        unsigned char in_ready,
                        int *in_bits,
                        unsigned char reset
                    )
{
    bool out_fire = *out_ready && out_valid;
    bool in_fire = *in_valid && in_ready;
    bool in_free = !(*in_valid);

    auto it = tsis.find(chip_id);

    if (it == tsis.end()) {
        s_vpi_vlog_info info;
        if (!vpi_get_vlog_info(&info))
          abort();

        // Prevent simv option enter htif
        remove_vcs_simv_opt(info.argc, info.argv);

        // TODO: We should somehow inspect whether or not our backing memory supports loadmem, instead of unconditionally setting it to true
        tsis[chip_id] = new testchip_tsi_t(info.argc, info.argv, true);
    }

    testchip_tsi_t* tsi = tsis[chip_id];
    
    //if(reset){
    //    tsi->tsi_t::reset();
    //}

    tsi->tick(out_valid, out_bits, in_ready);
    tsi->switch_to_host();

    *in_valid = tsi->in_valid();
    *in_bits = tsi->in_bits();
    *out_ready = tsi->out_ready();

    return tsi->done() ? (tsi->exit_code() << 1 | 1) : 0;
}
*/
