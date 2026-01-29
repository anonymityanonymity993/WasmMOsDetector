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
int32_t var_func15_block1;
int32_t var_func15_after_xor;
int32_t var_func15_block2;
int32_t var_func15_after_load;
int32_t var_func15_store;
int32_t var_start_ret;

int32_t func_15(int32_t a0, int32_t a1, int32_t a2) {
    FUNC_CALL();

    (void)a2;

    int32_t local3 = 0;


    if (1 != 0) {
        local3 = 1;
    }
    WRITE_VAR(var_func15_block1, 1);

    a0 = local3;          
    a1 = 1667885170;      
    a1 = a0 ^ a1;         
    WRITE_VAR(var_func15_after_xor, 2);

    a0 = 1;

    
    
    
    if (a1 == 0) {
        WRITE_VAR(var_func15_block2, 3);
        external_function();
        a0 = 0;
    } else {
        WRITE_VAR(var_func15_block2, 4);
    }

    /* i32.const 0; i32.load; drop */
    (void)memory[0];
    WRITE_VAR(var_func15_after_load, 5);

    /* i32.const 0; local.get $0; i32.store → memory[0] = a0 */
    memory[0] = a0;
    WRITE_VAR(var_func15_store, 6);


    return a0;
}

int32_t start(void) {
    FUNC_CALL();

    int32_t ret = func_15(0, 0, 0);
    WRITE_VAR(var_start_ret, 7);
    return ret;
}

int main(void) {
    FUNC_CALL();


    memory[0] = 0;
    WRITE_VAR(var_mem0_init, 10);

    (void)start();

    DUMP_TRACES();
    return 0;
}