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
int32_t var_loop_header;
int32_t var_branch_zero;
int32_t var_branch_nonzero;
int32_t var_after_store_v11;
int32_t var_after_store_v0;
int32_t var_cmp_ne_block3;
int32_t var_after_cmp_lt;
int32_t var_unreachable_hit;
int32_t var_store_loop;

int32_t start(void) {
    FUNC_CALL();

    int32_t v0 = 0;
    int32_t v1 = 0;
    int32_t v2 = 0;
    int32_t v3 = 0;
    int32_t v5 = 0;
    int32_t v6 = 0;
    int32_t v11 = 0;
    int32_t v7  = 0; 
    (void)v7;

    for (;;) {
        WRITE_VAR(var_loop_header, 1);

        int32_t m0 = memory[0];

        if (m0 == 0) {
            WRITE_VAR(var_branch_zero, 2);


            memory[0] = v11;
            WRITE_VAR(var_after_store_v11, 3);

            v5 = 0;

            v0 = 1;
            memory[0] = v0;
            WRITE_VAR(var_after_store_v0, 4);

            v6 = -346457217;


            if (v0 != v6) {
                WRITE_VAR(var_cmp_ne_block3, 5);
            } else {
                WRITE_VAR(var_cmp_ne_block3, 6);
                external_function();
            }


            (void)(v5 < memory[0]);
            WRITE_VAR(var_after_cmp_lt, 7);

            v1 = 0;
            (void)(v1 << v1);
            (void)v1;

            WRITE_VAR(var_unreachable_hit, 8);

            return 0;
        } else {
            WRITE_VAR(var_branch_nonzero, 9);


            v2 = m0;
            v3 = v2;
            memory[0] = v3;
            WRITE_VAR(var_store_loop, 10);


        }
    }
}

int main(void) {
    FUNC_CALL();


    memory[0] = 0;
    WRITE_VAR(var_mem0_init, 20);

    (void)start();

    DUMP_TRACES();
    return 0;
}