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

int32_t memory[4];


int32_t var_mem0_init;
int32_t var_main_block1;
int32_t var_main_after_assign;
int32_t var_main_block2_path;
int32_t var_main_store;

void wasm_main(void) {
    FUNC_CALL();

    int32_t local0 = 0;
    int32_t local1 = 0;


    local0 = 1;

    local1 = 1;
    WRITE_VAR(var_main_block1, 1);


    local0 = local1;
    local1 = 1;
    WRITE_VAR(var_main_after_assign, 2);


    if (local0 != 0) {
        WRITE_VAR(var_main_block2_path, 3);

    } else {

        uint8_t b = ((uint8_t *)memory)[0];
        local0 = (int32_t)b;
        if (local0 == 0) {
            external_function();
            local1 = 0;
        }
        WRITE_VAR(var_main_block2_path, 4);
    }


    memory[0] = local1;
    WRITE_VAR(var_main_store, 5);
}

int main(void) {
    FUNC_CALL();


    memory[0] = 0;
    WRITE_VAR(var_mem0_init, 10);

    wasm_main();

    DUMP_TRACES();
    return 0;
}