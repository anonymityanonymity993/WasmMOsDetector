(module
  (import "External" "external_function" (func $external_function))
  (func $_start (result i32)
    i32.const 0
    i32.const 0
    i32.const 0
    call $func_15
    drop
    unreachable)
  (func $func_15 (param $0 i32) (param $1 i32) (param $2 i32) (result i32)
    (local $3 i32)
    block  ;; label = @1
      i32.const 1
      i32.eqz
      br_if 0 (;@1;)
      i32.const 1
      local.set $3
    end
    local.get $3
    local.set $0
    i32.const 1667885170
    local.set $1
    local.get $0
    local.get $1
    i32.xor
    local.set $1
    i32.const 1
    local.set $0
    block  ;; label = @1
      local.get $1
      br_if 0 (;@1;)
      call $external_function
      i32.const 0
      local.set $0
    end
    i32.const 0
    i32.load
    drop
    i32.const 0
    local.get $0
    i32.store
    unreachable)
  (memory $0 258 258)
  (export "_start" (func $_start)))