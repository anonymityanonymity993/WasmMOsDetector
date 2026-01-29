(module
  (type $5 (func))
  (type $9 (func (param i32 i32 i32 i32) (result i32)))
  (type $2 (func (result i32)))
  (import "External" "external_function" (func $external_function (type $5)))
  (func $func_49 (type $9) (param $0 i32) (param $1 i32) (param $2 i32) (param $3 i32) (result i32)
    (local $4 i32) (local $5 i32) (local $6 i32) (local $7 i32) (local $8 i32) (local $9 i32) (local $10 i32) (local $11 i32)
    i32.const 0
    local.get $0
    i32.store
    i32.const 0
    local.get $1
    i32.store
    i32.const 0
    local.get $2
    i32.store16
    i32.const 0
    local.get $3
    i32.store
    call $safe_mul_func_int32_t_s_s
    local.set $5
    i32.const 0
    local.set $6
    local.get $5
    local.get $6
    i32.eq
    i32.const 0
    i32.lt_s
    local.set $9
    i32.const 0
    local.set $4
    block  ;; label = @1
      local.get $9
      i32.eqz
      br_if 0 (;@1;)
      call $external_function
      i32.const 0
      local.set $4
    end
    local.get $4
    local.set $11
    i32.const 0
    local.get $11
    i32.store
    unreachable)
  (func $safe_mul_func_int32_t_s_s (type $2) (result i32)
    i32.const 0
    i32.const 0
    i32.store
    i32.const 0
    i32.load
    if  ;; label = @1
      i32.const 1
      i32.load
      drop
      i32.const 0
      i32.const 0
      i32.div_u
      drop
    end
    i32.const 0
    i32.load
    drop
    i32.const 1
    i32.load
    drop
    unreachable)
  (func $_start (type $5)
    (local $0 i32) (local $1 i32)
    i32.const 0
    i32.const 1
    i32.const 0
    i32.const 0
    call $func_49
    local.set $0
    call $safe_mul_func_int32_t_s_s
    drop
    i32.const 1
    i32.const 0
    i32.const 1
    i32.const 1
    call $func_49
    drop
    i32.const 0
    local.get $0
    i32.store)
  (memory $0 258 258)
  (export "_start" (func $_start)))