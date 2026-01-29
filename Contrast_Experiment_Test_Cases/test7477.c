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
int32_t var_func_after_foo;
int32_t var_func_set_p2;
int32_t var_func_set_p1;
int32_t var_func_branch;
int32_t var_start_ret;

int32_t foo(void) {
    FUNC_CALL();

    int32_t v = memory[0];  


    WRITE_VAR(var_foo_called, 1);

    return v;
}

int32_t func(int32_t p1, int32_t p2) {
    FUNC_CALL();

    p2 = foo() & 1;
    WRITE_VAR(var_func_after_foo, 2);  
    WRITE_VAR(var_func_set_p2, 3);     


    // p1 = 0;
    p1 = 0;
    WRITE_VAR(var_func_set_p1, 4);

    
    
    

    if ((p2 | 1) != 0) {
        WRITE_VAR(var_func_branch, 5);
        return p1;
    } else {
        WRITE_VAR(var_func_branch, 6);
        external_function();
        return 0;
    }
}

int32_t start(void) {
    FUNC_CALL();

    int32_t ret = func(0, 0);
    WRITE_VAR(var_start_ret, 7);

    return ret;
}

int main(void) {
    FUNC_CALL();

    memory[0] = 0;
    WRITE_VAR(var_mem0_init, 10);

    (void)start();

    DUMP_TRACES();
    return 0;
}