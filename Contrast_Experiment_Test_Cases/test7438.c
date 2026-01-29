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
int32_t var_foo_called;
int32_t var_start_after_foo;
int32_t var_start_after_or;
int32_t var_start_block1;
int32_t var_start_block2;
int32_t var_start_ret;

int32_t foo(int32_t x) {
    FUNC_CALL();

    (void)x;                 

    int32_t v = memory[0];   /* i32.load 0 */


    WRITE_VAR(var_foo_called, 1);

    return v;
}

int32_t start(int32_t a0, int32_t a1, int32_t a4) {
    FUNC_CALL();

    (void)a4; 

    int32_t local5, local6;

    /* local6 = foo(0); */
    local6 = foo(0);
    WRITE_VAR(var_start_after_foo, 2);

    /* local5 = -1; */
    local5 = -1;

    /* a1 = local5 | local6; */
    a1 = (local5 | local6);
    WRITE_VAR(var_start_after_or, 3);

    /* a0 = 0; */
    a0 = 0;

    if (a1 == 0) {
        WRITE_VAR(var_start_block1, 4);
        external_function();
    } else {
        WRITE_VAR(var_start_block1, 5);
    }


    if (a0 != 0) {
        WRITE_VAR(var_start_block2, 6);
    } else {
        WRITE_VAR(var_start_block2, 7);
    }


    WRITE_VAR(var_start_ret, 8);
    return 0;
}

int main(void) {
    FUNC_CALL();


    memory[0] = 0;
    WRITE_VAR(var_mem0_init, 10);

    (void)start(0, 0, 0);

    DUMP_TRACES();
    return 0;
}