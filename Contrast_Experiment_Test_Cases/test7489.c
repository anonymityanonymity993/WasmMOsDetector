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
int32_t var_mem1_init;
int32_t var_foo_step1, var_foo_step2, var_foo_step3;
int32_t var_foo_step4, var_foo_step5, var_foo_step6;
int32_t var_start_after;

void foo(int32_t p0, int32_t p2) {
    FUNC_CALL();

    int32_t tmp;


    tmp = memory[0];
    (void)tmp;
    WRITE_VAR(var_foo_step1, 1);


    p2 = memory[1];
    WRITE_VAR(var_foo_step2, 2);


    p0 = 1;
    WRITE_VAR(var_foo_step3, 3);

    /* if (p2 != 0) break block; */
    if (p2 != 0) {

    } else {
        /* p0 = 1; */
        p0 = 1;
        WRITE_VAR(var_foo_step4, 4);


        if (p0 != 0) {

        } else {

            p0 = 0;
            WRITE_VAR(var_foo_step5, 5);
        }
    }


    if (p0 == 0) {
        WRITE_VAR(var_foo_step6, 6);
        external_function();
    } else {
    
        WRITE_VAR(var_foo_step6, 7);
    }
}

void start(void) {
    FUNC_CALL();

    foo(0, 0);
    WRITE_VAR(var_start_after, 8);
}

int main(void) {
    FUNC_CALL();


    memory[0] = 0;
    WRITE_VAR(var_mem0_init, 10);

    memory[1] = 0;
    WRITE_VAR(var_mem1_init, 11);

    start();

    DUMP_TRACES();
    return 0;
}