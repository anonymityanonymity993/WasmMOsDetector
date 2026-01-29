#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>



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


static int32_t var_main_entry;
static int32_t var_start_entry;
static int32_t var_start_after_foo;
static int32_t var_start_after_branch;
static int32_t var_foo_entry;
static int32_t var_foo_before_trap;

static int32_t foo(void) {
    FUNC_CALL();
    WRITE_VAR(var_foo_entry, 1);

    int32_t local0 = 0; 


    WRITE_VAR(var_foo_before_trap, 2);

    uint32_t lhs = 0u;
    uint32_t rhs = (uint32_t)local0;
    if (rhs == 0u) {

        abort();
    }
    uint32_t q = lhs / rhs;
    return (int32_t)q;
}

static void start(void) {
    FUNC_CALL();
    WRITE_VAR(var_start_entry, 3);


    int32_t r = foo();

    WRITE_VAR(var_start_after_foo, 4);

    if (r == 0) {

    } else {
        external_function();
    }

    WRITE_VAR(var_start_after_branch, 5);
}

int main(void) {
    FUNC_CALL();
    WRITE_VAR(var_main_entry, 10);

    start();


    DUMP_TRACES();
    return 0;
}