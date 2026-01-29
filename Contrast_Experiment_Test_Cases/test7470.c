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


uint8_t memory[77986u + 2u];


int32_t var_mem_init;
int32_t var_foo_called;
int32_t var_start_after_foo;

static uint32_t load16_u_at_77986(void) {
    uint32_t addr = 77986u;
    uint32_t b0 = memory[addr];
    uint32_t b1 = memory[addr + 1u];
    return b0 | (b1 << 8);
}

int32_t foo(int32_t x) {
    FUNC_CALL();


    (void)x;


    int32_t local1   = 0;
    int32_t local2   = 0;  
    int32_t local4   = 0;
    int32_t local5   = 0;
    int32_t scratch  = 0;
    int64_t local3   = 0;
    int64_t local6   = 0;

    (void)local1;
    (void)local4;
    (void)local5;
    (void)scratch;
    (void)local3;
    (void)local6;


    WRITE_VAR(var_foo_called, 1);



    uint32_t v = load16_u_at_77986();

    local2 = 16;
    int32_t t1 = (int32_t)(v << local2); 
    int32_t t2 = t1 >> local2;     

    int32_t t3 = (t2 == 65533) ? 1 : 0;  



    uint32_t v2 = load16_u_at_77986();
    int32_t t4  = (int32_t)(v2 << 16);  
    int32_t cond = t3 & t4;           

    if (cond != 0) {
        external_function();
        return 0;
    } else {
        return 1;
    }
}

void start(int64_t a0, int32_t a1, int32_t a2, int32_t a3) {
    FUNC_CALL();


    (void)a0;
    (void)a1;
    (void)a2;
    (void)a3;


    int32_t local4 = 0;
    int32_t local5 = 0;
    (void)local4;
    (void)local5;

    // wasm:
    //   i32.const 0
    //   call $foo
    //   drop
    int32_t ret = foo(0);
    (void)ret;

    WRITE_VAR(var_start_after_foo, 2);
}

int main(void) {
    FUNC_CALL();


    memory[0] = 0;
    WRITE_VAR(var_mem_init, 10);

    start(0, 0, 0, 0);

    DUMP_TRACES();
    return 0;
}