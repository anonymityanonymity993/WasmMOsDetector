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
int32_t var7, var8, var9, var10, var11, var12, var13, var4, var_start_ret;
int32_t var_foo_ret, var_bar_arg, var_bar_ret;

int32_t foo(int32_t x) {
    FUNC_CALL();

    (void)x;

    int32_t result = 0;          
    WRITE_VAR(var_foo_ret, 1);   
    return result;
}

int32_t bar(int32_t x) {
    FUNC_CALL();

    (void)x;                      

    WRITE_VAR(var_bar_arg, 2);    

    int32_t result = 1;           
    WRITE_VAR(var_bar_ret, 3);    
    return result;
}

int32_t start(int32_t a0, int32_t a1, int32_t a2, int32_t a3) {
    FUNC_CALL();

    (void)a0;
    (void)a1;
    (void)a2;
    (void)a3;

    int32_t local4;
    int32_t local7, local8, local9, local10, local11, local12, local13;

    /* local7 = memory[0]; */
    local7 = memory[0];
    WRITE_VAR(var7, 4);

    /* local8 = foo(0); */
    local8 = foo(0);
    WRITE_VAR(var8, 5);

    /* local9 = (uint32)local7 <= (uint32)local8; */
    local9 = (uint32_t)local7 <= (uint32_t)local8;
    WRITE_VAR(var9, 6);

    /* local10 = 1; */
    local10 = 1;
    WRITE_VAR(var10, 7);

    /* local11 = local9 & local10; */
    local11 = local9 & local10;
    WRITE_VAR(var11, 8);

    /* local12 = 4; */
    local12 = 4;
    WRITE_VAR(var12, 9);

    /* local13 = local11 & local12; */
    local13 = local11 & local12;
    WRITE_VAR(var13, 10);

    /* local4 = 0; */
    local4 = 0;
    WRITE_VAR(var4, 11);


    if (local13 != 0) {
        external_function();
        local4 = 0;
        WRITE_VAR(var4, 12);
    }

    int32_t ret = bar(local4);
    WRITE_VAR(var_start_ret, 13);
    return ret;
}

int main(void) {
    FUNC_CALL();


    memory[0] = 0;
    WRITE_VAR(var_mem0_init, 0);

    start(0, 0, 0, 0);

    DUMP_TRACES();
    return 0;
}