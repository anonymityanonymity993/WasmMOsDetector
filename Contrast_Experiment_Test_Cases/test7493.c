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
int32_t var_bar_called;
int32_t var_start_after_foo;
int32_t var_start_after_shift;
int32_t var_start_after_store;
int32_t var_start_cmp_path;

int32_t foo(void) {
    FUNC_CALL();

    int32_t v = memory[0];


    WRITE_VAR(var_foo_called, 1);

    return v;
}

void bar(void) {
    FUNC_CALL();


    WRITE_VAR(var_bar_called, 2);

    external_function();
}

void start(int32_t a0, int64_t a1, int32_t a2) {
    FUNC_CALL();

    (void)a0;
    (void)a1;
    (void)a2;

    int32_t v3, v4, v6, v7, v9;

    /* v4 = foo(); */
    v4 = foo();
    WRITE_VAR(var_start_after_foo, 3);

    v3 = 1;


    v6 = (v4 << v3);
    v6 = v6 >> 1;
    WRITE_VAR(var_start_after_shift, 4);

    /* memory[0] = 0; */
    memory[0] = 0;
    WRITE_VAR(var_start_after_store, 5);

    v7 = -259031342;
    v9 = (v6 != v7);


    if (v9 != 0) {
        WRITE_VAR(var_start_cmp_path, 6); 
        return;
    } else {
        WRITE_VAR(var_start_cmp_path, 7);
    }

    bar();
}

int main(void) {
    FUNC_CALL();


    memory[0] = 0;
    WRITE_VAR(var_mem0_init, 10);

    start(0, 0, 0);

    DUMP_TRACES();
    return 0;
}