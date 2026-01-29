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
int32_t var_start_after_load;
int32_t var_start_after_foo;
int32_t var_start_branch;
int32_t var_start_ret;

int32_t foo(int32_t p0, int32_t p1) {
    FUNC_CALL();

    (void)p0;
    (void)p1;

    // i32.store 0, 0
    memory[0] = 0;

    WRITE_VAR(var_foo_called, 1);

    return 0;
}

int32_t start(int32_t a0) {
    FUNC_CALL();

    (void)a0;

    int32_t local1 = 0;
    int32_t local2;

    /* local2 = i32.load 0 */
    local2 = memory[0];
    WRITE_VAR(var_start_after_load, 2);

    /* local1 = foo(local1, 0); */
    local1 = foo(local1, 0);
    WRITE_VAR(var_start_after_foo, 3);


    if ((uint32_t)local1 > (uint32_t)local2) {
        WRITE_VAR(var_start_branch, 4);
        external_function();
        WRITE_VAR(var_start_ret, 5);
        return 0;
    } else {
        WRITE_VAR(var_start_branch, 6);
        WRITE_VAR(var_start_ret, 7);
        return 1;
    }
}

int main(void) {
    FUNC_CALL();


    memory[0] = 0;
    WRITE_VAR(var_mem0_init, 10);

    (void)start(0);

    DUMP_TRACES();
    return 0;
}