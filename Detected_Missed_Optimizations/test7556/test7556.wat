(module
  (type $0 (func))
  (type $1 (func (param i32 i32) (result i32)))
  (type $2 (func (result i32)))
  (type $3 (func (param i32) (result i32)))
  (import "External" "external_function" (func $external_function (type $0)))
  (func $_start (type $3) (param $0 i32) (result i32)
    (local $1 i32) (local $2 i32)
    i32.const 0
    i32.load
    local.set $2
    local.get $1
    i32.const 0
    call $foo
    local.set $1
    local.get $1
    local.get $2
    i32.le_u
    if (result i32)
      i32.const 0
    else
      call $external_function
      i32.const 1
    end)
  (func $foo (type $1) (param $0 i32) (param $1 i32) (result i32)
    i32.const 0
    i32.const 0
    i32.store
    i32.const 0)
  (memory $0 258 258)
  (export "_start" (func $_start)))