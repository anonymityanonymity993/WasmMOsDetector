(module
  (import "External" "external_function" (func $external_function))
  (func $foo (result i32)
    i32.const 0
    i32.load)
  (func $_start (param $0 i32) (param $1 i64) (param $2 i32) 
    (local $3 i32) (local $4 i32) (local $5 i32) (local $6 i32) (local $7 i32) (local $9 i32)
    call $foo
    local.set $4
    i32.const 1
    local.set $3
    local.get $4
    local.get $3
    i32.shl
    i32.const 1
    i32.shr_s
    local.set $6
    i32.const 0
    i32.const 0
    i32.store
    i32.const -259031342
    local.set $7
    local.get $6
    local.get $7
    i32.ne
    local.set $9
    block  ;; label = @1
      local.get $9
      i32.eqz
      br_if 0 (;@1;)
      unreachable
    end
    call $bar)
  (func $bar call $external_function)
  (memory $0 258 258)
  (export "_start" (func $_start)))