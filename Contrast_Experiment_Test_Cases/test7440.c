#include <stdint.h>
#include <stdio.h>



typedef struct {
    const char *name;
    void *addr;
    int32_t val;
} GwTrace;

typedef struct {
    const char *name;
} CallTrace;

#define MAX_GW    128
#define MAX_CALL  128

static GwTrace   gw_traces[MAX_GW];
static int       gw_count = 0;
static CallTrace call_traces[MAX_CALL];
static int       call_count = 0;



static void trace_glob_write(const char *name, void *addr, int32_t val) {
    if (gw_count < MAX_GW) {
        gw_traces[gw_count].name = name;
        gw_traces[gw_count].addr = addr;
        gw_traces[gw_count].val  = val;
        gw_count++;
    }
}

static void trace_call_impl(const char *name) {
    if (call_count < MAX_CALL) {
        call_traces[call_count].name = name;
        call_count++;
    }
}

#define FUNC_CALL() trace_call_impl(__func__)

#define WRITE_VAR(v, expr)                       \
    do {                                         \
        (v) = (expr);                            \
        trace_glob_write(#v, (void *)&(v), (v)); \
    } while (0)


static void DUMP_TRACES(void) {
    printf("=== CALL TRACE ===\n");
    for (int i = 0; i < call_count; ++i) {
        printf("[CALL] %s\n", call_traces[i].name);
    }

    printf("=== GLOBAL WRITE TRACE ===\n");
    for (int i = 0; i < gw_count; ++i) {
        printf("[GW] %s = %d\n",
               gw_traces[i].name,
               gw_traces[i].val);
    }
}



extern void external_function(void) {}

int32_t  stack_pointer = 0;
uint64_t mem64[300];


int32_t var_sp_init;
int32_t var_after_sp_load;
int32_t var_after_store;
int32_t var_after_calc7;
int32_t var_after_calc11;
int32_t var_after_cmp;
int32_t var_block_path;
int32_t var_start_ret;

int32_t start(int32_t a0, int32_t a1) {
    FUNC_CALL();

    (void)a0;
    (void)a1;

    int32_t local2, local4, local7, local8, local11, local13;

    /* local2 = stack_pointer; */
    local2 = stack_pointer;
    WRITE_VAR(var_after_sp_load, 1);

    /* i64.store offset=1752, 0 */
    {
        int32_t addr  = local2 + 1752;
        int32_t index = addr / 8;
        mem64[index] = 0;
        WRITE_VAR(var_after_store, 2);
    }

    /* local4 = 4058; local7 = local2 + local4 + 1; */
    local4 = 4058;
    local7 = local2 + local4 + 1;
    WRITE_VAR(var_after_calc7, 3);

    /* local8 = 4058; local11 = local2 + local8 + 1; */
    local8  = 4058;
    local11 = local2 + local8 + 1;
    WRITE_VAR(var_after_calc11, 4);

    /* local13 = (local7 != local11); */
    local13 = (local7 != local11);
    WRITE_VAR(var_after_cmp, 5);

    if (local13 != 0) {
        WRITE_VAR(var_block_path, 6);
        external_function();
    } else {
        WRITE_VAR(var_block_path, 7);
    }


    WRITE_VAR(var_start_ret, 8);
    return 0;
}

int main(void) {
    FUNC_CALL();


    stack_pointer = 0;
    WRITE_VAR(var_sp_init, 10);

    (void)start(0, 0);

    DUMP_TRACES();
    return 0;
}