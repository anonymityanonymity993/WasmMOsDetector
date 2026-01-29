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
int32_t var_foo_step;
int32_t var_bar_called;
int32_t var_start_after_foo;
int32_t var_start_after_bar;
int32_t var_start_after_and;
int32_t var_start_branch;

int64_t foo(void) {
    FUNC_CALL();


    int64_t v = *(int64_t *)(void *)&memory[0];


    WRITE_VAR(var_foo_step, 1);

    return v;
}

int32_t bar(void) {
    FUNC_CALL();

    // i32.store 0, 0
    memory[0] = 0;


    WRITE_VAR(var_bar_called, 2);

    return 0;
}

void start(int32_t a0, int32_t a1, int32_t a2, int32_t a3) {
    FUNC_CALL();

    (void)a0;
    (void)a2;
    (void)a3;

    int64_t v4, v6;
    int32_t bar_ret;

    /* v6 = foo(); */
    v6 = foo();
    WRITE_VAR(var_start_after_foo, 3);

    /* bar_ret = bar(); */
    bar_ret = bar();
    WRITE_VAR(var_start_after_bar, 4);

    /* v4 = (int64_t)bar_ret; */
    v4 = (int64_t)bar_ret;

    /* v4 = v6 & v4; */
    v4 = v6 & v4;
    WRITE_VAR(var_start_after_and, 5);


    a1 = (int32_t)v4;


    if (a1 != 0) {
        WRITE_VAR(var_start_branch, 6);
        external_function();
    } else {
        WRITE_VAR(var_start_branch, 7);
    }
}

int main(void) {
    FUNC_CALL();


    memory[0] = 0;
    WRITE_VAR(var_mem0_init, 10);

    start(0, 0, 0, 0);

    DUMP_TRACES();
    return 0;
}