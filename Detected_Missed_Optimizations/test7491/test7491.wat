(module
  (import "External" "external_function" (func $external_function))
  (func $_start (param $0 i32) (param $1 i32) (param $2 i32) (param $3 i32)
    (local $4 i64) (local $5 i64) (local $6 i64)
    call $foo
    local.set $6
    call $bar
    i64.extend_i32_s
    local.set $4
    local.get $6
    local.get $4
    i64.and
    local.set $4
    local.get $4
    i32.wrap_i64
    local.set $1
    block  ;; label = @1
      local.get $1
      i32.eqz
      br_if 0 (;@1;)
      call $external_function
    end)
  (func $bar (result i32)
    i32.const 0
    i32.const 0
    i32.store
    i32.const 0)
  (func $foo (result i64)
    i32.const 0
    i64.load)
  (memory $0 258 258)
  (export "_start" (func $_start)))