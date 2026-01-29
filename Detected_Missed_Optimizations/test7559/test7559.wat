(module
  (import "External" "external_function" (func $external_function))
  (func $foo (param $0 i32) (param $1 i32) (result i32)
    i32.const 0)
  (func $_start (param $0 i32) (param $1 i64) (param $2 i32) (param $3 i32)
    i32.const 0
    i32.load
    local.set $0
    local.get $0
    local.set $2
    i32.const 0
    i32.const 0
    call $foo
    local.set $0
    block ;; label = @1
      local.get $0
      br_if 0 (;@1;)
      i32.const 0
      i32.const 0
      i32.store
      i32.const 0
      local.set $3
    end
    local.get $3
    local.set $0
    local.get $2
    local.get $0
    i32.and
    local.set $0
    block ;; label = @1
      local.get $0
      i32.eqz
      br_if 0 (;@1;)
      call $external_function
    end)
  (memory $0 258 258)
  (export "_start" (func $_start)))