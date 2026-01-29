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
int32_t var_bar_called;
int32_t var_foo_called;
int32_t var_start_after_bar;
int32_t var_start_after_foo;
int32_t var_start_after_cmp;
int32_t var_start_after_block1;
int32_t var_start_load13;
int32_t var_start_store;
int32_t var_start_end_block2;
int32_t var_start_ret;

int32_t foo(void) {
    FUNC_CALL();


    WRITE_VAR(var_foo_called, 1); 
    return 0;
}

int32_t bar(void) {
    FUNC_CALL();


    WRITE_VAR(var_bar_called, 2); 
    return 0;
}

int32_t start(int32_t a0, int32_t a1) {
    FUNC_CALL();

    (void)a0;
    (void)a1;

    int32_t local2, local3, local6, local8, local9, local11;
    int32_t local13, local14 = 0, local16 = 0;


    local6 = bar();
    WRITE_VAR(var_start_after_bar, 3);


    local2 = 1;


    if (local6 == 0) {
        local8  = foo();
        WRITE_VAR(var_start_after_foo, 4);

        local9  = 0;
        local11 = (uint32_t)local8 <= (uint32_t)local9; 
        WRITE_VAR(var_start_after_cmp, 5);

        local3  = 0;

      
        if (local11 == 0) {
            external_function();
            local3 = 1;
        }

        (void)local3; 
        local2 = 0;
    }

    (void)local2; 
    WRITE_VAR(var_start_after_block1, 6);


    local13 = memory[0];
    WRITE_VAR(var_start_load13, 7);


    if (local13 == 0) {

        memory[0] = 0;
        local14   = 1;
        WRITE_VAR(var_start_store, 8);

        local16 = local14;
    }

    (void)local16;  
    WRITE_VAR(var_start_end_block2, 9);

 
    WRITE_VAR(var_start_ret, 10);
    return 0;
}

int main(void) {
    FUNC_CALL();

    
    memory[0] = 0;
    WRITE_VAR(var_mem0_init, 11);

    (void)start(0, 0);

    DUMP_TRACES();
    return 0;
}