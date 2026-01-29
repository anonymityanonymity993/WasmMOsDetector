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
int32_t var_start_after_gt_u;
int32_t var_start_block_path;
int32_t var_start_after_load;
int32_t var_start_after_cmp;
int32_t var_start_before_shl;
int32_t var_start_ret;

int64_t foo(void) {
    FUNC_CALL();


    int64_t v = *(int64_t *)(void *)&memory[0];


    WRITE_VAR(var_foo_called, 1);

    return v;
}

int32_t start(void) {
    FUNC_CALL();

    int32_t v3, v4, v8, v12, v13, v15;
    int64_t v5, v6;


    v5 = foo();
    WRITE_VAR(var_start_after_foo, 2);


    v6 = -1;


    v8 = ((uint64_t)v5 > (uint64_t)v6);
    WRITE_VAR(var_start_after_gt_u, 3);

    v3 = 0;


    if (v8 != 0) {
        WRITE_VAR(var_start_block_path, 4);
        external_function();
        v3 = 0;
    } else {
        WRITE_VAR(var_start_block_path, 5);
    }

    v12 = v3;

    /* v13 = memory[0]; */
    v13 = memory[0];
    WRITE_VAR(var_start_after_load, 6);


    v15 = (v12 <= v13);
    WRITE_VAR(var_start_after_cmp, 7);

    /* v4 = 0; */
    v4 = 0;
    WRITE_VAR(var_start_before_shl, 8);


    (void)(v15 << v4);
    (void)v4;


    WRITE_VAR(var_start_ret, 9);
    return 0;
}

int main(void) {
    FUNC_CALL();


    memory[0] = 0;
    WRITE_VAR(var_mem0_init, 10);

    (void)start();

    DUMP_TRACES();
    return 0;
}