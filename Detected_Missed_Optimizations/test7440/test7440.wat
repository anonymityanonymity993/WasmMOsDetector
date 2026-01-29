(module
  (type $0 (func))
  (type $1 (func (param i32 i32) (result i32)))
  (import "External" "external_function" (func $external_function (type $0)))
  (func $_start (type $1) (param $0 i32) (param $1 i32) (result i32)
    (local $2 i32) (local $4 i32) (local $7 i32) (local $8 i32) (local $11 i32) (local $13 i32)
    global.get $__stack_pointer
    local.set $2
    local.get $2
    i64.const 0
    i64.store offset=1752
    i32.const 4058
    local.set $4
    local.get $2
    local.get $4
    i32.add
    i32.const 1
    i32.add
    local.set $7
    i32.const 4058
    local.set $8
    local.get $2
    local.get $8
    i32.add
    i32.const 1
    i32.add
    local.set $11
    local.get $7
    local.get $11
    i32.ne
    local.set $13
    block  ;; label = @1
      local.get $13
      i32.eqz
      br_if 0 (;@1;)
      call $external_function
    end
    unreachable)
  (memory $0 259 259)
  (global $__stack_pointer (mut i32) (i32.const 0))
  (export "_start" (func $_start)))
