(module
  (import "External" "external_function" (func $external_function))
  (func $foo (result i64)
    i32.const 0
    i64.load
    return)
  (func $_start (result i32)
    (local $3 i32) (local $4 i32) (local $8 i32) (local $12 i32) (local $13 i32) (local $15 i32) (local $5 i64) (local $6 i64)
    call $foo
    local.set $5
    i64.const -1
    local.set $6
    local.get $5
    local.get $6
    i64.gt_u
    local.set $8
    i32.const 0
    local.set $3
    block ;; label = @1
      local.get $8
      i32.eqz
      br_if 0 (;@1;)
      call $external_function
      i32.const 0
      local.set $3
    end
    local.get $3
    local.set $12
    i32.const 0
    i32.load
    local.set $13
    local.get $12
    local.get $13
    i32.le_s
    local.set $15
    i32.const 0
    local.set $4
    local.get $15
    local.get $4
    i32.shl
    drop
    local.get $4
    drop
    unreachable)
  (memory (;0;) 1)
  (export "_start" (func $_start)))