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
int32_t var_start_after_load;
int32_t var_start_after_copy;
int32_t var_start_after_foo;
int32_t var_start_after_block;
int32_t var_start_after_and;
int32_t var_start_branch2;

int32_t foo(int32_t p0, int32_t p1) {
    FUNC_CALL();

    (void)p0;
    (void)p1;

    // 对应 wasm: i32.const 0; return 0;
    WRITE_VAR(var_foo_called, 1);  // 仅记录 foo 被调用
    return 0;
}

void start(int32_t a0, int64_t a1, int32_t a2, int32_t a3) {
    FUNC_CALL();

    (void)a1;  // wasm 中未使用

    /* a0 = memory[0];  对应 i32.const 0; i32.load; local.set $0 */
    a0 = memory[0];
    WRITE_VAR(var_start_after_load, 2);

    /* a2 = a0;  对应 local.get $0; local.set $2 */
    a2 = a0;
    WRITE_VAR(var_start_after_copy, 3);

    /* a0 = foo(0, 0);  对应 i32.const 0; i32.const 0; call $foo; local.set $0 */
    a0 = foo(0, 0);
    WRITE_VAR(var_start_after_foo, 4);

    /* 第一个 block：if (a0 == 0) { memory[0] = 0; a3 = 0; } */
    if (a0 == 0) {
        memory[0] = 0;
        a3 = 0;
    }
    WRITE_VAR(var_start_after_block, 5);

    /* a0 = a3;  对应 local.get $3; local.set $0 */
    a0 = a3;

    /* a0 = a2 & a0;  对应 local.get $2; local.get $0; i32.and; local.set $0 */
    a0 = a2 & a0;
    WRITE_VAR(var_start_after_and, 6);

    /* 第二个 block：只有 a0 != 0 时才调用 external_function */
    if (a0 != 0) {
        WRITE_VAR(var_start_branch2, 7);
        external_function();
    } else {
        WRITE_VAR(var_start_branch2, 8);
    }
}

int main(void) {
    FUNC_CALL();

    // 初始化 memory[0]，并用常数记录一次写入，防止被优化掉
    memory[0] = 0;
    WRITE_VAR(var_mem0_init, 10);

    start(0, 0, 0, 0);

    DUMP_TRACES();
    return 0;
}