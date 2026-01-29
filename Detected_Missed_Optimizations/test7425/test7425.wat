(module
  (import "External" "external_function" (func $external_function))
  (func $_start (result i32)
    (local $0 i32) (local $1 i32) (local $2 i32) (local $3 i32)
    call $foo
    local.set $1
    i32.const 1
    local.set $2
    local.get $1
    local.get $2
    i32.and
    i32.const 0
    i32.ge_s
    local.set $3
    i32.const 0
    local.set $0
    block ;; label = @1
      local.get $3
      br_if 0 (;@1;)
      call $external_function
      i32.const 0
      local.set $0
    end
    local.get $0)
  (func $foo (result i32)
    i32.const 0
    i32.load
    return)
  (memory 1)
  (export "_start" (func $_start)))