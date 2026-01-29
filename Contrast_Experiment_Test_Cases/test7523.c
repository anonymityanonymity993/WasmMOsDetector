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


static uint8_t memory[8];


static int32_t var_safe_mul_entry;
static int32_t var_safe_mul_after_if;
static int32_t var_func49_entry;
static int32_t var_func49_after_cond;
static int32_t var_start_entry;
static int32_t var_start_after_first_call;
static int32_t var_start_after_safe_mul;
static int32_t var_start_after_second_call;
static int32_t var_main_entry;

static int32_t safe_mul_func_int32_t_s_s(void) {
    FUNC_CALL();
    WRITE_VAR(var_safe_mul_entry, 1);

    /* i32.const 0; i32.const 0; i32.store */
    *(int32_t *)(void *)(memory + 0) = 0;

    /* i32.const 0; i32.load */
    int32_t v0 = *(int32_t *)(void *)(memory + 0);

    if (v0 != 0) {

        int32_t tmp1 = *(int32_t *)(void *)(memory + 1);
        (void)tmp1;

        uint32_t div_res = 0u / 0u; 
        (void)div_res;
    }


    WRITE_VAR(var_safe_mul_after_if, 2);

    int32_t v2 = *(int32_t *)(void *)(memory + 0);
    (void)v2;

    int32_t v3 = *(int32_t *)(void *)(memory + 1);
    (void)v3;


    for (;;)
        ;
}

static int32_t func_49(int32_t p0, int32_t p1, int32_t p2, int32_t p3) {
    FUNC_CALL();
    WRITE_VAR(var_func49_entry, 3);

    int32_t local4 = 0;
    int32_t local5 = 0;
    int32_t local6 = 0;
    int32_t local7 = 0;
    int32_t local8 = 0;
    int32_t local9 = 0;
    int32_t local10 = 0;
    int32_t local11 = 0;


    *(int32_t *)(void *)(memory + 0) = p0;
    *(int32_t *)(void *)(memory + 0) = p1;
    *(uint16_t *)(void *)(memory + 0) = (uint16_t)p2;
    *(int32_t *)(void *)(memory + 0) = p3;

    local5 = safe_mul_func_int32_t_s_s();

    local6 = 0;

    int32_t t_eq = (local5 == local6);         /* i32.eq -> 0 或 1 */
    int32_t t_lt = (t_eq < 0) ? 1 : 0;         /* i32.lt_s 与 0 比较 */
    local9 = t_lt;

    local4 = 0;


    WRITE_VAR(var_func49_after_cond, 4);

    /* block: if (local9 != 0) { call external_function; local4 = 0; } */
    if (local9 != 0) {
        external_function();
        local4 = 0;
    }

    local11 = local4;
    *(int32_t *)(void *)(memory + 0) = local11;


    for (;;)
        ;
}

static void start(void) {
    FUNC_CALL();
    WRITE_VAR(var_start_entry, 5);

    int32_t local0 = 0;
    int32_t local1 = 0;


    local0 = func_49(0, 1, 0, 0);

    WRITE_VAR(var_start_after_first_call, 6);


    (void)safe_mul_func_int32_t_s_s();

    WRITE_VAR(var_start_after_safe_mul, 7);

    (void)func_49(1, 0, 1, 1);

    WRITE_VAR(var_start_after_second_call, 8);


    *(int32_t *)(void *)(memory + 0) = local0;
}

int main(void) {
    FUNC_CALL();
    WRITE_VAR(var_main_entry, 9);

    start();

    DUMP_TRACES();
    return 0;
}