(module
  (import "External" "external_function" (func $external_function))
  (func $_start (param $0 i32) (param $1 i32) (param $2 i32) (param $3 i32) (result i32)
    (local $4 i32) (local $5 i32) (local $6 i32) (local $9 i32) (local $10 i32) (local $11 i32) (local $12 i32) (local $13 i32) (local $14 i32) (local $15 i32) (local $16 i32) (local $7 i32) (local $8 i32)
    i32.const 0
    i32.load
    local.set $7
    i32.const 0
    call $foo
    local.set $8
    local.get $7
    local.get $8
    i32.le_u
    local.set $9
    i32.const 1
    local.set $10
    local.get $9
    local.get $10
    i32.and
    local.set $11
    i32.const 4
    local.set $12
    local.get $11
    local.get $12
    i32.and
    local.set $13
    i32.const 0
    local.set $4
    block  ;; label = @1
      local.get $13
      i32.eqz
      br_if 0 (;@1;)
      call $external_function
      i32.const 0
      local.set $4
    end
    local.get $4
    call $bar)
  (func $foo (param $0 i32) (result i32) i32.const 0)
  (func $bar (param $0 i32) (result i32) i32.const 1)
  (memory $0 258 258)
  (export "_start" (func $_start)))